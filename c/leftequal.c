#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int
main(void)
{
	char a[200] = "window.frame.widget";
	char b[200] = "window.";
	char c[200] = "window1.";
	char d[200] = "wind.";
	char *p;
	p = strstr(a,b);
	printf("result [%d]\n", a - p);
	p = strstr(a,c);
	printf("result [%d]\n", a - p);
	p = strstr(a,d);
	printf("result [%d]\n", a - p);
    return 0;
}
