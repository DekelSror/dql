#include <stdio.h>
#include <stdlib.h>

#include "btree_class.h"
#include "string_class.h"

#define TEST_SIZE (10)

static const char letters[] = "abcdefghijklmnopqrstuvwxyz";

string_t* RandomString(size_t length)
{
    char* buf = malloc(length);

    static char r = 0;

    srand(length << 3 + r);

    for (size_t i = 0; i < length; i++)
    {
        buf[i] = letters[(char)(rand() % sizeof(letters))];
    }

    string_t* res =  String.create_fixed(buf, length);

    printf("added: %s\n", String.chars(res));

    free(buf);

    if (!r) ++r;
    else --r;   


    return res;
}

typedef struct 
{
    string_t* name;
    int other_thing;
} human_t;


void* NameGet(const void* human)
{
    return ((const human_t*)human)->name;
}

int main(int argc, char const *argv[])
{

    size_t failures = 0;

    return 0;
}



void CreatePopulateFree(size_t* failures_count)
{
    btree_t* btree = Btree.create(String.cmp, NameGet);

    human_t ppl[TEST_SIZE] = { 0 };


    for (size_t i = 0; i < TEST_SIZE; i++)
    {
        const int v = rand() % 13 + 1;

        ppl[i].name = RandomString(v);
        ppl[i].other_thing = v << 4 + v - i;

        Btree.add(btree, ppl + i);
    }

    bterator_t* person = Bterator.first(btree);

    human_t* h = (human_t*)Bterator.get(person);

    for (size_t i = 0; i < TEST_SIZE; i++)
    {
        String.free(ppl[i].name);
    }

    Btree.free(btree);
    Bterator.free(person);   
}