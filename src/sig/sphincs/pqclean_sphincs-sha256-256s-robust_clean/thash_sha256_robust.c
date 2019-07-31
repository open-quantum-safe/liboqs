#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thash.h"

#include "sha2.h"
#include "sha256.h"

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
static void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
    unsigned char *out, unsigned char *buf,
    const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char outbuf[SPX_SHA256_OUTPUT_BYTES];
    unsigned char *bitmask = buf + SPX_N + SPX_SHA256_ADDR_BYTES + 4;
    sha256ctx sha2_state;
    unsigned int i;

    memcpy(buf, pub_seed, SPX_N);
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_compress_address(buf + SPX_N, addr);
    /* MGF1 requires us to have 4 extra bytes in 'buf' */
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_mgf1(bitmask, inblocks * SPX_N, buf, SPX_N + SPX_SHA256_ADDR_BYTES);

    /* Retrieve precomputed state containing pub_seed */
    sha256_inc_clone_state(&sha2_state, hash_state_seeded);

    for (i = 0; i < inblocks * SPX_N; i++) {
        buf[SPX_N + SPX_SHA256_ADDR_BYTES + i] = in[i] ^ bitmask[i];
    }

    sha256_inc_finalize(outbuf, &sha2_state, buf + SPX_N,
                        SPX_SHA256_ADDR_BYTES + inblocks * SPX_N);
    memcpy(out, outbuf, SPX_N);
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + 4 + 1 * SPX_N];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, 1, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + 4 + 2 * SPX_N];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, 2, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + 4 + SPX_WOTS_LEN * SPX_N];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, SPX_WOTS_LEN, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const sha256ctx *hash_state_seeded) {

    unsigned char buf[SPX_N + SPX_SHA256_ADDR_BYTES + 4 + SPX_FORS_TREES * SPX_N];
    PQCLEAN_SPHINCSSHA256256SROBUST_CLEAN_thash(
        out, buf, in, SPX_FORS_TREES, pub_seed, addr, hash_state_seeded);
}
