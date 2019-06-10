#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int i=0;
	if (1) {
		printf("%04d\n",__LINE__);
		if (1) {
			printf("%04d\n",__LINE__);
			break;
		} else {
			printf("%04d\n",__LINE__);
		}
		printf("%04d\n",__LINE__);
	}
	printf("%04d\n",__LINE__);
    return 0;
}
