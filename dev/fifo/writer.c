#include <stdio.h>
#include "fifo.h"

static char write_buf[] = "yodelay-hee-hoo!!\n\n";

int main(void)
{

    fifo_writer_t writer = Fifo.get_writer("dql_test_fifo");

    int written = Fifo.write(writer, write_buf);

    printf("wrote that %d\n", writer);

    Fifo.free_writer(writer);
    
    return 0;
}
