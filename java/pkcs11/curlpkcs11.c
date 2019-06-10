#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libcurl libssl libp11` -o $obj $src
exit
#endif

/* perform HTTPS GET on ssl client verification using curl,OpenSSL ENGINE pkcs11 and libp11
 * 
 * usage:
 *   % ./curlpkcs11.c # <= compile
 *   % ./curlpkcs11 /usr/lib/your_devices_pkcs11.so 1234(pin) https://example.com/
 *
 * the token must has ca certificate in store.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <curl/curl.h>
#include <libp11.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pkcs12.h>
#include <openssl/engine.h>

#define ENGINE_PKCS11_PATH "/usr/lib/engines/engine_pkcs11.so"

static ENGINE *e = NULL;
static CURL *curl = NULL;
static char *certid = NULL;
static char *cacertfile = NULL;

size_t write_data(
	void *buf,
	size_t size,
	size_t nmemb,
	void *userp)
{
	char *out;
	
	out = malloc(size*nmemb+1);
	memcpy(out,buf,size*nmemb);
	out[size*nmemb] = 0;
	fprintf(stderr,"%s",out);
	return size*nmemb;
}

static void
get_certid_and_cacert(
	const char *p11lib,
	const char *pin)
{
	int i,rc;
	unsigned int nslots,nslot,ncerts;
	char part[3],*id,*p;
	unsigned char *pp;
	PKCS11_CTX *p11ctx;
	PKCS11_SLOT *slots, *slot;
	PKCS11_CERT *certs,*cert;
	BIO *out;
	size_t size;

	nslot = 0;

	p11ctx = PKCS11_CTX_new();

	/* load pkcs #11 module */
	rc = PKCS11_CTX_load(p11ctx,p11lib);
	if (rc) {
		fprintf(stderr,"PKCS11_CTX_load\n");
		exit(-1);
	}

	/* get information on all slots */
	rc = PKCS11_enumerate_slots(p11ctx, &slots, &nslots);
	if (rc < 0) {
		fprintf(stderr,"PKCS11_enumerate_slots\n");
		exit(-1);
	}

	/* get first slot with a token */
	slot = PKCS11_find_token(p11ctx, slots, nslots);
	if (!slot || !slot->token) {
		fprintf(stderr,"PKCS11_find_token\n");
		exit(-1);
	}
	for(i=0;i<nslots;i++) {
		if (&slots[i] == slot) {
			nslot = i + 1;
		}
	}

	fprintf(stderr,"Slot manufacturer......: %s\n", slot->manufacturer);
	fprintf(stderr,"Slot description.......: %s\n", slot->description);
	fprintf(stderr,"Slot token label.......: %s\n", slot->token->label);
	fprintf(stderr,"Slot token manufacturer: %s\n", slot->token->manufacturer);
	fprintf(stderr,"Slot token model.......: %s\n", slot->token->model);
	fprintf(stderr,"Slot token serialnr....: %s\n", slot->token->serialnr);

	id = getenv("PKCS11_CERTID");
	if (id == NULL) {
		id = "cert";
	}

	size = strlen("slot_9999-id_") + strlen(id) * 2 + 1;
	certid = malloc(size);
	memset(certid,0,size);
	snprintf(certid,size-1,"slot_%d-id_",nslot);
	for(i=0,p=id;i<strlen(id);i++) {
		snprintf(part,sizeof(part),"%02x",*(p+i));
		strcat(certid,part);
	}
	fprintf(stderr,"%s\n",certid);

	/* get all certs */
	rc = PKCS11_enumerate_certs(slot->token, &certs, &ncerts);
	if (rc) {
		fprintf(stderr,"PKCS11_enumerate_certs\n");
		exit(-1);
	}
	fprintf(stderr,"ncerts:%d\n",ncerts);

	cacertfile = strdup("/tmp/curlpkcs11cacerts");
	out = BIO_new_file(cacertfile,"w");
	for(i=0;i<ncerts;i++) {
		cert=(PKCS11_CERT*)&certs[i];
		PEM_write_bio_X509(out,cert->x509);
	}
	BIO_free(out);

	PKCS11_release_all_slots(p11ctx, slots, nslots);
	PKCS11_CTX_unload(p11ctx);
	PKCS11_CTX_free(p11ctx);
}

static void
init(
	const char *p11lib,
	const char *pin)
{
	get_certid_and_cacert(p11lib,pin);

	ENGINE_load_dynamic();
	ENGINE_load_builtin_engines();
	e = ENGINE_by_id("dynamic");
	if (!e) {
		fprintf(stderr,"ENIGNE_by_id failure\n");
		exit(-1);
	}
	if(!ENGINE_ctrl_cmd_string(e, "SO_PATH", ENGINE_PKCS11_PATH, 0)||
	   !ENGINE_ctrl_cmd_string(e, "ID", "pkcs11", 0) ||
	   !ENGINE_ctrl_cmd_string(e, "LIST_ADD", "1", 0) ||
	   !ENGINE_ctrl_cmd_string(e, "LOAD", NULL, 0) ||
	   !ENGINE_ctrl_cmd_string(e, "MODULE_PATH", p11lib, 0) ||
	   !ENGINE_ctrl_cmd_string(e, "PIN", pin, 0) ) {
			fprintf(stderr,"ENGINE_ctrl_cmd_string failure\n");
			exit(-1);
	}
	if (!ENGINE_init(e)) {
		fprintf(stderr,"ENGINE_init failure\n");
		exit(-1);
	}

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr,"couldn't init curl\n");
		exit(-1);
	}
	curl_easy_setopt(curl,CURLOPT_SSLCERT,certid);
	curl_easy_setopt(curl,CURLOPT_SSLKEY,certid);
	free(certid);
	curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
	curl_easy_setopt(curl,CURLOPT_USE_SSL,CURLUSESSL_ALL);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER,1);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST,2);
	curl_easy_setopt(curl,CURLOPT_SSLENGINE,"pkcs11");
	curl_easy_setopt(curl,CURLOPT_SSLKEYTYPE,"ENG");
	curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"ENG");
	curl_easy_setopt(curl,CURLOPT_SSLENGINE_DEFAULT,1L);
	curl_easy_setopt(curl,CURLOPT_CAINFO,cacertfile);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,NULL);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_data);
}

static void
final()
{
	if (curl != NULL) {
		curl_easy_cleanup(curl);
		curl_global_cleanup();
	}
	if (e != NULL) {
		ENGINE_finish(e);
	}
	if (certid != NULL) {
		free(certid);
	}
	if (cacertfile != NULL) {
		free(cacertfile);
	}
	ENGINE_cleanup();
}

void
get(
	const char *url)
{
	CURLcode code;

	code = CURLE_COULDNT_CONNECT;
	curl_easy_setopt(curl, CURLOPT_URL, url);
	code = curl_easy_perform(curl);
	fprintf(stderr,"url[%s] code[%d]\n",url,code);
}

int 
main(int argc,char *argv[])
{
	int i;
	if (argc < 4) {
		fprintf(stderr,"./curlpkcs11 pkcs11.so pin url [url2...]\n");
		return 1;
	}

	init(argv[1],argv[2]);
	for (i = 3; i < argc; i++) {
		fprintf(stderr,"%s\n",argv[i]);
		get(argv[i]);
	}
	final();
	return 0;
}
