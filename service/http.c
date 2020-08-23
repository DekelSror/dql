#include <stdlib.h>
#include <stdio.h>

// GET /love HTTP/1.1

typedef struct
{
    char _method[10];
    char _version[20];
    char* _target;
} http_start_line_t;

typedef struct
{
    char* _name;
    char* _value;
} http_header_t;


typedef struct _request_t
{
    http_start_line_t _start_line;
    http_header_t* _headers;
    char* _body;
} http_request_t;


// minimal request
// GET / HTTP/1.1 \r\n (start line)
//\r\n headers
//\r\n body


void Parse(char* raw_req, char* res_buf) {
    http_request_t* req = malloc(sizeof(*req));

// parse start line

    char* tok = strtok(raw_req, " "); // tok = method
    sprintf(req->_start_line._method, "%s", tok);

    tok = strtok(NULL, " "); // tok = target
    sprintf(req->_start_line._target, "%s", tok);

    tok = strtok(NULL, "\r\n"); // tok = version
    sprintf(req->_start_line._version, "%s", tok);
  

// parse headers
    tok = strtok(NULL, "\r\n"); // tok = all the headers



// body

}
