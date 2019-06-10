#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs json` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "json.h"

int main(int argc, char **argv)
{
  int i;
  json_object *obj,*array,*obj2,*array2,*child;

  obj = json_object_new_object();
  array = json_object_new_array();
  json_object_array_add(array,json_object_new_string("0"));
  json_object_array_add(array,json_object_new_string("1"));
  json_object_array_add(array,json_object_new_string("2"));
  json_object_array_add(array,json_object_new_string("3"));
  json_object_array_add(array,json_object_new_string("4"));
  json_object_object_add(obj,"array",array);

  json_object_array_put_idx(array,3,NULL);
  fprintf(stderr,"%s\n",json_object_to_json_string(obj));

  obj2 = json_tokener_parse(json_object_to_json_string(obj));
  array2 = json_object_object_get(obj2,"array");
  for(i=0;i<json_object_array_length(array2);i++) {
    child = json_object_array_get_idx(array2,i);
    if (child == NULL) {
      fprintf(stderr,"[%d]NULL\n",i);
      continue;
    } else {
      if (is_error(child)) { 
        fprintf(stderr,"[%d]error\n",i);
        continue;
      }
      fprintf(stderr,"[%d][%s]\n",i,json_object_to_json_string(child));
    }
  }

  json_object_put(obj);

  return 0;
}
