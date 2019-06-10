#if 0
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libssl libp11` $0
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
#include <openssl/asn1.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define P11LIB "./libcastle.so.1.0.0"
#define SOPIN "entersafe"
#define PIN "12345678"

static void
make_csr()
{
	EVP_PKEY *pk;
	X509_REQ *req;
	X509_NAME *name;
	RSA *rsa;
	BIO *err;

	err = BIO_new_fp(stderr, BIO_NOCLOSE);
	rsa = RSA_generate_key(2048,0x10001,NULL,err);
	if (rsa == NULL) {
		fprintf(stderr,"RSA_generate_key failure\n");
		return;
	}
	pk = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(pk,rsa); 

	req = X509_REQ_new();
	if (!X509_REQ_set_version(req,0L)) {
		/* version 1 */
		fprintf(stderr,"X509_REQ_set_version failure\n");
		return;
	}

	if (!X509_REQ_set_pubkey(req,pk)) {
		fprintf(stderr,"X509_REQ_set_pubkey failure\n");
		return;
	}

	name = X509_NAME_new();
	if (name == NULL) {
		fprintf(stderr,"X509_NAME_new failure\n");
		return;
	}

	if (!X509_NAME_add_entry_by_txt(name,"C",MBSTRING_ASC,"JP",-1,-1,0)) {
		fprintf(stderr,"X509_NAME_add_entry_by_txt failure\n");
		return;
	}

	if (!X509_NAME_add_entry_by_txt(name,"O",MBSTRING_ASC,"test",-1,-1,0)) {
		fprintf(stderr,"X509_NAME_add_entry_by_txt failure\n");
		return;
	}

	if (!X509_NAME_add_entry_by_txt(name,"CN",MBSTRING_ASC,"test",-1,-1,0)) {
		fprintf(stderr,"X509_NAME_add_entry_by_txt failure\n");
		return;
	}

	if (!X509_REQ_set_subject_name(req, name)) {
		fprintf(stderr,"X509_REQ_set_subject_name failure\n");
		return;
	}
	X509_NAME_free(name);

	PEM_write_bio_X509_REQ(err,req);

	EVP_PKEY_free(pk);
	X509_REQ_free(req);
	BIO_free(err);
}

int 
main(int argc,char *argv[])
{
	make_csr();
	return 0;
}
