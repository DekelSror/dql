#ifndef dql_iterator_class_h
#define dql_iterator_class_h

typedef struct _iterator_api
{
    void*(*get)(const void*);
    void*(*first)(const void* ds);
    void*(*last)(const void* ds);
    void*(*next)(void*);
    void*(*prev)(void*);
    void(*free)(void*);
} iterator_api_t;


#endif // dql_iterator_class_h