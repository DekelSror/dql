#include <stdlib.h>


#define declare_queue(type) \
typedef struct \
{ \
    unsigned _capacity; \
    unsigned _first; \
    unsigned _size; \
    type _queue[]; \
} type##_queue; 


#define queue_api(type) \
typedef struct \
{ \
    type##_queue*(*create)(unsigned capacity); \
    void(*free)(type##_queue* q); \
    int(*enq)(type##_queue* q, type elem); \
    type(*deq)(type##_queue* q); \
} type##_queue_api; \
const type##_queue_api type##Queue = { type##QueueCreate, type##QueueFree, type##Enqueue, type##Dequeue };



#define queue_ctor(type) \
static type##_queue* type##QueueCreate(unsigned capacity) \
{ \
    type##_queue* q = (type##_queue*)malloc(sizeof(*q) + (sizeof(type) * capacity)); \
    if (q != NULL) \
    { \
        q->_capacity = capacity; \
        q->_size = 0; \
        q->_first = 0; \
    } \
    return q; \
}

#define queue_dtor(type) \
static void type##QueueFree(type##_queue* q) \
{ \
    free(q); \
    q = NULL; \
}

#define queue_enq(type) \
static int type##Enqueue(type##_queue* q, type elem) \
{ \
    if (q->_size == q->_capacity) return 1; \
    const unsigned i_enq = (q->_first + q->_size) % q->_capacity; \
    q->_queue[i_enq] = elem; \
    ++q->_size; \
    return 0; \
}

#define queue_deq(type) \
static type type##Dequeue(type##_queue* q) \
{ \
    if (0 == q->_size) return (type)NULL; \
    type rv = q->_queue[q->_first++]; \
    q->_first %= q->_capacity; \
    --q->_size; \
    return rv; \
}


#define make_queue(type) \
declare_queue(type) \
queue_ctor(type) \
queue_dtor(type) \
queue_enq(type) \
queue_deq(type) \
queue_api(type) \





