#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash_state.h"
#include "params.h"
#include "thash.h"

#include "fips202.h"

/**
 * Takes an array of inblocks concatenated arrays of PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N bytes.
 */
static void PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash(
    unsigned char *out, unsigned char *buf,
    const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8]) {

    memcpy(buf, pub_seed, PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N);
    PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_addr_to_bytes(buf + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N, addr);
    memcpy(buf + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_ADDR_BYTES, in, inblocks * PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N);

    shake256(out, PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N, buf, PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N);
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_ADDR_BYTES + 1 * PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N];
    PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash(
        out, buf, in, 1, pub_seed, addr);

    (void)hash_state_seeded;  /* Avoid unused parameter warning. */
}

void PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_ADDR_BYTES + 2 * PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N];
    PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash(
        out, buf, in, 2, pub_seed, addr);

    (void)hash_state_seeded;  /* Avoid unused parameter warning. */
}

void PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_ADDR_BYTES + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_WOTS_LEN * PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N];
    PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash(
        out, buf, in, PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_WOTS_LEN, pub_seed, addr);

    (void)hash_state_seeded;  /* Avoid unused parameter warning. */
}

void PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_ADDR_BYTES + PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_FORS_TREES * PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_N];
    PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_thash(
        out, buf, in, PQCLEAN_SPHINCSSHAKE256192SSIMPLE_AVX2_FORS_TREES, pub_seed, addr);

    (void)hash_state_seeded;  /* Avoid unused parameter warning. */
}
