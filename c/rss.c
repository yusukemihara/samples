#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <libgen.h>

#define SIZE_MEM (1000000)

int
main(int argc,char *argv[])
{
  char *mem;
  sleep(60);
  mem = malloc(SIZE_MEM);
  free(mem);
  sleep(3600*10);
  return 0;
}
