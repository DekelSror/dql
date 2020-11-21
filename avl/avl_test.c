#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // getpid
#include <math.h>

#include "avl.h"
#include "memblocks.h"

// #define avl_debug

#ifndef avl_debug
#define avl_debug
#endif // avl_debug

static int nums[] = { 1, 2, 3 };
static size_t height_errors = 0;

static void CheckHeight(const avl_t avl)
{
    size_t h = TreeHeight(avl); 
    size_t tree_size = Avl.size(avl);
    size_t max_nodes_for_h = pow(2, h) - 1;

    if (tree_size > max_nodes_for_h)
    {
        ++height_errors;
        printf("height is %lu, max is %lu got %lu\n", h, max_nodes_for_h, tree_size);
    }
}

static void Shuffle(int *array, size_t n)
{
    for (size_t i = 0; i < n - 1; i++) 
    {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}


// fills buf with n integers (1 .. n) and shuffles if shuffle != 0
void RangeInts(int* buf, size_t n, char shuffle)
{
    for (int i = 0; i < n; i++)
    {
        buf[i] = i + 1;
    }

    if (shuffle) Shuffle(buf, n);
}

static int MaxInt(const void* a, const void* b)
{
    return (const int*)a - (const int*)b;
}

static void LL(void)
{
    avl_t avl = Avl.create(MaxInt);

    Avl.insert(avl, nums + 2);
    Avl.insert(avl, nums + 1);
    Avl.insert(avl, nums);

    Avl.free(avl);
}

static void LR(void)
{
    avl_t avl = Avl.create(MaxInt);

    Avl.insert(avl, nums);
    Avl.insert(avl, nums + 2);
    Avl.insert(avl, nums + 1);

    Avl.free(avl);
}

static void RL(void)
{
    avl_t avl = Avl.create(MaxInt);

    Avl.insert(avl, nums + 2);
    Avl.insert(avl, nums);
    Avl.insert(avl, nums + 1);

    Avl.free(avl);
}

static void RR(void)
{
    avl_t avl = Avl.create(MaxInt);

    Avl.insert(avl, nums + 2);
    Avl.insert(avl, nums + 1);
    Avl.insert(avl, nums);

    Avl.free(avl);
}

static void InsertFind(void)
{
    const size_t test_size = 10;
    avl_t avl = Avl.create(MaxInt);
    int* many = malloc(test_size * sizeof(int));

    for (size_t i = 0; i < test_size; i++)
    {
        many[i] = rand() % test_size;
        Avl.insert(avl, many + i);
        CheckHeight(avl);
    }

    for (size_t i = 0; i < test_size; i++)
    {
        void* found = Avl.find(avl, many + i);

        if (NULL == found)
        {
            printf("failed to find %d\n", many[i]);
        }
    }

    Avl.free(avl);
    free(many);
}

static void RemoveFromEmpty(void)
{
    avl_t avl = Avl.create(MaxInt);

    Avl.remove(avl, NULL);

    Avl.free(avl);
}

static void InsertFindRemoveFind(void)
{
    avl_t avl = Avl.create(MaxInt);

    int elem = 6;

    Avl.insert(avl, &elem);

    void* found = Avl.find(avl, &elem);

    if (NULL == found)
    {
        printf("failed to find only elem\n");
    }

    Avl.remove(avl, &elem);

    found = Avl.find(avl, &elem);

    if (NULL != found)
    {
        printf("found non-existing elem\n");
    }

    Avl.free(avl);
}


static void InsertRemoveAll(void)
{
    avl_t avl = Avl.create(MaxInt);
    const size_t big_test_size = 6300;

    int* the_big = malloc(sizeof(int) * big_test_size);
    RangeInts(the_big, big_test_size, 0);
    size_t i = 0;
    height_errors = 0;
    
    for (i = 0; i < big_test_size; i++)
    {
        CheckHeight(avl);
        Avl.insert(avl, the_big + i);
    }

    printf("insertion height errors: %lu\n", height_errors);
    
    for (size_t i = 0; i < big_test_size; i++)
    {
        // printf("removing elem %lu\n", i);
        CheckHeight(avl);
        Avl.remove(avl, the_big + i);
    }

    printf("removal height errors: %lu\n", height_errors);
    printf("size after all dat: %lu\n", Avl.size(avl));
    height_errors = 0;
    

    free(the_big);
    Avl.free(avl);
}

void PrintNode(void* _data, void* args)
{
    size_t index = *(size_t*)args;
    printf("index: %lu node value: %d\n", index, *(int*)_data);
    ++*(size_t*)args;
}

memblocks_t node_pool;

void* AllocateNode(void)
{
    return Memblocks.get_block(node_pool);
}

void ReleaseNode(void* node)
{
    return Memblocks.free_block(node);
}

void TestOrder(void)
{
    const size_t test_size = 11;
    int* ns = malloc(test_size * sizeof(int));
    void* buf_node_pool = malloc(Memblocks.reqired_buf_size(32, 15));
    node_pool = Memblocks.create(buf_node_pool, 32, 14);
    avl_t avl = Avl.create_ext(MaxInt, AllocateNode, ReleaseNode);
    RangeInts(ns, test_size, 0);

    for (size_t i = 0; i < test_size; i++)
    {
        Avl.insert(avl, ns + i);
        printf("after %d root is %d\n", ns[i], AvlRootValue(avl));
    }
    
    size_t index = 0;
    Avl.for_each(avl, PrintNode, &index);

    Avl.free(avl);
    free(ns);
    free(buf_node_pool);
}


int main(void)
{
    srand(4 + getpid());

// test rotations
    // LL();
    // LR();
    // RL();
    // RR();

    // RemoveFromEmpty();
    // InsertFind();
    // InsertFindRemoveFind();
    TestOrder();
    // InsertRemoveAll();

    return 0;
} 
