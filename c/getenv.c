#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *env;
  env = getenv("MON_DB_REC_MEM_SAVE");
  if (env != NULL) {
    if (env[0] == '1') {
      fprintf(stderr,"TRUE\n");
    } else {
      fprintf(stderr,"FALSE\n");
    }
  } else {
      fprintf(stderr,"---\n");
  }
  return 0;
}
