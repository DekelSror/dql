#ifndef dql_vector_h
#define dql_vector_h

typedef void* vector_t;

typedef struct 
{

    vector_t(*create)(int initial_size, int element_size);
    void(*free)(vector_t);

    // adds to end of vector (higher indices)
    int(*push)(vector_t, void* elem);

    // removes last (highset index) element
    void*(*pop)(vector_t);

    // element at [0]
    void*(*top)(const vector_t);

    // element at [index]
    void*(*at)(vector_t, int index);

    // remove element by index (expansive)
    void*(*remove)(vector_t, int index);

} vector_api_t;

extern const vector_api_t Vector;

#endif // dql_vector_h
