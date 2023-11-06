#include "domains.h"
#include "fips202.h"
#include "shake_prng.h"

/**
 * @file shake_prng.c
 * @brief Implementation of SHAKE-256 based seed expander
 */

/**
 * @brief Initialise a SHAKE-256 based seed expander
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[out] state Keccak internal state and a counter
 * @param[in] seed A seed
 * @param[in] seedlen The seed bytes length
 */
void PQCLEAN_HQC128_CLEAN_seedexpander_init(seedexpander_state *state, const uint8_t *seed, size_t seedlen) {
    uint8_t domain = SEEDEXPANDER_DOMAIN;
    shake256_inc_init(state);
    shake256_inc_absorb(state, seed, seedlen);
    shake256_inc_absorb(state, &domain, 1);
    shake256_inc_finalize(state);
}

/**
 * @brief A SHAKE-256 based seed expander
 *
 * Derived from function SHAKE_256 in fips202.c
 * Squeezes Keccak state by 64-bit blocks (hardware version compatibility)
 *
 * @param[out] state Internal state of SHAKE
 * @param[out] output The XOF data
 * @param[in] outlen Number of bytes to return
 */
void PQCLEAN_HQC128_CLEAN_seedexpander(seedexpander_state *state, uint8_t *output, size_t outlen) {
    const size_t bsize = sizeof(uint64_t);
    const size_t remainder = outlen % bsize;
    uint8_t tmp[sizeof(uint64_t)];
    shake256_inc_squeeze(output, outlen - remainder, state);
    if (remainder != 0) {
        shake256_inc_squeeze(tmp, bsize, state);
        output += outlen - remainder;
        for (size_t i = 0; i < remainder; ++i) {
            output[i] = tmp[i];
        }
    }
}

/**
 * @brief Release the seed expander context
 * @param[in] state Internal state of the seed expander
 */
void PQCLEAN_HQC128_CLEAN_seedexpander_release(seedexpander_state *state) {
    shake256_inc_ctx_release(state);
}
