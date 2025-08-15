// clang-format off
// Command line : python monty.py 64
// 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
#ifdef RADIX_64

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
#define udpint __uint128_t
#define dpint __uint128_t

#define Wordlength 64
#define Nlimbs 7
#define Radix 55
#define Nbits 383
#define Nbytes 48

#define MONTGOMERY
// propagate carries
inline static spint prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 55u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 55u;
  n[0] &= mask;
  for (i = 1; i < 6; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 55u;
  }
  n[6] += (spint)carry;
  return -((n[6] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
inline static int flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[6] += ((spint)0x10400000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
inline static int modfsb(spint *n) {
  n[0] += (spint)1u;
  n[6] -= (spint)0x10400000000000u;
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
  n[5] = a[5] + b[5];
  n[6] = a[6] + b[6];
  n[0] += (spint)2u;
  n[6] -= (spint)0x20800000000000u;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[6] += ((spint)0x20800000000000u) & carry;
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
  n[5] = a[5] - b[5];
  n[6] = a[6] - b[6];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[6] += ((spint)0x20800000000000u) & carry;
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
  n[5] = (spint)0 - b[5];
  n[6] = (spint)0 - b[6];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[6] += ((spint)0x20800000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 9251314080475062396111552646217735
// Modular multiplication, c=a*b mod 2p
inline static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p6 = 0x10400000000000u;
  spint q = ((spint)1 << 55u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  t += (dpint)a[0] * b[0];
  spint v0 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  spint v1 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  spint v2 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  spint v3 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  spint v4 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  spint v5 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  t += (dpint)v0 * (dpint)p6;
  spint v6 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  t += (dpint)v1 * (dpint)p6;
  c[0] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[2] * b[6];
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)a[6] * b[2];
  t += (dpint)v2 * (dpint)p6;
  c[1] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)v3 * (dpint)p6;
  c[2] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)v4 * (dpint)p6;
  c[3] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)v5 * (dpint)p6;
  c[4] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[6] * b[6];
  t += (dpint)v6 * (dpint)p6;
  c[5] = ((spint)t & mask);
  t >>= 55;
  c[6] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
inline static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p6 = 0x10400000000000u;
  spint q = ((spint)1 << 55u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  spint v1 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  spint v2 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  spint v3 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  spint v4 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  spint v5 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v0 * p6;
  spint v6 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v1 * p6;
  c[0] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  t += (udpint)v2 * p6;
  c[1] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  t += (udpint)v3 * p6;
  c[2] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  t += (udpint)v4 * p6;
  c[3] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  t += (udpint)v5 * p6;
  c[4] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[6] * a[6];
  t += tot;
  t += (udpint)v6 * p6;
  c[5] = ((spint)t & mask);
  t >>= 55;
  c[6] = (spint)t;
}

// copy
inline static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 7; i++) {
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
  spint x[7];
  spint t0[7];
  spint t1[7];
  spint t2[7];
  spint t3[7];
  spint t4[7];
  spint t5[7];
  modcpy(w, x);
  modsqr(x, z);
  modsqr(z, t0);
  modmul(x, t0, t1);
  modmul(z, t1, z);
  modsqr(z, t0);
  modsqr(t0, t3);
  modsqr(t3, t4);
  modsqr(t4, t2);
  modcpy(t2, t5);
  modnsqr(t5, 3);
  modmul(t2, t5, t2);
  modcpy(t2, t5);
  modnsqr(t5, 6);
  modmul(t2, t5, t2);
  modcpy(t2, t5);
  modnsqr(t5, 2);
  modmul(t4, t5, t5);
  modnsqr(t5, 13);
  modmul(t2, t5, t2);
  modcpy(t2, t5);
  modnsqr(t5, 2);
  modmul(t4, t5, t4);
  modnsqr(t4, 28);
  modmul(t2, t4, t2);
  modsqr(t2, t4);
  modmul(t3, t4, t3);
  modnsqr(t3, 59);
  modmul(t2, t3, t2);
  modmul(t1, t2, t1);
  modmul(z, t1, z);
  modmul(t0, z, t0);
  modmul(t1, t0, t1);
  modsqr(t1, t2);
  modmul(t1, t2, t2);
  modsqr(t2, t2);
  modmul(t1, t2, t2);
  modmul(t0, t2, t0);
  modmul(z, t0, z);
  modsqr(z, t2);
  modmul(z, t2, t2);
  modmul(t0, t2, t0);
  modmul(t1, t0, t1);
  modcpy(t1, t2);
  modnsqr(t2, 128);
  modmul(t1, t2, t1);
  modmul(t0, t1, t0);
  modnsqr(t0, 125);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[7];
  spint t[7];
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
  const spint c[7] = {0xfc0fc0fc0fc4du,  0x781f81f81f81f8u, 0x3f03f03f03f03u,
                      0x7e07e07e07e07eu, 0x40fc0fc0fc0fc0u, 0x1f81f81f81f81fu,
                      0xcff03f03f03f0u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[7];
  c[0] = 1;
  for (i = 1; i < 7; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[7];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 7; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 55u) &
          (((c0 ^ (spint)1) - (spint)1) >> 55u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[7];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 7; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 55u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 7; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 7; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 7; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Modular multiplication by an integer, c=a*b mod 2p
inline static void modmli(const spint *a, int b, spint *c) {
  spint t[7];
  modint(b, t);
  modmul(a, t, c);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[7];
  if (h == NULL) {
    modpro(x, r);
    modsqr(r, r);
  } else {
    modsqr(h, r);
  }
  modmul(r, x, r);
  return modis1(r) | modis0(x);
}

// conditional move g to f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void modcmv(int b, const spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t;
  spint r = 0x3cc3c33c5aa5a55au;
  c0 = (1 - b) + r;
  c1 = b + r;
  for (i = 0; i < 7; i++) {
    s = g[i];
    t = f[i];
    f[i] = c0 * t + c1 * s;
    f[i] -= r * (t + s);
  }
}

// conditional swap g and f if d=1
// strongly recommend inlining be disabled using compiler specific syntax
static void modcsw(int b, volatile spint *g, volatile spint *f) {
  int i;
  spint c0, c1, s, t, w;
  spint r = 0x3cc3c33c5aa5a55au;
  c0 = (1 - b) + r;
  c1 = b + r;
  for (i = 0; i < 7; i++) {
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
  spint s[7];
  spint y[7];
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
  a[6] = ((a[6] << n)) | (a[5] >> (55u - n));
  for (i = 5; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x7fffffffffffff) | (a[i - 1] >> (55u - n));
  }
  a[0] = (a[0] << n) & (spint)0x7fffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 6; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (55u - n)) & (spint)0x7fffffffffffff);
  }
  a[6] = a[6] >> n;
  return r;
}

// set a= 2^r
static void mod2r(unsigned int r, spint *a) {
  unsigned int n = r / 55u;
  unsigned int m = r % 55u;
  modzer(a);
  if (r >= 48 * 8)
    return;
  a[n] = 1;
  a[n] <<= m;
  nres(a, a);
}

// export to byte array
static void modexp(const spint *a, char *b) {
  int i;
  spint c[7];
  redc(a, c);
  for (i = 47; i >= 0; i--) {
    b[i] = c[0] & (spint)0xff;
    (void)modshr(8, c);
  }
}

// import from byte array
// returns 1 if in range, else 0
static int modimp(const char *b, spint *a) {
  int i, res;
  for (i = 0; i < 7; i++) {
    a[i] = 0;
  }
  for (i = 0; i < 48; i++) {
    modshl(8, a);
    a[0] += (spint)(unsigned char)b[i];
  }
  res = modfsb(a);
  nres(a, a);
  return res;
}

// determine sign
static int modsign(const spint *a) {
  spint c[7];
  redc(a, c);
  return c[0] % 2;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[7], d[7];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 7; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 55) & 1;
  }
  return eq;
}

// clang-format on
/******************************************************************************
 API functions calling generated code above
 ******************************************************************************/

#include <fp.h>

const digit_t ZERO[NWORDS_FIELD] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
const digit_t ONE[NWORDS_FIELD] = { 0x0000000000000007, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                    0x0000000000000000, 0x0000000000000000, 0x000e400000000000 };
// Montgomery representation of 2^-1
static const digit_t TWO_INV[NWORDS_FIELD] = { 0x0000000000000003, 0x0000000000000000, 0x0000000000000000,
                                               0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                               0x000f400000000000 };
// Montgomery representation of 3^-1
static const digit_t THREE_INV[NWORDS_FIELD] = { 0x0055555555555557, 0x002aaaaaaaaaaaaa, 0x0055555555555555,
                                                 0x002aaaaaaaaaaaaa, 0x0055555555555555, 0x002aaaaaaaaaaaaa,
                                                 0x000f955555555555 };
// Montgomery representation of 2^384
static const digit_t R2[NWORDS_FIELD] = { 0x0007e07e07e07e26, 0x007c0fc0fc0fc0fc, 0x0001f81f81f81f81,
                                          0x003f03f03f03f03f, 0x00607e07e07e07e0, 0x000fc0fc0fc0fc0f,
                                          0x000e9f81f81f81f8 };

void
fp_set_small(fp_t *x, const digit_t val)
{
    modint((int)val, *x);
}

void
fp_mul_small(fp_t *x, const fp_t *a, const uint32_t val)
{
    modmli(*a, (int)val, *x);
}

void
fp_set_zero(fp_t *x)
{
    modzer(*x);
}

void
fp_set_one(fp_t *x)
{
    modone(*x);
}

uint32_t
fp_is_equal(const fp_t *a, const fp_t *b)
{
    return -(uint32_t)modcmp(*a, *b);
}

uint32_t
fp_is_zero(const fp_t *a)
{
    return -(uint32_t)modis0(*a);
}

void
fp_copy(fp_t *out, const fp_t *a)
{
    modcpy(*a, *out);
}

void
fp_cswap(fp_t *a, fp_t *b, uint32_t ctl)
{
    modcsw((int)(ctl & 0x1), *a, *b);
}

void
fp_add(fp_t *out, const fp_t *a, const fp_t *b)
{
    modadd(*a, *b, *out);
}

void
fp_sub(fp_t *out, const fp_t *a, const fp_t *b)
{
    modsub(*a, *b, *out);
}

void
fp_neg(fp_t *out, const fp_t *a)
{
    modneg(*a, *out);
}

void
fp_sqr(fp_t *out, const fp_t *a)
{
    modsqr(*a, *out);
}

void
fp_mul(fp_t *out, const fp_t *a, const fp_t *b)
{
    modmul(*a, *b, *out);
}

void
fp_inv(fp_t *x)
{
    modinv(*x, NULL, *x);
}

uint32_t
fp_is_square(const fp_t *a)
{
    return -(uint32_t)modqr(NULL, *a);
}

void
fp_sqrt(fp_t *a)
{
    modsqrt(*a, NULL, *a);
}

void
fp_half(fp_t *out, const fp_t *a)
{
    modmul(TWO_INV, *a, *out);
}

void
fp_exp3div4(fp_t *out, const fp_t *a)
{
    modpro(*a, *out);
}

void
fp_div3(fp_t *out, const fp_t *a)
{
    modmul(THREE_INV, *a, *out);
}

void
fp_encode(void *dst, const fp_t *a)
{
    // Modified version of modexp()
    int i;
    spint c[7];
    redc(*a, c);
    for (i = 0; i < 48; i++) {
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
    for (i = 0; i < 7; i++) {
        (*d)[i] = 0;
    }
    for (i = 47; i >= 0; i--) {
        modshl(8, *d);
        (*d)[0] += (spint)b[i];
    }
    res = (spint)-modfsb(*d);
    nres(*d, *d);
    // If the value was canonical then res = -1; otherwise, res = 0
    for (i = 0; i < 7; i++) {
        (*d)[i] &= res;
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

    // Split value in high (8 bits) and low (376 bits) parts.
    h = src[5] >> 56;
    l = src[5] & 0x00FFFFFFFFFFFFFF;

    // 65*2^376 = 1 mod q; hence, we add floor(h/65) + (h mod 65)*2^376
    // to the low part.
    quo = (h * 0xFC1) >> 18;
    rem = h - (65 * quo);
    cc = add_carry(0, src[0], quo, &out[0]);
    cc = add_carry(cc, src[1], 0, &out[1]);
    cc = add_carry(cc, src[2], 0, &out[2]);
    cc = add_carry(cc, src[3], 0, &out[3]);
    cc = add_carry(cc, src[4], 0, &out[4]);
    (void)add_carry(cc, l, rem << 56, &out[5]);
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
    uint64_t t[6];   // Stores Nbytes * 8 bits
    uint8_t tmp[48]; // Nbytes
    const uint8_t *b = src;

    fp_set_zero(d);
    if (len == 0) {
        return;
    }

    size_t rem = len % 48;
    if (rem != 0) {
        // Input size is not a multiple of 48, we decode a partial
        // block, which is already less than 2^376.
        size_t k = len - rem;
        memcpy(tmp, b + k, len - k);
        memset(tmp + len - k, 0, (sizeof tmp) - (len - k));
        fp_decode(d, tmp);
        len = k;
    }
    // Process all remaining blocks, in descending address order.
    while (len > 0) {
        fp_mul(d, d, &R2);
        len -= 48;
        t[0] = dec64le(b + len);
        t[1] = dec64le(b + len + 8);
        t[2] = dec64le(b + len + 16);
        t[3] = dec64le(b + len + 24);
        t[4] = dec64le(b + len + 32);
        t[5] = dec64le(b + len + 40);
        partial_reduce(t, t);
        enc64le(tmp, t[0]);
        enc64le(tmp + 8, t[1]);
        enc64le(tmp + 16, t[2]);
        enc64le(tmp + 24, t[3]);
        enc64le(tmp + 32, t[4]);
        enc64le(tmp + 40, t[5]);
        fp_t a;
        fp_decode(&a, tmp);
        fp_add(d, d, &a);
    }
}

#endif /* RADIX_64 */
