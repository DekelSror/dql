#include <stdlib.h>

#include "set_class.h"
#include "types.h"
#include "string_class.h"
#include "utils.h"
#include "table_shcema.h"

typedef struct table_field
{
    string_t* _name;
    basic_types_e _datatype;
    uint32_t _index;
} field_t;

struct table_schema
{
    const char* _name;
    set_t* _fields;
};

static table_schema_t* SchemaFromString(const char* _schema_string)
{
    uint32_t field_index = 0;

    string_t* schema_string = String.remove_whitespace(_schema_string);

    const char* schema_chars = String.chars(schema_string);

    table_schema_t* schema = malloc(sizeof(*schema));

    if(NULL == schema) return NULL;


    schema->_fields = Set.create();   

    while (*schema_chars)
    {
        string_t* name = String.create_until(schema_chars, ":");

        schema_chars += String.len(name);

        string_t* datatype_str = String.create_until(schema_chars, ",");

        basic_types_e datatype = Types.from_string(datatype_str);

        schema_chars += String.len(datatype_str);


        field_t* field = FieldCreate(name, datatype, field_index);
        
        if (NULL == field)
        {
            SchemaFree(schema);
            return NULL;
        }

        ++field_index;

        Set.add(schema->_fields, field);

        String.free(name);
        String.free(datatype_str);
    }
    
    return schema;
}

void SchemaFree(table_schema_t* schema)
{
    Set.free(schema->_fields);
    free(schema->_fields);
    schema->_fields = NULL;
    free(schema);
    schema = NULL;    
}

int SchemaExtend(table_schema_t* schema, table_schema_t* extension);



field_t* FieldCreate(const string_t* name, const string_t* datatype, uint32_t field_index)
{
    field_t* field = malloc(sizeof(*field));

    if (NULL == field) return NULL;

    field->_name = name;
    field->_datatype = datatype;
    field->_index = field_index;

    return field;
}