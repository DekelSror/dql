
#ifndef dql_db_h
#define dql_db_h


#include "string_class.h"
#include "vector.h"
#include "table.h"

typedef void* db_t;

struct table_t;
struct table_index_t;


typedef struct 
{
    db_t(*create)(string_t name, ...);
    void(*free)(db_t);
    //
    table_t(*add_table)(db_t, string_t name, vector_t user_schema);
    table_t(*get_table)(db_t, string_t name);
    int(*ingest_data)(db_t, ...);

    void*(*query_data)(db_t, ...);
    

} db_api_t;

extern const db_api_t Db;

#endif // dql_db_h
