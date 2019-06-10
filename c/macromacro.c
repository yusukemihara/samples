#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -o $obj $src
$obj
exit
#endif

#define PRICE "99"

#define SALEAD ""					\
"zaku      \\" PRICE "!\n"		\
"gufu      \\" PRICE "!?\n"		\
"char zaku \\" PRICE "!!?\n"


#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("%s",SALEAD);
	return 0;
}
