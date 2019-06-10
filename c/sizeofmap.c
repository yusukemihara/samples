#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define     SIZE_BUFF   100
#define     MAX_NUM   100

int
main(int argc,char *argv[])
{
    int i,j;
    char **data;
    data = (char **)malloc(sizeof(char*) * MAX_NUM);
    for(i=0;i<100;i++){
        data[i] = malloc(sizeof(char)*SIZE_BUFF);
        data[i] = strdup("mogera-"); 
    }
    printf("sizeof data [%d], sizeof data[0] [%d] [%s]\n",sizeof((char *)data[]),sizeof(data[0]),data[99]);
    return 0;
}
