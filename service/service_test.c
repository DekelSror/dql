#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>



#define request_size (1)
#define response_size (4096)


static const unsigned short server_port = 12121;
static const char* server_port_str = "12121";

// 192.168.1.142

void SetClientOnPort(void) {

    struct hostent* host_address = gethostbyname("192.168.1.142");

    printf("mah host:  %p\n", host_address);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    char request_buf[request_size] = { 0 };
    char response_buf[response_size] = { 0 };

    struct sockaddr_in server_address;
    socklen_t address_size = sizeof(struct sockaddr_in);
    memset(&server_address, 0, address_size);


    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);

    server_address.sin_addr = *((struct in_addr *)host_address->h_addr);
    memset(server_address.sin_zero, 0, 8);

    connect(client_socket, (const struct sockaddr*)&server_address, address_size);

    while (1)
    {
        memset(request_buf, 0, request_size);
        memset(response_buf, 0, response_size);
        
        scanf("%s", request_buf);

        ssize_t sent = send(client_socket, request_buf, request_size, 0);

        printf("sent an %ld long request \n", sent);

        ssize_t received = recv(client_socket, response_buf, response_size, 0);

        printf("got the (%ld long) response: %s\n", received, response_buf);
    }

    close(client_socket);
}


int main()
{

    SetClientOnPort();

    return 0;
}
