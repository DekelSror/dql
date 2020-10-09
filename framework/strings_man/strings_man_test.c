

#include <stdio.h>

#include "strings_man.h"


int main(void)
{
    strings_t strings_man = Strings.create();

    printf("string: %p, instancs: %p\n", Strings, strings_man);

    string_t hello = Strings.get(strings_man, "hello, Mr. b0ss");

    // printf("%s is %lu long", String.chars(hello), String.len(hello));

    // Strings.dispose(strings_man, hello);

    // Strings.free(strings_man);
    
    return 0;
}
