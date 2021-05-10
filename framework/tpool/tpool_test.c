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
        tpool_t* pool = Tpool.create(0);

        sleep(2);
        for (size_t i = 0; i < NUM_TASKS; i++)
        {       
                Tpool.add_task(pool, TaskPrintInfo, (void*)i, NULL);
        }


        Tpool.halt(pool);
        Tpool.free(pool);

        return 0;
}
