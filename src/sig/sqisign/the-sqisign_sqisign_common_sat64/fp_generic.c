// SPDX-License-Identifier: Apache-2.0
// See fp_generic.h. Requires FPG_SPARE >= 4 (the asm domain invariant) and
// FPG_C < 2^31, odd.

#include <fp_generic.h>
#include <tutil.h>
#include <string.h>

#define N FPG_N
#define TOPW (FPG_BITS - 64 * (N - 1)) // bits used in the top limb

__extension__ typedef unsigned __int128 fpg_u128_t;

static const fpg_t P = { FPG_P_INIT };
static const fpg_t R2 = { FPG_R2_INIT };
static const fpg_t TWO_INV = { FPG_TWO_INV_INIT };
static const fpg_t THREE_INV = { FPG_THREE_INV_INIT };
static const fpg_t ONE_RAW = { { 1 } };
const fpg_t fpg_ZERO = { { 0 } };
const fpg_t fpg_ONE = { FPG_ONE_INIT };

static uint64_t
mask64(uint32_t ctl)
{
    return (uint64_t)(int64_t)(int32_t)ctl;
}

static uint32_t
limbs_iszero(const uint64_t *v, const uint64_t *xorwith)
{
    uint64_t r = 0;
    for (int i = 0; i < N; i++)
        r |= v[i] ^ (xorwith ? xorwith[i] : 0);
    return (uint32_t)((r | (0 - r)) >> 63) - 1;
}

// w (N+1 limbs, < 2^(FPG_T+64)) -> d in [0, 2^FPG_BITS), value preserved mod p
static void
partial_reduce(fpg_t *d, const uint64_t *w)
{
    const int ti = FPG_T / 64, ts = FPG_T % 64;
    uint64_t h = w[ti] >> ts;
    if (ts)
        h |= w[ti + 1] << (64 - ts);
    uint64_t q = h / FPG_C, r = h - q * FPG_C;

    uint64_t out[FPG_N];
    for (int i = 0; i < N; i++)
        out[i] = (i < ti) ? w[i] : 0;
    if (ts)
        out[ti] = w[ti] & ((UINT64_C(1) << ts) - 1);

    // out += q + (r << FPG_T)
    fpg_u128_t cc = (fpg_u128_t)out[0] + q;
    out[0] = (uint64_t)cc;
    cc >>= 64;
    for (int i = 1; i < N; i++) {
        cc += out[i];
        out[i] = (uint64_t)cc;
        cc >>= 64;
    }
    fpg_u128_t rs = (fpg_u128_t)r << ts;
    cc = (fpg_u128_t)out[ti] + (uint64_t)rs;
    out[ti] = (uint64_t)cc;
    cc >>= 64;
    cc += (uint64_t)(rs >> 64);
    for (int i = ti + 1; i < N; i++) {
        cc += out[i];
        out[i] = (uint64_t)cc;
        cc >>= 64;
    }

    // one conditional subtraction of p (result <= p + q < 2^BITS + 2^64)
    uint64_t k = 0 - (out[N - 1] >> TOPW);
    unsigned char b = 0;
    for (int i = 0; i < N; i++) {
        fpg_u128_t t = (fpg_u128_t)out[i] - (P.v[i] & k) - b;
        d->v[i] = (uint64_t)t;
        b = (t >> 64) & 1;
    }
}

void
fp_copy(fpg_t *d, const fpg_t *a)
{
    memcpy(d, a, sizeof(fpg_t));
}

void
fp_set_zero(fpg_t *a)
{
    memcpy(a, &fpg_ZERO, sizeof(fpg_t));
}

void
fp_set_one(fpg_t *a)
{
    memcpy(a, &fpg_ONE, sizeof(fpg_t));
}

void
fp_neg(fpg_t *d, const fpg_t *a)
{
    fp_sub(d, &fpg_ZERO, a);
}

void
fp_half(fpg_t *d, const fpg_t *a)
{
    fp_mul(d, a, &TWO_INV);
}

void
fp_div3(fpg_t *d, const fpg_t *a)
{
    fp_mul(d, a, &THREE_INV);
}

void
fp_set_small(fpg_t *d, uint32_t x)
{
    fpg_t t = { { 0 } };
    t.v[0] = x;
    fp_mul(d, &t, &R2);
}

void
fp_mul_small(fpg_t *d, const fpg_t *a, uint32_t x)
{
    uint64_t w[FPG_N + 1];
    fpg_u128_t cc = 0;
    for (int i = 0; i < N; i++) {
        cc += (fpg_u128_t)a->v[i] * x;
        w[i] = (uint64_t)cc;
        cc >>= 64;
    }
    w[N] = (uint64_t)cc;
    partial_reduce(d, w);
}

void
fp_select(fpg_t *d, const fpg_t *a0, const fpg_t *a1, uint32_t ctl)
{
    uint64_t m = mask64(ctl);
    for (int i = 0; i < N; i++)
        d->v[i] = a0->v[i] ^ (m & (a0->v[i] ^ a1->v[i]));
}

void
fp_cswap(fpg_t *a, fpg_t *b, uint32_t ctl)
{
    uint64_t m = mask64(ctl);
    for (int i = 0; i < N; i++) {
        uint64_t t = m & (a->v[i] ^ b->v[i]);
        a->v[i] ^= t;
        b->v[i] ^= t;
    }
}

uint32_t
fp_is_zero(const fpg_t *a)
{
    return limbs_iszero(a->v, NULL) | limbs_iszero(a->v, P.v);
}

uint32_t
fp_is_equal(const fpg_t *a, const fpg_t *b)
{
    fpg_t t;
    fp_sub(&t, a, b);
    return fp_is_zero(&t);
}

// canonical value of a into m (in [0, p))
static void
canonical(uint64_t *m, const fpg_t *a)
{
    fpg_t t;
    fp_mul(&t, a, &ONE_RAW); // leave Montgomery; result < 2^BITS < 2p
    unsigned char b = 0;
    uint64_t s[FPG_N];
    for (int i = 0; i < N; i++) {
        fpg_u128_t u = (fpg_u128_t)t.v[i] - P.v[i] - b;
        s[i] = (uint64_t)u;
        b = (u >> 64) & 1;
    }
    uint64_t keep = 0 - (uint64_t)b; // borrow -> t < p -> keep t
    for (int i = 0; i < N; i++)
        m[i] = (t.v[i] & keep) | (s[i] & ~keep);
}

void
fp_encode(void *dst, const fpg_t *a)
{
    const size_t nbytes = (FPG_BITS + 7) / 8;
    uint64_t m[FPG_N];
    canonical(m, a);
    uint8_t *o = dst;
    for (int i = 0; i < N; i++)
        for (unsigned j = 0; j < (nbytes - 8 * i < 8 ? nbytes - 8 * i : 8); j++)
            o[8 * i + j] = (uint8_t)(m[i] >> (8 * j));
}

uint32_t
fp_decode(fpg_t *d, const void *src)
{
    const size_t nbytes = (FPG_BITS + 7) / 8;
    const uint8_t *s = src;
    fpg_t v;
    for (int i = 0; i < N; i++) {
        uint64_t l = 0;
        for (unsigned j = 0; j < (nbytes - 8 * i < 8 ? nbytes - 8 * i : 8); j++)
            l |= (uint64_t)s[8 * i + j] << (8 * j);
        v.v[i] = l;
    }
    unsigned char b = 0;
    for (int i = 0; i < N; i++) {
        fpg_u128_t u = (fpg_u128_t)v.v[i] - P.v[i] - b;
        b = (u >> 64) & 1;
    }
    uint32_t ok = (uint32_t)0 - (uint32_t)b; // borrow -> v < p -> valid
    fp_mul(d, &v, &R2);
    for (int i = 0; i < N; i++)
        d->v[i] &= mask64(ok);
    return ok;
}

void
fp_decode_reduce(fpg_t *d, const void *src, size_t len)
{
    const uint8_t *s = src;
    const size_t B = 8 * (size_t)N;
    fpg_t acc = { { 0 } };
    if (len == 0) {
        *d = acc;
        return;
    }
    size_t nb = (len + B - 1) / B;
    for (size_t blk = nb; blk-- > 0;) {
        size_t off = blk * B;
        size_t n = (off + B <= len) ? B : len - off;
        uint64_t w[FPG_N + 1];
        memset(w, 0, sizeof w);
        for (size_t i = 0; i < n; i++)
            w[i / 8] |= (uint64_t)s[off + i] << (8 * (i % 8));
        fpg_t bred;
        partial_reduce(&bred, w);
        if (blk == nb - 1) {
            acc = bred;
        } else {
            fpg_t sh;
            fp_mul(&sh, &acc, &R2); // acc * 2^(64N) mod p
            fp_add(&acc, &sh, &bred);
        }
    }
    fp_mul(d, &acc, &R2); // to Montgomery
}

// d = a^(2^k - 1), k >= 1
static void
pow_ones(fpg_t *d, const fpg_t *a, int k)
{
    fpg_t x = *a;
    int e = 1;
    for (int i = 62 - sqisign_clz64((unsigned long long)k); i >= 0; i--) {
        fpg_t y = x;
        for (int j = 0; j < e; j++)
            fp_sqr(&y, &y);
        fp_mul(&x, &y, &x);
        e <<= 1;
        if ((k >> i) & 1) {
            fp_sqr(&x, &x);
            fp_mul(&x, &x, a);
            e += 1;
        }
    }
    *d = x;
}

// a <- a^((p-3)/4), with (p-3)/4 = (C-1)*2^(T-2) + (2^(T-2) - 1)
void
fp_exp3div4(fpg_t *a)
{
    fpg_t base = *a, M;
    pow_ones(&M, &base, FPG_T - 2);
#if FPG_C == 1
    *a = M;
#else
    fpg_t Ma, z;
    fp_mul(&Ma, &M, &base); // a^(2^(T-2))
    z = Ma;
    for (int i = 62 - sqisign_clz64((unsigned long long)(FPG_C - 1)); i >= 0; i--) {
        fp_sqr(&z, &z);
        if (((FPG_C - 1) >> i) & 1)
            fp_mul(&z, &z, &Ma);
    }
    fp_mul(a, &z, &M);
#endif
}

void
fp_inv(fpg_t *a)
{
    fpg_t a0 = *a, t = *a;
    fp_exp3div4(&t);
    fp_sqr(&t, &t);
    fp_sqr(&t, &t);
    fp_mul(a, &t, &a0); // a^(p-2)
}

void
fp_sqrt(fpg_t *a)
{
    fpg_t a0 = *a, t = *a;
    fp_exp3div4(&t);
    fp_mul(a, &t, &a0); // a^((p+1)/4)
}

uint32_t
fp_is_square(const fpg_t *a)
{
    fpg_t t = *a;
    fp_exp3div4(&t);
    fp_sqr(&t, &t);
    fp_mul(&t, &t, a); // a^((p-1)/2)
    return fp_is_equal(&t, &fpg_ONE) | fp_is_zero(a);
}
