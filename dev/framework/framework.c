#include <stdlib.h>
#include <unistd.h>


#include "mtsq.h"
#include "framework.h"
#include "string_class.h"

typedef struct _dql_framework
{
    tpool_t _tpool;

    vlist_t _events;
    volatile __sig_atomic_t _events_state;
    void* _triggers;
    sem_t _event_trigger;
    strings_t _strings;
    pthread_t _event_loop;
    
        
} dql_framework_t;


void* EventLoop(void* arg);


static dql_framework_t app_framework;

static int is_module_initialized = 0;

typedef enum { Loop, Halt } events_state_e;

// lifecycle

void FrameworkInit(void)
{
    if (!is_module_initialized)
    {
        app_framework._tpool = Tpool.create();

        app_framework._events = Vlist.create();
        app_framework._events_state = Halt;
        app_framework._triggers = MTSQ.create(10);
        
        app_framework._strings = Strings.create();

        sem_init(&(app_framework._event_trigger), 0, 0);

        // pthread_create(&(app_framework._event_loop), NULL, EventLoop, NULL);

        is_module_initialized = 1;
    }
}


#include <stdio.h>

void FrameworkCleanup(void)
{    
    app_framework._events_state = Halt;
    Tpool.halt(app_framework._tpool);

    printf("stopped tpool\n");

    Vlist.free(app_framework._events);
    printf("freed events\n");
    MTSQ.free(app_framework._triggers);
    printf("freed triggers\n");
    Strings.free(app_framework._strings);
    printf("freed strings\n");

    Tpool.free(app_framework._tpool);
    printf("freed tpool\n");
    // pthread_join(app_framework._event_loop, NULL);
    // printf("joined eventloop thread\n");
}

// thread pool

void Task(void*(*func)(void*), void* arg, void* collector)
{
    const int res = Tpool.add_task(app_framework._tpool, func, arg, collector);
    (void)res;
}

// events

void* EventLoop(void* arg)
{
    (void)arg;
    
    while (Loop == &app_framework._events_state)
    {
        sem_wait(&(app_framework._event_trigger));

        event_t event = MTSQ.dequeue(app_framework._triggers);

        Event.trigger(event);
    }

    return NULL;
}

void* EventRegister(const char* name)
{
    event_t e = Event.create(name);
    Vlist.add(app_framework._events, e);

    return e;
}

void* EventAddListener(event_t e, void(*dispatch)(void))
{
    void* listed_event = Vlist.find(app_framework._events, e);
    if (NULL == listed_event) return NULL;

    return Event.subscribe(listed_event, dispatch);
}

void TriggerEvent(event_t e)
{
    MTSQ.enqueue(app_framework._triggers, e);
    sem_post(&(app_framework._event_trigger));
}

// strings

string_t* GetString(const char* str)
{
    return Strings.get(app_framework._strings, str);
}

void FreeString(string_t* str)
{
    Strings.dispose(app_framework._strings, str);
}








