#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `cups-config --cflags --libs` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "cups/cups.h"

int main(int argc, char **argv)
{
  int i,num;
  cups_dest_t *dests,*dest;
  
  num = cupsGetDests(&dests);
  fprintf(stderr,"num:%d\n",num);
  for(i=0,dest=dests;i<num;i++,dest++){
    fprintf(stderr,"name:%s\n",dest->name);
  }
  cupsFreeDests(num,dests);
  return 0;
}
