#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>

#ifdef __LP64__
typedef long		Obj;
#else
typedef uint64_t	Obj;
#endif

uint64_t
htonll(
	uint64_t value)
{
	return ((uint64_t)htonl(value>>32)) | ((uint64_t)htonl(value)<<32);
}

uint64_t
ntohll(
	uint64_t value)
{
	return ((uint64_t)ntohl(value>>32)) | ((uint64_t)ntohl(value)<<32);
}

int
main(int argc,char *argv[])
{
	unsigned char *p;
	Obj o;
	int i;

	o = 65530;
	fprintf(stderr,"o[%ld] size[%ld]\n",(long)o,sizeof(Obj));
	p = (unsigned char*)&o;
	for(i=0;i<sizeof(Obj);i++) {
		fprintf(stderr,"%02x ",(unsigned char)*(p+i));
	}
	fprintf(stderr,"\n");

	o = htonll(o);
	fprintf(stderr,"htonll ");
	for(i=0;i<sizeof(Obj);i++) {
		fprintf(stderr,"%02x ",(unsigned char)*(p+i));
	}
	fprintf(stderr,"\n");

	o = ntohll(o);
	fprintf(stderr,"ntohll ");
	for(i=0;i<sizeof(Obj);i++) {
		fprintf(stderr,"%02x ",(unsigned char)*(p+i));
	}
	fprintf(stderr,"\n");


	return 0;
}
