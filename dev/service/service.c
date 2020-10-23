#include <stdlib.h> // mallocs
#include <netinet/in.h> // 
#include <arpa/inet.h> //
#include <sys/socket.h> //
#include <unistd.h> //
#include <string.h> // memset
#include <stdio.h> // debugging
#include <errno.h> // debugging
#include <sys/select.h> // select, fd_set and Co.
#include <time.h> // struct timeval (unused)

#include "framework.h" // Task
#include "buffer.h"
#include "vlist.h"
#include "service.h"
#include "web_api.h"
#include "emotions_webapi.h"

#define request_buf_size (1)
#define response_buf_size (64)

void* RunSession(void* arg);
static void EndSession(session_t* s);

// each client has a session, and the client socket connects them
typedef struct 
{
    vlist_t _sessions;
    fd_set _clients;
    web_api_api_t* _api;
    struct sockaddr_in _address;
} _service_t;

typedef struct 
{
    int _socket;
    buffer_t* _req_buf;
    buffer_t* _res_buf;
    web_api_api_t* _api;
} session_t;

static const socklen_t address_size = (socklen_t)sizeof(struct sockaddr_in);

static service_t Create(const web_api_api_t* api, unsigned short port_hint, const void* protocol)
{
    _service_t* this = malloc(sizeof(*this));

    this->_api = api;

    this->_address.sin_family = AF_INET;
    this->_address.sin_addr.s_addr = INADDR_ANY;

    // remember, kids - the port is a part of the actual literal address of the service
    // but the hint might not be available so service needs to be able to declare its address
    this->_address.sin_port = htons(port_hint);
    FD_ZERO(&this->_clients);

    return this;
}

static void Run(service_t _this) {

    _service_t* this = _this;

    struct sockaddr_in client_address;
    
    int service_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > service_socket) printf("socket trouble\n");

    int optval = 1;
    setsockopt(service_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);  

    int bind_res = bind(service_socket, (struct sockaddr*)&this->_address, address_size);
    if (bind_res) printf("bind no good\n");

    printf("BOUND 2 %hu\n", this->_address.sin_port);    

    // backlog is set to 1 since every session is handed over
    int listen_res = listen(service_socket, 1);
    if (listen_res) printf("listen trouble\n");

    // ready to serve on empty list of connections
    // also this is better off on a separate thread as it will never yield control
    Task(ManageSessions, this, NULL);

    while(1)
    {
        memset(&client_address, 0, address_size);

        int client_socket = accept(service_socket,(struct sockaddr*)&client_address, &address_size);
        if (0 > client_socket) {
            printf("bad client %s\n", inet_ntoa(client_address.sin_addr));
            continue;
        }

        session_t* new_session = malloc(sizeof(*new_session));

        new_session->_socket = client_socket;
        Buffer.create(new_session->_req_buf, this->_api->required_buffer_size());
        Buffer.create(new_session->_res_buf, this->_api->required_buffer_size());
        new_session->_api = this->_api;
        Vlist.add(this->_sessions, new_session);
        
        FD_SET(client_socket, &this->_clients);
    }

    close(service_socket);    
}

static void InsertFd(void* _this, void* _set)
{
    session_t* this = _this;
    fd_set* set = _set;

    FD_SET(this->_socket, set);
}

static void* ManageSessions(_service_t* this)
{
    fd_set clients;
    struct timeval timeout = { 5, 0 };

    while (1)
    {
        FD_ZERO(&clients);
        // copy sokets from this->_sessions;
        Vlist.for_each(this->_sessions, InsertFd, &clients);

        int requests = select(Vlist.size(this->_sessions), &clients, NULL, NULL, NULL);
        // problem - long requests will block for long
        Vlist.for_each(this->_sessions, HandleRequest, &clients);
    }

    return NULL;
}

void HandleRequest(void* _this, void* _fd_set)
{
    session_t* this = _this;
    fd_set* clients = _fd_set;

    if (!FD_ISSET(this->_socket, clients)) return;

    memset(this->_res_buf->_data, 0, this->_res_buf->_size);
    memset(this->_req_buf, 0, this->_req_buf->_size);

    ssize_t received = recv(this->_socket, this->_req_buf->_data, this->_req_buf->_size, 0);
    if (0 < received) this->_req_buf->_written_size = received;

    response_status_t rs = this->_api->handle(this->_req_buf, this->_res_buf);

    if (rs == end_request) // how much does Mr. service knows about protocol?
    {
        EndSession(this); // how to remove session from the service's list?
        return;
    }

    ssize_t sent = send(this->_socket, this->_res_buf->_data, this->_res_buf->_written_size, 0);
}

static void EndSession(session_t* s)
{
    close(s->_socket);
    free(s->_req_buf);
    s->_req_buf = NULL;
    free(s->_res_buf);
    s->_res_buf = NULL;
    free(s);

    s = NULL;
}

static const char* Address(service_t _this)
{
    _service_t* this = _this;

    return inet_ntoa(this->_address.sin_addr);
}

const service_api_t Service = { Create, Run, Address };
