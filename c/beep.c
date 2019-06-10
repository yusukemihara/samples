#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    alarm(10);
    sleep(5);
    fprintf(stderr,"sleep 1\n");
    alarm(10);
    sleep(5);
    fprintf(stderr,"sleep 2\n");
    return 0;
}
