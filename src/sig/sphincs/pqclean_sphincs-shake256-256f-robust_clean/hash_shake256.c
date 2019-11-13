#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash.h"
#include "params.h"
#include "utils.h"

#include "fips202.h"

/* For SHAKE256, there is no immediate reason to initialize at the start,
   so this function is an empty operation. */
void PQCLEAN_SPHINCSSHAKE256256FROBUST_CLEAN_initialize_hash_function(
    hash_state *hash_state_seeded, // NOLINT(readability-non-const-parameter)
    const unsigned char *pub_seed, const unsigned char *sk_seed) {
    (void)hash_state_seeded; /* Suppress an 'unused parameter' warning. */
    (void)pub_seed; /* Suppress an 'unused parameter' warning. */
    (void)sk_seed; /* Suppress an 'unused parameter' warning. */
}

/*
 * Computes PRF(key, addr), given a secret key of SPX_N bytes and an address
 */
void PQCLEAN_SPHINCSSHAKE256256FROBUST_CLEAN_prf_addr(
    unsigned char *out, const unsigned char *key, const uint32_t addr[8],
    const hash_state *hash_state_seeded) {
    unsigned char buf[SPX_N + SPX_ADDR_BYTES];

    memcpy(buf, key, SPX_N);
    PQCLEAN_SPHINCSSHAKE256256FROBUST_CLEAN_addr_to_bytes(buf + SPX_N, addr);

    shake256(out, SPX_N, buf, SPX_N + SPX_ADDR_BYTES);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}

/**
 * Computes the message-dependent randomness R, using a secret seed and an
 * optional randomization value as well as the message.
 */
void PQCLEAN_SPHINCSSHAKE256256FROBUST_CLEAN_gen_message_random(
    unsigned char *R,
    const unsigned char *sk_prf, const unsigned char *optrand,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded) {
    shake256incctx state;

    shake256_inc_init(&state);
    shake256_inc_absorb(&state, sk_prf, SPX_N);
    shake256_inc_absorb(&state, optrand, SPX_N);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(R, SPX_N, &state);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}

/**
 * Computes the message hash using R, the public key, and the message.
 * Outputs the message digest and the index of the leaf. The index is split in
 * the tree index and the leaf index, for convenient copying to an address.
 */
void PQCLEAN_SPHINCSSHAKE256256FROBUST_CLEAN_hash_message(
    unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
    const unsigned char *R, const unsigned char *pk,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded) {
#define SPX_TREE_BITS (SPX_TREE_HEIGHT * (SPX_D - 1))
#define SPX_TREE_BYTES ((SPX_TREE_BITS + 7) / 8)
#define SPX_LEAF_BITS SPX_TREE_HEIGHT
#define SPX_LEAF_BYTES ((SPX_LEAF_BITS + 7) / 8)
#define SPX_DGST_BYTES (SPX_FORS_MSG_BYTES + SPX_TREE_BYTES + SPX_LEAF_BYTES)

    unsigned char buf[SPX_DGST_BYTES];
    unsigned char *bufp = buf;
    shake256incctx state;

    shake256_inc_init(&state);
    shake256_inc_absorb(&state, R, SPX_N);
    shake256_inc_absorb(&state, pk, SPX_PK_BYTES);
    shake256_inc_absorb(&state, m, mlen);
    shake256_inc_finalize(&state);
    shake256_inc_squeeze(buf, SPX_DGST_BYTES, &state);

    memcpy(digest, bufp, SPX_FORS_MSG_BYTES);
    bufp += SPX_FORS_MSG_BYTES;

    *tree = PQCLEAN_SPHINCSSHAKE256256FROBUST_CLEAN_bytes_to_ull(
                bufp, SPX_TREE_BYTES);
    *tree &= (~(uint64_t)0) >> (64 - SPX_TREE_BITS);
    bufp += SPX_TREE_BYTES;

    *leaf_idx = (uint32_t)PQCLEAN_SPHINCSSHAKE256256FROBUST_CLEAN_bytes_to_ull(
                    bufp, SPX_LEAF_BYTES);
    *leaf_idx &= (~(uint32_t)0) >> (32 - SPX_LEAF_BITS);

    (void)hash_state_seeded; /* Prevent unused parameter warning. */
}
