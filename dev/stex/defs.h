#ifndef stex_defs_H
#define stex_defs_H

#include <stddef.h>

#define only_stock_id ((size_t)1)
#define only_stock_inital_value ((unsigned)1000) // i.e 10.00

typedef enum { ask, bid, num_sides } order_sides_e;

typedef struct 
{
    size_t _stock_id;
    time_t _time;
    unsigned int _quantity;
    unsigned int _value;
    order_sides_e _side;
} offer_t;

typedef struct
{
    size_t _stock_id;
    time_t _time;
    unsigned _value;
    unsigned _quantity;
} deal_t;

typedef struct {
    size_t _stock_id;
    unsigned _value;
    time_t _time;
} value_update_t;

#endif // stex_defs_H