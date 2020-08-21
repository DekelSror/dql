
typedef struct table_field table_field_t;

typedef struct table_schema table_schema_t;


typedef struct _table_shcema_api
{
    table_schema_t*(*create)(const char* schema_string);
    void (*free)(table_schema_t*);
    //
    int (*add_field)(table_schema_t* schema, const char* name, basic_types_e datatype);
    table_schema_t*(*extend)(table_field_t* extra_cols);
    table_schema_t*(*project)(table_schema_t* new_schema);
} table_shcema_api_t;

extern const table_shcema_api_t TableSchema;
