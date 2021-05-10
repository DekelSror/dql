#ifndef dql_managed_scope_h
#define dql_managed_scope_h

typedef void* api_t;

typedef struct 
{
    void(*init)();
    void(*cleanup)();

    void(*start)();
    void(*end)();
    void*(*declare_obj)(api_t);
} managed_scope_api_t;

extern const managed_scope_api_t Managed_scope;

#endif // dql_managed_scope_h
