#include <stdlib.h>
#include <string.h>


#include "block.h"

#define block_thisify _block_t* this = _this;

typedef struct
{
    unsigned char* _prev_hash;
    unsigned char* _hash;
} _block_t;

static block_t Create(unsigned char* prev_hash)
{
    _block_t* this = malloc(sizeof(*this));


    this->_prev_hash = prev_hash;
    this->_hash = NULL;    

    return this;
}

static void Free(block_t _this)
{
    free(_this);
}

const block_api_t Block = {Create, Free}; 
