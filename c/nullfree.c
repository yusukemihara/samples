#include <stdio.h>


int main(void)
{
	int i;
	for(i=0;i<=10000;i++){
		printf("%d\n",i);
		free(0x0);
	}
    return 0;
}
