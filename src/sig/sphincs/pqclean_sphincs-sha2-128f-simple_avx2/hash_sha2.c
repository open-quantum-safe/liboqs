#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash.h"
#include "params.h"
#include "sha2.h"
#include "utils.h"

#define SPX_SHAX_OUTPUT_BYTES SPX_SHA256_OUTPUT_BYTES
#define SPX_SHAX_BLOCK_BYTES SPX_SHA256_BLOCK_BYTES
#define shaX_inc_init sha256_inc_init
#define shaX_inc_blocks sha256_inc_blocks
#define shaX_inc_finalize sha256_inc_finalize
#define shaX sha256
#define mgf1_X mgf1_256
#define shaXstate sha256ctx

/**
 * mgf1 function based on the SHA-256 hash function
 * Note that inlen should be sufficiently small that it still allows for
 * an array to be allocated on the stack. Typically 'in' is merely a seed.
 * Outputs outlen number of bytes
 */
void mgf1_256(unsigned char *out, unsigned long outlen,
              const unsigned char *in, unsigned long inlen) {
    PQCLEAN_VLA(uint8_t, inbuf, inlen + 4);
    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    uint32_t i;

    memcpy(inbuf, in, inlen);

    /* While we can fit in at least another full block of SHA256 output.. */
    for (i = 0; (i + 1)*SPX_SHA256_OUTPUT_BYTES <= outlen; i++) {
        u32_to_bytes(inbuf + inlen, i);
        sha256(out, inbuf, inlen + 4);
        out += SPX_SHA256_OUTPUT_BYTES;
    }
    /* Until we cannot anymore, and we fill the remainder. */
    if (outlen > i * SPX_SHA256_OUTPUT_BYTES) {
        u32_to_bytes(inbuf + inlen, i);
        sha256(outbuf, inbuf, inlen + 4);
        memcpy(out, outbuf, outlen - i * SPX_SHA256_OUTPUT_BYTES);
    }
}

/*
 * mgf1 function based on the SHA-512 hash function
 */
void mgf1_512(unsigned char *out, unsigned long outlen,
              const unsigned char *in, unsigned long inlen) {
    PQCLEAN_VLA(uint8_t, inbuf, inlen + 4);
    unsigned char outbuf[SPX_SHA512_OUTPUT_BYTES];
    uint32_t i;

    memcpy(inbuf, in, inlen);

    /* While we can fit in at least another full block of SHA512 output.. */
    for (i = 0; (i + 1)*SPX_SHA512_OUTPUT_BYTES <= outlen; i++) {
        u32_to_bytes(inbuf + inlen, i);
        sha512(out, inbuf, inlen + 4);
        out += SPX_SHA512_OUTPUT_BYTES;
    }
    /* Until we cannot anymore, and we fill the remainder. */
    if (outlen > i * SPX_SHA512_OUTPUT_BYTES) {
        u32_to_bytes(inbuf + inlen, i);
        sha512(outbuf, inbuf, inlen + 4);
        memcpy(out, outbuf, outlen - i * SPX_SHA512_OUTPUT_BYTES);
    }
}

/*
 * Computes PRF(pk_seed, sk_seed, addr).
 */
void prf_addr(unsigned char *out, const spx_ctx *ctx,
              const uint32_t addr[8]) {
    sha256ctx sha2_state;
    unsigned char buf[SPX_SHA256_ADDR_BYTES + SPX_N];
    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];

    /* Retrieve precomputed state containing pub_seed */
    sha256_inc_ctx_clone(&sha2_state, &ctx->state_seeded);

    /* Remainder: ADDR^c ‖ SK.seed */
    memcpy(buf, addr, SPX_SHA256_ADDR_BYTES);
    memcpy(buf + SPX_SHA256_ADDR_BYTES, ctx->sk_seed, SPX_N);

    sha256_inc_finalize(outbuf, &sha2_state, buf, SPX_SHA256_ADDR_BYTES + SPX_N);

    memcpy(out, outbuf, SPX_N);
}

/**
 * Computes the message-dependent randomness R, using a secret seed as a key
 * for HMAC, and an optional randomization value prefixed to the message.
 * This requires m to have at least SPX_SHAX_BLOCK_BYTES + SPX_N space
 * available in front of the pointer, i.e. before the message to use for the
 * prefix. This is necessary to prevent having to move the message around (and
 * allocate memory for it).
 */
void gen_message_random(unsigned char *R, const unsigned char *sk_prf,
                        const unsigned char *optrand,
                        const unsigned char *m, size_t mlen,
                        const spx_ctx *ctx) {
    (void)ctx;

    unsigned char buf[SPX_SHAX_BLOCK_BYTES + SPX_SHAX_OUTPUT_BYTES];
    shaXstate state;
    int i;

    /* This implements HMAC-SHA */
    for (i = 0; i < SPX_N; i++) {
        buf[i] = 0x36 ^ sk_prf[i];
    }
    memset(buf + SPX_N, 0x36, SPX_SHAX_BLOCK_BYTES - SPX_N);

    shaX_inc_init(&state);
    shaX_inc_blocks(&state, buf, 1);

    memcpy(buf, optrand, SPX_N);

    /* If optrand + message cannot fill up an entire block */
    if (SPX_N + mlen < SPX_SHAX_BLOCK_BYTES) {
        memcpy(buf + SPX_N, m, mlen);
        shaX_inc_finalize(buf + SPX_SHAX_BLOCK_BYTES, &state,
                          buf, mlen + SPX_N);
    }
    /* Otherwise first fill a block, so that finalize only uses the message */
    else {
        memcpy(buf + SPX_N, m, SPX_SHAX_BLOCK_BYTES - SPX_N);
        shaX_inc_blocks(&state, buf, 1);

        m += SPX_SHAX_BLOCK_BYTES - SPX_N;
        mlen -= SPX_SHAX_BLOCK_BYTES - SPX_N;
        shaX_inc_finalize(buf + SPX_SHAX_BLOCK_BYTES, &state, m, mlen);
    }

    for (i = 0; i < SPX_N; i++) {
        buf[i] = 0x5c ^ sk_prf[i];
    }
    memset(buf + SPX_N, 0x5c, SPX_SHAX_BLOCK_BYTES - SPX_N);

    shaX(buf, buf, SPX_SHAX_BLOCK_BYTES + SPX_SHAX_OUTPUT_BYTES);
    memcpy(R, buf, SPX_N);
}

/**
 * Computes the message hash using R, the public key, and the message.
 * Outputs the message digest and the index of the leaf. The index is split in
 * the tree index and the leaf index, for convenient copying to an address.
 */
void hash_message(unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
                  const unsigned char *R, const unsigned char *pk,
                  const unsigned char *m, size_t mlen,
                  const spx_ctx *ctx) {
    (void)ctx;
#define SPX_TREE_BITS (SPX_TREE_HEIGHT * (SPX_D - 1))
#define SPX_TREE_BYTES ((SPX_TREE_BITS + 7) / 8)
#define SPX_LEAF_BITS SPX_TREE_HEIGHT
#define SPX_LEAF_BYTES ((SPX_LEAF_BITS + 7) / 8)
#define SPX_DGST_BYTES (SPX_FORS_MSG_BYTES + SPX_TREE_BYTES + SPX_LEAF_BYTES)

    unsigned char seed[2 * SPX_N + SPX_SHAX_OUTPUT_BYTES];

    /* Round to nearest multiple of SPX_SHAX_BLOCK_BYTES */
#define SPX_INBLOCKS (((SPX_N + SPX_PK_BYTES + SPX_SHAX_BLOCK_BYTES - 1) & \
                       -SPX_SHAX_BLOCK_BYTES) / SPX_SHAX_BLOCK_BYTES)
    unsigned char inbuf[SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES];

    unsigned char buf[SPX_DGST_BYTES];
    unsigned char *bufp = buf;
    shaXstate state;

    shaX_inc_init(&state);

    // seed: SHA-X(R ‖ PK.seed ‖ PK.root ‖ M)
    memcpy(inbuf, R, SPX_N);
    memcpy(inbuf + SPX_N, pk, SPX_PK_BYTES);

    /* If R + pk + message cannot fill up an entire block */
    if (SPX_N + SPX_PK_BYTES + mlen < SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES) {
        memcpy(inbuf + SPX_N + SPX_PK_BYTES, m, mlen);
        shaX_inc_finalize(seed + 2 * SPX_N, &state, inbuf, SPX_N + SPX_PK_BYTES + mlen);
    }
    /* Otherwise first fill a block, so that finalize only uses the message */
    else {
        memcpy(inbuf + SPX_N + SPX_PK_BYTES, m,
               SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES - SPX_N - SPX_PK_BYTES);
        shaX_inc_blocks(&state, inbuf, SPX_INBLOCKS);

        m += SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES - SPX_N - SPX_PK_BYTES;
        mlen -= SPX_INBLOCKS * SPX_SHAX_BLOCK_BYTES - SPX_N - SPX_PK_BYTES;
        shaX_inc_finalize(seed + 2 * SPX_N, &state, m, (size_t)mlen);
    }

    // H_msg: MGF1-SHA-X(R ‖ PK.seed ‖ seed)
    memcpy(seed, R, SPX_N);
    memcpy(seed + SPX_N, pk, SPX_N);

    /* By doing this in two steps, we prevent hashing the message twice;
       otherwise each iteration in MGF1 would hash the message again. */
    mgf1_X(bufp, SPX_DGST_BYTES, seed, 2 * SPX_N + SPX_SHAX_OUTPUT_BYTES);

    memcpy(digest, bufp, SPX_FORS_MSG_BYTES);
    bufp += SPX_FORS_MSG_BYTES;

    *tree = bytes_to_ull(bufp, SPX_TREE_BYTES);
    *tree &= (~(uint64_t)0) >> (64 - SPX_TREE_BITS);
    bufp += SPX_TREE_BYTES;

    *leaf_idx = (uint32_t)bytes_to_ull(bufp, SPX_LEAF_BYTES);
    *leaf_idx &= (~(uint32_t)0) >> (32 - SPX_LEAF_BITS);
}
