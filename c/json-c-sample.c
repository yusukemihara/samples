#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs json-c` -o $obj $src
$obj
exit
#endif

/**************************************************
# ubuntu json-c package bug

  libjson-c3:amd64 0.12.1-1.3ubuntu0.1

# usage

```
% json-c-sample.c
lh_table_new: calloc failed
```

**************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "json.h"

int main(int argc, char **argv)
{
  json_object *obj,*obj2,*child;
  char str[257];
  int i;

  obj = json_object_new_object();
  child = json_object_new_object();
#if 1
  /* cause lh_table_new: calloc failed */
  for(i=0;i<100000;i++) {
#else
  /* no error */
  for(i=0;i<100000;i++) {
#endif
    memset(str,0,sizeof(str));
    snprintf(str,sizeof(str)-1,"%0255d",i);
    json_object_object_add(child,str,json_object_new_string(str));
  }
  json_object_object_add(obj,"child",child);
  printf("obj[%s]\n\n",json_object_to_json_string(obj));

  obj2 = json_tokener_parse(json_object_to_json_string(obj));
  printf("obj2[%s]\n",json_object_to_json_string(obj2));

  json_object_put(obj);
  json_object_put(obj2);

  return 0;
}
