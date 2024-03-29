#include <stdlib.h> // 
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "memblocks.h" // to a/de llocate offers w/o syscalls
#include "heap.h" // offers
#include "fsq.h" // deals
#include "defs.h" // stex defs
#include "msg_queue.h"

#define min(a, b) (((a) >= (b)) ? (a) : (b))

static void init(void);
static void cleanup(void);
static void Run(void);
static void StopMarket(void);

// low asks come first
static int CompareAsks(const void* a , const void* b);
// high bids come first
static int CompareBids(const void* a , const void* b);

static deal_t* DealCreate(size_t stock_id, unsigned value, unsigned quantity);
static void PrintDeal(void* _deal, void* _arg);

static void* market_pool_buffer = NULL;
static memblocks_t market_pool = NULL;
static heap_t* market[num_sides] = { 0 }; 
static fsq_t deals = NULL;
static msg_queue_t offers_mq;
static msg_queue_t values_mq;
static volatile __sig_atomic_t market_live = 1;

static void GetOffer(void);
static void Match(void);
static void RemoveTopOffer(order_sides_e side);
static void ResolveDeal(void);

static void* TaskRun(void* _arg)
{
    (void)_arg;
    Run();

    return NULL;
}

int main(void)
{
    pthread_t engine_thread;

    init();

    const int engine_thread_rc = pthread_create(&engine_thread, NULL, TaskRun, NULL);

    printf("initialized all and gonna run!\n");

    char input_buf[8] = { 0 };

    while (1)
    {
        memset(input_buf, 0, 8);
        scanf("%s", input_buf);

        if (0 == strncmp("quit", input_buf, 4))
        {
            printf("well\n\n");
            StopMarket();
            break;
        }
    }

    pthread_join(engine_thread, (void**)NULL);
    cleanup();
    return 0;
}



static void init(void)
{
    market_pool_buffer = malloc(Memblocks.reqired_buf_size(sizeof(offer_t), 100));
    market_pool = Memblocks.create(market_pool_buffer, sizeof(offer_t), 100);
    
    market[ask] = Heap.create(100, CompareAsks);
    market[bid] = Heap.create(100, CompareBids);

    deals = FSQ.create(100);

    offers_mq = MsgQueue.create("/stex_offers_mq", 100, sizeof(offer_t));
    values_mq = MsgQueue.create("/stex_values_mq", 100, sizeof(value_update_t));

    printf("got dat mq fd %d errno %d\n", offers_mq, errno);
}

static void Run(void)
{
    while (market_live)
    {
        GetOffer();
        Match();
        ResolveDeal();
    }
}

static void StopMarket(void)
{
    __sync_lock_test_and_set(&market_live, 0);
}

static void cleanup(void)
{
    Memblocks.free(market_pool_buffer);
    free(market_pool_buffer);

    MsgQueue.free(offers_mq);
    MsgQueue.free(values_mq);
    
    Heap.free(market[ask]);
    Heap.free(market[bid]);
    FSQ.free(deals);
}

static void GetOffer(void)
{
    const char* res = MsgQueue.deueue(offers_mq);
    offer_t* copy = Memblocks.get_block(market_pool);
    memmove(copy, res, sizeof(offer_t));
    Heap.insert(market[copy->_side], copy);
}

static void Match(void)
{
    offer_t* lowest_ask = Heap.top(market[ask]);
    offer_t* highest_bid = Heap.top(market[bid]);

    if (NULL != lowest_ask && NULL != highest_bid)
    {
        if (highest_bid->_value >= lowest_ask->_value)
        {
            unsigned deal_qty = min(highest_bid->_quantity, lowest_ask->_quantity);
            deal_t* deal = DealCreate(highest_bid->_stock_id, lowest_ask->_value, deal_qty);
            FSQ.enq(deals, deal);

            if (highest_bid->_quantity > lowest_ask->_quantity)
            {
                highest_bid->_quantity -= deal_qty;
                RemoveTopOffer(ask);
            }
            else if (highest_bid->_quantity < lowest_ask->_quantity)
            {
                lowest_ask->_quantity -= deal_qty;
                RemoveTopOffer(bid);
            }
            else
            {
                RemoveTopOffer(ask);
                RemoveTopOffer(bid);
            }
        }
    }
}

static void ResolveDeal(void)
{
    deal_t* deal;
    if (NULL != (deal = FSQ.deq(deals)))
    {
        const value_update_t update = { deal->_stock_id, deal->_value, time(NULL) };

        MsgQueue.enqueue(values_mq, (const char*)&update);

        printf("value update!\n");
    }
}

static void RemoveTopOffer(order_sides_e side)
{
    Memblocks.free_block(Heap.pop(market[side]));
}

static deal_t* DealCreate(size_t stock_id, unsigned value, unsigned quantity)
{
        deal_t* deal = malloc(sizeof(deal_t));

        if (NULL != deal)
        {            
            deal->_value = value;
            deal->_quantity = quantity;
            deal->_stock_id = stock_id;
            deal->_time = time(NULL);
        }

        return deal;
}

static int CompareAsks(const void* a , const void* b)
{
    return ((const offer_t*)b)->_value - ((const offer_t*)a)->_value;
}

static int CompareBids(const void* a , const void* b)
{
    return ((const offer_t*)a)->_value - ((const offer_t*)b)->_value;
}

static void PrintDeal(void* _deal, void* _arg)
{
    (void)_arg;
    deal_t* deal = _deal;
    printf("id %lu value %u qty %u time %ld\n", deal->_stock_id, deal->_value, deal->_quantity, deal->_time);
}


