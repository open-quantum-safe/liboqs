#include "shake_ds.h"

/**
 * @file shake_ds.c
 * @brief Implementation SHAKE-256 with incremental API and domain separation
 */

/**
 * @brief SHAKE-256 with incremental API and domain separation
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[out] state Internal state of SHAKE
 * @param[in] output Pointer to output
 * @param[in] input Pointer to input
 * @param[in] inlen length of input in bytes
 * @param[in] domain byte for domain separation
 */
void PQCLEAN_HQC128_CLEAN_shake256_512_ds(shake256incctx *state, uint8_t *output, const uint8_t *input, size_t inlen, uint8_t domain) {
    /* Init state */
    shake256_inc_init(state);

    /* Absorb input */
    shake256_inc_absorb(state, input, inlen);

    /* Absorb domain separation byte */
    shake256_inc_absorb(state, &domain, 1);

    /* Finalize */
    shake256_inc_finalize(state);

    /* Squeeze output */
    shake256_inc_squeeze(output, 512 / 8, state);

    /* Release ctx */
    shake256_inc_ctx_release(state);
}
