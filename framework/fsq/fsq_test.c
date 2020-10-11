#include <stdio.h>

#include "fsq.h"


int main(void)
{
    char test_data[] = {'a', 'b', 'c', 'd', 'e'};
    fsq_t q = FSQ.create(3);

    printf("enq a %d\n", FSQ.enq(q, test_data));
    printf("enq b %d\n", FSQ.enq(q, test_data + 1));
    printf("enq c %d\n", FSQ.enq(q, test_data + 2));

    printf("enq d -too many %d\n", FSQ.enq(q, test_data + 3));

    printf("deq %c (want a) \n", *(char*)FSQ.deq(q));
    printf("deq %c (want b) \n", *(char*)FSQ.deq(q));

    printf("re-enq %c %d\n", test_data[3], FSQ.enq(q, test_data + 3));

    printf("deq %c (want c) \n", *(char*)FSQ.deq(q));
    printf("deq %c (want d) \n", *(char*)FSQ.deq(q));

    printf("deq - too many %p (want NULL) \n", FSQ.deq(q));

    FSQ.free(q);

    return 0;
} 
