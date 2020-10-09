#ifndef dql_memblocks_h
#define dql_memblocks_h

#include <stddef.h>

typedef void* memblocks_t;
typedef void* block_t;

typedef struct 
{
    //
    memblocks_t(*create)(void* buf, size_t block_size, size_t num_blocks);
    void(*free)(memblocks_t);
    //
    block_t(*get_block)(memblocks_t);
    void(*free_block)(block_t);
    
    size_t(*reqired_buf_size)(size_t block_size, size_t num_blocks);

} memblocks_api_t;


extern const memblocks_api_t Memblocks;

#endif  // dql_memblocks_h

