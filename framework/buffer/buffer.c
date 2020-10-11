#include <stdlib.h>
#include "buffer.h"

static buffer_t* Create(unsigned size)
{
    buffer_t* this = malloc(sizeof(*this) + size);

    this->_size = size;
    this->_written_size = 0;

    return this;
}


static int Write(buffer_t* this, char* data, unsigned length)
{
    if (length > this->_size) return 1;

    memmove(this->_data, data, length);
    this->_written_size = length;

    return 0;
}

const buffer_api_t Buffer = {Create, Write}; 

