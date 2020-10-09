#ifndef dql_mock_webapi_h
#define dql_mock_webapi_h

#include <stddef.h>

#include "buffer.h"

const char* GiveLove(void)
{
    return "Oh, mah client, I love yah...\n";
}

const char* GiveHate(void)
{
    return "You suck, client!\n";
}

inline size_t MaxResponseSize(void) {return 31;}


typedef const char*(*response_fn)(void);
static const response_fn responses[] = { GiveLove, GiveHate };
static const int response_lengths[] = {31, 19};

static response_status_t Handle(buffer_t* req_buf, buffer_t* res_buf)
{
    if (req_buf->_size != 1 || req_buf->_written_size != 1) return bad_request;
    if ('q' == *req_buf->_data) return end_request;
    if (res_buf->_size < 31) return nomem_res_buf;

    const int route = *req_buf->_data - '0';

    if (route > 1) return bad_route;
    if (0) return bad_args;

    sprintf(res_buf, "%s", responses[route]());

    return success;
}

static size_t RequiredBufferSize(void) { return 64; }

const web_api_api_t EmotionsWebApi = {Handle, RequiredBufferSize};

#endif dql_mock_webapi_h
