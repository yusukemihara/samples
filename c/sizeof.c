#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int main(void)
{
	char *p,a[1024];
	p = malloc(100);
	free(p);
	p = malloc(256);
	fprintf(stderr,"p:%ld a:%ld\n",sizeof(*p),sizeof(a));
    return 0;
}
