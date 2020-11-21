#include <stdlib.h>
#include <string.h>
#include "bptree.h"

#define bptree_thisify _bptree_t* this = _this;

typedef struct bpnode_t bpnode_t;

struct bpnode_t
{
    bpnode_t* _parent;
    bpnode_t* _kids[bptree_nkids];
    bpnode_t* _keys[bptree_nkeys];
    char _is_leaf;
};

typedef struct
{
    cmp_fn _cmp;
    bpnode_t* _root;
    alloc_node_fn _alloc_node;
} _bptree_t;

static alloc_node_fn AllocNode;

static bptree_t Create(cmp_fn cmp, alloc_node_fn alloc_node)
{
    _bptree_t* this = malloc(sizeof(*this));

    this->_alloc_node = alloc_node != NULL ? alloc_node : AllocNode;
    this->_cmp = cmp;
    this->_root = this->_alloc_node();    
    this->_root->_is_leaf = 1;

    return this;
}

static bpnode_t* NodeKid(bpnode_t* node, void* key, cmp_fn cmp)
{
    size_t i;
    for (i = 0; i < bptree_nkeys; i++)
    {
        // if (NULL == node->_keys[i])
        if (cmp(key, node->_keys[i]) < 0) break;
    }

    return node->_kids[i];
}

static void* Find(bptree_t _this, void* key)
{
    bptree_thisify

    bpnode_t* leaf = NodeFind(this->_root, key, this->_cmp);

    return (NodeKid(leaf, key, this->_cmp));
}

static bpnode_t* NodeFind(bpnode_t* node, void* key, cmp_fn cmp)
{
    if (node == NULL) return NULL;

    if (node->_is_leaf)
    {
        return node;
    }

    return NodeFind(NodeKid(node, key, cmp), key, cmp);
}

static bpnode_t* NodeInsert(bpnode_t* node, void* key, cmp_fn cmp)
{
    if (node->_parent == NULL) return node;

    for (size_t i = 0; i < bptree_nkeys; i++)
    {
        if (node->_keys[i] == NULL)
        {
            node->_keys[i] = key;
            return node;
        }

        if (cmp(key, node->_keys[i]) < 0)
        {
            void* tmp = key;
            key = node->_keys[i];
            node->_keys[i] = tmp;            
        }
    }

    // full node
    // ...
    //

    return NodeInsert(node->_parent, key, cmp);
}

static int Insert(bptree_t _this, void* key, void* value)
{
    bptree_thisify

    this->_root = NodeInsert(NodeFind(this->_root, key, this->_cmp), key, this->_cmp);
    

    return 0;
}

const bptree_api_t Bptree = {}; 
