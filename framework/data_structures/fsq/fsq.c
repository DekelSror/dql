#include <string.h>
#include <stdlib.h>

#include "fsq.h"

#define fsq_thisify _fsq_t* this = _this;

typedef struct
{
    unsigned _capacity;
    unsigned _first;
    unsigned _size;
    void* _queue[];
} _fsq_t;


static fsq_t Create(unsigned capacity)
{
    _fsq_t* this = malloc(sizeof(*this) + (sizeof(void*) * capacity));

    this->_capacity = capacity;
    this->_first = 0;
    this->_size = 0;

    memset(this->_queue, 0, sizeof(void*) * capacity);

    return this;
}

static void Free(fsq_t _this)
{
    free(_this);

    _this = NULL;
}

static int Enqueue(fsq_t _this, void* elem)
{
    fsq_thisify

    if (this->_size == this->_capacity) return 1;

    const unsigned i_enq = (this->_first + this->_size) % this->_capacity;

    this->_queue[i_enq] = elem;

    ++this->_size;

    return 0;
}


static void* Dequeue(fsq_t _this)
{
    fsq_thisify

    if (0 == this->_size) return NULL;

    void* rv = this->_queue[this->_first++];

    this->_first %= this->_capacity;
    
    --this->_size;

    return rv;
}

const fsq_api_t FSQ = {Create, Free, Enqueue, Dequeue}; 
