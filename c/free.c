#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <libgen.h>

#define     SIZE_BUFF   100
#define     MAX_NUM   100

void
f(int i)
{
	char *buff;
	if (i==0) {
		buff = malloc(64);
	}
	free(buff);
}

int
main(int argc,char *argv[])
{
free(NULL);
fprintf(stderr,"ou!?\n");
exit(0);

	char *dir;
fprintf(stderr,"argv[0]:%s\n",argv[0]);
dir = dirname(argv[0]);
fprintf(stderr,"drname:%s\n",dir);
exit(0);
	f(0);
	f(1);
    return 0;
}
