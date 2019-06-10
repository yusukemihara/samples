#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs uuid` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>

int
main(int argc,char *argv[])
{
	uuid_t u;
	char buf[37];

	uuid_generate(u);
	uuid_unparse(u,buf);
	printf("uuid[%s]\n",buf);

	return 0;
}
