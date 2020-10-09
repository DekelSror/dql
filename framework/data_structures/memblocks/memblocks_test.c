#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "memblocks.h"
#include "car.h"


int main(void)
{
    size_t buf_size = Memblocks.reqired_buf_size(sizeof(car_t), 1);
    void* buf = malloc(buf_size);
    printf("got my %lu sized buf @%p ready...\n", buf_size, buf);

    memblocks_t car_pool = Memblocks.create(buf, sizeof(car_t), 1);

    block_t car;
    
    while ((car = Memblocks.get_block(car_pool)) != NULL)
    {
        FillCar(car);
        ShowCar(car);

        if (((car_t*)car)->_engine_id % 5 == 0)
        {
            memset(car, 0, sizeof(*car));
            Memblocks.free_block(car);
        }
    }

    Memblocks.free(car_pool);

    free(buf);

    
    return 0;
}
