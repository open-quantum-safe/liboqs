#ifndef __COMMON_H__
#define __COMMON_H__

#if !defined(EXTERNAL_COMMON_OVERRIDE)

/* MQOM3 parameters */
#include "mqom3_parameters.h"
#include <string.h>
#include <stdint.h>

/* Alignment macro */
#ifdef ALIGN
#undef ALIGN
#endif
#if defined(__GNUC__)
#define ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#define ALIGN(x) __declspec(align(x))
#elif defined(__ARMCC_VERSION)
#define ALIGN(x) __align(x)
#else
#define ALIGN(x)
#endif

/* Namespacing with the appropriate prefix */
/* Private API */
#ifndef MQOM_NAMESPACE
#ifdef APPLY_NAMESPACE
#ifndef concat2
#define _concat2(a, b) a ## b
#define concat2(a, b) _concat2(a, b)
#endif
#define MQOM_NAMESPACE(s) concat2(APPLY_NAMESPACE, s)
#else
#define MQOM_NAMESPACE(s) s
#endif
#endif
/* Public API */
#ifndef MQOM_PUBLIC_API_NAMESPACE
#ifdef APPLY_PUBLIC_API_NAMESPACE
#ifndef concat2
#define _concat2(a, b) a ## b
#define concat2(a, b) _concat2(a, b)
#endif
#define MQOM_PUBLIC_API_NAMESPACE(s) concat2(APPLY_PUBLIC_API_NAMESPACE, s)
#else
#define MQOM_PUBLIC_API_NAMESPACE(s) s
#endif
#endif

#if defined(MQOM3_FOR_LIBOQS)
#define USE_ENC_CTX_CLEANSING
/* Useful includes for libOQS integration */
#include <oqs/common.h>
#endif

#ifndef EMBEDDED_SRAM
#if defined(__GNUC__) && !defined(NO_EMBEDDED_SRAM_SECTION) && !(defined(__APPLE__) && defined(__MACH__))
#define EMBEDDED_SRAM __attribute__((section(".embedded_sram_tables")))
#else
#define EMBEDDED_SRAM
#endif
#endif


#if defined(MQOM3_FOR_LIBOQS) && !defined(USE_ENC_CTX_CLEANSING)
/* XXX: NOTE: for liboqs, cleansing is mandatory, we always perform it */
#define USE_ENC_CTX_CLEANSING
#endif

#if defined(USE_ENC_CTX_CLEANSING)
#define DECL_VAR(x) x = { 0 }
#else
#define DECL_VAR(x) x
#endif

/* SeedExpand key schedule cache. One feature, two realisations: a heap cache in
 * the default BLC path (seed_expand_cache.h), and the same schedules kept in
 * folding_sign_t/folding_verify_t in the memopt path (blc_memopt_folding.h),
 * which trades them against memory and therefore leaves them off unless asked.
 * Hence the two defaults: ON unless NO_BLC_SEEDEXPAND_CACHE for the default
 * path, OFF unless BLC_SEEDEXPAND_CACHE for memopt. */
#if !defined(BLC_SEEDEXPAND_CACHE) && !defined(NO_BLC_SEEDEXPAND_CACHE) && !defined(MEMORY_EFFICIENT_BLC)
#define BLC_SEEDEXPAND_CACHE
#endif
/**/

/* Malloc redirection to allow for max allocation accounting. */
#if (defined(USE_ALLOC_PROBE) || defined(BENCHMARK) || defined(BENCHMARK_CYCLES)) && !defined(MQOM3_FOR_LIBOQS) && !defined(NO_ALLOC_PROBE)
/* Single source of truth for "the probe is compiled in": consumers must test
 * this rather than restate the condition above, which would drift. */
#define MQOM_ALLOC_PROBE_ACTIVE 1
#include <stdio.h>
#include <stdlib.h>
#define MAX_TRACKED_POINTERS 100
typedef struct {
	void *ptr;
	size_t size;
	uint8_t alive;
} living_pointer;

/* Deal with namespacing */
#define living_pointers MQOM_NAMESPACE(living_pointers)
#define alloc_peak_usage MQOM_NAMESPACE(alloc_peak_usage)

__attribute__((weak)) living_pointer living_pointers[MAX_TRACKED_POINTERS] = { 0 };

__attribute__((weak)) long int alloc_peak_usage = 0;

static void update_alloc_stats(void) {
	long int alloc_current_usage = 0;
	unsigned int i;
	for (i = 0; i < MAX_TRACKED_POINTERS; i++) {
		if (living_pointers[i].alive) {
			alloc_current_usage += living_pointers[i].size;
		}
	}
	if (alloc_current_usage > alloc_peak_usage) {
		alloc_peak_usage = alloc_current_usage;
	}
}

static inline void *mqom_malloc(size_t size) {
	void *ptr = malloc(size); // IGNORE memory-check
	if (ptr != NULL) {
		/* Find a suitable slot */
		unsigned int i;
		for (i = 0; i < MAX_TRACKED_POINTERS; i++) {
			if (!living_pointers[i].alive) {
				break;
			}
		}
		if (i == MAX_TRACKED_POINTERS) {
			/* No more space ... */
			printf("[-] WARNING: no more slots available to track malloc!\r\n");
			goto out;
		}
		living_pointers[i].alive = 1;
		living_pointers[i].ptr = ptr;
		living_pointers[i].size = size;
	}
out:
	update_alloc_stats();
	return ptr;
}

static inline void *mqom_calloc(size_t nmemb, size_t size) {
	void *ptr = calloc(nmemb, size); // IGNORE memory-check
	size = nmemb * size;
	if (ptr != NULL) {
		/* Find a suitable slot */
		unsigned int i;
		for (i = 0; i < MAX_TRACKED_POINTERS; i++) {
			if (!living_pointers[i].alive) {
				break;
			}
		}
		if (i == MAX_TRACKED_POINTERS) {
			/* No more space ... */
			printf("[-] WARNING: no more slots available to track malloc!\r\n");
			goto out;
		}
		living_pointers[i].alive = 1;
		living_pointers[i].ptr = ptr;
		living_pointers[i].size = size;
	}
out:
	update_alloc_stats();
	return ptr;
}

static inline void mqom_free(void *ptr, size_t len) {
	(void)len;
	if (ptr != NULL) {
		/* Find the slot */
		unsigned int i;
		for (i = 0; i < MAX_TRACKED_POINTERS; i++) {
			if (living_pointers[i].ptr == ptr) {
				if (living_pointers[i].size != len) {
					printf("[-] WARNING: internal error in malloc tracking (inconsistent length in free) ...\r\n");
					goto out;
				}
				break;
			}
		}
		if ((i == MAX_TRACKED_POINTERS) || (!living_pointers[i].alive)) {
			/* Should not happen ... */
			printf("[-] WARNING: internal error in malloc tracking (living pointer not found during free) ...\r\n");
			goto out;
		}
		living_pointers[i].alive = 0;
		living_pointers[i].ptr = NULL;
		living_pointers[i].size = 0;
	}
out:
	free(ptr); // IGNORE memory-check
	update_alloc_stats();
}

#define reset_alloc_usage() do { \
    memset(living_pointers, 0, MAX_TRACKED_POINTERS * sizeof(living_pointer)); \
    alloc_peak_usage = 0; \
} while(0);

#define print_alloc_usage(s) do { \
    printf("[+] Malloc stats %s: Peak Usage is %ld bytes\r\n", s, alloc_peak_usage); \
} while(0);

#elif defined(MQOM3_FOR_LIBOQS)
/* libOQS specific helpers */
#define mqom_malloc OQS_MEM_malloc
#define mqom_calloc OQS_MEM_calloc
#define mqom_free OQS_MEM_secure_free
#define reset_alloc_usage()
#define print_alloc_usage(s)

#else
#include <stdlib.h>
// XXX: dummy variable
#define alloc_peak_usage MQOM_NAMESPACE(alloc_peak_usage)
__attribute__((weak)) long int alloc_peak_usage = 0;
#define mqom_malloc malloc // IGNORE memory-check
#define mqom_calloc calloc // IGNORE memory-check
#define mqom_free(ptr, sz) free(ptr) // IGNORE memory-check
#define reset_alloc_usage()
#define print_alloc_usage(s)
#endif

/* Cleaning function */
#if defined(MQOM3_FOR_LIBOQS)
#define mqom_cleanse OQS_MEM_cleanse
#else
static inline void mqom_cleanse(void *ptr, size_t len) {
	/* Usage of 'volatile' to avoid memset removal with agressive compiler optimizations */
	static void *(*volatile mqom_memset_ptr)(void*, int, size_t) = memset;
	if (ptr != NULL) {
		mqom_memset_ptr(ptr, 0, len);
	}
}
#endif

/* Deal with our internal "printf" */
#ifndef _STDIO_H
#include <stdio.h>
#endif
#define mqom_print printf

#undef ERR
#define ERR(r, e) if(r) { goto e; }

#undef ERR_NULL
#define ERR_NULL(r, e) if(r == NULL) { goto e; }

/* Helpers to compute some sizes of serialized objects */
#define MQOM3_PK_SIZE ((2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M/MQOM3_PARAM_MU))
#define MQOM3_SK_SIZE ((2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M/MQOM3_PARAM_MU) + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N))
/* --- BLC opening and signature sizes ------------------------------------
 *
 * One formula per BLC family
 */
#define MQOM3_PARAM_CT_OPENING_SIZE (MQOM3_PARAM_TAU*(                        \
    BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)-MQOM3_PARAM_SEED_SIZE              \
      + MQOM3_PARAM_NB_EVALS_LOG*MQOM3_PARAM_SEED_SIZE                        \
      + MQOM3_PARAM_DIGEST_SIZE                                               \
      + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA*MQOM3_PARAM_MU)                  \
))
#define MQOM3_CT_SIG_SIZE (                                                   \
    4 + (MQOM3_PARAM_SALT_SIZE) + (MQOM3_PARAM_DIGEST_SIZE)                   \
      + MQOM3_PARAM_CT_OPENING_SIZE                                           \
)

/* MQOM3_PARAM_LARGE_GGM_T_OPEN only exists in the OT parameter headers,
 * so the OT formula is only computable in an OT build
 */
#if MQOM3_PARAM_OT_VARIANT == 1
/* OT opening: T_open seeds + tau*(digest + delta_x + alpha1) */
#define MQOM3_PARAM_OT_OPENING_SIZE (                                         \
    MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE                      \
      + MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE                             \
      + MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)              \
      + MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU) \
)
#define MQOM3_OT_SIG_SIZE (                                                   \
    4 + (MQOM3_PARAM_SALT_SIZE) + (MQOM3_PARAM_DIGEST_SIZE)                   \
      + MQOM3_PARAM_OT_OPENING_SIZE                                           \
)
#endif

/* The agnostic pair. Aliased, never recomputed: no #undef, and no way for the
 * two to drift apart. */
#if MQOM3_PARAM_OT_VARIANT == 1
#define MQOM3_PARAM_OPENING_SIZE MQOM3_PARAM_OT_OPENING_SIZE
#define MQOM3_SIG_SIZE           MQOM3_OT_SIG_SIZE
#else
#define MQOM3_PARAM_OPENING_SIZE MQOM3_PARAM_CT_OPENING_SIZE
#define MQOM3_SIG_SIZE           MQOM3_CT_SIG_SIZE
#endif

/*
 * Helpers for aligned dynamic and static allocation of buffers.
 * NOTE: on AVX2 and AVX512 platforms, alignment of buffers can drastically
 * change performance! Hence, we ensure allocation on 64 bytes alignment.
 */
/* Generic alignement macro */
#ifdef ALIGN
#undef ALIGN
#endif

#if defined(__GNUC__)
#define ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#define ALIGN(x) __declspec(align(x))
#elif defined(__ARMCC_VERSION)
#define ALIGN(x) __align(x)
#else
#define ALIGN(x)
#endif

/* Compile-time assertion, portable down to C89: a false condition declares an
 * array of negative size, which every conforming compiler rejects. Deliberately
 * NOT _Static_assert, which would require C11 from every toolchain building this
 * code (including the embedded ones). Costs nothing at runtime: a typedef emits
 * no code.
 * The two-level concatenation is required - a single '##' would paste the token
 * __LINE__ itself instead of its value, so two assertions in one file would
 * collide on the same typedef name. */
#define MQOM3_SA_CAT_(a, b) a##b
#define MQOM3_SA_CAT(a, b)  MQOM3_SA_CAT_(a, b)
#define MQOM3_STATIC_ASSERT(cond) \
	typedef char MQOM3_SA_CAT(mqom3_static_assert_, __LINE__)[(cond) ? 1 : -1]

/* Block-scope variant. MQOM3_STATIC_ASSERT expands to a typedef, which GCC
 * reports as unused when it sits inside a function (-Wunused-local-typedefs,
 * implied by -Wall) - harmless in itself, but it breaks integrators building
 * with -Werror. This one is an expression statement instead, so it can assert
 * on the sizeof of a local: exactly the case worth pinning down, since a
 * refactor turning an array into a pointer would silently shrink a sizeof
 * without changing a line at the assertion site. Emits no code either. */
#define MQOM3_STATIC_ASSERT_BLOCK(cond) ((void)sizeof(char[(cond) ? 1 : -1]))

/* Compile-time counterpart of arch_is_big_endian() below. */
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define MQOM3_TARGET_IS_BIG_ENDIAN 1
#endif
#elif defined(__ARMEB__) || defined(__ARM_BIG_ENDIAN) || defined(__AARCH64EB__) || \
      defined(__MIPSEB__) || defined(__sparc__) || defined(__s390x__) || \
      (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__))
#define MQOM3_TARGET_IS_BIG_ENDIAN 1
#endif
/* Endianness detection:
 * Return 1 if architecture is big endian, 0 otherwise.
 */
static inline int arch_is_big_endian(void) {
	const uint16_t val = 0x0102;
	const uint8_t *buf = (const uint8_t *)(&val);

	return (buf[0] == 0x01);
}


/* Domain separation size: CT=2 bytes (16-bit tweak), OT=3 bytes (24-bit tweak). */
#if MQOM3_PARAM_OT_VARIANT == 1
#define MQOM3_PARAM_DOMAIN_SEP_BYTES 3
#else
#define MQOM3_PARAM_DOMAIN_SEP_BYTES 2
#endif

/* The tweak overwrites the trailing DOMAIN_SEP_BYTES of the salt: it must fit. */
#if MQOM3_PARAM_DOMAIN_SEP_BYTES > MQOM3_PARAM_SALT_SIZE
#error "TweakSalt requires MQOM3_PARAM_DOMAIN_SEP_BYTES <= MQOM3_PARAM_SALT_SIZE"
#endif

/* The SALT_SEL_* selectors are the only legal values of the "sel" argument below, so
 * they travel with the primitive: including them here means every TweakSalt caller
 * has them in scope by construction, without a per-file include to remember. */
#include "domain_separation.h"

/* Salt tweaking primitive: replaces the last DOMAIN_SEP_BYTES of the salt with
 * Bits_{8*DOMAIN_SEP_BYTES}(sel + 4*v) in little-endian order. */
static inline void TweakSalt(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE], uint8_t sel, uint32_t v) {
	uint32_t tweak = (uint32_t)sel + (4u * v);
	unsigned int i;
	memcpy(tweaked_salt, salt, MQOM3_PARAM_SALT_SIZE);
	for (i = 0; i < MQOM3_PARAM_DOMAIN_SEP_BYTES; i++) {
		tweaked_salt[MQOM3_PARAM_SALT_SIZE - MQOM3_PARAM_DOMAIN_SEP_BYTES + i] = (uint8_t)((tweak >> (8u * i)) & 0xffU);
	}
}

/* From TAU == 65 on, e can reach 64 and IndexIdentifier(64, 0)
 * equals IndexIdentifier(0, 1), handing two distinct tree levels the same
 * derivation key. Current max is TAU = 36 (cat5-gf16-*-ot). */
#if MQOM3_PARAM_TAU > 64
#error "IndexIdentifier requires MQOM3_PARAM_TAU <= 64 to avoid derivation collisions"
#endif

/* Domain the spec fixes for TweakSalt's index: v <= 2^(|tweak|-3)-1 */
#define MQOM3_TWEAK_MAX_V ((1 << ((8 * MQOM3_PARAM_DOMAIN_SEP_BYTES) - 3)) - 1)

/* Sanity checks on \tau parameter wrt tweak */
#if MQOM3_PARAM_OT_VARIANT == 1
/* OT: v is the raw heap index of a node of the single big tree. Leaves sit at
 * 2^h + i (deep) or tau*N + i' (shallow); both are bounded by 2*tau*N - 1, */
#if ((2 * MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS) - 1) > MQOM3_TWEAK_MAX_V
#error "TweakSalt: OT node index exceeds the tweak domain (raise DOMAIN_SEP_BYTES or shrink tau*N)"
#endif
#else
/* CT: v = IndexIdentifier(e, j) = e + 64*j, with e < TAU and j a tree level. */
#if ((MQOM3_PARAM_TAU - 1) + (64 * (MQOM3_PARAM_NB_EVALS_LOG - 1))) > MQOM3_TWEAK_MAX_V
#error "TweakSalt: CT IndexIdentifier exceeds the tweak domain (raise DOMAIN_SEP_BYTES or shrink tau/NB_EVALS_LOG)"
#endif
#endif

/* IndexIdentifier(e, j) = e + 64*j (14-bit value mapping execution e and level j) */
static inline uint16_t IndexIdentifier(uint32_t e, uint32_t j) {
	return (uint16_t)(e + 64 * j);
}

/* LinOrtho primitive */
static inline void LinOrtho(const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint8_t linortho_seed[MQOM3_PARAM_SEED_SIZE]) {
	unsigned int i;
	memcpy(&linortho_seed[0], &seed[MQOM3_PARAM_SEED_SIZE / 2], MQOM3_PARAM_SEED_SIZE / 2);
	memcpy(&linortho_seed[MQOM3_PARAM_SEED_SIZE / 2], &seed[0], MQOM3_PARAM_SEED_SIZE / 2);

	for (i = 0; i < MQOM3_PARAM_SEED_SIZE / 2; i++) {
		linortho_seed[i] ^= seed[i];
	}

	return;
}

/* Helper to xor MQOM3_PARAM_SEED_SIZE elements
 * NOTE: since we do not have information about alignment, we must
 * copy this to aligned buffers and do this on 64 bits, this will
 * hopefully trigger the compiler vectorization
 */
/* The loop below advances 8 bytes at a time and always moves a full uint64_t,
 * so a seed size that is not a multiple of 8 would read and write up to 7 bytes
 * past the end of the three buffers. That holds for every parameter set today
 * (16, 24 and 32 bytes); the guard is here so that a future one cannot break it
 * silently. */
#if (MQOM3_PARAM_SEED_SIZE % 8) != 0
#error "xor_blocks requires MQOM3_PARAM_SEED_SIZE to be a multiple of 8"
#endif
static inline void xor_blocks(const uint8_t seed_in[MQOM3_PARAM_SEED_SIZE], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint8_t seed_out[MQOM3_PARAM_SEED_SIZE]) {
	unsigned int i;

	for (i = 0; i < MQOM3_PARAM_SEED_SIZE; i += 8) {
		uint64_t a, b;

		memcpy(&a, &seed_in[i], sizeof(a));
		memcpy(&b, &delta[i], sizeof(b));
		a ^= b;
		memcpy(&seed_out[i], &a, sizeof(a));
	}

	return;
}

#else /* EXTERNAL_COMMON_OVERRIDE */

#include <external_common_override.h>

#endif

#endif /* __COMMON_H__ */
