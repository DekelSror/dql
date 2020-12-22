#include <openssl/sha.h>
#include <openssl/pem.h> // NID_md5 
// #include <openssl/rsa.h>
#include <openssl/ecdsa.h>
#include <stdlib.h>

#include "keys.h"

#define keys_thisify _keys_t* this = _this;

typedef unsigned char Sha256[SHA256_DIGEST_LENGTH];


typedef struct 
{
    unsigned _size;
    unsigned char _signature[1];
} _signature_t;

typedef struct
{
    BIGNUM _exponent;
    int _bits;
} _keys_t;

typedef struct 
{
    Sha256 _pk;
    EC_KEY* _sk;
} _keypair_t;

#include <stdio.h>

static keypair_t GetKeyPair(keys_t _this)
{
    _keypair_t* this = malloc(sizeof(*this));

    unsigned char n[sizeof(int)];
    sprintf(n, "%d", rand());

    SHA256(n, 4, this->_sk);    
    this->_sk = EC_KEY_new();

    EC_GROUP *group= EC_GROUP_new_by_curve_name(NID_secp192k1);
    EC_KEY_set_group(this->_pk, group);
    EC_KEY_generate_key(this->_pk);
    ECDSA_do_sign(this->_sk, SHA256_DIGEST_LENGTH, this->_pk);
    
    return this;
}

static void FreeKeyPair(keypair_t _this)
{
    free(_this);
    _this = NULL;
}

static signature_t Sign (const keypair_t _keypair, const unsigned char* data, unsigned data_len)
{
    _keypair_t* keypair = _keypair;
    const int sigsize = RSA_size(keypair->_sk);
    _signature_t* this = malloc(sizeof(*this) + sigsize);

    // int* sigsize = NULL;
    const int res = RSA_sign(NID_md5, data, data_len, this->_signature, &(this->_size), keypair->_sk);

    if (res == -1)
    {
        free(this);
        this = NULL;
    }

    return this;
}

static void FreeSignature(signature_t _this)
{
    free(_this);
    _this = NULL;
}

static void GetPublicKey(const keypair_t _this)
{
    _keypair_t* this = _this;

    RSA_print_fp(stdout, this->_pk, 0);
}

static int Verify(const keypair_t _keypair, const unsigned char* data, size_t data_len, const signature_t signature)
{
    _keypair_t* keypair = _keypair;
    _signature_t* sig = signature;

    return RSA_verify(NID_md5, data, data_len, sig->_signature, sig->_size, keypair->_pk);
}

static keys_t Create(size_t exponent, int bits)
{
    _keys_t* this = malloc(sizeof(*this));

    BN_init(&(this->_exponent));

    BN_set_word(&(this->_exponent), exponent);
    this->_bits = bits;

    return this;
}

static void Free(keys_t _this)
{
    keys_thisify
    BN_free(&(this->_exponent)); // i know, right?
    free(_this);
    _this = NULL;
}

const keys_api_t Keys = {Create, Free, GetKeyPair, FreeKeyPair, Sign, Verify, FreeSignature, GetPublicKey}; 
