#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define heap_thisify _heap_t* this = _this;
#define heap_top ((size_t)1)
#define l_kid_idx(n) ((size_t)(2*n))
#define r_kid_idx(n) ((size_t)(2*n + 1))

typedef struct
{
    comparator_fn_t _cmp;
    size_t _capacity;
    size_t _size;
    void* _heap[];
} _heap_t;

/*
cmp - function to determine values relations
cmp(_a, _b) > 0 means _a should be above _b in the heap
*/
static heap_t Create(size_t heap_size, comparator_fn_t cmp)
{
    _heap_t* this = malloc(sizeof(*this) + (sizeof(void*) * (heap_size + heap_top)));

    this->_size = 0 + heap_top;
    this->_cmp = cmp;
    this->_capacity = heap_size;

    memset(this->_heap + heap_top, 0, heap_size * sizeof(void*));

    return this;
}


static int Find(heap_t _this, void* elem)
{
    heap_thisify

    if (this->_size == 0) return NULL;
    
    size_t index = heap_top;

    while (index < this->_size)
    {
        const int cmp_res = this->_cmp(elem, this->_heap[index]);

        if (0 == cmp_res)
        {
            return 1;
        }

        if (0 < cmp_res) // elem before heap[index]
        {
            break;
        }

        index *= 2;
    }

    index /= 2;

    while (index < this->_size)
    {
        const int cmp_res = this->_cmp(elem, this->_heap[index]);

        if (0 == cmp_res)
        {
            return 1;
        }

        ++index;
    }
    
    return 0;
}

static int Insert(heap_t _this, void* elem)
{
    heap_thisify

    if (this->_size - heap_top == this->_capacity) return 1;

    
    this->_heap[this->_size] = elem;

    size_t index = this->_size;

    while (index > heap_top)
    {
        const int cmp_res = this->_cmp(this->_heap[index], this->_heap[index / 2]);

        if (0 < cmp_res)
        {
            void* tmp = this->_heap[index];
            this->_heap[index] = this->_heap[index / 2];
            this->_heap[index / 2] = tmp;
        }

        index /= 2;
    }

    ++this->_size;
    return 0;
}

static void* Pop(heap_t _this)
{
    heap_thisify

// actually pop the thing
    if (this->_size == heap_top) return NULL;
    --this->_size;

    void* top_node = this->_heap[heap_top];

    this->_heap[heap_top] = this->_heap[this->_size];
    this->_heap[this->_size] = NULL;
    //if (this->_size == heap_top) return top_node; // last node out

// re-balance
    size_t index = heap_top;

    while (index < this->_size)
    {
        if (NULL == this->_heap[l_kid_idx(index)]) // [index] has no kids
        {
            break;
        }

        if (NULL == this->_heap[r_kid_idx(index)]) // one kid - always the first (left)
        {
            const int cmp_res = this->_cmp(this->_heap[index], this->_heap[l_kid_idx(index)]);

            if (cmp_res < 0) // l_kid should be above [index]
            {
                void* tmp = this->_heap[index];
                this->_heap[index] = this->_heap[l_kid_idx(index)];
                this->_heap[l_kid_idx(index)] = tmp;
            }

            break;
        }

        // both kids - find the larger one and cmp with [index]
        const int kid_cmp = this->_cmp(this->_heap[l_kid_idx(index)], this->_heap[r_kid_idx(index)]);
        const int side = kid_cmp > 0;

        const int cmp_res = this->_cmp(this->_heap[index], this->_heap[2 * index + side]);

        if (cmp_res < 0)
        {
            void* tmp = this->_heap[index];
            this->_heap[index] = this->_heap[2 * index + side];
            this->_heap[2 * index + side] = tmp;            
        }

        index *= 2;
    }


    return top_node;
}

static void* Top(heap_t _this)
{
    heap_thisify

    return this->_heap[heap_top];
}

static void Free(heap_t _this)
{
    heap_thisify

    memset(this->_heap, 0, this->_capacity * sizeof(void*));

    free(this);

    this = NULL;
}


const heap_api_t Heap = {Create, Free, Top, Insert, Pop, NULL};

