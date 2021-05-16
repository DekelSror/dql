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
    ManagedScope.start();

    unsigned_arr* arr = ManagedScope.declare_obj(&ui10_api);

    arr->_at[0] = 90;
    arr->_at[9] = 50;

    ManagedScope.end();
}



int main(void)
{
    ManagedScope.init();


    BasicTest();




    ManagedScope.cleanup();



    return 0;
} 
