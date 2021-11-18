#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hash.h"

#include "car.h"


void SetAllGetAllRemoveAll(size_t table_size, size_t dataset_size)
{
    size_t test_errors = 0;
    printf("table size is %lu\n\n", table_size);

    hash_t h = Hash.create(table_size);

    car_t* cars = malloc(sizeof(*cars) * dataset_size);
    string_t** keys = malloc(sizeof(keys) * dataset_size);
    char buf[64] = {0};

    for (size_t i = 0; i < dataset_size; i++)
    {
        memset(buf, 0, 64);
        FillCar(cars + i);

        sprintf(buf, "%lu", cars[i]._engine_id);
        keys[i] = String.create(buf);

        const int set_res = Hash.set(h, keys[i], cars + i);
        if (set_res)
        {
            ++test_errors;
            printf("failed to set %s", String.chars(keys[i]));
        }
    }

    for (size_t i = 0; i < dataset_size; i++)
    {
        car_t* found = Hash.get(h, keys[i]);
        if (NULL == found)
        {
            ++test_errors;
            printf("cannot find %s \n", String.chars(keys[i]));
            continue;
        }

        car_t* local = cars + i;
        if (local->_license_plate != found->_license_plate)
        {
            ++test_errors;
            printf("found wrong key! original %lu from hash %lu", local->_license_plate, found->_license_plate);
        }
    }

    for (size_t i = 0; i < dataset_size; i++)
    {
        car_t* removed = Hash.remove(h, keys[i]);
        if (NULL == removed)
        {
            ++test_errors;
            printf("cannot remove %s, key is 0x%zx \n", String.chars(keys[i]), cars[i]._license_plate);
            continue;
        }

        car_t* local = cars + i;
        if (local->_license_plate != removed->_license_plate)
        {
            ++test_errors;
            printf("removed wrong key! original %lu from hash %lu", local->_license_plate, removed->_license_plate);
        }
    }

    for (size_t i = 0; i < dataset_size; i++)
    {
        String.free(keys[i]);
    }

    free(keys);
    free(cars);

    Hash.free(h);

    printf("test over with %lu errors\n", test_errors);
}


int main(void)
{
    SetAllGetAllRemoveAll(1000, 190);

    return 0;
}
