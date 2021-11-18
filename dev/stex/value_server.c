#include <stdio.h>
#include <netinet/in.h> // 
#include <arpa/inet.h> //
#include <sys/socket.h> //
#include <unistd.h> //
#include <mqueue.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include "defs.h"
#include "msg_queue.h"

int main(void)
{
// mq
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = sizeof(value_update_t);

    const mqd_t values_mq = MsgQueue.create("/stex_values_mq", 100,sizeof(value_update_t));

// server
    const int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    printf("valueServer has socket %d errno %d\n", server_socket, errno);
    const int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    socklen_t sockaddr_in_size = sizeof(struct sockaddr_in);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_address.sin_port = htons(14444);

    const int bind_res = bind(server_socket, (struct sockaddr*)&server_address, sockaddr_in_size);
    printf("valueServer bind %d errno %d\n", bind_res, errno);

    printf("my address %s\n", inet_ntoa(server_address.sin_addr));
    const int listen_res = listen(server_socket, 10);
    printf("valueServer listen %d errno %d\n", listen_res, errno);


    const int client_socket = accept(server_socket, (struct sockaddr*)& client_address, &sockaddr_in_size);
    printf("valueServer accepted on %d\n", client_socket);
    if (0 > client_socket) {
        printf("bad client %s\n", inet_ntoa(client_address.sin_addr));
    }

    char req_buf[8] = { 0 };
    value_update_t res_buf = { only_stock_id, only_stock_inital_value, time(NULL) };
    
    while (1)
    {
        *(size_t*)req_buf = 0;

        recv(client_socket, req_buf, 8, 0);
        size_t stock_id = *(size_t*)req_buf;

        printf("valueServer request for value of %lu\n", stock_id);

        const unsigned count = MsgQueue.count(values_mq);

        printf("mq has %ld msgs waiting\n", count);

        if (0 < count)
        {
            const char* rcvd = MsgQueue.deueue(values_mq);
            printf("rcvd %ld errno %d \n", rcvd, errno);
        }

        const ssize_t sent = send(client_socket, (const void*)&res_buf, sizeof(res_buf), 0);
        printf("sent res_buf %ld %d\n", sent, errno);
    }

    return 0;
}

