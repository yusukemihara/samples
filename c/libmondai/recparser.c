/* -*- Mode: C; c-basic-offset: 2 -*-
 * gcc -g `pkg-config --libs --cflags libmondai` recparser.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmondai.h>
#include <RecParser.h>

int
main(int argc,char *argv[])
{
  ValueStruct *value;
  char *vname;

  RecParserInit();
  value = RecParseValue("api47.rec",&vname);
  printf("vname:%s\n",vname);
  DumpValueStruct(value);
  return 0;
}
