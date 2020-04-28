#include <stdint.h>
#include <string.h>

#include "address.h"
#include "params.h"
#include "thash.h"

#include "sha2.h"
#include "sha256.h"

/**
 * Takes an array of inblocks concatenated arrays of PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N bytes.
 */
static void thash(
    unsigned char *out, unsigned char *buf,
    const unsigned char *in, unsigned int inblocks,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char outbuf[PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_OUTPUT_BYTES];
    unsigned char *bitmask = buf + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES + 4;
    sha256ctx sha2_state;
    unsigned int i;

    memcpy(buf, pub_seed, PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N);
    PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_compress_address(buf + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N, addr);
    /* MGF1 requires us to have 4 extra bytes in 'buf' */
    PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_mgf1(bitmask, inblocks * PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N, buf, PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES);

    /* Retrieve precomputed state containing pub_seed */
    sha256_inc_ctx_clone(&sha2_state, &hash_state_seeded->x1);

    for (i = 0; i < inblocks * PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N; i++) {
        buf[PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES + i] = in[i] ^ bitmask[i];
    }

    sha256_inc_finalize(outbuf, &sha2_state, buf + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N,
                        PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES + inblocks * PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N);
    memcpy(out, outbuf, PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N);
}

/* The wrappers below ensure that we use fixed-size buffers on the stack */

void PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_thash_1(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES + 4 + 1 * PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N];
    thash(out, buf, in, 1, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_thash_2(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES + 4 + 2 * PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N];
    thash(out, buf, in, 2, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_thash_WOTS_LEN(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES + 4 + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_WOTS_LEN * PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N];
    thash(out, buf, in, PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_WOTS_LEN, pub_seed, addr, hash_state_seeded);
}

void PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_thash_FORS_TREES(
    unsigned char *out, const unsigned char *in,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded) {

    unsigned char buf[PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_SHA256_ADDR_BYTES + 4 + PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_FORS_TREES * PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_N];
    thash(out, buf, in, PQCLEAN_SPHINCSSHA256128FROBUST_AVX2_FORS_TREES, pub_seed, addr, hash_state_seeded);
}
