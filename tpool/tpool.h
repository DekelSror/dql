#ifndef THREAD_P00L_H
#define THREAD_P00L_H

#ifndef TP00L_QUEUE_SIZE
#define TP00L_QUEUE_SIZE (50)
#endif

typedef void* tpool_t;
typedef struct pool_task_s pool_task_t;

typedef struct _tpool_api {
        tpool_t(*create)(void);
        int (*add_task)(tpool_t _this, void*(*func)(void*), void* arg, void* collector);
        void (*halt)(tpool_t);
        void(*free)(tpool_t);
} tpool_api_t;

extern const tpool_api_t Tpool;

#endif // THREAD_P00L_H
