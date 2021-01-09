#ifndef dql_table_h
#define dql_table_h

#include <string_class.h>

typedef void* table_t;
typedef enum { number, string, date, timespan } datatypes_e;
static const size_t types_sizes[] = { sizeof(double), sizeof(string_t), sizeof(void*), sizeof(void*) };


typedef struct 
{
    // schema will be given as pairs of args [colName (string_t), colType (enum)]
    table_t(*create)(string_t table_name, int user_schema_count, ...);

    table_t(*load)(int fd);
    void(*free)(table_t);
    //
    int(*add_row)(table_t, void* row);
    void(*print_row)(const table_t, size_t row_num, int fd);
    table_t(*query)(const table_t, const string_t);
} table_api_t;

extern const table_api_t Table;



typedef struct 
{
    long id;
    string_t description;
    size_t amount;
} test_row_t;

#endif // dql_table_h
