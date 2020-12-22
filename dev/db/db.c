#include "db.h"

#define db_thisify _db_t* this = _this;

typedef struct
{

} _db_t;

static db_t Create();
static void Free(db_t _this);

const db_api_t Db = {Create, Free}; 
