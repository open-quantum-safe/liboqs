#ifndef __FIELDS_H__
#define __FIELDS_H__

#include "common.h"

#ifndef _concat3
#define _concat3(a, b, c) a ## b ## c
#endif
#ifndef concat3
#define concat3(a, b, c) _concat3(a, b, c)
#endif
#ifndef _concat5
#define _concat5(a, b, c, d, e) a ## b ## c ## d ## e
#endif
#ifndef concat5
#define concat5(a, b, c, d, e) _concat5(a, b, c, d, e)
#endif

/* Small helpers to compute the byte size of num elements in the base
 * and extension fields */
#define BYTE_SIZE_FIELD_BASE(num) (((num) * FIELD_BASE_LOG2_CARD) / 8)
#define BYTE_SIZE_FIELD_EXT(num)  (((num) * FIELD_EXT_LOG2_CARD) / 8)


/* Adapt our types definition depending on the parameters */
/* ==== Base field definition ====== */
/* ==== Base field is GF(2) */
#if MQOM3_PARAM_BASE_FIELD == 1
typedef uint8_t field_base_elt; /* GF(2) */
#define FIELD_BASE_PREFIX gf2
#define FIELD_BASE_LOG2_CARD 1
#define FIELD_BASE_PACKING(num) ((num) / 8)
/* ==== Base field is GF(16) */
#elif MQOM3_PARAM_BASE_FIELD == 4
typedef uint8_t field_base_elt; /* GF(16) */
#define FIELD_BASE_PREFIX gf16
#define FIELD_BASE_LOG2_CARD 4
#define FIELD_BASE_PACKING(num) ((num) / 2)
#else
#error "Error: MQOM3_PARAM_BASE_FIELD is neither GF(2) nor GF(16)! Please choose one of those"
#endif

/* Sanity checks on fields deserializers */
#if (FIELD_BASE_LOG2_CARD == 1) && ((MQOM3_PARAM_MQ_N % 8) != 0)
#error "GF(2) base field requires MQOM3_PARAM_MQ_N to be a multiple of 8 (field_gf2_parse would overflow)"
#endif
#if (FIELD_BASE_LOG2_CARD == 4) && ((MQOM3_PARAM_MQ_N % 2) != 0)
#error "GF(16) base field requires MQOM3_PARAM_MQ_N to be even (field_gf16_parse would overflow)"
#endif
/* Sanity check for M and MU */
#if (MQOM3_PARAM_MQ_M % MQOM3_PARAM_MU) != 0
#error "MQOM3_PARAM_MQ_M must be a multiple of MQOM3_PARAM_MU"
#endif


/* ==== Extension field definition ====== */
/* ==== Extension field is GF(256) */
#if MQOM3_PARAM_EXT_FIELD == 8
typedef uint8_t field_ext_elt; /* GF(256) */
#define FIELD_EXT_PREFIX gf256
#define FIELD_EXT_LOG2_CARD 8
#define FIELD_EXT_PACKING(num) (num)
/* ==== Base field is GF(256^2) */
#elif MQOM3_PARAM_EXT_FIELD == 16
typedef uint16_t field_ext_elt; /* GF(256^2) = GF(2^16) */
#define FIELD_EXT_PREFIX gf256to2
#define FIELD_EXT_LOG2_CARD 16
#define FIELD_EXT_PACKING(num) (num)
#else
#error "Error: MQOM3_PARAM_EXT_FIELD is neither GF(256) nor GF(256^2)! Please choose one of those"
#endif


/* The four field backends are mutually exclusive. Without this the cascade
 * below would silently pick the first one that matches and drop the others, so
 * e.g. "make FIELDS_AVX512=1 FIELDS_REF=1" would quietly build portable C.
 * Same discipline as rijndael/rijndael.h, which guards its own backends.
 * (defined(X) evaluates to 0 or 1 inside #if, so the sum counts them.) */
#if (defined(FIELDS_REF) + defined(FIELDS_AVX2) + defined(FIELDS_AVX512) + defined(FIELDS_NEON)) > 1
#error "Error: FIELDS_REF, FIELDS_AVX2, FIELDS_AVX512 and FIELDS_NEON are mutually exclusive, pick exactly one"
#endif

/* This file adapts the fields definition depending on the compilation */
#if !defined(FIELDS_REF) && !defined(FIELDS_AVX2) && !defined(FIELDS_AVX512) && !defined(FIELDS_NEON)
#if defined(__AVX512BW__) && defined(__AVX512F__) && defined(__AVX512VL__) && defined(__AVX512VPOPCNTDQ__) && defined(__AVX512VBMI__)
#define FIELDS_AVX512
#else
#ifdef __AVX2__
#define FIELDS_AVX2
#elif (defined(__ARM_NEON) || defined(__ARM_NEON__)) && !defined(MQOM3_TARGET_IS_BIG_ENDIAN)
/* Big endian ARM deliberately falls through to FIELDS_REF: the NEON GF(256^2)
 * path mixes the byte view and the uint16_t view of the same register (see the
 * #error in fields_arm_neon.h), and no big endian ARM target is testable here.
 * The reference backend already handles big endian explicitly. */
#define FIELDS_NEON
#else
#define FIELDS_REF
#endif
#endif
#endif

#include "fields_handling.h"
#if defined(FIELDS_REF)
#include "fields_ref.h"
#if defined(USE_GF256_TABLE_MULT)
#if defined(GF256_MULT_TABLE_SRAM)
static const char fields_conf[] = "Fields ref implementation with (NON constant time) 65 kB GF(256) mult table in SRAM (pure C)";
#else
static const char fields_conf[] = "Fields ref implementation with (NON constant time) 65 kB GF(256) mult table in flash/ROM (pure C)";
#endif
#elif defined(USE_GF256_TABLE_LOG_EXP)
static const char fields_conf[] = "Fields ref implementation with (constant time on embedded platforms without cache to SRAM) log/exp GF(256) mult tables (pure C)";
#else
#if defined(NO_FIELDS_REF_SWAR_OPT)
static const char fields_conf[] = "Fields ref implementation (pure C, basic circuit)";
#else
static const char fields_conf[] = "Fields ref implementation (pure C, SWAR x4 circuit SIMD optimization)";
#endif
#endif
#define FIELD_IMPLEMENTATION_SUFFIX ref
#elif defined(FIELDS_AVX2)
#include "fields_avx2.h"
#if !defined(__AVX2__)
#error "FIELDS_AVX2 selected but the compiler does not target AVX2: add -mavx2 (or -march=native)"
#endif
#if defined(__GFNI__) && !defined(NO_GFNI)
static const char fields_conf[] = "Fields AVX2 implementation with GFNI";
#else
static const char fields_conf[] = "Fields AVX2 implementation (without GFNI)";
#endif
#define FIELD_IMPLEMENTATION_SUFFIX avx2
#elif defined(FIELDS_AVX512)
#include "fields_avx512.h"
#if !(defined(__AVX512BW__) && defined(__AVX512F__) && defined(__AVX512VL__) && defined(__AVX512VPOPCNTDQ__) && defined(__AVX512VBMI__))
#error "FIELDS_AVX512 selected but the compiler does not target AVX-512: add -mavx512bw -mavx512f -mavx512vl -mavx512vpopcntdq -mavx512vbmi"
#endif
#if defined(__GFNI__) && !defined(NO_GFNI)
static const char fields_conf[] = "Fields AVX512 implementation with GFNI";
#else
static const char fields_conf[] = "Fields AVX512 implementation (without GFNI)";
#endif
#define FIELD_IMPLEMENTATION_SUFFIX avx512
#elif defined(FIELDS_NEON)
#include "fields_arm_neon.h"
#if !(defined(__ARM_NEON) || defined(__ARM_NEON__))
#error "FIELDS_NEON selected but the compiler does not target NEON (e.g. -mfpu=neon on ARMv7)"
#endif
#ifdef __aarch64__
static const char fields_conf[] = "Fields ARM NEON implementation (AArch64)";
#else
static const char fields_conf[] = "Fields ARM NEON implementation (ARMv7)";
#endif
#define FIELD_IMPLEMENTATION_SUFFIX neon
#else
#error "Error: no low-level field implementation detected ..."
#endif


/* Now construct our base operations by constructing our low-level
 * functions depending on the prefix (fields types) and suffix (implementation) */
/* Base field handling */
#define field_base_vect_pack concat3(field_, FIELD_BASE_PREFIX, _vect_pack)
#define field_base_vect_unpack concat3(field_, FIELD_BASE_PREFIX, _vect_unpack)
#define field_base_parse concat3(field_, FIELD_BASE_PREFIX, _parse)
#define field_base_serialize concat3(field_, FIELD_BASE_PREFIX, _serialize)
/* Extension field handling */
#define field_ext_vect_pack concat3(field_, FIELD_EXT_PREFIX, _vect_pack)
#define field_ext_vect_unpack concat3(field_, FIELD_EXT_PREFIX, _vect_unpack)
#define field_ext_parse concat3(field_, FIELD_EXT_PREFIX, _parse)
#define field_ext_serialize concat3(field_, FIELD_EXT_PREFIX, _serialize)
/* Extension field operations */
#define _field_ext_mult concat3(FIELD_EXT_PREFIX, _mult_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_ext_constant_vect_mult concat3(FIELD_EXT_PREFIX, _constant_vect_mult_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_ext_vect_mult concat3(FIELD_EXT_PREFIX, _vect_mult_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_ext_vect_mult_multiple_public concat3(FIELD_EXT_PREFIX, _vect_mult_multiple_public_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_ext_mat_mult concat3(FIELD_EXT_PREFIX, _mat_mult_, FIELD_IMPLEMENTATION_SUFFIX)
/* Hybrid multiplications */
#define _field_base_ext_constant_vect_mult concat5(FIELD_BASE_PREFIX, _, FIELD_EXT_PREFIX, _constant_vect_mult_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_ext_base_constant_vect_mult concat5(FIELD_EXT_PREFIX, _, FIELD_BASE_PREFIX, _constant_vect_mult_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_base_ext_vect_mult concat5(FIELD_BASE_PREFIX, _, FIELD_EXT_PREFIX, _vect_mult_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_ext_base_vect_mult concat5(FIELD_EXT_PREFIX, _, FIELD_BASE_PREFIX, _vect_mult_, FIELD_IMPLEMENTATION_SUFFIX)
#define _field_ext_base_mat_mult concat5(FIELD_EXT_PREFIX, _, FIELD_BASE_PREFIX, _mat_mult_, FIELD_IMPLEMENTATION_SUFFIX)


/* Public API */
static inline field_ext_elt field_ext_mult(field_ext_elt a, field_ext_elt b) {
	return _field_ext_mult(a, b);
}

static inline void field_ext_constant_vect_mult(field_ext_elt a, const field_ext_elt *b, field_ext_elt *c, uint32_t len) {
	_field_ext_constant_vect_mult(a, b, c, len);
	return;
}

static inline field_ext_elt field_ext_vect_mult(const field_ext_elt *a, const field_ext_elt *b, uint32_t len) {
	return _field_ext_vect_mult(a, b, len);
}

static inline void field_ext_vect_mult_multiple_public(field_ext_elt* const *c, const field_ext_elt *a, field_ext_elt const* const *b, uint32_t len, uint32_t n) {
	_field_ext_vect_mult_multiple_public(c, a, b, len, n);
}

static inline void field_ext_mat_mult(const field_ext_elt *A, const field_ext_elt *X, field_ext_elt *Y, uint32_t n, matrix_type mtype) {
	_field_ext_mat_mult(A, X, Y, n, mtype);
	return;
}

static inline void field_base_ext_constant_vect_mult(field_base_elt a, const field_ext_elt *b, field_ext_elt *c, uint32_t len) {
	_field_base_ext_constant_vect_mult(a, b, c, len);
	return;
}

static inline void field_ext_base_constant_vect_mult(field_ext_elt a, const field_base_elt *b, field_ext_elt *c, uint32_t len) {
	_field_ext_base_constant_vect_mult(a, b, c, len);
	return;
}

static inline field_ext_elt field_base_ext_vect_mult(const field_base_elt *a, const field_ext_elt *b, uint32_t len) {
	return _field_base_ext_vect_mult(a, b, len);
}

static inline field_ext_elt field_ext_base_vect_mult(const field_ext_elt *a, const field_base_elt *b, uint32_t len) {
	return _field_ext_base_vect_mult(a, b, len);
}

static inline void field_ext_base_mat_mult(const field_ext_elt *A, const field_base_elt *X, field_ext_elt *Y, uint32_t n, matrix_type mtype) {
	_field_ext_base_mat_mult(A, X, Y, n, mtype);
	return;
}

/*
 * NOTE: these are rather "naive" xors between bytes, but the compiler optimization passes should
 * hopefully properly vectorize this.
 */
static inline void field_base_vect_add(const field_base_elt *a, const field_base_elt *b, field_base_elt *c, uint32_t len) {
	uint32_t i;

	for (i = 0; i < FIELD_BASE_PACKING(len); i++) {
		c[i] = a[i] ^ b[i];
	}
	return;
}

static inline void field_ext_vect_add(const field_ext_elt *a, const field_ext_elt *b, field_ext_elt *c, uint32_t len) {
	uint32_t i;

	for (i = 0; i < FIELD_EXT_PACKING(len); i++) {
		c[i] = a[i] ^ b[i];
	}
	return;
}

/* The cast below truncates to FIELD_EXT_LOG2_CARD bits. Since i ranges over
 * [0, NB_EVALS-1] and the Gray code is a bijection on that range, the extension
 * field must hold at least NB_EVALS distinct elements: otherwise two evaluation
 * indices would silently share an evaluation point, breaking soundness without
 * any test failing. */
#if MQOM3_PARAM_NB_EVALS_LOG > FIELD_EXT_LOG2_CARD
#error "get_evaluation_point requires MQOM3_PARAM_NB_EVALS_LOG <= FIELD_EXT_LOG2_CARD"
#endif

/* Lower bound, the counterpart of the check above. */
#if MQOM3_PARAM_NB_EVALS_LOG < 3
#error "BLC_ConvertToLineEval requires MQOM3_PARAM_NB_EVALS_LOG >= 3 (leaves are processed in groups of 8)"
#endif

static inline field_ext_elt get_evaluation_point(uint16_t i) {
	// Gray code
	return (field_ext_elt) (i ^ (i >> 1));
}

/* Closed form, no search: gray(n-1) and gray(n) differ in exactly one bit, the
 * one at position ctz(n) (the "ruler sequence").
 */
/* Count trailing zeros of a non-zero word. Kept separate from the function
 * below so that the portability choice and the Gray-code arithmetic stay
 * readable independently of one another. */
static inline uint8_t gray_code_ctz(uint32_t n) {
#if defined(__GNUC__) || defined(__clang__)
	return (uint8_t) __builtin_ctz(n);
#else
	uint8_t index = 0;

	while ((n & 1u) == 0u) {
		n >>= 1;
		index++;
	}
	return index;
#endif
}

/* Sanity check on leaf index as it is passed as uint16_t */
#if MQOM3_PARAM_NB_EVALS > 65536
#error "get_gray_code_bit_position takes a uint16_t leaf index: MQOM3_PARAM_NB_EVALS must be <= 65536"
#endif
static inline uint8_t get_gray_code_bit_position(uint16_t i) {
	uint32_t n = (uint32_t) i + 1;

	if (n < (uint32_t) MQOM3_PARAM_NB_EVALS) {
		return gray_code_ctz(n);
	}

	return (MQOM3_PARAM_NB_EVALS_LOG > 0) ? (uint8_t) (MQOM3_PARAM_NB_EVALS_LOG - 1) : 0;
}

#endif /* __FIELDS_H__ */
