// clang-format off
// Command line : python monty.py 64
// 0x4ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
#include <stdint.h>
#include <stdio.h>

#ifdef RADIX_64

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 5
#define Radix 51
#define Nbits 251
#define Nbytes 32

#define MONTGOMERY
// propagate carries
inline static spint prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 51u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 51u;
  n[0] &= mask;
  for (i = 1; i < 4; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 51u;
  }
  n[4] += (spint)carry;
  return -((n[4] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
inline static int flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[4] += ((spint)0x500000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
inline static int modfsb(spint *n) {
  n[0] += (spint)1u;
  n[4] -= (spint)0x500000000000u;
  return flatten(n);
}

// Modular addition - reduce less than 2p
inline static void modadd(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[0] += (spint)2u;
  n[4] -= (spint)0xa00000000000u;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[4] += ((spint)0xa00000000000u) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
inline static void modsub(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] - b[0];
  n[1] = a[1] - b[1];
  n[2] = a[2] - b[2];
  n[3] = a[3] - b[3];
  n[4] = a[4] - b[4];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[4] += ((spint)0xa00000000000u) & carry;
  (void)prop(n);
}

// Modular negation
inline static void modneg(const spint *b, spint *n) {
  spint carry;
  n[0] = (spint)0 - b[0];
  n[1] = (spint)0 - b[1];
  n[2] = (spint)0 - b[2];
  n[3] = (spint)0 - b[3];
  n[4] = (spint)0 - b[4];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[4] += ((spint)0xa00000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 25551082561965953719787503747077
// Modular multiplication, c=a*b mod 2p
inline static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p4 = 0x500000000000u;
  spint q = ((spint)1 << 51u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  t += (dpint)a[0] * b[0];
  spint v0 = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  spint v1 = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  spint v2 = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  spint v3 = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  t += (dpint)v0 * (dpint)p4;
  spint v4 = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)v1 * (dpint)p4;
  c[0] = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)v2 * (dpint)p4;
  c[1] = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)v3 * (dpint)p4;
  c[2] = ((spint)t & mask);
  t >>= 51;
  t += (dpint)a[4] * b[4];
  t += (dpint)v4 * (dpint)p4;
  c[3] = ((spint)t & mask);
  t >>= 51;
  c[4] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
inline static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p4 = 0x500000000000u;
  spint q = ((spint)1 << 51u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  spint v1 = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  spint v2 = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  spint v3 = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  t += (udpint)v0 * p4;
  spint v4 = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  t += (udpint)v1 * p4;
  c[0] = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v2 * p4;
  c[1] = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v3 * p4;
  c[2] = ((spint)t & mask);
  t >>= 51;
  tot = (udpint)a[4] * a[4];
  t += tot;
  t += (udpint)v4 * p4;
  c[3] = ((spint)t & mask);
  t >>= 51;
  c[4] = (spint)t;
}

// copy
inline static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 5; i++) {
    c[i] = a[i];
  }
}

// square n times
static void modnsqr(spint *a, int n) {
  int i;
  for (i = 0; i < n; i++) {
    modsqr(a, a);
  }
}

// Calculate progenitor
static void modpro(const spint *w, spint *z) {
  spint x[5];
  spint t0[5];
  spint t1[5];
  spint t2[5];
  spint t3[5];
  spint t4[5];
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, t0);
  modsqr(t0, z);
  modmul(x, z, z);
  modsqr(z, t1);
  modsqr(t1, t3);
  modsqr(t3, t2);
  modcpy(t2, t4);
  modnsqr(t4, 3);
  modmul(t2, t4, t2);
  modcpy(t2, t4);
  modnsqr(t4, 6);
  modmul(t2, t4, t2);
  modcpy(t2, t4);
  modnsqr(t4, 2);
  modmul(t3, t4, t3);
  modnsqr(t3, 13);
  modmul(t2, t3, t2);
  modcpy(t2, t3);
  modnsqr(t3, 27);
  modmul(t2, t3, t2);
  modmul(z, t2, z);
  modcpy(z, t2);
  modnsqr(t2, 4);
  modmul(t1, t2, t1);
  modmul(t0, t1, t0);
  modmul(t1, t0, t1);
  modmul(t0, t1, t0);
  modmul(t1, t0, t2);
  modmul(t0, t2, t0);
  modmul(t1, t0, t1);
  modnsqr(t1, 63);
  modmul(t0, t1, t1);
  modnsqr(t1, 64);
  modmul(t0, t1, t0);
  modnsqr(t0, 57);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[5];
  spint t[5];
  if (h == NULL) {
    modpro(x, t);
  } else {
    modcpy(h, t);
  }
  modcpy(x, s);
  modnsqr(t, 2);
  modmul(s, t, z);
}

// Convert m to n-residue form, n=nres(m)
static void nres(const spint *m, spint *n) {
  const spint c[5] = {0x4cccccccccf5cu, 0x1999999999999u, 0x3333333333333u,
                      0x6666666666666u, 0xcccccccccccu};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[5];
  c[0] = 1;
  for (i = 1; i < 5; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[5];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 5; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 51u) &
          (((c0 ^ (spint)1) - (spint)1) >> 51u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[5];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 5; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 51u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 5; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 5; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 5; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Modular multiplication by an integer, c=a*b mod 2p
inline static void modmli(const spint *a, int b, spint *c) {
  spint t[5];
  modint(b, t);
  modmul(a, t, c);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[5];
  if (h == NULL) {
    modpro(x, r);
    modsqr(r, r);
  } else {
    modsqr(h, r);
  }
  modmul(r, x, r);
  return modis1(r) | modis0(x);
}

// conditional swap g and f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void modcsw(int b, volatile spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, w;
  spint r = 0x3cc3c33c5aa5a55au;
  c0 = (1 - b) + r;
  c1 = b + r;
  for (i = 0; i < 5; i++) {
    s = g[i];
    t = f[i];
    w = r * (t + s);
    f[i] = c0 * t + c1 * s;
    f[i] -= w;
    g[i] = c0 * s + c1 * t;
    g[i] -= w;
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  spint s[5];
  spint y[5];
  if (h == NULL) {
    modpro(x, y);
  } else {
    modcpy(h, y);
  }
  modmul(y, x, s);
  modcpy(s, r);
}

// shift left by less than a word
static void modshl(unsigned int n, spint *a) {
  int i;
  a[4] = ((a[4] << n)) | (a[3] >> (51u - n));
  for (i = 3; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x7ffffffffffff) | (a[i - 1] >> (51u - n));
  }
  a[0] = (a[0] << n) & (spint)0x7ffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 4; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (51u - n)) & (spint)0x7ffffffffffff);
  }
  a[4] = a[4] >> n;
  return r;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[5], d[5];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 5; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 51) & 1;
  }
  return eq;
}

// clang-format on
/******************************************************************************
 API functions calling generated code above
 ******************************************************************************/

#include <fp.h>

const fp_t ZERO = {{ 0x0, 0x0, 0x0, 0x0, 0x0 }};
const fp_t ONE = {{ 0x0000000000000019,
                                    0x0000000000000000,
                                    0x0000000000000000,
                                    0x0000000000000000,
                                    0x0000300000000000 }};
// Montgomery representation of 2^-1
static const fp_t TWO_INV = {{ 0x000000000000000c,
                                               0x0000000000000000,
                                               0x0000000000000000,
                                               0x0000000000000000,
                                               0x0000400000000000 }};
// Montgomery representation of 3^-1
static const fp_t THREE_INV = {{ 0x000555555555555d,
                                                 0x0002aaaaaaaaaaaa,
                                                 0x0005555555555555,
                                                 0x0002aaaaaaaaaaaa,
                                                 0x0000455555555555 }};
// Montgomery representation of 2^256
static const fp_t R2 = {{ 0x0001999999999eb8,
                                          0x0003333333333333,
                                          0x0006666666666666,
                                          0x0004cccccccccccc,
                                          0x0000199999999999 }};

void
fp_set_small(fp_t *x, const digit_t val)
{
    modint((int)val, x->fp);
}

void
fp_mul_small(fp_t *x, const fp_t *a, const uint32_t val)
{
    modmli(a->fp, (int)val, x->fp);
}

void
fp_set_zero(fp_t *x)
{
    modzer(x->fp);
}

void
fp_set_one(fp_t *x)
{
    modone(x->fp);
}

uint32_t
fp_is_equal(const fp_t *a, const fp_t *b)
{
    return -(uint32_t)modcmp(a->fp, b->fp);
}

uint32_t
fp_is_zero(const fp_t *a)
{
    return -(uint32_t)modis0(a->fp);
}

void
fp_copy(fp_t *out, const fp_t *a)
{
    modcpy(a->fp, out->fp);
}

void
fp_cswap(fp_t *a, fp_t *b, uint32_t ctl)
{
    modcsw((int)(ctl & 0x1), a->fp, b->fp);
}

void
fp_add(fp_t *out, const fp_t *a, const fp_t *b)
{
    modadd(a->fp, b->fp, out->fp);
}

void
fp_sub(fp_t *out, const fp_t *a, const fp_t *b)
{
    modsub(a->fp, b->fp, out->fp);
}

void
fp_neg(fp_t *out, const fp_t *a)
{
    modneg(a->fp, out->fp);
}

void
fp_sqr(fp_t *out, const fp_t *a)
{
    modsqr(a->fp, out->fp);
}

void
fp_mul(fp_t *out, const fp_t *a, const fp_t *b)
{
    modmul(a->fp, b->fp, out->fp);
}

void
fp_inv(fp_t *x)
{
    modinv(x->fp, NULL, x->fp);
}

uint32_t
fp_is_square(const fp_t *a)
{
    return -(uint32_t)modqr(NULL, a->fp);
}

void
fp_sqrt(fp_t *a)
{
    modsqrt(a->fp, NULL, a->fp);
}

void
fp_half(fp_t *out, const fp_t *a)
{
    modmul(TWO_INV.fp, a->fp, out->fp);
}

void
fp_exp3div4(fp_t *out, const fp_t *a)
{
    modpro(a->fp, out->fp);
}

void
fp_div3(fp_t *out, const fp_t *a)
{
    modmul(THREE_INV.fp, a->fp, out->fp);
}

void
fp_encode(void *dst, const fp_t *a)
{
    // Modified version of modexp()
    int i;
    spint c[5];
    redc(a->fp, c);
    for (i = 0; i < 32; i++) {
        ((char *)dst)[i] = c[0] & (spint)0xff;
        (void)modshr(8, c);
    }
}

uint32_t
fp_decode(fp_t *d, const void *src)
{
    // Modified version of modimp()
    int i;
    spint res;
    const unsigned char *b = src;
    for (i = 0; i < 5; i++) {
        d->fp[i] = 0;
    }
    for (i = 31; i >= 0; i--) {
        modshl(8, d->fp);
        d->fp[0] += (spint)b[i];
    }
    res = (spint)-modfsb(d->fp);
    nres(d->fp, d->fp);
    // If the value was canonical then res = -1; otherwise, res = 0
    for (i = 0; i < 5; i++) {
        d->fp[i] &= res;
    }
    return (uint32_t)res;
}

static inline unsigned char
add_carry(unsigned char cc, spint a, spint b, spint *d)
{
    udpint t = (udpint)a + (udpint)b + cc;
    *d = (spint)t;
    return (unsigned char)(t >> Wordlength);
}

static void
partial_reduce(spint *out, const spint *src)
{
    spint h, l, quo, rem;
    unsigned char cc;

    // Split value in high (8 bits) and low (248 bits) parts.
    h = src[3] >> 56;
    l = src[3] & 0x00FFFFFFFFFFFFFF;

    // 5*2^248 = 1 mod q; hence, we add floor(h/5) + (h mod 5)*2^248
    // to the low part.
    quo = (h * 0xCD) >> 10;
    rem = h - (5 * quo);
    cc = add_carry(0, src[0], quo, &out[0]);
    cc = add_carry(cc, src[1], 0, &out[1]);
    cc = add_carry(cc, src[2], 0, &out[2]);
    (void)add_carry(cc, l, rem << 56, &out[3]);
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
    return (spint)buf[0] | ((spint)buf[1] << 8) | ((spint)buf[2] << 16) | ((spint)buf[3] << 24) |
           ((spint)buf[4] << 32) | ((spint)buf[5] << 40) | ((spint)buf[6] << 48) | ((spint)buf[7] << 56);
}

void
fp_decode_reduce(fp_t *d, const void *src, size_t len)
{
    uint64_t t[4];   // Stores Nbytes * 8 bits
    uint8_t tmp[32]; // Nbytes
    const uint8_t *b = src;

    fp_set_zero(d);
    if (len == 0) {
        return;
    }

    size_t rem = len % 32;
    if (rem != 0) {
        // Input size is not a multiple of 32, we decode a partial
        // block, which is already less than 2^248.
        size_t k = len - rem;
        memcpy(tmp, b + k, len - k);
        memset(tmp + len - k, 0, (sizeof tmp) - (len - k));
        fp_decode(d, tmp);
        len = k;
    }
    // Process all remaining blocks, in descending address order.
    while (len > 0) {
        fp_mul(d, d, &R2);
        len -= 32;
        t[0] = dec64le(b + len);
        t[1] = dec64le(b + len + 8);
        t[2] = dec64le(b + len + 16);
        t[3] = dec64le(b + len + 24);
        partial_reduce(t, t);
        enc64le(tmp, t[0]);
        enc64le(tmp + 8, t[1]);
        enc64le(tmp + 16, t[2]);
        enc64le(tmp + 24, t[3]);
        fp_t a;
        fp_decode(&a, tmp);
        fp_add(d, d, &a);
    }
}

#endif // RADIX_64
