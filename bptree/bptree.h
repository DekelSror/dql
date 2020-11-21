#ifndef dql_bptree_h
#define dql_bptree_h

typedef void* bptree_t;

typedef int(*cmp_fn)(const void*, const void*);
typedef void*(*alloc_node_fn)(void);


#define bptree_b (5)
#define bptree_nkids (bptree_b)
#define bptree_nkeys (bptree_b - 1)

typedef struct 
{
    bptree_t(*create)(cmp_fn cmp, alloc_node_fn alloc);
    bptree_t(*find)(bptree_t, void*);
    
} bptree_api_t;


extern const bptree_api_t Bptree;



#endif // dql_bptree_h
