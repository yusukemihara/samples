#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>


int main(void)
{
	char a[10],b[10];
	char *c = NULL;

	fprintf(stderr,"%d\n",strcmp("ginbee",c));

	fprintf(stderr,"%d\n", strcmp("1.4.4.00", "1.4.4.00"));
	fprintf(stderr,"%d\n", strcmp("1.4.4.00", "1.4.4.01"));
	fprintf(stderr,"%d\n", strcmp("1.4.4.00", "1.4.3.00"));
	fprintf(stderr,"%d\n", strcmp("1.4.4.00", "1.4.3.0"));
	fprintf(stderr,"%d\n", strcmp("1.4.4.00", "1.4.3.000"));
    return 0;
}
