#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thash.h"

#include "sha2.h"
#include "sha256.h"

/**
 * Takes an array of inblocks concatenated arrays of PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N bytes.
 */
static void PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash(
    unsigned char *out, unsigned char *buf,
    const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char outbuf[PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_SHA256_OUTPUT_BYTES];
    sha256ctx sha2_state;

    (void)pub_seed; /* Suppress an 'unused parameter' warning. */

    /* Retrieve precomputed state containing pub_seed */
    sha256_inc_ctx_clone(&sha2_state, hash_state_seeded);

    PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_compress_address(buf, addr);
    memcpy(buf + PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_SHA256_ADDR_BYTES, in, inblocks * PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N);

    sha256_inc_finalize(outbuf, &sha2_state, buf, PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N);
    memcpy(out, outbuf, PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N);
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_SHA256_ADDR_BYTES + 1 * PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N];
    PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash(
        out, buf, in, 1, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_SHA256_ADDR_BYTES + 2 * PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N];
    PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash(
        out, buf, in, 2, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_SHA256_ADDR_BYTES + PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_WOTS_LEN * PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N];
    PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash(
        out, buf, in, PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_WOTS_LEN, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_SHA256_ADDR_BYTES + PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_FORS_TREES * PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_N];
    PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_thash(
        out, buf, in, PQCLEAN_SPHINCSSHA256128SSIMPLE_CLEAN_FORS_TREES, pub_seed, addr, hash_state_seeded);
}
