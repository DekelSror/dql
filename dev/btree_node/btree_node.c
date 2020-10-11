#include <string.h>
#include <stdlib.h>

#include "btree_class.h"
#include "btree_node.h"

size_t FindIndexForKey(const btree_node_t* node, const void* key, comparator_fn_t cmp)
{
    for (size_t idx = 0; idx < num_keys; idx++)
    {
        if(!has_nth_key(node, idx)) return idx;
        if (0 > cmp(key, node->_keys[idx])) return idx;
    }
}

btree_node_t* NodeSplit(btree_node_t* node, const void* key, void* value, comparator_fn_t cmp)
{
    // sort node keys with insert key
    char* keys_buf[num_children] = { 0 };
    char* kids_buf[num_children] = { 0 };
    memset(kids_buf, 0, sizeof kids_buf);
    memset(keys_buf, 0, sizeof keys_buf);
    
    const size_t idx = FindIndexForKey(node, key, cmp);    

    // fill sorted buffers
    memmove(keys_buf, node->_keys, idx * sizeof(void*));
    memmove(kids_buf, node->_children, idx * sizeof(void*));

    keys_buf[idx] = key;
    kids_buf[idx] = value;

    memmove(keys_buf + idx + 1, node->_keys + idx, (num_keys - idx)* sizeof(void*));
    memmove(kids_buf + idx + 1, node->_children + idx, (num_keys - idx)* sizeof(void*));

    // new leaf
    btree_node_t* left_sibling =  NodeCreate();
    left_sibling->is_leaf = node->is_leaf;
    left_sibling->_parent = node->_parent;

    // divide sorted keys and children between siblings - (n/2 + 1) left, (n/2) right
    memmove(left_sibling->_keys, keys_buf, branch_factor + 1);
    memmove(left_sibling->_children, kids_buf, branch_factor + 1);
    memmove(node->_keys, keys_buf + branch_factor + 1, branch_factor);
    memmove(node->_children, kids_buf + branch_factor + 1, branch_factor);

    return left_sibling; // midval is left_sibling->_keys[branch_factor]
}

btree_node_t* Top(btree_node_t* node)
{
    if (NULL == node->_parent) return node;

    return Top(node->_parent);
}

btree_node_t* NodeAdd(btree_node_t* node, const void* key, void* value, comparator_fn_t cmp)
{
    if (node_is_full(node))
    {
        btree_node_t* left_sibling = NodeSplit(node, key, value, cmp);

        if (node->is_leaf)
        {
            left_sibling->_children[num_keys] = node; // assign next node
        }

        if (NULL == node->_parent)
        {
            node->_parent = NodeCreate();
            node->_parent->is_leaf = 0;

            left_sibling->_parent = node->_parent;

            node->_parent->_keys[0] = left_sibling->_keys[branch_factor];
            node->_parent->_children[0] = left_sibling;
            node->_parent->_children[1] = node;
        }

        return NodeAdd(node->_parent, key, left_sibling, cmp);
    }
    else
    {
        // regular insertion to bucket
        char* keys_buf[num_keys] = { 0 };
        char* kids_buf[num_keys] = { 0 };
        memset(kids_buf, 0, sizeof kids_buf);
        memset(keys_buf, 0, sizeof keys_buf);
        
        const size_t idx = FindIndexForKey(node, key, cmp);

        memmove(keys_buf, node->_keys + idx, sizeof(void*) * idx);
        node->_keys[idx] = key;
        memmove(node->_keys + idx + 1, keys_buf, sizeof(void*) * (num_keys - idx - 1));
        
        memmove(kids_buf, node->_children + idx, sizeof(void*) * idx);
        node->_children[idx] = value;
        memmove(node->_children + idx + 1, kids_buf, sizeof(void*) * (num_keys - idx - 1));
    }

    return Top(node);
}

btree_node_t* NodeCreate(void)
{
    btree_node_t* node = malloc(sizeof(*node));

    if (NULL == node) return NULL;

    memset(node, 0, sizeof(*node));

    return node;
}

btree_node_t* NodeFind(btree_node_t* node, const void* key, comparator_fn_t cmp)
{
    if (node->is_leaf) return node;

    const size_t idx = FindIndexForKey(node, key, cmp);

    return NodeFind(node->_children[idx], key, cmp);
}

void NodeFree(btree_node_t* node)
{
    if (NULL == node) return;

    if (!node->is_leaf) // distinction - leaves' children aren't nodes
    {
        
        for (size_t i = 0; i < num_children; i++)
        {
            NodeFree(node->_children[i]);
        }
    }

    memset(node->_children, sizeof(node->_children), 0);
    memset(node->_keys, sizeof(node->_keys), 0);

    node->_parent = NULL;

    free(node);
    node = NULL;
}