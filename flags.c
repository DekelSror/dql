#include <stdio.h>




typedef unsigned long flags_t;

#define flags_set(f, i) f | (1 << i)
#define flags_unset(f, i) f & ~(1 << i)


int main(int argc, char const *argv[])
{

    printf("%zu  \n", flags_unset(8, 3));

    return 0;
}

