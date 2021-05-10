#ifndef dql_graph_h
#define dql_graph_h

typedef void* graph_t;
typedef void* vertex_t;

typedef struct 
{
    graph_t(*create)();
    void(*free)(graph_t);

    int(*is_adjacent)(graph_t graph, vertex_t x, vertex_t y); // tests whether there is an edge from the vertex x to the vertex y;
    void*(*neighbors)(graph_t graph, vertex_t x); // lists all vertices y such that there is an edge from the vertex x to the vertex y;
    int(*add_vertex)(graph_t graph, vertex_t x); // adds the vertex x, if it is not there;
    void*(*remove_vertex)(graph_t graph, vertex_t x); // removes the vertex x, if it is there;
    int(*add_edge)(graph_t graph, vertex_t x, vertex_t y); // adds the edge from the vertex x to the vertex y, if it is not there;
    void*(*remove_edge)(graph_t graph, vertex_t x, vertex_t y); // removes the edge from the vertex x to the vertex y, if it is there;
    void*(*get_vertex_value)(graph_t graph, vertex_t x); // returns the value associated with the vertex x;
    int(*set_vertex_value)(graph_t graph, vertex_t x, void* val); // sets the value associated with the vertex x to v.
    
    
    //Structures that associate values to the edges usually also provide :[1]
    void*(*get_edge_value)(graph_t graph, vertex_t x, vertex_t  y); // returns the value associated with the edge (x, y);
    int(*set_edge_value)(graph_t graph, vertex_t x, vertex_t  y, void* val); // sets the value associated with the edge (x, y) to v.
} graph_api_t;

extern const graph_api_t Graph;

#endif // dql_graph_h
