#include <mqueue.h>
#include <stddef.h>

#include "msg_queue.h"

#define msg_queue_thisify _msg_queue_t* this = _this;
#define const_msg_queue_thisify const _msg_queue_t* this = _this;

typedef struct 
{
    mqd_t _mq;
    struct mq_attr _attrs;
    char _current_msg[];
} _msg_queue_t;

static msg_queue_t Create(const char* path, long capacity, long msg_size)
{

    _msg_queue_t* this = malloc(sizeof(*this) + msg_size);

    this->_attrs.mq_flags = 0;
    this->_attrs.mq_maxmsg = capacity;
    this->_attrs.mq_msgsize = msg_size;
    
    this->_mq = mq_open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &this->_attrs);

    return this;
}

static void UpdateAttrs(const _msg_queue_t* this)
{
    mq_getattr(this->_mq, &(this->_attrs));
}

static void Free(msg_queue_t _this)
{
    msg_queue_thisify
    mq_close(this->_mq);
    free(this);

    this = NULL;
}

static int Enqueue(msg_queue_t _this, const char* msg)
{
    msg_queue_thisify

    return mq_send(this->_mq, msg, this->_attrs.mq_msgsize, 0);
}

static void* Dequeue(msg_queue_t _this)
{
    msg_queue_thisify

    memset(this->_current_msg, 0, this->_attrs.mq_msgsize);

    ssize_t res = mq_receive(this->_mq, this->_current_msg, this->_attrs.mq_msgsize, NULL);

    return this->_current_msg;
}

static unsigned Count(const msg_queue_t _this)
{
    const_msg_queue_thisify

    UpdateAttrs(this);

    return this->_attrs.mq_curmsgs;
}

static int IsEmpty(const msg_queue_t _this)
{
    const_msg_queue_thisify

    UpdateAttrs(this);

    return this->_attrs.mq_curmsgs == 0;
}

const mqueue_api_t MsgQueue = {Create, Free, Enqueue, Dequeue, Count, IsEmpty}; 
