#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thash.h"

#include "haraka.h"

/**
 * Takes an array of inblocks concatenated arrays of SPX_N bytes.
 */
static void PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash(
    unsigned char *out, unsigned char *buf,
    const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char outbuf[32];
    unsigned char buf_tmp[64];

    (void)pub_seed; /* Suppress an 'unused parameter' warning. */

    if (inblocks == 1) {
        /* F function */
        /* Since SPX_N may be smaller than 32, we need a temporary buffer. */
        memset(buf_tmp, 0, 64);
        PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_addr_to_bytes(buf_tmp, addr);
        memcpy(buf_tmp + SPX_ADDR_BYTES, in, SPX_N);

        PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_haraka512(outbuf, buf_tmp, hash_state_seeded);
        memcpy(out, outbuf, SPX_N);
    } else {
        /* All other tweakable hashes*/
        PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_addr_to_bytes(buf, addr);
        memcpy(buf + SPX_ADDR_BYTES, in, inblocks * SPX_N);

        PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_haraka_S(
            out, SPX_N, buf, SPX_ADDR_BYTES + inblocks * SPX_N, hash_state_seeded);
    }
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[SPX_ADDR_BYTES + 1 * SPX_N];
    PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash(
        out, buf, in, 1, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[SPX_ADDR_BYTES + 2 * SPX_N];
    PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash(
        out, buf, in, 2, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[SPX_ADDR_BYTES + SPX_WOTS_LEN * SPX_N];
    PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash(
        out, buf, in, SPX_WOTS_LEN, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[SPX_ADDR_BYTES + SPX_FORS_TREES * SPX_N];
    PQCLEAN_SPHINCSHARAKA128FSIMPLE_CLEAN_thash(
        out, buf, in, SPX_FORS_TREES, pub_seed, addr, hash_state_seeded);
}
