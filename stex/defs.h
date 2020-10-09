#ifndef stex_defs_H
#define stex_defs_H

#include <semaphore.h>
#include <stddef.h>
#include <pthread.h>

#define only_stock_id ((size_t)1)
#define only_stock_inital_value ((unsigned)1000) // i.e 10.00


typedef enum { ask, bid } order_sides_e;

typedef struct 
{
    size_t _stock_id;
    time_t _time;
    unsigned int _quantity;
    unsigned int _value;
    order_sides_e _side;
} offer_t;

void init(void); // ideally, between init and cleanup no syscalls are made
void Run(void);
void StopMarket(void);
void cleanup(void);

unsigned current_value(size_t stock_id);

typedef struct
{
    size_t _stock_id;
    time_t _time;
    unsigned _value;
    unsigned _quantity;
} deal_t;





#endif // stex_defs_H