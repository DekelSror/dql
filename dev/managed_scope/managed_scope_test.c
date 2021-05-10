#include <stdlib.h>
#include <stdio.h>

#include "array_templates.h"
#include "managed_scope.h"

declare_array(unsigned);

void* MakeSize10Arr(void)
{
    return unsignedArrayCreate(10);
}

void Free10SizeArr(void* arr)
{
    free(arr);
}


struct 
{
    void*(*ctor)(void);
    void(*dtor)(void*);
} ui10_api = {MakeSize10Arr, Free10SizeArr};

void BasicTest(void)
{
    Managed_scope.start();

    unsigned_arr* arr = Managed_scope.declare_obj(&ui10_api);

    arr->_at[0] = 90;
    arr->_at[9] = 50;

    Managed_scope.end();
}



int main(void)
{
    Managed_scope.init();


    BasicTest();




    Managed_scope.cleanup();



    return 0;
} 
