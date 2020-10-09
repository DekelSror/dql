#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <mqueue.h>
#include <errno.h>

#include "memblocks.h"
#include "heap.h"
#include "vlist.h"
#include "defs.h"

#define min(a, b) (((a) >= (b)) ? (a) : (b))

void init(void);
static void SetInitialMarketState(void);
void cleanup(void);

// low asks come first
int CompareAsks(const void* a , const void* b);
// high bids come first
int CompareBids(const void* a , const void* b);

static void PrintDeal(void* _deal, void* _arg);

static void* market_pool_buffer = NULL;
static memblocks_t market_pool = NULL;
static heap_t* market[2] = {NULL}; 
static vlist_t deals = NULL;
static unsigned only_stock_current_value = only_stock_inital_value;
static mqd_t offers_mq;

static deal_t* DealCreate(size_t stock_id, unsigned value, unsigned quantity);

unsigned current_value(size_t stock_id) {(void)stock_id; return only_stock_current_value;}

void CheckForOffers(void)
{
    char buf[sizeof(offer_t)];
    ssize_t rcvd = mq_receive(offers_mq, buf, sizeof(offer_t), NULL);
    offer_t* copy = Memblocks.get_block(market_pool);
    memmove(copy, buf, sizeof(offer_t));
    Heap.insert(market[copy->_side], copy);
}

void RemoveOffer(offer_t* offer)
{
    Heap.pop(market[offer->_side]);
    Memblocks.free_block(offer);
}

void Match(void)
{
    offer_t* lowest_ask = Heap.top(market[ask]);
    offer_t* highest_bid = Heap.top(market[bid]);

    if (NULL != lowest_ask && NULL != highest_bid)
    {
        if (highest_bid->_value >= lowest_ask->_value)
        {
            printf("found a deal! %u %u \n", lowest_ask->_value, highest_bid->_value);
            unsigned deal_qty = min(highest_bid->_quantity, lowest_ask->_quantity);
            deal_t* deal = DealCreate(highest_bid->_stock_id, lowest_ask->_value, deal_qty);
            only_stock_current_value = lowest_ask->_value;
            Vlist.add(deals, deal); // delegate the accounting to another entity (event?)

            if (highest_bid->_quantity > lowest_ask->_quantity)
            {
                highest_bid->_quantity -= deal_qty;
                RemoveOffer(lowest_ask);
            }
            else if (highest_bid->_quantity < lowest_ask->_quantity)
            {
                lowest_ask->_quantity -= deal_qty;
                RemoveOffer(highest_bid);
            }
            else
            {
                RemoveOffer(lowest_ask);
                RemoveOffer(highest_bid);
            }

            Vlist.for_each(deals, PrintDeal, NULL);
        }
        else
        {
            printf("no match! ask %u bid %u!\n", lowest_ask->_value, highest_bid->_value);
            CheckForOffers();
        }
    }
    else
    {
        printf("no offers!\n");
        CheckForOffers();
    }
}


void Run(void)
{
    while (1)
    {
        Match();
    }
}

void init(void)
{

    market_pool_buffer = malloc(Memblocks.reqired_buf_size(sizeof(offer_t), 100));
    market_pool = Memblocks.create(market_pool_buffer, sizeof(offer_t), 100);
    
    market[ask] = Heap.create(100, CompareAsks);
    market[bid] = Heap.create(100, CompareBids);

    deals = Vlist.create();

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = sizeof(offer_t);

    offers_mq = mq_open("/stex_offers_mq", O_CREAT | O_RDONLY, 0666, &attr);

    printf("got dat mq fd %d errno %d\n", offers_mq, errno);
    
    // SetInitialMarketState();
}

void cleanup(void)
{
    Memblocks.free(market_pool_buffer);
    free(market_pool_buffer);
    
    Heap.free(market[ask]);
    Heap.free(market[bid]);
    Vlist.free(deals);
}

// utils

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

int CompareAsks(const void* a , const void* b)
{
    return ((const offer_t*)b)->_value - ((const offer_t*)a)->_value;
}

int CompareBids(const void* a , const void* b)
{
    return ((const offer_t*)a)->_value - ((const offer_t*)b)->_value;
}

static void SetInitialMarketState(void)
{
    offer_t* offer = Memblocks.get_block(market_pool);
    offer->_side = ask;
    offer->_quantity = 100;
    offer->_stock_id = only_stock_id;
    offer->_time = time(NULL);
    offer->_value = only_stock_inital_value - 5;
    Heap.insert(market[offer->_side], offer);

    offer = Memblocks.get_block(market_pool);
    offer->_side = bid;
    offer->_quantity = 50;
    offer->_stock_id = only_stock_id;
    offer->_time = time(NULL);
    offer->_value = only_stock_inital_value - 5;
    Heap.insert(market[offer->_side], offer);
    

    offer = Memblocks.get_block(market_pool);
    offer->_side = ask;
    offer->_quantity = 50;
    offer->_stock_id = only_stock_id;
    offer->_time = time(NULL);
    offer->_value = only_stock_inital_value + 10;
    Heap.insert(market[offer->_side], offer);
}

static void PrintDeal(void* _deal, void* _arg)
{
    (void)_arg;
    deal_t* deal = _deal;
    printf("id %lu value %u qty %u time %ld\n", deal->_stock_id, deal->_value, deal->_quantity, deal->_time);
}
