/*
 * This code is derived from discussions with Thomas Pornin
 */

#include "gf5248.h"

// see gf5248.h
const gf5248 ZERO = { 0, 0, 0, 0 };

// see gf5248.h
const gf5248 ONE = { 0x0000000000000033, 0x0000000000000000, 0x0000000000000000, 0x0100000000000000 };

// see gf5248.h
const gf5248 gf5248_MINUS_ONE = { 0xFFFFFFFFFFFFFFCC, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x03FFFFFFFFFFFFFF };

// Montgomery representation of 2^256.
static const gf5248 R2 = { 0x3333333333333d70, 0x3333333333333333, 0x3333333333333333, 0x0333333333333333 };

// The modulus itself (this is also a valid representation of zero).
static const gf5248 MODULUS = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x04FFFFFFFFFFFFFF };

// 1/2^244 (in Montgomery representation).
static const gf5248 INVT244 = { 0x0000000000001000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 };

static const gf5248 PM1O3 = { 0xaaaaaaaaaaaaaaaa, 0xaaaaaaaaaaaaaaaa, 0xaaaaaaaaaaaaaaaa, 0x01aaaaaaaaaaaaaa };

// Normalize value *a into *d.
static inline void
inner_gf5248_normalize(gf5248 *d, const gf5248 *a)
{
    uint64_t d0, d1, d2, d3, m;
    unsigned char cc;

    // Subtract q.
    cc = inner_gf5248_sbb(0, a->v0, 0xFFFFFFFFFFFFFFFF, &d0);
    cc = inner_gf5248_sbb(cc, a->v1, 0xFFFFFFFFFFFFFFFF, &d1);
    cc = inner_gf5248_sbb(cc, a->v2, 0xFFFFFFFFFFFFFFFF, &d2);
    cc = inner_gf5248_sbb(cc, a->v3, 0x04FFFFFFFFFFFFFF, &d3);

    // Add back q if the result is negative.
    (void)inner_gf5248_sbb(cc, 0, 0, &m);
    cc = inner_gf5248_adc(0, d0, m, &d0);
    cc = inner_gf5248_adc(cc, d1, m, &d1);
    cc = inner_gf5248_adc(cc, d2, m, &d2);
    (void)inner_gf5248_adc(cc, d3, m & 0x04FFFFFFFFFFFFFF, &d3);

    d->v0 = d0;
    d->v1 = d1;
    d->v2 = d2;
    d->v3 = d3;
}

// Expand the most significant bit of x into a full-width 64-bit word
// (0x0000000000000000 or 0xFFFFFFFFFFFFFFFF).
static inline uint64_t
sgnw(uint64_t x)
{
    return (uint64_t)(*(int64_t *)&x >> 63);
}

// d <- u*f + v*g  (in the field)
// Coefficients f and g are provided as unsigned integers, but they
// really are signed values which must be less than 2^62 (in absolute value).
static void
gf5248_lin(gf5248 *d, const gf5248 *u, const gf5248 *v, uint64_t f, uint64_t g)
{
    // f <- abs(f), keeping the sign in sf, and negating u accordingly
    uint64_t sf = sgnw(f);
    f = (f ^ sf) - sf;
    gf5248 tu;
    gf5248_neg(&tu, u);
    gf5248_select(&tu, u, &tu, (uint32_t)sf);

    // g <- abs(g), keeping the sign in sg, and negating v accordingly
    uint64_t sg = sgnw(g);
    g = (g ^ sg) - sg;
    gf5248 tv;
    gf5248_neg(&tv, v);
    gf5248_select(&tv, v, &tv, (uint32_t)sg);

    // Linear combination over plain integers.
    uint64_t d0, d1, d2, d3, t;
    inner_gf5248_umul_x2(d0, t, tu.v0, f, tv.v0, g);
    inner_gf5248_umul_x2_add(d1, t, tu.v1, f, tv.v1, g, t);
    inner_gf5248_umul_x2_add(d2, t, tu.v2, f, tv.v2, g, t);
    inner_gf5248_umul_x2_add(d3, t, tu.v3, f, tv.v3, g, t);

    // Reduction: split into low part (248 bits) and high part
    // (71 bits, since t can be up to 63 bits). If the high
    // part is h, then:
    //    h*2^248 = (h mod 5)*2^248 + floor(h/5)  mod q
    uint64_t h0 = (d3 >> 56) | (t << 8);
    uint64_t h1 = t >> 56;
    d3 &= 0x00FFFFFFFFFFFFFF;
    uint64_t z0, z1, quo0, rem0, quo1, rem1;
    inner_gf5248_umul(z0, z1, h0, 0xCCCCCCCCCCCCCCCD);
    (void)z0;
    quo0 = z1 >> 2;
    rem0 = h0 - (5 * quo0);
    quo1 = (h1 * 0xCD) >> 10;
    rem1 = h1 - (5 * quo1);

    // h = rem0 + 5*quo0 + (rem1 + 5*quo1)*2^64
    //   = rem0 + rem1 + 5*(quo0 + quo1*2^64 + rem1*((2^64 - 1)/5))
    // We add rem0 and rem1 modulo 5, with an extra carry that
    // goes into the folded part (multiple of 5).
    uint64_t e, f0, f1;
    unsigned char cc;
    cc = inner_gf5248_adc(0, rem0 + 0xFFFFFFFFFFFFFFFA, rem1, &e);
    cc = inner_gf5248_adc(cc, quo0, rem1 * 0x3333333333333333, &f0);
    (void)inner_gf5248_adc(cc, quo1, 0, &f1);
    e -= 0xFFFFFFFFFFFFFFFA;

    // Now we only have to add e*2^248 + f0:f1 to the low part.
    cc = inner_gf5248_adc(0, d0, f0, &d0);
    cc = inner_gf5248_adc(cc, d1, f1, &d1);
    cc = inner_gf5248_adc(cc, d2, 0, &d2);
    (void)inner_gf5248_adc(cc, d3, e << 56, &d3);

    d->v0 = d0;
    d->v1 = d1;
    d->v2 = d2;
    d->v3 = d3;
}

// d <- abs(floor((a*f + b*g) / 2^31))
// Coefficients f and g are provided as unsigned integer, but they really
// are signed values, which MUST be at most 2^31 in absolute value.
// The computation is performed over the integers, not modulo q. The low
// 31 bits are dropped (in practice, callers provided appropriate coefficients
// f and g such that a*f + b*g is a multiple of 2^31.
//
// If a*f + b*g is negative, then the absolute value is computed, and the
// function returns 0xFFFFFFFFFFFFFFFF; otherwise, the function returns
// 0x0000000000000000.
static uint64_t
lindiv31abs(gf5248 *d, const gf5248 *a, const gf5248 *b, uint64_t f, uint64_t g)
{
    // f <- abs(f), keeping the sign in sf
    uint64_t sf = sgnw(f);
    f = (f ^ sf) - sf;

    // g <- abs(g), keeping the sign in sg
    uint64_t sg = sgnw(g);
    g = (g ^ sg) - sg;

    // Apply the signs of f and g to the source operands.
    uint64_t a0, a1, a2, a3, a4;
    uint64_t b0, b1, b2, b3, b4;
    unsigned char cc;

    cc = inner_gf5248_sbb(0, a->v0 ^ sf, sf, &a0);
    cc = inner_gf5248_sbb(cc, a->v1 ^ sf, sf, &a1);
    cc = inner_gf5248_sbb(cc, a->v2 ^ sf, sf, &a2);
    cc = inner_gf5248_sbb(cc, a->v3 ^ sf, sf, &a3);
    (void)inner_gf5248_sbb(cc, 0, 0, &a4);

    cc = inner_gf5248_sbb(0, b->v0 ^ sg, sg, &b0);
    cc = inner_gf5248_sbb(cc, b->v1 ^ sg, sg, &b1);
    cc = inner_gf5248_sbb(cc, b->v2 ^ sg, sg, &b2);
    cc = inner_gf5248_sbb(cc, b->v3 ^ sg, sg, &b3);
    (void)inner_gf5248_sbb(cc, 0, 0, &b4);

    // Compute a*f + b*g into d0:d1:d2:d3:d4. Since f and g are at
    // most 2^31, we can add two 128-bit products with no overflow.
    // Note: a4 and b4 are both in {0, -1}.
    uint64_t d0, d1, d2, d3, d4, t;
    inner_gf5248_umul_x2(d0, t, a0, f, b0, g);
    inner_gf5248_umul_x2_add(d1, t, a1, f, b1, g, t);
    inner_gf5248_umul_x2_add(d2, t, a2, f, b2, g, t);
    inner_gf5248_umul_x2_add(d3, t, a3, f, b3, g, t);
    d4 = t - (a4 & f) - (b4 & g);

    // Right-shift the value by 31 bits.
    d0 = (d0 >> 31) | (d1 << 33);
    d1 = (d1 >> 31) | (d2 << 33);
    d2 = (d2 >> 31) | (d3 << 33);
    d3 = (d3 >> 31) | (d4 << 33);

    // If the result is negative, negate it.
    t = sgnw(d4);
    cc = inner_gf5248_sbb(0, d0 ^ t, t, &d0);
    cc = inner_gf5248_sbb(cc, d1 ^ t, t, &d1);
    cc = inner_gf5248_sbb(cc, d2 ^ t, t, &d2);
    (void)inner_gf5248_sbb(cc, d3 ^ t, t, &d3);

    d->v0 = d0;
    d->v1 = d1;
    d->v2 = d2;
    d->v3 = d3;
    return t;
}

// lzcnt(x) returns the number of leading bits of value 0 in x. It supports
// x == 0 (in which case the function returns 64).
#if defined __LZCNT__
static inline uint64_t
lzcnt(uint64_t x)
{
    return _lzcnt_u64(x);
}
#else
static inline uint64_t
lzcnt(uint64_t x)
{
    uint64_t m, s;
    m = sgnw((x >> 32) - 1);
    s = m & 32;
    x = (x >> 32) ^ (m & (x ^ (x >> 32)));
    m = sgnw((x >> 16) - 1);
    s |= m & 16;
    x = (x >> 16) ^ (m & (x ^ (x >> 16)));
    m = sgnw((x >> 8) - 1);
    s |= m & 8;
    x = (x >> 8) ^ (m & (x ^ (x >> 8)));
    m = sgnw((x >> 4) - 1);
    s |= m & 4;
    x = (x >> 4) ^ (m & (x ^ (x >> 4)));
    m = sgnw((x >> 2) - 1);
    s |= m & 2;
    x = (x >> 2) ^ (m & (x ^ (x >> 2)));

    // At this point, x fits on 2 bits. Count of extra zeros:
    //   x = 0  -> 2
    //   x = 1  -> 1
    //   x = 2  -> 0
    //   x = 3  -> 0
    s += (2 - x) & ((x - 3) >> 2);
    return s;
}
#endif

// see gf5248.h
uint32_t
gf5248_div(gf5248 *d, const gf5248 *x, const gf5248 *y)
{
    // Extended binary GCD:
    //
    //   a <- y
    //   b <- q (modulus)
    //   u <- x (self)
    //   v <- 0
    //
    // Value a is normalized (in the 0..q-1 range). Values a and b are
    // then considered as (signed) integers. Values u and v are field
    // elements.
    //
    // Invariants:
    //    a*x = y*u mod q
    //    b*x = y*v mod q
    //    b is always odd
    //
    // At each step:
    //    if a is even, then:
    //        a <- a/2, u <- u/2 mod q
    //    else:
    //        if a < b:
    //            (a, u, b, v) <- (b, v, a, u)
    //        a <- (a-b)/2, u <- (u-v)/2 mod q
    //
    // What we implement below is the optimized version of this
    // algorithm, as described in https://eprint.iacr.org/2020/972

    gf5248 a, b, u, v;
    uint64_t xa, xb, f0, g0, f1, g1;
    uint32_t r;

    r = ~gf5248_iszero(y);
    inner_gf5248_normalize(&a, y);
    b = MODULUS;
    u = *x;
    v = ZERO;

    // Generic loop does 15*31 = 465 inner iterations.
    for (int i = 0; i < 15; i++) {
        // Get approximations of a and b over 64 bits:
        //  - If len(a) <= 64 and len(b) <= 64, then we just use
        //    their values (low limbs).
        //  - Otherwise, with n = max(len(a), len(b)), we use:
        //       (a mod 2^31) + 2^31*floor(a / 2^(n - 33))
        //       (b mod 2^31) + 2^31*floor(b / 2^(n - 33))
        uint64_t m3 = a.v3 | b.v3;
        uint64_t m2 = a.v2 | b.v2;
        uint64_t m1 = a.v1 | b.v1;
        uint64_t tnz3 = sgnw(m3 | -m3);
        uint64_t tnz2 = sgnw(m2 | -m2) & ~tnz3;
        uint64_t tnz1 = sgnw(m1 | -m1) & ~tnz3 & ~tnz2;
        uint64_t tnzm = (m3 & tnz3) | (m2 & tnz2) | (m1 & tnz1);
        uint64_t tnza = (a.v3 & tnz3) | (a.v2 & tnz2) | (a.v1 & tnz1);
        uint64_t tnzb = (b.v3 & tnz3) | (b.v2 & tnz2) | (b.v1 & tnz1);
        uint64_t snza = (a.v2 & tnz3) | (a.v1 & tnz2) | (a.v0 & tnz1);
        uint64_t snzb = (b.v2 & tnz3) | (b.v1 & tnz2) | (b.v0 & tnz1);

        // If both len(a) <= 64 and len(b) <= 64, then:
        //    tnzm = 0
        //    tnza = 0, snza = 0, tnzb = 0, snzb = 0
        // Otherwise:
        //    tnzm != 0
        //    tnza contains the top non-zero limb of a
        //    snza contains the limb right below tnza
        //    tnzb contains the top non-zero limb of a
        //    snzb contains the limb right below tnzb
        //
        // We count the number of leading zero bits in tnzm:
        //  - If s <= 31, then the top 31 bits can be extracted from
        //    tnza and tnzb alone.
        //  - If 32 <= s <= 63, then we need some bits from snza and
        //    snzb as well.
        int64_t s = lzcnt(tnzm);
        uint64_t sm = (uint64_t)((31 - s) >> 63);
        tnza ^= sm & (tnza ^ ((tnza << 32) | (snza >> 32)));
        tnzb ^= sm & (tnzb ^ ((tnzb << 32) | (snzb >> 32)));
        s -= 32 & sm;
        tnza <<= s;
        tnzb <<= s;

        // At this point:
        //  - If len(a) <= 64 and len(b) <= 64, then:
        //       tnza = 0
        //       tnzb = 0
        //       tnz1 = tnz2 = tnz3 = 0
        //       we want to use the entire low words of a and b
        //  - Otherwise, we want to use the top 33 bits of tnza and
        //    tnzb, and the low 31 bits of the low words of a and b.
        uint64_t tzx = ~(tnz1 | tnz2 | tnz3);
        tnza |= a.v0 & tzx;
        tnzb |= b.v0 & tzx;
        xa = (a.v0 & 0x7FFFFFFF) | (tnza & 0xFFFFFFFF80000000);
        xb = (b.v0 & 0x7FFFFFFF) | (tnzb & 0xFFFFFFFF80000000);

        // Compute the 31 inner iterations on xa and xb.
        uint64_t fg0 = (uint64_t)1;
        uint64_t fg1 = (uint64_t)1 << 32;
        for (int j = 0; j < 31; j++) {
            uint64_t a_odd, swap, t0, t1, t2;
            unsigned char cc;
            a_odd = -(xa & 1);
            cc = inner_gf5248_sbb(0, xa, xb, &t0);
            (void)inner_gf5248_sbb(cc, 0, 0, &swap);
            swap &= a_odd;
            t1 = swap & (xa ^ xb);
            xa ^= t1;
            xb ^= t1;
            t2 = swap & (fg0 ^ fg1);
            fg0 ^= t2;
            fg1 ^= t2;
            xa -= a_odd & xb;
            fg0 -= a_odd & fg1;
            xa >>= 1;
            fg1 <<= 1;
        }
        fg0 += 0x7FFFFFFF7FFFFFFF;
        fg1 += 0x7FFFFFFF7FFFFFFF;
        f0 = (fg0 & 0xFFFFFFFF) - (uint64_t)0x7FFFFFFF;
        g0 = (fg0 >> 32) - (uint64_t)0x7FFFFFFF;
        f1 = (fg1 & 0xFFFFFFFF) - (uint64_t)0x7FFFFFFF;
        g1 = (fg1 >> 32) - (uint64_t)0x7FFFFFFF;

        // Propagate updates to a, b, u and v.
        gf5248 na, nb, nu, nv;
        uint64_t nega = lindiv31abs(&na, &a, &b, f0, g0);
        uint64_t negb = lindiv31abs(&nb, &a, &b, f1, g1);
        f0 = (f0 ^ nega) - nega;
        g0 = (g0 ^ nega) - nega;
        f1 = (f1 ^ negb) - negb;
        g1 = (g1 ^ negb) - negb;
        gf5248_lin(&nu, &u, &v, f0, g0);
        gf5248_lin(&nv, &u, &v, f1, g1);
        a = na;
        b = nb;
        u = nu;
        v = nv;
    }

    // If y is invertible, then the final GCD is 1, and
    // len(a) + len(b) <= 37, so we can end the computation with
    // the low words directly. We only need 35 iterations to reach
    // the point where b = 1.
    //
    // If y is zero, then v is unchanged (hence zero) and none of
    // the subsequent iterations will change it either, so we get
    // 0 on output, which is what we want.
    xa = a.v0;
    xb = b.v0;
    f0 = 1;
    g0 = 0;
    f1 = 0;
    g1 = 1;
    for (int j = 0; j < 35; j++) {
        uint64_t a_odd, swap, t0, t1, t2, t3;
        unsigned char cc;
        a_odd = -(xa & 1);
        cc = inner_gf5248_sbb(0, xa, xb, &t0);
        (void)inner_gf5248_sbb(cc, 0, 0, &swap);
        swap &= a_odd;
        t1 = swap & (xa ^ xb);
        xa ^= t1;
        xb ^= t1;
        t2 = swap & (f0 ^ f1);
        f0 ^= t2;
        f1 ^= t2;
        t3 = swap & (g0 ^ g1);
        g0 ^= t3;
        g1 ^= t3;
        xa -= a_odd & xb;
        f0 -= a_odd & f1;
        g0 -= a_odd & g1;
        xa >>= 1;
        f1 <<= 1;
        g1 <<= 1;
    }
    gf5248_lin(d, &u, &v, f1, g1);

    // At the point:
    //  - Numerator and denominator were both in Montgomery representation,
    //    but the two factors R canceled each other.
    //  - We have injected 31*15+35 = 500 extra factors of 2, hence we
    //    must divide the result by 2^500.
    //  - However, we also want to obtain the result in Montgomery
    //    representation, i.e. multiply by 2^256. We thus want to
    //    divide the current result by 2^(500 - 256) = 2^244.
    //  - We do this division by using a Montgomery multiplication with
    //    the Montgomery representation of 1/2^244, i.e. the integer
    //    2^256/2^244 = 4096.
    gf5248_mul(d, d, &INVT244);
    return r;
}

// see gf5248.h
uint32_t
gf5248_invert(gf5248 *d, const gf5248 *a)
{
    return gf5248_div(d, &ONE, a);
}

// see gf5248.h
int32_t
gf5248_legendre(const gf5248 *x)
{
    // Same algorithm as the binary GCD in gf5248_div(), with
    // a few differences:
    //   - We do not keep track of the Bézout coefficients u and v.
    //   - In each inner iteration we adjust the running symbol value,
    //     which uses the low 3 bits of the values.
    //   - Since we need two extra bits of look-ahead, we can only run
    //     29 inner iterations, and then need an extra recomputation
    //     for the last 2.

    gf5248 a, b;
    uint64_t xa, xb, f0, g0, f1, g1, ls;

    inner_gf5248_normalize(&a, x);
    b = MODULUS;
    ls = 0; // running symbol information in bit 1.

    // Outer loop
    for (int i = 0; i < 15; i++) {
        // Get approximations of a and b over 64 bits.
        uint64_t m3 = a.v3 | b.v3;
        uint64_t m2 = a.v2 | b.v2;
        uint64_t m1 = a.v1 | b.v1;
        uint64_t tnz3 = sgnw(m3 | -m3);
        uint64_t tnz2 = sgnw(m2 | -m2) & ~tnz3;
        uint64_t tnz1 = sgnw(m1 | -m1) & ~tnz3 & ~tnz2;
        uint64_t tnzm = (m3 & tnz3) | (m2 & tnz2) | (m1 & tnz1);
        uint64_t tnza = (a.v3 & tnz3) | (a.v2 & tnz2) | (a.v1 & tnz1);
        uint64_t tnzb = (b.v3 & tnz3) | (b.v2 & tnz2) | (b.v1 & tnz1);
        uint64_t snza = (a.v2 & tnz3) | (a.v1 & tnz2) | (a.v0 & tnz1);
        uint64_t snzb = (b.v2 & tnz3) | (b.v1 & tnz2) | (b.v0 & tnz1);

        int64_t s = lzcnt(tnzm);
        uint64_t sm = (uint64_t)((31 - s) >> 63);
        tnza ^= sm & (tnza ^ ((tnza << 32) | (snza >> 32)));
        tnzb ^= sm & (tnzb ^ ((tnzb << 32) | (snzb >> 32)));
        s -= 32 & sm;
        tnza <<= s;
        tnzb <<= s;

        uint64_t tzx = ~(tnz1 | tnz2 | tnz3);
        tnza |= a.v0 & tzx;
        tnzb |= b.v0 & tzx;
        xa = (a.v0 & 0x7FFFFFFF) | (tnza & 0xFFFFFFFF80000000);
        xb = (b.v0 & 0x7FFFFFFF) | (tnzb & 0xFFFFFFFF80000000);

        // First 290 inner iterations.
        uint64_t fg0 = (uint64_t)1;
        uint64_t fg1 = (uint64_t)1 << 32;
        for (int j = 0; j < 29; j++) {
            uint64_t a_odd, swap, t0, t1, t2;
            unsigned char cc;
            a_odd = -(xa & 1);
            cc = inner_gf5248_sbb(0, xa, xb, &t0);
            (void)inner_gf5248_sbb(cc, 0, 0, &swap);
            swap &= a_odd;
            ls ^= swap & xa & xb;
            t1 = swap & (xa ^ xb);
            xa ^= t1;
            xb ^= t1;
            t2 = swap & (fg0 ^ fg1);
            fg0 ^= t2;
            fg1 ^= t2;
            xa -= a_odd & xb;
            fg0 -= a_odd & fg1;
            xa >>= 1;
            fg1 <<= 1;
            ls ^= (xb + 2) >> 1;
        }

        // Compute the updated a and b (low words only) to get
        // enough bits for the next two iterations.
        uint64_t fg0z = fg0 + 0x7FFFFFFF7FFFFFFF;
        uint64_t fg1z = fg1 + 0x7FFFFFFF7FFFFFFF;
        f0 = (fg0z & 0xFFFFFFFF) - (uint64_t)0x7FFFFFFF;
        g0 = (fg0z >> 32) - (uint64_t)0x7FFFFFFF;
        f1 = (fg1z & 0xFFFFFFFF) - (uint64_t)0x7FFFFFFF;
        g1 = (fg1z >> 32) - (uint64_t)0x7FFFFFFF;
        uint64_t a0 = (a.v0 * f0 + b.v0 * g0) >> 29;
        uint64_t b0 = (a.v0 * f1 + b.v0 * g1) >> 29;
        for (int j = 0; j < 2; j++) {
            uint64_t a_odd, swap, t0, t1, t2, t3;
            unsigned char cc;
            a_odd = -(xa & 1);
            cc = inner_gf5248_sbb(0, xa, xb, &t0);
            (void)inner_gf5248_sbb(cc, 0, 0, &swap);
            swap &= a_odd;
            ls ^= swap & a0 & b0;
            t1 = swap & (xa ^ xb);
            xa ^= t1;
            xb ^= t1;
            t2 = swap & (fg0 ^ fg1);
            fg0 ^= t2;
            fg1 ^= t2;
            t3 = swap & (a0 ^ b0);
            a0 ^= t3;
            b0 ^= t3;
            xa -= a_odd & xb;
            fg0 -= a_odd & fg1;
            a0 -= a_odd & b0;
            xa >>= 1;
            fg1 <<= 1;
            a0 >>= 1;
            ls ^= (b0 + 2) >> 1;
        }

        // Propagate updates to a and b.
        fg0 += 0x7FFFFFFF7FFFFFFF;
        fg1 += 0x7FFFFFFF7FFFFFFF;
        f0 = (fg0 & 0xFFFFFFFF) - (uint64_t)0x7FFFFFFF;
        g0 = (fg0 >> 32) - (uint64_t)0x7FFFFFFF;
        f1 = (fg1 & 0xFFFFFFFF) - (uint64_t)0x7FFFFFFF;
        g1 = (fg1 >> 32) - (uint64_t)0x7FFFFFFF;
        gf5248 na, nb;
        uint64_t nega = lindiv31abs(&na, &a, &b, f0, g0);
        (void)lindiv31abs(&nb, &a, &b, f1, g1);
        ls ^= nega & nb.v0;
        a = na;
        b = nb;
    }

    // Final iterations: values are at most 37 bits now. We do not
    // need to keep track of update coefficients. Just like the GCD,
    // we need only 35 iterations, because after 35 iterations,
    // value a is 0 or 1, and b is 1, and no further modification to
    // the Legendre symbol may happen.
    xa = a.v0;
    xb = b.v0;
    for (int j = 0; j < 35; j++) {
        uint64_t a_odd, swap, t0, t1;
        unsigned char cc;
        a_odd = -(xa & 1);
        cc = inner_gf5248_sbb(0, xa, xb, &t0);
        (void)inner_gf5248_sbb(cc, 0, 0, &swap);
        swap &= a_odd;
        ls ^= swap & xa & xb;
        t1 = swap & (xa ^ xb);
        xa ^= t1;
        xb ^= t1;
        xa -= a_odd & xb;
        xa >>= 1;
        ls ^= (xb + 2) >> 1;
    }

    // At this point, if the source value was not zero, then the low
    // bit of ls contains the QR status (0 = square, 1 = non-square),
    // which we need to convert to the expected value (+1 or -1).
    // If y == 0, then we return 0, per the API.
    uint32_t r = 1 - ((uint32_t)ls & 2);
    r &= ~gf5248_iszero(x);
    return *(int32_t *)&r;
}

// see gf5248.h
uint32_t
gf5248_sqrt(gf5248 *d, const gf5248 *a)
{
    // Candidate root is a^((q+1)/4), with (q+1)/4 = 5*2^246
    gf5248 y;
    gf5248_xsquare(&y, a, 2);
    gf5248_mul(&y, &y, a);
    gf5248_xsquare(&y, &y, 246);

    // Normalize y and negate if necessary, to set the low bit to 0.
    // The low bit check must be on the normal representation,
    // not the Montgomery representation.
    gf5248 yn;
    inner_gf5248_montgomery_reduce(&yn, &y);
    uint32_t ctl = -((uint32_t)yn.v0 & 1);
    gf5248_neg(&yn, &y);
    gf5248_select(&y, &y, &yn, ctl);

    // Check whether the candidate is indeed a square root.
    gf5248_square(&yn, &y);
    uint32_t r = gf5248_equals(&yn, a);
    *d = y;
    return r;
}

// Little-endian encoding of a 64-bit integer.
static inline void
enc64le(void *dst, uint64_t x)
{
    uint8_t *buf = dst;
    buf[0] = (uint8_t)x;
    buf[1] = (uint8_t)(x >> 8);
    buf[2] = (uint8_t)(x >> 16);
    buf[3] = (uint8_t)(x >> 24);
    buf[4] = (uint8_t)(x >> 32);
    buf[5] = (uint8_t)(x >> 40);
    buf[6] = (uint8_t)(x >> 48);
    buf[7] = (uint8_t)(x >> 56);
}

// Little-endian decoding of a 64-bit integer.
static inline uint64_t
dec64le(const void *src)
{
    const uint8_t *buf = src;
    return (uint64_t)buf[0] | ((uint64_t)buf[1] << 8) | ((uint64_t)buf[2] << 16) | ((uint64_t)buf[3] << 24) |
           ((uint64_t)buf[4] << 32) | ((uint64_t)buf[5] << 40) | ((uint64_t)buf[6] << 48) | ((uint64_t)buf[7] << 56);
}

// see gf5248.h
void
gf5248_encode(void *dst, const gf5248 *a)
{
    uint8_t *buf = dst;
    gf5248 x;

    inner_gf5248_montgomery_reduce(&x, a);
    enc64le(buf, x.v0);
    enc64le(buf + 8, x.v1);
    enc64le(buf + 16, x.v2);
    enc64le(buf + 24, x.v3);
}

// see gf5248.h
uint32_t
gf5248_decode(gf5248 *d, const void *src)
{
    const uint8_t *buf = src;
    uint64_t d0, d1, d2, d3, t;
    unsigned char cc;

    d0 = dec64le(buf);
    d1 = dec64le(buf + 8);
    d2 = dec64le(buf + 16);
    d3 = dec64le(buf + 24);
    cc = inner_gf5248_sbb(0, d0, MODULUS.v0, &t);
    cc = inner_gf5248_sbb(cc, d1, MODULUS.v1, &t);
    cc = inner_gf5248_sbb(cc, d2, MODULUS.v2, &t);
    cc = inner_gf5248_sbb(cc, d3, MODULUS.v3, &t);
    (void)inner_gf5248_sbb(cc, 0, 0, &t);

    // If the value was not canonical then t = 0; otherwise, t = -1.
    d->v0 = d0 & t;
    d->v1 = d1 & t;
    d->v2 = d2 & t;
    d->v3 = d3 & t;

    // Convert to Montgomery representation.
    gf5248_mul(d, d, &R2);

    return (uint32_t)t;
}

// see gf5248.h
void
gf5248_decode_reduce(gf5248 *d, const void *src, size_t len)
{
    const uint8_t *buf = src;

    *d = ZERO;
    if (len == 0) {
        return;
    }

    if ((len & 31) != 0) {
        // Input size is not a multiple of 32, we decode a partial
        // block, which is already less than 2^248.
        uint8_t tmp[32];
        size_t k;

        k = len & ~(size_t)31;
        memcpy(tmp, buf + k, len - k);
        memset(tmp + len - k, 0, (sizeof tmp) - (len - k));
        d->v0 = dec64le(&tmp[0]);
        d->v1 = dec64le(&tmp[8]);
        d->v2 = dec64le(&tmp[16]);
        d->v3 = dec64le(&tmp[24]);
        len = k;
    } else {
        // Input size is a multiple of 32, we decode a full block,
        // and a reduction is needed.
        len -= 32;
        uint64_t d0 = dec64le(buf + len);
        uint64_t d1 = dec64le(buf + len + 8);
        uint64_t d2 = dec64le(buf + len + 16);
        uint64_t d3 = dec64le(buf + len + 24);
        inner_gf5248_partial_reduce(d, d0, d1, d2, d3);
    }

    // Process all remaining blocks, in descending address order.
    while (len > 0) {
        gf5248_mul(d, d, &R2);
        len -= 32;
        uint64_t t0 = dec64le(buf + len);
        uint64_t t1 = dec64le(buf + len + 8);
        uint64_t t2 = dec64le(buf + len + 16);
        uint64_t t3 = dec64le(buf + len + 24);
        gf5248 t;
        inner_gf5248_partial_reduce(&t, t0, t1, t2, t3);
        gf5248_add(d, d, &t);
    }

    // Final conversion to Montgomery representation.
    gf5248_mul(d, d, &R2);
}

void
gf5248_div3(gf5248 *d, const gf5248 *a)
{
    const digit_t MAGIC = 0xAAAAAAAAAAAAAAAB; // 3^-1 mod 2^64
    uint64_t c0, c1, f0, f1;
    gf5248 t;

    inner_gf5248_umul(f0, f1, a->arr[3], MAGIC);
    t.arr[3] = f1 >> 1;
    c1 = a->arr[3] - 3 * t.arr[3];

    for (int32_t i = 2; i >= 0; i--) {
        c0 = c1;
        inner_gf5248_umul(f0, f1, a->arr[i], MAGIC);
        t.arr[i] = f1 >> 1;
        c1 = c0 + a->arr[i] - 3 * t.arr[i];
        t.arr[i] += c0 * ((MAGIC - 1) >> 1);
        f0 = ((c1 >> 1) & c1);           /* c1 == 3 */
        f1 = ((c1 >> 2) & !(c1 & 0x11)); /* c1 == 4 */
        f0 |= f1;
        t.arr[i] += f0;
        c1 = c1 - 3 * f0;
    }
    *d = t;
    gf5248_sub(&t, d, &PM1O3);
    gf5248_select(d, d, &t, -((c1 & 1) | (c1 >> 1))); // c1 >= 1
    gf5248_sub(&t, d, &PM1O3);
    gf5248_select(d, d, &t, -(c1 == 2));
}
