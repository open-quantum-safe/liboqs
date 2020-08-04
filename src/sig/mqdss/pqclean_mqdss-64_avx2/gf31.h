#ifndef MQDSS_GF31_H
#define MQDSS_GF31_H

#include <stddef.h>
#include <stdint.h>

typedef unsigned short gf31;

/* Given a vector of elements in the range [0, 31], this reduces the elements
   to the range [0, 30] by mapping 31 to 0 (i.e reduction mod 31) */
void PQCLEAN_MQDSS64_AVX2_vgf31_unique(gf31 *out, gf31 *in);

/* Given a vector of 16-bit integers (i.e. in [0, 65535], this reduces the
   elements to the range [0, 30] by mapping 31 to 0 (i.e reduction mod 31) */
void PQCLEAN_MQDSS64_AVX2_vgf31_shorten_unique(gf31 *out, gf31 *in);

/* Given a seed, samples len gf31 elements (in the range [0, 30]), and places
   them in a vector of 16-bit elements */
void PQCLEAN_MQDSS64_AVX2_gf31_nrand(gf31 *out, size_t len, const uint8_t *seed, size_t seedlen);

/* Given a seed, samples len gf31 elements, transposed into unsigned range,
   i.e. in the range [-15, 15], and places them in an array of 8-bit integers.
   This is used for the expansion of F, which wants packed elements. */
void PQCLEAN_MQDSS64_AVX2_gf31_nrand_schar(signed char *out, size_t len, const uint8_t *seed, size_t seedlen);

/* Unpacks an array of packed GF31 elements to one element per gf31.
   Assumes that there is sufficient empty space available at the end of the
   array to unpack. Can perform in-place. */
void PQCLEAN_MQDSS64_AVX2_gf31_nunpack(gf31 *out, const uint8_t *in, size_t n);

/* Packs an array of GF31 elements from gf31's to concatenated 5-bit values.
   Assumes that there is sufficient space available to unpack.
   Can perform in-place. */
void PQCLEAN_MQDSS64_AVX2_gf31_npack(uint8_t *out, const gf31 *in, size_t n);

#endif
