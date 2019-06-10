#if 0
#!/bin/bash
src=$0
obj=${src%.*}
gcc -g -Wl,--no-as-needed `pkg-config --cflags --libs libmondai` -o $obj $src
$obj
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmondai.h>
#include <RecParser.h>

const char *recdef = ""
"test2 {\n"
"  int1 int;\n"
"  int2 int;\n"
"  int3 int[5][5];\n"
"  double1 number(10,5);\n"
"  bool1 bool;\n"
"  command varchar(256);\n"
"  arg1 varchar(256);\n"
"  arg2 varchar(256);\n"
"  arg3 varchar(256);\n"
"  status int;\n"
"  record1 {\n"
"    col1 varchar(256);\n"
"    col2 varchar(256);\n"
"    col3 varchar(256);\n"
"    record2 {\n"
"      int2 int;\n"
"      col21 varchar(256);\n"
"      col22 varchar(256);\n"
"    };\n"
"  }[10];\n"
"};";

int
main(int argc,char *argv[])
{
  ValueStruct *value,*v;
  size_t size;
  char *buf;

  RecParserInit();
  value = RecParseValueMem(recdef,NULL);
  InitializeValue(value);

  v = GetRecordItem(value,"command");
  SetValueString(v,"a\"a\\a/a\ba\fa\na\ra\ta",NULL);
  v = GetItemLongName(value,"record1[0].col1");
  SetValueString(v,"bbbb",NULL);
  v = GetItemLongName(value,"record1[0].record2.col21");
  SetValueString(v,"cccc",NULL);
  v = GetItemLongName(value,"int1");
  ValueInteger(v) = 10;
  v = GetItemLongName(value,"int2");
  ValueInteger(v) = 20;

  size = JSON_SizeValue(NULL,value);
fprintf(stderr,"size:%ld\n",size);
  buf = malloc(size+1);
  memset(buf,0,size+1);
  JSON_PackValue(NULL,buf,value);
fprintf(stderr,"[%s]\nsize:%ld\n",buf,strlen(buf));
  JSON_UnPackValue(NULL,buf,value);

  DumpValueStruct(value);
fprintf(stderr,"----\n");
  JSON_UnPackValue(NULL,"{\"int1\":1000,\"int2\":2000}",value);
  DumpValueStruct(value);

  free(buf);
  return 0;
}
