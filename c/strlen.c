#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>


int main(void)
{
	char a[10],b[10];

	fprintf(stderr,"%s\n", strndup("あいうえお",strlen("あいうえお")));
return 0;
	fprintf(stderr, "%p %s\n", strndup("hoge", 0),  strndup("hoge", 0));
return 0;
	fprintf(stderr, "%d\n",  strlen(NULL));
	sprintf(a, "hoge");
	b[0] = '\0';
	fprintf(stderr, "%s length: %d\n", "12345", strlen("12345"));
	fprintf(stderr,"strndup(null,0):%s\n",strndup("",0));
	fprintf(stderr,"%p\n", strstr("hoge\r\naa", "\r\n"));
//	fprintf(stderr,"%s:%d %s:%d NULL:%d\n", a, strlen(a), b, strlen(b), strlen(NULL));
    return 0;
}
