#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mqueue.h>
#include <string.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>

#include "defs.h"

offer_t* GenerateOffer(void)
{
    offer_t* offer = malloc(sizeof(*offer));

    offer->_stock_id = only_stock_id;
    offer->_side = rand() %2;
    offer->_quantity = (rand() % 20 + 1) * 50;
    offer->_time = time(NULL);
    offer->_value = only_stock_inital_value + (rand() % 40 - 20);

    return offer;
}

int main(int argc, char* const argv[])
{

    if (argc != 3) return 4;

    int batch_size = atoi(argv[1]);
    int interval = atoi(argv[2]);

    srand(time(NULL) >> 20);
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 100;
    attr.mq_msgsize = sizeof(offer_t);

    mqd_t offers_mq = mq_open("/stex_offers_mq", O_CREAT | O_RDWR, 0666, &attr);

    printf("mq fd %d errno %d\n\n", offers_mq, errno);

    char buf[sizeof(offer_t)];


    for (int i = 0; i < batch_size; i++)
    {
        offer_t* offer = GenerateOffer();
        memmove(buf, offer, sizeof(offer_t));

        int check = mq_send(offers_mq, buf, sizeof(offer_t), 0);

        printf("sent status %d\n", check);
        printf("offer: stock %lu side %d value %u qty %u\n", offer->_stock_id, offer->_side, offer->_value, offer->_quantity);

        free(offer);

        sleep(interval);
    }

    mq_close(offers_mq);
    mq_unlink("/stex_offers_mq");

    return 0;
}
