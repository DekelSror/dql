#ifndef dql_lock_h
#define dql_lock_h

typedef void* lock_t;

typedef struct 
{
    lock_t(*create)();
    void(*free)(lock_t);

    int(*acquire)(lock_t);
    void(*release)(lock_t);
} lock_api_t;

extern const lock_api_t Lock;

#endif // dql_lock_h
