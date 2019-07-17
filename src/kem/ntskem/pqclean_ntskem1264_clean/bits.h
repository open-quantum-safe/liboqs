/**
 *  bits.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  Bit manipulation helpers
 *
 *  References:
 *    [1] Jörg Arndt, "Matters Computational: Ideas, Algorithms, Source Code",
 *        Springer-Verlag Berlin Heidelberg, 2011
 *    [2] Thomas Pornin, BearSSL, https://bearssl.org
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef _BITS_H
#define _BITS_H

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#undef BITS_PER_LONG
#if ULONG_MAX == 0xffffffffUL
#	define	BITS_PER_LONG	32
#elif ULONG_MAX == 0xffffffffffffffffUL
#	define	BITS_PER_LONG	64
#else
#	error	Sorry, this architecture is not supported yet
#endif

/*
 * The following two definitions are important!
 */
#define BITSIZE				BITS_PER_LONG

/*
 * Values for LOG2, MOD and type of 'packed_t'
 */
#if   BITSIZE == 8
#	define	LOG2						3
#	define  MOD							0x7
#	define  ONE							0x1U
#   define  ALL_ONES                    0xFFU
	typedef uint8_t				packed_t;
#elif BITSIZE == 16
#   define  LOG2						4
#   define	MOD							0xF
#   define	ONE							0x1U
#   define  ALL_ONES                    0xFFFFU
	typedef uint16_t			packed_t;
#elif BITSIZE == 32
#   define  LOG2						5
#   define  MOD							0x1F
#   define  ONE							0x1U
#   define  ALL_ONES                    0xFFFFFFFFU
	typedef uint32_t			packed_t;
#elif BITSIZE == 64
#   define  LOG2						6
#   define  MOD							0x3F
#   define  ONE							0x1ULL
#   define  ALL_ONES                    0xFFFFFFFFFFFFFFFFULL
	typedef uint64_t			packed_t;
#else
#   error   Unknown BITSIZE, valid values are 8, 16, 32 and 64
#endif

#define ALIGNMENT   64 /* used for row-alignment */

/*
 * Elementary bitops
 */
#define is_bit_set(v,i)          ((v)[(i)>>LOG2]  &  (1ULL<<((i) & MOD)))
#define bit_set(v,i)              (v)[(i)>>LOG2] |=  (1ULL<<((i) & MOD))
#define bit_clear(v,i)            (v)[(i)>>LOG2] &= ~(1ULL<<((i) & MOD))
#define bit_toggle(v,i)           (v)[(i)>>LOG2] ^=  (1ULL<<((i) & MOD))
#define bit_set_value(v,i,a)      (v)[(i)>>LOG2] |=  ((a)<<((i) & MOD))
#define bit_toggle_value(v,i,a)   (v)[(i)>>LOG2] ^=  ((a)<<((i) & MOD))
#define bit_value(v,i)          (((v)[(i)>>LOG2] & (1ULL<<((i) & MOD))) >> ((i) & MOD))

static inline uint32_t vector_popcount(uint64_t a)
{
    uint64_t y;
    
    y = a;
    y -= ((y >> 1) & 0x5555555555555555ull);
    y = (y & 0x3333333333333333ull) + (y >> 2 & 0x3333333333333333ull);
    y = ((y + (y >> 4)) & 0xf0f0f0f0f0f0f0full) * 0x101010101010101ull >> 56;
    
    return (uint32_t)y;
}

#define popcount(x) vector_popcount(x)

/*
 * Returns the index of the highest set bit
 * Return -1 if no bit is set
 */
static __inline
#if BITS_PER_LONG >= 64
int64_t highest_bit_idx(packed_t x) {
    int64_t r = 0;
#else
int32_t highest_bit_idx(packed_t x) {
    int32_t r = 0;
#endif
    if (!x) return -1;
#if BITS_PER_LONG >= 64
    if ( x & 0xffffffff00000000UL ) { x >>= 32; r += 32; }
#endif
    if ( x & 0xffff0000UL ) { x >>= 16; r += 16; }
    if ( x & 0x0000ff00UL ) { x >>= 8; r += 8; }
    if ( x & 0x000000f0UL ) { x >>= 4; r += 4; }
    if ( x & 0x0000000cUL ) { x >>= 2; r += 2; }
    if ( x & 0x00000002UL ) { r += 1; }
    return r;
}
/*
 * Returns the index of the lowest set bit
 * Return -1 if no bit is set
 */
static __inline
#if BITS_PER_LONG >= 64
uint64_t lowest_bit_idx(packed_t x) {
	uint64_t r = 0;
#else
int32_t lowest_bit_idx(packed_t x) {
	uint32_t r = 0;
#endif
	if (!x) return -1;
	x &= -x;
#if BITS_PER_LONG >= 64
	if ( x & 0xffffffff00000000UL ) r += 32;
	if ( x & 0xffff0000ffff0000UL ) r += 16;
	if ( x & 0xff00ff00ff00ff00UL ) r +=  8;
	if ( x & 0xf0f0f0f0f0f0f0f0UL ) r +=  4;
	if ( x & 0xccccccccccccccccUL ) r +=  2;
	if ( x & 0xaaaaaaaaaaaaaaaaUL ) r +=  1;
#else
	if ( x & 0xffff0000UL ) r += 16;
	if ( x & 0xff00ff00UL ) r +=  8;
	if ( x & 0xf0f0f0f0UL ) r +=  4;
	if ( x & 0xccccccccUL ) r +=  2;
	if ( x & 0xaaaaaaaaUL ) r +=  1;
#endif
    return r;
}

/**
 *  The following routines are generally used for
 *  constant-time programming
 **/

/**
 *  Negate a boolean value 'b'
 *
 *  @param[in] b   Input b, a boolean
 *  @return negation of 'b'
 **/
static inline uint32_t CT_not(uint32_t b)
{
    return b ^ 1;
}
    
/**
 *  Return the minimum between 'a' and 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return a if a < b, otherwise b is returned
 **/
static inline int32_t CT_min(int32_t a, int32_t b)
{
    return b + ((a - b) & ((a - b) >> 31));
}

/**
 *  Return the maximum between 'a' and 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return a if a > b, otherwise b is returned
 **/
static inline int32_t CT_max(int32_t a, int32_t b)
{
    return a - ((a - b) & ((a - b) >> 31));
}

/**
 *  Return 1 if 'a' is equal to 0
 *
 *  @param[in] a    Input a
 *  @return 1 if a == 0, otherwise 0
 **/
static inline uint32_t CT_is_equal_zero(int32_t a)
{
    uint32_t b = (uint32_t)a;
    return ~(b | -b) >> 31;
}

/**
 *  Return 1 if 'a' is greater than 0
 *
 *  @param[in] a    Input a
 *  @return 1 if a > 0, otherwise 0
 **/
static inline uint32_t CT_is_greater_than_zero(int32_t a)
{
    uint32_t b = (uint32_t)a;
    return (~b & -b) >> 31;
}

/**
 *  Return 1 if 'a' is less than 0
 *
 *  @param[in] a    Input a
 *  @return 1 if a < 0, otherwise 0
 **/
static inline uint32_t CT_is_less_than_zero(int32_t a)
{
    return (uint32_t)a >> 31;
}

/**
 *  Return 1 if 'a' is greater than or equal to 0
 *
 *  @param[in] a    Input a
 *  @return 1 if a >= 0, otherwise 0
 **/
static inline uint32_t CT_is_greater_than_or_equal_to_zero(int32_t a)
{
    return ~(uint32_t)a >> 31;
}

/**
 *  Return 1 if 'a' is greater than or equal to 0
 *
 *  @param[in] a    Input a
 *  @return 1 if a <= 0, otherwise 0
 **/
static inline uint32_t CT_is_less_than_or_equal_to_zero(int32_t a)
{
    uint32_t b = (uint32_t)a;
    return (b | ~-b) >> 31;
}

/**
 *  Return 1 if 'a' is not equal to 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return 1 if a != b, otherwise 0
 **/
static inline uint32_t CT_is_not_equal(uint32_t a, uint32_t b)
{
    uint32_t c = a ^ b;
    return ((c | -c) >> 31);
}

/**
 *  Return 1 if 'a' is equal to 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return 1 if a == b, otherwise 0
 **/
static inline uint32_t CT_is_equal(uint32_t a, uint32_t b)
{
    return CT_not(CT_is_not_equal(a, b));
}

/**
 *  Return 1 if 'a' is less than 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return 1 if a < b, otherwise 0
 **/
static inline uint32_t CT_is_less_than(uint32_t a, uint32_t b)
{
    uint32_t c = a - b;
    return (c ^ ((a ^ b) & (b ^ c))) >> 31;
}

/**
 *  Return 1 if 'a' is greater than 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return 1 if a > b, otherwise 0
 **/
static inline uint32_t CT_is_greater_than(uint32_t a, uint32_t b)
{
    uint32_t c = b - a;
    return (c ^ ((a ^ b) & (a ^ c))) >> 31;
}

/**
 *  Return 1 if 'a' is less than or equal to 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return 1 if a <= b, otherwise 0
 **/
static inline uint32_t CT_is_less_than_or_equal_to(uint32_t a, uint32_t b)
{
    return CT_not(CT_is_greater_than(a, b));
}

/**
 *  Return 1 if 'a' is greater than or equal to 'b'
 *
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return 1 if a >= b, otherwise 0
 **/
static inline uint32_t CT_is_greater_than_or_equal_to(uint32_t a, uint32_t b)
{
    return CT_not(CT_is_greater_than(b, a));
}

/**
 *  Multiplexer: select 'a' or 'b' depending on 'ctl'
 *
 *  @param[in] ctl  Control value, either 0 or 1
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return a if ctl is 1, otherwise b is returned
 **/
static inline uint32_t CT_mux(uint32_t ctl, uint32_t a, uint32_t b)
{
    return b ^ (-ctl & (a ^ b));
}

/**
 *  Multiplexer: select 'a' or 'b' depending on 'ctl'
 *
 *  @param[in] ctl  Control value, either 0 or 1
 *  @param[in] a    Input a
 *  @param[in] b    Input b
 *  @return a if ctl is 1, otherwise b is returned
 **/
static inline uint64_t CT_mux64(uint64_t ctl, uint64_t a, uint64_t b)
{
    return b ^ (-ctl & (a ^ b));
}
    
/**
 *  Is 'a' a power of 2?
 *
 *  @param[in] a    Input a
 *  @return 1 if a = 2^m for some integer m, otherwise 0
 **/
static inline uint32_t CT_is_power_of_2(uint32_t a)
{
    return CT_is_equal_zero(a & (a - 1));
}

/**
 *  Compare two vectors in constant-time
 *
 *  @param[in] a    Input vector a
 *  @param[in] b    Input vector b
 *  @param[in] len  The length of the vector
 *  @return 1 if both vectors are the same, otherwise 0
 **/
static inline uint32_t CT_is_vector_equal(const void* a, const void* b, size_t len)
{
    size_t i;
    int32_t l, neq_sum = 0;
    uint32_t *a_ptr = (uint32_t *)a;
    uint32_t *b_ptr = (uint32_t *)b;
    uint8_t *a8_ptr = (uint8_t *)a;
    uint8_t *b8_ptr = (uint8_t *)b;
    l = (int32_t)len/sizeof(uint32_t);
    for (i=0; i<(size_t) l; i++) {
        neq_sum += CT_is_not_equal(*a_ptr++, *b_ptr++);
    }
    a8_ptr += (l*sizeof(uint32_t));
    b8_ptr += (l*sizeof(uint32_t));
    for (i=l*sizeof(uint32_t); i<len; i++) {
        neq_sum += CT_is_not_equal((uint32_t)*a8_ptr++, (uint32_t)*b8_ptr++);
    }
    return CT_is_equal_zero(neq_sum);
}
    
#endif /* _BITS_H */
