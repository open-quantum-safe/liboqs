/**
 * @file symmetric.c
 * @brief Cryptographic primitives: an extendable-output function (XOF), plus hash functions built on SHA3-256 and
 * SHA3-512.
 */

#include "symmetric.h"
#include <oqs/sha3.h>
#include <stdint.h>

/**
 * @brief Initializes a SHAKE256 XOF context with a given seed.
 *
 * @param[out] xof_ctx   Pointer to the XOF context to be initialized.
 * @param[in]  seed      Pointer to the input seed.
 * @param[in]  seed_size Size of the seed in bytes.
 */
void xof_init(shake256_xof_ctx *xof_ctx, const uint8_t *seed, uint32_t seed_size) {
    OQS_SHA3_shake256_inc_init(xof_ctx);
    OQS_SHA3_shake256_inc_absorb(xof_ctx, seed, seed_size);
    OQS_SHA3_shake256_inc_finalize(xof_ctx);
}

/**
 * @brief Initializes a SHAKE256 XOF context for KEM seed expansion, binding the
 *        parameter set through the Reed-Solomon dimension k.
 *
 * Absorbs seed || (k as a single byte), per HQC-KEM.KeyGen_internal and
 * HQC-KEM.Decaps_internal, so a seed reused across parameter sets yields
 * unrelated keys.
 *
 * @param[out] xof_ctx Pointer to the XOF context to be initialized.
 * @param[in]  seed    Pointer to the SEED_BYTES-byte KEM seed.
 * @param[in]  seed_size Size of the seed in bytes.
 * @param[in]  domain_separator Domain separator.
 */
void xof_init_dom_sep(shake256_xof_ctx *xof_ctx, const uint8_t *seed, uint32_t seed_size, uint8_t domain_separator) {
    OQS_SHA3_shake256_inc_init(xof_ctx);
    OQS_SHA3_shake256_inc_absorb(xof_ctx, seed, seed_size);
    OQS_SHA3_shake256_inc_absorb(xof_ctx, &domain_separator, 1);
    OQS_SHA3_shake256_inc_finalize(xof_ctx);
}

/**
 * @brief Extracts pseudorandom bytes from a SHAKE256 XOF context.
 *
 * @param[in,out] xof_ctx     Pointer to the initialized XOF context.
 * @param[out]    output      Pointer to the buffer where the output bytes will be written.
 * @param[in]     output_size Number of bytes to extract.
 *
 * @details This function squeezes the specified number of pseudorandom bytes from
 * the SHAKE256 XOF context and stores them in the provided output buffer.
 * The context must have been initialized beforehand using `xof_init()`.
 */
void xof_get_bytes(shake256_xof_ctx *xof_ctx, uint8_t *output, uint32_t output_size) {
    OQS_SHA3_shake256_inc_squeeze(output, output_size, xof_ctx);
}

void xof_ctx_release(shake256_xof_ctx *xof_ctx) {
    OQS_SHA3_shake256_inc_ctx_release(xof_ctx);
}

/**
 * @brief Compute the hash function G (SHA3-512) with domain separation.
 *
 * @param[out] output        Buffer (64 bytes) to receive the hash output.
 * @param[in]  hash_ek_kem   Hash of the KEM encapsulation key.
 * @param[in]  m             Message bytes.
 * @param[in]  salt          Salt value.
 */
void hash_g(uint8_t *output, const uint8_t hash_ek_kem[SEED_BYTES], const uint8_t m[PARAM_K],
            const uint8_t salt[SALT_BYTES]) {
    sha3_512_ctx g_hash_ctx = {0};
    uint8_t i_domain = HQC_G_FCT_DOMAIN;
    OQS_SHA3_sha3_512_inc_init(&g_hash_ctx);
    OQS_SHA3_sha3_512_inc_absorb(&g_hash_ctx, hash_ek_kem, SEED_BYTES);
    OQS_SHA3_sha3_512_inc_absorb(&g_hash_ctx, m, PARAM_K);
    OQS_SHA3_sha3_512_inc_absorb(&g_hash_ctx, salt, SALT_BYTES);
    OQS_SHA3_sha3_512_inc_absorb(&g_hash_ctx, &i_domain, 1);
    OQS_SHA3_sha3_512_inc_finalize(output, &g_hash_ctx);
    OQS_SHA3_sha3_512_inc_ctx_release(&g_hash_ctx);
}

/**
 * @brief Compute the hash function H (SHA3-256) with domain separation.
 *
 * @param[out] output      Buffer (32 bytes) to receive the hash output.
 * @param[in]  ek_kem      Encapsulation key of the KEM.
 */
void hash_h(uint8_t *output, const uint8_t ek_kem[PUBLIC_KEY_BYTES_KEM]) {
    sha3_256_ctx h_hash_ctx = {0};
    uint8_t h_domain = HQC_H_FCT_DOMAIN;
    OQS_SHA3_sha3_256_inc_init(&h_hash_ctx);
    OQS_SHA3_sha3_256_inc_absorb(&h_hash_ctx, ek_kem, PUBLIC_KEY_BYTES_KEM);
    OQS_SHA3_sha3_256_inc_absorb(&h_hash_ctx, &h_domain, 1);
    OQS_SHA3_sha3_256_inc_finalize(output, &h_hash_ctx);
    OQS_SHA3_sha3_256_inc_ctx_release(&h_hash_ctx);
}

/**
 * @brief Computes the hash function I (SHA3-512) with domain separation.
 *
 * @param[out] output Pointer to the buffer where the 64-byte hash output will be stored.
 * @param[in]  seed   Pointer to the input seed to be hashed.
 *
 * @details This function implements the random oracle `I` as specified,
 * using the SHA3-512 hash function. It produces a 64-byte output from the given seed.
 */
void hash_i(uint8_t *output, const uint8_t *seed) {
    sha3_512_ctx i_hash_ctx = {0};
    uint8_t i_domain = HQC_I_FCT_DOMAIN;
    OQS_SHA3_sha3_512_inc_init(&i_hash_ctx);
    OQS_SHA3_sha3_512_inc_absorb(&i_hash_ctx, seed, SEED_BYTES);
    OQS_SHA3_sha3_512_inc_absorb(&i_hash_ctx, &i_domain, 1);
    OQS_SHA3_sha3_512_inc_finalize(output, &i_hash_ctx);
    OQS_SHA3_sha3_512_inc_ctx_release(&i_hash_ctx);
}

/**
 * @brief Compute the hash function J (SHA3-256) with domain separation.
 *
 * @param[out] output       Buffer (32 bytes) to receive the hash output.
 * @param[in]  hash_ek_kem  Hash of the KEM encapsulation key.
 * @param[in]  sigma        The string sigma.
 * @param[in]  u            Pointer to the ciphertext u vector.
 * @param[in]  v            Pointer to the ciphertext v vector.
 * @param[in]  salt         Pointer to the ciphertext salt.
 */
void hash_j(uint8_t *output, const uint8_t hash_ek_kem[SEED_BYTES], const uint8_t sigma[PARAM_K], const uint64_t *u,
            const uint64_t *v, const uint8_t salt[SALT_BYTES]) {
    sha3_256_ctx k_hash_ctx = {0};
    uint8_t k_domain = HQC_J_FCT_DOMAIN;
    OQS_SHA3_sha3_256_inc_init(&k_hash_ctx);
    OQS_SHA3_sha3_256_inc_absorb(&k_hash_ctx, hash_ek_kem, SEED_BYTES);
    OQS_SHA3_sha3_256_inc_absorb(&k_hash_ctx, sigma, PARAM_K);
    OQS_SHA3_sha3_256_inc_absorb(&k_hash_ctx, (const uint8_t *)u, VEC_N_SIZE_BYTES);
    OQS_SHA3_sha3_256_inc_absorb(&k_hash_ctx, (const uint8_t *)v, VEC_N1N2_SIZE_BYTES);
    OQS_SHA3_sha3_256_inc_absorb(&k_hash_ctx, salt, SALT_BYTES);
    OQS_SHA3_sha3_256_inc_absorb(&k_hash_ctx, &k_domain, 1);
    OQS_SHA3_sha3_256_inc_finalize(output, &k_hash_ctx);
    OQS_SHA3_sha3_256_inc_ctx_release(&k_hash_ctx);
}
