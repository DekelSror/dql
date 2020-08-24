#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


#include "framework.h"

#define request_buf_size (1)
#define response_buf_size (64)

void* RunSession(void* arg);

const char* GiveLove(void)
{
    return "Oh, mah client, I love yah...\n";
}

const char* GiveHate(void)
{
    return "You suck, client!\n";
}

typedef enum { Love, Hate } responses_t;

typedef const char*(*response_fn)(void);

static const response_fn responses[] = { GiveLove, GiveHate };
static const int response_lengths[] = {31, 19};

void SetServerOnPort(unsigned short port) {

    int service_socket = socket(AF_INET, SOCK_STREAM, 0);


    struct sockaddr_in service_address;
    struct sockaddr_in client_address;
    socklen_t address_size = (unsigned int)sizeof(struct sockaddr_in);

    service_address.sin_family = AF_INET;
    service_address.sin_port = htons(port);
    service_address.sin_addr.s_addr = INADDR_ANY;

    bind(service_socket, (struct sockaddr*)&service_address, address_size);

    listen(service_socket, 1);

    while (1)
    {
        memset(&service_address, 0, address_size);
        int client_socket = accept(service_socket,(struct sockaddr*)&client_address, &address_size);

        struct 
        {
            int _socket;
            struct in_addr _address;
        } task_args = {client_socket, client_address.sin_addr};

        Task(RunSession, &task_args, NULL);

        close(client_socket);
        close(service_socket);
    }
}

void* RunSession(void* arg)
{
    struct 
    {
        int _socket;
        struct in_addr _address;
    }* task_args = arg;

    int client_socket = task_args->_socket;
    struct in_addr client_address = task_args->_address;

    char request_buf[request_buf_size] = {0};
    char response_buf[response_buf_size] = {0};

    while (1)
    {
        memset(response_buf, 0, response_buf_size);
        memset(request_buf, 0, request_buf_size);

        ssize_t received = recv(client_socket, request_buf, request_buf_size, 0);

        printf("got request! it's from %s, it's %ld long and says '%s'\n", inet_ntoa(client_address), received, request_buf);

        const int route = *request_buf - '0';

        printf("client wants %d\n", route);
        
        sprintf(response_buf, "%s", responses[route]());

        ssize_t sent = send(client_socket, response_buf, response_lengths[route], 0);

        printf("sent %ld bytes over to the client", sent);

    }

    return NULL;
}

int main(void)
{

    SetServerOnPort(12121);

    return 0;
}




