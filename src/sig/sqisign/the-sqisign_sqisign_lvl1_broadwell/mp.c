#include <mp.h>
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

// double-wide multiplication
void
MUL(digit_t *out, const digit_t a, const digit_t b)
{
#ifdef RADIX_32
    uint64_t r = (uint64_t)a * b;
    out[0] = r & 0xFFFFFFFFUL;
    out[1] = r >> 32;

#elif defined(RADIX_64) && defined(_MSC_VER)
    uint64_t umul_hi;
    out[0] = _umul128(a, b, &umul_hi);
    out[1] = umul_hi;

#elif defined(RADIX_64) && defined(HAVE_UINT128)
    unsigned __int128 umul_tmp;
    umul_tmp = (unsigned __int128)(a) * (unsigned __int128)(b);
    out[0] = (uint64_t)umul_tmp;
    out[1] = (uint64_t)(umul_tmp >> 64);

#else
    register digit_t al, ah, bl, bh, temp;
    digit_t albl, albh, ahbl, ahbh, res1, res2, res3, carry;
    digit_t mask_low = (digit_t)(-1) >> (sizeof(digit_t) * 4), mask_high = (digit_t)(-1) << (sizeof(digit_t) * 4);
    al = a & mask_low;               // Low part
    ah = a >> (sizeof(digit_t) * 4); // High part
    bl = b & mask_low;
    bh = b >> (sizeof(digit_t) * 4);

    albl = al * bl;
    albh = al * bh;
    ahbl = ah * bl;
    ahbh = ah * bh;
    out[0] = albl & mask_low; // out00

    res1 = albl >> (sizeof(digit_t) * 4);
    res2 = ahbl & mask_low;
    res3 = albh & mask_low;
    temp = res1 + res2 + res3;
    carry = temp >> (sizeof(digit_t) * 4);
    out[0] ^= temp << (sizeof(digit_t) * 4); // out01

    res1 = ahbl >> (sizeof(digit_t) * 4);
    res2 = albh >> (sizeof(digit_t) * 4);
    res3 = ahbh & mask_low;
    temp = res1 + res2 + res3 + carry;
    out[1] = temp & mask_low; // out10
    carry = temp & mask_high;
    out[1] ^= (ahbh & mask_high) + carry; // out11

#endif
}

void
mp_add(digit_t *c, const digit_t *a, const digit_t *b, const unsigned int nwords)
{ // Multiprecision addition
    unsigned int i, carry = 0;

    for (i = 0; i < nwords; i++) {
        ADDC(c[i], carry, a[i], b[i], carry);
    }
}

digit_t
mp_shiftr(digit_t *x, const unsigned int shift, const unsigned int nwords)
{ // Multiprecision right shift by 1...RADIX-1
    digit_t bit_out = x[0] & 1;

    for (unsigned int i = 0; i < nwords - 1; i++) {
        SHIFTR(x[i + 1], x[i], shift, x[i], RADIX);
    }
    x[nwords - 1] >>= shift;
    return bit_out;
}

void
mp_shiftl(digit_t *x, const unsigned int shift, const unsigned int nwords)
{ // Multiprecision left shift by 1...RADIX-1

    for (int i = nwords - 1; i > 0; i--) {
        SHIFTL(x[i], x[i - 1], shift, x[i], RADIX);
    }
    x[0] <<= shift;
}

void
multiple_mp_shiftl(digit_t *x, const unsigned int shift, const unsigned int nwords)
{
    int t = shift;
    while (t > RADIX - 1) {
        mp_shiftl(x, RADIX - 1, nwords);
        t = t - (RADIX - 1);
    }
    mp_shiftl(x, t, nwords);
}

// The below functions were taken from the EC module

void
mp_sub(digit_t *c, const digit_t *a, const digit_t *b, const unsigned int nwords)
{ // Multiprecision subtraction, assuming a > b
    unsigned int i, borrow = 0;

    for (i = 0; i < nwords; i++) {
        SUBC(c[i], borrow, a[i], b[i], borrow);
    }
}

void
select_ct(digit_t *c, const digit_t *a, const digit_t *b, const digit_t mask, const int nwords)
{ // Select c <- a if mask = 0, select c <- b if mask = 1...1

    for (int i = 0; i < nwords; i++) {
        c[i] = ((a[i] ^ b[i]) & mask) ^ a[i];
    }
}

void
swap_ct(digit_t *a, digit_t *b, const digit_t option, const int nwords)
{ // Swap entries
  // If option = 0 then P <- P and Q <- Q, else if option = 0xFF...FF then a <- b and b <- a
    digit_t temp;

    for (int i = 0; i < nwords; i++) {
        temp = option & (a[i] ^ b[i]);
        a[i] = temp ^ a[i];
        b[i] = temp ^ b[i];
    }
}

int
mp_compare(const digit_t *a, const digit_t *b, unsigned int nwords)
{ // Multiprecision comparison, a=b? : (1) a>b, (0) a=b, (-1) a<b

    for (int i = nwords - 1; i >= 0; i--) {
        if (a[i] > b[i])
            return 1;
        else if (a[i] < b[i])
            return -1;
    }
    return 0;
}

bool
mp_is_zero(const digit_t *a, unsigned int nwords)
{ // Is a multiprecision element zero?
  // Returns 1 (true) if a=0, 0 (false) otherwise
    digit_t r = 0;

    for (unsigned int i = 0; i < nwords; i++)
        r |= a[i] ^ 0;

    return (bool)is_digit_zero_ct(r);
}

void
mp_mul2(digit_t *c, const digit_t *a, const digit_t *b)
{ // Multiprecision multiplication fixed to two-digit operands
    unsigned int carry = 0;
    digit_t t0[2], t1[2], t2[2];

    MUL(t0, a[0], b[0]);
    MUL(t1, a[0], b[1]);
    ADDC(t0[1], carry, t0[1], t1[0], carry);
    ADDC(t1[1], carry, 0, t1[1], carry);
    MUL(t2, a[1], b[1]);
    ADDC(t2[0], carry, t2[0], t1[1], carry);
    ADDC(t2[1], carry, 0, t2[1], carry);
    c[0] = t0[0];
    c[1] = t0[1];
    c[2] = t2[0];
    c[3] = t2[1];
}

void
mp_print(const digit_t *a, size_t nwords)
{
    printf("0x");
    for (size_t i = 0; i < nwords; i++) {
#ifdef RADIX_32
        printf("%08" PRIx32, a[nwords - i - 1]); // Print each word with 8 hex digits
#elif defined(RADIX_64)
        printf("%016" PRIx64, a[nwords - i - 1]); // Print each word with 16 hex digits
#endif
    }
}

void
mp_copy(digit_t *b, const digit_t *a, size_t nwords)
{
    for (size_t i = 0; i < nwords; i++) {
        b[i] = a[i];
    }
}

void
mp_mul(digit_t *c, const digit_t *a, const digit_t *b, size_t nwords)
{
    // Multiprecision multiplication, c = a*b, for nwords-digit inputs, with nwords-digit output
    // explicitly does not use the higher half of c, as we do not need in our applications
    digit_t carry, UV[2], t[nwords], cc[nwords];

    for (size_t i = 0; i < nwords; i++) {
        cc[i] = 0;
    }

    for (size_t i = 0; i < nwords; i++) {

        MUL(t, a[i], b[0]);

        for (size_t j = 1; j < nwords - 1; j++) {
            MUL(UV, a[i], b[j]);
            ADDC(t[j], carry, t[j], UV[0], 0);
            t[j + 1] = UV[1] + carry;
        }

        int j = nwords - 1;
        MUL(UV, a[i], b[j]);
        ADDC(t[j], carry, t[j], UV[0], 0);

        mp_add(&cc[i], &cc[i], t, nwords - i);
    }

    mp_copy(c, cc, nwords);
}

void
mp_mod_2exp(digit_t *a, unsigned int e, unsigned int nwords)
{ // Multiprecision modulo 2^e, with 0 <= a < 2^(e)
    unsigned int i, q = e >> LOG2RADIX, r = e & (RADIX - 1);

    if (q < nwords) {
        a[q] &= ((digit_t)1 << r) - 1;

        for (i = q + 1; i < nwords; i++) {
            a[i] = 0;
        }
    }
}

void
mp_neg(digit_t *a, unsigned int nwords)
{ // negates a
    for (size_t i = 0; i < nwords; i++) {
        a[i] ^= -1;
    }

    a[0] += 1;
}

bool
mp_is_one(const digit_t *x, unsigned int nwords)
{ // returns true if x represents 1, and false otherwise
    if (x[0] != 1) {
        return false;
    }

    for (size_t i = 1; i < nwords; i++) {
        if (x[i] != 0) {
            return false;
        }
    }
    return true;
}

void
mp_inv_2e(digit_t *b, const digit_t *a, int e, unsigned int nwords)
{ // Inversion modulo 2^e, using Newton's method and Hensel lifting
    // we take the first power of 2 larger than e to use
    // requires a to be odd, of course
    // returns b such that a*b = 1 mod 2^e
    assert((a[0] & 1) == 1);

    digit_t x[nwords], y[nwords], aa[nwords], mp_one[nwords], tmp[nwords];
    mp_copy(aa, a, nwords);

    mp_one[0] = 1;
    for (unsigned int i = 1; i < nwords; i++) {
        mp_one[i] = 0;
    }

    int p = 1;
    while ((1 << p) < e) {
        p++;
    }
    p -= 2; // using k = 4 for initial inverse
    int w = (1 << (p + 2));

    mp_mod_2exp(aa, w, nwords);
    mp_add(x, aa, aa, nwords);
    mp_add(x, x, aa, nwords);  // should be 3a
    x[0] ^= (1 << 1);          // so that x equals (3a)^2 xor 2
    mp_mod_2exp(x, w, nwords); // now x*a = 1 mod 2^4, which we lift

    mp_mul(tmp, aa, x, nwords);
    mp_neg(tmp, nwords);
    mp_add(y, mp_one, tmp, nwords);

    // Hensel lifting for p rounds
    for (int i = 0; i < p; i++) {
        mp_add(tmp, mp_one, y, nwords);
        mp_mul(x, x, tmp, nwords);
        mp_mul(y, y, y, nwords);
    }

    mp_mod_2exp(x, w, nwords);
    mp_copy(b, x, nwords);

    //  verify results
    mp_mul(x, x, aa, nwords);
    mp_mod_2exp(x, w, nwords);
    assert(mp_is_one(x, nwords));
}

void
mp_invert_matrix(digit_t *r1, digit_t *r2, digit_t *s1, digit_t *s2, int e, unsigned int nwords)
{
    // given a matrix ( ( a, b ), (c,  d) ) of values mod 2^e
    // returns the inverse matrix gamma ( (d, -b), (-c, a) )
    // where gamma is the inverse of the determinant a*d - b*c
    // assumes the matrix is invertible, otherwises, inversion of determinant fails

    int p = 1;
    while ((1 << p) < e) {
        p++;
    }
    int w = (1 << (p));

    digit_t det[nwords], tmp[nwords], resa[nwords], resb[nwords], resc[nwords], resd[nwords];
    mp_mul(tmp, r1, s2, nwords);
    mp_mul(det, r2, s1, nwords);
    mp_sub(det, tmp, det, nwords);
    mp_inv_2e(det, det, e, nwords);

    mp_mul(resa, det, s2, nwords);
    mp_mul(resb, det, r2, nwords);
    mp_mul(resc, det, s1, nwords);
    mp_mul(resd, det, r1, nwords);

    mp_neg(resb, nwords);
    mp_neg(resc, nwords);

    mp_mod_2exp(resa, w, nwords);
    mp_mod_2exp(resb, w, nwords);
    mp_mod_2exp(resc, w, nwords);
    mp_mod_2exp(resd, w, nwords);

    mp_copy(r1, resa, nwords);
    mp_copy(r2, resb, nwords);
    mp_copy(s1, resc, nwords);
    mp_copy(s2, resd, nwords);
}
