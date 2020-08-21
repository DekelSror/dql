
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hash.h"

#define resize_load_factor_threshold (1.0)

#define cache_line_bytes (64)


static const size_t hash_fn_prime = 5381;

static size_t Basichash(const char *key, size_t key_len)
{
    assert(key_len);

    size_t h = hash_fn_prime;
    const char* end = key + key_len;

    while (key < end)
    {
        h += (h << 11) + *key;
        ++key;
    }

    return h;
}

struct pair_t
{
    size_t _key;
    void* _value;
};

typedef struct hash_node
{
    struct pair_t _kv[3];
    struct hnode_md
    {
        char _is_full;
        char _future[7];
    } _metadata;
    struct hash_node* _next;
} hnode_t;

typedef struct 
{
    size_t _capacity;
    size_t _load;
    hnode_t _table[];
} _hash_t;


static struct pair_t* Find(hash_t _h, const string_t* key);


static hash_t Create(size_t capacity)
{
    _hash_t* h = malloc(sizeof(*h) + (capacity * sizeof(hnode_t)));
    h->_capacity = capacity;
    h->_load = 0;    

    memset(h->_table, 0, capacity * sizeof(hnode_t));
    
    return h;
}

static int Insert(hash_t _h, const string_t* key, void* value)
{
    _hash_t* h = _h;

    const size_t hashed_key = Basichash(String.chars(key), String.len(key));

    hnode_t* node = h->_table + (hashed_key % h->_capacity);

    while (node->_metadata._is_full)
    {
        if (NULL == node->_next)
        {
            node->_next = malloc(sizeof(*node));

            if (NULL == node->_next) return 1;

            memset(node->_next, 0, sizeof(*node));
        }

        node = node->_next;
    }

    for (int i = 0; i < 3; ++i) // this is the good route
    {
        if (0 == node->_kv[i]._key)
        {
            node->_kv[i]._key = hashed_key;
            node->_kv[i]._value = value;
            if (2 == i) node->_metadata._is_full = 1;
            
            ++h->_load;

            return 0; 
        }
    }

    return 2; //madness
}

static int Set(hash_t _h, const string_t* key, void* value)
{
    // set for existing key
    struct pair_t* kv = Find(_h, key);
    if (NULL != kv) 
    {
        kv->_value = value;    
        return 0; 
    }
    else
    {
        return Insert(_h, key, value);
    }
}

static struct pair_t* Find(hash_t _h, const string_t* key)
{
    _hash_t* h = _h;

    const size_t hashed_key = Basichash(String.chars(key), String.len(key));

    hnode_t* node = h->_table + (hashed_key % h->_capacity);

    while (NULL != node)
    {
        for (unsigned i = 0; i < 3; i++)
        {
            if (node->_kv[i]._key == hashed_key) 
            {
                return node->_kv + i;
            }
        }

        node = node->_next;
    }

    return NULL;
}

static void* Get(hash_t _h, const string_t* key)
{
    struct pair_t* kv = Find(_h, key);    

    return kv ? kv->_value : NULL;
}

static void* Remove(hash_t _h, const string_t* key)
{
    _hash_t* h = _h;

    const size_t hashed_key = Basichash(String.chars(key), String.len(key));

    hnode_t* node = h->_table + (hashed_key % h->_capacity);

    while (NULL != node)
    {
        for (int i = 0; i < 3; i++)
        {
            if (node->_kv[i]._key == hashed_key) 
            {
                node->_kv[i]._key = 0;

                --h->_load;
                node->_metadata._is_full = 0;

                return node->_kv[i]._value;
            }
        }

        node = node->_next;
    }

    return NULL;
}

static void FreeNodes(hnode_t* node)
{
    if (NULL == node) return;

    FreeNodes(node->_next);

    node->_next = NULL;
    free(node);

    node = NULL;
}

static void Free(hash_t _h)
{
    _hash_t* h = _h;

    for (size_t i = 0; i < h->_capacity; i++)
    {
        FreeNodes(h->_table + i);
    }

    free(h);

    h = NULL;
}



const hash_api_t Hash = { Create, Free, Set, Get, Remove };