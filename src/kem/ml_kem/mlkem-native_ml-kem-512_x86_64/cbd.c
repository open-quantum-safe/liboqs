/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#include "cbd.h"
#include <stdint.h>

/* Static namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define load32_littleendian MLKEM_NAMESPACE(load32_littleendian)
#define load24_littleendian MLKEM_NAMESPACE(load24_littleendian)
#define cbd2 MLKEM_NAMESPACE(cbd2)
#define cbd3 MLKEM_NAMESPACE(cbd3)
/* End of static namespacing */

/*************************************************
 * Name:        load32_littleendian
 *
 * Description: load 4 bytes into a 32-bit integer
 *              in little-endian order
 *
 * Arguments:   - const uint8_t *x: pointer to input byte array
 *
 * Returns 32-bit unsigned integer loaded from x
 **************************************************/
static uint32_t load32_littleendian(const uint8_t x[4])
{
  uint32_t r;
  r = (uint32_t)x[0];
  r |= (uint32_t)x[1] << 8;
  r |= (uint32_t)x[2] << 16;
  r |= (uint32_t)x[3] << 24;
  return r;
}

#if MLKEM_ETA1 == 3
/*************************************************
 * Name:        load24_littleendian
 *
 * Description: load 3 bytes into a 32-bit integer
 *              in little-endian order.
 *              This function is only needed for ML-KEM-512
 *
 * Arguments:   - const uint8_t *x: pointer to input byte array
 *
 * Returns 32-bit unsigned integer loaded from x (most significant byte is zero)
 **************************************************/
static uint32_t load24_littleendian(const uint8_t x[3])
{
  uint32_t r;
  r = (uint32_t)x[0];
  r |= (uint32_t)x[1] << 8;
  r |= (uint32_t)x[2] << 16;
  return r;
}
#endif /* MLKEM_ETA1 == 3 */

/*************************************************
 * Name:        cbd2
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter eta=2
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static void cbd2(poly *r, const uint8_t buf[2 * MLKEM_N / 4])
{
  unsigned i;
  for (i = 0; i < MLKEM_N / 8; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N / 8)
    invariant(array_abs_bound(r->coeffs, 0, 8 * i, 3)))
  {
    unsigned j;
    uint32_t t = load32_littleendian(buf + 4 * i);
    uint32_t d = t & 0x55555555;
    d += (t >> 1) & 0x55555555;

    for (j = 0; j < 8; j++)
    __loop__(
      invariant(i >= 0 && i <= MLKEM_N / 8 && j >= 0 && j <= 8)
      invariant(array_abs_bound(r->coeffs, 0, 8 * i + j, 3)))
    {
      const int16_t a = (d >> (4 * j + 0)) & 0x3;
      const int16_t b = (d >> (4 * j + 2)) & 0x3;
      r->coeffs[8 * i + j] = a - b;
    }
  }
}

#if MLKEM_ETA1 == 3
/*************************************************
 * Name:        cbd3
 *
 * Description: Given an array of uniformly random bytes, compute
 *              polynomial with coefficients distributed according to
 *              a centered binomial distribution with parameter eta=3.
 *              This function is only needed for ML-KEM-512
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *buf: pointer to input byte array
 **************************************************/
static void cbd3(poly *r, const uint8_t buf[3 * MLKEM_N / 4])
{
  unsigned i;
  for (i = 0; i < MLKEM_N / 4; i++)
  __loop__(
    invariant(i >= 0 && i <= MLKEM_N / 4)
    invariant(array_abs_bound(r->coeffs, 0, 4 * i, 4)))
  {
    unsigned j;
    const uint32_t t = load24_littleendian(buf + 3 * i);
    uint32_t d = t & 0x00249249;
    d += (t >> 1) & 0x00249249;
    d += (t >> 2) & 0x00249249;

    for (j = 0; j < 4; j++)
    __loop__(
      invariant(i >= 0 && i <= MLKEM_N / 4 && j >= 0 && j <= 4)
      invariant(array_abs_bound(r->coeffs, 0, 4 * i + j, 4)))
    {
      const int16_t a = (d >> (6 * j + 0)) & 0x7;
      const int16_t b = (d >> (6 * j + 3)) & 0x7;
      r->coeffs[4 * i + j] = a - b;
    }
  }
}
#endif /* MLKEM_ETA1 == 3 */

MLKEM_NATIVE_INTERNAL_API
void poly_cbd_eta1(poly *r, const uint8_t buf[MLKEM_ETA1 * MLKEM_N / 4])
{
#if MLKEM_ETA1 == 2
  cbd2(r, buf);
#elif MLKEM_ETA1 == 3
  cbd3(r, buf);
#else
#error "This implementation requires eta1 in {2,3}"
#endif
}

#if MLKEM_K == 2 || MLKEM_K == 4
MLKEM_NATIVE_INTERNAL_API
void poly_cbd_eta2(poly *r, const uint8_t buf[MLKEM_ETA2 * MLKEM_N / 4])
{
#if MLKEM_ETA2 == 2
  cbd2(r, buf);
#else
#error "This implementation requires eta2 = 2"
#endif
}
#endif /* MLKEM_K == 2 || MLKEM_K == 4 */
