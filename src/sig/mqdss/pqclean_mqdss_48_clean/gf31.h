#ifndef MQDSS_GF31_H
#define MQDSS_GF31_H

#include <stddef.h>
#include <stdint.h>

typedef uint16_t gf31;

/* This performs a full unique reduction mod 13 on x; x can be any unsigned
   16-bit integer (i.e. in the range [0, 65535]) */
gf31 PQCLEAN_MQDSS48_CLEAN_mod31(gf31 x);

/* Given a vector of elements in the range [0, 31], this reduces the elements
   to the range [0, 30] by mapping 31 to 0 (i.e reduction mod 31) */
void PQCLEAN_MQDSS48_CLEAN_vgf31_unique(gf31 *out, const gf31 *in);

/* Given a vector of 16-bit integers (i.e. in [0, 65535], this reduces the
   elements to the range [0, 30] by mapping 31 to 0 (i.e reduction mod 31) */
void PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(gf31 *out, const gf31 *in);

/* Given a seed, samples len gf31 elements (in the range [0, 30]), and places
   them in a vector of 16-bit elements */
void PQCLEAN_MQDSS48_CLEAN_gf31_nrand(gf31 *out, int len, const unsigned char *seed, size_t seedlen);

/* Given a seed, samples len gf31 elements, transposed into unsigned range,
   i.e. in the range [-15, 15], and places them in an array of 8-bit integers.
   This is used for the expansion of F, which wants packed elements. */
void PQCLEAN_MQDSS48_CLEAN_gf31_nrand_schar(signed char *out, int len, const unsigned char *seed, size_t seedlen);

/* Unpacks an array of packed GF31 elements to one element per gf31.
   Assumes that there is sufficient empty space available at the end of the
   array to unpack. Can perform in-place. */
void PQCLEAN_MQDSS48_CLEAN_gf31_nunpack(gf31 *out, const unsigned char *in, unsigned int n);

/* Packs an array of GF31 elements from gf31's to concatenated 5-bit values.
   Assumes that there is sufficient space available to unpack.
   Can perform in-place. */
void PQCLEAN_MQDSS48_CLEAN_gf31_npack(unsigned char *out, const gf31 *in, unsigned int n);

#endif
