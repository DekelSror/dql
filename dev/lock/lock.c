
#include <signal.h>

#include "lock.h"

#define lock_thisify _lock_t* this = _this;

typedef struct
{
    sig_atomic_t _l;
} _lock_t;

static lock_t Create()
{

}

static void Free(lock_t _this);



static int Acquire(_lock_t* this)
{
    int res;

    __sync_lock_test_and_set(&(this->_l), res);

    asm("");

}

const lock_api_t Lock = {Create, Free}; 
