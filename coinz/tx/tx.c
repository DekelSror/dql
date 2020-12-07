#include <stdlib.h>
#include <time.h>

#include "asset.h"
#include "tx.h"


#define tx_thisify _tx_t* this = _this;

#define MAX_ASSETS (3)

typedef enum {confirmed} tx_status_e;

typedef struct
{
    asset_t _assets[MAX_ASSETS];
    time_t _bcast_time;
    tx_status_e _status;
} _tx_t;

static tx_t Create();
static void Free(tx_t _this);

const tx_api_t Tx = {Create, Free}; 
