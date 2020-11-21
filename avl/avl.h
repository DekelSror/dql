#ifndef dql_avl_h
#define dql_avl_h

typedef void* avl_t;

typedef int(*comparator_fn_t)(const void* a, const void* b);

typedef struct
{
    avl_t(*create)(comparator_fn_t cmp);
    avl_t(*create_ext)(comparator_fn_t cmp, void*(*get_node)(void), void(*free_node)(void*));
    void(*free)(avl_t avl);

    int(*insert)(avl_t, void*);
    void(*remove)(avl_t, void*);
    void*(*find)(const avl_t, const void*);

    size_t(*size)(const avl_t);
    int(*for_each)(avl_t, void(*fn)(void*, void*), void* fn_args);

    
} avl_api_t;

extern const avl_api_t Avl;

#ifdef avl_debug
size_t TreeHeight(const avl_t _this);
int AvlRootValue(const avl_t _this);
#endif


#endif // dql_avl_h


/*
like all modules, I would like it to support
working with Memblocks / other user-defined memory management
At least for the nodes, but for the tree handle as well

Also, iterators
*/