#include "fips202x2.h"
#include "params.h"
#include "symmetric.h"
#include <stddef.h>
#include <stdint.h>

/*************************************************
* Name:        kyber_shake128_absorb
*
* Description: Absorb step of the SHAKE128 specialized for the Kyber context.
*
* Arguments:   - keccak_state *state: pointer to (uninitialized) output
*                                     Keccak state
*              - const uint8_t *seed: pointer to KYBER_SYMBYTES input
*                                     to be absorbed into state
*              - uint8_t i            additional byte of input
*              - uint8_t j            additional byte of input
**************************************************/
void neon_kyber_shake128_absorb(keccakx2_state *state,
                                const uint8_t seed[KYBER_SYMBYTES],
                                uint8_t x1, uint8_t x2,
                                uint8_t y1, uint8_t y2) {
    unsigned int i;
    uint8_t extseed1[KYBER_SYMBYTES + 2 + 14];
    uint8_t extseed2[KYBER_SYMBYTES + 2 + 14];

    for (i = 0; i < KYBER_SYMBYTES; i++) {
        extseed1[i] = seed[i];
        extseed2[i] = seed[i];
    }
    extseed1[KYBER_SYMBYTES  ] = x1;
    extseed1[KYBER_SYMBYTES + 1] = y1;

    extseed2[KYBER_SYMBYTES  ] = x2;
    extseed2[KYBER_SYMBYTES + 1] = y2;

    shake128x2_absorb(state, extseed1, extseed2, KYBER_SYMBYTES + 2);
}

/*************************************************
* Name:        kyber_shake256_prf
*
* Description: Usage of SHAKE256 as a PRF, concatenates secret and public input
*              and then generates outlen bytes of SHAKE256 output
*
* Arguments:   - uint8_t *out:       pointer to output
*              - size_t outlen:      number of requested output bytes
*              - const uint8_t *key: pointer to the key
*                                    (of length KYBER_SYMBYTES)
*              - uint8_t nonce:      single-byte nonce (public PRF input)
**************************************************/
void neon_kyber_shake256_prf(uint8_t *out1, uint8_t *out2,
                             size_t outlen,
                             const uint8_t key[KYBER_SYMBYTES],
                             uint8_t nonce1, uint8_t nonce2) {
    unsigned int i;
    uint8_t extkey1[KYBER_SYMBYTES + 1 + 15];
    uint8_t extkey2[KYBER_SYMBYTES + 1 + 15];

    for (i = 0; i < KYBER_SYMBYTES; i++) {
        extkey1[i] = key[i];
        extkey2[i] = key[i];
    }

    extkey1[i] = nonce1;
    extkey2[i] = nonce2;

    shake256x2(out1, out2, outlen, extkey1, extkey2, KYBER_SYMBYTES + 1);
}
