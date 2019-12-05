#ifndef SPX_WOTS_H
#define SPX_WOTS_H

#include "hash_state.h"
#include "params.h"
#include <stdint.h>

/**
 * WOTS key generation. Takes a 32 byte seed for the private key, expands it to
 * a full WOTS private key and computes the corresponding public key.
 * It requires the seed pub_seed (used to generate bitmasks and hash keys)
 * and the address of this WOTS key pair.
 *
 * Writes the computed public key to 'pk'.
 */
void PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_wots_gen_pk(
    unsigned char *pk, const unsigned char *sk_seed,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded);

/**
 * Takes a n-byte message and the 32-byte seed for the private key to compute a
 * signature that is placed at 'sig'.
 */
void PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_wots_sign(
    unsigned char *sig, const unsigned char *msg,
    const unsigned char *sk_seed, const unsigned char *pub_seed,
    uint32_t addr[8], const hash_state *hash_state_seeded);

/**
 * Takes a WOTS signature and an n-byte message, computes a WOTS public key.
 *
 * Writes the computed public key to 'pk'.
 */
void PQCLEAN_SPHINCSHARAKA192FROBUST_CLEAN_wots_pk_from_sig(
    unsigned char *pk,
    const unsigned char *sig, const unsigned char *msg,
    const unsigned char *pub_seed, uint32_t addr[8],
    const hash_state *hash_state_seeded);

#endif
