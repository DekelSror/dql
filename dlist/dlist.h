#ifndef dql_dynamic_array_dequeue_list_h
#define dql_dynamic_array_dequeue_list_h

#include <stddef.h>

typedef void* darray_t;
typedef void* da_iter_t;

typedef struct 
{
    darray_t(*create)(void*(*alloc)(size_t));
    void(*free)(darray_t);
    //
    int(*push_front)(darray_t, void*);
    void*(*pop_front)(darray_t);
    //
    int(*push_back)(darray_t, void*);
    void*(*pop_back)(darray_t);
    //    
    da_iter_t(*first)(darray_t);
    da_iter_t(*last)(darray_t);
    da_iter_t(*next)(da_iter_t);
    da_iter_t(*prev)(da_iter_t);

    
} darray_api_t;

#endif // dql_dynamic_array_dequeue_list_h
