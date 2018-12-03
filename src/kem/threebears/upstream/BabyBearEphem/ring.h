/** Ring arithmetic header */
#ifndef __THREEBEARS_RING_H__
#define __THREEBEARS_RING_H__ 1

#include "common.h"
#include "params.h"

#if defined(WORD_BITS)
#elif (defined(__ILP64__) || defined(__amd64__) || defined(__x86_64__) || defined(__aarch64__))
#define WORD_BITS 64
#else
#define WORD_BITS 32
#endif

#if WORD_BITS == 16
typedef uint16_t limb_t;
typedef int16_t slimb_t;
typedef uint32_t dlimb_t;
typedef int32_t dslimb_t;
#define LBITS 10
#elif WORD_BITS == 32
typedef uint32_t limb_t;
typedef int32_t slimb_t;
typedef uint64_t dlimb_t;
typedef int64_t dslimb_t;
#define LBITS 26
#elif WORD_BITS == 64
typedef uint64_t limb_t;
typedef int64_t slimb_t;
typedef __uint128_t dlimb_t;
typedef __int128_t dslimb_t;
#define LBITS 60
#endif

#define NLIMBS (LGX*DIGITS/LBITS)
#define LMASK  (((limb_t)1<<LBITS)-1)

typedef limb_t gf_t[NLIMBS];

#define GF_BYTES ((LGX*DIGITS+7)/8)
#define contract contract_3120
#define expand   expand_3120
#define mac      mac_3120
#define canon    canon_3120
#define modulus  modulus_3120

/* Serialize a gf_t */
void WEAK contract(uint8_t ch[GF_BYTES], gf_t ll);

/* Deserialize a gf_t */
void WEAK expand(gf_t ll, const uint8_t ch[GF_BYTES]);

/* Multiply and accumulate c = c + a*b */
void WEAK mac(gf_t c, const gf_t a, const gf_t b);

/* Reduce ring element to canonical form */
void WEAK canon(gf_t c);

/** Return the i'th limb of the modulus */
static inline limb_t modulus(int i) {
    return (i==NLIMBS/2) ? LMASK-1 : LMASK;
}

#endif /*__THREEBEARS_RING_H__*/

