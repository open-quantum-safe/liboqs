/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef COMPRESS_H
#define COMPRESS_H

#include <stddef.h>
#include <stdint.h>
#include "cbmc.h"
#include "common.h"
#include "debug.h"
#include "poly.h"
#include "verify.h"

/* Static namespacing
 * This is to facilitate building multiple instances
 * of mlkem-native (e.g. with varying security levels)
 * within a single compilation unit. */
#define scalar_compress_d1 MLKEM_NAMESPACE(scalar_compress_d1)
#define scalar_compress_d4 MLKEM_NAMESPACE(scalar_compress_d4)
#define scalar_compress_d5 MLKEM_NAMESPACE(scalar_compress_d5)
#define scalar_compress_d10 MLKEM_NAMESPACE(scalar_compress_d10)
#define scalar_compress_d11 MLKEM_NAMESPACE(scalar_compress_d11)
#define scalar_decompress_d4 MLKEM_NAMESPACE(scalar_decompress_d4)
#define scalar_decompress_d5 MLKEM_NAMESPACE(scalar_decompress_d5)
#define scalar_decompress_d10 MLKEM_NAMESPACE(scalar_decompress_d10)
#define scalar_decompress_d11 MLKEM_NAMESPACE(scalar_decompress_d11)
/* End of static namespacing */

/************************************************************
 * Name: scalar_compress_d1
 *
 * Description: Computes round(u * 2 / q)
 *
 *              Implements Compress_d from FIPS203, Eq (4.7),
 *              for d = 1.
 *
 * Arguments: - u: Unsigned canonical modulus modulo q
 *                 to be compressed.
 ************************************************************/
/*
 * The multiplication in this routine will exceed UINT32_MAX
 * and wrap around for large values of u. This is expected and required.
 */
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "unsigned-overflow"
#endif
static INLINE uint32_t scalar_compress_d1(uint16_t u)
__contract__(
  requires(u <= MLKEM_Q - 1)
  ensures(return_value < 2)
  ensures(return_value == (((uint32_t)u * 2 + MLKEM_Q / 2) / MLKEM_Q) % 2)  )
{
  uint32_t d0 = u << 1;
  d0 *= 645083;
  d0 += 1u << 30;
  d0 >>= 31;
  return d0;
}
#ifdef CBMC
#pragma CPROVER check pop
#endif

/************************************************************
 * Name: scalar_compress_d4
 *
 * Description: Computes round(u * 16 / q) % 16
 *
 *              Implements Compress_d from FIPS203, Eq (4.7),
 *              for d = 4.
 *
 * Arguments: - u: Unsigned canonical modulus modulo q
 *                 to be compressed.
 ************************************************************/
/*
 * The multiplication in this routine will exceed UINT32_MAX
 * and wrap around for large values of u. This is expected and required.
 */
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "unsigned-overflow"
#endif
static INLINE uint32_t scalar_compress_d4(uint16_t u)
__contract__(
  requires(u <= MLKEM_Q - 1)
  ensures(return_value < 16)
  ensures(return_value == (((uint32_t)u * 16 + MLKEM_Q / 2) / MLKEM_Q) % 16))
{
  uint32_t d0 = (uint32_t)u * 1290160; /* 16 * round(2^28 / MLKEM_Q) */
  return (d0 + (1u << 27)) >> 28;      /* round(d0/2^28) */
}
#ifdef CBMC
#pragma CPROVER check pop
#endif

/************************************************************
 * Name: scalar_decompress_d4
 *
 * Description: Computes round(u * q / 16)
 *
 *              Implements Decompress_d from FIPS203, Eq (4.8),
 *              for d = 4.
 *
 * Arguments: - u: Unsigned canonical modulus modulo 16
 *                 to be decompressed.
 ************************************************************/
static INLINE uint16_t scalar_decompress_d4(uint32_t u)
__contract__(
  requires(0 <= u && u < 16)
  ensures(return_value <= (MLKEM_Q - 1))
) { return ((u * MLKEM_Q) + 8) >> 4; }

/************************************************************
 * Name: scalar_compress_d5
 *
 * Description: Computes round(u * 32 / q) % 32
 *
 *              Implements Compress_d from FIPS203, Eq (4.7),
 *              for d = 5.
 *
 * Arguments: - u: Unsigned canonical modulus modulo q
 *                 to be compressed.
 ************************************************************/
/*
 * The multiplication in this routine will exceed UINT32_MAX
 * and wrap around for large values of u. This is expected and required.
 */
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "unsigned-overflow"
#endif
static INLINE uint32_t scalar_compress_d5(uint16_t u)
__contract__(
  requires(u <= MLKEM_Q - 1)
  ensures(return_value < 32)
  ensures(return_value == (((uint32_t)u * 32 + MLKEM_Q / 2) / MLKEM_Q) % 32)  )
{
  uint32_t d0 = (uint32_t)u * 1290176; /* 2^5 * round(2^27 / MLKEM_Q) */
  return (d0 + (1u << 26)) >> 27;      /* round(d0/2^27) */
}
#ifdef CBMC
#pragma CPROVER check pop
#endif

/************************************************************
 * Name: scalar_decompress_d5
 *
 * Description: Computes round(u * q / 32)
 *
 *              Implements Decompress_d from FIPS203, Eq (4.8),
 *              for d = 5.
 *
 * Arguments: - u: Unsigned canonical modulus modulo 32
 *                 to be decompressed.
 ************************************************************/
static INLINE uint16_t scalar_decompress_d5(uint32_t u)
__contract__(
  requires(0 <= u && u < 32)
  ensures(return_value <= MLKEM_Q - 1)
) { return ((u * MLKEM_Q) + 16) >> 5; }

/************************************************************
 * Name: scalar_compress_d10
 *
 * Description: Computes round(u * 2**10 / q) % 2**10
 *
 *              Implements Compress_d from FIPS203, Eq (4.7),
 *              for d = 10.
 *
 * Arguments: - u: Unsigned canonical modulus modulo q
 *                 to be compressed.
 ************************************************************/
/*
 * The multiplication in this routine will exceed UINT32_MAX
 * and wrap around for large values of u. This is expected and required.
 */
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "unsigned-overflow"
#endif
static INLINE uint32_t scalar_compress_d10(uint16_t u)
__contract__(
  requires(u <= MLKEM_Q - 1)
  ensures(return_value < (1u << 10))
  ensures(return_value == (((uint32_t)u * (1u << 10) + MLKEM_Q / 2) / MLKEM_Q) % (1 << 10)))
{
  uint64_t d0 = (uint64_t)u * 2642263040; /* 2^10 * round(2^32 / MLKEM_Q) */
  d0 = (d0 + ((uint64_t)1u << 32)) >> 33;
  return (d0 & 0x3FF);
}
#ifdef CBMC
#pragma CPROVER check pop
#endif

/************************************************************
 * Name: scalar_decompress_d10
 *
 * Description: Computes round(u * q / 1024)
 *
 *              Implements Decompress_d from FIPS203, Eq (4.8),
 *              for d = 10.
 *
 * Arguments: - u: Unsigned canonical modulus modulo 16
 *                 to be decompressed.
 ************************************************************/
static INLINE uint16_t scalar_decompress_d10(uint32_t u)
__contract__(
  requires(0 <= u && u < 1024)
  ensures(return_value <= (MLKEM_Q - 1))
) { return ((u * MLKEM_Q) + 512) >> 10; }

/************************************************************
 * Name: scalar_compress_d11
 *
 * Description: Computes round(u * 2**11 / q) % 2**11
 *
 *              Implements Compress_d from FIPS203, Eq (4.7),
 *              for d = 11.
 *
 * Arguments: - u: Unsigned canonical modulus modulo q
 *                 to be compressed.
 ************************************************************/
/*
 * The multiplication in this routine will exceed UINT32_MAX
 * and wrap around for large values of u. This is expected and required.
 */
#ifdef CBMC
#pragma CPROVER check push
#pragma CPROVER check disable "unsigned-overflow"
#endif
static INLINE uint32_t scalar_compress_d11(uint16_t u)
__contract__(
  requires(u <= MLKEM_Q - 1)
  ensures(return_value < (1u << 11))
  ensures(return_value == (((uint32_t)u * (1u << 11) + MLKEM_Q / 2) / MLKEM_Q) % (1 << 11)))
{
  uint64_t d0 = (uint64_t)u * 5284526080; /* 2^11 * round(2^33 / MLKEM_Q) */
  d0 = (d0 + ((uint64_t)1u << 32)) >> 33;
  return (d0 & 0x7FF);
}
#ifdef CBMC
#pragma CPROVER check pop
#endif

/************************************************************
 * Name: scalar_decompress_d11
 *
 * Description: Computes round(u * q / 1024)
 *
 *              Implements Decompress_d from FIPS203, Eq (4.8),
 *              for d = 10.
 *
 * Arguments: - u: Unsigned canonical modulus modulo 16
 *                 to be decompressed.
 ************************************************************/
static INLINE uint16_t scalar_decompress_d11(uint32_t u)
__contract__(
  requires(0 <= u && u < 2048)
  ensures(return_value <= (MLKEM_Q - 1))
) { return ((u * MLKEM_Q) + 1024) >> 11; }

#if defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || \
    (MLKEM_K == 2 || MLKEM_K == 3)
#define poly_compress_d4 MLKEM_NAMESPACE(poly_compress_d4)
/*************************************************
 * Name:        poly_compress_d4
 *
 * Description: Compression (4 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D4 bytes)
 *              - const poly *a: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_compress_d4(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D4], const poly *a);

#define poly_compress_d10 MLKEM_NAMESPACE(poly_compress_d10)
/*************************************************
 * Name:        poly_compress_d10
 *
 * Description: Compression (10 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D10 bytes)
 *              - const poly *a: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_compress_d10(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D10], const poly *a);

#define poly_decompress_d4 MLKEM_NAMESPACE(poly_decompress_d4)
/*************************************************
 * Name:        poly_decompress_d4
 *
 * Description: De-serialization and subsequent decompression (dv bits) of a
 *              polynomial; approximate inverse of poly_compress
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D4 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_decompress_d4(poly *r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D4]);

#define poly_decompress_d10 MLKEM_NAMESPACE(poly_decompress_d10)
/*************************************************
 * Name:        poly_decompress_d10
 *
 * Description: De-serialization and subsequent decompression (10 bits) of a
 *              polynomial; approximate inverse of poly_compress_d10
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D10 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_decompress_d10(poly *r,
                         const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D10]);
#endif /* defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || (MLKEM_K == 2 \
          || MLKEM_K == 3) */

#if defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 4
#define poly_compress_d5 MLKEM_NAMESPACE(poly_compress_d5)
/*************************************************
 * Name:        poly_compress_d5
 *
 * Description: Compression (5 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D5 bytes)
 *              - const poly *a: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_compress_d5(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D5], const poly *a);

#define poly_compress_d11 MLKEM_NAMESPACE(poly_compress_d11)
/*************************************************
 * Name:        poly_compress_d11
 *
 * Description: Compression (11 bits) and subsequent serialization of a
 *              polynomial
 *
 * Arguments:   - uint8_t *r: pointer to output byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D11 bytes)
 *              - const poly *a: pointer to input polynomial
 *                  Coefficients must be unsigned canonical,
 *                  i.e. in [0,1,..,MLKEM_Q-1].
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_compress_d11(uint8_t r[MLKEM_POLYCOMPRESSEDBYTES_D11], const poly *a);

#define poly_decompress_d5 MLKEM_NAMESPACE(poly_decompress_d5)
/*************************************************
 * Name:        poly_decompress_d5
 *
 * Description: De-serialization and subsequent decompression (dv bits) of a
 *              polynomial; approximate inverse of poly_compress
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D5 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_decompress_d5(poly *r, const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D5]);

#define poly_decompress_d11 MLKEM_NAMESPACE(poly_decompress_d11)
/*************************************************
 * Name:        poly_decompress_d11
 *
 * Description: De-serialization and subsequent decompression (11 bits) of a
 *              polynomial; approximate inverse of poly_compress_d11
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *a: pointer to input byte array
 *                   (of length MLKEM_POLYCOMPRESSEDBYTES_D11 bytes)
 *
 * Upon return, the coefficients of the output polynomial are unsigned-canonical
 * (non-negative and smaller than MLKEM_Q).
 *
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_decompress_d11(poly *r,
                         const uint8_t a[MLKEM_POLYCOMPRESSEDBYTES_D11]);
#endif /* defined(MLKEM_NATIVE_MULTILEVEL_BUILD_WITH_SHARED) || MLKEM_K == 4 \
        */

#define poly_tobytes MLKEM_NAMESPACE(poly_tobytes)
/*************************************************
 * Name:        poly_tobytes
 *
 * Description: Serialization of a polynomial.
 *              Signed coefficients are converted to
 *              unsigned form before serialization.
 *
 * Arguments:   INPUT:
 *              - a: const pointer to input polynomial,
 *                with each coefficient in the range [0,1,..,Q-1]
 *              OUTPUT
 *              - r: pointer to output byte array
 *                   (of MLKEM_POLYBYTES bytes)
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_tobytes(uint8_t r[MLKEM_POLYBYTES], const poly *a)
__contract__(
  requires(memory_no_alias(r, MLKEM_POLYBYTES))
  requires(memory_no_alias(a, sizeof(poly)))
  requires(array_bound(a->coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  assigns(object_whole(r))
);


#define poly_frombytes MLKEM_NAMESPACE(poly_frombytes)
/*************************************************
 * Name:        poly_frombytes
 *
 * Description: De-serialization of a polynomial.
 *
 * Arguments:   INPUT
 *              - a: pointer to input byte array
 *                   (of MLKEM_POLYBYTES bytes)
 *              OUTPUT
 *              - r: pointer to output polynomial, with
 *                   each coefficient unsigned and in the range
 *                   0 .. 4095
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_frombytes(poly *r, const uint8_t a[MLKEM_POLYBYTES])
__contract__(
  requires(memory_no_alias(a, MLKEM_POLYBYTES))
  requires(memory_no_alias(r, sizeof(poly)))
  assigns(memory_slice(r, sizeof(poly)))
  ensures(array_bound(r->coeffs, 0, MLKEM_N, 0, UINT12_LIMIT))
);


#define poly_frommsg MLKEM_NAMESPACE(poly_frommsg)
/*************************************************
 * Name:        poly_frommsg
 *
 * Description: Convert 32-byte message to polynomial
 *
 * Arguments:   - poly *r: pointer to output polynomial
 *              - const uint8_t *msg: pointer to input message
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_frommsg(poly *r, const uint8_t msg[MLKEM_INDCPA_MSGBYTES])
__contract__(
  requires(memory_no_alias(msg, MLKEM_INDCPA_MSGBYTES))
  requires(memory_no_alias(r, sizeof(poly)))
  assigns(object_whole(r))
  ensures(array_bound(r->coeffs, 0, MLKEM_N, 0, MLKEM_Q))
);

#define poly_tomsg MLKEM_NAMESPACE(poly_tomsg)
/*************************************************
 * Name:        poly_tomsg
 *
 * Description: Convert polynomial to 32-byte message
 *
 * Arguments:   - uint8_t *msg: pointer to output message
 *              - const poly *r: pointer to input polynomial
 *                Coefficients must be unsigned canonical
 **************************************************/
MLKEM_NATIVE_INTERNAL_API
void poly_tomsg(uint8_t msg[MLKEM_INDCPA_MSGBYTES], const poly *r)
__contract__(
  requires(memory_no_alias(msg, MLKEM_INDCPA_MSGBYTES))
  requires(memory_no_alias(r, sizeof(poly)))
  requires(array_bound(r->coeffs, 0, MLKEM_N, 0, MLKEM_Q))
  assigns(object_whole(msg))
);

#endif /* COMPRESS_H */
