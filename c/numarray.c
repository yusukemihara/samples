#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  char a[][2][64] = {
    {"",""},
    {"",""},
    {"",""}
  };
  fprintf(stderr,"num:%ld\n",sizeof(a)/sizeof(a[0]));
  return 0;
}

