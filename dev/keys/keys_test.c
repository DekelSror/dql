#include <stdio.h>

#include "string_class.h"
#include "keys.h"

int main(void)
{
    keys_t keys = Keys.create(0x010001UL, 2048);
    keypair_t alice = Keys.make_keypair(keys);



    return 0;
} 
