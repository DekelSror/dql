#ifndef dql_pointer_stack_h
#define dql_pointer_stack_h

#include <stddef.h>

typedef struct _stack_api
{
    void*(*create)(size_t capacity);
    void(*free)(void*);
    //
    int(*push)(void*, void*);
    void*(*pop)(void*);
    size_t(*size)(const void*);
    int(*is_empty)(const void*);
} stack_api_t;


typedef void* stack_t;
extern const stack_api_t Stack;


#endif // dql_pointer_stack_h
