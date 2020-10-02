/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: internal header file for P503
*********************************************************************************************/

#ifndef P503_INTERNAL_H
#define P503_INTERNAL_H

#include "../config.h"

#if (TARGET == TARGET_AMD64) || (TARGET == TARGET_ARM64) || (TARGET == TARGET_S390X)
#define NWORDS_FIELD 8    // Number of words of a 503-bit field element
#define p503_ZERO_WORDS 3 // Number of "0" digits in the least significant part of p503 + 1
#elif (TARGET == TARGET_x86) || (TARGET == TARGET_ARM)
#define NWORDS_FIELD 16
#define p503_ZERO_WORDS 7
#endif

// Basic constants

#define NBITS_FIELD 503
#define MAXBITS_FIELD 512
#define MAXWORDS_FIELD ((MAXBITS_FIELD + RADIX - 1) / RADIX) // Max. number of words to represent field elements
#define NWORDS64_FIELD ((NBITS_FIELD + 63) / 64)             // Number of 64-bit words of a 503-bit field element
#define NBITS_ORDER 256
#define NWORDS_ORDER ((NBITS_ORDER + RADIX - 1) / RADIX) // Number of words of oA and oB, where oA and oB are the subgroup orders of Alice and Bob, resp.
#define NWORDS64_ORDER ((NBITS_ORDER + 63) / 64)         // Number of 64-bit words of a 256-bit element
#define MAXBITS_ORDER NBITS_ORDER
#define ALICE 0
#define BOB 1
#define OALICE_BITS 250
#define OBOB_BITS 253
#define OBOB_EXPON 159
#define MASK_ALICE 0x03
#define MASK_BOB 0x0F
#define PRIME p503
#define PARAM_A 6
#define PARAM_C 1
// Fixed parameters for isogeny tree computation
#define MAX_INT_POINTS_ALICE 7
#define MAX_INT_POINTS_BOB 8
#define MAX_Alice 125
#define MAX_Bob 159
#define MSG_BYTES 24
#define SECRETKEY_A_BYTES ((OALICE_BITS + 7) / 8)
#define SECRETKEY_B_BYTES ((OBOB_BITS - 1 + 7) / 8)
#define FP2_ENCODED_BYTES 2 * ((NBITS_FIELD + 7) / 8)

#ifdef COMPRESS
#define MASK2_BOB 0x03
#define MASK3_BOB 0xFF
#define ORDER_A_ENCODED_BYTES SECRETKEY_A_BYTES
#define ORDER_B_ENCODED_BYTES SECRETKEY_B_BYTES
#define PARTIALLY_COMPRESSED_CHUNK_CT (4*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES + 2)
#define COMPRESSED_CHUNK_CT (3 * ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES + 2)
#define UNCOMPRESSEDPK_BYTES 378
// Table sizes used by the Entangled basis generation
#define TABLE_R_LEN 17
#define TABLE_V_LEN 34
#define TABLE_V3_LEN 20
// Parameters for discrete log computations
// Binary Pohlig-Hellman reduced to smaller logs of order ell^W
#define W_2 5
#define W_3 3
// ell^w
#define ELL2_W (1 << W_2)
#define ELL3_W 27
// ell^(e mod w)
#define ELL2_EMODW (1 << (OALICE_BITS % W_2))
#define ELL3_EMODW 1
// # of digits in the discrete log
#define DLEN_2 ((OALICE_BITS + W_2 - 1) / W_2) // ceil(eA/W_2)
#define DLEN_3 ((OBOB_EXPON + W_3 - 1) / W_3) // ceil(eB/W_3)
// Use compressed tables: FULL_SIGNED
#define COMPRESSED_TABLES
#define ELL2_FULL_SIGNED    // Uses signed digits to reduce table size by half
#define ELL3_FULL_SIGNED    // Uses signed digits to reduce table size by half
// Length of the optimal strategy path for Pohlig-Hellman
#ifdef COMPRESSED_TABLES
  #ifdef ELL2_FULL_SIGNED
    #if W_2 == 5
      #define PLEN_2 51
    #endif
  #endif
  #ifdef ELL3_FULL_SIGNED
    #if W_3 == 3
      #define PLEN_3 54
    #endif
  #endif
#endif
#endif

// SIDH's basic element definitions and point representations

typedef digit_t felm_t[NWORDS_FIELD];      // Datatype for representing 503-bit field elements (512-bit max.)
typedef digit_t dfelm_t[2 * NWORDS_FIELD]; // Datatype for representing double-precision 2x503-bit field elements (2x512-bit max.)
typedef felm_t f2elm_t[2];                 // Datatype for representing quadratic extension field elements GF(p503^2)

typedef struct {
	f2elm_t X;
	f2elm_t Z;
} point_proj; // Point representation in projective XZ Montgomery coordinates.
typedef point_proj point_proj_t[1];

#ifdef COMPRESS
typedef struct {
	f2elm_t X;
	f2elm_t Y;
	f2elm_t Z;
} point_full_proj; // Point representation in full projective XYZ Montgomery coordinates
typedef point_full_proj point_full_proj_t[1];

typedef struct {
	f2elm_t x;
	f2elm_t y;
} point_affine; // Point representation in affine coordinates.
typedef point_affine point_t[1];

typedef f2elm_t publickey_t[3];
#endif

/**************** Function prototypes ****************/
/************* Multiprecision functions **************/

// 503-bit multiprecision addition, c = a+b
static void mp_add503(const digit_t *a, const digit_t *b, digit_t *c);
void oqs_kem_sike_mp_add503_asm(const digit_t *a, const digit_t *b, digit_t *c);

// 503-bit multiprecision subtraction, c = a-b+2p or c = a-b+4p
extern void mp_sub503_p2(const digit_t* a, const digit_t* b, digit_t* c);
extern void mp_sub503_p4(const digit_t* a, const digit_t* b, digit_t* c);
void oqs_kem_sike_mp_sub503_p2_asm(const digit_t* a, const digit_t* b, digit_t* c); 
void oqs_kem_sike_mp_sub503_p4_asm(const digit_t* a, const digit_t* b, digit_t* c); 

// 2x503-bit multiprecision subtraction followed by addition with p503*2^512, c = a-b+(p503*2^512) if a-b < 0, otherwise c=a-b
void oqs_kem_sike_mp_subaddx2_asm(const digit_t *a, const digit_t *b, digit_t *c);
void oqs_kem_sike_mp_subadd503x2_asm(const digit_t *a, const digit_t *b, digit_t *c);

// Double 2x503-bit multiprecision subtraction, c = c-a-b, where c > a and c > b
void oqs_kem_sike_mp_dblsub503x2_asm(const digit_t *a, const digit_t *b, digit_t *c);

/************ Field arithmetic functions *************/

// Copy of a field element, c = a
static void fpcopy503(const digit_t *a, digit_t *c);

// Zeroing a field element, a = 0
static void fpzero503(digit_t *a);

// Non constant-time comparison of two field elements. If a = b return TRUE, otherwise, return FALSE
static bool fpequal503_non_constant_time(const digit_t *a, const digit_t *b);

// Modular addition, c = a+b mod p503
extern void fpadd503(const digit_t *a, const digit_t *b, digit_t *c);
extern void oqs_kem_sike_fpadd503_asm(const digit_t *a, const digit_t *b, digit_t *c);

// Modular subtraction, c = a-b mod p503
extern void fpsub503(const digit_t *a, const digit_t *b, digit_t *c);
extern void oqs_kem_sike_fpsub503_asm(const digit_t *a, const digit_t *b, digit_t *c);

// Modular negation, a = -a mod p503
extern void fpneg503(digit_t *a);

// Modular division by two, c = a/2 mod p503.
static void fpdiv2_503(const digit_t *a, digit_t *c);

// Modular correction to reduce field element a in [0, 2*p503-1] to [0, p503-1].
static void fpcorrection503(digit_t *a);

// 503-bit Montgomery reduction, c = a mod p
void oqs_kem_sike_rdc503_asm(digit_t *ma, digit_t *mc);

// Field multiplication using Montgomery arithmetic, c = a*b*R^-1 mod p503, where R=2^768
static void fpmul503_mont(const digit_t *a, const digit_t *b, digit_t *c);
void oqs_kem_sike_mul503_asm(const digit_t *a, const digit_t *b, digit_t *c);

// Field squaring using Montgomery arithmetic, c = a*b*R^-1 mod p503, where R=2^768
static void fpsqr503_mont(const digit_t *ma, digit_t *mc);

// Field inversion, a = a^-1 in GF(p503)
static void fpinv503_mont(digit_t *a);

// Field inversion, a = a^-1 in GF(p503) using the binary GCD
static void fpinv503_mont_bingcd(digit_t *a);

// Chain to compute (p503-3)/4 using Montgomery arithmetic
static void fpinv503_chain_mont(digit_t *a);

/************ GF(p^2) arithmetic functions *************/

// Copy of a GF(p503^2) element, c = a
static void fp2copy503(const f2elm_t a, f2elm_t c);

// Zeroing a GF(p503^2) element, a = 0
static void fp2zero503(f2elm_t a);

// GF(p503^2) negation, a = -a in GF(p503^2)
static void fp2neg503(f2elm_t a);

// GF(p503^2) addition, c = a+b in GF(p503^2)
extern void fp2add503(const f2elm_t a, const f2elm_t b, f2elm_t c);

// GF(p503^2) subtraction, c = a-b in GF(p503^2)
extern void fp2sub503(const f2elm_t a, const f2elm_t b, f2elm_t c);

// GF(p503^2) division by two, c = a/2  in GF(p503^2)
static void fp2div2_503(const f2elm_t a, f2elm_t c);

// Modular correction, a = a in GF(p503^2)
static void fp2correction503(f2elm_t a);

// GF(p503^2) squaring using Montgomery arithmetic, c = a^2 in GF(p503^2)
static void fp2sqr503_mont(const f2elm_t a, f2elm_t c);

// GF(p503^2) multiplication using Montgomery arithmetic, c = a*b in GF(p503^2)
static void fp2mul503_mont(const f2elm_t a, const f2elm_t b, f2elm_t c);

// GF(p503^2) inversion using Montgomery arithmetic, a = (a0-i*a1)/(a0^2+a1^2)
static void fp2inv503_mont(f2elm_t a);

// GF(p503^2) inversion, a = (a0-i*a1)/(a0^2+a1^2), GF(p503) inversion done using the binary GCD
static void fp2inv503_mont_bingcd(f2elm_t a);

#endif
