#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "event.h"

#define event_thisify _event_t* e = (_event_t*)_e;

typedef struct _event
{
    char* _name;
    vlist_t _listeners;
} _event_t;

static event_t Create(const char* name)
{
    _event_t* e = malloc(sizeof(*e));

    e->_listeners = Vlist.create();
    e->_name = (char*)name;

    return e;
}

typedef struct 
{
    void(*_dispatch)(void);
    int _active;
} listener_t;


static listener_id_t Subscribe(event_t _e, void(*dispatch)(void))
{
    event_thisify

    listener_t* l = malloc(sizeof(*l));
    l->_dispatch = dispatch;
    l->_active = 1;

    Vlist.add(e->_listeners, l);

    return (listener_id_t)l;
}

static void Unsubscribe(event_t _e, listener_id_t listener)
{
    event_thisify
    listener_t* l = Vlist.find(e->_listeners, listener);
    l->_active = 0;
}

static void Fire(void* listener, void* arg)
{
    (void)arg;
    
    listener_t* l = (listener_t*)listener;

    if (l->_active) l->_dispatch();
}

static void Free(event_t _e)
{
    event_thisify
    Vlist.free(e->_listeners);
    e->_listeners = NULL;

    free(e);
    e = NULL;
}

static void Trigger(event_t _e)
{
    event_thisify
    Vlist.for_each(e->_listeners, Fire, NULL);
}

const event_api_t Event = { Create, Free, Subscribe, Unsubscribe, Trigger };
