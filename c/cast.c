#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int
main(int argc,char *argv[])
{
  void *p;
  int64_t a,b;

  a = 25;
  p = (void*)a;
  b = (int64_t)p;
  printf("%ld\n",b);

  a = -25;
  p = (void*)a;
  b = (int64_t)p;
  printf("%ld\n",b);
  printf("%d\n",(int)b);

  return 0;
}
