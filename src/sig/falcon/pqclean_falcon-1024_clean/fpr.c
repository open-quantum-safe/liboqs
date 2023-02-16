#include "inner.h"

/*
 * Floating-point operations.
 *
 * This file implements the non-inline functions declared in
 * fpr.h, as well as the constants for FFT / iFFT.
 *
 * ==========================(LICENSE BEGIN)============================
 *
 * Copyright (c) 2017-2019  Falcon Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ===========================(LICENSE END)=============================
 *
 * @author   Thomas Pornin <thomas.pornin@nccgroup.com>
 */



/*
 * Normalize a provided unsigned integer to the 2^63..2^64-1 range by
 * left-shifting it if necessary. The exponent e is adjusted accordingly
 * (i.e. if the value was left-shifted by n bits, then n is subtracted
 * from e). If source m is 0, then it remains 0, but e is altered.
 * Both m and e must be simple variables (no expressions allowed).
 */
#define FPR_NORM64(m, e)   do { \
        uint32_t nt; \
        \
        (e) -= 63; \
        \
        nt = (uint32_t)((m) >> 32); \
        nt = (nt | -nt) >> 31; \
        (m) ^= ((m) ^ ((m) << 32)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt << 5); \
        \
        nt = (uint32_t)((m) >> 48); \
        nt = (nt | -nt) >> 31; \
        (m) ^= ((m) ^ ((m) << 16)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt << 4); \
        \
        nt = (uint32_t)((m) >> 56); \
        nt = (nt | -nt) >> 31; \
        (m) ^= ((m) ^ ((m) <<  8)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt << 3); \
        \
        nt = (uint32_t)((m) >> 60); \
        nt = (nt | -nt) >> 31; \
        (m) ^= ((m) ^ ((m) <<  4)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt << 2); \
        \
        nt = (uint32_t)((m) >> 62); \
        nt = (nt | -nt) >> 31; \
        (m) ^= ((m) ^ ((m) <<  2)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt << 1); \
        \
        nt = (uint32_t)((m) >> 63); \
        (m) ^= ((m) ^ ((m) <<  1)) & ((uint64_t)nt - 1); \
        (e) += (int)(nt); \
    } while (0)

uint64_t
fpr_ursh(uint64_t x, int n) {
    x ^= (x ^ (x >> 32)) & -(uint64_t)(n >> 5);
    return x >> (n & 31);
}

int64_t
fpr_irsh(int64_t x, int n) {
    x ^= (x ^ (x >> 32)) & -(int64_t)(n >> 5);
    return x >> (n & 31);
}

uint64_t
fpr_ulsh(uint64_t x, int n) {
    x ^= (x ^ (x << 32)) & -(uint64_t)(n >> 5);
    return x << (n & 31);
}

fpr
FPR(int s, int e, uint64_t m) {
    fpr x;
    uint32_t t;
    unsigned f;

    /*
     * If e >= -1076, then the value is "normal"; otherwise, it
     * should be a subnormal, which we clamp down to zero.
     */
    e += 1076;
    t = (uint32_t)e >> 31;
    m &= (uint64_t)t - 1;

    /*
     * If m = 0 then we want a zero; make e = 0 too, but conserve
     * the sign.
     */
    t = (uint32_t)(m >> 54);
    e &= -(int)t;

    /*
     * The 52 mantissa bits come from m. Value m has its top bit set
     * (unless it is a zero); we leave it "as is": the top bit will
     * increment the exponent by 1, except when m = 0, which is
     * exactly what we want.
     */
    x = (((uint64_t)s << 63) | (m >> 2)) + ((uint64_t)(uint32_t)e << 52);

    /*
     * Rounding: if the low three bits of m are 011, 110 or 111,
     * then the value should be incremented to get the next
     * representable value. This implements the usual
     * round-to-nearest rule (with preference to even values in case
     * of a tie). Note that the increment may make a carry spill
     * into the exponent field, which is again exactly what we want
     * in that case.
     */
    f = (unsigned)m & 7U;
    x += (0xC8U >> f) & 1;
    return x;
}

fpr
fpr_scaled(int64_t i, int sc) {
    /*
     * To convert from int to float, we have to do the following:
     *  1. Get the absolute value of the input, and its sign
     *  2. Shift right or left the value as appropriate
     *  3. Pack the result
     *
     * We can assume that the source integer is not -2^63.
     */
    int s, e;
    uint32_t t;
    uint64_t m;

    /*
     * Extract sign bit.
     * We have: -i = 1 + ~i
     */
    s = (int)((uint64_t)i >> 63);
    i ^= -(int64_t)s;
    i += s;

    /*
     * For now we suppose that i != 0.
     * Otherwise, we set m to i and left-shift it as much as needed
     * to get a 1 in the top bit. We can do that in a logarithmic
     * number of conditional shifts.
     */
    m = (uint64_t)i;
    e = 9 + sc;
    FPR_NORM64(m, e);

    /*
     * Now m is in the 2^63..2^64-1 range. We must divide it by 512;
     * if one of the dropped bits is a 1, this should go into the
     * "sticky bit".
     */
    m |= ((uint32_t)m & 0x1FF) + 0x1FF;
    m >>= 9;

    /*
     * Corrective action: if i = 0 then all of the above was
     * incorrect, and we clamp e and m down to zero.
     */
    t = (uint32_t)((uint64_t)(i | -i) >> 63);
    m &= -(uint64_t)t;
    e &= -(int)t;

    /*
     * Assemble back everything. The FPR() function will handle cases
     * where e is too low.
     */
    return FPR(s, e, m);
}

fpr
fpr_of(int64_t i) {
    return fpr_scaled(i, 0);
}

int64_t
fpr_rint(fpr x) {
    uint64_t m, d;
    int e;
    uint32_t s, dd, f;

    /*
     * We assume that the value fits in -(2^63-1)..+(2^63-1). We can
     * thus extract the mantissa as a 63-bit integer, then right-shift
     * it as needed.
     */
    m = ((x << 10) | ((uint64_t)1 << 62)) & (((uint64_t)1 << 63) - 1);
    e = 1085 - ((int)(x >> 52) & 0x7FF);

    /*
     * If a shift of more than 63 bits is needed, then simply set m
     * to zero. This also covers the case of an input operand equal
     * to zero.
     */
    m &= -(uint64_t)((uint32_t)(e - 64) >> 31);
    e &= 63;

    /*
     * Right-shift m as needed. Shift count is e. Proper rounding
     * mandates that:
     *   - If the highest dropped bit is zero, then round low.
     *   - If the highest dropped bit is one, and at least one of the
     *     other dropped bits is one, then round up.
     *   - If the highest dropped bit is one, and all other dropped
     *     bits are zero, then round up if the lowest kept bit is 1,
     *     or low otherwise (i.e. ties are broken by "rounding to even").
     *
     * We thus first extract a word consisting of all the dropped bit
     * AND the lowest kept bit; then we shrink it down to three bits,
     * the lowest being "sticky".
     */
    d = fpr_ulsh(m, 63 - e);
    dd = (uint32_t)d | ((uint32_t)(d >> 32) & 0x1FFFFFFF);
    f = (uint32_t)(d >> 61) | ((dd | -dd) >> 31);
    m = fpr_ursh(m, e) + (uint64_t)((0xC8U >> f) & 1U);

    /*
     * Apply the sign bit.
     */
    s = (uint32_t)(x >> 63);
    return ((int64_t)m ^ -(int64_t)s) + (int64_t)s;
}

int64_t
fpr_floor(fpr x) {
    uint64_t t;
    int64_t xi;
    int e, cc;

    /*
     * We extract the integer as a _signed_ 64-bit integer with
     * a scaling factor. Since we assume that the value fits
     * in the -(2^63-1)..+(2^63-1) range, we can left-shift the
     * absolute value to make it in the 2^62..2^63-1 range: we
     * will only need a right-shift afterwards.
     */
    e = (int)(x >> 52) & 0x7FF;
    t = x >> 63;
    xi = (int64_t)(((x << 10) | ((uint64_t)1 << 62))
                   & (((uint64_t)1 << 63) - 1));
    xi = (xi ^ -(int64_t)t) + (int64_t)t;
    cc = 1085 - e;

    /*
     * We perform an arithmetic right-shift on the value. This
     * applies floor() semantics on both positive and negative values
     * (rounding toward minus infinity).
     */
    xi = fpr_irsh(xi, cc & 63);

    /*
     * If the true shift count was 64 or more, then we should instead
     * replace xi with 0 (if nonnegative) or -1 (if negative). Edge
     * case: -0 will be floored to -1, not 0 (whether this is correct
     * is debatable; in any case, the other functions normalize zero
     * to +0).
     *
     * For an input of zero, the non-shifted xi was incorrect (we used
     * a top implicit bit of value 1, not 0), but this does not matter
     * since this operation will clamp it down.
     */
    xi ^= (xi ^ -(int64_t)t) & -(int64_t)((uint32_t)(63 - cc) >> 31);
    return xi;
}

int64_t
fpr_trunc(fpr x) {
    uint64_t t, xu;
    int e, cc;

    /*
     * Extract the absolute value. Since we assume that the value
     * fits in the -(2^63-1)..+(2^63-1) range, we can left-shift
     * the absolute value into the 2^62..2^63-1 range, and then
     * do a right shift afterwards.
     */
    e = (int)(x >> 52) & 0x7FF;
    xu = ((x << 10) | ((uint64_t)1 << 62)) & (((uint64_t)1 << 63) - 1);
    cc = 1085 - e;
    xu = fpr_ursh(xu, cc & 63);

    /*
     * If the exponent is too low (cc > 63), then the shift was wrong
     * and we must clamp the value to 0. This also covers the case
     * of an input equal to zero.
     */
    xu &= -(uint64_t)((uint32_t)(cc - 64) >> 31);

    /*
     * Apply back the sign, if the source value is negative.
     */
    t = x >> 63;
    xu = (xu ^ -t) + t;
    return *(int64_t *)&xu;
}

fpr
fpr_add(fpr x, fpr y) {
    uint64_t m, xu, yu, za;
    uint32_t cs;
    int ex, ey, sx, sy, cc;

    /*
     * Make sure that the first operand (x) has the larger absolute
     * value. This guarantees that the exponent of y is less than
     * or equal to the exponent of x, and, if they are equal, then
     * the mantissa of y will not be greater than the mantissa of x.
     *
     * After this swap, the result will have the sign x, except in
     * the following edge case: abs(x) = abs(y), and x and y have
     * opposite sign bits; in that case, the result shall be +0
     * even if the sign bit of x is 1. To handle this case properly,
     * we do the swap is abs(x) = abs(y) AND the sign of x is 1.
     */
    m = ((uint64_t)1 << 63) - 1;
    za = (x & m) - (y & m);
    cs = (uint32_t)(za >> 63)
         | ((1U - (uint32_t)(-za >> 63)) & (uint32_t)(x >> 63));
    m = (x ^ y) & -(uint64_t)cs;
    x ^= m;
    y ^= m;

    /*
     * Extract sign bits, exponents and mantissas. The mantissas are
     * scaled up to 2^55..2^56-1, and the exponent is unbiased. If
     * an operand is zero, its mantissa is set to 0 at this step, and
     * its exponent will be -1078.
     */
    ex = (int)(x >> 52);
    sx = ex >> 11;
    ex &= 0x7FF;
    m = (uint64_t)(uint32_t)((ex + 0x7FF) >> 11) << 52;
    xu = ((x & (((uint64_t)1 << 52) - 1)) | m) << 3;
    ex -= 1078;
    ey = (int)(y >> 52);
    sy = ey >> 11;
    ey &= 0x7FF;
    m = (uint64_t)(uint32_t)((ey + 0x7FF) >> 11) << 52;
    yu = ((y & (((uint64_t)1 << 52) - 1)) | m) << 3;
    ey -= 1078;

    /*
     * x has the larger exponent; hence, we only need to right-shift y.
     * If the shift count is larger than 59 bits then we clamp the
     * value to zero.
     */
    cc = ex - ey;
    yu &= -(uint64_t)((uint32_t)(cc - 60) >> 31);
    cc &= 63;

    /*
     * The lowest bit of yu is "sticky".
     */
    m = fpr_ulsh(1, cc) - 1;
    yu |= (yu & m) + m;
    yu = fpr_ursh(yu, cc);

    /*
     * If the operands have the same sign, then we add the mantissas;
     * otherwise, we subtract the mantissas.
     */
    xu += yu - ((yu << 1) & -(uint64_t)(sx ^ sy));

    /*
     * The result may be smaller, or slightly larger. We normalize
     * it to the 2^63..2^64-1 range (if xu is zero, then it stays
     * at zero).
     */
    FPR_NORM64(xu, ex);

    /*
     * Scale down the value to 2^54..s^55-1, handling the last bit
     * as sticky.
     */
    xu |= ((uint32_t)xu & 0x1FF) + 0x1FF;
    xu >>= 9;
    ex += 9;

    /*
     * In general, the result has the sign of x. However, if the
     * result is exactly zero, then the following situations may
     * be encountered:
     *   x > 0, y = -x   -> result should be +0
     *   x < 0, y = -x   -> result should be +0
     *   x = +0, y = +0  -> result should be +0
     *   x = -0, y = +0  -> result should be +0
     *   x = +0, y = -0  -> result should be +0
     *   x = -0, y = -0  -> result should be -0
     *
     * But at the conditional swap step at the start of the
     * function, we ensured that if abs(x) = abs(y) and the
     * sign of x was 1, then x and y were swapped. Thus, the
     * two following cases cannot actually happen:
     *   x < 0, y = -x
     *   x = -0, y = +0
     * In all other cases, the sign bit of x is conserved, which
     * is what the FPR() function does. The FPR() function also
     * properly clamps values to zero when the exponent is too
     * low, but does not alter the sign in that case.
     */
    return FPR(sx, ex, xu);
}

fpr
fpr_sub(fpr x, fpr y) {
    y ^= (uint64_t)1 << 63;
    return fpr_add(x, y);
}

fpr
fpr_neg(fpr x) {
    x ^= (uint64_t)1 << 63;
    return x;
}

fpr
fpr_half(fpr x) {
    /*
     * To divide a value by 2, we just have to subtract 1 from its
     * exponent, but we have to take care of zero.
     */
    uint32_t t;

    x -= (uint64_t)1 << 52;
    t = (((uint32_t)(x >> 52) & 0x7FF) + 1) >> 11;
    x &= (uint64_t)t - 1;
    return x;
}

fpr
fpr_double(fpr x) {
    /*
     * To double a value, we just increment by one the exponent. We
     * don't care about infinites or NaNs; however, 0 is a
     * special case.
     */
    x += (uint64_t)((((unsigned)(x >> 52) & 0x7FFU) + 0x7FFU) >> 11) << 52;
    return x;
}

fpr
fpr_mul(fpr x, fpr y) {
    uint64_t xu, yu, w, zu, zv;
    uint32_t x0, x1, y0, y1, z0, z1, z2;
    int ex, ey, d, e, s;

    /*
     * Extract absolute values as scaled unsigned integers. We
     * don't extract exponents yet.
     */
    xu = (x & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);
    yu = (y & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);

    /*
     * We have two 53-bit integers to multiply; we need to split
     * each into a lower half and a upper half. Moreover, we
     * prefer to have lower halves to be of 25 bits each, for
     * reasons explained later on.
     */
    x0 = (uint32_t)xu & 0x01FFFFFF;
    x1 = (uint32_t)(xu >> 25);
    y0 = (uint32_t)yu & 0x01FFFFFF;
    y1 = (uint32_t)(yu >> 25);
    w = (uint64_t)x0 * (uint64_t)y0;
    z0 = (uint32_t)w & 0x01FFFFFF;
    z1 = (uint32_t)(w >> 25);
    w = (uint64_t)x0 * (uint64_t)y1;
    z1 += (uint32_t)w & 0x01FFFFFF;
    z2 = (uint32_t)(w >> 25);
    w = (uint64_t)x1 * (uint64_t)y0;
    z1 += (uint32_t)w & 0x01FFFFFF;
    z2 += (uint32_t)(w >> 25);
    zu = (uint64_t)x1 * (uint64_t)y1;
    z2 += (z1 >> 25);
    z1 &= 0x01FFFFFF;
    zu += z2;

    /*
     * Since xu and yu are both in the 2^52..2^53-1 range, the
     * product is in the 2^104..2^106-1 range. We first reassemble
     * it and round it into the 2^54..2^56-1 range; the bottom bit
     * is made "sticky". Since the low limbs z0 and z1 are 25 bits
     * each, we just take the upper part (zu), and consider z0 and
     * z1 only for purposes of stickiness.
     * (This is the reason why we chose 25-bit limbs above.)
     */
    zu |= ((z0 | z1) + 0x01FFFFFF) >> 25;

    /*
     * We normalize zu to the 2^54..s^55-1 range: it could be one
     * bit too large at this point. This is done with a conditional
     * right-shift that takes into account the sticky bit.
     */
    zv = (zu >> 1) | (zu & 1);
    w = zu >> 55;
    zu ^= (zu ^ zv) & -w;

    /*
     * Get the aggregate scaling factor:
     *
     *   - Each exponent is biased by 1023.
     *
     *   - Integral mantissas are scaled by 2^52, hence an
     *     extra 52 bias for each exponent.
     *
     *   - However, we right-shifted z by 50 bits, and then
     *     by 0 or 1 extra bit (depending on the value of w).
     *
     * In total, we must add the exponents, then subtract
     * 2 * (1023 + 52), then add 50 + w.
     */
    ex = (int)((x >> 52) & 0x7FF);
    ey = (int)((y >> 52) & 0x7FF);
    e = ex + ey - 2100 + (int)w;

    /*
     * Sign bit is the XOR of the operand sign bits.
     */
    s = (int)((x ^ y) >> 63);

    /*
     * Corrective actions for zeros: if either of the operands is
     * zero, then the computations above were wrong. Test for zero
     * is whether ex or ey is zero. We just have to set the mantissa
     * (zu) to zero, the FPR() function will normalize e.
     */
    d = ((ex + 0x7FF) & (ey + 0x7FF)) >> 11;
    zu &= -(uint64_t)d;

    /*
     * FPR() packs the result and applies proper rounding.
     */
    return FPR(s, e, zu);
}

fpr
fpr_sqr(fpr x) {
    return fpr_mul(x, x);
}

fpr
fpr_div(fpr x, fpr y) {
    uint64_t xu, yu, q, q2, w;
    int i, ex, ey, e, d, s;

    /*
     * Extract mantissas of x and y (unsigned).
     */
    xu = (x & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);
    yu = (y & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);

    /*
     * Perform bit-by-bit division of xu by yu. We run it for 55 bits.
     */
    q = 0;
    for (i = 0; i < 55; i ++) {
        /*
         * If yu is less than or equal xu, then subtract it and
         * push a 1 in the quotient; otherwise, leave xu unchanged
         * and push a 0.
         */
        uint64_t b;

        b = ((xu - yu) >> 63) - 1;
        xu -= b & yu;
        q |= b & 1;
        xu <<= 1;
        q <<= 1;
    }

    /*
     * We got 55 bits in the quotient, followed by an extra zero. We
     * want that 56th bit to be "sticky": it should be a 1 if and
     * only if the remainder (xu) is non-zero.
     */
    q |= (xu | -xu) >> 63;

    /*
     * Quotient is at most 2^56-1. Its top bit may be zero, but in
     * that case the next-to-top bit will be a one, since the
     * initial xu and yu were both in the 2^52..2^53-1 range.
     * We perform a conditional shift to normalize q to the
     * 2^54..2^55-1 range (with the bottom bit being sticky).
     */
    q2 = (q >> 1) | (q & 1);
    w = q >> 55;
    q ^= (q ^ q2) & -w;

    /*
     * Extract exponents to compute the scaling factor:
     *
     *   - Each exponent is biased and we scaled them up by
     *     52 bits; but these biases will cancel out.
     *
     *   - The division loop produced a 55-bit shifted result,
     *     so we must scale it down by 55 bits.
     *
     *   - If w = 1, we right-shifted the integer by 1 bit,
     *     hence we must add 1 to the scaling.
     */
    ex = (int)((x >> 52) & 0x7FF);
    ey = (int)((y >> 52) & 0x7FF);
    e = ex - ey - 55 + (int)w;

    /*
     * Sign is the XOR of the signs of the operands.
     */
    s = (int)((x ^ y) >> 63);

    /*
     * Corrective actions for zeros: if x = 0, then the computation
     * is wrong, and we must clamp e and q to 0. We do not care
     * about the case y = 0 (as per assumptions in this module,
     * the caller does not perform divisions by zero).
     */
    d = (ex + 0x7FF) >> 11;
    s &= d;
    e &= -d;
    q &= -(uint64_t)d;

    /*
     * FPR() packs the result and applies proper rounding.
     */
    return FPR(s, e, q);
}

fpr
fpr_inv(fpr x) {
    return fpr_div(4607182418800017408u, x);
}

fpr
fpr_sqrt(fpr x) {
    uint64_t xu, q, s, r;
    int ex, e;

    /*
     * Extract the mantissa and the exponent. We don't care about
     * the sign: by assumption, the operand is nonnegative.
     * We want the "true" exponent corresponding to a mantissa
     * in the 1..2 range.
     */
    xu = (x & (((uint64_t)1 << 52) - 1)) | ((uint64_t)1 << 52);
    ex = (int)((x >> 52) & 0x7FF);
    e = ex - 1023;

    /*
     * If the exponent is odd, double the mantissa and decrement
     * the exponent. The exponent is then halved to account for
     * the square root.
     */
    xu += xu & -(uint64_t)(e & 1);
    e >>= 1;

    /*
     * Double the mantissa.
     */
    xu <<= 1;

    /*
     * We now have a mantissa in the 2^53..2^55-1 range. It
     * represents a value between 1 (inclusive) and 4 (exclusive)
     * in fixed point notation (with 53 fractional bits). We
     * compute the square root bit by bit.
     */
    q = 0;
    s = 0;
    r = (uint64_t)1 << 53;
    for (int i = 0; i < 54; i ++) {
        uint64_t t, b;

        t = s + r;
        b = ((xu - t) >> 63) - 1;
        s += (r << 1) & b;
        xu -= t & b;
        q += r & b;
        xu <<= 1;
        r >>= 1;
    }

    /*
     * Now, q is a rounded-low 54-bit value, with a leading 1,
     * 52 fractional digits, and an additional guard bit. We add
     * an extra sticky bit to account for what remains of the operand.
     */
    q <<= 1;
    q |= (xu | -xu) >> 63;

    /*
     * Result q is in the 2^54..2^55-1 range; we bias the exponent
     * by 54 bits (the value e at that point contains the "true"
     * exponent, but q is now considered an integer, i.e. scaled
     * up.
     */
    e -= 54;

    /*
     * Corrective action for an operand of value zero.
     */
    q &= -(uint64_t)((ex + 0x7FF) >> 11);

    /*
     * Apply rounding and back result.
     */
    return FPR(0, e, q);
}

int
fpr_lt(fpr x, fpr y) {
    /*
     * If both x and y are positive, then a signed comparison yields
     * the proper result:
     *   - For positive values, the order is preserved.
     *   - The sign bit is at the same place as in integers, so
     *     sign is preserved.
     * Moreover, we can compute [x < y] as sgn(x-y) and the computation
     * of x-y will not overflow.
     *
     * If the signs differ, then sgn(x) gives the proper result.
     *
     * If both x and y are negative, then the order is reversed.
     * Hence [x < y] = sgn(y-x). We must compute this separately from
     * sgn(x-y); simply inverting sgn(x-y) would not handle the edge
     * case x = y properly.
     */
    int cc0, cc1;
    int64_t sx;
    int64_t sy;

    sx = *(int64_t *)&x;
    sy = *(int64_t *)&y;
    sy &= ~((sx ^ sy) >> 63); /* set sy=0 if signs differ */

    cc0 = (int)((sx - sy) >> 63) & 1; /* Neither subtraction overflows when */
    cc1 = (int)((sy - sx) >> 63) & 1; /* the signs are the same. */

    return cc0 ^ ((cc0 ^ cc1) & (int)((x & y) >> 63));
}

uint64_t
fpr_expm_p63(fpr x, fpr ccs) {
    /*
     * Polynomial approximation of exp(-x) is taken from FACCT:
     *   https://eprint.iacr.org/2018/1234
     * Specifically, values are extracted from the implementation
     * referenced from the FACCT article, and available at:
     *   https://github.com/raykzhao/gaussian
     * Here, the coefficients have been scaled up by 2^63 and
     * converted to integers.
     *
     * Tests over more than 24 billions of random inputs in the
     * 0..log(2) range have never shown a deviation larger than
     * 2^(-50) from the true mathematical value.
     */
    static const uint64_t C[] = {
        0x00000004741183A3u,
        0x00000036548CFC06u,
        0x0000024FDCBF140Au,
        0x0000171D939DE045u,
        0x0000D00CF58F6F84u,
        0x000680681CF796E3u,
        0x002D82D8305B0FEAu,
        0x011111110E066FD0u,
        0x0555555555070F00u,
        0x155555555581FF00u,
        0x400000000002B400u,
        0x7FFFFFFFFFFF4800u,
        0x8000000000000000u
    };

    uint64_t z, y;
    size_t u;
    uint32_t z0, z1, y0, y1;
    uint64_t a, b;

    y = C[0];
    z = (uint64_t)fpr_trunc(fpr_mul(x, fpr_ptwo63)) << 1;
    for (u = 1; u < (sizeof C) / sizeof(C[0]); u ++) {
        /*
         * Compute product z * y over 128 bits, but keep only
         * the top 64 bits.
         *
         * TODO: On some architectures/compilers we could use
         * some intrinsics (__umulh() on MSVC) or other compiler
         * extensions (unsigned __int128 on GCC / Clang) for
         * improved speed; however, most 64-bit architectures
         * also have appropriate IEEE754 floating-point support,
         * which is better.
         */
        uint64_t c;

        z0 = (uint32_t)z;
        z1 = (uint32_t)(z >> 32);
        y0 = (uint32_t)y;
        y1 = (uint32_t)(y >> 32);
        a = ((uint64_t)z0 * (uint64_t)y1)
            + (((uint64_t)z0 * (uint64_t)y0) >> 32);
        b = ((uint64_t)z1 * (uint64_t)y0);
        c = (a >> 32) + (b >> 32);
        c += (((uint64_t)(uint32_t)a + (uint64_t)(uint32_t)b) >> 32);
        c += (uint64_t)z1 * (uint64_t)y1;
        y = C[u] - c;
    }

    /*
     * The scaling factor must be applied at the end. Since y is now
     * in fixed-point notation, we have to convert the factor to the
     * same format, and do an extra integer multiplication.
     */
    z = (uint64_t)fpr_trunc(fpr_mul(ccs, fpr_ptwo63)) << 1;
    z0 = (uint32_t)z;
    z1 = (uint32_t)(z >> 32);
    y0 = (uint32_t)y;
    y1 = (uint32_t)(y >> 32);
    a = ((uint64_t)z0 * (uint64_t)y1)
        + (((uint64_t)z0 * (uint64_t)y0) >> 32);
    b = ((uint64_t)z1 * (uint64_t)y0);
    y = (a >> 32) + (b >> 32);
    y += (((uint64_t)(uint32_t)a + (uint64_t)(uint32_t)b) >> 32);
    y += (uint64_t)z1 * (uint64_t)y1;

    return y;
}

const fpr fpr_gm_tab[] = {
    0, 0,
    9223372036854775808U,  4607182418800017408U,
    4604544271217802189U,  4604544271217802189U,
    13827916308072577997U,  4604544271217802189U,
    4606496786581982534U,  4600565431771507043U,
    13823937468626282851U,  4606496786581982534U,
    4600565431771507043U,  4606496786581982534U,
    13829868823436758342U,  4600565431771507043U,
    4607009347991985328U,  4596196889902818827U,
    13819568926757594635U,  4607009347991985328U,
    4603179351334086856U,  4605664432017547683U,
    13829036468872323491U,  4603179351334086856U,
    4605664432017547683U,  4603179351334086856U,
    13826551388188862664U,  4605664432017547683U,
    4596196889902818827U,  4607009347991985328U,
    13830381384846761136U,  4596196889902818827U,
    4607139046673687846U,  4591727299969791020U,
    13815099336824566828U,  4607139046673687846U,
    4603889326261607894U,  4605137878724712257U,
    13828509915579488065U,  4603889326261607894U,
    4606118860100255153U,  4602163548591158843U,
    13825535585445934651U,  4606118860100255153U,
    4598900923775164166U,  4606794571824115162U,
    13830166608678890970U,  4598900923775164166U,
    4606794571824115162U,  4598900923775164166U,
    13822272960629939974U,  4606794571824115162U,
    4602163548591158843U,  4606118860100255153U,
    13829490896955030961U,  4602163548591158843U,
    4605137878724712257U,  4603889326261607894U,
    13827261363116383702U,  4605137878724712257U,
    4591727299969791020U,  4607139046673687846U,
    13830511083528463654U,  4591727299969791020U,
    4607171569234046334U,  4587232218149935124U,
    13810604255004710932U,  4607171569234046334U,
    4604224084862889120U,  4604849113969373103U,
    13828221150824148911U,  4604224084862889120U,
    4606317631232591731U,  4601373767755717824U,
    13824745804610493632U,  4606317631232591731U,
    4599740487990714333U,  4606655894547498725U,
    13830027931402274533U,  4599740487990714333U,
    4606912484326125783U,  4597922303871901467U,
    13821294340726677275U,  4606912484326125783U,
    4602805845399633902U,  4605900952042040894U,
    13829272988896816702U,  4602805845399633902U,
    4605409869824231233U,  4603540801876750389U,
    13826912838731526197U,  4605409869824231233U,
    4594454542771183930U,  4607084929468638487U,
    13830456966323414295U,  4594454542771183930U,
    4607084929468638487U,  4594454542771183930U,
    13817826579625959738U,  4607084929468638487U,
    4603540801876750389U,  4605409869824231233U,
    13828781906679007041U,  4603540801876750389U,
    4605900952042040894U,  4602805845399633902U,
    13826177882254409710U,  4605900952042040894U,
    4597922303871901467U,  4606912484326125783U,
    13830284521180901591U,  4597922303871901467U,
    4606655894547498725U,  4599740487990714333U,
    13823112524845490141U,  4606655894547498725U,
    4601373767755717824U,  4606317631232591731U,
    13829689668087367539U,  4601373767755717824U,
    4604849113969373103U,  4604224084862889120U,
    13827596121717664928U,  4604849113969373103U,
    4587232218149935124U,  4607171569234046334U,
    13830543606088822142U,  4587232218149935124U,
    4607179706000002317U,  4582730748936808062U,
    13806102785791583870U,  4607179706000002317U,
    4604386048625945823U,  4604698657331085206U,
    13828070694185861014U,  4604386048625945823U,
    4606409688975526202U,  4600971798440897930U,
    13824343835295673738U,  4606409688975526202U,
    4600154912527631775U,  4606578871587619388U,
    13829950908442395196U,  4600154912527631775U,
    4606963563043808649U,  4597061974398750563U,
    13820434011253526371U,  4606963563043808649U,
    4602994049708411683U,  4605784983948558848U,
    13829157020803334656U,  4602994049708411683U,
    4605539368864982914U,  4603361638657888991U,
    13826733675512664799U,  4605539368864982914U,
    4595327571478659014U,  4607049811591515049U,
    13830421848446290857U,  4595327571478659014U,
    4607114680469659603U,  4593485039402578702U,
    13816857076257354510U,  4607114680469659603U,
    4603716733069447353U,  4605276012900672507U,
    13828648049755448315U,  4603716733069447353U,
    4606012266443150634U,  4602550884377336506U,
    13825922921232112314U,  4606012266443150634U,
    4598476289818621559U,  4606856142606846307U,
    13830228179461622115U,  4598476289818621559U,
    4606727809065869586U,  4599322407794599425U,
    13822694444649375233U,  4606727809065869586U,
    4601771097584682078U,  4606220668805321205U,
    13829592705660097013U,  4601771097584682078U,
    4604995550503212910U,  4604058477489546729U,
    13827430514344322537U,  4604995550503212910U,
    4589965306122607094U,  4607158013403433018U,
    13830530050258208826U,  4589965306122607094U,
    4607158013403433018U,  4589965306122607094U,
    13813337342977382902U,  4607158013403433018U,
    4604058477489546729U,  4604995550503212910U,
    13828367587357988718U,  4604058477489546729U,
    4606220668805321205U,  4601771097584682078U,
    13825143134439457886U,  4606220668805321205U,
    4599322407794599425U,  4606727809065869586U,
    13830099845920645394U,  4599322407794599425U,
    4606856142606846307U,  4598476289818621559U,
    13821848326673397367U,  4606856142606846307U,
    4602550884377336506U,  4606012266443150634U,
    13829384303297926442U,  4602550884377336506U,
    4605276012900672507U,  4603716733069447353U,
    13827088769924223161U,  4605276012900672507U,
    4593485039402578702U,  4607114680469659603U,
    13830486717324435411U,  4593485039402578702U,
    4607049811591515049U,  4595327571478659014U,
    13818699608333434822U,  4607049811591515049U,
    4603361638657888991U,  4605539368864982914U,
    13828911405719758722U,  4603361638657888991U,
    4605784983948558848U,  4602994049708411683U,
    13826366086563187491U,  4605784983948558848U,
    4597061974398750563U,  4606963563043808649U,
    13830335599898584457U,  4597061974398750563U,
    4606578871587619388U,  4600154912527631775U,
    13823526949382407583U,  4606578871587619388U,
    4600971798440897930U,  4606409688975526202U,
    13829781725830302010U,  4600971798440897930U,
    4604698657331085206U,  4604386048625945823U,
    13827758085480721631U,  4604698657331085206U,
    4582730748936808062U,  4607179706000002317U,
    13830551742854778125U,  4582730748936808062U,
    4607181740574479067U,  4578227681973159812U,
    13801599718827935620U,  4607181740574479067U,
    4604465633578481725U,  4604621949701367983U,
    13827993986556143791U,  4604465633578481725U,
    4606453861145241227U,  4600769149537129431U,
    13824141186391905239U,  4606453861145241227U,
    4600360675823176935U,  4606538458821337243U,
    13829910495676113051U,  4600360675823176935U,
    4606987119037722413U,  4596629994023683153U,
    13820002030878458961U,  4606987119037722413U,
    4603087070374583113U,  4605725276488455441U,
    13829097313343231249U,  4603087070374583113U,
    4605602459698789090U,  4603270878689749849U,
    13826642915544525657U,  4605602459698789090U,
    4595762727260045105U,  4607030246558998647U,
    13830402283413774455U,  4595762727260045105U,
    4607127537664763515U,  4592606767730311893U,
    13815978804585087701U,  4607127537664763515U,
    4603803453461190356U,  4605207475328619533U,
    13828579512183395341U,  4603803453461190356U,
    4606066157444814153U,  4602357870542944470U,
    13825729907397720278U,  4606066157444814153U,
    4598688984595225406U,  4606826008603986804U,
    13830198045458762612U,  4598688984595225406U,
    4606761837001494797U,  4599112075441176914U,
    13822484112295952722U,  4606761837001494797U,
    4601967947786150793U,  4606170366472647579U,
    13829542403327423387U,  4601967947786150793U,
    4605067233569943231U,  4603974338538572089U,
    13827346375393347897U,  4605067233569943231U,
    4590846768565625881U,  4607149205763218185U,
    13830521242617993993U,  4590846768565625881U,
    4607165468267934125U,  4588998070480937184U,
    13812370107335712992U,  4607165468267934125U,
    4604141730443515286U,  4604922840319727473U,
    13828294877174503281U,  4604141730443515286U,
    4606269759522929756U,  4601573027631668967U,
    13824945064486444775U,  4606269759522929756U,
    4599531889160152938U,  4606692493141721470U,
    13830064529996497278U,  4599531889160152938U,
    4606884969294623682U,  4598262871476403630U,
    13821634908331179438U,  4606884969294623682U,
    4602710690099904183U,  4605957195211051218U,
    13829329232065827026U,  4602710690099904183U,
    4605343481119364930U,  4603629178146150899U,
    13827001215000926707U,  4605343481119364930U,
    4594016801320007031U,  4607100477024622401U,
    13830472513879398209U,  4594016801320007031U,
    4607068040143112603U,  4594891488091520602U,
    13818263524946296410U,  4607068040143112603U,
    4603451617570386922U,  4605475169017376660U,
    13828847205872152468U,  4603451617570386922U,
    4605843545406134034U,  4602900303344142735U,
    13826272340198918543U,  4605843545406134034U,
    4597492765973365521U,  4606938683557690074U,
    13830310720412465882U,  4597492765973365521U,
    4606618018794815019U,  4599948172872067014U,
    13823320209726842822U,  4606618018794815019U,
    4601173347964633034U,  4606364276725003740U,
    13829736313579779548U,  4601173347964633034U,
    4604774382555066977U,  4604305528345395596U,
    13827677565200171404U,  4604774382555066977U,
    4585465300892538317U,  4607176315382986589U,
    13830548352237762397U,  4585465300892538317U,
    4607176315382986589U,  4585465300892538317U,
    13808837337747314125U,  4607176315382986589U,
    4604305528345395596U,  4604774382555066977U,
    13828146419409842785U,  4604305528345395596U,
    4606364276725003740U,  4601173347964633034U,
    13824545384819408842U,  4606364276725003740U,
    4599948172872067014U,  4606618018794815019U,
    13829990055649590827U,  4599948172872067014U,
    4606938683557690074U,  4597492765973365521U,
    13820864802828141329U,  4606938683557690074U,
    4602900303344142735U,  4605843545406134034U,
    13829215582260909842U,  4602900303344142735U,
    4605475169017376660U,  4603451617570386922U,
    13826823654425162730U,  4605475169017376660U,
    4594891488091520602U,  4607068040143112603U,
    13830440076997888411U,  4594891488091520602U,
    4607100477024622401U,  4594016801320007031U,
    13817388838174782839U,  4607100477024622401U,
    4603629178146150899U,  4605343481119364930U,
    13828715517974140738U,  4603629178146150899U,
    4605957195211051218U,  4602710690099904183U,
    13826082726954679991U,  4605957195211051218U,
    4598262871476403630U,  4606884969294623682U,
    13830257006149399490U,  4598262871476403630U,
    4606692493141721470U,  4599531889160152938U,
    13822903926014928746U,  4606692493141721470U,
    4601573027631668967U,  4606269759522929756U,
    13829641796377705564U,  4601573027631668967U,
    4604922840319727473U,  4604141730443515286U,
    13827513767298291094U,  4604922840319727473U,
    4588998070480937184U,  4607165468267934125U,
    13830537505122709933U,  4588998070480937184U,
    4607149205763218185U,  4590846768565625881U,
    13814218805420401689U,  4607149205763218185U,
    4603974338538572089U,  4605067233569943231U,
    13828439270424719039U,  4603974338538572089U,
    4606170366472647579U,  4601967947786150793U,
    13825339984640926601U,  4606170366472647579U,
    4599112075441176914U,  4606761837001494797U,
    13830133873856270605U,  4599112075441176914U,
    4606826008603986804U,  4598688984595225406U,
    13822061021450001214U,  4606826008603986804U,
    4602357870542944470U,  4606066157444814153U,
    13829438194299589961U,  4602357870542944470U,
    4605207475328619533U,  4603803453461190356U,
    13827175490315966164U,  4605207475328619533U,
    4592606767730311893U,  4607127537664763515U,
    13830499574519539323U,  4592606767730311893U,
    4607030246558998647U,  4595762727260045105U,
    13819134764114820913U,  4607030246558998647U,
    4603270878689749849U,  4605602459698789090U,
    13828974496553564898U,  4603270878689749849U,
    4605725276488455441U,  4603087070374583113U,
    13826459107229358921U,  4605725276488455441U,
    4596629994023683153U,  4606987119037722413U,
    13830359155892498221U,  4596629994023683153U,
    4606538458821337243U,  4600360675823176935U,
    13823732712677952743U,  4606538458821337243U,
    4600769149537129431U,  4606453861145241227U,
    13829825898000017035U,  4600769149537129431U,
    4604621949701367983U,  4604465633578481725U,
    13827837670433257533U,  4604621949701367983U,
    4578227681973159812U,  4607181740574479067U,
    13830553777429254875U,  4578227681973159812U,
    4607182249242036882U,  4573724215515480177U,
    13797096252370255985U,  4607182249242036882U,
    4604505071555817232U,  4604583231088591477U,
    13827955267943367285U,  4604505071555817232U,
    4606475480113671417U,  4600667422348321968U,
    13824039459203097776U,  4606475480113671417U,
    4600463181646572228U,  4606517779747998088U,
    13829889816602773896U,  4600463181646572228U,
    4606998399608725124U,  4596413578358834022U,
    13819785615213609830U,  4606998399608725124U,
    4603133304188877240U,  4605694995810664660U,
    13829067032665440468U,  4603133304188877240U,
    4605633586259814045U,  4603225210076562971U,
    13826597246931338779U,  4605633586259814045U,
    4595979936813835462U,  4607019963775302583U,
    13830392000630078391U,  4595979936813835462U,
    4607133460805585796U,  4592167175087283203U,
    13815539211942059011U,  4607133460805585796U,
    4603846496621587377U,  4605172808754305228U,
    13828544845609081036U,  4603846496621587377U,
    4606092657816072624U,  4602260871257280788U,
    13825632908112056596U,  4606092657816072624U,
    4598795050632330097U,  4606810452769876110U,
    13830182489624651918U,  4598795050632330097U,
    4606778366364612594U,  4599006600037663623U,
    13822378636892439431U,  4606778366364612594U,
    4602065906208722008U,  4606144763310860551U,
    13829516800165636359U,  4602065906208722008U,
    4605102686554936490U,  4603931940768740167U,
    13827303977623515975U,  4605102686554936490U,
    4591287158938884897U,  4607144295058764886U,
    13830516331913540694U,  4591287158938884897U,
    4607168688050493276U,  4588115294056142819U,
    13811487330910918627U,  4607168688050493276U,
    4604183020748362039U,  4604886103475043762U,
    13828258140329819570U,  4604183020748362039U,
    4606293848208650998U,  4601473544562720001U,
    13824845581417495809U,  4606293848208650998U,
    4599636300858866724U,  4606674353838411301U,
    13830046390693187109U,  4599636300858866724U,
    4606898891031025132U,  4598136582470364665U,
    13821508619325140473U,  4606898891031025132U,
    4602758354025980442U,  4605929219593405673U,
    13829301256448181481U,  4602758354025980442U,
    4605376811039722786U,  4603585091850767959U,
    13826957128705543767U,  4605376811039722786U,
    4594235767444503503U,  4607092871118901179U,
    13830464907973676987U,  4594235767444503503U,
    4607076652372832968U,  4594673119063280916U,
    13818045155918056724U,  4607076652372832968U,
    4603496309891590679U,  4605442656228245717U,
    13828814693083021525U,  4603496309891590679U,
    4605872393621214213U,  4602853162432841185U,
    13826225199287616993U,  4605872393621214213U,
    4597707695679609371U,  4606925748668145757U,
    13830297785522921565U,  4597707695679609371U,
    4606637115963965612U,  4599844446633109139U,
    13823216483487884947U,  4606637115963965612U,
    4601273700967202825U,  4606341107699334546U,
    13829713144554110354U,  4601273700967202825U,
    4604811873195349477U,  4604264921241055824U,
    13827636958095831632U,  4604811873195349477U,
    4586348876009622851U,  4607174111710118367U,
    13830546148564894175U,  4586348876009622851U,
    4607178180169683960U,  4584498631466405633U,
    13807870668321181441U,  4607178180169683960U,
    4604345904647073908U,  4604736643460027021U,
    13828108680314802829U,  4604345904647073908U,
    4606387137437298591U,  4601072712526242277U,
    13824444749381018085U,  4606387137437298591U,
    4600051662802353687U,  4606598603759044570U,
    13829970640613820378U,  4600051662802353687U,
    4606951288507767453U,  4597277522845151878U,
    13820649559699927686U,  4606951288507767453U,
    4602947266358709886U,  4605814408482919348U,
    13829186445337695156U,  4602947266358709886U,
    4605507406967535927U,  4603406726595779752U,
    13826778763450555560U,  4605507406967535927U,
    4595109641634432498U,  4607059093103722971U,
    13830431129958498779U,  4595109641634432498U,
    4607107746899444102U,  4593797652641645341U,
    13817169689496421149U,  4607107746899444102U,
    4603673059103075106U,  4605309881318010327U,
    13828681918172786135U,  4603673059103075106U,
    4605984877841711338U,  4602646891659203088U,
    13826018928513978896U,  4605984877841711338U,
    4598369669086960528U,  4606870719641066940U,
    13830242756495842748U,  4598369669086960528U,
    4606710311774494716U,  4599427256825614420U,
    13822799293680390228U,  4606710311774494716U,
    4601672213217083403U,  4606245366082353408U,
    13829617402937129216U,  4601672213217083403U,
    4604959323120302796U,  4604100215502905499U,
    13827472252357681307U,  4604959323120302796U,
    4589524267239410099U,  4607161910007591876U,
    13830533946862367684U,  4589524267239410099U,
    4607153778602162496U,  4590406145430462614U,
    13813778182285238422U,  4607153778602162496U,
    4604016517974851588U,  4605031521104517324U,
    13828403557959293132U,  4604016517974851588U,
    4606195668621671667U,  4601869677011524443U,
    13825241713866300251U,  4606195668621671667U,
    4599217346014614711U,  4606744984357082948U,
    13830117021211858756U,  4599217346014614711U,
    4606841238740778884U,  4598582729657176439U,
    13821954766511952247U,  4606841238740778884U,
    4602454542796181607U,  4606039359984203741U,
    13829411396838979549U,  4602454542796181607U,
    4605241877142478242U,  4603760198400967492U,
    13827132235255743300U,  4605241877142478242U,
    4593046061348462537U,  4607121277474223905U,
    13830493314328999713U,  4593046061348462537U,
    4607040195955932526U,  4595545269419264690U,
    13818917306274040498U,  4607040195955932526U,
    4603316355454250015U,  4605571053506370248U,
    13828943090361146056U,  4603316355454250015U,
    4605755272910869620U,  4603040651631881451U,
    13826412688486657259U,  4605755272910869620U,
    4596846128749438754U,  4606975506703684317U,
    13830347543558460125U,  4596846128749438754U,
    4606558823023444576U,  4600257918160607478U,
    13823629955015383286U,  4606558823023444576U,
    4600870609507958271U,  4606431930490633905U,
    13829803967345409713U,  4600870609507958271U,
    4604660425598397818U,  4604425958770613225U,
    13827797995625389033U,  4604660425598397818U,
    4580962600092897021U,  4607180892816495009U,
    13830552929671270817U,  4580962600092897021U,
    4607180892816495009U,  4580962600092897021U,
    13804334636947672829U,  4607180892816495009U,
    4604425958770613225U,  4604660425598397818U,
    13828032462453173626U,  4604425958770613225U,
    4606431930490633905U,  4600870609507958271U,
    13824242646362734079U,  4606431930490633905U,
    4600257918160607478U,  4606558823023444576U,
    13829930859878220384U,  4600257918160607478U,
    4606975506703684317U,  4596846128749438754U,
    13820218165604214562U,  4606975506703684317U,
    4603040651631881451U,  4605755272910869620U,
    13829127309765645428U,  4603040651631881451U,
    4605571053506370248U,  4603316355454250015U,
    13826688392309025823U,  4605571053506370248U,
    4595545269419264690U,  4607040195955932526U,
    13830412232810708334U,  4595545269419264690U,
    4607121277474223905U,  4593046061348462537U,
    13816418098203238345U,  4607121277474223905U,
    4603760198400967492U,  4605241877142478242U,
    13828613913997254050U,  4603760198400967492U,
    4606039359984203741U,  4602454542796181607U,
    13825826579650957415U,  4606039359984203741U,
    4598582729657176439U,  4606841238740778884U,
    13830213275595554692U,  4598582729657176439U,
    4606744984357082948U,  4599217346014614711U,
    13822589382869390519U,  4606744984357082948U,
    4601869677011524443U,  4606195668621671667U,
    13829567705476447475U,  4601869677011524443U,
    4605031521104517324U,  4604016517974851588U,
    13827388554829627396U,  4605031521104517324U,
    4590406145430462614U,  4607153778602162496U,
    13830525815456938304U,  4590406145430462614U,
    4607161910007591876U,  4589524267239410099U,
    13812896304094185907U,  4607161910007591876U,
    4604100215502905499U,  4604959323120302796U,
    13828331359975078604U,  4604100215502905499U,
    4606245366082353408U,  4601672213217083403U,
    13825044250071859211U,  4606245366082353408U,
    4599427256825614420U,  4606710311774494716U,
    13830082348629270524U,  4599427256825614420U,
    4606870719641066940U,  4598369669086960528U,
    13821741705941736336U,  4606870719641066940U,
    4602646891659203088U,  4605984877841711338U,
    13829356914696487146U,  4602646891659203088U,
    4605309881318010327U,  4603673059103075106U,
    13827045095957850914U,  4605309881318010327U,
    4593797652641645341U,  4607107746899444102U,
    13830479783754219910U,  4593797652641645341U,
    4607059093103722971U,  4595109641634432498U,
    13818481678489208306U,  4607059093103722971U,
    4603406726595779752U,  4605507406967535927U,
    13828879443822311735U,  4603406726595779752U,
    4605814408482919348U,  4602947266358709886U,
    13826319303213485694U,  4605814408482919348U,
    4597277522845151878U,  4606951288507767453U,
    13830323325362543261U,  4597277522845151878U,
    4606598603759044570U,  4600051662802353687U,
    13823423699657129495U,  4606598603759044570U,
    4601072712526242277U,  4606387137437298591U,
    13829759174292074399U,  4601072712526242277U,
    4604736643460027021U,  4604345904647073908U,
    13827717941501849716U,  4604736643460027021U,
    4584498631466405633U,  4607178180169683960U,
    13830550217024459768U,  4584498631466405633U,
    4607174111710118367U,  4586348876009622851U,
    13809720912864398659U,  4607174111710118367U,
    4604264921241055824U,  4604811873195349477U,
    13828183910050125285U,  4604264921241055824U,
    4606341107699334546U,  4601273700967202825U,
    13824645737821978633U,  4606341107699334546U,
    4599844446633109139U,  4606637115963965612U,
    13830009152818741420U,  4599844446633109139U,
    4606925748668145757U,  4597707695679609371U,
    13821079732534385179U,  4606925748668145757U,
    4602853162432841185U,  4605872393621214213U,
    13829244430475990021U,  4602853162432841185U,
    4605442656228245717U,  4603496309891590679U,
    13826868346746366487U,  4605442656228245717U,
    4594673119063280916U,  4607076652372832968U,
    13830448689227608776U,  4594673119063280916U,
    4607092871118901179U,  4594235767444503503U,
    13817607804299279311U,  4607092871118901179U,
    4603585091850767959U,  4605376811039722786U,
    13828748847894498594U,  4603585091850767959U,
    4605929219593405673U,  4602758354025980442U,
    13826130390880756250U,  4605929219593405673U,
    4598136582470364665U,  4606898891031025132U,
    13830270927885800940U,  4598136582470364665U,
    4606674353838411301U,  4599636300858866724U,
    13823008337713642532U,  4606674353838411301U,
    4601473544562720001U,  4606293848208650998U,
    13829665885063426806U,  4601473544562720001U,
    4604886103475043762U,  4604183020748362039U,
    13827555057603137847U,  4604886103475043762U,
    4588115294056142819U,  4607168688050493276U,
    13830540724905269084U,  4588115294056142819U,
    4607144295058764886U,  4591287158938884897U,
    13814659195793660705U,  4607144295058764886U,
    4603931940768740167U,  4605102686554936490U,
    13828474723409712298U,  4603931940768740167U,
    4606144763310860551U,  4602065906208722008U,
    13825437943063497816U,  4606144763310860551U,
    4599006600037663623U,  4606778366364612594U,
    13830150403219388402U,  4599006600037663623U,
    4606810452769876110U,  4598795050632330097U,
    13822167087487105905U,  4606810452769876110U,
    4602260871257280788U,  4606092657816072624U,
    13829464694670848432U,  4602260871257280788U,
    4605172808754305228U,  4603846496621587377U,
    13827218533476363185U,  4605172808754305228U,
    4592167175087283203U,  4607133460805585796U,
    13830505497660361604U,  4592167175087283203U,
    4607019963775302583U,  4595979936813835462U,
    13819351973668611270U,  4607019963775302583U,
    4603225210076562971U,  4605633586259814045U,
    13829005623114589853U,  4603225210076562971U,
    4605694995810664660U,  4603133304188877240U,
    13826505341043653048U,  4605694995810664660U,
    4596413578358834022U,  4606998399608725124U,
    13830370436463500932U,  4596413578358834022U,
    4606517779747998088U,  4600463181646572228U,
    13823835218501348036U,  4606517779747998088U,
    4600667422348321968U,  4606475480113671417U,
    13829847516968447225U,  4600667422348321968U,
    4604583231088591477U,  4604505071555817232U,
    13827877108410593040U,  4604583231088591477U,
    4573724215515480177U,  4607182249242036882U,
    13830554286096812690U,  4573724215515480177U,
    4607182376410422530U,  4569220649180767418U,
    13792592686035543226U,  4607182376410422530U,
    4604524701268679793U,  4604563781218984604U,
    13827935818073760412U,  4604524701268679793U,
    4606486172460753999U,  4600616459743653188U,
    13823988496598428996U,  4606486172460753999U,
    4600514338912178239U,  4606507322377452870U,
    13829879359232228678U,  4600514338912178239U,
    4607003915349878877U,  4596305267720071930U,
    13819677304574847738U,  4607003915349878877U,
    4603156351203636159U,  4605679749231851918U,
    13829051786086627726U,  4603156351203636159U,
    4605649044311923410U,  4603202304363743346U,
    13826574341218519154U,  4605649044311923410U,
    4596088445927168004U,  4607014697483910382U,
    13830386734338686190U,  4596088445927168004U,
    4607136295912168606U,  4591947271803021404U,
    13815319308657797212U,  4607136295912168606U,
    4603867938232615808U,  4605155376589456981U,
    13828527413444232789U,  4603867938232615808U,
    4606105796280968177U,  4602212250118051877U,
    13825584286972827685U,  4606105796280968177U,
    4598848011564831930U,  4606802552898869248U,
    13830174589753645056U,  4598848011564831930U,
    4606786509620734768U,  4598953786765296928U,
    13822325823620072736U,  4606786509620734768U,
    4602114767134999006U,  4606131849150971908U,
    13829503886005747716U,  4602114767134999006U,
    4605120315324767624U,  4603910660507251362U,
    13827282697362027170U,  4605120315324767624U,
    4591507261658050721U,  4607141713064252300U,
    13830513749919028108U,  4591507261658050721U,
    4607170170974224083U,  4587673791460508439U,
    13811045828315284247U,  4607170170974224083U,
    4604203581176243359U,  4604867640218014515U,
    13828239677072790323U,  4604203581176243359U,
    4606305777984577632U,  4601423692641949331U,
    13824795729496725139U,  4606305777984577632U,
    4599688422741010356U,  4606665164148251002U,
    13830037201003026810U,  4599688422741010356U,
    4606905728766014348U,  4598029484874872834U,
    13821401521729648642U,  4606905728766014348U,
    4602782121393764535U,  4605915122243179241U,
    13829287159097955049U,  4602782121393764535U,
    4605393374401988274U,  4603562972219549215U,
    13826935009074325023U,  4605393374401988274U,
    4594345179472540681U,  4607088942243446236U,
    13830460979098222044U,  4594345179472540681U,
    4607080832832247697U,  4594563856311064231U,
    13817935893165840039U,  4607080832832247697U,
    4603518581031047189U,  4605426297151190466U,
    13828798334005966274U,  4603518581031047189U,
    4605886709123365959U,  4602829525820289164U,
    13826201562675064972U,  4605886709123365959U,
    4597815040470278984U,  4606919157647773535U,
    13830291194502549343U,  4597815040470278984U,
    4606646545123403481U,  4599792496117920694U,
    13823164532972696502U,  4606646545123403481U,
    4601323770373937522U,  4606329407841126011U,
    13829701444695901819U,  4601323770373937522U,
    4604830524903495634U,  4604244531615310815U,
    13827616568470086623U,  4604830524903495634U,
    4586790578280679046U,  4607172882816799076U,
    13830544919671574884U,  4586790578280679046U,
    4607178985458280057U,  4583614727651146525U,
    13806986764505922333U,  4607178985458280057U,
    4604366005771528720U,  4604717681185626434U,
    13828089718040402242U,  4604366005771528720U,
    4606398451906509788U,  4601022290077223616U,
    13824394326931999424U,  4606398451906509788U,
    4600103317933788342U,  4606588777269136769U,
    13829960814123912577U,  4600103317933788342U,
    4606957467106717424U,  4597169786279785693U,
    13820541823134561501U,  4606957467106717424U,
    4602970680601913687U,  4605799732098147061U,
    13829171768952922869U,  4602970680601913687U,
    4605523422498301790U,  4603384207141321914U,
    13826756243996097722U,  4605523422498301790U,
    4595218635031890910U,  4607054494135176056U,
    13830426530989951864U,  4595218635031890910U,
    4607111255739239816U,  4593688012422887515U,
    13817060049277663323U,  4607111255739239816U,
    4603694922063032361U,  4605292980606880364U,
    13828665017461656172U,  4603694922063032361U,
    4605998608960791335U,  4602598930031891166U,
    13825970966886666974U,  4605998608960791335U,
    4598423001813699022U,  4606863472012527185U,
    13830235508867302993U,  4598423001813699022U,
    4606719100629313491U,  4599374859150636784U,
    13822746896005412592U,  4606719100629313491U,
    4601721693286060937U,  4606233055365547081U,
    13829605092220322889U,  4601721693286060937U,
    4604977468824438271U,  4604079374282302598U,
    13827451411137078406U,  4604977468824438271U,
    4589744810590291021U,  4607160003989618959U,
    13830532040844394767U,  4589744810590291021U,
    4607155938267770208U,  4590185751760970393U,
    13813557788615746201U,  4607155938267770208U,
    4604037525321326463U,  4605013567986435066U,
    13828385604841210874U,  4604037525321326463U,
    4606208206518262803U,  4601820425647934753U,
    13825192462502710561U,  4606208206518262803U,
    4599269903251194481U,  4606736437002195879U,
    13830108473856971687U,  4599269903251194481U,
    4606848731493011465U,  4598529532600161144U,
    13821901569454936952U,  4606848731493011465U,
    4602502755147763107U,  4606025850160239809U,
    13829397887015015617U,  4602502755147763107U,
    4605258978359093269U,  4603738491917026584U,
    13827110528771802392U,  4605258978359093269U,
    4593265590854265407U,  4607118021058468598U,
    13830490057913244406U,  4593265590854265407U,
    4607045045516813836U,  4595436449949385485U,
    13818808486804161293U,  4607045045516813836U,
    4603339021357904144U,  4605555245917486022U,
    13828927282772261830U,  4603339021357904144U,
    4605770164172969910U,  4603017373458244943U,
    13826389410313020751U,  4605770164172969910U,
    4596954088216812973U,  4606969576261663845U,
    13830341613116439653U,  4596954088216812973U,
    4606568886807728474U,  4600206446098256018U,
    13823578482953031826U,  4606568886807728474U,
    4600921238092511730U,  4606420848538580260U,
    13829792885393356068U,  4600921238092511730U,
    4604679572075463103U,  4604406033021674239U,
    13827778069876450047U,  4604679572075463103U,
    4581846703643734566U,  4607180341788068727U,
    13830552378642844535U,  4581846703643734566U,
    4607181359080094673U,  4579996072175835083U,
    13803368109030610891U,  4607181359080094673U,
    4604445825685214043U,  4604641218080103285U,
    13828013254934879093U,  4604445825685214043U,
    4606442934727379583U,  4600819913163773071U,
    13824191950018548879U,  4606442934727379583U,
    4600309328230211502U,  4606548680329491866U,
    13829920717184267674U,  4600309328230211502U,
    4606981354314050484U,  4596738097012783531U,
    13820110133867559339U,  4606981354314050484U,
    4603063884010218172U,  4605740310302420207U,
    13829112347157196015U,  4603063884010218172U,
    4605586791482848547U,  4603293641160266722U,
    13826665678015042530U,  4605586791482848547U,
    4595654028864046335U,  4607035262954517034U,
    13830407299809292842U,  4595654028864046335U,
    4607124449686274900U,  4592826452951465409U,
    13816198489806241217U,  4607124449686274900U,
    4603781852316960384U,  4605224709411790590U,
    13828596746266566398U,  4603781852316960384U,
    4606052795787882823U,  4602406247776385022U,
    13825778284631160830U,  4606052795787882823U,
    4598635880488956483U,  4606833664420673202U,
    13830205701275449010U,  4598635880488956483U,
    4606753451050079834U,  4599164736579548843U,
    13822536773434324651U,  4606753451050079834U,
    4601918851211878557U,  4606183055233559255U,
    13829555092088335063U,  4601918851211878557U,
    4605049409688478101U,  4603995455647851249U,
    13827367492502627057U,  4605049409688478101U,
    4590626485056654602U,  4607151534426937478U,
    13830523571281713286U,  4590626485056654602U,
    4607163731439411601U,  4589303678145802340U,
    13812675715000578148U,  4607163731439411601U,
    4604121000955189926U,  4604941113561600762U,
    13828313150416376570U,  4604121000955189926U,
    4606257600839867033U,  4601622657843474729U,
    13824994694698250537U,  4606257600839867033U,
    4599479600326345459U,  4606701442584137310U,
    13830073479438913118U,  4599479600326345459U,
    4606877885424248132U,  4598316292140394014U,
    13821688328995169822U,  4606877885424248132U,
    4602686793990243041U,  4605971073215153165U,
    13829343110069928973U,  4602686793990243041U,
    4605326714874986465U,  4603651144395358093U,
    13827023181250133901U,  4605326714874986465U,
    4593907249284540294U,  4607104153983298999U,
    13830476190838074807U,  4593907249284540294U,
    4607063608453868552U,  4595000592312171144U,
    13818372629166946952U,  4607063608453868552U,
    4603429196809300824U,  4605491322423429598U,
    13828863359278205406U,  4603429196809300824U,
    4605829012964735987U,  4602923807199184054U,
    13826295844053959862U,  4605829012964735987U,
    4597385183080791534U,  4606945027305114062U,
    13830317064159889870U,  4597385183080791534U,
    4606608350964852124U,  4599999947619525579U,
    13823371984474301387U,  4606608350964852124U,
    4601123065313358619U,  4606375745674388705U,
    13829747782529164513U,  4601123065313358619U,
    4604755543975806820U,  4604325745441780828U,
    13827697782296556636U,  4604755543975806820U,
    4585023436363055487U,  4607177290141793710U,
    13830549326996569518U,  4585023436363055487U,
    4607175255902437396U,  4585907115494236537U,
    13809279152349012345U,  4607175255902437396U,
    4604285253548209224U,  4604793159020491611U,
    13828165195875267419U,  4604285253548209224U,
    4606352730697093817U,  4601223560006786057U,
    13824595596861561865U,  4606352730697093817U,
    4599896339047301634U,  4606627607157935956U,
    13829999644012711764U,  4599896339047301634U,
    4606932257325205256U,  4597600270510262682U,
    13820972307365038490U,  4606932257325205256U,
    4602876755014813164U,  4605858005670328613U,
    13829230042525104421U,  4602876755014813164U,
    4605458946901419122U,  4603473988668005304U,
    13826846025522781112U,  4605458946901419122U,
    4594782329999411347U,  4607072388129742377U,
    13830444424984518185U,  4594782329999411347U,
    4607096716058023245U,  4594126307716900071U,
    13817498344571675879U,  4607096716058023245U,
    4603607160562208225U,  4605360179893335444U,
    13828732216748111252U,  4603607160562208225U,
    4605943243960030558U,  4602734543519989142U,
    13826106580374764950U,  4605943243960030558U,
    4598209407597805010U,  4606891971185517504U,
    13830264008040293312U,  4598209407597805010U,
    4606683463531482757U,  4599584122834874440U,
    13822956159689650248U,  4606683463531482757U,
    4601523323048804569U,  4606281842017099424U,
    13829653878871875232U,  4601523323048804569U,
    4604904503566677638U,  4604162403772767740U,
    13827534440627543548U,  4604904503566677638U,
    4588556721781247689U,  4607167120476811757U,
    13830539157331587565U,  4588556721781247689U,
    4607146792632922887U,  4591066993883984169U,
    13814439030738759977U,  4607146792632922887U,
    4603953166845776383U,  4605084992581147553U,
    13828457029435923361U,  4603953166845776383U,
    4606157602458368090U,  4602016966272225497U,
    13825389003127001305U,  4606157602458368090U,
    4599059363095165615U,  4606770142132396069U,
    13830142178987171877U,  4599059363095165615U,
    4606818271362779153U,  4598742041476147134U,
    13822114078330922942U,  4606818271362779153U,
    4602309411551204896U,  4606079444829232727U,
    13829451481684008535U,  4602309411551204896U,
    4605190175055178825U,  4603825001630339212U,
    13827197038485115020U,  4605190175055178825U,
    4592387007752762956U,  4607130541380624519U,
    13830502578235400327U,  4592387007752762956U,
    4607025146816593591U,  4595871363584150300U,
    13819243400438926108U,  4607025146816593591U,
    4603248068256948438U,  4605618058006716661U,
    13828990094861492469U,  4603248068256948438U,
    4605710171610479304U,  4603110210506737381U,
    13826482247361513189U,  4605710171610479304U,
    4596521820799644122U,  4606992800820440327U,
    13830364837675216135U,  4596521820799644122U,
    4606528158595189433U,  4600411960456200676U,
    13823783997310976484U,  4606528158595189433U,
    4600718319105833937U,  4606464709641375231U,
    13829836746496151039U,  4600718319105833937U,
    4604602620643553229U,  4604485382263976838U,
    13827857419118752646U,  4604602620643553229U,
    4576459225186735875U,  4607182037296057423U,
    13830554074150833231U,  4576459225186735875U,
    4607182037296057423U,  4576459225186735875U,
    13799831262041511683U,  4607182037296057423U,
    4604485382263976838U,  4604602620643553229U,
    13827974657498329037U,  4604485382263976838U,
    4606464709641375231U,  4600718319105833937U,
    13824090355960609745U,  4606464709641375231U,
    4600411960456200676U,  4606528158595189433U,
    13829900195449965241U,  4600411960456200676U,
    4606992800820440327U,  4596521820799644122U,
    13819893857654419930U,  4606992800820440327U,
    4603110210506737381U,  4605710171610479304U,
    13829082208465255112U,  4603110210506737381U,
    4605618058006716661U,  4603248068256948438U,
    13826620105111724246U,  4605618058006716661U,
    4595871363584150300U,  4607025146816593591U,
    13830397183671369399U,  4595871363584150300U,
    4607130541380624519U,  4592387007752762956U,
    13815759044607538764U,  4607130541380624519U,
    4603825001630339212U,  4605190175055178825U,
    13828562211909954633U,  4603825001630339212U,
    4606079444829232727U,  4602309411551204896U,
    13825681448405980704U,  4606079444829232727U,
    4598742041476147134U,  4606818271362779153U,
    13830190308217554961U,  4598742041476147134U,
    4606770142132396069U,  4599059363095165615U,
    13822431399949941423U,  4606770142132396069U,
    4602016966272225497U,  4606157602458368090U,
    13829529639313143898U,  4602016966272225497U,
    4605084992581147553U,  4603953166845776383U,
    13827325203700552191U,  4605084992581147553U,
    4591066993883984169U,  4607146792632922887U,
    13830518829487698695U,  4591066993883984169U,
    4607167120476811757U,  4588556721781247689U,
    13811928758636023497U,  4607167120476811757U,
    4604162403772767740U,  4604904503566677638U,
    13828276540421453446U,  4604162403772767740U,
    4606281842017099424U,  4601523323048804569U,
    13824895359903580377U,  4606281842017099424U,
    4599584122834874440U,  4606683463531482757U,
    13830055500386258565U,  4599584122834874440U,
    4606891971185517504U,  4598209407597805010U,
    13821581444452580818U,  4606891971185517504U,
    4602734543519989142U,  4605943243960030558U,
    13829315280814806366U,  4602734543519989142U,
    4605360179893335444U,  4603607160562208225U,
    13826979197416984033U,  4605360179893335444U,
    4594126307716900071U,  4607096716058023245U,
    13830468752912799053U,  4594126307716900071U,
    4607072388129742377U,  4594782329999411347U,
    13818154366854187155U,  4607072388129742377U,
    4603473988668005304U,  4605458946901419122U,
    13828830983756194930U,  4603473988668005304U,
    4605858005670328613U,  4602876755014813164U,
    13826248791869588972U,  4605858005670328613U,
    4597600270510262682U,  4606932257325205256U,
    13830304294179981064U,  4597600270510262682U,
    4606627607157935956U,  4599896339047301634U,
    13823268375902077442U,  4606627607157935956U,
    4601223560006786057U,  4606352730697093817U,
    13829724767551869625U,  4601223560006786057U,
    4604793159020491611U,  4604285253548209224U,
    13827657290402985032U,  4604793159020491611U,
    4585907115494236537U,  4607175255902437396U,
    13830547292757213204U,  4585907115494236537U,
    4607177290141793710U,  4585023436363055487U,
    13808395473217831295U,  4607177290141793710U,
    4604325745441780828U,  4604755543975806820U,
    13828127580830582628U,  4604325745441780828U,
    4606375745674388705U,  4601123065313358619U,
    13824495102168134427U,  4606375745674388705U,
    4599999947619525579U,  4606608350964852124U,
    13829980387819627932U,  4599999947619525579U,
    4606945027305114062U,  4597385183080791534U,
    13820757219935567342U,  4606945027305114062U,
    4602923807199184054U,  4605829012964735987U,
    13829201049819511795U,  4602923807199184054U,
    4605491322423429598U,  4603429196809300824U,
    13826801233664076632U,  4605491322423429598U,
    4595000592312171144U,  4607063608453868552U,
    13830435645308644360U,  4595000592312171144U,
    4607104153983298999U,  4593907249284540294U,
    13817279286139316102U,  4607104153983298999U,
    4603651144395358093U,  4605326714874986465U,
    13828698751729762273U,  4603651144395358093U,
    4605971073215153165U,  4602686793990243041U,
    13826058830845018849U,  4605971073215153165U,
    4598316292140394014U,  4606877885424248132U,
    13830249922279023940U,  4598316292140394014U,
    4606701442584137310U,  4599479600326345459U,
    13822851637181121267U,  4606701442584137310U,
    4601622657843474729U,  4606257600839867033U,
    13829629637694642841U,  4601622657843474729U,
    4604941113561600762U,  4604121000955189926U,
    13827493037809965734U,  4604941113561600762U,
    4589303678145802340U,  4607163731439411601U,
    13830535768294187409U,  4589303678145802340U,
    4607151534426937478U,  4590626485056654602U,
    13813998521911430410U,  4607151534426937478U,
    4603995455647851249U,  4605049409688478101U,
    13828421446543253909U,  4603995455647851249U,
    4606183055233559255U,  4601918851211878557U,
    13825290888066654365U,  4606183055233559255U,
    4599164736579548843U,  4606753451050079834U,
    13830125487904855642U,  4599164736579548843U,
    4606833664420673202U,  4598635880488956483U,
    13822007917343732291U,  4606833664420673202U,
    4602406247776385022U,  4606052795787882823U,
    13829424832642658631U,  4602406247776385022U,
    4605224709411790590U,  4603781852316960384U,
    13827153889171736192U,  4605224709411790590U,
    4592826452951465409U,  4607124449686274900U,
    13830496486541050708U,  4592826452951465409U,
    4607035262954517034U,  4595654028864046335U,
    13819026065718822143U,  4607035262954517034U,
    4603293641160266722U,  4605586791482848547U,
    13828958828337624355U,  4603293641160266722U,
    4605740310302420207U,  4603063884010218172U,
    13826435920864993980U,  4605740310302420207U,
    4596738097012783531U,  4606981354314050484U,
    13830353391168826292U,  4596738097012783531U,
    4606548680329491866U,  4600309328230211502U,
    13823681365084987310U,  4606548680329491866U,
    4600819913163773071U,  4606442934727379583U,
    13829814971582155391U,  4600819913163773071U,
    4604641218080103285U,  4604445825685214043U,
    13827817862539989851U,  4604641218080103285U,
    4579996072175835083U,  4607181359080094673U,
    13830553395934870481U,  4579996072175835083U,
    4607180341788068727U,  4581846703643734566U,
    13805218740498510374U,  4607180341788068727U,
    4604406033021674239U,  4604679572075463103U,
    13828051608930238911U,  4604406033021674239U,
    4606420848538580260U,  4600921238092511730U,
    13824293274947287538U,  4606420848538580260U,
    4600206446098256018U,  4606568886807728474U,
    13829940923662504282U,  4600206446098256018U,
    4606969576261663845U,  4596954088216812973U,
    13820326125071588781U,  4606969576261663845U,
    4603017373458244943U,  4605770164172969910U,
    13829142201027745718U,  4603017373458244943U,
    4605555245917486022U,  4603339021357904144U,
    13826711058212679952U,  4605555245917486022U,
    4595436449949385485U,  4607045045516813836U,
    13830417082371589644U,  4595436449949385485U,
    4607118021058468598U,  4593265590854265407U,
    13816637627709041215U,  4607118021058468598U,
    4603738491917026584U,  4605258978359093269U,
    13828631015213869077U,  4603738491917026584U,
    4606025850160239809U,  4602502755147763107U,
    13825874792002538915U,  4606025850160239809U,
    4598529532600161144U,  4606848731493011465U,
    13830220768347787273U,  4598529532600161144U,
    4606736437002195879U,  4599269903251194481U,
    13822641940105970289U,  4606736437002195879U,
    4601820425647934753U,  4606208206518262803U,
    13829580243373038611U,  4601820425647934753U,
    4605013567986435066U,  4604037525321326463U,
    13827409562176102271U,  4605013567986435066U,
    4590185751760970393U,  4607155938267770208U,
    13830527975122546016U,  4590185751760970393U,
    4607160003989618959U,  4589744810590291021U,
    13813116847445066829U,  4607160003989618959U,
    4604079374282302598U,  4604977468824438271U,
    13828349505679214079U,  4604079374282302598U,
    4606233055365547081U,  4601721693286060937U,
    13825093730140836745U,  4606233055365547081U,
    4599374859150636784U,  4606719100629313491U,
    13830091137484089299U,  4599374859150636784U,
    4606863472012527185U,  4598423001813699022U,
    13821795038668474830U,  4606863472012527185U,
    4602598930031891166U,  4605998608960791335U,
    13829370645815567143U,  4602598930031891166U,
    4605292980606880364U,  4603694922063032361U,
    13827066958917808169U,  4605292980606880364U,
    4593688012422887515U,  4607111255739239816U,
    13830483292594015624U,  4593688012422887515U,
    4607054494135176056U,  4595218635031890910U,
    13818590671886666718U,  4607054494135176056U,
    4603384207141321914U,  4605523422498301790U,
    13828895459353077598U,  4603384207141321914U,
    4605799732098147061U,  4602970680601913687U,
    13826342717456689495U,  4605799732098147061U,
    4597169786279785693U,  4606957467106717424U,
    13830329503961493232U,  4597169786279785693U,
    4606588777269136769U,  4600103317933788342U,
    13823475354788564150U,  4606588777269136769U,
    4601022290077223616U,  4606398451906509788U,
    13829770488761285596U,  4601022290077223616U,
    4604717681185626434U,  4604366005771528720U,
    13827738042626304528U,  4604717681185626434U,
    4583614727651146525U,  4607178985458280057U,
    13830551022313055865U,  4583614727651146525U,
    4607172882816799076U,  4586790578280679046U,
    13810162615135454854U,  4607172882816799076U,
    4604244531615310815U,  4604830524903495634U,
    13828202561758271442U,  4604244531615310815U,
    4606329407841126011U,  4601323770373937522U,
    13824695807228713330U,  4606329407841126011U,
    4599792496117920694U,  4606646545123403481U,
    13830018581978179289U,  4599792496117920694U,
    4606919157647773535U,  4597815040470278984U,
    13821187077325054792U,  4606919157647773535U,
    4602829525820289164U,  4605886709123365959U,
    13829258745978141767U,  4602829525820289164U,
    4605426297151190466U,  4603518581031047189U,
    13826890617885822997U,  4605426297151190466U,
    4594563856311064231U,  4607080832832247697U,
    13830452869687023505U,  4594563856311064231U,
    4607088942243446236U,  4594345179472540681U,
    13817717216327316489U,  4607088942243446236U,
    4603562972219549215U,  4605393374401988274U,
    13828765411256764082U,  4603562972219549215U,
    4605915122243179241U,  4602782121393764535U,
    13826154158248540343U,  4605915122243179241U,
    4598029484874872834U,  4606905728766014348U,
    13830277765620790156U,  4598029484874872834U,
    4606665164148251002U,  4599688422741010356U,
    13823060459595786164U,  4606665164148251002U,
    4601423692641949331U,  4606305777984577632U,
    13829677814839353440U,  4601423692641949331U,
    4604867640218014515U,  4604203581176243359U,
    13827575618031019167U,  4604867640218014515U,
    4587673791460508439U,  4607170170974224083U,
    13830542207828999891U,  4587673791460508439U,
    4607141713064252300U,  4591507261658050721U,
    13814879298512826529U,  4607141713064252300U,
    4603910660507251362U,  4605120315324767624U,
    13828492352179543432U,  4603910660507251362U,
    4606131849150971908U,  4602114767134999006U,
    13825486803989774814U,  4606131849150971908U,
    4598953786765296928U,  4606786509620734768U,
    13830158546475510576U,  4598953786765296928U,
    4606802552898869248U,  4598848011564831930U,
    13822220048419607738U,  4606802552898869248U,
    4602212250118051877U,  4606105796280968177U,
    13829477833135743985U,  4602212250118051877U,
    4605155376589456981U,  4603867938232615808U,
    13827239975087391616U,  4605155376589456981U,
    4591947271803021404U,  4607136295912168606U,
    13830508332766944414U,  4591947271803021404U,
    4607014697483910382U,  4596088445927168004U,
    13819460482781943812U,  4607014697483910382U,
    4603202304363743346U,  4605649044311923410U,
    13829021081166699218U,  4603202304363743346U,
    4605679749231851918U,  4603156351203636159U,
    13826528388058411967U,  4605679749231851918U,
    4596305267720071930U,  4607003915349878877U,
    13830375952204654685U,  4596305267720071930U,
    4606507322377452870U,  4600514338912178239U,
    13823886375766954047U,  4606507322377452870U,
    4600616459743653188U,  4606486172460753999U,
    13829858209315529807U,  4600616459743653188U,
    4604563781218984604U,  4604524701268679793U,
    13827896738123455601U,  4604563781218984604U,
    4569220649180767418U,  4607182376410422530U,
    13830554413265198338U,  4569220649180767418U
};

const fpr fpr_p2_tab[] = {
    4611686018427387904U,
    4607182418800017408U,
    4602678819172646912U,
    4598175219545276416U,
    4593671619917905920U,
    4589168020290535424U,
    4584664420663164928U,
    4580160821035794432U,
    4575657221408423936U,
    4571153621781053440U,
    4566650022153682944U
};
