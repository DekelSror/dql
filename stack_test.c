#include <stdio.h>

#include "stack_templates.h"


typedef char* chp;

make_stack(chp)

int main(int argc, char const *argv[])
{

    const unsigned stacksize = 20;

    chp_stack* stack = chpStack.create(stacksize);

    for (long i = 0; i < stacksize * 2; i++)
    {
        int res = chpStack.push(stack, "smallak");

        if (res != 0)
        {
            printf("cannot push, got %lu / %lu elems\n", chpStack.size(stack), stack->_capacity);
            break;
        }
    }

    for (size_t i = 0; i < stacksize * 2; i++)
    {
        chp pop = chpStack.pop(stack);
        if (0l == pop) 
        {
            printf("cannot pop - %lu stack is %d empty\n", chpStack.size(stack), chpStack.is_empty(stack));
            break;
        }
        else printf("elem at %lu %s\n", i, pop);
    }

    chpStack.free(stack);
    
    

    return 0;
}
