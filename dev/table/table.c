#include <stdlib.h>

#include "table.h"
#include "vector.h"


#define table_thisify _table_t* this = _this;

typedef enum { number, string, date, timespan } datatypes_e;

static const size_t types_sizes[] = { sizeof(double), sizeof(void*), sizeof(void*), sizeof(void*) };

typedef struct
{
    string_t _name;
    datatypes_e _type;
    int _index;
} column_t;

typedef struct
{
    vector_t _columns;
    
} _table_t;


static table_t Create()
{
    _table_t* this = malloc(sizeof(*this));

    this->_columns = Vector.create(20, sizeof(column_t));

    return this;
}


static void Free(table_t _this);

const table_api_t Table = {Create, Free}; 
