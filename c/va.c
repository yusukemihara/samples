#include <stdio.h>
#include <stdarg.h>

void
foo(
	char *moge,
	...)
{
	va_list ap;
	char *p;

	va_start(ap,NULL);
	while((p=va_arg(ap,char*))) {
		fprintf(stderr,"[%s]\n",p);
	}
	va_end(ap);
}

int
main(int argc,char *argv[])
{
	foo("moge","rie","fu","get","subekika",NULL);
	foo("moge","atari","kosuke",NULL);
	return 0;
}
