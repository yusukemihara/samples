#include <stdio.h>
#include <stdlib.h>

typedef enum _ENUM {
a,
b,
#if 1
c,
#endif
d,
e,
f,
g,
ENUM_END
} ENUM;

void func_a(int i, int j)
{
	printf("func_a %d %d\n",i,j);
}

int main(void)
{
	int array[ENUM_END];
	ENUM hoge = a;
	printf("ENUM_END:%d sizeofarray:%d\n",ENUM_END,sizeof(array));
	printf("a:%d b:%d c:%d d:%d e:%d f:%d g:%d ENUM_END:%d\n",a,b,c,d,e,f,g,ENUM_END);
	array[hoge] = 1000;
	func_a(array[hoge],20);
    return 0;
}

