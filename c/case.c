#include <stdio.h>

#define A 0
#define B (long)10
#define C (unsigned long)90000000

int main(void)
{
    long a = 10;
    switch(a){
    case A:
        printf("moga\n"); 
        break;
    case B:
        printf("mogi\n"); 
        break;
    case C:
        printf("mogu\n"); 
        break;
    }
    return 0;
}
