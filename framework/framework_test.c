#include <stdio.h>
#include <stdlib.h>

#include "framework.h"



void* SumThemUp(void* arg)
{
    size_t up_to = (size_t)arg;
    size_t sum = 0;

    printf("gonna sum up %lu numbers!\n", up_to);

    for (size_t i = 0; i < up_to; i++)
    {
        sum += up_to;
    }

    printf("dude it's %lu\n", sum);

    return (void*)sum;
}

void* PrintAhlan(void* arg)
{
    (void)arg;
    printf("Ahlan!\n");
    return NULL;
}

void* PrintWhoAmI(void* arg)
{

    printf("%s", (char*)arg);
    return NULL;
}


void* TestLifecycle(void* arg)
{
    (void)arg;
    return NULL;
}

void TestTPool(void)
{
    FrameworkInit();

    Task(PrintAhlan, NULL, NULL);
    Task(PrintWhoAmI, "Oroko Sake\n", NULL);


    size_t up_to = 500000;
    size_t sum = 0;
    Task(SumThemUp, (void*)up_to, &sum);



    getchar();
    FrameworkCleanup();
}

void TestStringsMan(void)
{
    FrameworkInit();

    string_t* str = GetString("מברוכ עליכ יא אריס מברוכ\n");

    printf("str @%p is %s\n", str, String.chars(str));

    str = GetString("מברוכ עליכ יא אריס מברוכ\n");

    printf("str @%p is %s\n", str, String.chars(str));

    FreeString(str);
    printf("str @%p is %s\n", str, String.chars(str));

    FreeString(str);

    str = GetString("מברוכ עליכ יא אריס מברוכ\n");

    printf("str @%p is %s\n", str, String.chars(str));


}

void TestEvents(void)
{

}



int main(void)
{
    // TestTPool();

    TestStringsMan();
    return 0;
}
