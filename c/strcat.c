#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>


int main(void)
{
	char a[256],b[256];
	
	a[0] = 0;

#define HOGE "hoge"
printf("aaa " HOGE "_muge\n");

	sprintf(b,"pubsubhubbub");
	strncat(a,b,3);
	fprintf(stderr, "a:[%s] size:%d\n",a, strlen(a));
    return 0;
}
