#ifndef GF2X_ARITH_H
#define GF2X_ARITH_H

#include <inttypes.h>
#include <stddef.h>

/*
 * Elements of GF(2)[x] are stored in compact dense binary form.
 *
 * Each bit in a byte is assumed to be the coefficient of a binary
 * polynomial f(x), in Big-Endian format (i.e., reading everything from
 * left to right, the most significant element is met first):
 *
 * byte:(0000 0000) == 0x00 ... f(x) == 0
 * byte:(0000 0001) == 0x01 ... f(x) == 1
 * byte:(0000 0010) == 0x02 ... f(x) == x
 * byte:(0000 0011) == 0x03 ... f(x) == x+1
 * ...                      ... ...
 * byte:(0000 1111) == 0x0F ... f(x) == x^{3}+x^{2}+x+1
 * ...                      ... ...
 * byte:(1111 1111) == 0xFF ... f(x) == x^{7}+x^{6}+x^{5}+x^{4}+x^{3}+x^{2}+x+1
 *
 *
 * A "machine word" (A_i) is considered as a DIGIT.
 * Bytes in a DIGIT are assumed in Big-Endian format:
 * E.g., if sizeof(DIGIT) == 4:
 * A_i: A_{i,3} A_{i,2} A_{i,1} A_{i,0}.
 * A_{i,3} denotes the most significant byte, A_{i,0} the least significant one.
 * f(x) ==   x^{31} + ... + x^{24} +
 *         + x^{23} + ... + x^{16} +
 *         + x^{15} + ... + x^{8}  +
 *         + x^{7}  + ... + x^{0}
 *
 *
 * Multi-precision elements (i.e., with multiple DIGITs) are stored in
 * Big-endian format:
 *           A = A_{n-1} A_{n-2} ... A_1 A_0
 *
 *           position[A_{n-1}] == 0
 *           position[A_{n-2}] == 1
 *           ...
 *           position[A_{1}]  ==  n-2
 *           position[A_{0}]  ==  n-1
 */

typedef uint64_t DIGIT;
#define DIGIT_SIZE_B (8)
#define DIGIT_SIZE_b (DIGIT_SIZE_B << 3)
#define POSITION_T uint32_t

#define MIN_KAR_DIGITS      10
#define MIN_TOOM_DIGITS     42

#define STACK_KAR_ONLY      2433
#define STACK_WORDS         2892

void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_add(DIGIT Res[], const DIGIT A[], const DIGIT B[], size_t n);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_cmov(DIGIT *r, const DIGIT *a, size_t len, int c);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_right_bit_shift_n(size_t length, DIGIT in[], size_t amount);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_left_bit_shift_n(size_t length, DIGIT in[], size_t amount);
void PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_mul(DIGIT *R, const DIGIT *A, const DIGIT *B, size_t n);

#endif
