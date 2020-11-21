#include "parameters.h"
#include "reed_muller.h"
#include <immintrin.h>
#include <stdint.h>
#include <string.h>
/**
 * @file reed_muller.c
 * Constant time implementation of Reed-Muller code RM(1,7)
 */


// number of repeated code words
#define MULTIPLICITY                   CEIL_DIVIDE(PARAM_N2, 128)

// copy bit 0 into all bits of a 64 bit value
#define BIT0MASK(x) (int64_t)(-((x) & 1))

static void encode(uint8_t *word, uint8_t message);
static void expand_and_sum(__m256i *dst, const uint64_t *src);
static void hadamard(__m256i *src, __m256i *dst);
static uint32_t find_peaks(__m256i *transform);



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
 * 5   00000000 ffffffff 00000000 ffffffff
 * 6   00000000 00000000 ffffffff ffffffff
 * 7   ffffffff ffffffff ffffffff ffffffff
 *
 * @param[out] word An RM(1,7) codeword
 * @param[in] message A message to encode
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
 * @brief Add multiple codewords into expanded codeword
 *
 * Note: this does not write the codewords as -1 or +1 as the green machine does
 * instead, just 0 and 1 is used.
 * The resulting hadamard transform has:
 * all values are halved
 * the first entry is 64 too high
 *
 * @param[out] dst Structure that contain the expanded codeword
 * @param[in] src Structure that contain the codeword
 */
inline void expand_and_sum(__m256i *dst, const uint64_t *src) {
    uint16_t v[16];
    for (size_t part = 0; part < 8; part++) {
        dst[part] = _mm256_setzero_si256();
    }
    for (size_t copy = 0; copy < MULTIPLICITY; copy++) {
        for (size_t part = 0; part < 8; part++) {
            for (size_t bit = 0; bit < 16; bit++) {
                v[bit] = (((uint16_t *)(&src[2 * copy]))[part] >> bit) & 1;
            }
            dst[part] += _mm256_set_epi16(v[15], v[14], v[13], v[12], v[11], v[10], v[9], v[8],
                                          v[7], v[6], v[5], v[4], v[3], v[2], v[1], v[0]);
        }
    }
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
 * a-b+c-d+e-f+g-h   a-b-c+d+e-f-g+h   a-b+c-d-e+f-g+h   a-b-c+d-e+f+g-h
 * This order of computation is chosen because it vectorises well.
 * Likewise, this routine multiplies by H(7) in seven passes.
 *
 * @param[out] src Structure that contain the expanded codeword
 * @param[out] dst Structure that contain the expanded codeword
 */
inline void hadamard(__m256i *src, __m256i *dst) {
    // the passes move data:
    // src -> dst -> src -> dst -> src -> dst -> src -> dst
    // using p1 and p2 alternately
    __m256i *p1 = src;
    __m256i *p2 = dst;
    __m256i *p3;
    for (size_t pass = 0; pass < 7; pass++) {
        // warning: hadd works "within lanes" as Intel call it
        // so you have to swap the middle 64 bit blocks of the result
        for (size_t part = 0; part < 4; part++) {
            p2[part] = _mm256_permute4x64_epi64(_mm256_hadd_epi16(p1[2 * part], p1[2 * part + 1]), 0xd8);
            p2[part + 4] = _mm256_permute4x64_epi64(_mm256_hsub_epi16(p1[2 * part], p1[2 * part + 1]), 0xd8);
        }
        // swap p1, p2 for next round
        p3 = p1;
        p1 = p2;
        p2 = p3;
    }
}



/**
 * @brief Finding the location of the highest value
 *
 * This is the final step of the green machine: find the location of the highest value,
 * and add 128 if the peak is positive
 * Notes on decoding
 * The standard "Green machine" decoder words as follows:
 * if the received codeword is W, compute (2 * W - 1) * H7
 * The entries of the resulting vector are always even and vary from
 * -128 (= the complement is a code word, add bit 7 to decode)
 * via 0 (this is a different codeword)
 * to 128 (this is the code word).
 *
 * Our decoding differs in two ways:
 * - We take W instead of 2 * W - 1 (so the entries are 0,1 instead of -1,1)
 * - We take the sum of the repititions (so the entries are 0..MULTIPLICITY)
 * This implies that we have to subtract 64M (M=MULTIPLICITY)
 * from the first entry to make sure the first codewords is handled properly
 * and that the entries vary from -64M to 64M.
 * -64M or 64M stands for a perfect codeword.
 * If there are fewer than 32M errors, there is always a unique codeword
 * which an entry with absolute value > 32M;
 * this is because an error changes an entry by 1.
 * The highest number that seem to be decodable is 50 errors, so that the
 * highest entries in the hadamard transform can be as low as 12.
 * But this is different for the repeated code.
 * Because multiple codewords are added, this changes: the lowest value of the
 * hadamard transform of the sum of six words is seen to be as low as 43 (!),
 * which is way less than 12*6.
 *
 * It is possible that there are more errors, but the word is still uniquely
 * decodable: we found a word with distance of 50 from the nearest codeword.
 * That means that the highest entry can be as low as 14M.
 * Since we have to do binary search, we search for the range 1-64M
 * which can be done in 6+l2g(M) steps.
 * The binary search is based on (values>32M are unique):
 * M  32M     min>  max>  firstStep #steps
 * 2   64       1   64    33 +- 16    6
 * 4  128       1  128    65 +- 32    7
 * 6  192       1  192   129 +- 64    8
 *
 * As a check, we run a sample for M=6 to see the peak value; it ranged
 * from 43 to 147, so my analysis looks right. Also, it shows that decoding
 * far beyond the bound of 32M is needed.
 *
 * For the vectors, it would be tempting to use 8 bit ints,
 * because the values "almost" fit in there.
 * We could use some trickery to fit it in 8 bits, like saturated add or
 * division by 2 in a late step.
 * Unfortunately, these instructions do not exist.
 * the adds _mm512_adds_epi8 is available only on the latest processors,
 * and division, shift, mulhi are not available at all for 8 bits.
 * So, we use 16 bit ints.
 *
 * For the search of the optimal comparison value,
 * remember the transform contains 64M-d,
 * where d are the distances to the codewords.
 * The highest value gives the most likely codeword.
 * There is not fast vectorized way to find this value, so we search for the
 * maximum value itself.
 * In each pass, we collect a bit map of the transform values that are,
 * say >bound.  There are three cases:
 * bit map = 0: all code words are further away than 64M-bound (decrease bound)
 * bit map has one bit: one unique code word has distance < 64M-bound
 * bit map has multiple bits: multiple words (increase bound)
 * We will search for the lowest value of bound that gives a nonzero bit map.
 *
 * @param[in] transform Structure that contain the expanded codeword
 */
inline uint32_t find_peaks(__m256i *transform) {
    // a whole lot of vector variables
    __m256i bitmap, abs_rows[8], bound, active_row, max_abs_rows;
    __m256i tmp = _mm256_setzero_si256();
    __m256i vect_mask;
    __m256i res;
    int32_t lower;
    int32_t width;
    uint32_t message;
    uint32_t mask;
    int8_t index;
    int8_t abs_value;
    int8_t mask1;
    int8_t mask2;
    uint16_t result;

    // compute absolute value of transform
    for (size_t i = 0; i < 8; i++) {
        abs_rows[i] = _mm256_abs_epi16(transform[i]);
    }
    // compute a vector of 16 elements which contains the maximum somewhere
    // (later used to compute bits 0 through 3 of message)
    max_abs_rows = abs_rows[0];
    for (size_t i = 1; i < 8; i++) {
        max_abs_rows = _mm256_max_epi16(max_abs_rows, abs_rows[i]);
    }

    // do binary search for the highest value that is lower than the maximum
    // loop invariant: lower gives bit map = 0, lower + width gives bit map > 0
    lower = 1;
    // this gives 64, 128 or 256 for MULTIPLICITY = 2, 4, 6
    width = 1 << (5 + MULTIPLICITY / 2);
    // if you don't unroll this loop, it fits in the loop cache
    // uncomment the line below to speeding up the program by a few percent
    // #pragma GCC unroll 0
    while (width > 1) {
        width >>= 1;
        // compare with lower + width; put result in bitmap
        // make vector from value of new bound
        bound = _mm256_broadcastw_epi16(_mm_cvtsi32_si128(lower + width));
        bitmap = _mm256_cmpgt_epi16(max_abs_rows, bound);
        // step up if there are any matches
        // rely on compiler to use conditional move here
        mask = (uint32_t) _mm256_testz_si256(bitmap, bitmap);
        mask = ~(uint32_t) ((-(int64_t) mask) >> 63);
        lower += mask & width;
    }
    // lower+width contains the maximum value of the vector
    // or less, if the maximum is very high (which is OK)
    // normally, there is one maximum, but sometimes there are more
    // find where the maxima occur in the maximum vector
    // (each determines lower 4 bits of peak position)
    // construct vector filled with bound-1
    bound = _mm256_broadcastw_epi16(_mm_cvtsi32_si128(lower + width - 1));

    // find in which of the 8 groups a maximum occurs to compute bits 4, 5, 6 of message
    // find lowest value by searching backwards skip first check to save time
    message = 0x70;
    for (size_t i = 0; i < 8; i++) {
        bitmap = _mm256_cmpgt_epi16(abs_rows[7 - i], bound);
        mask = (uint32_t) _mm256_testz_si256(bitmap, bitmap);
        mask = ~(uint32_t) ((-(int64_t) mask) >> 63);
        message ^= mask & (message ^ ((7 - i) << 4));
    }
    // we decided which row of the matrix contains the lowest match
    // select proper row
    index = message >> 4;

    tmp = _mm256_setzero_si256();
    for (size_t i = 0; i < 8; i++) {
        abs_value = (int8_t)(index - i);
        mask1 = abs_value >> 7;
        abs_value ^= mask1;
        abs_value -= mask1;
        mask2 = ((uint8_t) - abs_value >> 7);
        mask = (-1ULL) + mask2;
        vect_mask = _mm256_set1_epi32(mask);
        res = _mm256_and_si256(abs_rows[i], vect_mask);
        tmp = _mm256_or_si256(tmp, res);
    }

    active_row = tmp;

    // get the column number of the vector element
    // by setting the bits corresponding to the columns
    // and then adding elements within two groups of 8
    vect_mask = _mm256_cmpgt_epi16(active_row, bound);
    vect_mask &= _mm256_set_epi16(-32768, 16384, 8192, 4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1);
    for (size_t i = 0; i < 3; i++) {
        vect_mask = _mm256_hadd_epi16(vect_mask, vect_mask);
    }
    // add low 4 bits of message
    message |= __tzcnt_u16(_mm256_extract_epi16(vect_mask, 0) + _mm256_extract_epi16(vect_mask, 8));

    // set bit 7 if sign of biggest value is positive
    // make sure a jump isn't generated by the compiler
    tmp = _mm256_setzero_si256();
    for (size_t i = 0; i < 8; i++) {
        mask = ~(uint32_t) ((-(int64_t)(i ^ message / 16)) >> 63);
        vect_mask = _mm256_set1_epi32(mask);
        tmp = _mm256_or_si256(tmp, _mm256_and_si256(vect_mask, transform[i]));
    }
    result = 0;
    for (size_t i = 0; i < 16; i++) {
        mask = ~(uint32_t) ((-(int64_t)(i ^ message % 16)) >> 63);
        result |= mask & ((uint16_t *)&tmp)[i];
    }
    message |= (0x8000 & ~result) >> 8;
    return message;
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
void PQCLEAN_HQCRMRS256_AVX2_reed_muller_encode(uint8_t *cdw, const uint8_t *msg) {
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
void PQCLEAN_HQCRMRS256_AVX2_reed_muller_decode(uint8_t *msg, const uint8_t *cdw) {
    __m256i expanded[8];
    __m256i transform[8];
    for (size_t i = 0; i < VEC_N1_SIZE_BYTES; i++) {
        // collect the codewords
        expand_and_sum(expanded, (uint64_t *)&cdw[16 * i * MULTIPLICITY]);
        // apply hadamard transform
        hadamard(expanded, transform);
        // fix the first entry to get the half Hadamard transform
        transform[0] -= _mm256_set_epi16(0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, 0, 64 * MULTIPLICITY);
        // finish the decoding
        msg[i] = find_peaks(transform);
    }
}
