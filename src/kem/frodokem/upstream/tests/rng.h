//
//  rng.h
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//
//  Modified to use OpenSSL's AES256_ECB or a standalone implementation

#ifndef rng_h
#define rng_h

#include <stdio.h>
#include "../src/aes/aes.h"

#define RNG_SUCCESS      0
#define RNG_BAD_MAXLEN  -1
#define RNG_BAD_OUTBUF  -2
#define RNG_BAD_REQ_LEN -3

static __inline void AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer) {
#if !defined(USE_OPENSSL)
    uint8_t schedule[16*15];
    AES256_load_schedule(key, schedule);
    AES256_ECB_enc_sch(ctr, 16, schedule, buffer);
    AES256_free_schedule(schedule);
#else
    EVP_CIPHER_CTX *ctx;    
    int len; 
    
    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
    
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL))
        handleErrors();
    
    if(1 != EVP_EncryptUpdate(ctx, buffer, &len, ctr, 16))
        handleErrors();
    
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
#endif    
}

typedef struct {
    unsigned char   buffer[16];
    int             buffer_pos;
    unsigned long   length_remaining;
    unsigned char   key[32];
    unsigned char   ctr[16];
} AES_XOF_struct;

typedef struct {
    unsigned char   Key[32];
    unsigned char   V[16];
    int             reseed_counter;
} AES256_CTR_DRBG_struct;


void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *V);

void
randombytes_init(unsigned char *entropy_input,
                 unsigned char *personalization_string,
                 int security_strength);

int
randombytes(unsigned char *x, unsigned long long xlen);

#endif /* rng_h */
