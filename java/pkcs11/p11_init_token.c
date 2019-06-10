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
p11_init_token(
	const char *p11lib,
	const char *sopin,
	const char *pin,
	const char *label)
{
	int i,rc = 0;
	unsigned int nslots;

	PKCS11_CTX *p11ctx;
	PKCS11_SLOT *slots, *slot;

	p11ctx = PKCS11_CTX_new();

	/* load pkcs #11 module */
	rc = PKCS11_CTX_load(p11ctx,p11lib);
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

	rc = PKCS11_init_token(slot->token,sopin,label);
	if (rc != 0) {
		ERR_load_PKCS11_strings();
		fprintf(stderr,"PKCS11_init_token %s\n",
			ERR_reason_error_string(ERR_get_error()));
		return -1;
	}

	/* perform pkcs #11 SO login */
	rc = PKCS11_login(slot, 1, sopin);
	if (rc != 0) {
		ERR_load_PKCS11_strings();
		fprintf(stderr,"PKCS11_init_login %s\n",
			ERR_reason_error_string(ERR_get_error()));
		return -1;
	}

	rc = PKCS11_init_pin(slot->token,pin);
	if (rc != 0) {
		ERR_load_PKCS11_strings();
		fprintf(stderr,"PKCS11_init_pin %s\n",
			ERR_reason_error_string(ERR_get_error()));
		return -1;
	}

	PKCS11_logout(slot);
	PKCS11_release_all_slots(p11ctx, slots, nslots);
	PKCS11_CTX_unload(p11ctx);
	PKCS11_CTX_free(p11ctx);

	fprintf(stderr,"\n\ninit token succeed\n");
	return 0;
}

int 
main(int argc,char *argv[])
{
	if (argc < 5) {
		fprintf(stderr,"p11_init_token pkcs11.so sopin pin(new) label\n");
		return -1;
	}
	return p11_init_token(argv[1],argv[2],argv[3],argv[4]);
}
