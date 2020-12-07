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
    buffer_t*(*create)(unsigned);
    int(*write)(buffer_t*, char*, unsigned);    
    
} buffer_api_t;

extern const buffer_api_t Buffer;


#endif // dql_buffer_h
