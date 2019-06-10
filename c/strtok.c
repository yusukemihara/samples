#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <string.h

int main(int argc, char *argv[])
{
	const char *str = "C=JP,O=org,CN=name";

	return 0;
}
