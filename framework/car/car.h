#ifndef dql_car_for_tests_h
#define dql_car_for_tests_h


#include <stddef.h>


// to test stuff with a bit more complex object
// defined in header to be able to declare it by val when needed


#define name_length (20)

typedef struct 
{
    char _brand_name[name_length];
    char _model_name[name_length];
    size_t _engine_id;
    size_t _license_plate;
} car_t;


void FillCar(car_t* this);
void ShowCar(car_t* this);


#endif // dql_car_for_tests_h
