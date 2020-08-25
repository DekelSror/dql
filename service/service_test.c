#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/wait.h>

#include "framework.h"


#define request_size (1)
#define response_size (4096)


// 192.168.1.142

static char requests[] = "011010110q";

void Session(unsigned short port) {

    const int pid = getpid();
    struct hostent* host_address = gethostbyname("192.168.1.142");

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    char request_buf[request_size] = { 0 };
    char response_buf[response_size] = { 0 };

    struct sockaddr_in server_address;
    socklen_t address_size = sizeof(struct sockaddr_in);
    memset(&server_address, 0, address_size);


    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    server_address.sin_addr = *((struct in_addr *)host_address->h_addr);
    memset(server_address.sin_zero, 0, 8);

    int connect_res = connect(client_socket, (const struct sockaddr*)&server_address, address_size);
    if (connect_res) printf("client connection trouble\n");


    printf("%d connected!\n\n", pid);

    for (char* req = requests; *req;  ++req)
    {
        memset(request_buf, 0, request_size);
        memset(response_buf, 0, response_size);
        
        request_buf[0] = *req;
        ssize_t sent = send(client_socket, request_buf, request_size, 0);

        printf("%d sent an %ld long request %c \n", pid, sent, *req);

        ssize_t received = recv(client_socket, response_buf, response_size, 0);

        printf("%d got the (%ld long) response: %s\n", pid, received, response_buf);
    }

    close(client_socket);
}

#include <stdlib.h>

int main(int argc, char const *argv[])
{
    unsigned short port = strtoul(argv[1], NULL, 10) & 0xffff;

    Session(port);


    return 0;
}
