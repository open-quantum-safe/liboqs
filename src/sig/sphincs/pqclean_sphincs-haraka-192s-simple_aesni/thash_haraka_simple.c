#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thash.h"

#include "haraka.h"

/**
 * Takes an array of inblocks concatenated arrays of PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N bytes.
 */
static void PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash(
    unsigned char *out, unsigned char *buf,
    const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char outbuf[32];
    unsigned char buf_tmp[64];

    (void)pub_seed; /* Suppress an 'unused parameter' warning. */

    if (inblocks == 1) {
        /* F function */
        /* Since PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N may be smaller than 32, we need a temporary buffer. */
        memset(buf_tmp, 0, 64);
        PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_addr_to_bytes(buf_tmp, addr);
        memcpy(buf_tmp + PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_ADDR_BYTES, in, PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N);

        PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_haraka512(outbuf, buf_tmp, hash_state_seeded);
        memcpy(out, outbuf, PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N);
    } else {
        /* All other tweakable hashes*/
        PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_addr_to_bytes(buf, addr);
        memcpy(buf + PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_ADDR_BYTES, in, inblocks * PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N);

        PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_haraka_S(
            out, PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N, buf, PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N, hash_state_seeded);
    }
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_ADDR_BYTES + 1 * PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N];
    PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash(
        out, buf, in, 1, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_ADDR_BYTES + 2 * PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N];
    PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash(
        out, buf, in, 2, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_ADDR_BYTES + PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_WOTS_LEN * PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N];
    PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash(
        out, buf, in, PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_WOTS_LEN, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_ADDR_BYTES + PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_FORS_TREES * PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_N];
    PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_thash(
        out, buf, in, PQCLEAN_SPHINCSHARAKA192SSIMPLE_AESNI_FORS_TREES, pub_seed, addr, hash_state_seeded);
}
