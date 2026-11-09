/**
 * @file    data_structures.h
 * @brief   HQC-PKE and HQC-KEM ciphertext data structures.
 */

#ifndef HQC_DATA_STRUCTURES_H
#define HQC_DATA_STRUCTURES_H

#include <stdint.h>
#include "parameters.h"

/**
 * @brief 128-bit codeword representation.
 *
 * A Reed-Muller RM(1,7) codeword is 128 bits long. This union allows
 * viewing the same data as an array of bytes or 32-bit words.
 */
typedef union {
    uint8_t u8[16];  /**< Byte-wise access (16 bytes) */
    uint32_t u32[4]; /**< Word-wise access (4 32-bit words) */
} rm_codeword_t;

#endif  // HQC_DATA_STRUCTURES_H
