#include <stdlib.h>
#include "avl.h"

#define avl_thisify _avl_t* this = _this;
#define avl_const_thisify const _avl_t* this = _this;
#define branch_height(node, side) (NULL != node->_kids[side] ? node->_kids[side]->_height : 0)
#define max(a, b) ((a > b) ? a : b)
#define NodeUpdateHeight(node) (node->_height = max(branch_height(node, right), branch_height(node, left)) + 1)

typedef struct avl_node avl_node_t;
typedef enum {left, right} sides_e;

struct avl_node
{
    avl_node_t* _kids[2];
    void* _data;
    size_t _height;
};

typedef struct
{
    comparator_fn_t _cmp;
    size_t _size;
    avl_node_t* _root;
    avl_node_t* (*_get_node)(void);
    void(*_free_node)(avl_node_t*);
} _avl_t;

static avl_node_t* Rotate_LL(avl_node_t* node);
static avl_node_t* Rotate_LR(avl_node_t* node);
static avl_node_t* Rotate_RL(avl_node_t* node);
static avl_node_t* Rotate_RR(avl_node_t* node);

// recursive node api
static void* NodeFind(avl_node_t* node, const void* elem, comparator_fn_t cmp);
static avl_node_t* NodeRemove(avl_node_t* node, void* elem, comparator_fn_t cmp, void(*free_node)(avl_node_t*));
static avl_node_t* NodeInsert(avl_node_t* node, void* elem, comparator_fn_t cmp, avl_node_t*(*get_node)(void));
static void NodeFree(avl_node_t* node, void(*free_node)(avl_node_t*));
static int NodeForEach(avl_node_t* node, void(*fn)(void*, void*), void* fn_args);

typedef avl_node_t*(*rotation_fn_t)(avl_node_t*);

static const rotation_fn_t Rotate[2][2] = {{Rotate_LL, Rotate_LR}, {Rotate_RL, Rotate_RR}};

avl_node_t* DefaultGetNode(void)
{
    return malloc(sizeof(avl_node_t));
}

void DefaultFreeNode(avl_node_t* node)
{
    return free(node);
}

static avl_t CreateExt(comparator_fn_t cmp, void*(*get_node)(void), void(*free_node)(void*))
{
    _avl_t* this = malloc(sizeof(*this));

    if (NULL == this) return NULL;

    this->_cmp = cmp;
    this->_size = 0;
    this->_root = NULL;
    this->_get_node = NULL != get_node ? get_node : DefaultGetNode;
    this->_free_node = NULL != free_node ? free_node : DefaultFreeNode;

    return this;
}

static avl_t Create(comparator_fn_t cmp)
{
    _avl_t* this = malloc(sizeof(*this));

    if (NULL == this) return NULL;

    this->_cmp = cmp;
    this->_size = 0;
    this->_root = NULL;
    this->_get_node = DefaultGetNode;
    this->_free_node = DefaultFreeNode;

    return this;
}

static int Insert(avl_t _this, void* elem)
{
    avl_thisify

    this->_root = NodeInsert(this->_root, elem, this->_cmp, this->_get_node);

    ++this->_size;

    return 0;
}

static void Remove(avl_t _this, void* elem)
{
    avl_thisify

    if (NULL != this->_root) --this->_size;
    this->_root = NodeRemove(this->_root, elem, this->_cmp, this->_free_node);
}

static void Free(avl_t _this)
{
    avl_thisify

    NodeFree(this->_root, this->_free_node);

    free(this);
    this = NULL;
}

static void* Find(const avl_t _this, const void* elem)
{
    avl_const_thisify

    return NodeFind(this->_root, elem, this->_cmp);
}

static size_t Size(const avl_t _this)
{
    avl_thisify

    return this->_size;
}

static int ForEach(avl_t _this, void(*fn)(void*, void*), void* fn_args)
{
    avl_thisify

    return NodeForEach(this->_root, fn, fn_args);
}

// node


// node utils
static void FreeLeaf(avl_node_t* node, void(*free_node)(avl_node_t*));
static avl_node_t* CreateLeaf(void* val, avl_node_t*(get_node)(void));
static int NodeBalance(const avl_node_t* node);


static int NodeForEach(avl_node_t* node, void(*fn)(void*, void*), void* fn_args)
{
    if (NULL == node) return 0;

    NodeForEach(node->_kids[left], fn, fn_args);
    fn(node->_data, fn_args);
    NodeForEach(node->_kids[right], fn, fn_args);

    return 0;
}

static void* NodeFind(avl_node_t* node, const  void* elem, comparator_fn_t cmp)
{
    if (NULL == node) return node;

    const int cmp_res = cmp(elem, node->_data);

    if (0 == cmp_res) return node->_data;

    return NodeFind(node->_kids[cmp_res > 0], elem, cmp);
}

static void NodeFree(avl_node_t* node, void(*free_node)(avl_node_t*))
{
    if (NULL == node) return;

    NodeFree(node->_kids[left], free_node);
    NodeFree(node->_kids[right], free_node);
    FreeLeaf(node, free_node);
}

static avl_node_t* NodeGetClosest(avl_node_t* node, sides_e side)
{
    node = node->_kids[side];

    while (NULL != node->_kids[!side] && NULL != node->_kids[!side]->_kids[!side])
    {
        node = node->_kids[!side];
    }

    return node;
}

static avl_node_t* NodeRemove(avl_node_t* node, void* elem, comparator_fn_t cmp, void(*free_node)(avl_node_t*))
{
    if (NULL == node) return node;

    const int cmp_res = cmp(elem, node->_data);

    if (0 == cmp_res)
    {
        if (NULL == node->_kids[right] && NULL == node->_kids[left])
        {
            FreeLeaf(node, free_node);
            node = NULL;
        }
        else if (NULL == node->_kids[right] || NULL == node->_kids[left])
        {
            const sides_e side = (NULL != node->_kids[right]);

            node->_data = node->_kids[side]->_data;
            node->_height = 1;

            FreeLeaf(node->_kids[side], free_node);
            node->_kids[side] = NULL;
        }
        else
        {            
            avl_node_t* next = node->_kids[left];

            while (next->_kids[right])
            {
                next = next->_kids[right];
            }

            void* tmp = node->_data;
            node->_data = next->_data;
            next->_data = tmp;

            node = NodeRemove(node, next->_data, cmp, free_node);
        }

        return node;
    }

    const sides_e side = cmp_res > 0;
    node->_kids[side] = NodeRemove(node->_kids[side], elem, cmp, free_node);
    

    const int balance = NodeBalance(node);
    if (-1 > balance || 1 < balance)
    {
        const sides_e second_side = 
            NodeBalance(node->_kids[right]) > NodeBalance(node->_kids[left]);
        
        node = Rotate[balance > 1][second_side](node);
    }

    NodeUpdateHeight(node);
    
    return node;
}

static avl_node_t* NodeInsert(avl_node_t* node, void* elem, comparator_fn_t cmp, avl_node_t*(get_node)(void))
{
    if (NULL == node)
    {
        return CreateLeaf(elem, get_node);
    }

    const int side = cmp(elem, node->_data) > 0;

    node->_kids[side] = NodeInsert(node->_kids[side], elem, cmp, get_node);

    const int balance = NodeBalance(node);

    if (-1 > balance || 1 < balance)
    {
        const sides_e first_side = balance > 1;
        const sides_e second_side = NodeBalance(node->_kids[first_side])  > 0;
        
        node = Rotate[first_side][second_side](node);
    }

    NodeUpdateHeight(node);

    return node;
}

static int NodeBalance(const avl_node_t* node)
{
    if (NULL == node) return 0;

    return branch_height(node, right) - branch_height(node, left);
}

static avl_node_t* CreateLeaf(void* val, avl_node_t*(get_node)(void))
{
    avl_node_t* n = get_node();

    if (NULL == n) return NULL;

    n->_kids[left] = NULL;
    n->_kids[right] = NULL;
    n->_data = val;
    n->_height = 1;

    return n;
}

static void FreeLeaf(avl_node_t* node, void(*free_node)(avl_node_t*))
{
    node->_data = NULL;
    node->_kids[left] = NULL;
    node->_kids[right] = NULL;

    free_node(node);
    node = NULL;
}

// rotations


static avl_node_t* Rotate_LL(avl_node_t* node)
{
    avl_node_t* anchor = node->_kids[left];

    node->_kids[left] = anchor->_kids[right];
    anchor->_kids[right] = node;

    NodeUpdateHeight(node);
    NodeUpdateHeight(anchor);

    return anchor;
}

static avl_node_t* Rotate_LR(avl_node_t* node)
{
    avl_node_t* anchor = node->_kids[left]->_kids[right];
    node->_kids[left]->_kids[right] = anchor->_kids[left];

    anchor->_kids[left] = node->_kids[left];
    node->_kids[left] = anchor->_kids[right];

    anchor->_kids[right] = node;

    NodeUpdateHeight(node);
    NodeUpdateHeight(anchor);

    return anchor;
}

static avl_node_t* Rotate_RL(avl_node_t* node)
{
    avl_node_t* anchor = node->_kids[right]->_kids[left];
    node->_kids[right]->_kids[left] = anchor->_kids[right];

    anchor->_kids[right] = node->_kids[right];
    node->_kids[right] = anchor->_kids[left];

    anchor->_kids[left] = node;

    NodeUpdateHeight(node);
    NodeUpdateHeight(anchor);
    return anchor;

}

static avl_node_t* Rotate_RR(avl_node_t* node)
{
    avl_node_t* anchor = node->_kids[right];

    node->_kids[right] = anchor->_kids[left];
    anchor->_kids[left] = node;

    NodeUpdateHeight(node);
    NodeUpdateHeight(anchor);

    return anchor;
}


#ifdef avl_debug
size_t TreeHeight(const avl_t _this)
{
    avl_const_thisify

    if (NULL == this->_root) return 0;

    return this->_root->_height;
}

int AvlRootValue(const avl_t _this)
{
    avl_const_thisify

    return *(int*)(this->_root->_data);
}

#endif


const avl_api_t Avl = {Create, CreateExt, Free, Insert, Remove, Find, Size, ForEach }; 
