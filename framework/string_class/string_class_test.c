#include <stdio.h>
#include <string.h>

#include "string_class.h"

static unsigned errcount = 0;

void TestCtors(void)
{
    char* test = "smallak wallak";
    const unsigned test_len = strlen(test);

    string_t str = String.create("smallak wallak");

    if (String.len(str) != test_len)
    {
        printf("TestCtors wrong len %lu\n", String.len(str));
        ++errcount;
    }

    if (0 != memcmp(String.chars(str), test, test_len))
    {
        printf("TestCtors wrong chars \ntest %s \nctor %s", test, String.chars(str));
    }


    String.free(str);

    str = String.create_fixed(test, 1);

    if ('s' != *(String.chars(str)))
    {
        ++errcount;
        printf("TestCtors fixed 1 wrong char test \'s\' str %c\n", *(String.chars(str)));
    }

    String.free(str);


    str = String.create_until(test, " w");
    const char* test_part_1 = "smallak";

    if (0 != strncmp(String.chars(str), test_part_1, strlen(test_part_1)))
    {
        printf("TestCtors until wrong values test %s str %s", test_part_1, String.chars(str));
    }

    String.free(str);
}



int main()
{
    TestCtors();
    return 0;
}
