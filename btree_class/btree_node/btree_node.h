#ifndef dql_btree_node_h
#define dql_btree_node_h

#include "btree_class.h"
#include "types.h"

typedef struct _btree_node  btree_node_t;

struct _btree_node
{
    void* _keys[num_keys];
    // in leaf nodes children are ptrs to values and the last one is ptr to next leaf
    btree_node_t* _children[num_children];
    btree_node_t* _parent;
    char is_leaf;
};

btree_node_t* NodeCreate(void);

btree_node_t* NodeFind(btree_node_t* node, const void* key, comparator_fn_t cmp);
btree_node_t* NodeAdd(btree_node_t* node, const void* key, void* value, comparator_fn_t cmp);
void NodeFree(btree_node_t* node);

// api


#endif // dql_btree_node_h



