#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <pthread.h>

#include "defs.h"

int main(void)
{
    

    init();


    printf("initialized all and gonna run!\n");

    Run();    

    printf("this is not supposed to appear\n");


    cleanup();

    return 0;
}
