#include <stdio.h>

#include "queue_templates.h"


typedef char* chp;

make_queue(chp);

int main(int argc, char const *argv[])
{

    const unsigned qsize = 20;

    chp_queue* q = chpQueue.create(qsize);

    for (long i = 0; i < qsize * 2; i++)
    {
        int res = chpQueue.enq(q, "smallak");

        if (res != 0)
        {
            printf("cannot enq, got %u / %u elems\n", q->_size, q->_capacity);
            break;
        }
    }

    for (size_t i = 0; i < qsize * 2; i++)
    {
        chp deq = chpQueue.deq(q);
        if (0l == deq) 
        {
            printf("cannot deq - %u\n", q->_size);
            break;
        }
        else printf("elem at %ld %s\n", i, deq);
    }

    chpQueue.free(q);
    
    

    return 0;
}
