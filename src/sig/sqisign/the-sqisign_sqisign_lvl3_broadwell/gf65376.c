#include "gf65376.h"

// see gf65376.h
const gf65376 ZERO = { 0, 0, 0, 0, 0, 0 };

// see gf65376.h
const gf65376 ONE = { 0x0000000000000003, 0x0000000000000000, 0x0000000000000000,
                              0x0000000000000000, 0x0000000000000000, 0x3D00000000000000 };

// see gf65376.h
const gf65376 gf65376_MINUS_ONE = { 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x03FFFFFFFFFFFFFF };

// Montgomery representation of 2^256.
static const gf65376 R2 = { 0x3F03F03F03F03F13, 0x03F03F03F03F03F0, 0xF03F03F03F03F03F,
                            0x3F03F03F03F03F03, 0x03F03F03F03F03F0, 0x1D3F03F03F03F03F };

// The modulus itself (this is also a valid representation of zero).
static const gf65376 MODULUS = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x40FFFFFFFFFFFFFF };

// 1/2^380 (in Montgomery representation).
static const gf65376 INVT380 = { 0x0000000000000010, 0x0000000000000000, 0x0000000000000000,
                                 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 };

static const gf65376 PM1O3 = { 0xaaaaaaaaaaaaaaaa, 0xaaaaaaaaaaaaaaaa, 0xaaaaaaaaaaaaaaaa,
                               0xaaaaaaaaaaaaaaaa, 0xaaaaaaaaaaaaaaaa, 0x15aaaaaaaaaaaaaa };

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
gf65376_lin(gf65376 *d, const gf65376 *u, const gf65376 *v, uint64_t f, uint64_t g)
{
    // f <- abs(f), keeping the sign in sf, and negating u accordingly
    uint64_t sf = sgnw(f);
    f = (f ^ sf) - sf;
    gf65376 tu;
    gf65376_neg(&tu, u);
    gf65376_select(&tu, u, &tu, (uint32_t)sf);

    // g <- abs(g), keeping the sign in sg, and negating v accordingly
    uint64_t sg = sgnw(g);
    g = (g ^ sg) - sg;
    gf65376 tv;
    gf65376_neg(&tv, v);
    gf65376_select(&tv, v, &tv, (uint32_t)sg);

    // Linear combination over plain integers.
    uint64_t d0, d1, d2, d3, d4, d5, t;
    inner_gf65376_umul_x2(d0, t, tu.v0, f, tv.v0, g);
    inner_gf65376_umul_x2_add(d1, t, tu.v1, f, tv.v1, g, t);
    inner_gf65376_umul_x2_add(d2, t, tu.v2, f, tv.v2, g, t);
    inner_gf65376_umul_x2_add(d3, t, tu.v3, f, tv.v3, g, t);
    inner_gf65376_umul_x2_add(d4, t, tu.v4, f, tv.v4, g, t);
    inner_gf65376_umul_x2_add(d5, t, tu.v5, f, tv.v5, g, t);

    // Reduction: split into low part (376 bits) and high part
    // (71 bits, since t can be up to 63 bits). If the high
    // part is h, then:
    //    h*2^376 = (h mod 65)*2^376 + floor(h/65)  mod q
    uint64_t h0 = (d5 >> 56) | (t << 8);
    uint64_t h1 = t >> 56;
    d5 &= 0x00FFFFFFFFFFFFFF;

    // NOTE: 0xFC0FC0FC0FC0FC1 = 65^-1 % 2^64
    // NOTE: 0xFC1 = 65^-1 % 2^12
    uint64_t z0, z1, quo0, rem0, quo1, rem1;
    inner_gf65376_umul(z0, z1, h0, 0xFC0FC0FC0FC0FC1);
    (void)z0;
    quo0 = z1 >> 2;
    rem0 = h0 - (65 * quo0);
    quo1 = (h1 * 0xFC1) >> 18; // Only keep bottom two bits
    rem1 = h1 - (65 * quo1);

    // h = rem0 + 65*quo0 + (rem1 + 65*quo1)*2^64
    //   = rem0 + rem1 + 65*(quo0 + quo1*2^64 + rem1*((2^64 - 1)/65))
    // We add rem0 and rem1 modulo 65, with an extra carry that
    // goes into the folded part (multiple of 65).
    uint64_t e, f0, f1;
    unsigned char cc;
    cc = inner_gf65376_adc(0, rem0 + 0xFFFFFFFFFFFFFFBE, rem1, &e);
    cc = inner_gf65376_adc(cc, quo0, rem1 * 0x3F03F03F03F03F0, &f0);
    cc = inner_gf65376_adc(cc, quo1, 0, &f1);
    assert(cc == 0);
    e -= 0xFFFFFFFFFFFFFFBE;

    // Now we only have to add e*2^384 + f0:f1 to the low part.
    cc = inner_gf65376_adc(0, d0, f0, &d0);
    cc = inner_gf65376_adc(cc, d1, f1, &d1);
    cc = inner_gf65376_adc(cc, d2, 0, &d2);
    cc = inner_gf65376_adc(cc, d3, 0, &d3);
    cc = inner_gf65376_adc(cc, d4, 0, &d4);
    (void)inner_gf65376_adc(cc, d5, e << 56, &d5);

    d->v0 = d0;
    d->v1 = d1;
    d->v2 = d2;
    d->v3 = d3;
    d->v4 = d4;
    d->v5 = d5;
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
lindiv31abs(gf65376 *d, const gf65376 *a, const gf65376 *b, uint64_t f, uint64_t g)
{
    // f <- abs(f), keeping the sign in sf
    uint64_t sf = sgnw(f);
    f = (f ^ sf) - sf;

    // g <- abs(g), keeping the sign in sg
    uint64_t sg = sgnw(g);
    g = (g ^ sg) - sg;

    // Apply the signs of f and g to the source operands.
    uint64_t a0, a1, a2, a3, a4, a5, a6;
    uint64_t b0, b1, b2, b3, b4, b5, b6;
    unsigned char cc;

    cc = inner_gf65376_sbb(0, a->v0 ^ sf, sf, &a0);
    cc = inner_gf65376_sbb(cc, a->v1 ^ sf, sf, &a1);
    cc = inner_gf65376_sbb(cc, a->v2 ^ sf, sf, &a2);
    cc = inner_gf65376_sbb(cc, a->v3 ^ sf, sf, &a3);
    cc = inner_gf65376_sbb(cc, a->v4 ^ sf, sf, &a4);
    cc = inner_gf65376_sbb(cc, a->v5 ^ sf, sf, &a5);
    (void)inner_gf65376_sbb(cc, 0, 0, &a6);

    cc = inner_gf65376_sbb(0, b->v0 ^ sg, sg, &b0);
    cc = inner_gf65376_sbb(cc, b->v1 ^ sg, sg, &b1);
    cc = inner_gf65376_sbb(cc, b->v2 ^ sg, sg, &b2);
    cc = inner_gf65376_sbb(cc, b->v3 ^ sg, sg, &b3);
    cc = inner_gf65376_sbb(cc, b->v4 ^ sg, sg, &b4);
    cc = inner_gf65376_sbb(cc, b->v5 ^ sg, sg, &b5);
    (void)inner_gf65376_sbb(cc, 0, 0, &b6);

    // Compute a*f + b*g into d0:d1:d2:d3:d4. Since f and g are at
    // most 2^31, we can add two 128-bit products with no overflow.
    // Note: a4 and b4 are both in {0, -1}.
    uint64_t d0, d1, d2, d3, d4, d5, d6, t;
    inner_gf65376_umul_x2(d0, t, a0, f, b0, g);
    inner_gf65376_umul_x2_add(d1, t, a1, f, b1, g, t);
    inner_gf65376_umul_x2_add(d2, t, a2, f, b2, g, t);
    inner_gf65376_umul_x2_add(d3, t, a3, f, b3, g, t);
    inner_gf65376_umul_x2_add(d4, t, a4, f, b4, g, t);
    inner_gf65376_umul_x2_add(d5, t, a5, f, b5, g, t);
    d6 = t - (a6 & f) - (b6 & g);

    // Right-shift the value by 31 bits.
    d0 = (d0 >> 31) | (d1 << 33);
    d1 = (d1 >> 31) | (d2 << 33);
    d2 = (d2 >> 31) | (d3 << 33);
    d3 = (d3 >> 31) | (d4 << 33);
    d4 = (d4 >> 31) | (d5 << 33);
    d5 = (d5 >> 31) | (d6 << 33);

    // If the result is negative, negate it.
    t = sgnw(d6);
    cc = inner_gf65376_sbb(0, d0 ^ t, t, &d0);
    cc = inner_gf65376_sbb(cc, d1 ^ t, t, &d1);
    cc = inner_gf65376_sbb(cc, d2 ^ t, t, &d2);
    cc = inner_gf65376_sbb(cc, d3 ^ t, t, &d3);
    cc = inner_gf65376_sbb(cc, d4 ^ t, t, &d4);
    (void)inner_gf65376_sbb(cc, d5 ^ t, t, &d5);

    d->v0 = d0;
    d->v1 = d1;
    d->v2 = d2;
    d->v3 = d3;
    d->v4 = d4;
    d->v5 = d5;
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

// see gf65376.h
uint32_t
gf65376_div(gf65376 *d, const gf65376 *x, const gf65376 *y)
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

    gf65376 a, b, u, v;
    uint64_t xa, xb, f0, g0, f1, g1;
    uint32_t r;

    r = ~gf65376_iszero(y);
    inner_gf65376_normalize(&a, y);
    b = MODULUS;
    u = *x;
    v = ZERO;

    // Generic loop does 23*31 = 713 inner iterations.
    for (int i = 0; i < 23; i++) {
        // Get approximations of a and b over 64 bits:
        //  - If len(a) <= 64 and len(b) <= 64, then we just use
        //    their values (low limbs).
        //  - Otherwise, with n = max(len(a), len(b)), we use:
        //       (a mod 2^31) + 2^31*floor(a / 2^(n - 33))
        //       (b mod 2^31) + 2^31*floor(b / 2^(n - 33))
        uint64_t m5 = a.v5 | b.v5;
        uint64_t m4 = a.v4 | b.v4;
        uint64_t m3 = a.v3 | b.v3;
        uint64_t m2 = a.v2 | b.v2;
        uint64_t m1 = a.v1 | b.v1;
        uint64_t tnz5 = sgnw(m5 | -m5);
        uint64_t tnz4 = sgnw(m4 | -m4) & ~tnz5;
        uint64_t tnz3 = sgnw(m3 | -m3) & ~tnz5 & ~tnz4;
        uint64_t tnz2 = sgnw(m2 | -m2) & ~tnz5 & ~tnz4 & ~tnz3;
        uint64_t tnz1 = sgnw(m1 | -m1) & ~tnz5 & ~tnz4 & ~tnz3 & ~tnz2;
        uint64_t tnzm = (m5 & tnz5) | (m4 & tnz4) | (m3 & tnz3) | (m2 & tnz2) | (m1 & tnz1);
        uint64_t tnza = (a.v5 & tnz5) | (a.v4 & tnz4) | (a.v3 & tnz3) | (a.v2 & tnz2) | (a.v1 & tnz1);
        uint64_t tnzb = (b.v5 & tnz5) | (b.v4 & tnz4) | (b.v3 & tnz3) | (b.v2 & tnz2) | (b.v1 & tnz1);
        uint64_t snza = (a.v4 & tnz5) | (a.v3 & tnz4) | (a.v2 & tnz3) | (a.v1 & tnz2) | (a.v0 & tnz1);
        uint64_t snzb = (b.v4 & tnz5) | (b.v3 & tnz4) | (b.v2 & tnz3) | (b.v1 & tnz2) | (b.v0 & tnz1);

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
        //       tnz1 = tnz2 = tnz3 = tnz4 = tnz5 = 0
        //       we want to use the entire low words of a and b
        //  - Otherwise, we want to use the top 33 bits of tnza and
        //    tnzb, and the low 31 bits of the low words of a and b.
        uint64_t tzx = ~(tnz1 | tnz2 | tnz3 | tnz4 | tnz5);
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
            cc = inner_gf65376_sbb(0, xa, xb, &t0);
            (void)inner_gf65376_sbb(cc, 0, 0, &swap);
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
        gf65376 na, nb, nu, nv;
        uint64_t nega = lindiv31abs(&na, &a, &b, f0, g0);
        uint64_t negb = lindiv31abs(&nb, &a, &b, f1, g1);
        f0 = (f0 ^ nega) - nega;
        g0 = (g0 ^ nega) - nega;
        f1 = (f1 ^ negb) - negb;
        g1 = (g1 ^ negb) - negb;
        gf65376_lin(&nu, &u, &v, f0, g0);
        gf65376_lin(&nv, &u, &v, f1, g1);
        a = na;
        b = nb;
        u = nu;
        v = nv;
    }

    // If y is invertible, then the final GCD is 1, and
    // len(a) + len(b) <= 53, so we can end the computation with
    // the low words directly. We only need 51 iterations to reach
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
    for (int j = 0; j < 51; j++) {
        uint64_t a_odd, swap, t0, t1, t2, t3;
        unsigned char cc;
        a_odd = -(xa & 1);
        cc = inner_gf65376_sbb(0, xa, xb, &t0);
        (void)inner_gf65376_sbb(cc, 0, 0, &swap);
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
    gf65376_lin(d, &u, &v, f1, g1);

    // At the point:
    //  - Numerator and denominator were both in Montgomery representation,
    //    but the two factors R canceled each other.
    //  - We have injected 31*23+51 = 764 extra factors of 2, hence we
    //    must divide the result by 2^764.
    //  - However, we also want to obtain the result in Montgomery
    //    representation, i.e. multiply by 2^256. We thus want to
    //    divide the current result by 2^(764 - 384) = 2^380.
    //  - We do this division by using a Montgomery multiplication with
    //    the Montgomery representation of 1/2^380, i.e. the integer
    //    2^384/2^380 = 16.
    gf65376_mul(d, d, &INVT380);
    return r;
}

// see gf65376.h
uint32_t
gf65376_invert(gf65376 *d, const gf65376 *a)
{
    return gf65376_div(d, &ONE, a);
}

// see gf65376.h
int32_t
gf65376_legendre(const gf65376 *x)
{
    // Same algorithm as the binary GCD in gf65376_div(), with
    // a few differences:
    //   - We do not keep track of the Bézout coefficients u and v.
    //   - In each inner iteration we adjust the running symbol value,
    //     which uses the low 3 bits of the values.
    //   - Since we need two extra bits of look-ahead, we can only run
    //     29 inner iterations, and then need an extra recomputation
    //     for the last 2.

    gf65376 a, b;
    uint64_t xa, xb, f0, g0, f1, g1, ls;

    inner_gf65376_normalize(&a, x);
    b = MODULUS;
    ls = 0; // running symbol information in bit 1.

    // Outer loop
    for (int i = 0; i < 23; i++) {
        // Get approximations of a and b over 64 bits.
        uint64_t m5 = a.v5 | b.v5;
        uint64_t m4 = a.v4 | b.v4;
        uint64_t m3 = a.v3 | b.v3;
        uint64_t m2 = a.v2 | b.v2;
        uint64_t m1 = a.v1 | b.v1;
        uint64_t tnz5 = sgnw(m5 | -m5);
        uint64_t tnz4 = sgnw(m4 | -m4) & ~tnz5;
        uint64_t tnz3 = sgnw(m3 | -m3) & ~tnz5 & ~tnz4;
        uint64_t tnz2 = sgnw(m2 | -m2) & ~tnz5 & ~tnz4 & ~tnz3;
        uint64_t tnz1 = sgnw(m1 | -m1) & ~tnz5 & ~tnz4 & ~tnz3 & ~tnz2;
        uint64_t tnzm = (m5 & tnz5) | (m4 & tnz4) | (m3 & tnz3) | (m2 & tnz2) | (m1 & tnz1);
        uint64_t tnza = (a.v5 & tnz5) | (a.v4 & tnz4) | (a.v3 & tnz3) | (a.v2 & tnz2) | (a.v1 & tnz1);
        uint64_t tnzb = (b.v5 & tnz5) | (b.v4 & tnz4) | (b.v3 & tnz3) | (b.v2 & tnz2) | (b.v1 & tnz1);
        uint64_t snza = (a.v4 & tnz5) | (a.v3 & tnz4) | (a.v2 & tnz3) | (a.v1 & tnz2) | (a.v0 & tnz1);
        uint64_t snzb = (b.v4 & tnz5) | (b.v3 & tnz4) | (b.v2 & tnz3) | (b.v1 & tnz2) | (b.v0 & tnz1);

        int64_t s = lzcnt(tnzm);
        uint64_t sm = (uint64_t)((31 - s) >> 63);
        tnza ^= sm & (tnza ^ ((tnza << 32) | (snza >> 32)));
        tnzb ^= sm & (tnzb ^ ((tnzb << 32) | (snzb >> 32)));
        s -= 32 & sm;
        tnza <<= s;
        tnzb <<= s;

        uint64_t tzx = ~(tnz1 | tnz2 | tnz3 | tnz4 | tnz5);
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
            cc = inner_gf65376_sbb(0, xa, xb, &t0);
            (void)inner_gf65376_sbb(cc, 0, 0, &swap);
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
            cc = inner_gf65376_sbb(0, xa, xb, &t0);
            (void)inner_gf65376_sbb(cc, 0, 0, &swap);
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
        gf65376 na, nb;
        uint64_t nega = lindiv31abs(&na, &a, &b, f0, g0);
        (void)lindiv31abs(&nb, &a, &b, f1, g1);
        ls ^= nega & nb.v0;
        a = na;
        b = nb;
    }

    // Final iterations: values are at most 53 bits now. We do not
    // need to keep track of update coefficients. Just like the GCD,
    // we need only 51 iterations, because after 51 iterations,
    // value a is 0 or 1, and b is 1, and no further modification to
    // the Legendre symbol may happen.
    xa = a.v0;
    xb = b.v0;
    for (int j = 0; j < 51; j++) {
        uint64_t a_odd, swap, t0, t1;
        unsigned char cc;
        a_odd = -(xa & 1);
        cc = inner_gf65376_sbb(0, xa, xb, &t0);
        (void)inner_gf65376_sbb(cc, 0, 0, &swap);
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
    r &= ~gf65376_iszero(x);
    return *(int32_t *)&r;
}

// see gf65376.h
uint32_t
gf65376_sqrt(gf65376 *d, const gf65376 *a)
{
    // Candidate root is a^((q+1)/4), with (q+1)/4 = 65*2^374
    gf65376 y;
    gf65376_xsquare(&y, a, 6);
    gf65376_mul(&y, &y, a);
    gf65376_xsquare(&y, &y, 374);

    // Normalize y and negate if necessary, to set the low bit to 0.
    // The low bit check must be on the normal representation,
    // not the Montgomery representation.
    gf65376 yn;
    inner_gf65376_montgomery_reduce(&yn, &y);
    uint32_t ctl = -((uint32_t)yn.v0 & 1);
    gf65376_neg(&yn, &y);
    gf65376_select(&y, &y, &yn, ctl);

    // Check whether the candidate is indeed a square root.
    gf65376_square(&yn, &y);
    uint32_t r = gf65376_equals(&yn, a);
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

// see gf65376.h
void
gf65376_encode(void *dst, const gf65376 *a)
{
    uint8_t *buf = dst;
    gf65376 x;

    inner_gf65376_montgomery_reduce(&x, a);
    enc64le(buf, x.v0);
    enc64le(buf + 8, x.v1);
    enc64le(buf + 16, x.v2);
    enc64le(buf + 24, x.v3);
    enc64le(buf + 32, x.v4);
    enc64le(buf + 40, x.v5);
}

// see gf65376.h
uint32_t
gf65376_decode(gf65376 *d, const void *src)
{
    const uint8_t *buf = src;
    uint64_t d0, d1, d2, d3, d4, d5, t;
    unsigned char cc;

    d0 = dec64le(buf);
    d1 = dec64le(buf + 8);
    d2 = dec64le(buf + 16);
    d3 = dec64le(buf + 24);
    d4 = dec64le(buf + 32);
    d5 = dec64le(buf + 40);
    cc = inner_gf65376_sbb(0, d0, MODULUS.v0, &t);
    cc = inner_gf65376_sbb(cc, d1, MODULUS.v1, &t);
    cc = inner_gf65376_sbb(cc, d2, MODULUS.v2, &t);
    cc = inner_gf65376_sbb(cc, d3, MODULUS.v3, &t);
    cc = inner_gf65376_sbb(cc, d4, MODULUS.v4, &t);
    cc = inner_gf65376_sbb(cc, d5, MODULUS.v5, &t);

    (void)inner_gf65376_sbb(cc, 0, 0, &t);

    // If the value was not canonical then t = 0; otherwise, t = -1.
    d->v0 = d0 & t;
    d->v1 = d1 & t;
    d->v2 = d2 & t;
    d->v3 = d3 & t;
    d->v4 = d4 & t;
    d->v5 = d5 & t;

    // Convert to Montgomery representation.
    gf65376_mul(d, d, &R2);

    return (uint32_t)t;
}

// see gf65376.h
void
gf65376_decode_reduce(gf65376 *d, const void *src, size_t len)
{
    const uint8_t *buf = src;

    *d = ZERO;
    if (len == 0) {
        return;
    }

    size_t rem = len % 48;
    if (rem != 0) {
        // Input size is not a multiple of 48, we decode a partial
        // block, which is already less than 2^383.
        uint8_t tmp[48];
        size_t k;

        k = len - rem;

        memcpy(tmp, buf + k, len - k);
        memset(tmp + len - k, 0, (sizeof tmp) - (len - k));
        d->v0 = dec64le(&tmp[0]);
        d->v1 = dec64le(&tmp[8]);
        d->v2 = dec64le(&tmp[16]);
        d->v3 = dec64le(&tmp[24]);
        d->v4 = dec64le(&tmp[32]);
        d->v5 = dec64le(&tmp[40]);

        len = k;
    } else {
        // Input size is a multiple of 48, we decode a full block,
        // and a reduction is needed.
        len -= 48;
        uint64_t d0 = dec64le(buf + len);
        uint64_t d1 = dec64le(buf + len + 8);
        uint64_t d2 = dec64le(buf + len + 16);
        uint64_t d3 = dec64le(buf + len + 24);
        uint64_t d4 = dec64le(buf + len + 32);
        uint64_t d5 = dec64le(buf + len + 40);
        inner_gf65376_partial_reduce(d, d0, d1, d2, d3, d4, d5);
    }

    // Process all remaining blocks, in descending address order.
    while (len > 0) {
        gf65376_mul(d, d, &R2);
        len -= 48;
        uint64_t t0 = dec64le(buf + len);
        uint64_t t1 = dec64le(buf + len + 8);
        uint64_t t2 = dec64le(buf + len + 16);
        uint64_t t3 = dec64le(buf + len + 24);
        uint64_t t4 = dec64le(buf + len + 32);
        uint64_t t5 = dec64le(buf + len + 40);

        gf65376 t;
        inner_gf65376_partial_reduce(&t, t0, t1, t2, t3, t4, t5);
        gf65376_add(d, d, &t);
    }

    // Final conversion to Montgomery representation.
    gf65376_mul(d, d, &R2);
}

void
gf65376_div3(gf65376 *d, const gf65376 *a)
{
    const digit_t MAGIC = 0xAAAAAAAAAAAAAAAB; // 3^-1 mod 2^64
    uint64_t c0, c1, f0, f1;
    gf65376 t;

    inner_gf65376_umul(f0, f1, a->arr[5], MAGIC);
    t.arr[5] = f1 >> 1;
    c1 = a->arr[5] - 3 * t.arr[5];

    for (int32_t i = 4; i >= 0; i--) {
        c0 = c1;
        inner_gf65376_umul(f0, f1, a->arr[i], MAGIC);
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
    gf65376_sub(&t, d, &PM1O3);
    gf65376_select(d, d, &t, -((c1 & 1) | (c1 >> 1))); // c1 >= 1
    gf65376_sub(&t, d, &PM1O3);
    gf65376_select(d, d, &t, -(c1 == 2));
}
