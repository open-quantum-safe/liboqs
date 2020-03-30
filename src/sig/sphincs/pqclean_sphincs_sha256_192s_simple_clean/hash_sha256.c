#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash.h"
#include "params.h"
#include "utils.h"

#include "sha2.h"
#include "sha256.h"

/* For SHA256, there is no immediate reason to initialize at the start,
   so this function is an empty operation. */
void PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_initialize_hash_function(
    hash_state *hash_state_seeded,
    const unsigned char *pub_seed, const unsigned char *sk_seed) {
    PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_seed_state(hash_state_seeded, pub_seed);
    (void)sk_seed; /* Suppress an 'unused parameter' warning. */
}

/* Clean up hash state */
void PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_destroy_hash_function(hash_state *hash_state_seeded) {
    sha256_inc_ctx_release(hash_state_seeded);
}

/*
 * Computes PRF(key, addr), given a secret key of PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N bytes and an address
 */
void PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_prf_addr(
    unsigned char *out, const unsigned char *key, const uint32_t addr[8],
    const hash_state *hash_state_seeded) {
    unsigned char buf[PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_ADDR_BYTES];
    unsigned char outbuf[PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_OUTPUT_BYTES];

    memcpy(buf, key, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);
    PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_compress_address(buf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N, addr);

    sha256(outbuf, buf, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_ADDR_BYTES);
    memcpy(out, outbuf, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}

/**
 * Computes the message-dependent randomness R, using a secret seed as a key
 * for HMAC, and an optional randomization value prefixed to the message.
 * This requires m to have at least PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N space
 * available in front of the pointer, i.e. before the message to use for the
 * prefix. This is necessary to prevent having to move the message around (and
 * allocate memory for it).
 */
void PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_gen_message_random(
    unsigned char *R,
    const unsigned char *sk_prf, const unsigned char *optrand,
    const unsigned char *m, size_t mlen, const hash_state *hash_state_seeded) {
    unsigned char buf[PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_OUTPUT_BYTES];
    sha256ctx state;
    int i;

    /* This implements HMAC-SHA256 */
    for (i = 0; i < PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N; i++) {
        buf[i] = 0x36 ^ sk_prf[i];
    }
    memset(buf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N, 0x36, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);

    sha256_inc_init(&state);
    sha256_inc_blocks(&state, buf, 1);

    memcpy(buf, optrand, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);

    /* If optrand + message cannot fill up an entire block */
    if (PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + mlen < PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES) {
        memcpy(buf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N, m, mlen);
        sha256_inc_finalize(buf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES, &state,
                            buf, mlen + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);
    }
    /* Otherwise first fill a block, so that finalize only uses the message */
    else {
        memcpy(buf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N, m, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);
        sha256_inc_blocks(&state, buf, 1);

        m += PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N;
        mlen -= PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N;
        sha256_inc_finalize(buf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES, &state, m, mlen);
    }

    for (i = 0; i < PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N; i++) {
        buf[i] = 0x5c ^ sk_prf[i];
    }
    memset(buf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N, 0x5c, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);

    sha256(buf, buf, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_OUTPUT_BYTES);
    memcpy(R, buf, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}

/**
 * Computes the message hash using R, the public key, and the message.
 * Outputs the message digest and the index of the leaf. The index is split in
 * the tree index and the leaf index, for convenient copying to an address.
 */
void PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_hash_message(
    unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
    const unsigned char *R, const unsigned char *pk,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded) {
#define PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_BITS (PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_HEIGHT * (PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_D - 1))
#define PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_BYTES ((PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_BITS + 7) / 8)
#define PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_LEAF_BITS PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_HEIGHT
#define PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_LEAF_BYTES ((PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_LEAF_BITS + 7) / 8)
#define PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_DGST_BYTES (PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_FORS_MSG_BYTES + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_BYTES + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_LEAF_BYTES)

    unsigned char seed[PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_OUTPUT_BYTES + 4];

    /* Round to nearest multiple of PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES */
#define PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_INBLOCKS (((PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - 1) & \
        -PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES) / PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES)
    unsigned char inbuf[PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_INBLOCKS * PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES];

    unsigned char buf[PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_DGST_BYTES];
    unsigned char *bufp = buf;
    sha256ctx state;

    sha256_inc_init(&state);

    memcpy(inbuf, R, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N);
    memcpy(inbuf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N, pk, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES);

    /* If R + pk + message cannot fill up an entire block */
    if (PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES + mlen < PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_INBLOCKS * PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES) {
        memcpy(inbuf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES, m, mlen);
        sha256_inc_finalize(seed, &state, inbuf, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES + mlen);
    }
    /* Otherwise first fill a block, so that finalize only uses the message */
    else {
        memcpy(inbuf + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES, m,
               PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_INBLOCKS * PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES);
        sha256_inc_blocks(&state, inbuf, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_INBLOCKS);

        m += PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_INBLOCKS * PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES;
        mlen -= PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_INBLOCKS * PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_BLOCK_BYTES - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_N - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_PK_BYTES;
        sha256_inc_finalize(seed, &state, m, mlen);
    }

    /* By doing this in two steps, we prevent hashing the message twice;
       otherwise each iteration in MGF1 would hash the message again. */
    PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_mgf1(bufp, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_DGST_BYTES, seed, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_SHA256_OUTPUT_BYTES);

    memcpy(digest, bufp, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_FORS_MSG_BYTES);
    bufp += PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_FORS_MSG_BYTES;

    *tree = PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_bytes_to_ull(bufp, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_BYTES);
    *tree &= (~(uint64_t)0) >> (64 - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_BITS);
    bufp += PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_TREE_BYTES;

    *leaf_idx = (uint32_t)PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_bytes_to_ull(
                    bufp, PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_LEAF_BYTES);
    *leaf_idx &= (~(uint32_t)0) >> (32 - PQCLEAN_SPHINCSSHA256192SSIMPLE_CLEAN_LEAF_BITS);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}
