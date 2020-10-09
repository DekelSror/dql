#ifndef dql_framework_h
#define dql_framework_h

#include <time.h>
#include <semaphore.h>
#include <pthread.h>

#include "tpool.h"
#include "vlist.h"
#include "event.h"
#include "queue_class.h"
#include "strings_man.h"



// asynchronously runs func on a separate thread. 
// collector = func(arg), although arg and collector can be NULL
void Task(void*(*func)(void*), void* arg, void* collector);
void FrameworkInit(void);
void FrameworkCleanup(void);
void* EventRegister(const char*);
void* EventAddListener(event_t , void(*)(void));
void TriggerEvent(event_t e);


string_t* GetString(const char* str);
void FreeString(string_t* str);




#endif // dql_framework_h