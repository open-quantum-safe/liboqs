#include <stdint.h>
#include <string.h>

#include "address.h"
#include "haraka.h"
#include "hash.h"
#include "params.h"
#include "utils.h"

void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_initialize_hash_function(
    hash_state *hash_state_seeded,
    const unsigned char *pub_seed, const unsigned char *sk_seed) {
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_tweak_constants(hash_state_seeded, pub_seed, sk_seed, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N);
}

/* The haraka implementation is stack based and won't be replaced in PQClean/OQS,
   so we don't need to do anything */
void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_destroy_hash_function(
    hash_state *hash_state_seeded) { // NOLINT(readability-non-const-parameter)
    (void)hash_state_seeded;
}

/*
 * Computes PRF(key, addr), given a secret key of PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N bytes and an address
 */
void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_prf_addr(
    unsigned char *out, const unsigned char *key, const uint32_t addr[8],
    const hash_state *hash_state_seeded) {
    unsigned char buf[PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_ADDR_BYTES];
    /* Since PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N may be smaller than 32, we need a temporary buffer. */
    unsigned char outbuf[32];

    (void)key; /* Suppress an 'unused parameter' warning. */

    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_addr_to_bytes(buf, addr);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka256_sk(outbuf, buf, hash_state_seeded);
    memcpy(out, outbuf, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N);
}

/**
 * Computes the message-dependent randomness R, using a secret seed and an
 * optional randomization value as well as the message.
 */
void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_gen_message_random(
    unsigned char *R,
    const unsigned char *sk_prf, const unsigned char *optrand,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded) {
    uint8_t s_inc[65];

    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_init(s_inc);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_absorb(s_inc, sk_prf, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N, hash_state_seeded);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_absorb(s_inc, optrand, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N, hash_state_seeded);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_absorb(s_inc, m, mlen, hash_state_seeded);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_finalize(s_inc);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_squeeze(R, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N, s_inc, hash_state_seeded);
}

/**
 * Computes the message hash using R, the public key, and the message.
 * Outputs the message digest and the index of the leaf. The index is split in
 * the tree index and the leaf index, for convenient copying to an address.
 */
void PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_hash_message(
    unsigned char *digest, uint64_t *tree, uint32_t *leaf_idx,
    const unsigned char *R, const unsigned char *pk,
    const unsigned char *m, size_t mlen,
    const hash_state *hash_state_seeded) {
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_BITS (PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_HEIGHT * (PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_D - 1))
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_BYTES ((PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_BITS + 7) / 8)
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_LEAF_BITS PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_HEIGHT
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_LEAF_BYTES ((PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_LEAF_BITS + 7) / 8)
#define PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_DGST_BYTES (PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_FORS_MSG_BYTES + PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_BYTES + PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_LEAF_BYTES)

    unsigned char buf[PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_DGST_BYTES];
    unsigned char *bufp = buf;
    uint8_t s_inc[65];

    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_init(s_inc);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_absorb(s_inc, R, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N, hash_state_seeded);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_absorb(s_inc, pk + PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_N, hash_state_seeded);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_absorb(s_inc, m, mlen, hash_state_seeded);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_finalize(s_inc);
    PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_haraka_S_inc_squeeze(buf, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_DGST_BYTES, s_inc, hash_state_seeded);

    memcpy(digest, bufp, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_FORS_MSG_BYTES);
    bufp += PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_FORS_MSG_BYTES;

    *tree = PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_bytes_to_ull(bufp, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_BYTES);
    *tree &= (~(uint64_t)0) >> (64 - PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_BITS);
    bufp += PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_TREE_BYTES;

    *leaf_idx = (uint32_t)PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_bytes_to_ull(
                    bufp, PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_LEAF_BYTES);
    *leaf_idx &= (~(uint32_t)0) >> (32 - PQCLEAN_SPHINCSHARAKA256FROBUST_AESNI_LEAF_BITS);
}
