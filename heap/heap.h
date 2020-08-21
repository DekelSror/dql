#ifndef dql_heap_h
#define dql_heap_h

#include <stddef.h>

#include "types.h"

typedef void* heap_t;


typedef struct 
{
    heap_t(*create)(size_t, comparator_fn_t);
    void(*free)(heap_t);
    void*(*take_top)(heap_t);
    int(*insert)(heap_t, void*);    
    void*(*remove)(heap_t, void*);
} heap_api_t;


extern const heap_api_t Heap;


#endif // dql_heap_h
