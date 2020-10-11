#ifndef dql_events_module_h
#define dql_events_module_h

#include <pthread.h>
#include <semaphore.h>

#include "vlist.h"

typedef void* event_t;


typedef void(*callback_fn)(void*);
typedef void* listener_id_t;

typedef struct 
{
    event_t(*create)(const char*);
    void(*free)(event_t);
    listener_id_t (*subscribe)(event_t, void(*)(void));
    void(*unsubscribe)(event_t, listener_id_t);
    void(*trigger)(event_t);
} event_api_t;

extern const event_api_t Event;


#endif // dql_events_module_h