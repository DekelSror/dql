#ifndef dql_cache_h
#define dql_cache_h

typedef void* cache_t;

typedef struct 
{
    cache_t(*create)();
    void(*free)(cache_t);
} cache_api_t;

extern const cache_api_t Cache;

#endif // dql_cache_h
