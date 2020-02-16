#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash.h"
#include "params.h"
#include "utils.h"

#include "fips202.h"

/* For SHAKE256, there is no immediate reason to initialize at the start,
   so this function is an empty operation. */
void PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_initialize_hash_function(
    hash_state *hash_state_seeded, // NOLINT(readability-non-const-parameter)
    const unsigned char *pub_seed, const unsigned char *sk_seed) {
    (void)hash_state_seeded; /* Suppress an 'unused parameter' warning. */
    (void)pub_seed; /* Suppress an 'unused parameter' warning. */
    (void)sk_seed; /* Suppress an 'unused parameter' warning. */
}

/* This is not necessary for SHAKE256, so we don't do anything */
void PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_destroy_hash_function(
    hash_state *hash_state_seeded) { // NOLINT(readability-non-const-parameter)
    (void)hash_state_seeded;
}

/*
 * Computes PRF(key, addr), given a secret key of PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N bytes and an address
 */
void PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_prf_addr(
    unsigned char *out, const unsigned char *key, const uint32_t addr[8],
    const hash_state *hash_state_seeded) {
    unsigned char buf[PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_ADDR_BYTES];

    memcpy(buf, key, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N);
    PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_addr_to_bytes(buf + PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N, addr);

    shake256(out, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N, buf, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N + PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_ADDR_BYTES);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}

/**
 * Computes the message-dependent randomness R, using a secret seed and an
 * optional randomization value as well as the message.
 */
void PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_gen_message_random(
    unsigned char *R,
    const unsigned char *sk_prf, const unsigned char *optrand,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded) {
    shake256incctx state;

    shake256_inc_init(&state);
    shake256_inc_absorb(&state, sk_prf, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N);
    shake256_inc_absorb(&state, optrand, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(R, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N, &state);
    shake256_inc_ctx_release(&state);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}

/**
 * Computes the message hash using R, the public key, and the message.
 * Outputs the message digest and the index of the leaf. The index is split in
 * the tree index and the leaf index, for convenient copying to an address.
 */
void PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_hash_message(
    unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
    const unsigned char *R, const unsigned char *pk,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded) {
#define PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_BITS (PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_HEIGHT * (PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_D - 1))
#define PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_BYTES ((PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_BITS + 7) / 8)
#define PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_LEAF_BITS PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_HEIGHT
#define PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_LEAF_BYTES ((PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_LEAF_BITS + 7) / 8)
#define PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_DGST_BYTES (PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_FORS_MSG_BYTES + PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_BYTES + PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_LEAF_BYTES)

    unsigned char buf[PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_DGST_BYTES];
    unsigned char *bufp = buf;
    shake256incctx state;

    shake256_inc_init(&state);
    shake256_inc_absorb(&state, R, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_N);
    shake256_inc_absorb(&state, pk, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_PK_BYTES);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(buf, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_DGST_BYTES, &state);
    shake256_inc_ctx_release(&state);

    memcpy(digest, bufp, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_FORS_MSG_BYTES);
    bufp += PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_FORS_MSG_BYTES;

    *tree = PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_bytes_to_ull(
                bufp, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_BYTES);
    *tree &= (~(uint64_t)0) >> (64 - PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_BITS);
    bufp += PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_TREE_BYTES;

    *leaf_idx = (uint32_t)PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_bytes_to_ull(
                    bufp, PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_LEAF_BYTES);
    *leaf_idx &= (~(uint32_t)0) >> (32 - PQCLEAN_SPHINCSSHAKE256128SSIMPLE_CLEAN_LEAF_BITS);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}
