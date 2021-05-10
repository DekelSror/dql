#include <stdlib.h>
#include <stdarg.h>

#include "table.h"
#include "vector.h"

#define table_thisify _table_t* this = _this;

typedef struct
{
    string_t _name;
    datatypes_e _type;
    int _index;
} column_t;

typedef struct
{
    vector_t _columns;
    vector_t _rows;
    unsigned _row_size;
    
} _table_t;

#include <stdio.h>

static unsigned RowSize(table_t _this)
{
    table_thisify

    unsigned vsize = 0;
    for (size_t i = 0; i < Vector.size(this->_columns); i++)
    {
        column_t* col = Vector.at(this->_columns, i);
        vsize += types_sizes[col->_type];
    }


    printf("row size is %u\n", vsize);
    return vsize;
}

static void SetMetaColumns(_table_t* this)
{
    // stuff like ingestion time, rownum
}

static table_t Create(string_t name, int user_schema_count, ...)
{
    _table_t* this = malloc(sizeof(*this));

    this->_columns = Vector.create(20, sizeof(column_t));

    SetMetaColumns(this);

    va_list args;
    va_start(args, user_schema_count);

    for (int i = 0; i < user_schema_count; i++)
    {
        string_t col_name = va_arg(args, string_t);
        datatypes_e col_dt = va_arg(args, datatypes_e);
        column_t col = {col_name, col_dt, i};
        printf("adding col %s type %d\n", String.chars(col_name), col_dt);
        Vector.push(this->_columns, &col);
    }
    
    this->_row_size = RowSize(this);
    this->_rows = Vector.create(20, this->_row_size);

    return this;
}

static void Free(table_t _this)
{
    table_thisify    
    
    Vector.free(this->_columns);
    this->_columns = NULL;
    Vector.free(this->_rows);
    this->_rows = NULL;

    free(this);

    this = NULL;
}

static int AddRow(table_t _this, void* row)
{
    table_thisify
    Vector.push(this->_rows, row);
    printf("rows after addtion: %u\n", Vector.size(this->_rows));

    return 0;
}

#include <unistd.h>


static void OutputRow(table_t _this, size_t row_num, int fd)
{
    table_thisify

    char row_buf[4096] = { 0 };
    unsigned row_offset = 0;

    char* cell = Vector.at(this->_rows, row_num);

    if (cell == NULL) return;

    for (size_t i = 0; i < Vector.size(this->_columns); i++)
    {
        column_t* col = Vector.at(this->_columns, i);
        printf("parsing col %s dt %u\n", String.chars(col->_name), col->_type);

        union 
        {
            double _num; string_t _str;
        } val;        

        switch (col->_type)
        {
        case number:
            val._num = *(double*)cell;
            sprintf(row_buf + row_offset, "%lf,", val._num);
            row_offset += 8;
            break;
        
        case string:
            val._str = cell;
            printf("parsing '%s' which is %lu long\n", String.chars(val._str), String.len(val._str));
            sprintf(row_buf + row_offset, "%s,", String.chars(val._str));
            row_offset += String.len(val._str);
            break;

        default:
            break;        
        }

        cell  += 8;
        ++row_offset; // for the comma
    }

    *(row_buf + row_offset) = '\n';
    ++row_offset;

    printf("row offset is %u\n\n", row_offset);

    write(fd, row_buf, row_offset);
}


const table_api_t Table = {Create, NULL, Free, AddRow, OutputRow, NULL};
