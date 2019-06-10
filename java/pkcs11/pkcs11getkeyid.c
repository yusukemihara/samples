#if 0
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libp11 libssl` $0
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

#define PKCS11_BUF_SIZE (1024)

void getid()
{
	char certid[PKCS11_BUF_SIZE];
	char certidbuf[PKCS11_BUF_SIZE];

	int rc = 0;
	int i,j;
	unsigned int nslots,ncerts;
	int nslot = 0;

	PKCS11_CTX *p11ctx;
	PKCS11_SLOT *slots, *slot;
	PKCS11_CERT *certs,*cert;

	p11ctx = PKCS11_CTX_new();

	/* load pkcs #11 module */
	rc = PKCS11_CTX_load(p11ctx,"./libcastle.so.1.0.0");
	if (rc) {
		fprintf(stderr,"PKCS11_CTX_load\n");
		return;
	}

	/* get information on all slots */
	rc = PKCS11_enumerate_slots(p11ctx, &slots, &nslots);
	if (rc < 0) {
		fprintf(stderr,"PKCS11_enumerate_slots\n");
		return;
	}

	/* get first slot with a token */
	slot = PKCS11_find_token(p11ctx, slots, nslots);
	if (!slot || !slot->token) {
		fprintf(stderr,"PKCS11_find_token\n");
		return;
	}
	for(i=0;i<nslots;i++) {
		if (&slots[i] == slot) {
			nslot = i;
		}
	}

	printf("Slot manufacturer......: %s\n", slot->manufacturer);
	printf("Slot description.......: %s\n", slot->description);
	printf("Slot token label.......: %s\n", slot->token->label);
	printf("Slot token manufacturer: %s\n", slot->token->manufacturer);
	printf("Slot token model.......: %s\n", slot->token->model);
	printf("Slot token serialnr....: %s\n", slot->token->serialnr);

	/* perform pkcs #11 login */
	rc = PKCS11_login(slot, 0, "12345678");
	if (rc != 0) {
		fprintf(stderr,"PKCS11_login\n");
		return;
	}

	/* get all certs */
	rc = PKCS11_enumerate_certs(slot->token, &certs, &ncerts);
	if (rc) {
		fprintf(stderr,"PKCS11_enumerate_certs\n");
		return;
	}
	if (ncerts <= 0) {
		fprintf(stderr,"no certs\n");
		return;
	}

	for(j=0;j<ncerts;j++) {
		cert=(PKCS11_CERT*)&certs[j];

		sprintf(certid,"slot_%d-id_",nslot);
		for(i=0;i<cert->id_len;i++) {
			sprintf(certidbuf,"%02x",(unsigned int)(cert->id[i]));
			strcat(certid,certidbuf);
		}
		printf("id:[%s] label:%s [%p]\n",certid,cert->label,cert->x509);
	}

	PKCS11_logout(slot);
	PKCS11_release_all_slots(p11ctx, slots, nslots);
	PKCS11_CTX_unload(p11ctx);
	PKCS11_CTX_free(p11ctx);
}

int 
main(int argc,char *argv[])
{
	getid();
	return 0;
}
