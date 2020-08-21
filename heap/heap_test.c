#include <stdio.h>

#include "heap.h"

int CompareIntegers(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}


static int nums[] = {5, 4, 1, 3, 2};

int main(void)
{
    heap_t heap = Heap.create(5, CompareIntegers);


    for (size_t i = 0; i < 5; i++)
    {
        Heap.insert(heap, nums + i);
    }

    printf("%d \n", Heap.insert(heap, nums));

    return 0;
}