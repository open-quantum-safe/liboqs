#include "gf2x_arith.h"

#include <string.h>  // memset(...)

void PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_add(DIGIT Res[], const DIGIT A[], const DIGIT B[], size_t n) {
    for (size_t i = 0; i < n; i++) {
        Res[i] = A[i] ^ B[i];
    }
}

/* copies len digits from a to r if b == 1 */
void PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_cmov(DIGIT *r, const DIGIT *a, size_t len, int c) {
    DIGIT mask = (DIGIT)(-c);
    for (size_t i = 0; i < len; i++) {
        r[i] ^= mask & (a[i] ^ r[i]);
    }
}

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void PQCLEAN_LEDAKEMLT52_LEAKTIME_right_bit_shift_n(size_t length, DIGIT in[], size_t amount) {
    if ( amount == 0 ) {
        return;
    }
    size_t j;
    DIGIT mask;
    mask = ((DIGIT)0x01 << amount) - 1;
    for (j = length - 1; j > 0; j--) {
        in[j] >>= amount;
        in[j] |=  (in[j - 1] & mask) << (DIGIT_SIZE_b - amount);
    }
    in[j] >>= amount;
}

/* PRE: MAX ALLOWED ROTATION AMOUNT : DIGIT_SIZE_b */
void PQCLEAN_LEDAKEMLT52_LEAKTIME_left_bit_shift_n(size_t length, DIGIT in[], size_t amount) {
    if ( amount == 0 ) {
        return;
    }
    size_t j;
    DIGIT mask;
    mask = ~(((DIGIT)0x01 << (DIGIT_SIZE_b - amount)) - 1);
    for (j = 0 ; j < length - 1; j++) {
        in[j] <<= amount;
        in[j] |=  (in[j + 1] & mask) >> (DIGIT_SIZE_b - amount);
    }
    in[j] <<= amount;
}


static void gf2x_mul1(DIGIT *R, const DIGIT A, const DIGIT B) {
    DIGIT tmp;

    R[0] = 0;
    R[1] = (A & 1) * B;
    for (uint8_t i = 1; i < DIGIT_SIZE_b; i++) {
        tmp = ((A >> i) & 1) * B;
        R[1] ^= tmp << i;
        R[0] ^= tmp >> (DIGIT_SIZE_b - i);
    }
}

static void gf2x_mul_n(DIGIT *R, const DIGIT *A, const DIGIT *B, size_t n) {
    DIGIT tmp[2];

    memset(R, 0x00, 2 * n * sizeof(DIGIT));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            gf2x_mul1(tmp, A[i], B[j]);
            R[i + j] ^= tmp[0];
            R[i + j + 1] ^= tmp[1];
        }
    }
}

static void gf2x_cpy(DIGIT *R, const DIGIT *A, size_t len) {
    for (size_t i = 0; i < len; i++) {
        R[i] = A[i];
    }
}

/* Accumulate */
#define gf2x_add(R, A, B, n) PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_add(R, A, B, n)
#define gf2x_acc(R, B, n) PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_add(R, R, B, n)

/* allows the operands to be of different size
 * first operand must be the bigger one.
 * aligns last array elements */
static inline void gf2x_add_asymm(DIGIT *R,
                                  size_t na, const DIGIT *A,
                                  size_t nb, const DIGIT *B) {
    size_t delta = na - nb;
    gf2x_cpy(R, A, delta);
    gf2x_add(R + delta, A + delta, B, nb);;
}

/* aligns first array elements */
static inline void gf2x_add_asymm2(DIGIT *R,
                                   size_t na, const DIGIT *A,
                                   size_t nb, const DIGIT *B) {
    size_t delta = na - nb;
    gf2x_add(R, A, B, nb);
    gf2x_cpy(R + nb, A + nb, delta);
}

/*  Karatsuba with lowered space complexity
 *  T(n) = 3 * ceil(n/2) + T(ceil(n / 2)) */
static void gf2x_mul_kar(DIGIT *R,
                         const DIGIT *A,
                         const DIGIT *B,
                         size_t n,
                         DIGIT *stack) {

    if (n < MIN_KAR_DIGITS) {
        gf2x_mul_n(R, A, B, n);
        return;
    }

    size_t l = (n + 1) / 2; // limb size = ceil(n / 2)
    size_t d = n & 1;

    const DIGIT *a1 = A;            // length n - d
    const DIGIT *a0 = A + l - d;    // length n
    const DIGIT *b1 = B;
    const DIGIT *b0 = B + l - d;

    DIGIT *aa = stack;
    DIGIT *bb = aa + l;
    DIGIT *cc = bb + l;
    stack = cc + l; // 3l space requirement at each level

    DIGIT *c3 = R + l - 2 * d;
    DIGIT *c2 = c3 + l;
    DIGIT *c1 = c2 + l;

    gf2x_mul_kar(c2, a0, b0, l, stack);      // L in low part of R
    gf2x_mul_kar(R, a1, b1, l - d, stack);   // H in higher part of R
    gf2x_add_asymm(aa, l, a0, l - d, a1);    // AH + AL
    gf2x_add_asymm(bb, l, b0, l - d, b1);    // BH + BL
    gf2x_add(cc, c3, c2, l);                 // HL + LH in cc
    gf2x_mul_kar(c3, aa, bb, l, stack);      // M = (AH + AL) x (BH + BL)
    gf2x_add_asymm(c3, l, c3, l - 2 * d, R); // add HH
    gf2x_acc(c2, c1, l);                     // add LL
    gf2x_acc(c3, cc, l);                     // add HL + LH
    gf2x_acc(c2, cc, l);                     // add HL + LH
}

static void gf2x_div_w_plus_one(DIGIT *A, size_t n) {
    size_t i;
    for (i = 0; i < n - 2; i++) {
        A[i + 1] ^= A[i]; // runs n - 2 times
    }
}

static void gf2x_shift_left_w(DIGIT *A, size_t n) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
        A[i] = A[i + 1];
    }
    A[i] = 0;
}

/* Word-aligned Toom-Cook 3, source:
 * Brent, Richard P., et al. "Faster multiplication in GF (2)[x]."
 * International Algorithmic Number Theory Symposium.
 * Springer, Berlin, Heidelberg, 2008. */
static void gf2x_mul_tc3w(DIGIT *R,
                          const DIGIT *A,
                          const DIGIT *B,
                          size_t n,
                          DIGIT *stack) {

    if (n < MIN_TOOM_DIGITS) {
        gf2x_mul_kar(R, A, B, n, stack);
        return;
    }

    size_t l = (n + 2) / 3;                     // size of a0, a1, b0, b1
    size_t r = n - 2 * l;                       // remaining sizes (a2, b2)
    size_t x = 2 * l + 4;                       // size of c1, c2, c3, c4
    size_t z = r + 2 > l + 1 ? r + 2 : l + 1;   // size of c5

    const DIGIT *a0 = A;
    const DIGIT *a1 = A + l;
    const DIGIT *a2 = A + 2 * l;
    const DIGIT *b0 = B;
    const DIGIT *b1 = B + l;
    const DIGIT *b2 = B + 2 * l;

    DIGIT *c0 = R;                              // c0 and c4 in the result
    DIGIT *c4 = R + 4 * l;
    DIGIT *c1 = stack;                          // the rest in the stack
    DIGIT *c2 = c1 + x;
    DIGIT *c3 = c2 + x;
    DIGIT *c5 = c3 + x;
    stack = c5 + z;                             // Worst-case 7l + 14

    // Evaluation
    c0[0] = 0;                                  // c0[z] = a1*W + a2*W^2
    c0[l + 1] = 0;
    gf2x_cpy(c0 + 1, a1, l);
    gf2x_acc(c0 + 2, a2, r);

    c4[0] = 0;                                  // c4[z] = b1*W + b2*W^2
    c4[l + 1] = 0;
    gf2x_cpy(c4 + 1, b1, l);
    gf2x_acc(c4 + 2, b2, r);

    gf2x_cpy(c5, a0, l);                        // c5[l] = a0 + a1 + a2
    gf2x_acc(c5, a1, l);
    gf2x_acc(c5, a2, r);

    gf2x_cpy(c2, b0, l);                        // c2[l] = b0 + b1 + b2
    gf2x_acc(c2, b1, l);
    gf2x_acc(c2, b2, r);

    gf2x_mul_tc3w(c1, c2, c5, l, stack);        // c1[2l] = c2 * c5
    gf2x_add_asymm2(c5, z, c0, l, c5);          // c5[z] += c0, z >= l
    gf2x_add_asymm2(c2, z, c4, l, c2);          // c2[z] += c4, idem
    gf2x_acc(c0, a0, l);                        // c0[l] += a0
    gf2x_acc(c4, b0, l);                        // c4[l] += b0
    gf2x_mul_tc3w(c3, c2, c5, z, stack);        // c3[2z] = c2 * c5
    gf2x_mul_tc3w(c2, c0, c4, z, stack);        // c2[2z] = c0 * c4
    gf2x_mul_tc3w(c0, a0, b0, l, stack);        // c0[2l] = a0 * b0
    gf2x_mul_tc3w(c4, a2, b2, r, stack);        // c4[2r] = a2 * b2

    // Interpolation
    gf2x_acc(c3, c2, 2 * z);                    // c3[2z] += c2
    gf2x_acc(c2, c0, 2 * l);                    // c2[2z] += c0
    gf2x_shift_left_w(c2, 2 * z);               // c2[2z] = c2/y + c3
    gf2x_acc(c2, c3, 2 * z);
    gf2x_acc(c2, c4, 2 * r);                    // c2[2z] += c4 + c4**3
    gf2x_acc(c2 + 3, c4, 2 * r);
    gf2x_div_w_plus_one(c2, 2 * z);             // c2[2z-1] = c2/(W+1)
    gf2x_acc(c1, c0, 2 * l);                    // c1[2l] += c0
    gf2x_acc(c3, c1, 2 * l);                    // c3[2z] += c1
    gf2x_shift_left_w(c3, 2 * z);               // c3[2z-2] = c3/(W^2 + W)
    gf2x_div_w_plus_one(c3, 2 * z - 1);
    gf2x_add_asymm2(c1, 2 * z, c2, 2 * l, c1);  // c1[2z-1] += c2 + c4
    gf2x_acc(c1, c4, 2 * r);                    // size c2 >= c1 >= c4
    gf2x_acc(c2, c3, 2 * z - 1);                // c2[2z-1] += c3

    // Recombination
    gf2x_cpy(R + 2 * l, c2, 2 * l);
    gf2x_acc(R + l, c1, 2 * z - 1);
    gf2x_acc(R + 3 * l, c3, 2 * z - 1);
}

void PQCLEAN_LEDAKEMLT52_LEAKTIME_gf2x_mul(DIGIT *R, const DIGIT *A, const DIGIT *B, size_t n) {
    DIGIT stack[STACK_WORDS];
    gf2x_mul_tc3w(R, A, B, n, stack);
}

