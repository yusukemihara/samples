#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *list[] = {
#include "csvinclude.txt"
};

int 
main(int argc, char *argv[])
{
	int n_list = sizeof(list)/sizeof(char*);
fprintf(stderr,"list size[%d]\n",n_list);
	return 0;
}
