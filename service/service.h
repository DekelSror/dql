#ifndef dql_service_h
#define dql_service_h

typedef void* service_t;


#include "web_api.h"


typedef struct 
{
    // port is only a hint, use .address to be sure. If protocol is NULL, HTTP will be used
    service_t*(*create)(const web_api_api_t* api, unsigned short port, const void* protocol);

    // bind address from create to a socket and set an accept() loop
    // once it is running, clients can send reqests comforming to the protocol
    // and within the api bounds
    void(*run)(service_t);

    //
    const char*(*address)(const service_t);
} service_api_t;


extern const service_api_t Service;





#endif // dql_service_h
