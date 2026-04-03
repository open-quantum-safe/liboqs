/**
 * @file reed_muller.c
 * @brief Constant time implementation of Reed-Muller code RM(1,7)
 */

#include "reed_muller.h"
#include <stdint.h>
#include <string.h>
#include "data_structures.h"
#include "parameters.h"

/**
 * @brief Number of repeated 128-bit codeword blocks.
 *
 * Calculates the ceiling of PARAM_N2/128 to determine how many
 * copies of each 128-bit codeword are used in the code expansion.
 */
#define MULTIPLICITY CEIL_DIVIDE(PARAM_N2, 128)

/**
 * @typedef rm_expanded_cdw
 * @brief Internal representation of a codeword with each bit expanded to a 16-bit signed value.
 */
typedef int16_t rm_expanded_cdw[128];

// clang-format off
/**
 * @def BIT0MASK(x)
 * @brief Broadcast the least significant bit of \p x to a 32-bit mask.
 *
 * @param x  An integer expression; only bit 0 is examined.
 * @return   A 32-bit value of all ones (if \p x&1 == 1) or all zeros (if \p x&1 == 0).
 */
#define BIT0MASK(x) (int32_t)(-((x) & 1))
// clang-format on

void encode(rm_codeword_t *word, int32_t message);
void hadamard(rm_expanded_cdw *src, rm_expanded_cdw *dst);
void expand_and_sum(rm_expanded_cdw *dest, rm_codeword_t src[]);
int32_t find_peaks(rm_expanded_cdw *transform);

/**
 * @brief Encode a single byte into a single codeword using RM(1,7)
 *
 * Encoding matrix of this code:
 * bit pattern (note that bits are numbered big endian)
 * 0   aaaaaaaa aaaaaaaa aaaaaaaa aaaaaaaa
 * 1   cccccccc cccccccc cccccccc cccccccc
 * 2   f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
 * 3   ff00ff00 ff00ff00 ff00ff00 ff00ff00
 * 4   ffff0000 ffff0000 ffff0000 ffff0000
 * 5   ffffffff 00000000 ffffffff 00000000
 * 6   ffffffff ffffffff 00000000 00000000
 * 7   ffffffff ffffffff ffffffff ffffffff
 *
 * @param[out] word An RM(1,7) codeword
 * @param[in] message A message
 */
void encode(rm_codeword_t *word, int32_t message) {
    int32_t first_word;

    first_word = BIT0MASK(message >> 7);

    first_word ^= BIT0MASK(message >> 0) & 0xaaaaaaaa;
    first_word ^= BIT0MASK(message >> 1) & 0xcccccccc;
    first_word ^= BIT0MASK(message >> 2) & 0xf0f0f0f0;
    first_word ^= BIT0MASK(message >> 3) & 0xff00ff00;
    first_word ^= BIT0MASK(message >> 4) & 0xffff0000;

    word->u32[0] = first_word;

    first_word ^= BIT0MASK(message >> 5);
    word->u32[1] = first_word;
    first_word ^= BIT0MASK(message >> 6);
    word->u32[3] = first_word;
    first_word ^= BIT0MASK(message >> 5);
    word->u32[2] = first_word;
    return;
}

/**
 * @brief Hadamard transform
 *
 * Perform hadamard transform of src and store result in dst
 * src is overwritten
 *
 * @param[out] src Structure that contain the expanded codeword
 * @param[out] dst Structure that contain the expanded codeword
 */
void hadamard(rm_expanded_cdw *src, rm_expanded_cdw *dst) {
    // the passes move data:
    // src -> dst -> src -> dst -> src -> dst -> src -> dst
    // using p1 and p2 alternately
    rm_expanded_cdw *p1 = src;
    rm_expanded_cdw *p2 = dst;
    for (int32_t pass = 0; pass < 7; pass++) {
        for (int32_t i = 0; i < 64; i++) {
            (*p2)[i] = (*p1)[2 * i] + (*p1)[2 * i + 1];
            (*p2)[i + 64] = (*p1)[2 * i] - (*p1)[2 * i + 1];
        }
        // swap p1, p2 for next round
        rm_expanded_cdw *p3 = p1;
        p1 = p2;
        p2 = p3;
    }
}

/**
 * @brief Add multiple codewords into expanded codeword
 *
 * Accesses memory in order
 * Note: this does not write the codewords as -1 or +1 as the green machine does
 * instead, just 0 and 1 is used.
 * The resulting hadamard transform has:
 * all values are halved
 * the first entry is 64 too high
 *
 * @param[out] dest Structure that contain the expanded codeword
 * @param[in] src Structure that contain the codeword
 */
void expand_and_sum(rm_expanded_cdw *dest, rm_codeword_t src[]) {
    // start with the first copy
    for (int32_t part = 0; part < 4; part++) {
        for (int32_t bit = 0; bit < 32; bit++) {
            (*dest)[part * 32 + bit] = src[0].u32[part] >> bit & 1;
        }
    }
    // sum the rest of the copies
    for (int32_t copy = 1; copy < MULTIPLICITY; copy++) {
        for (int32_t part = 0; part < 4; part++) {
            for (int32_t bit = 0; bit < 32; bit++) {
                (*dest)[part * 32 + bit] += src[copy].u32[part] >> bit & 1;
            }
        }
    }
}

/**
 * @brief Finding the location of the highest value
 *
 * This is the final step of the green machine: find the location of the highest value,
 * and add 128 if the peak is positive
 * if there are two identical peaks, the peak with smallest value
 * in the lowest 7 bits it taken
 * @param[in] transform Structure that contain the expanded codeword
 */
int32_t find_peaks(rm_expanded_cdw *transform) {
    int32_t peak_abs_value = 0;
    int32_t peak_value = 0;
    int32_t peak_pos = 0;
    for (int32_t i = 0; i < 128; i++) {
        // get absolute value
        int32_t t = (*transform)[i];
        int32_t pos_mask = -(t > 0);
        int32_t absolute = (pos_mask & t) | (~pos_mask & -t);
        peak_value = absolute > peak_abs_value ? t : peak_value;
        peak_pos = absolute > peak_abs_value ? i : peak_pos;
        peak_abs_value = absolute > peak_abs_value ? absolute : peak_abs_value;
    }
    // set bit 7
    peak_pos |= 128 * (peak_value > 0);
    return peak_pos;
}

/**
 * @brief Encodes the received word
 *
 * The message consists of N1 bytes each byte is encoded into PARAM_N2 bits,
 * or MULTIPLICITY repeats of 128 bits
 *
 * @param[out] cdw Array of size VEC_N1N2_SIZE_64 receiving the encoded message
 * @param[in] msg Array of size VEC_N1_SIZE_64 storing the message
 */
void reed_muller_encode(uint64_t *cdw, const uint64_t *msg) {
    uint8_t *message_array = (uint8_t *)msg;
    rm_codeword_t *codeArray = (rm_codeword_t *)cdw;
    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; i++) {
        // fill entries i * MULTIPLICITY to (i+1) * MULTIPLICITY
        int32_t pos = i * MULTIPLICITY;
        // encode first word
        encode(&codeArray[pos], message_array[i]);
        // copy to other identical codewords
        for (size_t copy = 1; copy < MULTIPLICITY; copy++) {
            memcpy(&codeArray[pos + copy], &codeArray[pos], sizeof(rm_codeword_t));
        }
    }
}

/**
 * @brief Decodes the received word
 *
 * Decoding uses fast hadamard transform, for a more complete picture on Reed-Muller decoding, see MacWilliams, Florence
 * Jessie, and Neil James Alexander Sloane. The theory of error-correcting codes codes @cite macwilliams1977theory
 *
 * @param[out] msg Array of size VEC_N1_SIZE_64 receiving the decoded message
 * @param[in] cdw Array of size VEC_N1N2_SIZE_64 storing the received word
 */
void reed_muller_decode(uint64_t *msg, const uint64_t *cdw) {
    uint8_t *message_array = (uint8_t *)msg;
    rm_codeword_t *codeArray = (rm_codeword_t *)cdw;
    rm_expanded_cdw expanded;
    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; i++) {
        // collect the codewords
        expand_and_sum(&expanded, &codeArray[i * MULTIPLICITY]);
        // apply hadamard transform
        rm_expanded_cdw transform;
        hadamard(&expanded, &transform);
        // fix the first entry to get the half Hadamard transform
        transform[0] -= 64 * MULTIPLICITY;
        // finish the decoding
        message_array[i] = find_peaks(&transform);
    }
}
