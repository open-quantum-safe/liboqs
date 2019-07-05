/**
* \file sha3.h
* \brief <b>SHA3 header definition</b> \n
* Contains the public api and documentation for SHA3 digest and SHAKE implementations.
*
* \author John Underhill
* \date December 29, 2017
* \remarks For usage examples, see sha3_test.h
*/

#ifndef OQS_SHA3_H
#define OQS_SHA3_H

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* SHA3 */

#define OQS_SHA3_SHA3_256_RATE 136

/**
* \brief Process a message with SHA3-256 and return the hash code in the output byte array.
*
* \warning The output array must be at least 32 bytes in length.
*
* \param output The output byte array
* \param input The message input byte array
* \param inplen The number of message bytes to process
*/
void OQS_SHA3_sha3_256(uint8_t *output, const uint8_t *input, size_t inplen);

typedef struct {
    uint64_t ctx[26];
} OQS_SHA3_sha3_256_inc_ctx;

void OQS_SHA3_sha3_256_inc_init(OQS_SHA3_sha3_256_inc_ctx *state);
void OQS_SHA3_sha3_256_inc_absorb(OQS_SHA3_sha3_256_inc_ctx *state, const uint8_t *input, size_t inlen);
void OQS_SHA3_sha3_256_inc_finalize(uint8_t *output, OQS_SHA3_sha3_256_inc_ctx *state);

#define OQS_SHA3_SHA3_384_RATE 104

/**
* \brief Process a message with SHA3-384 and return the hash code in the output byte array.
*
* \warning The output array must be at least 32 bytes in length.
*
* \param output The output byte array
* \param input The message input byte array
* \param inplen The number of message bytes to process
*/
void OQS_SHA3_sha3_384(uint8_t *output, const uint8_t *input, size_t inplen);

typedef struct {
    uint64_t ctx[26];
} OQS_SHA3_sha3_384_inc_ctx;

void OQS_SHA3_sha3_384_inc_init(OQS_SHA3_sha3_384_inc_ctx *state);
void OQS_SHA3_sha3_384_inc_absorb(OQS_SHA3_sha3_384_inc_ctx *state, const uint8_t *input, size_t inlen);
void OQS_SHA3_sha3_384_inc_finalize(uint8_t *output, OQS_SHA3_sha3_384_inc_ctx *state);

#define OQS_SHA3_SHA3_512_RATE 72

/**
* \brief Process a message with SHA3-512 and return the hash code in the output byte array.
*
* \warning The output array must be at least 64 bytes in length.
*
* \param output The output byte array
* \param input The message input byte array
* \param inplen The number of message bytes to process
*/
void OQS_SHA3_sha3_512(uint8_t *output, const uint8_t *input, size_t inplen);

typedef struct {
    uint64_t ctx[26];
} OQS_SHA3_sha3_512_inc_ctx;

void OQS_SHA3_sha3_512_inc_init(OQS_SHA3_sha3_512_inc_ctx *state);
void OQS_SHA3_sha3_512_inc_absorb(OQS_SHA3_sha3_512_inc_ctx *state, const uint8_t *input, size_t inlen);
void OQS_SHA3_sha3_512_inc_finalize(uint8_t *output, OQS_SHA3_sha3_512_inc_ctx *state);

/* SHAKE */

#define OQS_SHA3_SHAKE128_RATE 168

/**
* \brief Seed a SHAKE-128 instance, and generate an array of pseudo-random bytes.
*
* \warning The output array length must not be zero.
*
* \param output The output byte array
* \param outlen The number of output bytes to generate
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_shake128(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen);

typedef struct {
    uint64_t ctx[25];
} OQS_SHA3_shake128_ctx;

/**
* \brief The SHAKE-128 absorb function.
* Absorb and finalize an input seed byte array.
* Should be used in conjunction with the shake128_squeezeblocks function.
*
* \warning Finalizes the seed state, should not be used in consecutive calls. \n
* State must be initialized (and zeroed) by the caller.
*
* \param state The function state; must be pre-initialized
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_shake128_absorb(OQS_SHA3_shake128_ctx *state, const uint8_t *input, size_t inplen);

/**
* \brief The SHAKE-128 squeeze function.
* Permutes and extracts the state to an output byte array.
* Should be used in conjunction with the shake128_absorb function.
*
* \warning Output array must be initialized to a multiple of the byte rate.
*
* \param output The output byte array
* \param nblocks The number of blocks to extract
* \param state The function state; must be pre-initialized
*/
void OQS_SHA3_shake128_squeezeblocks(uint8_t *output, size_t nblocks, OQS_SHA3_shake128_ctx *state);

typedef struct {
    uint64_t ctx[26];
} OQS_SHA3_shake128_inc_ctx;

/**
 * \brief Initialize the incremental hashing API state
 */
void OQS_SHA3_shake128_inc_init(OQS_SHA3_shake128_inc_ctx *s_inc);

/**
 * \brief Absorb into the state
 *
 * \param s_inc state
 * \param input input buffer
 * \param inlen length of input buffer
 */
void OQS_SHA3_shake128_inc_absorb(OQS_SHA3_shake128_inc_ctx *s_inc, const uint8_t *input, size_t inlen);
/*
 * \brief Finalizes output
 *
 * \param s_inc Incremental hashing state
 */
void OQS_SHA3_shake128_inc_finalize(OQS_SHA3_shake128_inc_ctx *s_inc);

/**
 * \brief Obtains output
 *
 * \param output output buffer
 * \param outlen bytes of outbut buffer
 * \param state
 */
void OQS_SHA3_shake128_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake128_inc_ctx *s_inc);

#define OQS_SHA3_SHAKE256_RATE 136

/**
* \brief Seed a SHAKE-256 instance, and generate an array of pseudo-random bytes.
*
* \warning The output array length must not be zero.
*
* \param output The output byte array
* \param outlen The number of output bytes to generate
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_shake256(uint8_t *output, size_t outlen, const uint8_t *input, size_t inplen);

typedef struct {
    uint64_t ctx[25];
} OQS_SHA3_shake256_ctx;

/**
* \brief The SHAKE-256 absorb function.
* Absorb and finalize an input seed byte array.
* Should be used in conjunction with the shake256_squeezeblocks function.
*
* \warning Finalizes the seed state, should not be used in consecutive calls. \n
* State must be initialized (and zeroed) by the caller.
*
* \param state The function state; must be pre-initialized
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_shake256_absorb(OQS_SHA3_shake256_ctx *state, const uint8_t *input, size_t inplen);

/**
* \brief The SHAKE-256 squeeze function.
* Permutes and extracts the state to an output byte array.
*
* \warning Output array must be initialized to a multiple of the byte rate.
*
* \param output The output byte array
* \param nblocks The number of blocks to extract
* \param state The function state; must be pre-initialized
*/
void OQS_SHA3_shake256_squeezeblocks(uint8_t *output, size_t nblocks, OQS_SHA3_shake256_ctx *state);

typedef struct {
    uint64_t ctx[26];
} OQS_SHA3_shake256_inc_ctx;

/**
 * \brief Initialize the incremental hashing API state
 */
void OQS_SHA3_shake256_inc_init(OQS_SHA3_shake256_inc_ctx *s_inc);

/**
 * \brief Absorb into the state
 *
 * \param s_inc state
 * \param input input buffer
 * \param inlen length of input buffer
 */
void OQS_SHA3_shake256_inc_absorb(OQS_SHA3_shake256_inc_ctx *s_inc, const uint8_t *input, size_t inlen);
/*
 * \brief Finalizes output
 *
 * \param s_inc Incremental hashing state
 */
void OQS_SHA3_shake256_inc_finalize(OQS_SHA3_shake256_inc_ctx *s_inc);

/**
 * \brief Obtains output
 *
 * \param output output buffer
 * \param outlen bytes of outbut buffer
 * \param state
 */
void OQS_SHA3_shake256_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake256_inc_ctx *s_inc);

/* cSHAKE */

void OQS_SHA3_cshake128(uint8_t *output, size_t outlen, const uint8_t *name, size_t namelen, const uint8_t *cstm, size_t cstmlen, const uint8_t *input, size_t inlen);

void OQS_SHA3_cshake128_inc_init(OQS_SHA3_shake128_inc_ctx *state, const uint8_t *name, size_t namelen, const uint8_t *cstm, size_t cstmlen);
void OQS_SHA3_cshake128_inc_absorb(OQS_SHA3_shake128_inc_ctx *state, const uint8_t *input, size_t inlen);
void OQS_SHA3_cshake128_inc_finalize(OQS_SHA3_shake128_inc_ctx *state);
void OQS_SHA3_cshake128_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake128_inc_ctx *state);

void OQS_SHA3_cshake256(uint8_t *output, size_t outlen, const uint8_t *name, size_t namelen, const uint8_t* cstm, size_t cstmlen, const uint8_t *input, size_t inlen);

void OQS_SHA3_cshake256_inc_init(OQS_SHA3_shake256_inc_ctx *state, const uint8_t *name, size_t namelen, const uint8_t *cstm, size_t cstmlen);
void OQS_SHA3_cshake256_inc_absorb(OQS_SHA3_shake256_inc_ctx *state, const uint8_t *input, size_t inlen);
void OQS_SHA3_cshake256_inc_finalize(OQS_SHA3_shake256_inc_ctx *state);
void OQS_SHA3_cshake256_inc_squeeze(uint8_t *output, size_t outlen, OQS_SHA3_shake256_inc_ctx *state);

/**
* \brief Seed a cSHAKE-128 instance and generate pseudo-random output.
* Permutes and extracts the state to an output byte array.
*
* \warning This function has a counter period of 2^16.
*
* \param output The output byte array
* \param outlen The number of output bytes to generate
* \param cstm The 16bit customization integer
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_cshake128_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen);

/**
* \brief Seed a cSHAKE-256 instance and generate pseudo-random output.
* Permutes and extracts the state to an output byte array.
*
* \warning This function has a counter period of 2^16.
*
* \param output The output byte array
* \param outlen The number of output bytes to generate
* \param cstm The 16bit customization integer
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_cshake256_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen);

#if 0
/**
* \brief The cSHAKE-256 simple absorb function.
* Absorb and finalize an input seed directly into the state.
* Should be used in conjunction with the cshake256_simple_squeezeblocks function.
*
* \warning Finalizes the seed state, should not be used in consecutive calls. \n
* State must be initialized (and zeroed) by the caller.
*
* \param state The function state; must be pre-initialized
* \param cstm The 16bit customization integer
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_cshake256_simple_absorb(uint64_t *state, uint16_t cstm, const uint8_t *input, size_t inplen);

/**
* \brief The cSHAKE-256 simple squeeze function.
* Permutes and extracts blocks of state to an output byte array.
*
* \warning Output array must be initialized to a multiple of the byte rate.
*
* \param output The output byte array
* \param nblocks The number of blocks to extract
* \param state The function state; must be pre-initialized
*/
void OQS_SHA3_cshake256_simple_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state);
#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif
