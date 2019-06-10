#include <stdio.h>

int main(int argc,char **argv) {
  int i,j;
  for(i=0,j=10000;i<100;i++) {
    printf("%d %d\n",i,j);
  }
  return 0;
}
