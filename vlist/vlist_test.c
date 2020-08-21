

#include <stdio.h>

#include "vlist.h"



void PrintLink(void* link_data, void* arg)
{
    (void)arg;
    printf("link data: %d\n", *(int*)link_data);
}

void Increment(void* link_data, void* arg)
{
    *(int*)link_data += *(const int*)arg; 
}


int main()
{

    int vals[] = { 655, 43255, 85, 425, 75 };
    int inc = 5;

    vlist_t vlist = Vlist.create();

    for (int* v = vals; v < vals + 1; ++v)
    {
        Vlist.add(vlist, v);
    }
    

    Vlist.for_each(vlist, PrintLink, NULL);

    Vlist.for_each(vlist, Increment, &inc);

    printf("\n\n");

    Vlist.remove(vlist, vals);

    Vlist.for_each(vlist, PrintLink, NULL);

    Vlist.free(vlist);

    return 0;
}
