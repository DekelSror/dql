#include "graph.h"
#include "vlist.h"

#define graph_thisify _graph_t* this = _this;

typedef struct
{

} _graph_t;

typedef struct 
{
    void* _data;
    vlist_t _edges;
} _vertex_t;

static graph_t Create() 
{

}

static void Free(graph_t _this)
{

}


const graph_api_t Graph = {Create, Free}; 
