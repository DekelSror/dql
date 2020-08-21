#ifndef dql_btree_class_h
#define dql_btree_class_h

#include "iterator_class.h"
#include "types.h"

typedef void*(*membergetter_fn_t)(void*);


typedef struct _btree btree_t;

typedef struct _bterator bterator_t;


typedef struct _btree_api
{
    btree_t*(*create)(comparator_fn_t cmp, membergetter_fn_t key_getter);
    void(*free)(btree_t*);
    //
    int(*add)(btree_t*, void*);
    void*(*get)(const btree_t*, const void*);
    void(*del)(btree_t*, void*);
} btree_api_t;

extern const btree_api_t Btree;
extern const iterator_api_t Bterator;

// tree width
#define branch_factor (2)

#define num_keys (branch_factor * 2)
#define num_children (branch_factor * 2 + 1)

// util macros
#define has_nth_key(node, n) (NULL != node->_keys[n])
#define has_nth_child(node, n) (NULL != node->_children[n])
#define node_is_full(node) (NULL != node->_keys[num_keys - 1])

#endif // dql_btree_class_h
