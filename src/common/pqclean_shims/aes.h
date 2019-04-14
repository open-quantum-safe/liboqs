#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdlib.h>

#include <oqs/aes.h>

#define AES128_KEYBYTES 16
#define AES192_KEYBYTES 24
#define AES256_KEYBYTES 32
#define AESCTR_NONCEBYTES 12
#define AES_BLOCKBYTES 16

typedef void * aes128ctx;

static void aes128_keyexp(aes128ctx *r, const unsigned char *key) {
    OQS_AES128_load_schedule(key, r, 1);
}

static void aes128_ecb(unsigned char *out, const unsigned char *in, size_t nblocks, aes128ctx *ctx) {
    OQS_AES128_ECB_enc_sch(in, nblocks * AES_BLOCKBYTES, *ctx, out);
    OQS_AES128_free_schedule(*ctx);
    // FIXME: PQClean AES API expects that aes128_ecb can be called multiple 
    // times with the same key schedule, but this instantiation does not, since
    // it frees the key schedule immediately
}

#endif
