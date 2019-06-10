#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
	FILE *fp;
	umask(S_IRGRP |  S_IWGRP |  S_IROTH  |  S_IWOTH);
	fp = fopen("/tmp/hoge.txt","w");
	fprintf(fp,"ugege");
	fclose(fp);
    return 0;
}
