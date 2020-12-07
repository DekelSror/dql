#ifndef dql_keys_h
#define dql_keys_h

typedef void* keys_t;

typedef void* secret_key_t;
typedef void* public_key_t;
typedef void* signature_t;

typedef void* keypair_t;

typedef struct 
{
    keys_t(*create)(unsigned long, int);
    void(*free)(keys_t);
    //
    keypair_t(*make_keypair)(keys_t);
    void(*free_keypair)(keypair_t);    
    signature_t(*sign)(const keypair_t, const unsigned char*, unsigned);
    int(*verify)(const keypair_t, const unsigned char* , size_t , const signature_t);
    void(*free_signature)(signature_t);
    void(*get_pk)(const keypair_t);
} keys_api_t;


extern const keys_api_t Keys;

#endif // dql_keys_h
