#ifndef dql_hash_h
#define dql_hash_h

#include "string_class.h"

typedef void* hash_t;


typedef struct 
{
    hash_t (*create)(size_t);
    void(*free)(hash_t);
    //
    int(*set)(hash_t, const string_t, void*);
    void*(*get)(hash_t, const string_t);
    void*(*remove)(hash_t, const string_t);
    void(*for_each)(hash_t, void(*fn)(void*, void*), void* fn_arg);
} hash_api_t;

extern const hash_api_t Hash;

#endif // dql_hash_h

