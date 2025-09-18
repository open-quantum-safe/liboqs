// clang-format off
// Command line : python monty.py 32
// 0x40ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
#include <stdint.h>
#include <stdio.h>

#ifdef RADIX_32

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 14
#define Radix 28
#define Nbits 383
#define Nbytes 48

#define MONTGOMERY
// propagate carries
inline static spint prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 28u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 28u;
  n[0] &= mask;
  for (i = 1; i < 13; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 28u;
  }
  n[13] += (spint)carry;
  return -((n[13] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
inline static int flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[13] += ((spint)0x41000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] += (spint)1u;
  n[13] -= (spint)0x41000u;
  return flatten(n);
}

// Modular addition - reduce less than 2p
static void modadd(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] + b[0];
  n[1] = a[1] + b[1];
  n[2] = a[2] + b[2];
  n[3] = a[3] + b[3];
  n[4] = a[4] + b[4];
  n[5] = a[5] + b[5];
  n[6] = a[6] + b[6];
  n[7] = a[7] + b[7];
  n[8] = a[8] + b[8];
  n[9] = a[9] + b[9];
  n[10] = a[10] + b[10];
  n[11] = a[11] + b[11];
  n[12] = a[12] + b[12];
  n[13] = a[13] + b[13];
  n[0] += (spint)2u;
  n[13] -= (spint)0x82000u;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[13] += ((spint)0x82000u) & carry;
  (void)prop(n);
}

// Modular subtraction - reduce less than 2p
static void modsub(const spint *a, const spint *b, spint *n) {
  spint carry;
  n[0] = a[0] - b[0];
  n[1] = a[1] - b[1];
  n[2] = a[2] - b[2];
  n[3] = a[3] - b[3];
  n[4] = a[4] - b[4];
  n[5] = a[5] - b[5];
  n[6] = a[6] - b[6];
  n[7] = a[7] - b[7];
  n[8] = a[8] - b[8];
  n[9] = a[9] - b[9];
  n[10] = a[10] - b[10];
  n[11] = a[11] - b[11];
  n[12] = a[12] - b[12];
  n[13] = a[13] - b[13];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[13] += ((spint)0x82000u) & carry;
  (void)prop(n);
}

// Modular negation
static void modneg(const spint *b, spint *n) {
  spint carry;
  n[0] = (spint)0 - b[0];
  n[1] = (spint)0 - b[1];
  n[2] = (spint)0 - b[2];
  n[3] = (spint)0 - b[3];
  n[4] = (spint)0 - b[4];
  n[5] = (spint)0 - b[5];
  n[6] = (spint)0 - b[6];
  n[7] = (spint)0 - b[7];
  n[8] = (spint)0 - b[8];
  n[9] = (spint)0 - b[9];
  n[10] = (spint)0 - b[10];
  n[11] = (spint)0 - b[11];
  n[12] = (spint)0 - b[12];
  n[13] = (spint)0 - b[13];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[13] += ((spint)0x82000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1008877845989814286
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p13 = 0x41000u;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  t += (dpint)a[0] * b[0];
  spint v0 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[1];
  t += (dpint)a[1] * b[0];
  spint v1 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[2];
  t += (dpint)a[1] * b[1];
  t += (dpint)a[2] * b[0];
  spint v2 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[3];
  t += (dpint)a[1] * b[2];
  t += (dpint)a[2] * b[1];
  t += (dpint)a[3] * b[0];
  spint v3 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[4];
  t += (dpint)a[1] * b[3];
  t += (dpint)a[2] * b[2];
  t += (dpint)a[3] * b[1];
  t += (dpint)a[4] * b[0];
  spint v4 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[5];
  t += (dpint)a[1] * b[4];
  t += (dpint)a[2] * b[3];
  t += (dpint)a[3] * b[2];
  t += (dpint)a[4] * b[1];
  t += (dpint)a[5] * b[0];
  spint v5 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[6];
  t += (dpint)a[1] * b[5];
  t += (dpint)a[2] * b[4];
  t += (dpint)a[3] * b[3];
  t += (dpint)a[4] * b[2];
  t += (dpint)a[5] * b[1];
  t += (dpint)a[6] * b[0];
  spint v6 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[7];
  t += (dpint)a[1] * b[6];
  t += (dpint)a[2] * b[5];
  t += (dpint)a[3] * b[4];
  t += (dpint)a[4] * b[3];
  t += (dpint)a[5] * b[2];
  t += (dpint)a[6] * b[1];
  t += (dpint)a[7] * b[0];
  spint v7 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[8];
  t += (dpint)a[1] * b[7];
  t += (dpint)a[2] * b[6];
  t += (dpint)a[3] * b[5];
  t += (dpint)a[4] * b[4];
  t += (dpint)a[5] * b[3];
  t += (dpint)a[6] * b[2];
  t += (dpint)a[7] * b[1];
  t += (dpint)a[8] * b[0];
  spint v8 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[9];
  t += (dpint)a[1] * b[8];
  t += (dpint)a[2] * b[7];
  t += (dpint)a[3] * b[6];
  t += (dpint)a[4] * b[5];
  t += (dpint)a[5] * b[4];
  t += (dpint)a[6] * b[3];
  t += (dpint)a[7] * b[2];
  t += (dpint)a[8] * b[1];
  t += (dpint)a[9] * b[0];
  spint v9 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[10];
  t += (dpint)a[1] * b[9];
  t += (dpint)a[2] * b[8];
  t += (dpint)a[3] * b[7];
  t += (dpint)a[4] * b[6];
  t += (dpint)a[5] * b[5];
  t += (dpint)a[6] * b[4];
  t += (dpint)a[7] * b[3];
  t += (dpint)a[8] * b[2];
  t += (dpint)a[9] * b[1];
  t += (dpint)a[10] * b[0];
  spint v10 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[11];
  t += (dpint)a[1] * b[10];
  t += (dpint)a[2] * b[9];
  t += (dpint)a[3] * b[8];
  t += (dpint)a[4] * b[7];
  t += (dpint)a[5] * b[6];
  t += (dpint)a[6] * b[5];
  t += (dpint)a[7] * b[4];
  t += (dpint)a[8] * b[3];
  t += (dpint)a[9] * b[2];
  t += (dpint)a[10] * b[1];
  t += (dpint)a[11] * b[0];
  spint v11 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[12];
  t += (dpint)a[1] * b[11];
  t += (dpint)a[2] * b[10];
  t += (dpint)a[3] * b[9];
  t += (dpint)a[4] * b[8];
  t += (dpint)a[5] * b[7];
  t += (dpint)a[6] * b[6];
  t += (dpint)a[7] * b[5];
  t += (dpint)a[8] * b[4];
  t += (dpint)a[9] * b[3];
  t += (dpint)a[10] * b[2];
  t += (dpint)a[11] * b[1];
  t += (dpint)a[12] * b[0];
  spint v12 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[13];
  t += (dpint)a[1] * b[12];
  t += (dpint)a[2] * b[11];
  t += (dpint)a[3] * b[10];
  t += (dpint)a[4] * b[9];
  t += (dpint)a[5] * b[8];
  t += (dpint)a[6] * b[7];
  t += (dpint)a[7] * b[6];
  t += (dpint)a[8] * b[5];
  t += (dpint)a[9] * b[4];
  t += (dpint)a[10] * b[3];
  t += (dpint)a[11] * b[2];
  t += (dpint)a[12] * b[1];
  t += (dpint)a[13] * b[0];
  t += (dpint)v0 * (dpint)p13;
  spint v13 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[1] * b[13];
  t += (dpint)a[2] * b[12];
  t += (dpint)a[3] * b[11];
  t += (dpint)a[4] * b[10];
  t += (dpint)a[5] * b[9];
  t += (dpint)a[6] * b[8];
  t += (dpint)a[7] * b[7];
  t += (dpint)a[8] * b[6];
  t += (dpint)a[9] * b[5];
  t += (dpint)a[10] * b[4];
  t += (dpint)a[11] * b[3];
  t += (dpint)a[12] * b[2];
  t += (dpint)a[13] * b[1];
  t += (dpint)v1 * (dpint)p13;
  c[0] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[2] * b[13];
  t += (dpint)a[3] * b[12];
  t += (dpint)a[4] * b[11];
  t += (dpint)a[5] * b[10];
  t += (dpint)a[6] * b[9];
  t += (dpint)a[7] * b[8];
  t += (dpint)a[8] * b[7];
  t += (dpint)a[9] * b[6];
  t += (dpint)a[10] * b[5];
  t += (dpint)a[11] * b[4];
  t += (dpint)a[12] * b[3];
  t += (dpint)a[13] * b[2];
  t += (dpint)v2 * (dpint)p13;
  c[1] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[3] * b[13];
  t += (dpint)a[4] * b[12];
  t += (dpint)a[5] * b[11];
  t += (dpint)a[6] * b[10];
  t += (dpint)a[7] * b[9];
  t += (dpint)a[8] * b[8];
  t += (dpint)a[9] * b[7];
  t += (dpint)a[10] * b[6];
  t += (dpint)a[11] * b[5];
  t += (dpint)a[12] * b[4];
  t += (dpint)a[13] * b[3];
  t += (dpint)v3 * (dpint)p13;
  c[2] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[4] * b[13];
  t += (dpint)a[5] * b[12];
  t += (dpint)a[6] * b[11];
  t += (dpint)a[7] * b[10];
  t += (dpint)a[8] * b[9];
  t += (dpint)a[9] * b[8];
  t += (dpint)a[10] * b[7];
  t += (dpint)a[11] * b[6];
  t += (dpint)a[12] * b[5];
  t += (dpint)a[13] * b[4];
  t += (dpint)v4 * (dpint)p13;
  c[3] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[5] * b[13];
  t += (dpint)a[6] * b[12];
  t += (dpint)a[7] * b[11];
  t += (dpint)a[8] * b[10];
  t += (dpint)a[9] * b[9];
  t += (dpint)a[10] * b[8];
  t += (dpint)a[11] * b[7];
  t += (dpint)a[12] * b[6];
  t += (dpint)a[13] * b[5];
  t += (dpint)v5 * (dpint)p13;
  c[4] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[6] * b[13];
  t += (dpint)a[7] * b[12];
  t += (dpint)a[8] * b[11];
  t += (dpint)a[9] * b[10];
  t += (dpint)a[10] * b[9];
  t += (dpint)a[11] * b[8];
  t += (dpint)a[12] * b[7];
  t += (dpint)a[13] * b[6];
  t += (dpint)v6 * (dpint)p13;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[13];
  t += (dpint)a[8] * b[12];
  t += (dpint)a[9] * b[11];
  t += (dpint)a[10] * b[10];
  t += (dpint)a[11] * b[9];
  t += (dpint)a[12] * b[8];
  t += (dpint)a[13] * b[7];
  t += (dpint)v7 * (dpint)p13;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[13];
  t += (dpint)a[9] * b[12];
  t += (dpint)a[10] * b[11];
  t += (dpint)a[11] * b[10];
  t += (dpint)a[12] * b[9];
  t += (dpint)a[13] * b[8];
  t += (dpint)v8 * (dpint)p13;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  t += (dpint)v9 * (dpint)p13;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  t += (dpint)v10 * (dpint)p13;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  t += (dpint)v11 * (dpint)p13;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  t += (dpint)v12 * (dpint)p13;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[13];
  t += (dpint)v13 * (dpint)p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
  c[13] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p13 = 0x41000u;
  spint q = ((spint)1 << 28u); // q is unsaturated radix
  spint mask = (spint)(q - (spint)1);
  tot = (udpint)a[0] * a[0];
  t = tot;
  spint v0 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[1];
  tot *= 2;
  t += tot;
  spint v1 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[2];
  tot *= 2;
  tot += (udpint)a[1] * a[1];
  t += tot;
  spint v2 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[3];
  tot += (udpint)a[1] * a[2];
  tot *= 2;
  t += tot;
  spint v3 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[4];
  tot += (udpint)a[1] * a[3];
  tot *= 2;
  tot += (udpint)a[2] * a[2];
  t += tot;
  spint v4 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[5];
  tot += (udpint)a[1] * a[4];
  tot += (udpint)a[2] * a[3];
  tot *= 2;
  t += tot;
  spint v5 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[6];
  tot += (udpint)a[1] * a[5];
  tot += (udpint)a[2] * a[4];
  tot *= 2;
  tot += (udpint)a[3] * a[3];
  t += tot;
  spint v6 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[7];
  tot += (udpint)a[1] * a[6];
  tot += (udpint)a[2] * a[5];
  tot += (udpint)a[3] * a[4];
  tot *= 2;
  t += tot;
  spint v7 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[8];
  tot += (udpint)a[1] * a[7];
  tot += (udpint)a[2] * a[6];
  tot += (udpint)a[3] * a[5];
  tot *= 2;
  tot += (udpint)a[4] * a[4];
  t += tot;
  spint v8 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[9];
  tot += (udpint)a[1] * a[8];
  tot += (udpint)a[2] * a[7];
  tot += (udpint)a[3] * a[6];
  tot += (udpint)a[4] * a[5];
  tot *= 2;
  t += tot;
  spint v9 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[10];
  tot += (udpint)a[1] * a[9];
  tot += (udpint)a[2] * a[8];
  tot += (udpint)a[3] * a[7];
  tot += (udpint)a[4] * a[6];
  tot *= 2;
  tot += (udpint)a[5] * a[5];
  t += tot;
  spint v10 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[11];
  tot += (udpint)a[1] * a[10];
  tot += (udpint)a[2] * a[9];
  tot += (udpint)a[3] * a[8];
  tot += (udpint)a[4] * a[7];
  tot += (udpint)a[5] * a[6];
  tot *= 2;
  t += tot;
  spint v11 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[12];
  tot += (udpint)a[1] * a[11];
  tot += (udpint)a[2] * a[10];
  tot += (udpint)a[3] * a[9];
  tot += (udpint)a[4] * a[8];
  tot += (udpint)a[5] * a[7];
  tot *= 2;
  tot += (udpint)a[6] * a[6];
  t += tot;
  spint v12 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[13];
  tot += (udpint)a[1] * a[12];
  tot += (udpint)a[2] * a[11];
  tot += (udpint)a[3] * a[10];
  tot += (udpint)a[4] * a[9];
  tot += (udpint)a[5] * a[8];
  tot += (udpint)a[6] * a[7];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p13;
  spint v13 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[1] * a[13];
  tot += (udpint)a[2] * a[12];
  tot += (udpint)a[3] * a[11];
  tot += (udpint)a[4] * a[10];
  tot += (udpint)a[5] * a[9];
  tot += (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  t += (udpint)v1 * p13;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[2] * a[13];
  tot += (udpint)a[3] * a[12];
  tot += (udpint)a[4] * a[11];
  tot += (udpint)a[5] * a[10];
  tot += (udpint)a[6] * a[9];
  tot += (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  t += (udpint)v2 * p13;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[3] * a[13];
  tot += (udpint)a[4] * a[12];
  tot += (udpint)a[5] * a[11];
  tot += (udpint)a[6] * a[10];
  tot += (udpint)a[7] * a[9];
  tot *= 2;
  tot += (udpint)a[8] * a[8];
  t += tot;
  t += (udpint)v3 * p13;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[4] * a[13];
  tot += (udpint)a[5] * a[12];
  tot += (udpint)a[6] * a[11];
  tot += (udpint)a[7] * a[10];
  tot += (udpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  t += (udpint)v4 * p13;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[5] * a[13];
  tot += (udpint)a[6] * a[12];
  tot += (udpint)a[7] * a[11];
  tot += (udpint)a[8] * a[10];
  tot *= 2;
  tot += (udpint)a[9] * a[9];
  t += tot;
  t += (udpint)v5 * p13;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[6] * a[13];
  tot += (udpint)a[7] * a[12];
  tot += (udpint)a[8] * a[11];
  tot += (udpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  t += (udpint)v6 * p13;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[7] * a[13];
  tot += (udpint)a[8] * a[12];
  tot += (udpint)a[9] * a[11];
  tot *= 2;
  tot += (udpint)a[10] * a[10];
  t += tot;
  t += (udpint)v7 * p13;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[8] * a[13];
  tot += (udpint)a[9] * a[12];
  tot += (udpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  t += (udpint)v8 * p13;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[9] * a[13];
  tot += (udpint)a[10] * a[12];
  tot *= 2;
  tot += (udpint)a[11] * a[11];
  t += tot;
  t += (udpint)v9 * p13;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[10] * a[13];
  tot += (udpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  t += (udpint)v10 * p13;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[11] * a[13];
  tot *= 2;
  tot += (udpint)a[12] * a[12];
  t += tot;
  t += (udpint)v11 * p13;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  t += (udpint)v12 * p13;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[13] * a[13];
  t += tot;
  t += (udpint)v13 * p13;
  c[12] = ((spint)t & mask);
  t >>= 28;
  c[13] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 14; i++) {
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
  spint x[14];
  spint t0[14];
  spint t1[14];
  spint t2[14];
  spint t3[14];
  spint t4[14];
  spint t5[14];
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
  spint s[14];
  spint t[14];
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
  const spint c[14] = {0xf13732fu, 0x3f03f03u, 0x3f03f0u,  0xf03f03fu,
                       0x3f03f03u, 0x3f03f0u,  0xf03f03fu, 0x3f03f03u,
                       0x3f03f0u,  0xf03f03fu, 0x3f03f03u, 0x3f03f0u,
                       0xf03f03fu, 0x14f03u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[14];
  c[0] = 1;
  for (i = 1; i < 14; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[14];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 14; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 28u) &
          (((c0 ^ (spint)1) - (spint)1) >> 28u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[14];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 14; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 28u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 14; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 14; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 14; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// Modular multiplication by an integer, c=a*b mod 2p
static void modmli(const spint *a, int b, spint *c) {
  spint t[14];
  modint(b, t);
  modmul(a, t, c);
}

// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[14];
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
  spint r = 0x5aa5a55au;
  c0 = (1 - b) + r;
  c1 = b + r;
  for (i = 0; i < 14; i++) {
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
  spint s[14];
  spint y[14];
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
  a[13] = ((a[13] << n)) | (a[12] >> (28u - n));
  for (i = 12; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) | (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 13; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[13] = a[13] >> n;
  return r;
}

// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[14], d[14];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 14; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
  }
  return eq;
}

// clang-format on
/******************************************************************************
 API functions calling generated code above
 ******************************************************************************/

#include <fp.h>

const fp_t ZERO = {{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }};
const fp_t ONE = {{
    0x000003f0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010000
}};
// Montgomery representation of 2^-1
static const fp_t TWO_INV = {{ 0x000001f8, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                               0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
                                               0x00000000, 0x00000000, 0x00000000, 0x00008000 }};
// Montgomery representation of 3^-1
static const fp_t THREE_INV = {{ 0x0aaaabfa, 0x0aaaaaaa, 0x0aaaaaaa, 0x0aaaaaaa, 0x0aaaaaaa,
                                                 0x0aaaaaaa, 0x0aaaaaaa, 0x0aaaaaaa, 0x0aaaaaaa, 0x0aaaaaaa,
                                                 0x0aaaaaaa, 0x0aaaaaaa, 0x0aaaaaaa, 0x00030aaa }};
// Montgomery representation of 2^384
static const fp_t R2 = {{ 0x003f1373, 0x0f03f03f, 0x03f03f03, 0x003f03f0, 0x0f03f03f,
                                          0x03f03f03, 0x003f03f0, 0x0f03f03f, 0x03f03f03, 0x003f03f0,
                                          0x0f03f03f, 0x03f03f03, 0x003f03f0, 0x0000c03f }};

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
    spint c[14];
    redc(a->fp, c);
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
    for (i = 0; i < 14; i++) {
        d->fp[i] = 0;
    }
    for (i = 47; i >= 0; i--) {
        modshl(8, d->fp);
        d->fp[0] += (spint)b[i];
    }
    res = (spint)-modfsb(d->fp);
    nres(d->fp, d->fp);
    // If the value was canonical then res = -1; otherwise, res = 0
    for (i = 0; i < 14; i++) {
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

    // Split value in high (8 bits) and low (376 bits) parts.
    h = src[11] >> 24;
    l = src[11] & 0x00FFFFFF;

    // 65*2^376 = 1 mod q; hence, we add floor(h/65) + (h mod 65)*2^376
    // to the low part.
    quo = (h * 0xFC1) >> 18;
    rem = h - (65 * quo);
    cc = add_carry(0, src[0], quo, &out[0]);
    cc = add_carry(cc, src[1], 0, &out[1]);
    cc = add_carry(cc, src[2], 0, &out[2]);
    cc = add_carry(cc, src[3], 0, &out[3]);
    cc = add_carry(cc, src[4], 0, &out[4]);
    cc = add_carry(cc, src[5], 0, &out[5]);
    cc = add_carry(cc, src[6], 0, &out[6]);
    cc = add_carry(cc, src[7], 0, &out[7]);
    cc = add_carry(cc, src[8], 0, &out[8]);
    cc = add_carry(cc, src[9], 0, &out[9]);
    cc = add_carry(cc, src[10], 0, &out[10]);
    (void)add_carry(cc, l, rem << 24, &out[11]);
}

// Little-endian encoding of a 32-bit integer.
static inline void
enc32le(void *dst, uint32_t x)
{
    uint8_t *buf = dst;
    buf[0] = (uint8_t)x;
    buf[1] = (uint8_t)(x >> 8);
    buf[2] = (uint8_t)(x >> 16);
    buf[3] = (uint8_t)(x >> 24);
}

// Little-endian decoding of a 32-bit integer.
static inline uint32_t
dec32le(const void *src)
{
    const uint8_t *buf = src;
    return (spint)buf[0] | ((spint)buf[1] << 8) | ((spint)buf[2] << 16) | ((spint)buf[3] << 24);
}

void
fp_decode_reduce(fp_t *d, const void *src, size_t len)
{
    uint32_t t[12];  // Stores Nbytes * 8 bits
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
        t[0] = dec32le(b + len);
        t[1] = dec32le(b + len + 4);
        t[2] = dec32le(b + len + 8);
        t[3] = dec32le(b + len + 12);
        t[4] = dec32le(b + len + 16);
        t[5] = dec32le(b + len + 20);
        t[6] = dec32le(b + len + 24);
        t[7] = dec32le(b + len + 28);
        t[8] = dec32le(b + len + 32);
        t[9] = dec32le(b + len + 36);
        t[10] = dec32le(b + len + 40);
        t[11] = dec32le(b + len + 44);
        partial_reduce(t, t);
        enc32le(tmp, t[0]);
        enc32le(tmp + 4, t[1]);
        enc32le(tmp + 8, t[2]);
        enc32le(tmp + 12, t[3]);
        enc32le(tmp + 16, t[4]);
        enc32le(tmp + 20, t[5]);
        enc32le(tmp + 24, t[6]);
        enc32le(tmp + 28, t[7]);
        enc32le(tmp + 32, t[8]);
        enc32le(tmp + 36, t[9]);
        enc32le(tmp + 40, t[10]);
        enc32le(tmp + 44, t[11]);
        fp_t a;
        fp_decode(&a, tmp);
        fp_add(d, d, &a);
    }
}

#endif /* RADIX_32 */
