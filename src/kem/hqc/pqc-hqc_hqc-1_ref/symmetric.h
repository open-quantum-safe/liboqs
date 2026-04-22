/**
 * @file symmetric.h
 * @brief Header file of symmetric.c
 */

#ifndef HQC_SYMMETRIC_H
#define HQC_SYMMETRIC_H

#include <stdint.h>
#include <oqs/sha3.h>
#include "data_structures.h"
#include "parameters.h"
#include "namespace.h"

/**
 * @typedef sha3_512_ctx
 * @brief Incremental SHA3-512 hashing context.
 *
 * Alias for the low-level `sha3_512incctx`, used to
 * accumulate input and squeeze out a 512-bit digest.
 */
typedef OQS_SHA3_sha3_512_inc_ctx sha3_512_ctx;

/**
 * @typedef sha3_256_ctx
 * @brief Incremental SHA3-256 hashing context.
 *
 * Alias for the low-level `sha3_256incctx`, used to
 * accumulate input and squeeze out a 256-bit digest.
 */
typedef OQS_SHA3_sha3_256_inc_ctx sha3_256_ctx;

/**
 * @typedef shake256_xof_ctx
 * @brief Incremental SHAKE-256 extendable-output context.
 *
 * Alias for `shake256incctx`, used to absorb input and
 * generate arbitrary-length XOF output.
 */
typedef OQS_SHA3_shake256_inc_ctx shake256_xof_ctx;

/** @def HQC_PRNG_DOMAIN
 *  @brief Domain separator for HQC prng function.
 */
#define HQC_PRNG_DOMAIN 0

/** @def HQC_XOF_DOMAIN
 *  @brief Domain separator for HQC extendable-output function.
 */
#define HQC_XOF_DOMAIN 1

/** @def HQC_G_FCT_DOMAIN
 *  @brief Domain separator for the G(·) function in HQC.
 */
#define HQC_G_FCT_DOMAIN 0

/** @def HQC_H_FCT_DOMAIN
 *  @brief Domain separator for the H(·) function in HQC.
 */
#define HQC_H_FCT_DOMAIN 1

/** @def HQC_I_FCT_DOMAIN
 *  @brief Domain separator for the I(·) function in HQC.
 */
#define HQC_I_FCT_DOMAIN 2

/** @def HQC_J_FCT_DOMAIN
 *  @brief Domain separator for the J(·) function in HQC.
 */
#define HQC_J_FCT_DOMAIN 3

#define prng_init PQCHQC_NAMESPACE(prng_init)
#define prng_get_bytes PQCHQC_NAMESPACE(prng_get_bytes)
#define xof_init PQCHQC_NAMESPACE(xof_init)
#define xof_get_bytes PQCHQC_NAMESPACE(xof_get_bytes)
#define xof_ctx_release PQCHQC_NAMESPACE(xof_ctx_release)
#define hash_g PQCHQC_NAMESPACE(hash_g)
#define hash_h PQCHQC_NAMESPACE(hash_h)
#define hash_i PQCHQC_NAMESPACE(hash_i)
#define hash_j PQCHQC_NAMESPACE(hash_j)

void prng_init(uint8_t *entropy_input, uint8_t *personalization_string, uint32_t enlen, uint32_t perlen);
void prng_get_bytes(uint8_t *output, uint32_t outlen);

void xof_init(shake256_xof_ctx *xof_ctx, const uint8_t *seed, uint32_t seed_size);
void xof_get_bytes(shake256_xof_ctx *xof_ctx, uint8_t *output, uint32_t output_size);
void xof_ctx_release(shake256_xof_ctx *xof_ctx);

void hash_g(uint8_t *output, const uint8_t h_ek[SEED_BYTES], const uint8_t m[VEC_K_SIZE_BYTES],
            const uint8_t salt[SALT_BYTES]);
void hash_h(uint8_t *output, const uint8_t pk[PUBLIC_KEY_BYTES]);
void hash_i(uint8_t *output, const uint8_t *seed);
void hash_j(uint8_t *output, const uint8_t h_ek[SEED_BYTES], const uint8_t sigma[VEC_K_SIZE_BYTES],
            const ciphertext_kem_t *c_kem);

#endif  // HQC_SYMMETRIC_H
