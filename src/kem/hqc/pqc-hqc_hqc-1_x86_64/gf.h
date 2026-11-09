/**
 * @file gf.h
 * @brief GF(2^8) constants and helpers for the field defined by @c PARAM_GF_POLY.
 */

#ifndef HQC_GF_H
#define HQC_GF_H

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief  Build a 256-bit AVX2 register from two 128-bit halves.
 *
 * Takes two __m128i values and constructs a __m256i where
 * - the upper 128 bits come from \p v0, and
 * - the lower 128 bits come from \p v1.
 *
 * @param v0  The __m128i to use for the high 128 bits.
 * @param v1  The __m128i to use for the low 128 bits.
 * @return    A __m256i with [ high = v0 | low = v1 ].
 */
#define _mm256_set_m128i(v0, v1) _mm256_insertf128_si256(_mm256_castsi128_si256(v1), (v0), 1)

/**
 * @brief Primitive element used to enumerate the non-zero field elements.
 */
static const uint16_t gf_alpha = 0x03;

/**
 * Powers of the primitive element ::gf_alpha in GF(2^8) defined by @c PARAM_GF_POLY.
 * The last two elements are needed by the gf_mul function
 * (for example if both elements to multiply are zero).
 */
static const uint16_t gf_exp[258] = {
    1,   3,   5,   15,  17,  51,  85,  255, 26,  46,  114, 150, 161, 248, 19,  53,  95,  225, 56,  72,  216, 115,
    149, 164, 247, 2,   6,   10,  30,  34,  102, 170, 229, 52,  92,  228, 55,  89,  235, 38,  106, 190, 217, 112,
    144, 171, 230, 49,  83,  245, 4,   12,  20,  60,  68,  204, 79,  209, 104, 184, 211, 110, 178, 205, 76,  212,
    103, 169, 224, 59,  77,  215, 98,  166, 241, 8,   24,  40,  120, 136, 131, 158, 185, 208, 107, 189, 220, 127,
    129, 152, 179, 206, 73,  219, 118, 154, 181, 196, 87,  249, 16,  48,  80,  240, 11,  29,  39,  105, 187, 214,
    97,  163, 254, 25,  43,  125, 135, 146, 173, 236, 47,  113, 147, 174, 233, 32,  96,  160, 251, 22,  58,  78,
    210, 109, 183, 194, 93,  231, 50,  86,  250, 21,  63,  65,  195, 94,  226, 61,  71,  201, 64,  192, 91,  237,
    44,  116, 156, 191, 218, 117, 159, 186, 213, 100, 172, 239, 42,  126, 130, 157, 188, 223, 122, 142, 137, 128,
    155, 182, 193, 88,  232, 35,  101, 175, 234, 37,  111, 177, 200, 67,  197, 84,  252, 31,  33,  99,  165, 244,
    7,   9,   27,  45,  119, 153, 176, 203, 70,  202, 69,  207, 74,  222, 121, 139, 134, 145, 168, 227, 62,  66,
    198, 81,  243, 14,  18,  54,  90,  238, 41,  123, 141, 140, 143, 138, 133, 148, 167, 242, 13,  23,  57,  75,
    221, 124, 132, 151, 162, 253, 28,  36,  108, 180, 199, 82,  246, 1,   3,   5};

/**
 * Logarithm of the elements of GF(2^8) to the base ::gf_alpha.
 * The logarithm of 0 is set to 0 by convention.
 */
static const uint16_t gf_log[256] = {
    0,   0,   25,  1,   50,  2,   26,  198, 75,  199, 27,  104, 51,  238, 223, 3,   100, 4,   224, 14,  52,  141,
    129, 239, 76,  113, 8,   200, 248, 105, 28,  193, 125, 194, 29,  181, 249, 185, 39,  106, 77,  228, 166, 114,
    154, 201, 9,   120, 101, 47,  138, 5,   33,  15,  225, 36,  18,  240, 130, 69,  53,  147, 218, 142, 150, 143,
    219, 189, 54,  208, 206, 148, 19,  92,  210, 241, 64,  70,  131, 56,  102, 221, 253, 48,  191, 6,   139, 98,
    179, 37,  226, 152, 34,  136, 145, 16,  126, 110, 72,  195, 163, 182, 30,  66,  58,  107, 40,  84,  250, 133,
    61,  186, 43,  121, 10,  21,  155, 159, 94,  202, 78,  212, 172, 229, 243, 115, 167, 87,  175, 88,  168, 80,
    244, 234, 214, 116, 79,  174, 233, 213, 231, 230, 173, 232, 44,  215, 117, 122, 235, 22,  11,  245, 89,  203,
    95,  176, 156, 169, 81,  160, 127, 12,  246, 111, 23,  196, 73,  236, 216, 67,  31,  45,  164, 118, 123, 183,
    204, 187, 62,  90,  251, 96,  177, 134, 59,  82,  161, 108, 170, 85,  41,  157, 151, 178, 135, 144, 97,  190,
    220, 252, 188, 149, 207, 205, 55,  63,  91,  209, 83,  57,  132, 60,  65,  162, 109, 71,  20,  42,  158, 93,
    86,  242, 211, 171, 68,  17,  146, 217, 35,  32,  46,  137, 180, 124, 184, 38,  119, 153, 227, 165, 103, 74,
    237, 222, 197, 49,  254, 24,  13,  99,  140, 128, 192, 247, 112, 7};

/**
 * @brief Masks needed for the computation of 16-way multiplication in GF(2^8)
 */
static const __m256i mr0 = {0x0100010001000100UL, 0x0100010001000100UL, 0x0100010001000100UL,
                            0x0100010001000100UL}; /**< Replicates the 0x0100 coefficient mask in each 64-bit lane for
                                                      the low-half multiply step. */

static const __m256i lastMask = {
    0x00ff00ff00ff00ffUL, 0x00ff00ff00ff00ffUL, 0x00ff00ff00ff00ffUL,
    0x00ff00ff00ff00ffUL}; /**< Selects the lowest byte of each 16-bit element (used to mask off high bits). */

static const __m128i maskl = {0x0000ffff0000ffffUL,
                              0x0000ffff0000ffffUL}; /**< 128-bit mask for the lower 16 bits of each 32-bit lane. */

static const __m128i maskh = {0xffff0000ffff0000UL,
                              0xffff0000ffff0000UL}; /**< 128-bit mask for the upper 16 bits of each 32-bit lane. */

static const __m128i indexh = {
    0xffffffffffffffffUL,
    0x0d0c090805040100UL}; /**< Shuffle indices for extracting the high halves in the interleaved layout. */

static const __m128i indexl = {
    0x0d0c090805040100UL,
    0xffffffffffffffffUL}; /**< Shuffle indices for extracting the low halves in the interleaved layout. */

static const __m128i middlemaskl = {
    0x000000000000ffffUL, 0x000000000000ffffUL}; /**< Mask for the lower 16 bits of the middle 32-bit lanes. */

static const __m128i middlemaskh = {
    0x0000ffff00000000UL, 0x0000ffff00000000UL}; /**< Mask for the upper 16 bits of the middle 32-bit lanes. */

/**
 * @brief x^i modulo @c PARAM_GF_POLY, duplicated four times to fit a 256-bit register.
 */
static const __m256i red[7] = {
    {0x001b001b001b001bUL, 0x001b001b001b001bUL, 0x001b001b001b001bUL, 0x001b001b001b001bUL},
    {0x0036003600360036UL, 0x0036003600360036UL, 0x0036003600360036UL, 0x0036003600360036UL},
    {0x006c006c006c006cUL, 0x006c006c006c006cUL, 0x006c006c006c006cUL, 0x006c006c006c006cUL},
    {0x00d800d800d800d8UL, 0x00d800d800d800d8UL, 0x00d800d800d800d8UL, 0x00d800d800d800d8UL},
    {0x00ab00ab00ab00abUL, 0x00ab00ab00ab00abUL, 0x00ab00ab00ab00abUL, 0x00ab00ab00ab00abUL},
    {0x004d004d004d004dUL, 0x004d004d004d004dUL, 0x004d004d004d004dUL, 0x004d004d004d004dUL},
    {0x009a009a009a009aUL, 0x009a009a009a009aUL, 0x009a009a009a009aUL, 0x009a009a009a009aUL},

};

void gf_generate(uint16_t *exp, uint16_t *log, const int16_t m);

uint16_t gf_mul(uint16_t a, uint16_t b);
__m256i gf_mul_vect(__m256i a, __m256i b);
uint16_t gf_square(uint16_t a);
uint16_t gf_inverse(uint16_t a);

#endif  // HQC_GF_H
