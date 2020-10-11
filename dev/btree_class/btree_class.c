#include <stdlib.h>
#include <string.h>

#include "btree_class.h"
#include "btree_node.h"

struct _btree
{
    comparator_fn_t _cmp;
    membergetter_fn_t _key_getter;
    btree_node_t* _root;
};

struct _bterator
{
    btree_node_t* _node;
    size_t _idx;
};


// api declarations
// Btree
static btree_t* BtreeCreate(comparator_fn_t cmp, membergetter_fn_t key_getter);
static void BtreeFree(btree_t*);
//
static int BtreeAdd(btree_t*, void*);
static void* BtreeGet(const btree_t*, const void*);
static void BtreeDel(btree_t*, void*);
//
// Bterator
static void BteratorFree(void* bter);
static void* BteratorPrev(void* bter);
static void* BteratorNext(void* bter);
static void* BteratorGet(const void* bter);
static void* BtreeLast(const btree_t* btree);
static void* BtreeFirst(const btree_t* btree);
//

// class api object
const iterator_api_t Bterator = { BteratorGet, BtreeFirst, BtreeLast, BteratorNext, BteratorPrev, BteratorFree };
const btree_api_t Btree = {BtreeCreate, BtreeFree, BtreeAdd, BtreeGet, BtreeDel};
//


// not implemented
static void BtreeDel(btree_t* btree, void* key)
{

}



// api implementation
static btree_t* BtreeCreate(comparator_fn_t cmp, membergetter_fn_t key_getter)
{
    btree_t* btree = malloc(sizeof(*btree));

    if (NULL == btree) return NULL;

    btree->_cmp = cmp;
    btree->_key_getter = key_getter;
    btree->_root = NodeCreate();
    btree->_root->_parent = NULL;

    btree->_root->is_leaf = 1;

    return btree;
}

static void* BtreeGet(const btree_t* btree, const void* key)
{
    btree_node_t* node = NodeFind(btree->_root, key, btree->_cmp);    
    
    for (size_t idx = 0; idx < num_keys; idx++)
    {
        if (btree->_cmp(key, node->_keys[idx]) == 0) 
        {
            return node->_children[idx];
        }
    }

    return NULL;
}

static int BtreeAdd(btree_t* btree, void* value)
{
    btree_node_t* leaf = NodeFind(btree->_root, btree->_key_getter(value), btree->_cmp);

    btree_node_t* node = NodeAdd(leaf,
        btree->_key_getter(value),
        value,
        btree->_cmp
    );  

    if (NULL != node)
    {
        btree->_root = node;
        return 0;
    }

    return 1;
}

static void BtreeFree(btree_t* btree)
{
    NodeFree(btree->_root);
    free(btree);
    btree = NULL;    
}

static btree_node_t* NodeLowest(btree_node_t* node)
{
    while (!(node->is_leaf))
    {
        node = node->_children[0];
    }

    return node;
}

static btree_node_t* NodeHighest(btree_node_t* node)
{
    while (!node->is_leaf)
    {
        size_t idx = 0;
        while(has_nth_key(node, idx) && idx < num_keys) ++idx;

        node = node->_children[idx];
    }

    return node;
}

void BtreeForEach(btree_t* btree, void(*fn)(void* val, void* arg), void* fn_arg)
{
    btree_node_t* node = NodeLowest(btree->_root);

    while (NULL != node)
    {
        for (size_t i = 0; i < num_keys; i++)
        {
            if (!has_nth_child(node, i)) break;

            fn(node->_children[0], fn_arg);
        }

        node = node->_children[num_keys];
    }
}

// bterator related functions

static void* BtreeFirst(const btree_t* btree)
{
    bterator_t* bter = malloc(sizeof(*bter));

    bter->_node = NodeLowest(btree->_root);

    bter->_idx = 0;

    return bter;
}

static void* BtreeLast(const btree_t* btree)
{
    bterator_t* bter = malloc(sizeof(*bter));

    bter->_node = NodeHighest(btree->_root);

    bter->_idx = 0;

    return bter;
}

static void* BteratorGet(const void* _bter)
{
    bterator_t* bter = (bterator_t*)_bter;

    return bter->_node->_children[bter->_idx];
}

static void* BteratorNext(void* _bter)
{
    bterator_t* bter = (bterator_t*)_bter;

    if (num_keys == bter->_idx) 
    {
        bter->_node = (btree_node_t*)bter->_node->_children[num_keys];
        bter->_idx = 0;
    }
    else
    {
        ++bter->_idx;
    }

    return bter;
}

static void* BteratorPrev(void* _bter)
{
    bterator_t* bter = (bterator_t*)_bter;

    if (0 == bter->_idx) 
    {
        bter->_node = (btree_node_t*)bter->_node->_children[num_children];
        bter->_idx = 0;
    }
    else
    {
        ++bter->_idx;
    }

    return bter;
}

static void BteratorFree(void* _bter)
{
    bterator_t* bter = (bterator_t*)_bter;

    bter->_node = NULL;
    free(bter);
    bter = NULL;
}




