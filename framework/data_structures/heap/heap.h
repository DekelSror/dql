#ifndef dql_heap_h
#define dql_heap_h

#include <stddef.h>


typedef void* heap_t;
typedef int(*comparator_fn_t)(const void*, const void*);


typedef struct 
{
    // capacity - maximum heap size
    // int (*cmp)(const void* _a, const void* _b) 
    // comparator for values. positive return means _a precedes _b
    heap_t(*create)(size_t capacity, comparator_fn_t cmp);
    void(*free)(heap_t);

    // do not modify the return value of this (the parts that has to do with cmp) or no guarantees
    // provided for speed, to prevent pop-check-insert back in pattern
    void*(*top)(heap_t);
    int(*insert)(heap_t, void*);    
    void*(*pop)(heap_t);
    // not implemented
    void*(*remove)(heap_t, void*);
} heap_api_t;


extern const heap_api_t Heap;


#endif // dql_heap_h
