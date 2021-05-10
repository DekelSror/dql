#ifndef dql_mqueue_h
#define dql_mqueue_h

typedef void* msg_queue_t;

typedef struct 
{
    msg_queue_t(*create)();
    void(*free)(msg_queue_t);

    int(*enqueue)(msg_queue_t, const char*);
    void*(*deueue)(msg_queue_t);

    unsigned(*count)(const msg_queue_t);
    int(*is_empty)(const msg_queue_t);
} mqueue_api_t;

extern const mqueue_api_t MsgQueue;

#endif // dql_mqueue_h
