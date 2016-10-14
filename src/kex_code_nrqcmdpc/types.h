#ifndef OQS_KEX_CODE_NRQCMPDC_TYPES_H
#define OQS_KEX_CODE_NRQCMPDC_TYPES_H

/**
 * \file types.h
 * Defines all types (and their sizes) used in the implementation.
 */

#include <stdint.h>

#include "config.h"

/**
 * \typedef limb_t  Chunks of the polynomial that fit in a register
 */

#ifndef LIMB_BITS
#define LIMB_BITS 64
#endif

#if   LIMB_BITS == 64
typedef uint64_t limb_t;
#define LIMB_INDEX_BITS 6
#elif LIMB_BITS == 32
typedef uint32_t limb_t;
#define LIMB_INDEX_BITS 5
#elif LIMB_BITS == 16
typedef uint16_t limb_t;
#define LIMB_INDEX_BITS 4
#elif LIMB_BITS == 8
typedef uint8_t limb_t;
#define LIMB_INDEX_BITS 3
#endif

/**
 * \typedef index_t Index of set bit in a sparse polynomial
 * TODO: allow for indices > 2^16
 */

#ifndef INDEX_BITS
#define INDEX_BITS 16
#endif

#if INDEX_BITS == 16
typedef uint16_t index_t;
#endif


/**
 * \typedef byte_t  8-bit byte
 */
typedef uint8_t byte_t;


#define TO_BYTES(bits) (((bits) + 7) / 8)
#define TO_LIMBS(bits) (((bits) + LIMB_BITS - 1) / LIMB_BITS)
#define TO_MASK(bits)  ((limb_t)((1 << bits) - 1))

#define TAIL_BITS (POLY_BITS % LIMB_BITS)
#define TAIL_MASK TO_MASK(TAIL_BITS)
#define TAIL_BYTES TO_BYTES(TAIL_BITS)

#define LIMB_BYTES TO_BYTES(LIMB_BITS)
#define INDEX_BYTES TO_BYTES(INDEX_BITS)

#define POLY_BYTES TO_BYTES(POLY_BITS)
#define POLY_LIMBS TO_LIMBS(POLY_BITS)


/**
 * Dense polynomial type (fixed size array of limbs)
 *
 * Polynomial representation: The least significant bit of each word
 * holds the coefficient of the lowest degree. Words with a higher
 * index hold represent higher degrees. For example, f (two words of 8
 * bits) with f[0] = 0b00010011 and f[1] = 0b00000001 represents the
 * polynomial x^8 + x^4 + x + 1.
 *
 * Polynomials are defined modulo G, where
 *   G = x^POLY_BITS - 1
 */
typedef limb_t poly_t[POLY_LIMBS];

/**
 * Sparse polynomial type (fixed weight array of indices)
 */
typedef index_t sp_poly_t[POLY_WEIGHT];

/**
 * Systematic parity-check type (fixed size array of dense polynomials)
 */
typedef poly_t sys_par_ch_t[POLY_COUNT - 1];

/**
 * Sparse error type (fixed weight array of indices)
 */
typedef index_t sp_error_t[ERROR_WEIGHT];

/**
 * Error type (fixed size array of dense polynomials)
 */
typedef poly_t error_t[POLY_COUNT];

/**
 * Syndrome type (alias for poly_t)
 */
typedef poly_t syn_t;

/**
 * Parity-check type (fixed size array of sparse polynomials)
 */
typedef sp_poly_t par_ch_t[POLY_COUNT];

/**
 * Dense parity-check type (fixed size array of dense polynomials)
 */
typedef poly_t dense_par_ch_t[POLY_COUNT];

#define PUBLIC_KEY_BYTES  ((POLY_COUNT - 1) * POLY_BYTES)
#define ERROR_BYTES       (POLY_COUNT * POLY_BYTES)
#define SYNDROME_BYTES    POLY_BYTES

#endif /* OQS_KEX_CODE_NRQCMPDC_TYPES_H */
