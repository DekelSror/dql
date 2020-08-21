
#include <stdio.h>
#include <sys/types.h>

#include "mtsq.h"

int main(void)
{
        mtsq_t* test = MTSQCreate(5);
        int elems[] = { 100, 200, 300, 400, 500 };

        MTSQEnqueue(test, &elems[0]);
        MTSQEnqueue(test, &elems[1]);        
        MTSQEnqueue(test, &elems[2]);
        MTSQEnqueue(test, &elems[3]);
        MTSQEnqueue(test, &elems[4]);
        printf("%d\n", MTSQEmpty(test));
        printf("%d\n", *(int*)MTSQDequeue(test));
        printf("%d\n", *(int*)MTSQDequeue(test));
        printf("%d\n", MTSQEmpty(test));
        printf("%d\n", *(int*)MTSQDequeue(test));
        printf("%d\n", *(int*)MTSQDequeue(test));
        printf("%d\n", *(int*)MTSQDequeue(test));

        printf("%d\n", MTSQEmpty(test));


        MTSQDestroy(test);

        return 0;
}