#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libssl libp11` -o $obj $src
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libp11.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#if 0
#define P11LIB "./libcastle.so.1.0.0"
#define SOPIN "entersafe"
#define PIN "12345678"
#endif

static int
p11_store_cert(
	const char *libp11,
	const char *pin,
	const char *certid,
	const char *certfile)
{
	int i,rc;
	unsigned int nslots;
	size_t size;
	PKCS11_CTX *p11ctx;
	PKCS11_SLOT *slots, *slot;
	PKCS11_CERT *cert;
	X509 *x509;
	BIO *bio;

	p11ctx = PKCS11_CTX_new();

	/* load pkcs #11 module */
	rc = PKCS11_CTX_load(p11ctx,libp11);
	if (rc) {
		fprintf(stderr,"PKCS11_CTX_load\n");
		return -1;
	}

	/* get information on all slots */
	rc = PKCS11_enumerate_slots(p11ctx, &slots, &nslots);
	if (rc < 0) {
		fprintf(stderr,"PKCS11_enumerate_slots\n");
		return -1;
	}

	/* get first slot with a token */
	slot = PKCS11_find_token(p11ctx, slots, nslots);
	if (!slot || !slot->token) {
		fprintf(stderr,"PKCS11_find_token\n");
		return -1;
	}

	fprintf(stderr,"Slot manufacturer......: %s\n", slot->manufacturer);
	fprintf(stderr,"Slot description.......: %s\n", slot->description);
	fprintf(stderr,"Slot token label.......: %s\n", slot->token->label);
	fprintf(stderr,"Slot token manufacturer: %s\n", slot->token->manufacturer);
	fprintf(stderr,"Slot token model.......: %s\n", slot->token->model);
	fprintf(stderr,"Slot token serialnr....: %s\n", slot->token->serialnr);

	rc = PKCS11_open_session(slot, 1);
	if (rc != 0) {
		ERR_load_PKCS11_strings();
		fprintf(stderr,"PKCS11_open_session %s\n",
			ERR_reason_error_string(ERR_get_error()));
		return -1;
	}

	rc = PKCS11_login(slot, 0, pin);
	if (rc != 0) {
		ERR_load_PKCS11_strings();
		fprintf(stderr,"PKCS11_login %s\n",
			ERR_reason_error_string(ERR_get_error()));
		return -1;
	}

	/* load cert */
	if ((bio = BIO_new(BIO_s_file())) == NULL)
	{
		fprintf(stderr,"BIO_new\n");
		return -1;
	}
	if (BIO_read_filename(bio,certfile) <= 0) {
		fprintf(stderr,"BIO_read_filename\n");
		return -1;
	}
	x509 = PEM_read_bio_X509_AUX(bio,NULL, NULL, NULL);
	if (x509 == NULL) {
		fprintf(stderr,"PKCS11_enumerate_certs\n");
		return -1;
	}

	/* store cert */
	rc = PKCS11_store_certificate(slot->token,x509,
			(char*)certid,(unsigned char*)certid,strlen(certid),&cert);
	if (rc != 0) {
		ERR_load_PKCS11_strings();
		fprintf(stderr,"PKCS11_store_certificate %s rc:%d\n",
			ERR_reason_error_string(ERR_get_error()),rc);
		return -1;
	}

	X509_free(x509);
	BIO_free(bio);

	PKCS11_logout(slot);
	PKCS11_release_all_slots(p11ctx, slots, nslots);
	PKCS11_CTX_unload(p11ctx);
	PKCS11_CTX_free(p11ctx);

	fprintf(stderr,"\n\nstore cert succeed\n");

	return 0;
}

int 
main(int argc,char *argv[])
{
	if (argc < 4) {
		fprintf(stderr,"./p11_store_cert pkcs11.so pin certid cert.pem\n");
		return 1;
	}
	return p11_store_cert(argv[1],argv[2],argv[3],argv[4]);
}
