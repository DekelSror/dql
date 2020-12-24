#ifndef dql_table_h
#define dql_table_h

typedef void* table_t;

#include <string_class.h>


typedef struct 
{
    table_t(*create)(string_t table_name);
    table_t(*load)(int fd);
    void(*free)(table_t);
    //
    int(*add_rows)(table_t, ...);
    table_t(*query)(const table_t, const string_t);
} table_api_t;

extern const table_api_t Table;

#endif // dql_table_h
