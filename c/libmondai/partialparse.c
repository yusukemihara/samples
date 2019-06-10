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

const char *recdef1 = ""
"test {\n"
"  command varchar(256);\n"
"};";

const char *recdef2 = ""
"test2 {\n"
"  command varchar(256);\n"
"  arg1 varchar(256);\n"
"  arg2 varchar(256);\n"
"  arg3 varchar(256);\n"
"  status int;\n"
"};";

int
main(int argc,char *argv[])
{
  ValueStruct *val1,*val2,*v;
  LargeByteString *buff;

  RecParserInit();
  val1 = RecParseValueMem(recdef1,NULL);
  InitializeValue(val1);
  val2 = RecParseValueMem(recdef2,NULL);
  InitializeValue(val2);
  v = GetRecordItem(val2,"command");
  SetValueString(v,"test",NULL);
  v = GetRecordItem(val2,"arg1");
  SetValueString(v,"arg1",NULL);
  v = GetRecordItem(val2,"arg2");
  SetValueString(v,"arg2",NULL);
  v = GetRecordItem(val2,"arg3");
  SetValueString(v,"arg3",NULL);
  v = GetRecordItem(val2,"status");
  SetValueInteger(v,50);
  
  buff = NewLBS();
  LBS_ReserveSize(buff,NativeSizeValue(NULL,val2),FALSE);
  NativePackValue(NULL,LBS_Body(buff),val2);

  NativeUnPackValue(NULL,LBS_Body(buff),val1);
  DumpValueStruct(val1);

  return 0;
}
