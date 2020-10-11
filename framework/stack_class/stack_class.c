#include <stdlib.h>

#include "stack_class.h"

typedef struct
{
    size_t _top;
    size_t _capacity;
    void* _stack[];
} _stack_t;

#define stack_thisify _stack_t* this = (_stack_t*)_this;

// fward decl
int IsEmpty(const void* _this);

stack_t Create(size_t capacity)
{
    _stack_t* this = malloc(sizeof(*this) + capacity * sizeof(void*));

    this->_capacity = capacity;
    this->_top = 0;

    return this;
}

void Free(void* _this)
{
    stack_thisify
    free(this);

    this = NULL;
}

//
int Push(void* _this, void* elem)
{
    stack_thisify

    if (this->_top >= this->_capacity) return 1;

    this->_stack[this->_top++] = elem;

    return 0;
}

void* Pop(void* _this)
{
    stack_thisify

    if (IsEmpty(this)) return NULL;
    
    return this->_stack[--this->_top];
}

size_t Size(const void* _this)
{
    stack_thisify

    return this->_top;
}

int IsEmpty(const void* _this)
{
    stack_thisify
    
    return this->_top == 0;
}

const stack_api_t Stack = {Create, Free, Push, Pop, Size, IsEmpty};
// const stack_api_t* Stack = &_Stack;





