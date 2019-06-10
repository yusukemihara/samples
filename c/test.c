#include <stdio.h>

int main(int argc,char *argv[])
{
  fprintf(stderr,"%02x %02x\n",1<<4,0<<4);
}
