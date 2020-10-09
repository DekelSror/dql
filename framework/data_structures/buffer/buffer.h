#ifndef dql_buffer_h
#define dql_buffer_h


typedef struct 
{
    unsigned _size;
    unsigned _written_size;
    char _data[];
} buffer_t;


typedef struct 
{
    buffer_t*(*create)(unsigned, unsigned);
    int(*write)(buffer_t*, char*, unsigned);    
    
} buffer_api_t;



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



#endif // dql_buffer_h
