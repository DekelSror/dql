#include "stdlib.h"

#include "asset.h"

#define asset_thisify _asset_t* this = _this;

typedef struct
{
    size_t _amount;
} _asset_t;

static asset_t Create();
static void Free(asset_t _this);

const asset_api_t Asset = {Create, Free}; 
