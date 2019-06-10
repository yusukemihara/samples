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
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libmondai.h>
#include <RecParser.h>

extern long
GetNowTime(void)
{
	struct	timeval	tv;
	
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000L + tv.tv_usec / 1000L;
}

extern	void
TimerPrintf(
	long	start,
	long	end,
	char	*format,
	...)
{
	char	buff[65535];
	va_list	va;

	va_start(va,format);
	vsnprintf(buff, sizeof(buff), format, va);
	va_end(va);
	printf(" %6ld(ms) %s", (end - start), buff);
}

int
main(int argc,char *argv[])
{
  ValueStruct *value,*v;
  size_t size;
  char *buf;
  int i;
  long a,b;

#define N (10000)

  RecParserInit();
  value = RecParseValue("K02.rec",NULL);
  InitializeValue(value);

  printf("initialized value\n");

  a = GetNowTime();
  for(i=0;i<N;i++) {
    size = JSON_SizeValue(NULL,value);
    buf = malloc(size+1);
    memset(buf,0,size+1);
    JSON_PackValue(NULL,buf,value);
  	free(buf);
  }
  b = GetNowTime();
  TimerPrintf(a,b,"JSON_PackValue\n");

  a = GetNowTime();
  for(i=0;i<N;i++) {
    size = JSON_SizeValueOmmit(NULL,value);
    buf = malloc(size+1);
    memset(buf,0,size+1);
    JSON_PackValueOmmit(NULL,buf,value);
  	free(buf);
  }
  b = GetNowTime();
  TimerPrintf(a,b,"JSON_PackValueOmmit\n");

  return 0;
}
