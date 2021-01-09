
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <openssl/sha.h>

#include "hash.h"

#define resize_load_factor_threshold (1.0)

#define cache_line_bytes (64)

typedef struct 
{
    size_t _key;
    void* _value;
} pair_t;

static size_t TableHash(const char* key, size_t key_len)
{
    unsigned char buf[SHA512_DIGEST_LENGTH];
    SHA512(key, key_len, buf);

    return *(size_t*)buf;
}

typedef struct hash_node
{
    pair_t _kv[3];
    struct hnode_md
    {
        unsigned char _is_full;
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


static pair_t* Find(_hash_t* h, const string_t key);


static hash_t Create(size_t capacity)
{
    _hash_t* h = malloc(sizeof(*h) + (capacity * sizeof(hnode_t)));
    h->_capacity = capacity;
    h->_load = 0;    

    memset(h->_table, 0, capacity * sizeof(hnode_t));
    
    return h;
}

static int Insert(hash_t _h, const string_t key, void* value)
{
    _hash_t* h = _h;

    const size_t hashed_key = TableHash(String.chars(key), String.len(key));

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

    for (int i = 0; i < 3; ++i)
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

static int Set(hash_t _h, const string_t key, void* value)
{
    // set for existing key
    pair_t* kv = Find(_h, key);
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

static pair_t* Find(_hash_t* h, const string_t key)
{
    const size_t hashed_key = TableHash(String.chars(key), String.len(key));

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

static void* Get(hash_t _h, const string_t key)
{
    pair_t* kv = Find(_h, key);    

    return kv ? kv->_value : NULL;
}

static void* Remove(hash_t _h, const string_t key)
{
    _hash_t* h = _h;

    const size_t hashed_key = TableHash(String.chars(key), String.len(key));
    hnode_t* node = h->_table + (hashed_key % h->_capacity);

    while (NULL != node)
    {
        for (unsigned i = 0; i < 3; i++)
        {
            if (node->_kv[i]._key == hashed_key) 
            {
                void* value = node->_kv[i]._value;

                node->_metadata._is_full = 0;

                node->_kv[i]._key = 0;
                node->_kv[i]._value = NULL;        

                --h->_load;    

                return value;
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

    free(node);
    node = NULL;
}

static void Free(hash_t _h)
{
    _hash_t* h = _h;

    for (size_t i = 0; i < h->_capacity; i++)
    {
        FreeNodes(h->_table[i]._next);
    }

    free(h);
    h = NULL;
}

static void ForEach(hash_t _this, void(*fn)(void*, void*), void* fn_arg)
{
    _hash_t* this = _this;

    for (size_t i = 0; i < this->_capacity; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            void* val = this->_table[i]._kv[j]._value;
            if (val != NULL)  fn(val, fn_arg);
        }
    }    
}



const hash_api_t Hash = { Create, Free, Set, Get, Remove, ForEach };