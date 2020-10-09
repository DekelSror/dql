#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "tpool.h"

#define NUM_TASKS (10)


static void* TaskPrintInfo(void* arg)
{
        printf("thread no. %ld, hacky index %lu\n", pthread_self(), (size_t)arg);

        return NULL;
}


int main(void)
{
        FrameworkInit();

        for (size_t i = 0; i < NUM_TASKS; i++)
        {                
                Task(TaskPrintInfo, (void*)i, NULL);
        }

        FrameworkCleanup();

        return 0;
}
