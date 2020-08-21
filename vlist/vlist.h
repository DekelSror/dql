#ifndef dql_vlist_h
#define dql_vlist_h

#include <stddef.h>

typedef void* vlist_t;

typedef struct 
{
    vlist_t(*create)(void);
    // vlist_t(*create_on)(void*, size_t);    
    void(*free)(vlist_t);
    int(*add)(vlist_t, void*);
    int(*remove)(vlist_t, void*);
    void*(*find)(vlist_t, const void*);
    int(*for_each)(vlist_t, void(*)(void*, void*), void*);

} vlist_api_t;

extern const vlist_api_t Vlist;


#endif //dql_vlist_h
