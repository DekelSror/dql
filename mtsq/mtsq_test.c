
#include <stdio.h>
#include <sys/types.h>

#include "mtsq.h"

int main(void)
{
        mtsq_t test = MTSQ.create(5);
        int elems[] = { 100, 200, 300, 400, 500 };

        MTSQ.enqueue(test, &elems[0]);
        MTSQ.enqueue(test, &elems[1]);        
        MTSQ.enqueue(test, &elems[2]);
        MTSQ.enqueue(test, &elems[3]);
        MTSQ.enqueue(test, &elems[4]);
        printf("%d\n", MTSQ.empty(test));
        printf("%d\n", *(int*)MTSQ.dequeue(test));
        printf("%d\n", *(int*)MTSQ.dequeue(test));
        printf("%d\n", MTSQ.empty(test));
        printf("%d\n", *(int*)MTSQ.dequeue(test));
        printf("%d\n", *(int*)MTSQ.dequeue(test));
        printf("%d\n", *(int*)MTSQ.dequeue(test));

        printf("%d\n", MTSQ.empty(test));


        MTSQ.free(test);

        return 0;
}