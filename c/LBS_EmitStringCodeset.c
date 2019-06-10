/*
	gcc -g `pkg-config --cflags --libs libmondai` LBS_EmitStringCodeset.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <libmondai.h>

int main(int argc,char *argv[])
{
	char *str = "あいうえおかき\xFF\xFF\xFF";
	LargeByteString *lbs;

	lbs = NewLBS();
	LBS_EmitStringCodeset(lbs,str,strlen(str),1024,"euc-jisx0213");
	printf("%s",(char*)LBS_Body(lbs));
}
