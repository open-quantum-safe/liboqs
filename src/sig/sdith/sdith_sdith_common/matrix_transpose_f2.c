#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "vole_private.h"

EXPORT void transpose_128_128_naive(void* out, const void* in) {
  CASSERT(out != in);
  typedef __uint128_t T;
  const T* x = (T*)in;
  T* r = (T*)out;
  for (uint64_t i = 0; i < 128; ++i) {
    __uint128_t oi = 0;
    for (uint64_t j = 0; j < 128; ++j) {
      oi |= ((x[j] >> i) & 1) << j;
    }
    r[i] = oi;
  }
}

EXPORT void transpose_192_192_naive(void* out, const void* in) {
  CASSERT(out != in);
  const uint64_t* x = (uint64_t*)in;
  uint64_t *r = out;
  for (uint64_t i = 0; i < 192; ++i) {
    uint64_t tmp[3] = {0};
    for (uint64_t j = 0; j < 192; ++j) {
      tmp[j/64] |= ((x[j*3 + i/64] >> (i%64)) & 1) << (j%64);
    }
    for (uint64_t j = 0; j < 3; ++j)
    {
      r[i*3 + j] = tmp[j];
    }
  }
}

EXPORT void transpose_256_256_naive(void* out, const void* in) {
  CASSERT(out != in);
  const uint64_t* x = (uint64_t*)in;
  uint64_t *r = out;
  for (uint64_t i = 0; i < 256; ++i) {
    uint64_t tmp[4] = {0};
    for (uint64_t j = 0; j < 256; ++j) {
      tmp[j/64] |= ((x[j*4 + i/64] >> (i%64)) & 1) << (j%64);
    }
    for (uint64_t j = 0; j < 4; ++j)
    {
      r[i*4 + j] = tmp[j];
    }
  }
}

__always_inline void transpose8(uint8_t *a, uint64_t stride) {
    int j, k;
    uint64_t m, t;
    m = 0x0F;
    for (j = 4; j != 0; j = j >> 1, m = m ^ (m << j)) {
        for (k = 0; k < 8; k = (k + j + 1) & ~j) {
            t = (a[(k + j)*stride] ^ (a[k*stride] >> j)) & m;
            a[(k + j)*stride] = a[(k + j)*stride] ^ t;
            a[k*stride] = a[k*stride] ^ (t << j);
        }
    }
}

__always_inline void transpose64(uint64_t *a, uint64_t stride) {
    int j, k;
    uint64_t m, t;
    m = 0x00000000FFFFFFFFULL;
    for (j = 32; j != 0; j = j >> 1, m = m ^ (m << j)) {
        for (k = 0; k < 64; k = (k + j + 1) & ~j) {
            t = (a[(k + j)*stride] ^ (a[k*stride] >> j)) & m;
            a[(k + j)*stride] = a[(k + j)*stride] ^ t;
            a[k*stride] = a[k*stride] ^ (t << j);
        }
    }
}

__always_inline void transpose64_unaligned(uint8_t *a, uint64_t byte_stride) 
{
    uint64_t tmp[64];
    for (size_t i = 0; i < 64; i++)
    {
      tmp[i] = *((uint64_t*)&a[i*byte_stride]);
    }
    transpose64(tmp, 1);
    for (size_t i = 0; i < 64; i++)
    {
      *((uint64_t*)&a[i*byte_stride]) = tmp[i];
    }
}

__always_inline void transpose128(__uint128_t *a, uint64_t stride) {
    int j, k;
    __uint128_t m, t;
    m = 1;
    m <<= 64;
    m -= 1;
    for (j = 64; j != 0; j = j >> 1, m = m ^ (m << j)) {
        for (k = 0; k < 128; k = (k + j + 1) & ~j) {
            t = (a[(k + j)*stride] ^ (a[k*stride] >> j)) & m;
            a[(k + j)*stride] = a[(k + j)*stride] ^ t;
            a[k*stride] = a[k*stride] ^ (t << j);
        }
    }
}

EXPORT void transpose_128_128_ref(void* x) {
  transpose128((__uint128_t*)x, 1);
  
  // legacy code:
  //uint64_t *x64 = x;
  //// split into 4 submatrices:
  //// A B
  //// C D
  //// A starts at x64[0]
  //// B starts at x64[1]
  //// C starts at x64[128]
  //// D starts at x64[129]
  //transpose64(&x64[0],   2);
  //transpose64(&x64[1],   2);
  //transpose64(&x64[128], 2);
  //transpose64(&x64[129], 2);
  //
  //// swap submatrix B and C
  //for (uint64_t i = 0; i < 64; i++)
  //{
  //  x64[2*i+  1] ^= x64[2*i+128];
  //  x64[2*i+128] ^= x64[2*i+  1];
  //  x64[2*i+  1] ^= x64[2*i+128];
  //}
}

EXPORT void transpose_192_192_ref(void* x) {
  uint64_t *x64 = x;
  // split into 9 submatrices:
  // A B C
  // D E F
  // G H I
  uint64_t *A = &x64[0];
  uint64_t *B = &x64[1];
  uint64_t *C = &x64[2];
  uint64_t *D = &x64[192];
  uint64_t *E = &x64[193];
  uint64_t *F = &x64[194];
  uint64_t *G = &x64[384];
  uint64_t *H = &x64[385];
  uint64_t *I = &x64[386];
  
  transpose64(A, 3);
  transpose64(B, 3);
  transpose64(C, 3);
  transpose64(D, 3);
  transpose64(E, 3);
  transpose64(F, 3);
  transpose64(G, 3);
  transpose64(H, 3);
  transpose64(I, 3);
  
  // swap C and G
  for (uint64_t i = 0; i < 64; i++)
  {
    C[3*i] ^= G[3*i];
    G[3*i] ^= C[3*i];
    C[3*i] ^= G[3*i];
  }
  // swap B and D
  for (uint64_t i = 0; i < 64; i++)
  {
    B[3*i] ^= D[3*i];
    D[3*i] ^= B[3*i];
    B[3*i] ^= D[3*i];
  }
  // swap F and H
  for (uint64_t i = 0; i < 64; i++)
  {
    F[3*i] ^= H[3*i];
    H[3*i] ^= F[3*i];
    F[3*i] ^= H[3*i];
  }
}

EXPORT void transpose_256_256_ref(void* x) {
  __uint128_t *x128 = x;
  // split into 4 submatrices:
  // A B
  // C D
  __uint128_t *A = &x128[0];
  __uint128_t *B = &x128[1];
  __uint128_t *C = &x128[256];
  __uint128_t *D = &x128[257];
  
  transpose128(A, 2);
  transpose128(B, 2);
  transpose128(C, 2);
  transpose128(D, 2);
  
  // swap submatrix B and C
  for (uint64_t i = 0; i < 128; i++)
  {
    B[2*i] ^= C[2*i];
    C[2*i] ^= B[2*i];
    B[2*i] ^= C[2*i];
  }
}

EXPORT void transpose_128_L_naive(void* out, const void* in, uint64_t L, uint64_t Lslice)
{
  CASSERT(out != in);
  CREQUIRE((L & 7) == 0, "L must be a multiple of 8");
  /*
   * Input order:  lambda elements of Lbyte bytes
   * Output order: Lbyte*8 elements of lambda bits
   */
  const uint64_t Lbyte = L/8;
  CREQUIRE(Lslice >= Lbyte, "Lslice must be larger than L/8");
  const uint8_t* x = (uint8_t*)in;
  // x[0..Lbyte-1] hold the first row
  // x[Lbyte..2Lbyte-1] the second and so on, until
  // x[127Lbyte..128Lbyte-1] hold the last row
  uint8_t *r = out;
  // r[0..15] hold the first output row
  // r[16..31] hold the second output row and so on, until
  // r[(L-1)*16..L*16-1] hold the last output row
  for (uint64_t i = 0; i < 128*Lbyte; i++)
  {
    r[i] = 0;
  }
  for (uint64_t i = 0; i < L; ++i) 
  {
    for (uint64_t j = 0; j < 128; ++j) 
    {
      // get the i-th bit in the j-th input row
      uint8_t tmp = (x[j*Lslice+i/8] >> (i%8)) & 1;
      // store it to the j-th output bit of the i-th output row
      r[i*16+j/8] |= tmp << (j%8);
    }
  }
}

EXPORT void transpose_128_L_ref(void* out, const void* in, uint64_t L, uint64_t Lslice)
{
  CASSERT(out != in);
  CREQUIRE((L & 7) == 0, "L must be a multiple of 8");
  /*
   * Input order:  128 elements of Lbyte bytes
   * Output order: Lbyte*8 elements of 128 bits
   */
  const uint64_t Lbyte = L/8;
  CASSERT(Lslice >= Lbyte, "Lslice must be larger than L/8");
  uint64_t* o64 = (uint64_t*)out;

  if ((L%128) == 0)
  {
    uint64_t* x64 = (uint64_t*)in;
    
    // x64[0..Lbyte/8-1] hold the first row
    // x64[Lbyte/8..2Lbyte/8-1] the second and so on, until
    // x64[127Lbyte/8..128Lbyte/8-1] hold the last row
    // x64[0..1] hold the first output row
    // x64[2..3] hold the second output row and so on, until
    // x64[(L-1)*2..L*2-1] hold the last output row

    // we split the whole matrix into Lbyte/16 sub-matrices (128x128 bit) 
    for (size_t i = 0; i < Lbyte/16; i++)
    {
      // first copy into out
      for (size_t j = 0; j < 64*4; j++)
      {
        o64[j + i*64*4] = x64[(j%2) + (j/2)*Lslice/8 + i*2];
      }
      transpose_128_128_ref(&o64[i*64*4]);
    }

  } else {

    uint8_t* x8 = (uint8_t*)in;
    uint8_t* o8 = (uint8_t*)out;
    // x8[0..Lbyte-1] hold the first row
    // x8[Lbyte..2Lbyte-1] the second and so on, until
    // x8[127Lbyte..128Lbyte-1] hold the last row
    // o8[0..15] hold the first output row
    // o8[16..31] hold the second output row and so on, until
    // o8[(L-1)*16..L*16-1] hold the last output row
    
    for (size_t i = 0; i < Lbyte/16; i++)
    {
      // copy 128x128 submatrix to output
      for (size_t j = 0; j < 128*128/8; j++)
      {
        o8[j + i*128*16] = x8[(j%16) + (j/16)*Lslice + i*16];
      }
      transpose_128_128_ref(&o64[i*64*4]);
    }

    // now transpose the remainder bytes byte-wise
    for (size_t i = (Lbyte/16)*16; i < Lbyte; i++)
    {
      for (size_t j = 0; j < 128/8; j++)
      {
        // copy this 8x8 matrix to its destination, then transpose
        for (size_t k = 0; k < 8; k++)
        {
          o8[16*8*i + 16*k + j] = x8[i + Lslice*8*j + Lslice*k];
        }
        transpose8(&o8[16*8*i + j], 16);
      }
    }
    
  }
}

EXPORT void transpose_192_L_naive(void* out, const void* in, uint64_t L, uint64_t Lslice)
{
  CASSERT(out != in);
  CREQUIRE((L & 7) == 0, "L must be a multiple of 8");
  /*
   * Input order:  192 elements of Lbyte bytes
   * Output order: Lbyte*8 elements of 192 bits
   */
  const uint64_t Lbyte = L/8;
  CREQUIRE(Lslice >= Lbyte, "Lslice must be larger than L/8");
  const uint8_t* x = (uint8_t*)in;
  // x[0..Lbyte-1] hold the first row
  // x[Lbyte..2Lbyte-1] the second and so on, until
  // x[191Lbyte..192Lbyte-1] hold the last row
  uint8_t *r = out;
  // r[0..23] hold the first output row
  // r[24..47] hold the second output row and so on, until
  // r[(L-1)*24..L*24-1] hold the last output row
  for (uint64_t i = 0; i < 192*Lbyte; i++)
  {
    r[i] = 0;
  }
  for (uint64_t i = 0; i < L; ++i) 
  {
    for (uint64_t j = 0; j < 192; ++j) 
    {
      // get the i-th bit in the j-th input row
      uint8_t tmp = (x[j*Lslice+i/8] >> (i%8)) & 1;
      // store it to the j-th output bit of the i-th output row
      r[i*24+j/8] |= tmp << (j%8);
    }
  }
}

EXPORT void transpose_192_L_ref(void* out, const void* in, uint64_t L, uint64_t Lslice)
{
  CASSERT(out != in);
  CREQUIRE((L & 7) == 0, "L must be a multiple of 8");
  /*
   * Input order:  192 elements of Lbyte bytes
   * Output order: Lbyte*8 elements of 192 bits
   */
  const uint64_t Lbyte = L/8;
  CASSERT(Lslice >= Lbyte, "Lslice must be larger than L/8");
  uint64_t* o64 = (uint64_t*)out;

  if ((L%192) == 0)
  {
    uint64_t* x64 = (uint64_t*)in;
    
    // x64[0..Lbyte/8-1] hold the first row
    // x64[Lbyte/8..2Lbyte/8-1] the second and so on, until
    // x64[191Lbyte/8..192Lbyte/8-1] hold the last row
    // x64[0..1] hold the first output row
    // x64[2..3] hold the second output row and so on, until
    // x64[(L-1)*2..L*2-1] hold the last output row

    // we split the whole matrix into Lbyte/24 sub-matrices (192x192 bit) 
    for (size_t i = 0; i < Lbyte/24; i++)
    {
      // first copy into out
      for (size_t j = 0; j < 64*9; j++)
      {
        o64[j + i*64*9] = x64[(j%3) + (j/3)*Lslice/8 + i*3];
      }
      transpose_192_192_ref(&o64[i*64*9]);
    }

  } else {

    uint8_t* x8 = (uint8_t*)in;
    uint8_t* o8 = (uint8_t*)out;
    // x8[0..Lbyte-1] hold the first row
    // x8[Lbyte..2Lbyte-1] the second and so on, until
    // x8[191Lbyte..192Lbyte-1] hold the last row
    // o8[0..15] hold the first output row
    // o8[16..31] hold the second output row and so on, until
    // o8[(L-1)*16..L*16-1] hold the last output row
    
    for (size_t i = 0; i < Lbyte/24; i++)
    {
      // copy 192x192 submatrix to output
      for (size_t j = 0; j < 192*192/8; j++)
      {
        o8[j + i*192*24] = x8[(j%24) + (j/24)*Lslice + i*24];
      }
      transpose_192_192_ref(&o64[i*64*9]);
    }

    // now transpose the remainder bytes byte-wise
    for (size_t i = (Lbyte/24)*24; i < Lbyte; i++)
    {
      for (size_t j = 0; j < 192/8; j++)
      {
        // copy this 8x8 matrix to its destination, then transpose
        for (size_t k = 0; k < 8; k++)
        {
          o8[24*8*i + 24*k + j] = x8[i + Lslice*8*j + Lslice*k];
        }
        transpose8(&o8[24*8*i + j], 24);
      }
    }
    
  }
}

EXPORT void transpose_256_L_naive(void* out, const void* in, uint64_t L, uint64_t Lslice)
{
  CASSERT(out != in);
  CREQUIRE((L & 7) == 0, "L must be a multiple of 8");
  /*
   * Input order:  lambda elements of Lbyte bytes
   * Output order: Lbyte*8 elements of lambda bits
   */
  const uint64_t Lbyte = L/8;
  CREQUIRE(Lslice >= Lbyte, "Lslice must be larger than L/8");
  const uint8_t* x = (uint8_t*)in;
  // x[0..Lbyte-1] hold the first row
  // x[Lbyte..2Lbyte-1] the second and so on, until
  // x[255Lbyte..256Lbyte-1] hold the last row
  uint8_t *r = out;
  // r[0..31] hold the first output row
  // r[32..64] hold the second output row and so on, until
  // r[(L-1)*32..L*32-1] hold the last output row
  for (uint64_t i = 0; i < 256*Lbyte; i++)
  {
    r[i] = 0;
  }
  for (uint64_t i = 0; i < L; ++i) 
  {
    for (uint64_t j = 0; j < 256; ++j) 
    {
      // get the i-th bit in the j-th input row
      uint8_t tmp = (x[j*Lslice+i/8] >> (i%8)) & 1;
      // store it to the j-th output bit of the i-th output row
      r[i*32+j/8] |= tmp << (j%8);
    }
  }
}

EXPORT void transpose_256_L_ref(void* out, const void* in, uint64_t L, uint64_t Lslice)
{
  CASSERT(out != in);
  CREQUIRE((L & 7) == 0, "L must be a multiple of 8");
  /*
   * Input order:  256 elements of Lbyte bytes
   * Output order: Lbyte*8 elements of 256 bits
   */
  const uint64_t Lbyte = L/8;
  CASSERT(Lslice >= Lbyte, "Lslice must be larger than L/8");
  uint64_t* o64 = (uint64_t*)out;

  if ((L%256) == 0)
  {
    uint64_t* x64 = (uint64_t*)in;
    
    // x64[0..Lbyte/8-1] hold the first row
    // x64[Lbyte/8..2Lbyte/8-1] the second and so on, until
    // x64[255Lbyte/8..256Lbyte/8-1] hold the last row
    // x64[0..1] hold the first output row
    // x64[2..3] hold the second output row and so on, until
    // x64[(L-1)*2..L*2-1] hold the last output row

    // we split the whole matrix into Lbyte/32 sub-matrices (256x256 bit) 
    for (size_t i = 0; i < Lbyte/32; i++)
    {
      // first copy into out
      for (size_t j = 0; j < 64*16; j++)
      {
        o64[j + i*64*16] = x64[(j%4) + (j/4)*Lslice/8 + i*4];
      }
      transpose_256_256_ref(&o64[i*64*16]);
    }

  } else {

    uint8_t* x8 = (uint8_t*)in;
    uint8_t* o8 = (uint8_t*)out;
    // x8[0..Lbyte-1] hold the first row
    // x8[Lbyte..2Lbyte-1] the second and so on, until
    // x8[255Lbyte..256Lbyte-1] hold the last row
    // o8[0..15] hold the first output row
    // o8[16..31] hold the second output row and so on, until
    // o8[(L-1)*16..L*16-1] hold the last output row
    
    for (size_t i = 0; i < Lbyte/32; i++)
    {
      // copy 256x256 submatrix to output
      for (size_t j = 0; j < 256*256/8; j++)
      {
        o8[j + i*256*32] = x8[(j%32) + (j/32)*Lslice + i*32];
      }
      transpose_256_256_ref(&o64[i*64*16]);
    }

    // now transpose the remainder bytes byte-wise
    for (size_t i = (Lbyte/32)*32; i < Lbyte; i++)
    {
      for (size_t j = 0; j < 256/8; j++)
      {
        // copy this 8x8 matrix to its destination, then transpose
        for (size_t k = 0; k < 8; k++)
        {
          o8[32*8*i + 32*k + j] = x8[i + Lslice*8*j + Lslice*k];
        }
        transpose8(&o8[32*8*i + j], 32);
      }
    }
    
  }
}