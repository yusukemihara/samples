#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *p;
	char *line;
	line = malloc(sizeof(char) * 512);
	sprintf(line, "GET /window HTTP/1.1\r\n");
	p = strsep(&line, " ");
	printf("%s : %s\n", p, line);
	p = strsep(&line, " ");
	printf("%s : %s\n", p, line);
	return 0;
}
