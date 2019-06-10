#include <stdio.h>
 
/* DDRC |= (1 << PC0); */
#define DDRD(n) printf("ddrd[%d]\n",(n))
#define DSET(p,n) (DDR##p(n))
 
/* PORTC |= (1 << PC0); */
#define PSET(p,n,d) (PORT##p|=(d<<P##p##n))

unsigned char matrix[8] = {
0b11111111,
0b10000001,
0b10111101,
0b10100101,
0b10100101,
0b10111101,
0b10000001,
0b11111111
};
 
int 
main(void) 
{
  int i,j,k;
  for (k=0;k<2;k++){
    printf("-----\n");
    for(i=0;i<8;i++) {
      for(j=0;j<8;j++) {
        printf("%d ",((matrix[i]&1<<j)>>j)^(k&1));
      }
      printf("\n");
    }
    printf("-----\n");
  }
  return 0;
}
