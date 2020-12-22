
#ifndef dql_db_h
#define dql_db_h

typedef void* db_t;


struct table_t;
struct table_index_t;


typedef struct 
{
    db_t(*create)();
    void(*free)(db_t);
    //
    int(*add_table)(db_t, ...);
    int(*ingest_data)(db_t, ...);

    void*(*query_data)(db_t, ...);
    

} db_api_t;

extern const db_api_t Db;

#endif // dql_db_h
