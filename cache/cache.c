#include "cache.h"

#define cache_thisify _cache_t* this = _this;

typedef struct
{

} _cache_t;

static cache_t Create();
static void Free(cache_t _this);

const cache_api_t Cache = {Create, Free}; 
