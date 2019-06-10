#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>


int main(void)
{
	char a[10],b[10];
	
	strcpy(a,"hogemoge");
    b[0] = 0;
	strcpy(a,b);
	fprintf(stderr, "a:[%s] size:%d\n",a, strlen(a));
    return 0;
}
