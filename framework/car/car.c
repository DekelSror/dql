#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "car.h"



const char* brand_names[] = {"GMC", "BMW", "Nissan", "Toyota", "Subaro"};
const int brand_len[] = {4, 4, 7, 7, 7};
const char* model_names[] = {"SUV", "Sports", "Truck, Pickup", "Family", "Seven Seats"};
const int model_len[] = {4, 7, 14, 7, 12};

void FillCar(car_t* this)
{
    static size_t idx = 0;
    srand(time(NULL));

    const int brand = rand() % 5;
    memmove(this->_brand_name, brand_names[brand], brand_len[brand]);

    const int model = rand() % 5;
    memmove(this->_model_name, model_names[model], model_len[model]);

    this->_license_plate = idx << 36 | rand();
    this->_engine_id = idx++;
}

void ShowCar(car_t* this)
{
    printf("it's a %s, by %s, plate #%lu, engine #%lu\n", this->_model_name, this->_brand_name, this->_license_plate, this->_engine_id);
}