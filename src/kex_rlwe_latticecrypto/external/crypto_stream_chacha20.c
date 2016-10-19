/* Based on the public domain implemntation in
 * crypto_stream/chacha20/e/ref from http://bench.cr.yp.to/supercop.html
 * by Daniel J. Bernstein */

#include <stdint.h>

#define ROUNDS 20

typedef uint32_t uint32;

static uint32 load_littleendian(const unsigned char *x)
{
  return
      (uint32) (x[0]) \
  | (((uint32) (x[1])) << 8) \
  | (((uint32) (x[2])) << 16) \
  | (((uint32) (x[3])) << 24);
}

static void store_littleendian(unsigned char *x,uint32 u)
{
  x[0] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[3] = u;
}

#define ROTATE(v,c) ((v << c) | (v >> (32-c)))
#define XOR(v,w) ((v) ^ (w))
#define PLUS(v,w) ((v) + (w))
#define PLUSONE(v) (PLUS((v),1))

#define QUARTERROUND(a,b,c,d) \
  a = PLUS(a,b); d = ROTATE(XOR(d,a),16); \
  c = PLUS(c,d); b = ROTATE(XOR(b,c),12); \
  a = PLUS(a,b); d = ROTATE(XOR(d,a), 8); \
  c = PLUS(c,d); b = ROTATE(XOR(b,c), 7);


static int crypto_core_chacha20(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
)
{
  uint32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
  uint32 j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14, j15;
  int i;

  j0  = x0  = load_littleendian(c +  0);
  j1  = x1  = load_littleendian(c +  4);
  j2  = x2  = load_littleendian(c +  8);
  j3  = x3  = load_littleendian(c + 12);
  j4  = x4  = load_littleendian(k +  0);
  j5  = x5  = load_littleendian(k +  4);
  j6  = x6  = load_littleendian(k +  8);
  j7  = x7  = load_littleendian(k + 12);
  j8  = x8  = load_littleendian(k + 16);
  j9  = x9  = load_littleendian(k + 20);
  j10 = x10 = load_littleendian(k + 24);
  j11 = x11 = load_littleendian(k + 28);
  j12 = x12 = load_littleendian(in+  8);
  j13 = x13 = load_littleendian(in+ 12);
  j14 = x14 = load_littleendian(in+  0);
  j15 = x15 = load_littleendian(in+  4);

  for (i = ROUNDS;i > 0;i -= 2) {
    QUARTERROUND(x0, x4, x8,x12);
    QUARTERROUND(x1, x5, x9,x13);
    QUARTERROUND(x2, x6,x10,x14);
    QUARTERROUND(x3, x7,x11,x15);
    QUARTERROUND(x0, x5,x10,x15);
    QUARTERROUND(x1, x6,x11,x12);
    QUARTERROUND(x2, x7, x8,x13);
    QUARTERROUND(x3, x4, x9,x14);
  }

  x0 += j0;
  x1 += j1;
  x2 += j2;
  x3 += j3;
  x4 += j4;
  x5 += j5;
  x6 += j6;
  x7 += j7;
  x8 += j8;
  x9 += j9;
  x10 += j10;
  x11 += j11;
  x12 += j12;
  x13 += j13;
  x14 += j14;
  x15 += j15;

  store_littleendian(out + 0,x0);
  store_littleendian(out + 4,x1);
  store_littleendian(out + 8,x2);
  store_littleendian(out + 12,x3);
  store_littleendian(out + 16,x4);
  store_littleendian(out + 20,x5);
  store_littleendian(out + 24,x6);
  store_littleendian(out + 28,x7);
  store_littleendian(out + 32,x8);
  store_littleendian(out + 36,x9);
  store_littleendian(out + 40,x10);
  store_littleendian(out + 44,x11);
  store_littleendian(out + 48,x12);
  store_littleendian(out + 52,x13);
  store_littleendian(out + 56,x14);
  store_littleendian(out + 60,x15);

  return 0;
}

static const unsigned char sigma[16] = "expand 32-byte k";

int crypto_stream_chacha20(unsigned char *c,unsigned long long clen, const unsigned char *n, const unsigned char *k)
{
  unsigned char in[16];
  unsigned char block[64];
  unsigned char kcopy[32];
  unsigned long long i;
  unsigned int u;

  if (!clen) return 0;

  for (i = 0;i < 32;++i) kcopy[i] = k[i];
  for (i = 0;i < 8;++i) in[i] = n[i];
  for (i = 8;i < 16;++i) in[i] = 0;

  while (clen >= 64) {
    crypto_core_chacha20(c,in,kcopy,sigma);

    u = 1;
    for (i = 8;i < 16;++i) {
      u += (unsigned int) in[i];
      in[i] = u;
      u >>= 8;
    }

    clen -= 64;
    c += 64;
  }

  if (clen) {
    crypto_core_chacha20(block,in,kcopy,sigma);
    for (i = 0;i < clen;++i) c[i] = block[i];
  }
  return 0;
}
