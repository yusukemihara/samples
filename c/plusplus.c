#include <stdio.h>

int
main(int argc,char *argv[])
{
	int a,b;

	a = 0;
	b = 1 + a++;
	fprintf(stderr,"b[%d]\n",b);
	return 1;
}
