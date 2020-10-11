#ifndef dql_fsq_h
#define dql_fsq_h

typedef void* fsq_t;




typedef struct 
{
    fsq_t(*create)(unsigned);
    void(*free)(fsq_t);
    int(*enq)(fsq_t, void*);
    void*(*deq)(fsq_t);
} fsq_api_t;


extern const fsq_api_t FSQ;



#endif // dql_fsq_h
