// SPDX-License-Identifier: CC0 OR Apache-2.0
/// @file utils_prng.h
/// @brief the interface for adapting PRNG functions.
///
///

#ifndef _UTILS_PRNG_H_
#define _UTILS_PRNG_H_

#include <stdint.h>

#include "config.h"
#include "params.h"  // for macro _4ROUND_AES_

#include "utils_randombytes.h"   // declare randombytes(), including here for backward compatibility

//
// This is not for traditional PRNG functions. It is PRNG with PUBLIC INPUTS.
// It uses AES128CTR or CTR with only 4 rounds AES128.
//

#define AES128CTR_KEYLEN   16
#define AES128CTR_NONCELEN 16
#define AES128_BLOCKSIZE 16


#ifdef  __cplusplus
extern  "C" {
#endif

/////////////////  defination of prng_publicinputs_t  /////////////////////////////////


#ifdef _UTILS_PQM4_
// fixsliced implementation processes two blocks in parallel
#define RNG_OUTPUTLEN 32


#include "aes.h"
#ifdef _4ROUND_AES_
#include "aes4-publicinputs.h"
#else
#include "aes-publicinputs.h"
#endif


typedef struct {
    unsigned used;
    uint32_t ctr;
    unsigned char buf[RNG_OUTPUTLEN];
    #ifdef _4ROUND_AES_
    aes128_4rounds_ctx_publicinputs ctx;
    #else
    aes128ctx_publicinputs ctx;
    #endif
} prng_publicinputs_t;


#elif defined(_UTILS_AESNI_)

// TODO: check that this is actually the best value here;
#define RNG_OUTPUTLEN 64

#include "x86aesni.h"

typedef struct {
    unsigned used;
    uint32_t ctr;
    unsigned char   buf[RNG_OUTPUTLEN];
    unsigned key_offset; // for_16byte_align;
    #ifdef _4ROUND_AES_
    unsigned char   key[_AES128_4R_ROUNDKEY_BYTE + 16];
    #else  // round key of the normal aes256
    unsigned char   key[_AES128_ROUNDKEY_BYTE + 16];
    #endif
} prng_publicinputs_t;



#elif defined(_UTILS_NEONAES_)

// TODO: check that this is actually the best value here;
#define RNG_OUTPUTLEN 64

#include "aes_neonaes.h"
typedef struct {
    unsigned used;
    uint32_t ctr;
    unsigned char   buf[RNG_OUTPUTLEN];
    #ifdef _4ROUND_AES_
    unsigned char   key[5 * 16];
    #else  // round key of the normal aes128
    unsigned char   key[11 * 16];
    #endif
} prng_publicinputs_t;


#elif defined(_UTILS_NEONBSAES_)

#define RNG_OUTPUTLEN 128
typedef struct {
    unsigned used;
    uint32_t ctr;
    unsigned char   buf[RNG_OUTPUTLEN];
    #ifdef _4ROUND_AES_
    uint32_t key[ 40 ];
    #else  // round key of the normal aes128
    uint32_t key[ 88 ];
    #endif
} prng_publicinputs_t;

#elif defined(_UTILS_OQS_)
#define RNG_OUTPUTLEN 16


typedef struct {
    unsigned used;
    uint32_t ctr;
    unsigned char   buf[RNG_OUTPUTLEN];

    void *ctx;
} prng_publicinputs_t;

#else

//
// default
//


// TODO: check that this is actually the best value here;
// for aes256ctr.c it needs to be 64, but for openssl, we may change it
#define RNG_OUTPUTLEN 64


typedef struct {
    unsigned used;
    uint32_t ctr;
    unsigned char   buf[RNG_OUTPUTLEN];

    #ifdef _4ROUND_AES_
    uint32_t key[40];
    #else  // round key of the normal aes128
    uint32_t key[88];
    #endif
} prng_publicinputs_t;



#endif


///////////////// end of defination of prng_publicinputs_t  /////////////////////////////////
#define prng_set_publicinputs PQOV_NAMESPACE(prng_set_publicinputs)
int prng_set_publicinputs(prng_publicinputs_t *ctx, const unsigned char prng_seed[16]);

#define prng_gen_publicinputs PQOV_NAMESPACE(prng_gen_publicinputs)
int prng_gen_publicinputs(prng_publicinputs_t *ctx, unsigned char *out, unsigned long outlen);

#define prng_skip_publicinputs PQOV_NAMESPACE(prng_skip_publicinputs)
void prng_skip_publicinputs(prng_publicinputs_t *ctx, unsigned long outlen);


#define prng_release_publicinputs PQOV_NAMESPACE(prng_release_publicinputs)
void prng_release_publicinputs(prng_publicinputs_t *ctx);

#ifdef  __cplusplus
}
#endif



#endif // _UTILS_PRNG_H_


