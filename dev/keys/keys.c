#include <openssl/sha.h>
#include <openssl/pem.h> // NID_md5 
#include <openssl/rsa.h>

#include "keys.h"

#define keys_thisify _keys_t* this = _this;

typedef unsigned char Sha512[64];


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
    RSA* _pk;
    RSA* _sk;
} _keypair_t;

static keypair_t GetKeyPair(keys_t _this)
{
    keys_thisify

    RSA* raw_pair = RSA_new();
    RSA_generate_key_ex(raw_pair, this->_bits, &(this->_exponent), NULL);

    _keypair_t* keypair = malloc(sizeof(*keypair));

    keypair->_pk = RSAPublicKey_dup(raw_pair);
    keypair->_sk = RSAPrivateKey_dup(raw_pair);

    RSA_free(raw_pair);

    return keypair;
}

static void FreeKeyPair(keypair_t _this)
{
    _keypair_t* this = _this;

    RSA_free(this->_pk);
    this->_pk = NULL;
    RSA_free(this->_sk);
    this->_sk = NULL;

    free(this);
    this = NULL;
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
