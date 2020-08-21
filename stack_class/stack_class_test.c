
#include <stdio.h>
#include <stdlib.h>

// #include "class_utils.h"
#include "stack_class.h"


#define classify const class_base_api* class = (const class_base_api*)T;


static stack_t stack;
static size_t errors = 0;

void NewStackIsEmpty(stack_t stack)
{
    if (!(Stack.is_empty(stack))) ++errors;
}

void AfterPushIsNotEmpty(stack_t stack)
{
    long elem = 5;

    Stack.push(stack, &elem);

    if (Stack.is_empty(stack)) ++errors;
}

void AfterPushAndPopIsEmpty(stack_t stack)
{
    long elem = 5;

    Stack.push(stack, &elem);

    Stack.pop(stack);

    if (!Stack.is_empty(stack)) ++errors;
}

void PopFromEmptyStackIsNull(stack_t stack)
{
    if (NULL != Stack.pop(stack)) ++errors;
}

void PopValueEqualsPushValue(stack_t stack)
{
    long elem = 5;

    Stack.push(stack, &elem);

    long pop_val = *((long*)Stack.pop(stack));
    
    if(elem != pop_val) 
    {
        ++errors;
        printf("popped %lu; expected %lu", pop_val, elem);
    }
}

void AfterPushSizeIsOne(stack_t stack)
{
    long elem = 5;

    Stack.push(stack, &elem);

    if (Stack.size(stack) != 1) ++errors;
}

void PushFailsAtFullCapacity(stack_t stack)
{
    long elems[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    
    for (size_t i = 0; i < 10; i++)
    {
        Stack.push(stack, elems + i);
    }

    const int push_rc = Stack.push(stack, elems + 10);
    if (!push_rc) ++errors;
}

void ValuesMaintainLIFO(stack_t stack)
{
    long elems[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    for (size_t i = 0; i < 10; i++)
    {
        Stack.push(stack, elems + i);
    }

    for (size_t i = 0; i < 10; i++)
    {
        long pop_val = *(long*)Stack.pop(stack);
        if (pop_val != elems[9 - i]) ++errors;
    }
}

// test lists

static const void(*tests[])(stack_t) = 
{
    NewStackIsEmpty,
    AfterPushIsNotEmpty,
    AfterPushAndPopIsEmpty,
    PopFromEmptyStackIsNull,
    PopValueEqualsPushValue,
    AfterPushSizeIsOne,
    PushFailsAtFullCapacity,
    ValuesMaintainLIFO
};

static const char* test_names[] = 
{
    "NewStackIsEmpty",
    "AfterPushIsNotEmpty",
    "AfterPushAndPopIsEmpty",
    "PopFromEmptyStackIsNull",
    "PopValueEqualsPushValue",
    "AfterPushSizeIsOne",
    "PushFailsAtFullCapacity",
    "ValuesMaintainLIFO"
};


int main(void)
{
    for (size_t i = 0; i < (sizeof(tests) / sizeof(tests[0])); i++)
    {
        stack = Stack.create(10);

        tests[i](stack);

        printf("errors from %s : %lu\n", test_names[i], errors);

        errors = 0;
        Stack.free(stack);
    }
    

    return 0;
}
