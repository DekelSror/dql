#include <time.h>

#include "defs.h"
#include "hash.h"
#include "queue_class.h"

typedef struct
{
    size_t _stock_id;
    unsigned _quantity;
} stock_stock_t;

typedef struct 
{
    queue_t _pending_offers;
    hash_t _inventory;
    size_t _id;
} broker__t;

typedef void* broker_t;

static broker_t Create()
{
    
}




