#ifndef dql_web_api_api_h
#define dql_web_api_api_h

typedef void* wep_api_t;

typedef enum { success, bad_request, bad_route, end_request, nomem_res_buf, bad_args } response_status_t;

typedef struct
{
    response_status_t(*handle)(buffer_t* req_buf, buffer_t* res_buf);
    size_t(*required_buffer_size)(void);
} web_api_api_t;

#endif dql_web_api_api_h
