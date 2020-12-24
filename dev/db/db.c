

#include "hash.h"
#include "db.h"

#define db_thisify _db_t* this = _this;

typedef struct
{
    hash_t _tables;
} _db_t;



static db_t Create();
static void Free(db_t _this);


static table_t GetTable(db_t _this, string_t table_name)
{
    db_thisify

    return Hash.get(this->_tables, table_name);
}

const db_api_t Db = {Create, Free}; 
