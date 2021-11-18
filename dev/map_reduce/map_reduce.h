#ifndef dql_map_reduce_h
#define dql_map_reduce_h

typedef void* map_reduce_t;

typedef struct 
{
    map_reduce_t(*create)();
    void(*free)(map_reduce_t);
} map_reduce_api_t;

extern const map_reduce_api_t MapReduce;

#endif // dql_map_reduce_h
