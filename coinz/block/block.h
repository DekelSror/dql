#ifndef dql_block_h
#define dql_block_h

#include "coinz_defs.h"

typedef void* block_t;

typedef struct 
{
    block_t(*create)(Sha512);    
    void(*free)(block_t);    
    //
    block_t(*add)(block_t);
} block_api_t;

extern const block_api_t Block;

#endif // dql_block_h
