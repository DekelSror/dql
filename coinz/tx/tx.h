#ifndef dql_tx_h
#define dql_tx_h

typedef void* tx_t;

typedef struct 
{
    tx_t(*create)();
    void(*free)(tx_t);
} tx_api_t;

extern const tx_api_t Tx;

#endif // dql_tx_h
