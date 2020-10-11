

#include <stdlib.h>

#include "vlist.h"

typedef struct vlist_node vlist_node_t;

struct vlist_node
{
    void* _data;
    vlist_node_t* _next;
};

typedef struct 
{
    vlist_node_t _anchor;
    size_t _size;
} _vlist_t;

static void FreeNode(vlist_node_t* node);
static vlist_node_t* FindNode(vlist_node_t* node, const void* val);

static vlist_t Create(void)
{
    _vlist_t* vlist = malloc(sizeof(*vlist));

    vlist->_anchor._data = NULL;
    vlist->_anchor._next = NULL;
    vlist->_size = 0;

    return vlist;
}

static int Add(vlist_t _this, void* data)
{
    _vlist_t* this = _this;

    vlist_node_t* node = malloc(sizeof(*node));

    node->_data = data;
    node->_next = this->_anchor._next;

    this->_anchor._next = node;

    ++this->_size;

    return 0;
}

static int Remove(vlist_t _this, void* val)
{
    _vlist_t* this = _this;
    vlist_node_t* node = FindNode(this->_anchor._next, val);

    if (NULL == node) return 0;

    vlist_node_t* to_delete = node;
    vlist_node_t* rest = node->_next;

    if (rest)
    {
        node->_next = rest->_next;
        node->_data = rest->_data;
        
        to_delete = rest;
    }
    else if (this->_anchor._next == to_delete)
    {
        this->_anchor._next = NULL;
    }

    FreeNode(to_delete);

    --this->_size;

    return 0;
}

static vlist_node_t* FindNode(vlist_node_t* node, const void* val)
{
    while (node)
    {
        if (node->_data == val) return node;
        node = node->_next;
    }

    return NULL;
}

static void* Find(vlist_t _this, const void* val)
{
    _vlist_t* this = _this;

    vlist_node_t* node = FindNode(this->_anchor._next, val);

    return node ? node->_data : NULL;
}



static void FreeNode(vlist_node_t* node)
{
    node->_next = NULL;
    node->_data = NULL;
    free(node);
    node = NULL;
}

static int ForEach(vlist_t _this, void(*fn)(void*, void*), void* fn_args)
{
    _vlist_t* this = _this;
    vlist_node_t* node = this->_anchor._next;

    while (node)
    {
        fn(node->_data, fn_args);

        node = node->_next;
    }

    return 0;
}


static void Free(vlist_t _this)
{
    _vlist_t* this = _this;
    vlist_node_t* node = this->_anchor._next;

    while (node)
    {
        vlist_node_t* next = node->_next;

        node->_data= NULL;
        node->_next = NULL;
        free(node);

        node = next;
    }

    this->_anchor._next = NULL;

    free(this);

    this = NULL;
}

static size_t Size(const vlist_t _this) { return ((const _vlist_t*)_this)->_size;}

const vlist_api_t Vlist = { Create, Free,  Add, Remove, Find, ForEach, Size };