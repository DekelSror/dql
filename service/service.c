#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

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

typedef struct 
{
    int _socket;
    struct in_addr _address;
} session_args_t;

static const response_fn responses[] = { GiveLove, GiveHate };
static const int response_lengths[] = {31, 19};

void SetServerOnPort(unsigned short port) {

    int service_socket = socket(AF_INET, SOCK_STREAM, 0);

    int optval = 1;
    setsockopt(service_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    if (0 > service_socket) printf("socket trouble\n");


    struct sockaddr_in service_address;
    struct sockaddr_in client_address;
    socklen_t address_size = (unsigned int)sizeof(struct sockaddr_in);

    service_address.sin_family = AF_INET;
    service_address.sin_addr.s_addr = INADDR_ANY;

    service_address.sin_port = htons(port);

    int bind_res = 0;
    do 
    {
        bind_res = bind(service_socket, (struct sockaddr*)&service_address, address_size);

        if (0 == bind_res) break;
        printf("bind to %hu is no good %d, trying another port\n", port, errno);
        ++port;
    } while (bind_res);

    printf("BOUND 2 %hu\n", port);    

    int listen_res = listen(service_socket, 1);
    if (listen_res) printf("listen trouble\n");

    size_t current_sessions = 0;
    do
    {
        memset(&client_address, 0, address_size);
        int client_socket = accept(service_socket,(struct sockaddr*)&client_address, &address_size);
        if (0 > client_socket) {
            printf("bad client\n");
            continue;
        }

        ++current_sessions;

        printf("sending client socket %d to Task\n", client_socket);
        Task(RunSession, (void*)client_socket, NULL); // what happens when all the pool's threads are running sessions?

        sleep(3);
        printf("done sleeping\n");
        break;
    } while(current_sessions);

    close(service_socket);    
}

void* RunSession(void* arg)
{
    int socket = (int)arg;
    printf("Task got socket %d\n", socket);

    char request_buf[request_buf_size] = {0};
    char response_buf[response_buf_size] = {0};

    while (1)
    {
        memset(response_buf, 0, response_buf_size);
        memset(request_buf, 0, request_buf_size);

        ssize_t received = recv(socket, request_buf, request_buf_size, 0);
        if ('q' == *request_buf) break;

        const int route = *request_buf - '0';

        printf("client wants %d\n", route);
        
        sprintf(response_buf, "%s", responses[route]());

        ssize_t sent = send(socket, response_buf, response_lengths[route], 0);

        printf("sent %ld bytes over to the client\n", sent);

    }

    close(socket);
    printf("session ended\n");

    return NULL;
}

int main(void)
{

    FrameworkInit();

    SetServerOnPort(12121);

    printf("done serving, cleanup...\n");
    FrameworkCleanup();

    return 0;
}




