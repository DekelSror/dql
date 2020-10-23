#include <stdio.h>
#include <unistd.h>


// #include "framework.h"
#include "event.h"


static size_t counter = 0; 


static void CounterTrigger1(void)
{
    printf("I am the first listener\n");
}

static void CounterTrigger2(void)
{
    printf("I am the second listener\n");
}

int main(void)
{
    // write a test w/o extra threads w/o using the framework
    // then in another module have it start an event loop thread


    return 0;
}
