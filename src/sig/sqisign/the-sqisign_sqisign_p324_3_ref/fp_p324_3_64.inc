// clang-format off
// Automatically generated modular arithmetic C code
// Command line : python /Users/bhe/git/modarith/monty.py 64
// 0x2fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdint.h>
#include <stdio.h>

#define sspint int64_t
#define spint uint64_t
// for ISO-C compatibility
__extension__ typedef __uint128_t fp_u128_t;
#define udpint fp_u128_t
#define dpint fp_u128_t

#define Wordlength 64
#define Nlimbs 6
#define Radix 55
#define Nbits 326
#define Nbytes 41

#define MONTGOMERY
// propagate carries
static inline spint prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 55u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 55u;
  n[0] &= mask;
  for (i = 1; i < 5; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 55u;
  }
  n[5] += (spint)carry;
  return -((n[5] >> 1) >> 62u);
}

// propagate carries and add p if negative, propagate carries again
static inline int flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[5] += ((spint)0x6000000000000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static inline int modfsb(spint *n) {
  n[0] += (spint)1u;
  n[5] -= (spint)0x6000000000000u;
  return flatten(n);
}

// Modular addition - reduce less than 2p
static inline void modadd(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[5] = a[5] + b[5];
  n[0] += (spint)2u;
  n[5] -= (spint)0xc000000000000u;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[5] += ((spint)0xc000000000000u) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static inline void modsub(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] - b[0];
  n[1] = a[1] - b[1];
  n[2] = a[2] - b[2];
  n[3] = a[3] - b[3];
  n[4] = a[4] - b[4];
  n[5] = a[5] - b[5];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[5] += ((spint)0xc000000000000u) & carry;
  (void)prop(n);
}

// Modular negation
static inline void modneg(const spint *b, spint *n) {
  spint carry;
  n[0] = (spint)0 - b[0];
  n[1] = (spint)0 - b[1];
  n[2] = (spint)0 - b[2];
  n[3] = (spint)0 - b[3];
  n[4] = (spint)0 - b[4];
  n[5] = (spint)0 - b[5];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[5] += ((spint)0xc000000000000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 340282366920938463463374607431768211456
// maximum possible = 7849292516622640752998911450284038
// Modular multiplication, c=a*b mod 2p
static inline void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p5 = 0x6000000000000u;
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
  t += (dpint)v0 * (dpint)p5;
  spint v5 = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)v1 * (dpint)p5;
  c[0] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)v2 * (dpint)p5;
  c[1] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)v3 * (dpint)p5;
  c[2] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)v4 * (dpint)p5;
  c[3] = ((spint)t & mask);
  t >>= 55;
  t += (dpint)a[5] * b[5];
  t += (dpint)v5 * (dpint)p5;
  c[4] = ((spint)t & mask);
  t >>= 55;
  c[5] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static inline void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p5 = 0x6000000000000u;
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
  t += (udpint)v0 * p5;
  spint v5 = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  t += (udpint)v1 * p5;
  c[0] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  t += (udpint)v2 * p5;
  c[1] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  t += (udpint)v3 * p5;
  c[2] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  t += (udpint)v4 * p5;
  c[3] = ((spint)t & mask);
  t >>= 55;
  tot = (udpint)a[5] * a[5];
  t += tot;
  t += (udpint)v5 * p5;
  c[4] = ((spint)t & mask);
  t >>= 55;
  c[5] = (spint)t;
}

// copy
static inline void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 6; i++) {
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
  spint x[6];
  spint t0[6];
  spint t1[6];
  spint t2[6];
  spint t3[6];
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, z);
  modcpy(z, t0);
  modnsqr(t0, 2);
  modmul(z, t0, t0);
  modcpy(t0, t1);
  modnsqr(t1, 4);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 2);
  modmul(z, t1, z);
  modcpy(z, t2);
  modnsqr(t2, 4);
  modcpy(t2, t1);
  modnsqr(t1, 4);
  modcpy(t1, t3);
  modnsqr(t3, 10);
  modmul(t1, t3, t1);
  modcpy(t1, t3);
  modnsqr(t3, 6);
  modmul(t2, t3, t2);
  modnsqr(t2, 24);
  modmul(t1, t2, t1);
  modmul(t0, t1, t0);
  modcpy(t0, t1);
  modnsqr(t1, 10);
  modmul(z, t1, z);
  modcpy(z, t1);
  modnsqr(t1, 58);
  modmul(t0, t1, t1);
  modmul(x, t1, t0);
  modsqr(t0, t2);
  modmul(t1, t2, t1);
  modcpy(t1, t2);
  modnsqr(t2, 128);
  modmul(t1, t2, t1);
  modmul(t0, t1, t0);
  modnsqr(t0, 68);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[6];
  spint t[6];
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
  const spint c[6] = {0x5555555555571cu, 0x2aaaaaaaaaaaaau, 0x55555555555555u,
                      0x2aaaaaaaaaaaaau, 0x55555555555555u, 0x2aaaaaaaaaaaau};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[6];
  c[0] = 1;
  for (i = 1; i < 6; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[6];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 6; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 55u) &
          (((c0 ^ (spint)1) - (spint)1) >> 55u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[6];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 6; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 55u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 6; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 6; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 6; i++) {
    a[i] = 0;
  }
  nres(a, a);
}


// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[6];
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
static void __attribute__((noinline)) modcsw(int b, volatile spint *g,
                                             volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, v, aux;
  static spint R = 0;
  R += 0x3cc3c33c5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 6; i++) {
    s = g[i];
    t = f[i];
    v = w * (t + s);
    f[i] = aux = c0 * t + c1 * s;
    f[i] = aux - v;
    g[i] = aux = c0 * s + c1 * t;
    g[i] = aux - v;
  }
}

// Modular square root, provide progenitor h if available, NULL if not
static void modsqrt(const spint *x, const spint *h, spint *r) {
  spint s[6];
  spint y[6];
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
  a[5] = ((a[5] << n)) | (a[4] >> (55u - n));
  for (i = 4; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0x7fffffffffffff) | (a[i - 1] >> (55u - n));
  }
  a[0] = (a[0] << n) & (spint)0x7fffffffffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 5; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (55u - n)) & (spint)0x7fffffffffffff);
  }
  a[5] = a[5] >> n;
  return r;
}





// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[6], d[6];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 6; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 55) & 1;
  }
  return eq;
}

/******************************************************************************
 * SQIsign fp_* API wrappers over the modarith code above.
 * Auto-generated by scripts/add_parameter_set.py — generic for any prime
 * with p == 3 (mod 4). Do not hand-edit; re-run the generator instead.
 ******************************************************************************/

#include <fp.h>

const fp_t ZERO = {
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0
};

const fp_t ONE = {
    0x15,
    0x0,
    0x0,
    0x0,
    0x0,
    0x2000000000000
};

// Montgomery representation of 2^-1
static const fp_t TWO_INV = {
    0xa,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4000000000000
};

// Montgomery representation of 3^-1
static const fp_t THREE_INV = {
    0x5555555555555c,
    0x2aaaaaaaaaaaaa,
    0x55555555555555,
    0x2aaaaaaaaaaaaa,
    0x55555555555555,
    0x2aaaaaaaaaaaa
};

void
fp_set_small(fp_t *x, const digit_t val)
{
    modint((int)val, x->fp);
}

void
fp_mul_small(fp_t *x, const fp_t *a, const uint32_t val)
{
    // via modmul (radix-independent; avoids modmli small-multiplier limits)
    fp_t t;
    modint((int)val, t.fp);
    modmul(a->fp, t.fp, x->fp);
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
    // p == 3 (mod 4)  =>  modpro computes a^((p-3)/4)
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
    // little-endian canonical encoding
    int i;
    spint c[6];
    unsigned char *out = dst;
    redc(a->fp, c);
    for (i = 0; i < 41; i++) {
        out[i] = (unsigned char)(c[0] & (spint)0xff);
        (void)modshr(8, c);
    }
}

uint32_t
fp_decode(fp_t *d, const void *src)
{
    // inverse of fp_encode; returns 0xFFFFFFFF iff the input was canonical
    int i;
    spint res;
    const unsigned char *b = src;
    for (i = 0; i < 6; i++) {
        d->fp[i] = 0;
    }
    for (i = 41 - 1; i >= 0; i--) {
        modshl(8, d->fp);
        d->fp[0] += (spint)b[i];
    }
    res = (spint)-modfsb(d->fp);
    nres(d->fp, d->fp);
    for (i = 0; i < 6; i++) {
        d->fp[i] &= res;
    }
    return (uint32_t)res;
}

void
fp_decode_reduce(fp_t *d, const void *src, size_t len)
{
    // Reduce a little-endian byte string of arbitrary length mod p.
    // Radix-256 Horner using only fp_add/fp_set_small: correct for ANY prime,
    // no prime-shape-specific fast reduction required.
    const unsigned char *b = src;
    size_t i;
    fp_set_zero(d);
    for (i = len; i-- > 0;) {
        fp_t t;
        int k;
        for (k = 0; k < 8; k++) {
            fp_add(d, d, d); // d *= 2  (eight times => d *= 256)
        }
        fp_set_small(&t, (digit_t)b[i]);
        fp_add(d, d, &t);
    }
}
