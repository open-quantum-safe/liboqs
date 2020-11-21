#include "parameters.h"
#include "reed_muller.h"
#include <stdint.h>
#include <string.h>
/**
 * @file reed_muller.c
 * Constant time implementation of Reed-Muller code RM(1,7)
 */



// number of repeated code words
#define MULTIPLICITY                   CEIL_DIVIDE(PARAM_N2, 128)

// copy bit 0 into all bits of a 32 bit value
#define BIT0MASK(x) (-((x) & 1))


static void encode(uint8_t *word, uint8_t message);
static void hadamard(uint16_t src[128], uint16_t dst[128]);
static void expand_and_sum(uint16_t dest[128], const uint8_t src[16 * MULTIPLICITY]);
static uint8_t find_peaks(const uint16_t transform[128]);



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
static void encode(uint8_t *word, uint8_t message) {
    uint32_t e;
    // bit 7 flips all the bits, do that first to save work
    e = BIT0MASK(message >> 7);
    // bits 0, 1, 2, 3, 4 are the same for all four longs
    // (Warning: in the bit matrix above, low bits are at the left!)
    e ^= BIT0MASK(message >> 0) & 0xaaaaaaaa;
    e ^= BIT0MASK(message >> 1) & 0xcccccccc;
    e ^= BIT0MASK(message >> 2) & 0xf0f0f0f0;
    e ^= BIT0MASK(message >> 3) & 0xff00ff00;
    e ^= BIT0MASK(message >> 4) & 0xffff0000;
    // we can store this in the first quarter
    word[0 + 0] = (e >> 0x00) & 0xff;
    word[0 + 1] = (e >> 0x08) & 0xff;
    word[0 + 2] = (e >> 0x10) & 0xff;
    word[0 + 3] = (e >> 0x18) & 0xff;
    // bit 5 flips entries 1 and 3; bit 6 flips 2 and 3
    e ^= BIT0MASK(message >> 5);
    word[4 + 0] = (e >> 0x00) & 0xff;
    word[4 + 1] = (e >> 0x08) & 0xff;
    word[4 + 2] = (e >> 0x10) & 0xff;
    word[4 + 3] = (e >> 0x18) & 0xff;
    e ^= BIT0MASK(message >> 6);
    word[12 + 0] = (e >> 0x00) & 0xff;
    word[12 + 1] = (e >> 0x08) & 0xff;
    word[12 + 2] = (e >> 0x10) & 0xff;
    word[12 + 3] = (e >> 0x18) & 0xff;
    e ^= BIT0MASK(message >> 5);
    word[8 + 0] = (e >> 0x00) & 0xff;
    word[8 + 1] = (e >> 0x08) & 0xff;
    word[8 + 2] = (e >> 0x10) & 0xff;
    word[8 + 3] = (e >> 0x18) & 0xff;
}



/**
 * @brief Hadamard transform
 *
 * Perform hadamard transform of src and store result in dst
 * src is overwritten: it is also used as intermediate buffer
 * Method is best explained if we use H(3) instead of H(7):
 *
 * The routine multiplies by the matrix H(3):
 *                     [1  1  1  1  1  1  1  1]
 *                     [1 -1  1 -1  1 -1  1 -1]
 *                     [1  1 -1 -1  1  1 -1 -1]
 * [a b c d e f g h] * [1 -1 -1  1  1 -1 -1  1] = result of routine
 *                     [1  1  1  1 -1 -1 -1 -1]
 *                     [1 -1  1 -1 -1  1 -1  1]
 *                     [1  1 -1 -1 -1 -1  1  1]
 *                     [1 -1 -1  1 -1  1  1 -1]
 * You can do this in three passes, where each pass does this:
 * set lower half of buffer to pairwise sums,
 * and upper half to differences
 * index     0        1        2        3        4        5        6        7
 * input:    a,       b,       c,       d,       e,       f,       g,       h
 * pass 1:   a+b,     c+d,     e+f,     g+h,     a-b,     c-d,     e-f,     g-h
 * pass 2:   a+b+c+d, e+f+g+h, a-b+c-d, e-f+g-h, a+b-c-d, e+f-g-h, a-b-c+d, e-f-g+h
 * pass 3:   a+b+c+d+e+f+g+h   a+b-c-d+e+f-g-h   a+b+c+d-e-f-g-h   a+b-c-d-e+-f+g+h
 *                    a-b+c-d+e-f+g-h   a-b-c+d+e-f-g+h   a-b+c-d-e+f-g+h   a-b-c+d-e+f+g-h
 * This order of computation is chosen because it vectorises well.
 * Likewise, this routine multiplies by H(7) in seven passes.
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
    for (uint32_t pass = 0; pass < 7; pass++) {
        for (uint32_t i = 0; i < 64; i++) {
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
static void expand_and_sum(uint16_t dest[128], const uint8_t src[16 * MULTIPLICITY]) {
    size_t part, bit, copy;
    // start with the first copy
    for (part = 0; part < 16; part++) {
        for (bit = 0; bit < 8; bit++) {
            dest[part * 8 + bit] = (uint16_t) ((src[part] >> bit) & 1);
        }
    }
    // sum the rest of the copies
    for (copy = 1; copy < MULTIPLICITY; copy++) {
        for (part = 0; part < 16; part++) {
            for (bit = 0; bit < 8; bit++) {
                dest[part * 8 + bit] += (uint16_t) ((src[16 * copy + part] >> bit) & 1);
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
    for (uint16_t i = 0; i < 128; i++) {
        t = transform[i];
        abs = t ^ ((-(t >> 15)) & (t ^ -t)); // t = abs(t)
        mask = -(((uint16_t)(peak_abs - abs)) >> 15);
        peak ^= mask & (peak ^ t);
        pos ^= mask & (pos ^ i);
        peak_abs ^= mask & (peak_abs ^ abs);
    }
    pos |= 128 & ((peak >> 15) - 1);
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
void PQCLEAN_HQCRMRS192_CLEAN_reed_muller_encode(uint8_t *cdw, const uint8_t *msg) {
    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; i++) {
        // encode first word
        encode(&cdw[16 * i * MULTIPLICITY], msg[i]);
        // copy to other identical codewords
        for (size_t copy = 1; copy < MULTIPLICITY; copy++) {
            memcpy(&cdw[16 * i * MULTIPLICITY + 16 * copy], &cdw[16 * i * MULTIPLICITY], 16);
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
void PQCLEAN_HQCRMRS192_CLEAN_reed_muller_decode(uint8_t *msg, const uint8_t *cdw) {
    uint16_t expanded[128];
    uint16_t transform[128];
    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; i++) {
        // collect the codewords
        expand_and_sum(expanded, &cdw[16 * i * MULTIPLICITY]);
        // apply hadamard transform
        hadamard(expanded, transform);
        // fix the first entry to get the half Hadamard transform
        transform[0] -= 64 * MULTIPLICITY;
        // finish the decoding
        msg[i] = find_peaks(transform);
    }
}
