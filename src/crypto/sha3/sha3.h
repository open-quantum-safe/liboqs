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

/*!
\def OQS_SHA3_CSHAKE_DOMAIN
* The cSHAKE function domain code
*/
#define OQS_SHA3_CSHAKE_DOMAIN 0x04

/*!
\def OQS_SHA3_CSHAKE128_RATE
* The cSHAKE-128 byte absorption rate
*/
#define OQS_SHA3_CSHAKE128_RATE 168

/*!
\def OQS_SHA3_CSHAKE256_RATE
* The cSHAKE-256 byte absorption rate
*/
#define OQS_SHA3_CSHAKE256_RATE 136

/*!
\def OQS_SHA3_SHA3_DOMAIN
* The SHA3 function domain code
*/
#define OQS_SHA3_SHA3_DOMAIN 0x06

/*!
\def OQS_SHA3_SHA3_256_RATE
* The SHA-256 byte absorption rate
*/
#define OQS_SHA3_SHA3_256_RATE 136

/*!
\def OQS_SHA3_SHA3_512_RATE
* The SHA-512 byte absorption rate
*/
#define OQS_SHA3_SHA3_512_RATE 72

/*!
\def OQS_SHA3_SHAKE_DOMAIN
* The function domain code
*/
#define OQS_SHA3_SHAKE_DOMAIN 0x1F

/*!
\def OQS_SHA3_SHAKE128_RATE
* The SHAKE-128 byte absorption rate
*/
#define OQS_SHA3_SHAKE128_RATE 168

/*!
\def OQS_SHA3_SHAKE256_RATE
* The SHAKE-256 byte absorption rate
*/
#define OQS_SHA3_SHAKE256_RATE 136

/*!
\def OQS_SHA3_STATESIZE
* The Keccak SHA3 state array size
*/
#define OQS_SHA3_STATESIZE 25

/* SHA3 */

/**
* \brief Process a message with SHA3-256 and return the hash code in the output byte array.
*
* \warning The output array must be at least 32 bytes in length.
*
* \param output The output byte array
* \param input The message input byte array
* \param inplen The number of message bytes to process
*/
void OQS_SHA3_sha3256(uint8_t *output, const uint8_t *input, size_t inplen);

/**
* \brief Process a message with SHA3-512 and return the hash code in the output byte array.
*
* \warning The output array must be at least 64 bytes in length.
*
* \param output The output byte array
* \param input The message input byte array
* \param inplen The number of message bytes to process
*/
void OQS_SHA3_sha3512(uint8_t *output, const uint8_t *input, size_t inplen);

/**
* \brief The Keccak absorb function.
* Absorb an input message array directly into the state.
*
* \warning Finalizes the message state, can not be used in consecutive calls. \n
* State must be initialized (and zeroed) by the caller.
*
* \param state The function state; must be initialized
* \param rate The rate of absorption, in bytes
* \param input The input message byte array
* \param inplen The number of message bytes to process
* \param domain The domain seperation code (SHA3=0x06, SHAKE=0x1F, cSHAKE=0x04)
*/
void OQS_SHA3_keccak_absorb(uint64_t *state, size_t rate, const uint8_t *input, size_t inplen, uint8_t domain);

/**
* \brief The Keccak permute function.
* Permutes the state array, can be used in conjunction with the keccak_absorb function.
*
* \param state The function state; must be initialized
*/
void OQS_SHA3_keccak_permute(uint64_t *state);

/**
* \brief The Keccak squeeze function.
* Permutes and extracts the state to an output byte array.
*
* \warning Output array must be initialized to a multiple of the byte rate.
*
* \param output The output byte array
* \param nblocks The number of blocks to extract
* \param state The function state; must be pre-initialized
* \param rate The rate of absorption, in bytes
*/
void OQS_SHA3_keccak_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state, size_t rate);

/* SHAKE */

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
void OQS_SHA3_shake128_absorb(uint64_t *state, const uint8_t *input, size_t inplen);

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
void OQS_SHA3_shake128_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state);

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
void OQS_SHA3_shake256_absorb(uint64_t *state, const uint8_t *input, size_t inplen);

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
void OQS_SHA3_shake256_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state);

/* cSHAKE */

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
* \brief The cSHAKE-128 simple absorb function.
* Absorb and finalize an input seed directly into the state.
* Should be used in conjunction with the cshake128_simple_squeezeblocks function.
*
* \warning Finalizes the seed state, should not be used in consecutive calls. \n
* State must be initialized (and zeroed) by the caller.
*
* \param state The function state; must be pre-initialized
* \param cstm The 16bit customization integer
* \param input The input seed byte array
* \param inplen The number of seed bytes to process
*/
void OQS_SHA3_cshake128_simple_absorb(uint64_t *state, uint16_t cstm, const uint8_t *input, size_t inplen);

/**
* \brief The cSHAKE-128 simple squeeze function.
* Permutes and extracts blocks of state to an output byte array.
*
* \warning Output array must be initialized to a multiple of the byte rate.
*
* \param output The output byte array
* \param nblocks The number of blocks to extract
* \param state The function state; must be pre-initialized
*/
void OQS_SHA3_cshake128_simple_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state);

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
