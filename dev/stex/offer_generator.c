#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mqueue.h>
#include <string.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>

#include "defs.h"
#include "msg_queue.h"

offer_t* GenerateOffer(value_update_t* update)
{   
    static offer_t new_offer = { 0 };
    offer_t* offer = &new_offer;

    offer->_stock_id = update->_stock_id;
    offer->_side = rand() %2;
    offer->_quantity = (rand() % 20 + 1) * 50;
    offer->_time = time(NULL);
    offer->_value = update->_value + (rand() % 40 - 20);

    return offer;
}

int main(int argc, char* const argv[])
{
    if (argc != 3) return 4;

    int batch_size = atoi(argv[1]);
    unsigned interval;
    sscanf(argv[2], "%u", &interval);

    srand(time(NULL) >> 20);

// set mq
    msg_queue_t offers_mq = MsgQueue.create("/stex_offers_mq", 100, sizeof(offer_t));

    printf("mq fd %d errno %d\n\n", offers_mq, errno);

// set connection to value service
    struct hostent* host_address = gethostbyname("0.0.0.0");

    
    char post_buf[sizeof(offer_t)] = { 0 };

    struct sockaddr_in server_address;
    socklen_t address_size = sizeof(struct sockaddr_in);
    memset(&server_address, 0, address_size);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(14444);

    server_address.sin_addr = *((struct in_addr *)host_address->h_addr);

    const int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket %d errno %d\n", client_socket, errno);

    const int connect_res = connect(client_socket, (const struct sockaddr*)&server_address, address_size);
    if (connect_res) printf("client connection trouble %d\n", errno);

// work
    for (int i = 0; i < batch_size; i++)
    {
        char request_buf[8] = { 0 };
        value_update_t response_buf = { 0 };

        *(size_t*)request_buf = only_stock_id;

        printf("sending request to value server...\n");
        send(client_socket, request_buf, 8, 0);
        printf("request sent errno %d\n", errno);

        const ssize_t rcvd = recv(client_socket, &response_buf, sizeof(response_buf), 0);
        printf("received %ld bytes from value server: stock %lu value %u\n", rcvd, response_buf._stock_id, response_buf._value);

        if (-1 == rcvd)
        {
            printf("trouble receivng. errno %d \n", errno);
        }

        offer_t* offer = GenerateOffer(&response_buf);
        memmove(post_buf, offer, sizeof(offer_t));

        int check = MsgQueue.enqueue(offers_mq, post_buf);

        printf("sent status %d\n", check);
        printf("offer: stock %lu side %d value %u qty %u\n", offer->_stock_id, offer->_side, offer->_value, offer->_quantity);

        usleep(interval);
    }

    MsgQueue.free(offers_mq);
    mq_unlink("/stex_offers_mq");
    close(client_socket);

    return 0;
}
