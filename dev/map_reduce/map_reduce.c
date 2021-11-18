#include "map_reduce.h"

#define map_reduce_thisify _map_reduce_t* this = _this;


typedef void (*map_fn)(void* src, void *dest, void* arg);




typedef struct
{

} _map_reduce_t;

static map_reduce_t Create() {}
static void Free(map_reduce_t _this) {}





const map_reduce_api_t Map_reduce = {Create, Free}; 
