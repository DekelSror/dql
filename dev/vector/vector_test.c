#include "stdio.h"
#include "vector.h"


int main(void)
{

    char smalla[] = "smalla";

    vector_t this = Vector.create(10, sizeof(char));

    Vector.push(this, smalla);

    void* dbg = Vector.top(this);

    printf("top after 's' pushed: %c\n", *(char*)dbg);
    printf("pop after 's' pushed: %c\n", *(char*)Vector.pop(this));

    printf("top after 's' popped: %p\n", Vector.top(this));
    printf("pop after 's' popped: %p\n", Vector.pop(this));


    Vector.push(this, smalla);
    Vector.push(this, smalla + 1);
    Vector.push(this, smalla + 2);
    Vector.push(this, smalla + 3);
    Vector.push(this, smalla + 4);
    Vector.push(this, smalla + 5);

    for (size_t i = 0; i < 6; i++)
    {
        printf("char at[%lu] after smalla pushed: %c\n",i, *(char*)Vector.at(this, i));
    }

    Vector.free(this);
    
    return 0;
} 
