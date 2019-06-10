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
  json_object *str,*obj,*child;

  str = json_object_new_string("abcあいう");
  printf("str[%s]\n",json_object_get_string(str));
  printf("json_str[%s]\n",json_object_to_json_string(str));
  json_object_put(str);

  obj = json_tokener_parse("\"aiu\\u3042\\u3044\\u3046\"");
  printf("obj[%s]\n",json_object_to_json_string(obj));

  obj = json_tokener_parse("-----");
  if (is_error(obj)) {
    printf("error\n");
  }

  obj = json_object_new_object();
  child = json_object_new_object();
  json_object_object_add(child,"a",json_object_new_string("b"));
  json_object_object_add(obj,"child",child);
  printf("json_str[%s]\n",json_object_to_json_string(obj));

  child = json_object_object_get(obj,"notexist");
  printf("child:%p\n",child);
  if (is_error(child)) {
    printf("error\n");
  } else {
    printf("type:%d\n",json_object_get_type(child));
  }

  json_object_put(obj);

  return 0;
}
