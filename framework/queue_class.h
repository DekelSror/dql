#ifndef dql_queue_adt_h
#define dql_queue_adt_h

#include <stddef.h>


typedef void* queue_t;

typedef struct _queue_api
{
    queue_t (*create)(u_int32_t);
    void (*free)(queue_t);

    int (*enqueue)(queue_t, void*);
    void* (*dequeue)(queue_t);

    u_int32_t (*size)(const queue_t);
    int (*empty)(const queue_t);
} queue_api_t;


#endif




