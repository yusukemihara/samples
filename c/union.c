#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define HogeAttr(p,t,a) ((p)->Data.t.a)

typedef struct {
    union {
      struct {
        int a;
        int b;
        int c;
      } Int;
      struct {
        float d;
        float e;
        float f;
      } Float;
    } Data;
} Hoge;

int
main(void)
{
	char *p = ""; 

	free(p);
	exit(0);

    Hoge *hoge;
    hoge = (Hoge *)malloc(sizeof(Hoge));
    HogeAttr(hoge,Int,a) = 0;
    HogeAttr(hoge,Int,b) = 1;
    HogeAttr(hoge,Int,c) = 2;
    printf("%d\n",HogeAttr(hoge,Int,a));
    printf("%d\n",HogeAttr(hoge,Int,b));
    printf("%d\n",HogeAttr(hoge,Int,c));
    HogeAttr(hoge,Float,d) = 0.0;
    HogeAttr(hoge,Float,e) = 0.1;
    HogeAttr(hoge,Float,f) = 0.2;
    printf("%f\n",HogeAttr(hoge,Float,d));
    printf("%f\n",HogeAttr(hoge,Float,e));
    printf("%f\n",HogeAttr(hoge,Float,f));
    return 0;
}
