#ifndef dql_set_class_h
#define dql_set_class_h

#include <stddef.h>

#include "types.h"
#include "utils.h"

typedef struct _set set_t;

typedef struct _object_api_base
{
    void*(*create)(void*, va_list*);
    void(*free)(void*);    
    comparator_fn_t cmp;
} object_api_base_t;

typedef struct _set_api
{
    class_base_api class;
    //
    int (*add)(set_t*, void*);
    void*(*get)(const set_t*, const void*);
    void(*del)(set_t*, void*);
} set_api_t;


extern set_api_t Set;


#endif // dql_set_class_h