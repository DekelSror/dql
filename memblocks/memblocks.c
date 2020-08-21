

#include "memblocks.h"

typedef struct 
{
    size_t _block_size;
    size_t _num_blocks;
    long _next_block;

    char _memory[];

} _memblocks_t;

typedef union 
{
    long _next;
    _memblocks_t* _origin;
} mblock_t;


static size_t Calculate(size_t block_size, size_t num_blocks)
{
    return sizeof(_memblocks_t) + ((block_size + sizeof(long)) * num_blocks);
}


static memblocks_t Create(void* buf, size_t block_size, size_t num_blocks)
{
    _memblocks_t* pool = buf;

    pool->_block_size = block_size + sizeof(long);
    pool->_num_blocks = num_blocks;
    pool->_next_block = 0;

    long i = 0;
    for (; i < pool->_num_blocks - 1; i++)
    {
        mblock_t* block = pool->_memory + pool->_block_size * i;
        block->_next = i + 1;
    }

    mblock_t* block = pool->_memory + pool->_block_size * i;
    block->_next = -1;

    return pool;
}

static block_t GetBlock(memblocks_t _pool)
{
    _memblocks_t* pool = _pool;

    if (-1 == pool->_next_block) return NULL;

    mblock_t* block = pool->_memory + pool->_next_block * pool->_block_size;

    pool->_next_block = *(long*)block;

    block->_origin = pool;

    return (char*)block + sizeof(long);
}

static void FreeBlock(block_t block)
{
    mblock_t* meta_block = ((void**)block - 1);
    _memblocks_t* pool = meta_block->_origin;

    const long next = pool->_next_block;

    meta_block->_next = next;
    pool->_next_block = ((char*)meta_block - (char*)pool) / pool->_block_size;
}

static void Free(memblocks_t pool)
{

}



const memblocks_api_t Memblocks = { Create, Free, GetBlock, FreeBlock, Calculate };








