#include <stdio.h>

#include "fifo.h"

static char readbuf[4097] = {0};

int main(void)
{
    int fifo = Fifo.create("dql_test_fifo");


    fifo_reader_t reader = Fifo.get_reader(fifo);


    const char* read = Fifo.read(reader, 4096);

    printf("%s\n", read);
    
    Fifo.free_reader(reader);
    
    return 0;
}
