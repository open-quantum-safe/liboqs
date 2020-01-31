#include "gf2x_arith_mod_xPplusOne.h"
#include "rng.h"
#include "sort.h"

#include <string.h>  // memcpy(...), memset(...)

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_copy(DIGIT dest[], const DIGIT in[]) {
    for (size_t i = 0; i < NUM_DIGITS_GF2X_ELEMENT; i++) {
        dest[i] = in[i];
    }
}

/* returns the coefficient of the x^exponent term as the LSB of a digit */
DIGIT PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_get_coeff(const DIGIT poly[], size_t exponent) {
    size_t straightIdx = (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - 1) - exponent;
    size_t digitIdx = straightIdx / DIGIT_SIZE_b;
    size_t inDigitIdx = straightIdx % DIGIT_SIZE_b;
    return (poly[digitIdx] >> (DIGIT_SIZE_b - 1 - inDigitIdx)) & ((DIGIT) 1) ;
}

/* sets the coefficient of the x^exponent term as the LSB of a digit */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_set_coeff(DIGIT poly[], size_t exponent, DIGIT value) {
    size_t straightIdx = (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - 1) - exponent;
    size_t digitIdx = straightIdx / DIGIT_SIZE_b;
    size_t inDigitIdx = straightIdx % DIGIT_SIZE_b;

    /* clear given coefficient */
    DIGIT mask = ~(((DIGIT) 1) << (DIGIT_SIZE_b - 1 - inDigitIdx));
    poly[digitIdx] = poly[digitIdx] & mask;
    poly[digitIdx] = poly[digitIdx] | ((value & ((DIGIT) 1)) << (DIGIT_SIZE_b - 1 - inDigitIdx));
}

/* toggles (flips) the coefficient of the x^exponent term as the LSB of a digit */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_toggle_coeff(DIGIT poly[], size_t exponent) {
    size_t straightIdx = (NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - 1) - exponent;
    size_t digitIdx = straightIdx / DIGIT_SIZE_b;
    size_t inDigitIdx = straightIdx % DIGIT_SIZE_b;

    /* clear given coefficient */
    DIGIT mask = (((DIGIT) 1) << (DIGIT_SIZE_b - 1 - inDigitIdx));
    poly[digitIdx] = poly[digitIdx] ^ mask;
}

/* population count for an unsigned 64-bit integer
   Source: Hacker's delight, p.66  */
static int popcount_uint64t(uint64_t x) {
    x -= (x >> 1) & 0x5555555555555555;
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
    return (int)((x * 0x0101010101010101) >> 56);
}

/* population count for a single polynomial */
int PQCLEAN_LEDAKEMLT12_LEAKTIME_population_count(const DIGIT *poly) {
    int ret = 0;
    for (int i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= 0; i--) {
        ret += popcount_uint64t(poly[i]);
    }
    return ret;
}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_add(DIGIT Res[], const DIGIT A[], const DIGIT B[]) {
    PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_add(Res, A, B, NUM_DIGITS_GF2X_ELEMENT);
}

static void gf2x_mod(DIGIT out[],  const DIGIT in[]) {
    DIGIT aux[NUM_DIGITS_GF2X_ELEMENT + 1];

    memcpy(aux, in, (NUM_DIGITS_GF2X_ELEMENT + 1)*DIGIT_SIZE_B);
    PQCLEAN_LEDAKEMLT12_LEAKTIME_right_bit_shift_n(NUM_DIGITS_GF2X_ELEMENT + 1, aux,
            MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS);
    PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_add(out, aux + 1, in + NUM_DIGITS_GF2X_ELEMENT,
                                          NUM_DIGITS_GF2X_ELEMENT);
    out[0] &= ((DIGIT)1 << MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS) - 1;
}

static void right_bit_shift(size_t length, DIGIT in[]) {
    size_t j;
    for (j = length - 1; j > 0; j--) {
        in[j] >>= 1;
        in[j] |=  (in[j - 1] & (DIGIT)0x01) << (DIGIT_SIZE_b - 1);
    }
    in[j] >>= 1;
}


/* shifts by whole digits */
static void left_DIGIT_shift_n(size_t length, DIGIT in[], size_t amount) {
    size_t j;
    for (j = 0; (j + amount) < length; j++) {
        in[j] = in[j + amount];
    }
    for (; j < length; j++) {
        in[j] = (DIGIT)0;
    }
}

/* may shift by an arbitrary amount*/
static void left_bit_shift_wide_n(size_t length, DIGIT in[], size_t amount) {
    left_DIGIT_shift_n(length, in, amount / DIGIT_SIZE_b);
    PQCLEAN_LEDAKEMLT12_LEAKTIME_left_bit_shift_n(length, in, amount % DIGIT_SIZE_b);
}

/* Hackers delight, reverses a uint64_t */
static DIGIT reverse_digit(DIGIT x) {
    uint64_t t;
    x = (x << 31) | (x >> 33);
    t = (x ^ (x >> 20)) & 0x00000FFF800007FFLL;
    x = (t | (t << 20)) ^ x;
    t = (x ^ (x >> 8)) & 0x00F8000F80700807LL;
    x = (t | (t << 8)) ^ x;
    t = (x ^ (x >> 4)) & 0x0808708080807008LL;
    x = (t | (t << 4)) ^ x;
    t = (x ^ (x >> 2)) & 0x1111111111111111LL;
    x = (t | (t << 2)) ^ x;
    return x;
}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_transpose_in_place(DIGIT A[]) {
    /* it keeps the lsb in the same position and
     * inverts the sequence of the remaining bits */

    DIGIT mask = (DIGIT)0x1;
    DIGIT rev1, rev2, a00;
    int slack_bits_amount = NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_b - P;

    a00 = A[NUM_DIGITS_GF2X_ELEMENT - 1] & mask;
    right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, A);

    for (size_t i = NUM_DIGITS_GF2X_ELEMENT - 1; i >= (NUM_DIGITS_GF2X_ELEMENT + 1) / 2; i--) {
        rev1 = reverse_digit(A[i]);
        rev2 = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT - 1 - i]);
        A[i] = rev2;
        A[NUM_DIGITS_GF2X_ELEMENT - 1 - i] = rev1;
    }

    A[NUM_DIGITS_GF2X_ELEMENT / 2] = reverse_digit(A[NUM_DIGITS_GF2X_ELEMENT / 2]);

    if (slack_bits_amount) {
        PQCLEAN_LEDAKEMLT12_LEAKTIME_right_bit_shift_n(NUM_DIGITS_GF2X_ELEMENT, A, slack_bits_amount);
    }
    A[NUM_DIGITS_GF2X_ELEMENT - 1] = (A[NUM_DIGITS_GF2X_ELEMENT - 1] & (~mask)) | a00;
}

static void rotate_bit_right(DIGIT in[]) { /*  x^{-1} * in(x) mod x^P+1 */

    DIGIT rotated_bit = in[NUM_DIGITS_GF2X_ELEMENT - 1] & ((DIGIT)0x1);
    right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, in);
    int msb_offset_in_digit = MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS - 1;
    rotated_bit = rotated_bit << msb_offset_in_digit;
    in[0] |= rotated_bit;
}

/* cond swap: swaps digits A and B if swap_mask == -1 */
static void gf2x_cswap(DIGIT *a, DIGIT *b, int32_t swap_mask) {
    DIGIT t;
    for (size_t i = 0; i < NUM_DIGITS_GF2X_ELEMENT; i++) {
        t = swap_mask & (a[i] ^ b[i]);
        a[i] ^= t;
        b[i] ^= t;
    }
}

/* returns -1 mask if x != 0, otherwise 0 */
static inline int32_t nonzero(DIGIT x) {
    DIGIT t = x;
    t = (~t) + 1;
    t >>= DIGIT_SIZE_b - 1;
    return -((int32_t)t);
}

/* returns -1 mask if x < 0 else 0 */
static inline int32_t negative(int x) {
    uint32_t u = x;
    u >>= 31;
    return -((int32_t)u);
}

/* return f(0) as digit */
static inline DIGIT lsb(const DIGIT *p) {
    DIGIT mask = (DIGIT)1;
    return p[NUM_DIGITS_GF2X_ELEMENT - 1] & mask;
}

/* multiply poly with scalar and accumulate, expects s all-zero of all-one mask */
static void gf2x_mult_scalar_acc(DIGIT *f, const DIGIT *g, const DIGIT s) {
    for (size_t i = 0; i < NUM_DIGITS_GF2X_ELEMENT; i++) {
        f[i] = f[i] ^ (s & g[i]);
    }
}

/* constant-time inverse, source: gcd.cr.yp.to */
int PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_inverse(DIGIT out[], const DIGIT in[]) {
    int32_t swap, delta = 1;
    DIGIT g0_mask;

    DIGIT f[NUM_DIGITS_GF2X_MODULUS] = {0}; // f = x^P + 1
    DIGIT g[NUM_DIGITS_GF2X_ELEMENT];       // g = in
    DIGIT *v = out;                         // v = 0, save space
    DIGIT r[NUM_DIGITS_GF2X_ELEMENT] = {0}; // r = 1

    f[NUM_DIGITS_GF2X_MODULUS - 1] = 1;
    f[0] |= ((DIGIT)1 << MSb_POSITION_IN_MSB_DIGIT_OF_MODULUS);

    for (size_t i = 0; i < NUM_DIGITS_GF2X_ELEMENT; i++) {
        g[i] = in[i];
    }

    for (size_t i = 0; i < NUM_DIGITS_GF2X_ELEMENT; i++) {
        v[i] = 0;
    }

    r[NUM_DIGITS_GF2X_ELEMENT - 1] = 1;

    for (int loop = 0; loop < 2 * P - 1; ++loop) {

        swap = negative(-delta) & nonzero(lsb(g));              // swap = -1 if -delta < 0 AND g(0) != 0
        delta ^= swap & (delta ^ -delta);                       // cond swap delta with -delta if swap
        delta++;

        gf2x_cswap(f, g, swap);
        gf2x_cswap(v, r, swap);

        g0_mask = ~lsb(g) + 1;

        // g = (g - g0 * f) / x
        gf2x_mult_scalar_acc(g, f, g0_mask);
        right_bit_shift(NUM_DIGITS_GF2X_ELEMENT, g);

        // r = (r - g0 * v) / x
        gf2x_mult_scalar_acc(r, v, g0_mask);
        rotate_bit_right(r);

    }

    return nonzero(delta); // -1 if fail, 0 if success
}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_mul(DIGIT Res[], const DIGIT A[], const DIGIT B[]) {

    DIGIT aux[2 * NUM_DIGITS_GF2X_ELEMENT];
    PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mul(aux, A, B, NUM_DIGITS_GF2X_ELEMENT);
    gf2x_mod(Res, aux);

}

/*PRE: the representation of the sparse coefficients is sorted in increasing
 order of the coefficients themselves */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_mul_dense_to_sparse(DIGIT Res[], const DIGIT dense[],
        POSITION_T sparse[], size_t nPos) {

    DIGIT aux[2 * NUM_DIGITS_GF2X_ELEMENT] = {0x00};
    DIGIT resDouble[2 * NUM_DIGITS_GF2X_ELEMENT] = {0x00};
    memcpy(aux + NUM_DIGITS_GF2X_ELEMENT, dense, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    memcpy(resDouble + NUM_DIGITS_GF2X_ELEMENT, dense, NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);

    if (sparse[0] != INVALID_POS_VALUE) {
        left_bit_shift_wide_n(2 * NUM_DIGITS_GF2X_ELEMENT, resDouble, sparse[0]);
        left_bit_shift_wide_n(2 * NUM_DIGITS_GF2X_ELEMENT, aux, sparse[0]);

        for (size_t i = 1; i < nPos; i++) {
            if (sparse[i] != INVALID_POS_VALUE) {
                left_bit_shift_wide_n(2 * NUM_DIGITS_GF2X_ELEMENT, aux, (sparse[i] - sparse[i - 1]) );
                PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_add(resDouble, aux, resDouble, 2 * NUM_DIGITS_GF2X_ELEMENT);
            }
        }
    }

    gf2x_mod(Res, resDouble);

}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_transpose_in_place_sparse(size_t sizeA, POSITION_T A[]) {
    POSITION_T t;
    size_t i = 0, j;

    if (A[i] == 0) {
        i = 1;
    }
    j = i;

    for (; i < sizeA && A[i] != INVALID_POS_VALUE; i++) {
        A[i] = P - A[i];
    }

    for (i -= 1; j < i; j++, i--) {
        t = A[j];
        A[j] = A[i];
        A[i] = t;
    }

}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_mul_sparse(size_t sizeR, POSITION_T Res[],
        size_t sizeA, const POSITION_T A[],
        size_t sizeB, const POSITION_T B[]) {

    POSITION_T prod;
    POSITION_T lastReadPos;
    size_t duplicateCount;
    size_t write_idx, read_idx;

    /* compute all the coefficients, filling invalid positions with P*/
    size_t lastFilledPos = 0;
    for (size_t i = 0 ; i < sizeA ; i++) {
        for (size_t j = 0 ; j < sizeB ; j++) {
            prod = A[i] + B[j];
            prod = ( (prod >= P) ? prod - P : prod);
            if ((A[i] != INVALID_POS_VALUE) &&
                    (B[j] != INVALID_POS_VALUE)) {
                Res[lastFilledPos] = prod;
            } else {
                Res[lastFilledPos] = INVALID_POS_VALUE;
            }
            lastFilledPos++;
        }
    }
    while (lastFilledPos < sizeR) {
        Res[lastFilledPos] = INVALID_POS_VALUE;
        lastFilledPos++;
    }

    PQCLEAN_LEDAKEMLT12_LEAKTIME_uint32_sort(Res, sizeR);

    /* eliminate duplicates */
    write_idx = read_idx = 0;
    while (read_idx < sizeR  && Res[read_idx] != INVALID_POS_VALUE) {
        lastReadPos = Res[read_idx];
        read_idx++;
        duplicateCount = 1;
        while ( (Res[read_idx] == lastReadPos) && (Res[read_idx] != INVALID_POS_VALUE)) {
            read_idx++;
            duplicateCount++;
        }
        if (duplicateCount % 2) {
            Res[write_idx] = lastReadPos;
            write_idx++;
        }
    }
    /* fill remaining cells with INVALID_POS_VALUE */
    for (; write_idx < sizeR; write_idx++) {
        Res[write_idx] = INVALID_POS_VALUE;
    }
}

/* the implementation is safe even in case A or B alias with the result
 * PRE: A and B should be sorted, disjunct arrays ending with INVALID_POS_VALUE */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mod_add_sparse(size_t sizeR, POSITION_T Res[],
        size_t sizeA, const POSITION_T A[],
        size_t sizeB, const POSITION_T B[]) {

    POSITION_T tmpRes[DV * M];
    size_t idxA = 0, idxB = 0, idxR = 0;
    while ( idxA < sizeA  &&
            idxB < sizeB  &&
            A[idxA] != INVALID_POS_VALUE &&
            B[idxB] != INVALID_POS_VALUE ) {

        if (A[idxA] == B[idxB]) {
            idxA++;
            idxB++;
        } else {
            if (A[idxA] < B[idxB]) {
                tmpRes[idxR] = A[idxA];
                idxA++;
            } else {
                tmpRes[idxR] = B[idxB];
                idxB++;
            }
            idxR++;
        }
    }

    while (idxA < sizeA && A[idxA] != INVALID_POS_VALUE) {
        tmpRes[idxR] = A[idxA];
        idxA++;
        idxR++;
    }

    while (idxB < sizeB && B[idxB] != INVALID_POS_VALUE) {
        tmpRes[idxR] = B[idxB];
        idxB++;
        idxR++;
    }

    while (idxR < sizeR) {
        tmpRes[idxR] = INVALID_POS_VALUE;
        idxR++;
    }
    memcpy(Res, tmpRes, sizeof(POSITION_T)*sizeR);

}

/* Return a uniform random value in the range 0..n-1 inclusive,
 * applying a rejection sampling strategy and exploiting as a random source
 * the NIST seedexpander seeded with the proper key.
 * Assumes that the maximum value for the range n is 2^32-1
 */
static uint32_t rand_range(const unsigned int n, const int logn, AES_XOF_struct *seed_expander_ctx) {
    unsigned long required_rnd_bytes = (logn + 7) / 8;
    unsigned char rnd_char_buffer[4];
    uint32_t rnd_value;
    uint32_t mask = ( (uint32_t)1 << logn) - 1;

    do {
        PQCLEAN_LEDAKEMLT12_LEAKTIME_seedexpander(seed_expander_ctx, rnd_char_buffer, required_rnd_bytes);
        /* obtain an endianness independent representation of the generated random
         bytes into an unsigned integer */
        rnd_value =  ((uint32_t)rnd_char_buffer[3] << 24) +
                     ((uint32_t)rnd_char_buffer[2] << 16) +
                     ((uint32_t)rnd_char_buffer[1] <<  8) +
                     ((uint32_t)rnd_char_buffer[0] <<  0) ;
        rnd_value = mask & rnd_value;
    } while (rnd_value >= n);

    return rnd_value;
}

/* Obtains fresh randomness and seed-expands it until all the required positions
 * for the '1's in the circulant block are obtained */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_rand_circulant_sparse_block(POSITION_T *pos_ones,
        size_t countOnes,
        AES_XOF_struct *seed_expander_ctx) {

    size_t duplicated, placedOnes = 0;
    POSITION_T p;

    while (placedOnes < countOnes) {
        p = rand_range(NUM_BITS_GF2X_ELEMENT,
                       P_BITS,
                       seed_expander_ctx);
        duplicated = 0;
        for (size_t j = 0; j < placedOnes; j++) {
            if (pos_ones[j] == p) {
                duplicated = 1;
            }
        }
        if (duplicated == 0) {
            pos_ones[placedOnes] = p;
            placedOnes++;
        }
    }
}

/* Returns random weight-t circulant block */
void PQCLEAN_LEDAKEMLT12_LEAKTIME_rand_circulant_blocks_sequence(DIGIT sequence[N0 * NUM_DIGITS_GF2X_ELEMENT],
        AES_XOF_struct *seed_expander_ctx) {

    size_t polyIndex, duplicated, counter = 0;
    POSITION_T p, exponent, rndPos[NUM_ERRORS_T];

    memset(sequence, 0x00, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);

    while (counter < NUM_ERRORS_T) {
        p = rand_range(N0 * NUM_BITS_GF2X_ELEMENT, P_BITS, seed_expander_ctx);
        duplicated = 0;
        for (size_t j = 0; j < counter; j++) {
            if (rndPos[j] == p) {
                duplicated = 1;
            }
        }
        if (duplicated == 0) {
            rndPos[counter] = p;
            counter++;
        }
    }
    for (size_t j = 0; j < counter; j++) {
        polyIndex = rndPos[j] / P;
        exponent = rndPos[j] % P;
        PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_set_coeff( sequence + NUM_DIGITS_GF2X_ELEMENT * polyIndex, exponent,
                ( (DIGIT) 1));
    }

}


void PQCLEAN_LEDAKEMLT12_LEAKTIME_rand_error_pos(POSITION_T errorPos[NUM_ERRORS_T],
        AES_XOF_struct *seed_expander_ctx) {

    int duplicated;
    size_t counter = 0;

    while (counter < NUM_ERRORS_T) {
        POSITION_T p = rand_range(N0 * NUM_BITS_GF2X_ELEMENT, P_BITS, seed_expander_ctx);
        duplicated = 0;
        for (size_t j = 0; j < counter; j++) {
            if (errorPos[j] == p) {
                duplicated = 1;
            }
        }
        if (duplicated == 0) {
            errorPos[counter] = p;
            counter++;
        }
    }
}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_expand_error(DIGIT sequence[N0 * NUM_DIGITS_GF2X_ELEMENT],
        const POSITION_T errorPos[NUM_ERRORS_T]) {

    size_t polyIndex;
    POSITION_T exponent;

    memset(sequence, 0x00, N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    for (int j = 0; j < NUM_ERRORS_T; j++) {
        polyIndex = errorPos[j] / P;
        exponent = errorPos[j] % P;
        PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_set_coeff( sequence + NUM_DIGITS_GF2X_ELEMENT * polyIndex, exponent,
                ( (DIGIT) 1));
    }
}


void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_tobytes(uint8_t *bytes, const DIGIT *poly) {
    size_t i, j;
    for (i = 0; i < NUM_DIGITS_GF2X_ELEMENT; i++) {
        for (j = 0; j < DIGIT_SIZE_B; j++) {
            bytes[i * DIGIT_SIZE_B + j] = (uint8_t) (poly[i] >> 8 * j);
        }
    }
}

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_frombytes(DIGIT *poly, const uint8_t *poly_bytes) {
    size_t i, j;
    for (i = 0; i < NUM_DIGITS_GF2X_ELEMENT; i++) {
        poly[i] = (DIGIT) 0;
        for (j = 0; j < DIGIT_SIZE_B; j++) {
            poly[i] |= (DIGIT) poly_bytes[i * DIGIT_SIZE_B + j] << 8 * j;
        }
    }
}
