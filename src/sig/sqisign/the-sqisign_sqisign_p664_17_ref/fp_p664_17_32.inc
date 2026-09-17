// clang-format off
// Automatically generated modular arithmetic C code
// Command line : python /Users/bhe/git/modarith/monty.py 32
// 0x10ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
// Python Script by Mike Scott (Technology Innovation Institute, UAE, 2025)

#include <stdint.h>
#include <stdio.h>

#define sspint int32_t
#define spint uint32_t
#define udpint uint64_t
#define dpint uint64_t

#define Wordlength 32
#define Nlimbs 24
#define Radix 28
#define Nbits 669
#define Nbytes 84

#define MONTGOMERY
// propagate carries
static inline spint prop(spint *n) {
  int i;
  spint mask = ((spint)1 << 28u) - (spint)1;
  sspint carry = (sspint)n[0];
  carry >>= 28u;
  n[0] &= mask;
  for (i = 1; i < 23; i++) {
    carry += (sspint)n[i];
    n[i] = (spint)carry & mask;
    carry >>= 28u;
  }
  n[23] += (spint)carry;
  return -((n[23] >> 1) >> 30u);
}

// propagate carries and add p if negative, propagate carries again
static inline int flatten(spint *n) {
  spint carry = prop(n);
  n[0] -= (spint)1u & carry;
  n[23] += ((spint)0x1100000u) & carry;
  (void)prop(n);
  return (int)(carry & 1);
}

// Montgomery final subtract
static int modfsb(spint *n) {
  n[0] += (spint)1u;
  n[23] -= (spint)0x1100000u;
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
  n[14] = a[14] + b[14];
  n[15] = a[15] + b[15];
  n[16] = a[16] + b[16];
  n[17] = a[17] + b[17];
  n[18] = a[18] + b[18];
  n[19] = a[19] + b[19];
  n[20] = a[20] + b[20];
  n[21] = a[21] + b[21];
  n[22] = a[22] + b[22];
  n[23] = a[23] + b[23];
  n[0] += (spint)2u;
  n[23] -= (spint)0x2200000u;
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[23] += ((spint)0x2200000u) & carry;
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
  n[14] = a[14] - b[14];
  n[15] = a[15] - b[15];
  n[16] = a[16] - b[16];
  n[17] = a[17] - b[17];
  n[18] = a[18] - b[18];
  n[19] = a[19] - b[19];
  n[20] = a[20] - b[20];
  n[21] = a[21] - b[21];
  n[22] = a[22] - b[22];
  n[23] = a[23] - b[23];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[23] += ((spint)0x2200000u) & carry;
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
  n[14] = (spint)0 - b[14];
  n[15] = (spint)0 - b[15];
  n[16] = (spint)0 - b[16];
  n[17] = (spint)0 - b[17];
  n[18] = (spint)0 - b[18];
  n[19] = (spint)0 - b[19];
  n[20] = (spint)0 - b[20];
  n[21] = (spint)0 - b[21];
  n[22] = (spint)0 - b[22];
  n[23] = (spint)0 - b[23];
  carry = prop(n);
  n[0] -= (spint)2u & carry;
  n[23] += ((spint)0x2200000u) & carry;
  (void)prop(n);
}

// Overflow limit   = 18446744073709551616
// maximum possible = 1734167387331100696
// Modular multiplication, c=a*b mod 2p
static void modmul(const spint *a, const spint *b, spint *c) {
  dpint t = 0;
  spint p23 = 0x1100000u;
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
  spint v13 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[14];
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
  t += (dpint)a[14] * b[0];
  spint v14 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[15];
  t += (dpint)a[1] * b[14];
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
  t += (dpint)a[14] * b[1];
  t += (dpint)a[15] * b[0];
  spint v15 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[16];
  t += (dpint)a[1] * b[15];
  t += (dpint)a[2] * b[14];
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
  t += (dpint)a[14] * b[2];
  t += (dpint)a[15] * b[1];
  t += (dpint)a[16] * b[0];
  spint v16 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[17];
  t += (dpint)a[1] * b[16];
  t += (dpint)a[2] * b[15];
  t += (dpint)a[3] * b[14];
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
  t += (dpint)a[14] * b[3];
  t += (dpint)a[15] * b[2];
  t += (dpint)a[16] * b[1];
  t += (dpint)a[17] * b[0];
  spint v17 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[18];
  t += (dpint)a[1] * b[17];
  t += (dpint)a[2] * b[16];
  t += (dpint)a[3] * b[15];
  t += (dpint)a[4] * b[14];
  t += (dpint)a[5] * b[13];
  t += (dpint)a[6] * b[12];
  t += (dpint)a[7] * b[11];
  t += (dpint)a[8] * b[10];
  t += (dpint)a[9] * b[9];
  t += (dpint)a[10] * b[8];
  t += (dpint)a[11] * b[7];
  t += (dpint)a[12] * b[6];
  t += (dpint)a[13] * b[5];
  t += (dpint)a[14] * b[4];
  t += (dpint)a[15] * b[3];
  t += (dpint)a[16] * b[2];
  t += (dpint)a[17] * b[1];
  t += (dpint)a[18] * b[0];
  spint v18 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[19];
  t += (dpint)a[1] * b[18];
  t += (dpint)a[2] * b[17];
  t += (dpint)a[3] * b[16];
  t += (dpint)a[4] * b[15];
  t += (dpint)a[5] * b[14];
  t += (dpint)a[6] * b[13];
  t += (dpint)a[7] * b[12];
  t += (dpint)a[8] * b[11];
  t += (dpint)a[9] * b[10];
  t += (dpint)a[10] * b[9];
  t += (dpint)a[11] * b[8];
  t += (dpint)a[12] * b[7];
  t += (dpint)a[13] * b[6];
  t += (dpint)a[14] * b[5];
  t += (dpint)a[15] * b[4];
  t += (dpint)a[16] * b[3];
  t += (dpint)a[17] * b[2];
  t += (dpint)a[18] * b[1];
  t += (dpint)a[19] * b[0];
  spint v19 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[20];
  t += (dpint)a[1] * b[19];
  t += (dpint)a[2] * b[18];
  t += (dpint)a[3] * b[17];
  t += (dpint)a[4] * b[16];
  t += (dpint)a[5] * b[15];
  t += (dpint)a[6] * b[14];
  t += (dpint)a[7] * b[13];
  t += (dpint)a[8] * b[12];
  t += (dpint)a[9] * b[11];
  t += (dpint)a[10] * b[10];
  t += (dpint)a[11] * b[9];
  t += (dpint)a[12] * b[8];
  t += (dpint)a[13] * b[7];
  t += (dpint)a[14] * b[6];
  t += (dpint)a[15] * b[5];
  t += (dpint)a[16] * b[4];
  t += (dpint)a[17] * b[3];
  t += (dpint)a[18] * b[2];
  t += (dpint)a[19] * b[1];
  t += (dpint)a[20] * b[0];
  spint v20 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[21];
  t += (dpint)a[1] * b[20];
  t += (dpint)a[2] * b[19];
  t += (dpint)a[3] * b[18];
  t += (dpint)a[4] * b[17];
  t += (dpint)a[5] * b[16];
  t += (dpint)a[6] * b[15];
  t += (dpint)a[7] * b[14];
  t += (dpint)a[8] * b[13];
  t += (dpint)a[9] * b[12];
  t += (dpint)a[10] * b[11];
  t += (dpint)a[11] * b[10];
  t += (dpint)a[12] * b[9];
  t += (dpint)a[13] * b[8];
  t += (dpint)a[14] * b[7];
  t += (dpint)a[15] * b[6];
  t += (dpint)a[16] * b[5];
  t += (dpint)a[17] * b[4];
  t += (dpint)a[18] * b[3];
  t += (dpint)a[19] * b[2];
  t += (dpint)a[20] * b[1];
  t += (dpint)a[21] * b[0];
  spint v21 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[22];
  t += (dpint)a[1] * b[21];
  t += (dpint)a[2] * b[20];
  t += (dpint)a[3] * b[19];
  t += (dpint)a[4] * b[18];
  t += (dpint)a[5] * b[17];
  t += (dpint)a[6] * b[16];
  t += (dpint)a[7] * b[15];
  t += (dpint)a[8] * b[14];
  t += (dpint)a[9] * b[13];
  t += (dpint)a[10] * b[12];
  t += (dpint)a[11] * b[11];
  t += (dpint)a[12] * b[10];
  t += (dpint)a[13] * b[9];
  t += (dpint)a[14] * b[8];
  t += (dpint)a[15] * b[7];
  t += (dpint)a[16] * b[6];
  t += (dpint)a[17] * b[5];
  t += (dpint)a[18] * b[4];
  t += (dpint)a[19] * b[3];
  t += (dpint)a[20] * b[2];
  t += (dpint)a[21] * b[1];
  t += (dpint)a[22] * b[0];
  spint v22 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[0] * b[23];
  t += (dpint)a[1] * b[22];
  t += (dpint)a[2] * b[21];
  t += (dpint)a[3] * b[20];
  t += (dpint)a[4] * b[19];
  t += (dpint)a[5] * b[18];
  t += (dpint)a[6] * b[17];
  t += (dpint)a[7] * b[16];
  t += (dpint)a[8] * b[15];
  t += (dpint)a[9] * b[14];
  t += (dpint)a[10] * b[13];
  t += (dpint)a[11] * b[12];
  t += (dpint)a[12] * b[11];
  t += (dpint)a[13] * b[10];
  t += (dpint)a[14] * b[9];
  t += (dpint)a[15] * b[8];
  t += (dpint)a[16] * b[7];
  t += (dpint)a[17] * b[6];
  t += (dpint)a[18] * b[5];
  t += (dpint)a[19] * b[4];
  t += (dpint)a[20] * b[3];
  t += (dpint)a[21] * b[2];
  t += (dpint)a[22] * b[1];
  t += (dpint)a[23] * b[0];
  t += (dpint)v0 * (dpint)p23;
  spint v23 = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[1] * b[23];
  t += (dpint)a[2] * b[22];
  t += (dpint)a[3] * b[21];
  t += (dpint)a[4] * b[20];
  t += (dpint)a[5] * b[19];
  t += (dpint)a[6] * b[18];
  t += (dpint)a[7] * b[17];
  t += (dpint)a[8] * b[16];
  t += (dpint)a[9] * b[15];
  t += (dpint)a[10] * b[14];
  t += (dpint)a[11] * b[13];
  t += (dpint)a[12] * b[12];
  t += (dpint)a[13] * b[11];
  t += (dpint)a[14] * b[10];
  t += (dpint)a[15] * b[9];
  t += (dpint)a[16] * b[8];
  t += (dpint)a[17] * b[7];
  t += (dpint)a[18] * b[6];
  t += (dpint)a[19] * b[5];
  t += (dpint)a[20] * b[4];
  t += (dpint)a[21] * b[3];
  t += (dpint)a[22] * b[2];
  t += (dpint)a[23] * b[1];
  t += (dpint)v1 * (dpint)p23;
  c[0] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[2] * b[23];
  t += (dpint)a[3] * b[22];
  t += (dpint)a[4] * b[21];
  t += (dpint)a[5] * b[20];
  t += (dpint)a[6] * b[19];
  t += (dpint)a[7] * b[18];
  t += (dpint)a[8] * b[17];
  t += (dpint)a[9] * b[16];
  t += (dpint)a[10] * b[15];
  t += (dpint)a[11] * b[14];
  t += (dpint)a[12] * b[13];
  t += (dpint)a[13] * b[12];
  t += (dpint)a[14] * b[11];
  t += (dpint)a[15] * b[10];
  t += (dpint)a[16] * b[9];
  t += (dpint)a[17] * b[8];
  t += (dpint)a[18] * b[7];
  t += (dpint)a[19] * b[6];
  t += (dpint)a[20] * b[5];
  t += (dpint)a[21] * b[4];
  t += (dpint)a[22] * b[3];
  t += (dpint)a[23] * b[2];
  t += (dpint)v2 * (dpint)p23;
  c[1] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[3] * b[23];
  t += (dpint)a[4] * b[22];
  t += (dpint)a[5] * b[21];
  t += (dpint)a[6] * b[20];
  t += (dpint)a[7] * b[19];
  t += (dpint)a[8] * b[18];
  t += (dpint)a[9] * b[17];
  t += (dpint)a[10] * b[16];
  t += (dpint)a[11] * b[15];
  t += (dpint)a[12] * b[14];
  t += (dpint)a[13] * b[13];
  t += (dpint)a[14] * b[12];
  t += (dpint)a[15] * b[11];
  t += (dpint)a[16] * b[10];
  t += (dpint)a[17] * b[9];
  t += (dpint)a[18] * b[8];
  t += (dpint)a[19] * b[7];
  t += (dpint)a[20] * b[6];
  t += (dpint)a[21] * b[5];
  t += (dpint)a[22] * b[4];
  t += (dpint)a[23] * b[3];
  t += (dpint)v3 * (dpint)p23;
  c[2] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[4] * b[23];
  t += (dpint)a[5] * b[22];
  t += (dpint)a[6] * b[21];
  t += (dpint)a[7] * b[20];
  t += (dpint)a[8] * b[19];
  t += (dpint)a[9] * b[18];
  t += (dpint)a[10] * b[17];
  t += (dpint)a[11] * b[16];
  t += (dpint)a[12] * b[15];
  t += (dpint)a[13] * b[14];
  t += (dpint)a[14] * b[13];
  t += (dpint)a[15] * b[12];
  t += (dpint)a[16] * b[11];
  t += (dpint)a[17] * b[10];
  t += (dpint)a[18] * b[9];
  t += (dpint)a[19] * b[8];
  t += (dpint)a[20] * b[7];
  t += (dpint)a[21] * b[6];
  t += (dpint)a[22] * b[5];
  t += (dpint)a[23] * b[4];
  t += (dpint)v4 * (dpint)p23;
  c[3] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[5] * b[23];
  t += (dpint)a[6] * b[22];
  t += (dpint)a[7] * b[21];
  t += (dpint)a[8] * b[20];
  t += (dpint)a[9] * b[19];
  t += (dpint)a[10] * b[18];
  t += (dpint)a[11] * b[17];
  t += (dpint)a[12] * b[16];
  t += (dpint)a[13] * b[15];
  t += (dpint)a[14] * b[14];
  t += (dpint)a[15] * b[13];
  t += (dpint)a[16] * b[12];
  t += (dpint)a[17] * b[11];
  t += (dpint)a[18] * b[10];
  t += (dpint)a[19] * b[9];
  t += (dpint)a[20] * b[8];
  t += (dpint)a[21] * b[7];
  t += (dpint)a[22] * b[6];
  t += (dpint)a[23] * b[5];
  t += (dpint)v5 * (dpint)p23;
  c[4] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[6] * b[23];
  t += (dpint)a[7] * b[22];
  t += (dpint)a[8] * b[21];
  t += (dpint)a[9] * b[20];
  t += (dpint)a[10] * b[19];
  t += (dpint)a[11] * b[18];
  t += (dpint)a[12] * b[17];
  t += (dpint)a[13] * b[16];
  t += (dpint)a[14] * b[15];
  t += (dpint)a[15] * b[14];
  t += (dpint)a[16] * b[13];
  t += (dpint)a[17] * b[12];
  t += (dpint)a[18] * b[11];
  t += (dpint)a[19] * b[10];
  t += (dpint)a[20] * b[9];
  t += (dpint)a[21] * b[8];
  t += (dpint)a[22] * b[7];
  t += (dpint)a[23] * b[6];
  t += (dpint)v6 * (dpint)p23;
  c[5] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[7] * b[23];
  t += (dpint)a[8] * b[22];
  t += (dpint)a[9] * b[21];
  t += (dpint)a[10] * b[20];
  t += (dpint)a[11] * b[19];
  t += (dpint)a[12] * b[18];
  t += (dpint)a[13] * b[17];
  t += (dpint)a[14] * b[16];
  t += (dpint)a[15] * b[15];
  t += (dpint)a[16] * b[14];
  t += (dpint)a[17] * b[13];
  t += (dpint)a[18] * b[12];
  t += (dpint)a[19] * b[11];
  t += (dpint)a[20] * b[10];
  t += (dpint)a[21] * b[9];
  t += (dpint)a[22] * b[8];
  t += (dpint)a[23] * b[7];
  t += (dpint)v7 * (dpint)p23;
  c[6] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[8] * b[23];
  t += (dpint)a[9] * b[22];
  t += (dpint)a[10] * b[21];
  t += (dpint)a[11] * b[20];
  t += (dpint)a[12] * b[19];
  t += (dpint)a[13] * b[18];
  t += (dpint)a[14] * b[17];
  t += (dpint)a[15] * b[16];
  t += (dpint)a[16] * b[15];
  t += (dpint)a[17] * b[14];
  t += (dpint)a[18] * b[13];
  t += (dpint)a[19] * b[12];
  t += (dpint)a[20] * b[11];
  t += (dpint)a[21] * b[10];
  t += (dpint)a[22] * b[9];
  t += (dpint)a[23] * b[8];
  t += (dpint)v8 * (dpint)p23;
  c[7] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[9] * b[23];
  t += (dpint)a[10] * b[22];
  t += (dpint)a[11] * b[21];
  t += (dpint)a[12] * b[20];
  t += (dpint)a[13] * b[19];
  t += (dpint)a[14] * b[18];
  t += (dpint)a[15] * b[17];
  t += (dpint)a[16] * b[16];
  t += (dpint)a[17] * b[15];
  t += (dpint)a[18] * b[14];
  t += (dpint)a[19] * b[13];
  t += (dpint)a[20] * b[12];
  t += (dpint)a[21] * b[11];
  t += (dpint)a[22] * b[10];
  t += (dpint)a[23] * b[9];
  t += (dpint)v9 * (dpint)p23;
  c[8] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[10] * b[23];
  t += (dpint)a[11] * b[22];
  t += (dpint)a[12] * b[21];
  t += (dpint)a[13] * b[20];
  t += (dpint)a[14] * b[19];
  t += (dpint)a[15] * b[18];
  t += (dpint)a[16] * b[17];
  t += (dpint)a[17] * b[16];
  t += (dpint)a[18] * b[15];
  t += (dpint)a[19] * b[14];
  t += (dpint)a[20] * b[13];
  t += (dpint)a[21] * b[12];
  t += (dpint)a[22] * b[11];
  t += (dpint)a[23] * b[10];
  t += (dpint)v10 * (dpint)p23;
  c[9] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[11] * b[23];
  t += (dpint)a[12] * b[22];
  t += (dpint)a[13] * b[21];
  t += (dpint)a[14] * b[20];
  t += (dpint)a[15] * b[19];
  t += (dpint)a[16] * b[18];
  t += (dpint)a[17] * b[17];
  t += (dpint)a[18] * b[16];
  t += (dpint)a[19] * b[15];
  t += (dpint)a[20] * b[14];
  t += (dpint)a[21] * b[13];
  t += (dpint)a[22] * b[12];
  t += (dpint)a[23] * b[11];
  t += (dpint)v11 * (dpint)p23;
  c[10] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[12] * b[23];
  t += (dpint)a[13] * b[22];
  t += (dpint)a[14] * b[21];
  t += (dpint)a[15] * b[20];
  t += (dpint)a[16] * b[19];
  t += (dpint)a[17] * b[18];
  t += (dpint)a[18] * b[17];
  t += (dpint)a[19] * b[16];
  t += (dpint)a[20] * b[15];
  t += (dpint)a[21] * b[14];
  t += (dpint)a[22] * b[13];
  t += (dpint)a[23] * b[12];
  t += (dpint)v12 * (dpint)p23;
  c[11] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[13] * b[23];
  t += (dpint)a[14] * b[22];
  t += (dpint)a[15] * b[21];
  t += (dpint)a[16] * b[20];
  t += (dpint)a[17] * b[19];
  t += (dpint)a[18] * b[18];
  t += (dpint)a[19] * b[17];
  t += (dpint)a[20] * b[16];
  t += (dpint)a[21] * b[15];
  t += (dpint)a[22] * b[14];
  t += (dpint)a[23] * b[13];
  t += (dpint)v13 * (dpint)p23;
  c[12] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[14] * b[23];
  t += (dpint)a[15] * b[22];
  t += (dpint)a[16] * b[21];
  t += (dpint)a[17] * b[20];
  t += (dpint)a[18] * b[19];
  t += (dpint)a[19] * b[18];
  t += (dpint)a[20] * b[17];
  t += (dpint)a[21] * b[16];
  t += (dpint)a[22] * b[15];
  t += (dpint)a[23] * b[14];
  t += (dpint)v14 * (dpint)p23;
  c[13] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[15] * b[23];
  t += (dpint)a[16] * b[22];
  t += (dpint)a[17] * b[21];
  t += (dpint)a[18] * b[20];
  t += (dpint)a[19] * b[19];
  t += (dpint)a[20] * b[18];
  t += (dpint)a[21] * b[17];
  t += (dpint)a[22] * b[16];
  t += (dpint)a[23] * b[15];
  t += (dpint)v15 * (dpint)p23;
  c[14] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[16] * b[23];
  t += (dpint)a[17] * b[22];
  t += (dpint)a[18] * b[21];
  t += (dpint)a[19] * b[20];
  t += (dpint)a[20] * b[19];
  t += (dpint)a[21] * b[18];
  t += (dpint)a[22] * b[17];
  t += (dpint)a[23] * b[16];
  t += (dpint)v16 * (dpint)p23;
  c[15] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[17] * b[23];
  t += (dpint)a[18] * b[22];
  t += (dpint)a[19] * b[21];
  t += (dpint)a[20] * b[20];
  t += (dpint)a[21] * b[19];
  t += (dpint)a[22] * b[18];
  t += (dpint)a[23] * b[17];
  t += (dpint)v17 * (dpint)p23;
  c[16] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[18] * b[23];
  t += (dpint)a[19] * b[22];
  t += (dpint)a[20] * b[21];
  t += (dpint)a[21] * b[20];
  t += (dpint)a[22] * b[19];
  t += (dpint)a[23] * b[18];
  t += (dpint)v18 * (dpint)p23;
  c[17] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[19] * b[23];
  t += (dpint)a[20] * b[22];
  t += (dpint)a[21] * b[21];
  t += (dpint)a[22] * b[20];
  t += (dpint)a[23] * b[19];
  t += (dpint)v19 * (dpint)p23;
  c[18] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[20] * b[23];
  t += (dpint)a[21] * b[22];
  t += (dpint)a[22] * b[21];
  t += (dpint)a[23] * b[20];
  t += (dpint)v20 * (dpint)p23;
  c[19] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[21] * b[23];
  t += (dpint)a[22] * b[22];
  t += (dpint)a[23] * b[21];
  t += (dpint)v21 * (dpint)p23;
  c[20] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[22] * b[23];
  t += (dpint)a[23] * b[22];
  t += (dpint)v22 * (dpint)p23;
  c[21] = ((spint)t & mask);
  t >>= 28;
  t += (dpint)a[23] * b[23];
  t += (dpint)v23 * (dpint)p23;
  c[22] = ((spint)t & mask);
  t >>= 28;
  c[23] = (spint)t;
}

// Modular squaring, c=a*a  mod 2p
static void modsqr(const spint *a, spint *c) {
  udpint tot;
  udpint t = 0;
  spint p23 = 0x1100000u;
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
  spint v13 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[14];
  tot += (udpint)a[1] * a[13];
  tot += (udpint)a[2] * a[12];
  tot += (udpint)a[3] * a[11];
  tot += (udpint)a[4] * a[10];
  tot += (udpint)a[5] * a[9];
  tot += (udpint)a[6] * a[8];
  tot *= 2;
  tot += (udpint)a[7] * a[7];
  t += tot;
  spint v14 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[15];
  tot += (udpint)a[1] * a[14];
  tot += (udpint)a[2] * a[13];
  tot += (udpint)a[3] * a[12];
  tot += (udpint)a[4] * a[11];
  tot += (udpint)a[5] * a[10];
  tot += (udpint)a[6] * a[9];
  tot += (udpint)a[7] * a[8];
  tot *= 2;
  t += tot;
  spint v15 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[16];
  tot += (udpint)a[1] * a[15];
  tot += (udpint)a[2] * a[14];
  tot += (udpint)a[3] * a[13];
  tot += (udpint)a[4] * a[12];
  tot += (udpint)a[5] * a[11];
  tot += (udpint)a[6] * a[10];
  tot += (udpint)a[7] * a[9];
  tot *= 2;
  tot += (udpint)a[8] * a[8];
  t += tot;
  spint v16 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[17];
  tot += (udpint)a[1] * a[16];
  tot += (udpint)a[2] * a[15];
  tot += (udpint)a[3] * a[14];
  tot += (udpint)a[4] * a[13];
  tot += (udpint)a[5] * a[12];
  tot += (udpint)a[6] * a[11];
  tot += (udpint)a[7] * a[10];
  tot += (udpint)a[8] * a[9];
  tot *= 2;
  t += tot;
  spint v17 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[18];
  tot += (udpint)a[1] * a[17];
  tot += (udpint)a[2] * a[16];
  tot += (udpint)a[3] * a[15];
  tot += (udpint)a[4] * a[14];
  tot += (udpint)a[5] * a[13];
  tot += (udpint)a[6] * a[12];
  tot += (udpint)a[7] * a[11];
  tot += (udpint)a[8] * a[10];
  tot *= 2;
  tot += (udpint)a[9] * a[9];
  t += tot;
  spint v18 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[19];
  tot += (udpint)a[1] * a[18];
  tot += (udpint)a[2] * a[17];
  tot += (udpint)a[3] * a[16];
  tot += (udpint)a[4] * a[15];
  tot += (udpint)a[5] * a[14];
  tot += (udpint)a[6] * a[13];
  tot += (udpint)a[7] * a[12];
  tot += (udpint)a[8] * a[11];
  tot += (udpint)a[9] * a[10];
  tot *= 2;
  t += tot;
  spint v19 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[20];
  tot += (udpint)a[1] * a[19];
  tot += (udpint)a[2] * a[18];
  tot += (udpint)a[3] * a[17];
  tot += (udpint)a[4] * a[16];
  tot += (udpint)a[5] * a[15];
  tot += (udpint)a[6] * a[14];
  tot += (udpint)a[7] * a[13];
  tot += (udpint)a[8] * a[12];
  tot += (udpint)a[9] * a[11];
  tot *= 2;
  tot += (udpint)a[10] * a[10];
  t += tot;
  spint v20 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[21];
  tot += (udpint)a[1] * a[20];
  tot += (udpint)a[2] * a[19];
  tot += (udpint)a[3] * a[18];
  tot += (udpint)a[4] * a[17];
  tot += (udpint)a[5] * a[16];
  tot += (udpint)a[6] * a[15];
  tot += (udpint)a[7] * a[14];
  tot += (udpint)a[8] * a[13];
  tot += (udpint)a[9] * a[12];
  tot += (udpint)a[10] * a[11];
  tot *= 2;
  t += tot;
  spint v21 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[22];
  tot += (udpint)a[1] * a[21];
  tot += (udpint)a[2] * a[20];
  tot += (udpint)a[3] * a[19];
  tot += (udpint)a[4] * a[18];
  tot += (udpint)a[5] * a[17];
  tot += (udpint)a[6] * a[16];
  tot += (udpint)a[7] * a[15];
  tot += (udpint)a[8] * a[14];
  tot += (udpint)a[9] * a[13];
  tot += (udpint)a[10] * a[12];
  tot *= 2;
  tot += (udpint)a[11] * a[11];
  t += tot;
  spint v22 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[0] * a[23];
  tot += (udpint)a[1] * a[22];
  tot += (udpint)a[2] * a[21];
  tot += (udpint)a[3] * a[20];
  tot += (udpint)a[4] * a[19];
  tot += (udpint)a[5] * a[18];
  tot += (udpint)a[6] * a[17];
  tot += (udpint)a[7] * a[16];
  tot += (udpint)a[8] * a[15];
  tot += (udpint)a[9] * a[14];
  tot += (udpint)a[10] * a[13];
  tot += (udpint)a[11] * a[12];
  tot *= 2;
  t += tot;
  t += (udpint)v0 * p23;
  spint v23 = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[1] * a[23];
  tot += (udpint)a[2] * a[22];
  tot += (udpint)a[3] * a[21];
  tot += (udpint)a[4] * a[20];
  tot += (udpint)a[5] * a[19];
  tot += (udpint)a[6] * a[18];
  tot += (udpint)a[7] * a[17];
  tot += (udpint)a[8] * a[16];
  tot += (udpint)a[9] * a[15];
  tot += (udpint)a[10] * a[14];
  tot += (udpint)a[11] * a[13];
  tot *= 2;
  tot += (udpint)a[12] * a[12];
  t += tot;
  t += (udpint)v1 * p23;
  c[0] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[2] * a[23];
  tot += (udpint)a[3] * a[22];
  tot += (udpint)a[4] * a[21];
  tot += (udpint)a[5] * a[20];
  tot += (udpint)a[6] * a[19];
  tot += (udpint)a[7] * a[18];
  tot += (udpint)a[8] * a[17];
  tot += (udpint)a[9] * a[16];
  tot += (udpint)a[10] * a[15];
  tot += (udpint)a[11] * a[14];
  tot += (udpint)a[12] * a[13];
  tot *= 2;
  t += tot;
  t += (udpint)v2 * p23;
  c[1] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[3] * a[23];
  tot += (udpint)a[4] * a[22];
  tot += (udpint)a[5] * a[21];
  tot += (udpint)a[6] * a[20];
  tot += (udpint)a[7] * a[19];
  tot += (udpint)a[8] * a[18];
  tot += (udpint)a[9] * a[17];
  tot += (udpint)a[10] * a[16];
  tot += (udpint)a[11] * a[15];
  tot += (udpint)a[12] * a[14];
  tot *= 2;
  tot += (udpint)a[13] * a[13];
  t += tot;
  t += (udpint)v3 * p23;
  c[2] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[4] * a[23];
  tot += (udpint)a[5] * a[22];
  tot += (udpint)a[6] * a[21];
  tot += (udpint)a[7] * a[20];
  tot += (udpint)a[8] * a[19];
  tot += (udpint)a[9] * a[18];
  tot += (udpint)a[10] * a[17];
  tot += (udpint)a[11] * a[16];
  tot += (udpint)a[12] * a[15];
  tot += (udpint)a[13] * a[14];
  tot *= 2;
  t += tot;
  t += (udpint)v4 * p23;
  c[3] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[5] * a[23];
  tot += (udpint)a[6] * a[22];
  tot += (udpint)a[7] * a[21];
  tot += (udpint)a[8] * a[20];
  tot += (udpint)a[9] * a[19];
  tot += (udpint)a[10] * a[18];
  tot += (udpint)a[11] * a[17];
  tot += (udpint)a[12] * a[16];
  tot += (udpint)a[13] * a[15];
  tot *= 2;
  tot += (udpint)a[14] * a[14];
  t += tot;
  t += (udpint)v5 * p23;
  c[4] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[6] * a[23];
  tot += (udpint)a[7] * a[22];
  tot += (udpint)a[8] * a[21];
  tot += (udpint)a[9] * a[20];
  tot += (udpint)a[10] * a[19];
  tot += (udpint)a[11] * a[18];
  tot += (udpint)a[12] * a[17];
  tot += (udpint)a[13] * a[16];
  tot += (udpint)a[14] * a[15];
  tot *= 2;
  t += tot;
  t += (udpint)v6 * p23;
  c[5] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[7] * a[23];
  tot += (udpint)a[8] * a[22];
  tot += (udpint)a[9] * a[21];
  tot += (udpint)a[10] * a[20];
  tot += (udpint)a[11] * a[19];
  tot += (udpint)a[12] * a[18];
  tot += (udpint)a[13] * a[17];
  tot += (udpint)a[14] * a[16];
  tot *= 2;
  tot += (udpint)a[15] * a[15];
  t += tot;
  t += (udpint)v7 * p23;
  c[6] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[8] * a[23];
  tot += (udpint)a[9] * a[22];
  tot += (udpint)a[10] * a[21];
  tot += (udpint)a[11] * a[20];
  tot += (udpint)a[12] * a[19];
  tot += (udpint)a[13] * a[18];
  tot += (udpint)a[14] * a[17];
  tot += (udpint)a[15] * a[16];
  tot *= 2;
  t += tot;
  t += (udpint)v8 * p23;
  c[7] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[9] * a[23];
  tot += (udpint)a[10] * a[22];
  tot += (udpint)a[11] * a[21];
  tot += (udpint)a[12] * a[20];
  tot += (udpint)a[13] * a[19];
  tot += (udpint)a[14] * a[18];
  tot += (udpint)a[15] * a[17];
  tot *= 2;
  tot += (udpint)a[16] * a[16];
  t += tot;
  t += (udpint)v9 * p23;
  c[8] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[10] * a[23];
  tot += (udpint)a[11] * a[22];
  tot += (udpint)a[12] * a[21];
  tot += (udpint)a[13] * a[20];
  tot += (udpint)a[14] * a[19];
  tot += (udpint)a[15] * a[18];
  tot += (udpint)a[16] * a[17];
  tot *= 2;
  t += tot;
  t += (udpint)v10 * p23;
  c[9] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[11] * a[23];
  tot += (udpint)a[12] * a[22];
  tot += (udpint)a[13] * a[21];
  tot += (udpint)a[14] * a[20];
  tot += (udpint)a[15] * a[19];
  tot += (udpint)a[16] * a[18];
  tot *= 2;
  tot += (udpint)a[17] * a[17];
  t += tot;
  t += (udpint)v11 * p23;
  c[10] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[12] * a[23];
  tot += (udpint)a[13] * a[22];
  tot += (udpint)a[14] * a[21];
  tot += (udpint)a[15] * a[20];
  tot += (udpint)a[16] * a[19];
  tot += (udpint)a[17] * a[18];
  tot *= 2;
  t += tot;
  t += (udpint)v12 * p23;
  c[11] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[13] * a[23];
  tot += (udpint)a[14] * a[22];
  tot += (udpint)a[15] * a[21];
  tot += (udpint)a[16] * a[20];
  tot += (udpint)a[17] * a[19];
  tot *= 2;
  tot += (udpint)a[18] * a[18];
  t += tot;
  t += (udpint)v13 * p23;
  c[12] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[14] * a[23];
  tot += (udpint)a[15] * a[22];
  tot += (udpint)a[16] * a[21];
  tot += (udpint)a[17] * a[20];
  tot += (udpint)a[18] * a[19];
  tot *= 2;
  t += tot;
  t += (udpint)v14 * p23;
  c[13] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[15] * a[23];
  tot += (udpint)a[16] * a[22];
  tot += (udpint)a[17] * a[21];
  tot += (udpint)a[18] * a[20];
  tot *= 2;
  tot += (udpint)a[19] * a[19];
  t += tot;
  t += (udpint)v15 * p23;
  c[14] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[16] * a[23];
  tot += (udpint)a[17] * a[22];
  tot += (udpint)a[18] * a[21];
  tot += (udpint)a[19] * a[20];
  tot *= 2;
  t += tot;
  t += (udpint)v16 * p23;
  c[15] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[17] * a[23];
  tot += (udpint)a[18] * a[22];
  tot += (udpint)a[19] * a[21];
  tot *= 2;
  tot += (udpint)a[20] * a[20];
  t += tot;
  t += (udpint)v17 * p23;
  c[16] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[18] * a[23];
  tot += (udpint)a[19] * a[22];
  tot += (udpint)a[20] * a[21];
  tot *= 2;
  t += tot;
  t += (udpint)v18 * p23;
  c[17] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[19] * a[23];
  tot += (udpint)a[20] * a[22];
  tot *= 2;
  tot += (udpint)a[21] * a[21];
  t += tot;
  t += (udpint)v19 * p23;
  c[18] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[20] * a[23];
  tot += (udpint)a[21] * a[22];
  tot *= 2;
  t += tot;
  t += (udpint)v20 * p23;
  c[19] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[21] * a[23];
  tot *= 2;
  tot += (udpint)a[22] * a[22];
  t += tot;
  t += (udpint)v21 * p23;
  c[20] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[22] * a[23];
  tot *= 2;
  t += tot;
  t += (udpint)v22 * p23;
  c[21] = ((spint)t & mask);
  t >>= 28;
  tot = (udpint)a[23] * a[23];
  t += tot;
  t += (udpint)v23 * p23;
  c[22] = ((spint)t & mask);
  t >>= 28;
  c[23] = (spint)t;
}

// copy
static void modcpy(const spint *a, spint *c) {
  int i;
  for (i = 0; i < 24; i++) {
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
  spint x[24];
  spint t0[24];
  spint t1[24];
  spint t2[24];
  spint t3[24];
  modcpy(w, x);
  modsqr(x, z);
  modmul(x, z, z);
  modsqr(z, z);
  modmul(x, z, z);
  modsqr(z, t0);
  modmul(x, t0, t0);
  modsqr(t0, t2);
  modcpy(t2, t1);
  modnsqr(t1, 2);
  modcpy(t1, t3);
  modnsqr(t3, 2);
  modmul(t2, t3, t2);
  modnsqr(t2, 6);
  modmul(t1, t2, t1);
  modcpy(t1, t2);
  modnsqr(t2, 12);
  modmul(t1, t2, t2);
  modmul(z, t2, z);
  modcpy(z, t2);
  modnsqr(t2, 15);
  modcpy(t2, t3);
  modnsqr(t3, 27);
  modmul(t2, t3, t2);
  modcpy(t2, t3);
  modnsqr(t3, 54);
  modmul(t2, t3, t2);
  modmul(t1, t2, t1);
  modmul(t0, t1, t0);
  modmul(t1, t0, t1);
  modsqr(t1, t2);
  modmul(t1, t2, t2);
  modsqr(t2, t2);
  modmul(t1, t2, t2);
  modmul(t0, t2, t0);
  modmul(t1, t0, t2);
  modmul(t0, t2, t0);
  modmul(t1, t0, t1);
  modnsqr(t1, 127);
  modmul(t0, t1, t1);
  modnsqr(t1, 128);
  modmul(t0, t1, t1);
  modnsqr(t1, 128);
  modmul(t0, t1, t1);
  modnsqr(t1, 128);
  modmul(t0, t1, t0);
  modnsqr(t0, 27);
  modmul(z, t0, z);
}

// calculate inverse, provide progenitor h if available
static void modinv(const spint *x, const spint *h, spint *z) {
  spint s[24];
  spint t[24];
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
  const spint c[24] = {
      0xf0f0ff1u, 0xf0f0f0u, 0xf0f0f0fu, 0xf0f0f0u, 0xf0f0f0fu, 0xf0f0f0u,
      0xf0f0f0fu, 0xf0f0f0u, 0xf0f0f0fu, 0xf0f0f0u, 0xf0f0f0fu, 0xf0f0f0u,
      0xf0f0f0fu, 0xf0f0f0u, 0xf0f0f0fu, 0xf0f0f0u, 0xf0f0f0fu, 0xf0f0f0u,
      0xf0f0f0fu, 0xf0f0f0u, 0xf0f0f0fu, 0xf0f0f0u, 0xf0f0f0fu, 0xe0f0f0u};
  modmul(m, c, n);
}

// Convert n back to normal form, m=redc(n)
static void redc(const spint *n, spint *m) {
  int i;
  spint c[24];
  c[0] = 1;
  for (i = 1; i < 24; i++) {
    c[i] = 0;
  }
  modmul(n, c, m);
  (void)modfsb(m);
}

// is unity?
static int modis1(const spint *a) {
  int i;
  spint c[24];
  spint c0;
  spint d = 0;
  redc(a, c);
  for (i = 1; i < 24; i++) {
    d |= c[i];
  }
  c0 = (spint)c[0];
  return ((spint)1 & ((d - (spint)1) >> 28u) &
          (((c0 ^ (spint)1) - (spint)1) >> 28u));
}

// is zero?
static int modis0(const spint *a) {
  int i;
  spint c[24];
  spint d = 0;
  redc(a, c);
  for (i = 0; i < 24; i++) {
    d |= c[i];
  }
  return ((spint)1 & ((d - (spint)1) >> 28u));
}

// set to zero
static void modzer(spint *a) {
  int i;
  for (i = 0; i < 24; i++) {
    a[i] = 0;
  }
}

// set to one
static void modone(spint *a) {
  int i;
  a[0] = 1;
  for (i = 1; i < 24; i++) {
    a[i] = 0;
  }
  nres(a, a);
}

// set to integer
static void modint(int x, spint *a) {
  int i;
  a[0] = (spint)x;
  for (i = 1; i < 24; i++) {
    a[i] = 0;
  }
  nres(a, a);
}


// Test for quadratic residue
static int modqr(const spint *h, const spint *x) {
  spint r[24];
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
static void SQISIGN_NOINLINE modcsw(int b, volatile spint *g,
                                             volatile spint *f) {
  int i;
  spint c0, c1, s, t, w, v, aux;
  static spint R = 0;
  R += 0x5aa5a55au;
  w = R;
  c0 = (~b) & (w + 1);
  c1 = b + w;
  for (i = 0; i < 24; i++) {
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
  spint s[24];
  spint y[24];
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
  a[23] = ((a[23] << n)) | (a[22] >> (28u - n));
  for (i = 22; i > 0; i--) {
    a[i] = ((a[i] << n) & (spint)0xfffffff) | (a[i - 1] >> (28u - n));
  }
  a[0] = (a[0] << n) & (spint)0xfffffff;
}

// shift right by less than a word. Return shifted out part
static int modshr(unsigned int n, spint *a) {
  int i;
  spint r = a[0] & (((spint)1 << n) - (spint)1);
  for (i = 0; i < 23; i++) {
    a[i] = (a[i] >> n) | ((a[i + 1] << (28u - n)) & (spint)0xfffffff);
  }
  a[23] = a[23] >> n;
  return r;
}





// return true if equal
static int modcmp(const spint *a, const spint *b) {
  spint c[24], d[24];
  int i, eq = 1;
  redc(a, c);
  redc(b, d);
  for (i = 0; i < 24; i++) {
    eq &= (((c[i] ^ d[i]) - 1) >> 28) & 1;
  }
  return eq;
}

/******************************************************************************
 * SQIsign fp_* API wrappers over the modarith code above.
 * Auto-generated by scripts/add_parameter_set.py — generic for any prime
 * with p == 3 (mod 4). Do not hand-edit; re-run the generator instead.
 ******************************************************************************/

#include <fp.h>

const fp_t ZERO = { {
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0
} };

const fp_t ONE = { {
    0xf,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x100000
} };

// Montgomery representation of 2^-1
static const fp_t TWO_INV = { {
    0x7,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x900000
} };

// Montgomery representation of 3^-1
static const fp_t THREE_INV = { {
    0xaaaaaaf,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xaaaaaaa,
    0xbaaaaa
} };

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
    spint c[24];
    unsigned char *out = dst;
    redc(a->fp, c);
    for (i = 0; i < 84; i++) {
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
    for (i = 0; i < 24; i++) {
        d->fp[i] = 0;
    }
    for (i = 84 - 1; i >= 0; i--) {
        modshl(8, d->fp);
        d->fp[0] += (spint)b[i];
    }
    res = (spint)-modfsb(d->fp);
    nres(d->fp, d->fp);
    for (i = 0; i < 24; i++) {
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
