#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "mtsq.h"

struct mtsq_s
{
        u_int32_t _first;
        u_int32_t _num_items;
        pthread_mutex_t _lock;
        u_int32_t _qsize;

        void* _queue[];
};

typedef struct mtsq_s mtsq_t;


#define mtsq_thisify mtsq_t* q = (mtsq_t*)_q;
#define mtsq_thisify_const const mtsq_t* q = (const mtsq_t*)_q;

//api decl
static queue_t Create(u_int32_t qsize);
static void Destroy(queue_t _q);

static int Enqueue(queue_t _q, void* element);
static void* Dequeue(queue_t _q);

static uint32_t Size(const queue_t _q) { mtsq_thisify_const return q->_num_items; }
static int Empty(const queue_t _q) { mtsq_thisify_const return 0 == q->_num_items; }

//class object
const queue_api_t MTSQ = { Create, Destroy, Enqueue, Dequeue, Size, Empty };

static void* Create(u_int32_t qsize)
{
        mtsq_t* q = malloc(sizeof(mtsq_t) + qsize * sizeof(void*));

        q->_first = 0;
        q->_num_items = 0;
        q->_qsize = qsize;
        int s_init = pthread_mutex_init(&q->_lock, NULL);

        return q;
}

/* adds element to back of queue, 0 = success */
static int Enqueue(void* _q, void* element)
{
        mtsq_thisify
        if (pthread_mutex_lock(&q->_lock)) return -1;
        if (q->_num_items >= q->_qsize)
        {
                pthread_mutex_unlock(&q->_lock);
                return 1;
        }
        
        q->_queue[(q->_first + q->_num_items) % q->_qsize] = element;
        ++q->_num_items;
        
        pthread_mutex_unlock(&q->_lock);
 
        return 0;
}

/* removes first item from locked queue and returns it */
static void* Dequeue(void* _q)
{
        mtsq_thisify        
        if (pthread_mutex_lock(&q->_lock)) return NULL;

        void* retval = NULL;
        if (!Empty(q))
        {
                retval = q->_queue[q->_first];
                q->_first = (q->_first + 1) % q->_qsize;
                --q->_num_items;
        }

        pthread_mutex_unlock(&q->_lock);
        return retval;
}

static void Destroy(void* _q)
{
        mtsq_thisify        
        pthread_mutex_destroy(&q->_lock);
        free(q);
        q = NULL;
        
}
