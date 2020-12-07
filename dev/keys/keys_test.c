#include <stdio.h>

#include "string_class.h"
#include "keys.h"

int main(void)
{
    keys_t keys = Keys.create(0x010001UL, 2048);
    keypair_t alice = Keys.make_keypair(keys);

    Keys.get_pk(alice);

    // FILE* f = fopen()
   

    // keypair_t bob = Keys.make_keypair(keys);

    // const string_t msg = String.create("hi, bob!");

    // signature_t sig = Keys.sign(alice, String.chars(msg), String.len(msg));

    // int res = Keys.verify(alice, String.chars(msg), String.len(msg), sig);

    // printf("verified? %d\n", res);

    // String.free(msg);
    // Keys.free(keys);

    // Keys.free_keypair(alice);
    // Keys.free_keypair(bob);
    // Keys.free_signature(sig);


    return 0;
} 
