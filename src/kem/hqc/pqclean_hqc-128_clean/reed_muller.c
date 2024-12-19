#include "parameters.h"
#include "reed_muller.h"
#include <stdint.h>
#include <string.h>
/**
 * @file reed_muller.c
 * @brief Constant time implementation of Reed-Muller code RM(1,7)
 */


// number of repeated code words
#define MULTIPLICITY                   CEIL_DIVIDE(PARAM_N2, 128)

// copy bit 0 into all bits of a 32 bit value
#define BIT0MASK(x) (uint32_t)(-((x) & 1))

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
static void encode(uint64_t *cword, uint8_t message) {
    uint32_t first_word;
    // bit 7 flips all the bits, do that first to save work
    first_word = BIT0MASK(message >> 7);
    // bits 0, 1, 2, 3, 4 are the same for all four longs
    // (Warning: in the bit matrix above, low bits are at the left!)
    first_word ^= BIT0MASK(message >> 0) & 0xaaaaaaaa;
    first_word ^= BIT0MASK(message >> 1) & 0xcccccccc;
    first_word ^= BIT0MASK(message >> 2) & 0xf0f0f0f0;
    first_word ^= BIT0MASK(message >> 3) & 0xff00ff00;
    first_word ^= BIT0MASK(message >> 4) & 0xffff0000;
    // we can store this in the first quarter
    cword[0] = first_word;
    // bit 5 flips entries 1 and 3; bit 6 flips 2 and 3
    first_word ^= BIT0MASK(message >> 5);
    cword[0] |= (uint64_t)first_word << 32;
    first_word ^= BIT0MASK(message >> 6);
    cword[1] = (uint64_t)first_word << 32;
    first_word ^= BIT0MASK(message >> 5);
    cword[1] |= first_word;
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
static void hadamard(uint16_t src[128], uint16_t dst[128]) {
    // the passes move data:
    // src -> dst -> src -> dst -> src -> dst -> src -> dst
    // using p1 and p2 alternately
    uint16_t *p1 = src;
    uint16_t *p2 = dst;
    uint16_t *p3;
    for (size_t pass = 0; pass < 7; ++pass) {
        for (size_t i = 0; i < 64; ++i) {
            p2[i] = p1[2 * i] + p1[2 * i + 1];
            p2[i + 64] = p1[2 * i] - p1[2 * i + 1];
        }
        // swap p1, p2 for next round
        p3 = p1;
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
static void expand_and_sum(uint16_t dest[128], const uint64_t src[2 * MULTIPLICITY]) {
    // start with the first copy
    for (size_t part = 0; part < 2; ++part) {
        for (size_t bit = 0; bit < 64; ++bit) {
            dest[part * 64 + bit] = ((src[part] >> bit) & 1);
        }
    }
    // sum the rest of the copies
    for (size_t copy = 1; copy < MULTIPLICITY; ++copy) {
        for (size_t part = 0; part < 2; ++part) {
            for (size_t bit = 0; bit < 64; ++bit) {
                dest[part * 64 + bit] += (uint16_t) ((src[2 * copy + part] >> bit) & 1);
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
static uint8_t find_peaks(const uint16_t transform[128]) {
    uint16_t peak_abs = 0;
    uint16_t peak = 0;
    uint16_t pos = 0;
    uint16_t t, abs, mask;
    for (uint16_t i = 0; i < 128; ++i) {
        t = transform[i];
        abs = t ^ ((uint16_t)(-(t >> 15)) & (t ^ -t)); // t = abs(t)
        mask = -(((uint16_t)(peak_abs - abs)) >> 15);
        peak ^= mask & (peak ^ t);
        pos ^= mask & (pos ^ i);
        peak_abs ^= mask & (peak_abs ^ abs);
    }
    // set bit 7
    pos |= 128 & (uint16_t)((peak >> 15) - 1);
    return (uint8_t) pos;
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
void PQCLEAN_HQC128_CLEAN_reed_muller_encode(uint64_t *cdw, const uint8_t *msg) {
    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
        // encode first word
        encode(&cdw[2 * i * MULTIPLICITY], msg[i]);
        // copy to other identical codewords
        for (size_t copy = 1; copy < MULTIPLICITY; ++copy) {
            memcpy(&cdw[2 * i * MULTIPLICITY + 2 * copy], &cdw[2 * i * MULTIPLICITY], 16);
        }
    }
}



/**
 * @brief Decodes the received word
 *
 * Decoding uses fast hadamard transform, for a more complete picture on Reed-Muller decoding, see MacWilliams, Florence Jessie, and Neil James Alexander Sloane.
 * The theory of error-correcting codes codes @cite macwilliams1977theory
 *
 * @param[out] msg Array of size VEC_N1_SIZE_64 receiving the decoded message
 * @param[in] cdw Array of size VEC_N1N2_SIZE_64 storing the received word
 */
void PQCLEAN_HQC128_CLEAN_reed_muller_decode(uint8_t *msg, const uint64_t *cdw) {
    uint16_t expanded[128];
    uint16_t transform[128];
    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
        // collect the codewords
        expand_and_sum(expanded, &cdw[2 * i * MULTIPLICITY]);
        // apply hadamard transform
        hadamard(expanded, transform);
        // fix the first entry to get the half Hadamard transform
        transform[0] -= 64 * MULTIPLICITY;
        // finish the decoding
        msg[i] = find_peaks(transform);
    }
}
