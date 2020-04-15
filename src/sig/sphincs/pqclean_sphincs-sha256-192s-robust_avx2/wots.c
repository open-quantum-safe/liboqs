#include <stdint.h>
#include <string.h>

#include "address.h"
#include "hash.h"
#include "hash_state.h"
#include "hashx8.h"
#include "params.h"
#include "thash.h"
#include "thashx8.h"
#include "utils.h"
#include "wots.h"

// TODO clarify address expectations, and make them more uniform.
// TODO i.e. do we expect types to be set already?
// TODO and do we expect modifications or copies?

/**
 * Computes the starting value for a chain, i.e. the secret key.
 * Expects the address to be complete up to the chain address.
 */
static void wots_gen_sk(unsigned char *sk, const unsigned char *sk_seed,
                        uint32_t wots_addr[8], const hash_state *state_seeded) {
    /* Make sure that the hash address is actually zeroed. */
    PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_set_hash_addr(wots_addr, 0);

    /* Generate sk element. */
    PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_prf_addr(sk, sk_seed, wots_addr, state_seeded);
}

/**
 * 8-way parallel version of wots_gen_sk; expects 8x as much space in sk
 */
static void wots_gen_skx8(unsigned char *skx8, const unsigned char *sk_seed,
                          uint32_t wots_addrx8[8 * 8]) {
    unsigned int j;

    /* Make sure that the hash address is actually zeroed. */
    for (j = 0; j < 8; j++) {
        PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_set_hash_addr(wots_addrx8 + j * 8, 0);
    }

    /* Generate sk element. */
    PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_prf_addrx8(skx8 + 0 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            skx8 + 1 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            skx8 + 2 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            skx8 + 3 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            skx8 + 4 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            skx8 + 5 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            skx8 + 6 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            skx8 + 7 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
            sk_seed, wots_addrx8);
}

/**
 * Computes the chaining function.
 * out and in have to be n-byte arrays.
 *
 * Interprets in as start-th value of the chain.
 * addr has to contain the address of the chain.
 */
static void gen_chain(unsigned char *out, const unsigned char *in,
                      unsigned int start, unsigned int steps,
                      const unsigned char *pub_seed, uint32_t addr[8],
                      const hash_state *state_seeded) {
    uint32_t i;

    /* Initialize out with the value at position 'start'. */
    memcpy(out, in, PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N);

    /* Iterate 'steps' calls to the hash function. */
    for (i = start; i < (start + steps) && i < PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_W; i++) {
        PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_set_hash_addr(addr, i);
        PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_thash_1(out, out, pub_seed, addr, state_seeded);
    }
}

/**
 * 8-way parallel version of gen_chain; expects 8x as much space in out, and
 * 8x as much space in inx8. Assumes start and step identical across chains.
 */
static void gen_chainx8(unsigned char *outx8, const unsigned char *inx8,
                        unsigned int start, unsigned int steps,
                        const unsigned char *pub_seed, uint32_t addrx8[8 * 8],
                        const hash_state *state_seeded) {
    uint32_t i;
    unsigned int j;

    /* Initialize outx8 with the value at position 'start'. */
    memcpy(outx8, inx8, 8 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N);

    /* Iterate 'steps' calls to the hash function. */
    for (i = start; i < (start + steps) && i < PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_W; i++) {
        for (j = 0; j < 8; j++) {
            PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_set_hash_addr(addrx8 + j * 8, i);
        }
        PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_thashx8_1(outx8 + 0 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 1 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 2 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 3 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 4 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 5 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 6 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 7 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 0 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 1 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 2 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 3 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 4 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 5 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 6 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                outx8 + 7 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                pub_seed, addrx8, state_seeded);
    }
}

/**
 * base_w algorithm as described in draft.
 * Interprets an array of bytes as integers in base w.
 * This only works when log_w is a divisor of 8.
 */
static void base_w(unsigned int *output, const int out_len, const unsigned char *input) {
    int in = 0;
    int out = 0;
    unsigned char total = 0;
    int bits = 0;
    int consumed;

    for (consumed = 0; consumed < out_len; consumed++) {
        if (bits == 0) {
            total = input[in];
            in++;
            bits += 8;
        }
        bits -= PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LOGW;
        output[out] = (unsigned int)(total >> bits) & (PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_W - 1);
        out++;
    }
}

/* Computes the WOTS+ checksum over a message (in base_w). */
static void wots_checksum(unsigned int *csum_base_w, const unsigned int *msg_base_w) {
    unsigned int csum = 0;
    unsigned char csum_bytes[(PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN2 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LOGW + 7) / 8];
    unsigned int i;

    /* Compute checksum. */
    for (i = 0; i < PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN1; i++) {
        csum += PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_W - 1 - msg_base_w[i];
    }

    /* Convert checksum to base_w. */
    /* Make sure expected empty zero bits are the least significant bits. */
    csum = csum << (8 - ((PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN2 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LOGW) % 8));
    PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_ull_to_bytes(csum_bytes, sizeof(csum_bytes), csum);
    base_w(csum_base_w, PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN2, csum_bytes);
}

/* Takes a message and derives the matching chain lengths. */
static void chain_lengths(unsigned int *lengths, const unsigned char *msg) {
    base_w(lengths, PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN1, msg);
    wots_checksum(lengths + PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN1, lengths);
}

/**
 * WOTS key generation. Takes a 32 byte sk_seed, expands it to WOTS private key
 * elements and computes the corresponding public key.
 * It requires the seed pub_seed (used to generate bitmasks and hash keys)
 * and the address of this WOTS key pair.
 *
 * Writes the computed public key to 'pk'.
 */
void PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_wots_gen_pk(unsigned char *pk, const unsigned char *sk_seed,
        const unsigned char *pub_seed, uint32_t addr[8],
        const hash_state *state_seeded) {
    uint32_t i;
    unsigned int j;

    uint32_t addrx8[8 * 8];
    unsigned char pkbuf[8 * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N];

    for (j = 0; j < 8; j++) {
        memcpy(addrx8 + j * 8, addr, sizeof(uint32_t) * 8);
    }

    /* The last iteration typically does not have complete set of 4 chains,
       but because we use pkbuf, this is not an issue -- we still do as many
       in parallel as possible. */
    for (i = 0; i < ((PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN + 7) & ~0x7); i += 8) {
        for (j = 0; j < 8; j++) {
            PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_set_chain_addr(addrx8 + j * 8, i + j);
        }
        wots_gen_skx8(pkbuf, sk_seed, addrx8);
        gen_chainx8(pkbuf, pkbuf, 0, PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_W - 1, pub_seed, addrx8, state_seeded);
        for (j = 0; j < 8; j++) {
            if (i + j < PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN) {
                memcpy(pk + (i + j)*PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N, pkbuf + j * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N, PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N);
            }
        }
    }
}

/**
 * Takes a n-byte message and the 32-byte sk_see to compute a signature 'sig'.
 */
void PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_wots_sign(unsigned char *sig, const unsigned char *msg,
        const unsigned char *sk_seed, const unsigned char *pub_seed,
        uint32_t addr[8], const hash_state *state_seeded) {
    unsigned int lengths[PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN];
    uint32_t i;

    chain_lengths(lengths, msg);

    for (i = 0; i < PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN; i++) {
        PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_set_chain_addr(addr, i);
        wots_gen_sk(sig + i * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N, sk_seed, addr, state_seeded);
        gen_chain(sig + i * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N, sig + i * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N, 0, lengths[i], pub_seed, addr, state_seeded);
    }
}

/**
 * Takes a WOTS signature and an n-byte message, computes a WOTS public key.
 *
 * Writes the computed public key to 'pk'.
 */
void PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_wots_pk_from_sig(unsigned char *pk,
        const unsigned char *sig, const unsigned char *msg,
        const unsigned char *pub_seed, uint32_t addr[8],
        const hash_state *state_seeded) {
    unsigned int lengths[PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN];
    uint32_t i;

    chain_lengths(lengths, msg);

    for (i = 0; i < PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_LEN; i++) {
        PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_set_chain_addr(addr, i);
        gen_chain(pk + i * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N, sig + i * PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_N,
                  lengths[i], PQCLEAN_SPHINCSSHA256192SROBUST_AVX2_WOTS_W - 1 - lengths[i], pub_seed, addr, state_seeded);
    }
}
