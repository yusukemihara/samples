#include <stdio.h>

void
kahen(char *hoge,...)
{
}

int
main(int argc,char *argv[])
{
	kahen("moge");
	kahen("moge","age");
	kahen("moge","aaaa","dddd");
	kahen("moge","aaa");
}
