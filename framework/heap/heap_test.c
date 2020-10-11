#include <stdio.h>

#include "heap.h"

static errcount = 0;

int MaxOf(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int MinOf(const void* a, const void* b)
{
    return *(int*)b - *(int*)a;
}

void InsertAndPop(void)
{
    int val = 10;
    heap_t heap = Heap.create(1, MinOf);

    if (Heap.insert(heap, &val))
    {
        printf("InsertAndPop failed insert\n");
        ++errcount;
    }

    if (*(int*)Heap.pop(heap) != val)
    {
        printf("InsertAndPop failed pop\n");
        ++errcount;
    }
}

void InsertOverPopOver(void)
{
    static int nums[] = {1, 2, 3, 0};
    static int nums_max_sorted[] = {3, 2, 1, 0};
    const size_t test_size = sizeof(nums) / sizeof(int);
    heap_t heap = Heap.create(test_size, MaxOf);
    int status = 0;


    for (size_t i = 0; i < test_size; i++)
    {
        if(Heap.insert(heap, nums + i))
        {
            printf("InsertOverPopOver failed insert %lu\n", i);
            ++errcount;
        }
    }

    if(1 != (status = Heap.insert(heap, nums)))
    {
        printf("InsertOverPopOver failed over insert %d\n", status);
        ++errcount;
    }

    for (size_t i = 0; i < test_size; i++)
    {
        if(nums_max_sorted[i] != *(int*)Heap.pop(heap))
        {
            printf("InsertOverPopOver failed pop %lu\n", i);
            ++errcount;
        }
    }

    if (NULL != Heap.pop(heap))
    {
        printf("InsertOverPopOver failed over pop\n");
        ++errcount;
    }

}

int main(void)
{
    InsertAndPop();
    errcount = 0;
    InsertOverPopOver();
    
    return 0;
}