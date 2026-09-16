#ifndef __RIJDAEL_PLATFORM_H__
#define __RIJDAEL_PLATFORM_H__

/* Defined when we want to overload some low-level APIs */
#if defined(USE_WEAK_LOW_LEVEL_API)
#define WEAK __attribute__((weak))
#else
#define WEAK
#endif

#if defined(_MSC_VER)
#define RIJNDAEL_ALIGN(bytes) __declspec(align(bytes))
#elif defined(__GNUC__) || defined(__clang__)
#define RIJNDAEL_ALIGN(bytes) __attribute__((aligned(bytes)))
#else
#define RIJNDAEL_ALIGN(bytes)
#endif

/* Select the best Rijndael implementation depending on the platform if
 * not overloaded by the user.
 * Priority: VAES (if __VAES__ detected) > AES-NI > ARM AES > bitslice.
 * Any explicit -DRIJNDAEL_* flag bypasses auto-detection.
 */
#if !defined(RIJNDAEL_CONSTANT_TIME_REF) && !defined(RIJNDAEL_TABLE) && !defined(RIJNDAEL_AES_NI) && !defined(RIJNDAEL_BITSLICE) && !defined(RIJNDAEL_EXTERNAL) && !defined(RIJNDAEL_VAES) && !defined(RIJNDAEL_ARM_AES)
#if defined(__VAES__)
/* VAES present: use the vectorized VAES implementation (superset of AES-NI) */
#  define RIJNDAEL_VAES
#elif defined(__AES__)
/* AES-NI present: use the optimized AES-NI implementation */
#  define RIJNDAEL_AES_NI
#elif defined(__ARM_FEATURE_CRYPTO) || defined(__ARM_FEATURE_AES)
/* ARM Crypto Extension present: use the ARM hardware AES implementation */
#  define RIJNDAEL_ARM_AES
#else
/* Fallback: portable constant-time bitslice implementation */
#  define RIJNDAEL_BITSLICE
#endif
#endif

/* VAES (Vector AES) sub-levels, active when RIJNDAEL_VAES is selected.
 *
 * Two levels depending on available ISA:
 *   RIJNDAEL_VAES512 : AVX-512 VAES (_mm512_aesenc_epi128, 4 lanes/instr)
 *   RIJNDAEL_VAES256 : AVX2   VAES (_mm256_aesenc_epi128, 2 lanes/instr)
 *
 * RIJNDAEL_VAES512 implies RIJNDAEL_VAES256 (AVX-512 has AVX2).
 * VAES-512 requires AVX512BW for _mm512_shuffle_epi8/_mm512_mask_blend_epi8
 * (Rijndael-256 ShiftRows compensation).
 *
 * __VAES__ is required on top of RIJNDAEL_VAES, and not implied by it: an
 * explicit -DRIJNDAEL_VAES bypasses the auto-detection above, so RIJNDAEL_VAES
 * can be set on a target whose ISA flags never enabled VAES. Selecting a sub-
 * level on the AVX flags alone would then compile the ymm/zmm paths against
 * _mm256_aesenc_epi128 and friends, which need -mvaes: GCC rejects that at the
 * always_inline boundary rather than degrading, so the build fails outright.
 * Requiring __VAES__ here sends every primitive down its scalar AES-NI #else
 * branch instead - the same graceful fallback already used when VAES is asked
 * for without AVX2 - and the banner in rijndael.h reports what was really
 * built.
 */
#if defined(RIJNDAEL_VAES) && defined(__VAES__)
#  if defined(__AVX512F__) && defined(__AVX512BW__)
#    define RIJNDAEL_VAES512
#    define RIJNDAEL_VAES256
#  elif defined(__AVX2__)
#    define RIJNDAEL_VAES256
#  endif
#  if defined(__AVX512VBMI__) && defined(__AVX512VL__)
#    define RIJNDAEL_VAES256_VBMI
#  endif
/* GFNI-accelerated x2/x4/x8 key schedule (VGF2P8AFFINEINVQB for the SubWord
 * affine transform): faster than the AESKEYGENASSIST-based expansion on
 * GFNI-capable CPUs. AESKEYGENASSIST has no ymm form and so cannot batch, while
 * VGF2P8AFFINEINVQB has one - that is where the gain comes from, two schedules
 * running lane-parallel. Covers both AES-128 (cat1) and Rijndael-256-256 (the
 * cat3 and cat5 primitive), so this knob does reach every category.
 * Auto-detected like RIJNDAEL_VAES256_VBMI above; set
 * NO_RIJNDAEL_VAES_GFNI_KEYSCHED to force it off even when GFNI is present. */
#  if defined(RIJNDAEL_VAES256) && defined(__GFNI__) && !defined(NO_RIJNDAEL_VAES_GFNI_KEYSCHED)
#    define RIJNDAEL_VAES_GFNI_KEYSCHED
#  endif
#endif

#endif /* __RIJDAEL_PLATFORM_H__ */
