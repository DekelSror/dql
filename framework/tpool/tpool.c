#include <pthread.h> // phtread_t + API, mutex_t + API
#include <semaphore.h> // sem_t + API
#include <sys/sysinfo.h> // get_nprocs_conf
#include <stdlib.h> // malloc, free

#include "mtsq.h"
#include "tpool.h"

#define tpool_thisify _tpool_t* this = (_tpool_t*)_this;


#include <stdio.h>

typedef enum {WORK, HALT, KILL} tpool_states_t;

typedef struct tpool_s
{
        void* _tasks;
        volatile __sig_atomic_t _state;
        sem_t _pending;
        pthread_mutex_t _queue_lock;
        int _num_threads;
        pthread_t _threads[];
} _tpool_t;

typedef struct task_props_s
{
        void* (*_func)(void*);
        void* _arg;
        void* _collector;
} task_props_t;

// api

// num_threads 0 will start an efficient amount of threads for the system
// any other value will be used as is
static tpool_t TPoolCreate(unsigned _num_threads);
static void TPoolDestroy(tpool_t _this);
static void TPoolHalt(tpool_t _this);
static int TPoolAddTask(tpool_t _this, void*(*func)(void*), void* arg, void* collector);

// utils
static void* Worker(void* _pool);
static void* EmptyFunc(void* arg);

// class object
const tpool_api_t Tpool = {TPoolCreate, TPoolAddTask, TPoolHalt, TPoolDestroy};


// api implementation

static tpool_t TPoolCreate(unsigned nthreads_hint)
{
        _tpool_t* pool = NULL;
        int num_threads = nthreads_hint != 0u ? nthreads_hint : get_nprocs_conf() + 1;

        pool = malloc(sizeof(*pool) + sizeof(pthread_t) * num_threads);
        pool->_state = HALT;

        if (sem_init(&pool->_pending, 0, 0)) return NULL;
        pool->_tasks = MTSQ.create(TP00L_QUEUE_SIZE);

        if (NULL == pool->_tasks)
        {
                TPoolDestroy(pool);
                return NULL;
        }

        for (int i = 0; i < num_threads; i++)
        {                
                if (pthread_create(pool->_threads + i, NULL, Worker, pool))
                {
                        TPoolDestroy(pool);
                        return NULL;
                }
        }
        
        pool->_num_threads = num_threads;
        pool->_state = WORK;

        return pool;
}

static int TPoolAddTask(tpool_t _this, void*(*func)(void*), void* arg, void* collector)
{
        tpool_thisify

        task_props_t* props = malloc(sizeof(*props));

        props->_arg = arg;
        props->_collector = collector;
        props->_func = func;

        int res = MTSQ.enqueue(this->_tasks, props);

        if (0 == res) sem_post(&this->_pending);

        return res;
}

static void TPoolHalt(tpool_t _this)
{        
        tpool_thisify
        __sync_lock_test_and_set(&this->_state, HALT);
}

static void* EmptyFunc(void* arg)
{
        (void)arg;
        return NULL;
}

static void TPoolDestroy(tpool_t _this)
{
        tpool_thisify
        this->_state = KILL;

        static task_props_t apple = {EmptyFunc, NULL, NULL};

        for (u_int16_t i = 0; i < this->_num_threads; i++)
        {
                if (0 == MTSQ.enqueue(this->_tasks, &apple))
                {
                        sem_post(&this->_pending);
                }
        }

        for (u_int16_t i = 0; i < this->_num_threads; i++)
        {
                pthread_join(this->_threads[i], NULL);
        }        

        // manual free tasks queue
        while(!MTSQ.empty(this->_tasks))
        {
                task_props_t* to_delete = (task_props_t*)MTSQ.dequeue(this->_tasks);

                if (EmptyFunc != to_delete->_func)
                {
                        free(to_delete);
                        to_delete = NULL;
                }
        }

        MTSQ.free(this->_tasks);
        this->_tasks = NULL;

        sem_destroy(&(this->_pending));
        free(this);
        this = NULL;
}

static void* Worker(void* _pool)
{
        _tpool_t* pool = _pool;

        while(pool->_state == WORK)
        {
                sem_wait(&pool->_pending);
                if (KILL == pool->_state) return NULL;
                if (HALT == pool->_state) continue;

                task_props_t* task = (task_props_t*)MTSQ.dequeue(pool->_tasks);

                if (NULL != task) 
                {
                        void* res = task->_func(task->_arg);

                        if (NULL != task->_collector)
                        {
                                task->_collector = res;
                        }

                        free(task);
                        task = NULL;
                }
        }
}


