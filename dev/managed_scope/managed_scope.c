#include <string.h>


#include "array_templates.h"
#include "managed_scope.h"

#define managed_scope_thisify _managed_scope_t* this = _this;

#define scope_size (256)

typedef void(*dtor)(void*);
typedef void*(*ctor)();

typedef struct
{
    ctor _ctor;
    dtor _dtor;
} _api_t;

typedef struct
{
    void* _obj;
    _api_t* _api;
} object_t;

declare_array(object_t);


static object_t_arr* scoped_objects = NULL;
static unsigned objcount = 0;
static int is_initialized = 0;

static void Init(void)
{
    if (!is_initialized)
    {
        scoped_objects = object_tArrayCreate(scope_size);
        is_initialized = 1;
    }
}

static void Cleanup(void)
{
    if (is_initialized)
    {
        free(scoped_objects);
        is_initialized = 0;
    }
}

static void Start(void)
{
    memset(scoped_objects, 0, scope_size * sizeof(object_t));
}

static void End(void)
{
    for (size_t i = 0; i < objcount; i++)
    {
        scoped_objects->_at[i]._api->_dtor(scoped_objects->_at[i]._obj);
    }

    objcount = 0;
}


// what about ctor args?????
static void* DeclareObject(_api_t* api)
{
    if (objcount == scope_size) return NULL;

    void* obj = api->_ctor();

    if (obj != NULL)
    {
        scoped_objects->_at[objcount]._api = api;
        scoped_objects->_at[objcount]._obj = obj;
        
        ++objcount;
    }

    return obj;
}

const managed_scope_api_t ManagedScope = {Init, Cleanup, Start, End, DeclareObject}; 
