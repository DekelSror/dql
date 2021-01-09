#include "stdlib.h"
#include "string.h"
#include "vector.h"

#define vector_thisify _vector_t* this = _this;
#define vector_const_thisify const _vector_t* this = _this;

typedef struct
{
    int _capacity;
    int _size;
    int _element_size;
    char _data[];
} _vector_t;

static vector_t Create(int initial_size, int element_size)
{
    _vector_t* this = malloc(sizeof(*this) + (initial_size * element_size));
    this->_capacity = initial_size;
    this->_element_size = element_size;
    this->_size = 0;
    memset(this->_data, 0, element_size * initial_size);

    return this;
}

static void Free(vector_t _this)
{
    free(_this);
}

static int Push(vector_t _this, void* elem)
{
    vector_thisify
    if (this->_size == this->_capacity) return 1;

    memmove(this->_data + (this->_size * this->_element_size), elem, this->_element_size);

    ++this->_size;

    return 0;
}

static void* Pop(vector_t _this)
{
    vector_thisify

    if (0 == this->_size) return NULL;

    --this->_size;
    void* rv = this->_data + (this->_size * this->_element_size);

    return rv;
}

static void* Top(const vector_t _this)
{
    vector_const_thisify

    if (0 == this->_size) return NULL;

    return this->_data + ((this->_size - 1) * this->_element_size);
}

static void* At(const vector_t _this, int index)
{
    vector_thisify

    if (index > this->_size || 0 == this->_size) return NULL;

    return this->_data + (index * this->_element_size);
}

static void* Remove(vector_t _this, int index)
{
    vector_thisify

    if (index > this->_size || 0 == this->_size) return NULL;

    void* rv = this->_data + (this->_size * this->_element_size);

    // if index == size this moves nothing
    memmove(this->_data + (index * this->_element_size),
        this->_data + ((index + 1) * this->_element_size),
        this->_element_size * (this->_size - index));

    return rv;
}

static unsigned Size(const vector_t _this)
{
    vector_const_thisify

    return this->_size;
}

static void ForEach(vector_t _this, void(*fn)(void*, void*), void* fn_arg)
{
    vector_thisify

    for (size_t i = 0; i < this->_size; i++)
    {
        fn(Vector.at(this, i), fn_arg);
    }    
}

const vector_api_t Vector = {Create, Free, Push, Pop, Top, At, Remove, Size, ForEach}; 
