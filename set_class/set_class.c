#include <stdarg.h>
#include <stdlib.h>

#include "utils.h"
#include "types.h"
#include "set_class.h"


typedef size_t(*hash_fn_t)(const void*);

#define BOUND_HASH(val) (set->_hash(val) % set->_capacity)
#define HASH_AT(i) (set->_table + (set->_elem_size * i))


struct _set
{
    comparator_fn_t _cmp;
    hash_fn_t _hash;
    size_t _capacity;
    size_t _elem_size;
    char _table[1];
};

typedef struct _object_api_base
{
    void*(*create)();
    void(*free)(void*);
} object_api_base_t;


void* SetCreate(void* _set, va_list* ctor_args);
void SetFree(set_t*);
//
int  SetAdd(set_t*, void*);
void* SetGet(const set_t*, const void*);
void SetDel(set_t*, void*);

#define set_thisify set_t* set = (set_t*)_set;

void* SetCreate(void* _set, va_list* ctor_args)
{
    set_thisify
    
    set->_elem_size = va_arg(*ctor_args, size_t);
    set->_capacity = va_arg(*ctor_args, size_t);


    return set;
}

void* SetGet(const set_t* set, const void* val)
{
    size_t h = BOUND_HASH(val);

    const char* element = HASH_AT(h);

    while (0 != set->_cmp(element, val)) 
    {
        if (element >= set->_table + (set->_capacity * set->_elem_size))
        {
            return NULL;
        }

        element += set->_elem_size;
    }

    return element;
}

int SetAdd(set_t* set, void* val)
{
    if (NULL != SetGet(set, val)) return 1; // exists

    size_t h = BOUND_HASH(val);


}

void SetDel(set_t* set, void* val)
{
    if (NULL != SetGet(set, val)) return; // exists

    
}


const set_api_t Set = {{SetCreate, SetFree, sizeof(set_t)}, SetAdd, SetGet, SetDel};


