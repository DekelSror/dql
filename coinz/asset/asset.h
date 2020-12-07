#ifndef dql_asset_h
#define dql_asset_h

typedef void* asset_t;

typedef struct 
{
    asset_t(*create)();
    void(*free)(asset_t);
} asset_api_t;

extern const asset_api_t Asset;

#endif // dql_asset_h
