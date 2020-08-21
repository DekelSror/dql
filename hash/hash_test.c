#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#include "car.h"

#define test_size (2)

int main()
{
    hash_t h = Hash.create(test_size);

    car_t cars[test_size] = {0};

    for (size_t i = 0; i < test_size; i++)
    {
        FillCar(cars + i);
        ShowCar(cars + i);
    }


    string_t* car_key_lol = String.create(cars[0]._license_plate);

    Hash.set(h, car_key_lol, cars);

    car_t* found = Hash.get(h, car_key_lol);

    printf("found %p \n", (void*)found);
    ShowCar(found);


    // Hash.free(h);
    
    return 0;
}
