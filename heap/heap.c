#include <stdlib.h>
#include <string.h>

#include "heap.h"

#define heap_thisify _heap_t* this = _this;

typedef struct 
{
    comparator_fn_t _cmp;
    size_t _capacity;
    size_t _size;
    void* _heap[];
} _heap_t;


static _heap_t* Create(size_t capacity, comparator_fn_t cmp)
{
    const size_t heap_size = sizeof(_heap_t) + ((capacity + 1) * sizeof(void*));

    _heap_t* heap = malloc(heap_size);

    memset(heap, 0, heap_size);

    heap->_capacity = capacity;
    heap->_cmp = cmp;
    heap->_size = 1;

    return heap;
}

#define heap_top (1)
#define l_kid_idx(n) (2*n)
#define r_kid_idx(n) (2*n + 1)

static void HeapifyDown(_heap_t* this, void* elem, size_t idx)
{    
    if (this->_size >= this->_capacity) return;
    if (NULL == this->_heap[idx]) // found the place
    {
        this->_heap[idx] = elem;
        ++this->_size;

        return;
    }
    else
    {
        const int diff = this->_cmp(elem, this->_heap[idx]);

        if (diff < 0) // swap elem
        {
            void* tmp = this->_heap[idx];
            this->_heap[idx] = elem;
            elem = tmp;
        }

        size_t dive_idx = l_kid_idx(idx);

        if (NULL != this->_heap[l_kid_idx(idx)]
            && NULL == this->_heap[r_kid_idx(idx)])
        {
            dive_idx = r_kid_idx(idx);
        }

        HeapifyDown(this, elem, dive_idx);
    }
}


static int Insert(heap_t _this, void* elem)
{
    heap_thisify

    const size_t size_b4 = this->_size;
    HeapifyDown(this, elem, 1);
    
    return this->_size > size_b4 ? 0 : 1;
}

static void* Top(heap_t _this)
{
    heap_thisify

    void* retval = this->_heap[1];
    this->_heap[1] = NULL;

    void* elem = this->_heap[this->_size];
    this->_heap[this->_size] = NULL;

    --this->_size;

    size_t idx = this->_size / 2;

    while (idx)
    {

        if (NULL == this->_heap[idx]) // found it
        {

        }
        else
        {
            const int diff = this->_cmp(elem, this->_heap[idx]);

            if (diff >= 0) // swap
            {

            }

            idx /= 2;
        }
        
    }
    

    return retval;
}



const heap_api_t Heap = {Create, NULL, Insert, NULL};








