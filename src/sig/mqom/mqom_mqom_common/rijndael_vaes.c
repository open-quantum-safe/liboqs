#include "rijndael_platform.h"
#if defined(RIJNDAEL_VAES)

#include "rijndael_vaes.h"

/* ======== Single-block encryption macros (scalar, same-key) ======== */

#define DO_ENC_BLOCK_VAES_128_128(m, k) \
	do { \
		(m) = _mm_xor_si128       ((m), (k)[ 0]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 1]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 2]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 3]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 4]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 5]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 6]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 7]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 8]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 9]); \
		(m) = _mm_aesenclast_si128((m), (k)[10]); \
	} while (0)

#define DO_ENC_BLOCK_VAES_128_256(m, k) \
	do { \
		(m) = _mm_xor_si128       ((m), (k)[ 0]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 1]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 2]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 3]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 4]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 5]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 6]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 7]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 8]); \
		(m) = _mm_aesenc_si128    ((m), (k)[ 9]); \
		(m) = _mm_aesenc_si128    ((m), (k)[10]); \
		(m) = _mm_aesenc_si128    ((m), (k)[11]); \
		(m) = _mm_aesenc_si128    ((m), (k)[12]); \
		(m) = _mm_aesenc_si128    ((m), (k)[13]); \
		(m) = _mm_aesenclast_si128((m), (k)[14]); \
	} while (0)

#define RINJDAEL_COMPENSATE_SR_VAES(ml, mr, tmpl, tmpr, mblend, mshuff) \
	do { \
		(tmpl) = _mm_blendv_epi8((ml), (mr), (mblend)); \
		(tmpr) = _mm_blendv_epi8((mr), (ml), (mblend)); \
		(tmpl) = _mm_shuffle_epi8((tmpl), (mshuff)); \
		(tmpr) = _mm_shuffle_epi8((tmpr), (mshuff)); \
	} while (0)

#define ENC_256_BLOCK_VAES(ml, mr, kl, kr, mb, ms) \
	do { \
		__m128i _tmpl, _tmpr; \
		RINJDAEL_COMPENSATE_SR_VAES(ml, mr, _tmpl, _tmpr, mb, ms); \
		(ml) = _mm_aesenc_si128    (_tmpl, kl); \
		(mr) = _mm_aesenc_si128    (_tmpr, kr); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES(ml, mr, kl, kr, mb, ms) \
	do { \
		__m128i _tmpl, _tmpr; \
		RINJDAEL_COMPENSATE_SR_VAES(ml, mr, _tmpl, _tmpr, mb, ms); \
		(ml) = _mm_aesenclast_si128(_tmpl, kl); \
		(mr) = _mm_aesenclast_si128(_tmpr, kr); \
	} while (0)

#define DO_ENC_BLOCK_VAES_256_256(ml, mr, k) \
	do { \
		const __m128i _mb = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000); \
		const __m128i _ms = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100); \
		(ml) = _mm_xor_si128((ml), (k)[ 0]); \
		(mr) = _mm_xor_si128((mr), (k)[ 1]); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[ 2], (k)[ 3], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[ 4], (k)[ 5], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[ 6], (k)[ 7], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[ 8], (k)[ 9], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[10], (k)[11], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[12], (k)[13], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[14], (k)[15], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[16], (k)[17], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[18], (k)[19], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[20], (k)[21], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[22], (k)[23], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[24], (k)[25], _mb, _ms); \
		ENC_256_BLOCK_VAES(ml, mr, (k)[26], (k)[27], _mb, _ms); \
		ENC_256_BLOCK_LAST_VAES(ml, mr, (k)[28], (k)[29], _mb, _ms); \
	} while (0)

#ifdef RIJNDAEL_VAES256

/* One Rijndael-256 state per YMM: [left half | right half].
 * Swapping the 128-bit lanes lets one blend construct both halves of the
 * ShiftRows compensation before a single two-lane VAES round. */
#define LOAD_RK_PAIR_VAES256(k, off) \
	_mm256_loadu_si256((const __m256i *)(const void *)&(k)[off])

#ifdef RIJNDAEL_VAES256_VBMI
#define ENC_256_BLOCK_VAES256_AOS(m, k, off, mb, ms) \
	do { \
		__m256i _tmp = _mm256_permutexvar_epi8(_idx_aos, (m)); \
		(m) = _mm256_aesenc_epi128(_tmp, LOAD_RK_PAIR_VAES256(k, off)); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_AOS(m, k, off, mb, ms) \
	do { \
		__m256i _tmp = _mm256_permutexvar_epi8(_idx_aos, (m)); \
		(m) = _mm256_aesenclast_epi128(_tmp, LOAD_RK_PAIR_VAES256(k, off)); \
	} while (0)
#else
#define ENC_256_BLOCK_VAES256_AOS(m, k, off, mb, ms) \
	do { \
		__m256i _swapped = _mm256_permute2x128_si256((m), (m), 0x01); \
		__m256i _tmp = _mm256_blendv_epi8((m), _swapped, (mb)); \
		_tmp = _mm256_shuffle_epi8(_tmp, (ms)); \
		(m) = _mm256_aesenc_epi128(_tmp, LOAD_RK_PAIR_VAES256(k, off)); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_AOS(m, k, off, mb, ms) \
	do { \
		__m256i _swapped = _mm256_permute2x128_si256((m), (m), 0x01); \
		__m256i _tmp = _mm256_blendv_epi8((m), _swapped, (mb)); \
		_tmp = _mm256_shuffle_epi8(_tmp, (ms)); \
		(m) = _mm256_aesenclast_epi128(_tmp, LOAD_RK_PAIR_VAES256(k, off)); \
	} while (0)
#endif

#define ADD_RK_PAIR_VAES256_AOS(m, k) \
	do { \
		(m) = _mm256_xor_si256((m), LOAD_RK_PAIR_VAES256(k, 0)); \
	} while (0)

#define ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, off, mb, ms) \
	do { \
		ENC_256_BLOCK_VAES256_AOS(m1, k1, off, mb, ms); \
		ENC_256_BLOCK_VAES256_AOS(m2, k2, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_AOS_x2(m1, m2, k1, k2, off, mb, ms) \
	do { \
		ENC_256_BLOCK_LAST_VAES256_AOS(m1, k1, off, mb, ms); \
		ENC_256_BLOCK_LAST_VAES256_AOS(m2, k2, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, off, mb, ms) \
	do { \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, off, mb, ms); \
		ENC_256_BLOCK_VAES256_AOS_x2(m3, m4, k3, k4, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, off, mb, ms) \
	do { \
		ENC_256_BLOCK_LAST_VAES256_AOS_x2(m1, m2, k1, k2, off, mb, ms); \
		ENC_256_BLOCK_LAST_VAES256_AOS_x2(m3, m4, k3, k4, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, off, mb, ms) \
	do { \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, off, mb, ms); \
		ENC_256_BLOCK_VAES256_AOS_x4(m5, m6, m7, m8, k5, k6, k7, k8, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, off, mb, ms) \
	do { \
		ENC_256_BLOCK_LAST_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, off, mb, ms); \
		ENC_256_BLOCK_LAST_VAES256_AOS_x4(m5, m6, m7, m8, k5, k6, k7, k8, off, mb, ms); \
	} while (0)

#ifdef RIJNDAEL_VAES256_VBMI
#define VAES256_AOS_MASKS(mb, ms) \
	const __m256i _idx_aos = _mm256_setr_epi8(RIJNDAEL256_VBMI_AOS_IDX)

#define VAES256_PACKED_MASKS(mb, ms) \
	const __m256i _idx_l = _mm256_setr_epi8(RIJNDAEL256_VBMI_PACKED_L_IDX); \
	const __m256i _idx_r = _mm256_setr_epi8(RIJNDAEL256_VBMI_PACKED_R_IDX)
#else
#define VAES256_AOS_MASKS(mb, ms) \
	const __m128i _mb128 = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000); \
	const __m128i _ms128 = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100); \
	const __m256i mb = _mm256_broadcastsi128_si256(_mb128); \
	const __m256i ms = _mm256_broadcastsi128_si256(_ms128)

#define VAES256_PACKED_MASKS(mb, ms) VAES256_AOS_MASKS(mb, ms)
#endif

#define DO_ENC_BLOCK_VAES256_AOS_256_256(m, k) \
	do { \
		VAES256_AOS_MASKS(_mb256, _ms256); \
		ADD_RK_PAIR_VAES256_AOS(m, k); \
		ENC_256_BLOCK_VAES256_AOS(m, k,  2, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k,  4, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k,  6, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k,  8, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 10, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 12, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 14, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 16, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 18, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 20, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 22, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 24, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS(m, k, 26, _mb256, _ms256); \
		ENC_256_BLOCK_LAST_VAES256_AOS(m, k, 28, _mb256, _ms256); \
	} while (0)

#define DO_ENC_BLOCK_VAES256_AOS_256_256_x2(m1, m2, k1, k2) \
	do { \
		VAES256_AOS_MASKS(_mb256, _ms256); \
		ADD_RK_PAIR_VAES256_AOS(m1, k1); \
		ADD_RK_PAIR_VAES256_AOS(m2, k2); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2,  2, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2,  4, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2,  6, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2,  8, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 10, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 12, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 14, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 16, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 18, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 20, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 22, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 24, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x2(m1, m2, k1, k2, 26, _mb256, _ms256); \
		ENC_256_BLOCK_LAST_VAES256_AOS_x2(m1, m2, k1, k2, 28, _mb256, _ms256); \
	} while (0)

#define DO_ENC_BLOCK_VAES256_AOS_256_256_x4(m1, m2, m3, m4, k1, k2, k3, k4) \
	do { \
		VAES256_AOS_MASKS(_mb256, _ms256); \
		ADD_RK_PAIR_VAES256_AOS(m1, k1); \
		ADD_RK_PAIR_VAES256_AOS(m2, k2); \
		ADD_RK_PAIR_VAES256_AOS(m3, k3); \
		ADD_RK_PAIR_VAES256_AOS(m4, k4); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4,  2, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4,  4, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4,  6, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4,  8, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 10, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 12, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 14, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 16, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 18, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 20, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 22, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 24, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 26, _mb256, _ms256); \
		ENC_256_BLOCK_LAST_VAES256_AOS_x4(m1, m2, m3, m4, k1, k2, k3, k4, 28, _mb256, _ms256); \
	} while (0)

#define DO_ENC_BLOCK_VAES256_AOS_256_256_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8) \
	do { \
		VAES256_AOS_MASKS(_mb256, _ms256); \
		ADD_RK_PAIR_VAES256_AOS(m1, k1); \
		ADD_RK_PAIR_VAES256_AOS(m2, k2); \
		ADD_RK_PAIR_VAES256_AOS(m3, k3); \
		ADD_RK_PAIR_VAES256_AOS(m4, k4); \
		ADD_RK_PAIR_VAES256_AOS(m5, k5); \
		ADD_RK_PAIR_VAES256_AOS(m6, k6); \
		ADD_RK_PAIR_VAES256_AOS(m7, k7); \
		ADD_RK_PAIR_VAES256_AOS(m8, k8); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8,  2, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8,  4, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8,  6, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8,  8, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 10, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 12, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 14, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 16, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 18, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 20, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 22, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 24, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 26, _mb256, _ms256); \
		ENC_256_BLOCK_LAST_VAES256_AOS_x8(m1, m2, m3, m4, m5, m6, m7, m8, k1, k2, k3, k4, k5, k6, k7, k8, 28, _mb256, _ms256); \
	} while (0)

/* Pairwise SoA core used by the compound contexts.  The round keys are
 * transposed once during setkey_xN, avoiding both state lane swaps and
 * per-encryption key packing. */
#define LOAD_RK_VAES256_PACKED(k, off) \
	_mm256_loadu_si256((const __m256i *)(const void *)(k)[off])

#ifdef RIJNDAEL_VAES256_VBMI
#define ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, off, mb, ms) \
	do { \
		__m256i _tmpl = _mm256_permutex2var_epi8((ml), _idx_l, (mr)); \
		__m256i _tmpr = _mm256_permutex2var_epi8((ml), _idx_r, (mr)); \
		(ml) = _mm256_aesenc_epi128(_tmpl, LOAD_RK_VAES256_PACKED(k, off)); \
		(mr) = _mm256_aesenc_epi128(_tmpr, LOAD_RK_VAES256_PACKED(k, (off) + 1)); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_PACKED(ml, mr, k, off, mb, ms) \
	do { \
		__m256i _tmpl = _mm256_permutex2var_epi8((ml), _idx_l, (mr)); \
		__m256i _tmpr = _mm256_permutex2var_epi8((ml), _idx_r, (mr)); \
		(ml) = _mm256_aesenclast_epi128(_tmpl, LOAD_RK_VAES256_PACKED(k, off)); \
		(mr) = _mm256_aesenclast_epi128(_tmpr, LOAD_RK_VAES256_PACKED(k, (off) + 1)); \
	} while (0)
#else
#define ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, off, mb, ms) \
	do { \
		__m256i _tmpl, _tmpr; \
		RINJDAEL_COMPENSATE_SR_VAES256(ml, mr, _tmpl, _tmpr, mb, ms); \
		(ml) = _mm256_aesenc_epi128(_tmpl, LOAD_RK_VAES256_PACKED(k, off)); \
		(mr) = _mm256_aesenc_epi128(_tmpr, LOAD_RK_VAES256_PACKED(k, (off) + 1)); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_PACKED(ml, mr, k, off, mb, ms) \
	do { \
		__m256i _tmpl, _tmpr; \
		RINJDAEL_COMPENSATE_SR_VAES256(ml, mr, _tmpl, _tmpr, mb, ms); \
		(ml) = _mm256_aesenclast_epi128(_tmpl, LOAD_RK_VAES256_PACKED(k, off)); \
		(mr) = _mm256_aesenclast_epi128(_tmpr, LOAD_RK_VAES256_PACKED(k, (off) + 1)); \
	} while (0)
#endif

#define ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, off, mb, ms) \
	do { \
		ENC_256_BLOCK_VAES256_PACKED(ml1, mr1, k1, off, mb, ms); \
		ENC_256_BLOCK_VAES256_PACKED(ml2, mr2, k2, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, off, mb, ms) \
	do { \
		ENC_256_BLOCK_LAST_VAES256_PACKED(ml1, mr1, k1, off, mb, ms); \
		ENC_256_BLOCK_LAST_VAES256_PACKED(ml2, mr2, k2, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, off, mb, ms) \
	do { \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, off, mb, ms); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml3, mr3, ml4, mr4, k3, k4, off, mb, ms); \
	} while (0)

#define ENC_256_BLOCK_LAST_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, off, mb, ms) \
	do { \
		ENC_256_BLOCK_LAST_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, off, mb, ms); \
		ENC_256_BLOCK_LAST_VAES256_PACKED_x2(ml3, mr3, ml4, mr4, k3, k4, off, mb, ms); \
	} while (0)

#define DO_ENC_BLOCK_VAES256_PACKED_256_256(ml, mr, k) \
	do { \
		VAES256_PACKED_MASKS(_mb256, _ms256); \
		(ml) = _mm256_xor_si256((ml), LOAD_RK_VAES256_PACKED(k, 0)); \
		(mr) = _mm256_xor_si256((mr), LOAD_RK_VAES256_PACKED(k, 1)); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k,  2, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k,  4, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k,  6, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k,  8, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 10, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 12, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 14, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 16, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 18, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 20, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 22, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 24, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED(ml, mr, k, 26, _mb256, _ms256); \
		ENC_256_BLOCK_LAST_VAES256_PACKED(ml, mr, k, 28, _mb256, _ms256); \
	} while (0)

#define DO_ENC_BLOCK_VAES256_PACKED_256_256_x2(ml1, mr1, ml2, mr2, k1, k2) \
	do { \
		VAES256_PACKED_MASKS(_mb256, _ms256); \
		(ml1) = _mm256_xor_si256((ml1), LOAD_RK_VAES256_PACKED(k1, 0)); \
		(mr1) = _mm256_xor_si256((mr1), LOAD_RK_VAES256_PACKED(k1, 1)); \
		(ml2) = _mm256_xor_si256((ml2), LOAD_RK_VAES256_PACKED(k2, 0)); \
		(mr2) = _mm256_xor_si256((mr2), LOAD_RK_VAES256_PACKED(k2, 1)); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2,  2, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2,  4, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2,  6, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2,  8, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 10, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 12, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 14, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 16, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 18, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 20, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 22, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 24, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 26, _mb256, _ms256); \
		ENC_256_BLOCK_LAST_VAES256_PACKED_x2(ml1, mr1, ml2, mr2, k1, k2, 28, _mb256, _ms256); \
	} while (0)

#define DO_ENC_BLOCK_VAES256_PACKED_256_256_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4) \
	do { \
		VAES256_PACKED_MASKS(_mb256, _ms256); \
		(ml1) = _mm256_xor_si256((ml1), LOAD_RK_VAES256_PACKED(k1, 0)); \
		(mr1) = _mm256_xor_si256((mr1), LOAD_RK_VAES256_PACKED(k1, 1)); \
		(ml2) = _mm256_xor_si256((ml2), LOAD_RK_VAES256_PACKED(k2, 0)); \
		(mr2) = _mm256_xor_si256((mr2), LOAD_RK_VAES256_PACKED(k2, 1)); \
		(ml3) = _mm256_xor_si256((ml3), LOAD_RK_VAES256_PACKED(k3, 0)); \
		(mr3) = _mm256_xor_si256((mr3), LOAD_RK_VAES256_PACKED(k3, 1)); \
		(ml4) = _mm256_xor_si256((ml4), LOAD_RK_VAES256_PACKED(k4, 0)); \
		(mr4) = _mm256_xor_si256((mr4), LOAD_RK_VAES256_PACKED(k4, 1)); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4,  2, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4,  4, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4,  6, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4,  8, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 10, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 12, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 14, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 16, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 18, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 20, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 22, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 24, _mb256, _ms256); \
		ENC_256_BLOCK_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 26, _mb256, _ms256); \
		ENC_256_BLOCK_LAST_VAES256_PACKED_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, k1, k2, k3, k4, 28, _mb256, _ms256); \
	} while (0)

#endif /* RIJNDAEL_VAES256 */

/* ======== Key schedule helpers ======== */

#define AES_128_KEY_EXP_VAES(k, rcon) \
	aes_128_key_expansion_vaes((k), _mm_aeskeygenassist_si128((k), (rcon)))

static inline __m128i aes_128_key_expansion_vaes(__m128i key, __m128i keygened) {
	keygened = _mm_shuffle_epi32(keygened, _MM_SHUFFLE(3, 3, 3, 3));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	return _mm_xor_si128(key, keygened);
}

static inline void KEY_256_ASSIST_1_VAES(__m128i *temp1, __m128i *temp2) {
	__m128i temp4;
	*temp2 = _mm_shuffle_epi32(*temp2, 0xff);
	temp4 = _mm_slli_si128(*temp1, 0x4);
	*temp1 = _mm_xor_si128(*temp1, temp4);
	temp4 = _mm_slli_si128(temp4, 0x4);
	*temp1 = _mm_xor_si128(*temp1, temp4);
	temp4 = _mm_slli_si128(temp4, 0x4);
	*temp1 = _mm_xor_si128(*temp1, temp4);
	*temp1 = _mm_xor_si128(*temp1, *temp2);
}

static inline void KEY_256_ASSIST_2_VAES(__m128i *temp1, __m128i *temp3) {
	__m128i temp2, temp4;
	temp4 = _mm_aeskeygenassist_si128(*temp1, 0x0);
	temp2 = _mm_shuffle_epi32(temp4, 0xaa);
	temp4 = _mm_slli_si128(*temp3, 0x4);
	*temp3 = _mm_xor_si128(*temp3, temp4);
	temp4 = _mm_slli_si128(temp4, 0x4);
	*temp3 = _mm_xor_si128(*temp3, temp4);
	temp4 = _mm_slli_si128(temp4, 0x4);
	*temp3 = _mm_xor_si128(*temp3, temp4);
	*temp3 = _mm_xor_si128(*temp3, temp2);
}

/* ======== Key schedule ======== */

int aes128_vaes_setkey_enc(rijndael_vaes_ctx_aes128 *ctx, const uint8_t key[16]) {
	__m128i *ks;
	if ((ctx == NULL) || (key == NULL)) {
		return -1;
	}
	ks = (__m128i*)ctx->rk;
	ctx->rtype = AES128;
	ks[ 0] = _mm_loadu_si128((const __m128i*)key);
	ks[ 1] = AES_128_KEY_EXP_VAES(ks[ 0], 0x01);
	ks[ 2] = AES_128_KEY_EXP_VAES(ks[ 1], 0x02);
	ks[ 3] = AES_128_KEY_EXP_VAES(ks[ 2], 0x04);
	ks[ 4] = AES_128_KEY_EXP_VAES(ks[ 3], 0x08);
	ks[ 5] = AES_128_KEY_EXP_VAES(ks[ 4], 0x10);
	ks[ 6] = AES_128_KEY_EXP_VAES(ks[ 5], 0x20);
	ks[ 7] = AES_128_KEY_EXP_VAES(ks[ 6], 0x40);
	ks[ 8] = AES_128_KEY_EXP_VAES(ks[ 7], 0x80);
	ks[ 9] = AES_128_KEY_EXP_VAES(ks[ 8], 0x1B);
	ks[10] = AES_128_KEY_EXP_VAES(ks[ 9], 0x36);
	return 0;
}

int aes256_vaes_setkey_enc(rijndael_vaes_ctx_aes256 *ctx, const uint8_t key[32]) {
	__m128i temp1, temp2, temp3;
	__m128i *ks;
	if ((ctx == NULL) || (key == NULL)) {
		return -1;
	}
	ks = (__m128i*)ctx->rk;
	ctx->rtype = AES256;
	temp1 = _mm_loadu_si128((__m128i*)key);
	temp3 = _mm_loadu_si128((__m128i*)(key + 16));
	ks[0] = temp1;
	ks[1] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x01); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 2] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 3] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x02); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 4] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 5] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x04); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 6] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 7] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x08); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 8] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 9] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x10); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[10] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[11] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x20); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[12] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[13] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x40); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[14] = temp1;
	return 0;
}

int rijndael256_vaes_setkey_enc(rijndael_vaes_ctx_rijndael256 *ctx, const uint8_t key[32]) {
	__m128i temp1, temp2, temp3;
	__m128i *ks;
	if ((ctx == NULL) || (key == NULL)) {
		return -1;
	}
	ks = (__m128i*)ctx->rk;
	ctx->rtype = RIJNDAEL_256_256;
	temp1 = _mm_loadu_si128((__m128i*)key);
	temp3 = _mm_loadu_si128((__m128i*)(key + 16));
	ks[ 0] = temp1;
	ks[ 1] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x01); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 2] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 3] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x02); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 4] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 5] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x04); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 6] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 7] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x08); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[ 8] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[ 9] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x10); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[10] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[11] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x20); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[12] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[13] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x40); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[14] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[15] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x80); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[16] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[17] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x1b); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[18] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[19] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x36); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[20] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[21] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x6c); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[22] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[23] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0xd8); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[24] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[25] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0xab); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[26] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[27] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x4d); KEY_256_ASSIST_1_VAES(&temp1, &temp2); ks[28] = temp1;
	KEY_256_ASSIST_2_VAES(&temp1, &temp3); ks[29] = temp3;
	return 0;
}

/* ======== Single-block encryption ======== */

int aes128_vaes_enc(const rijndael_vaes_ctx_aes128 *ctx, const uint8_t plainText[16], uint8_t cipherText[16]) {
	__m128i m;
	int ret = -1;
	if ((ctx == NULL) || (ctx->rtype != AES128)) { goto err; }
	m = _mm_loadu_si128((const __m128i*)plainText);
	DO_ENC_BLOCK_VAES_128_128(m, ctx->rk);
	_mm_storeu_si128((__m128i*)cipherText, m);
	ret = 0;
err:
	return ret;
}

int aes256_vaes_enc(const rijndael_vaes_ctx_aes256 *ctx, const uint8_t plainText[16], uint8_t cipherText[16]) {
	__m128i m;
	int ret = -1;
	if ((ctx == NULL) || (ctx->rtype != AES256)) { goto err; }
	m = _mm_loadu_si128((const __m128i*)plainText);
	DO_ENC_BLOCK_VAES_128_256(m, ctx->rk);
	_mm_storeu_si128((__m128i*)cipherText, m);
	ret = 0;
err:
	return ret;
}

int rijndael256_vaes_enc(const rijndael_vaes_ctx_rijndael256 *ctx, const uint8_t plainText[32], uint8_t cipherText[32]) {
	int ret = -1;
	if ((ctx == NULL) || (ctx->rtype != RIJNDAEL_256_256)) { goto err; }
#ifdef RIJNDAEL_VAES256
	__m256i m = _mm256_loadu_si256((const __m256i *)(const void *)plainText);
	DO_ENC_BLOCK_VAES256_AOS_256_256(m, ctx->rk);
	_mm256_storeu_si256((__m256i *)(void *)cipherText, m);
#else
	__m128i ml = _mm_loadu_si128((const __m128i*)plainText);
	__m128i mr = _mm_loadu_si128((const __m128i*)(plainText + 16));
	DO_ENC_BLOCK_VAES_256_256(ml, mr, ctx->rk);
	_mm_storeu_si128((__m128i*)cipherText, ml);
	_mm_storeu_si128((__m128i*)(cipherText + 16), mr);
#endif
	ret = 0;
err:
	return ret;
}

/* ======== enc_xN: N independent-key parallel encryptions ======== */

#ifdef RIJNDAEL_VAES256
/* Pack round r of two *separate* (not pre-packed) AES-128 contexts into one
 * ymm lane pair, on the fly. Unlike LOAD_AES128_PAIR_RK (which reads a
 * pre-packed compound context, built once via aes128_vaes_setkey_enc_x8),
 * this works directly off N independent single-key contexts -- exactly
 * enc_xN's contract -- at the cost of a pack (insert) per round instead of
 * a single wide load. Still pays off: interleaving via _mm256_aesenc_epi128
 * removes half the round instructions versus N serial single-block chains. */
#define _VAES_PACK_AES128_RK(ctxA, ctxB, round) \
	_mm256_set_m128i((ctxB)->rk[(round)], (ctxA)->rk[(round)])

/* ...but "removes half the round instructions" only holds where a 256-bit
 * VAESENC is a single AES uop. On cores that split it into two 128-bit uops
 * (Zen1 through Zen3), the packed form buys no AES throughput whatsoever,
 * while the on-the-fly pack still costs two extra uops per AES uop: a
 * 128-bit load plus a vinserti128, every round, for every register pair.
 * The alternative is one _mm_aesenc_si128 per block with the round key read
 * straight out of the context as a (micro-fused) memory operand: same AES
 * throughput, none of the packing. Measured on Zen3, enc_x4 drops from 26.4
 * to 18.4 cycles that way, and enc_x8 follows.
 *
 * Cores with native 256-bit-or-wider AES units (Zen4+, Sunny Cove+) do halve
 * their AES uop count with the packed form, and Intel client cores retire
 * only one AESENC per cycle either way, so packing stays the better default
 * everywhere except Zen1..Zen3 -- which is exactly what is selected below.
 * Force either way from the build with -DRIJNDAEL_VAES_XN_128BIT_CHAINS or
 * -DNO_RIJNDAEL_VAES_XN_128BIT_CHAINS. Both paths compute the very same AES,
 * so this never changes an output byte. */
#if !defined(RIJNDAEL_VAES_XN_128BIT_CHAINS) && !defined(NO_RIJNDAEL_VAES_XN_128BIT_CHAINS)
#if defined(__znver1__) || defined(__znver2__) || defined(__znver3__)
#define RIJNDAEL_VAES_XN_128BIT_CHAINS
#endif
#endif
#endif

int aes128_vaes_enc_x2(const rijndael_vaes_ctx_aes128 *ctx1, const rijndael_vaes_ctx_aes128 *ctx2,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	if ((ctx1 == NULL) || (ctx1->rtype != AES128) || (ctx2 == NULL) || (ctx2->rtype != AES128)) {
		return -1;
	}
#if defined(RIJNDAEL_VAES256) && defined(RIJNDAEL_VAES_XN_128BIT_CHAINS)
	{
		unsigned int r;
		__m128i m0 = _mm_loadu_si128((const __m128i*)plainText1);
		__m128i m1 = _mm_loadu_si128((const __m128i*)plainText2);
		m0 = _mm_xor_si128(m0, ctx1->rk[0]);
		m1 = _mm_xor_si128(m1, ctx2->rk[0]);
		for (r = 1; r < 10; r++) {
			m0 = _mm_aesenc_si128(m0, ctx1->rk[r]);
			m1 = _mm_aesenc_si128(m1, ctx2->rk[r]);
		}
		m0 = _mm_aesenclast_si128(m0, ctx1->rk[10]);
		m1 = _mm_aesenclast_si128(m1, ctx2->rk[10]);
		_mm_storeu_si128((__m128i*)cipherText1, m0);
		_mm_storeu_si128((__m128i*)cipherText2, m1);
		return 0;
	}
#elif defined(RIJNDAEL_VAES256)
	unsigned int r;
	__m256i m = _mm256_set_m128i(_mm_loadu_si128((const __m128i*)plainText2), _mm_loadu_si128((const __m128i*)plainText1));
	m = _mm256_xor_si256(m, _VAES_PACK_AES128_RK(ctx1, ctx2, 0));
	for (r = 1; r < 10; r++) {
		m = _mm256_aesenc_epi128(m, _VAES_PACK_AES128_RK(ctx1, ctx2, r));
	}
	m = _mm256_aesenclast_epi128(m, _VAES_PACK_AES128_RK(ctx1, ctx2, 10));
	_mm_storeu_si128((__m128i*)cipherText1, _mm256_castsi256_si128(m));
	_mm_storeu_si128((__m128i*)cipherText2, _mm256_extracti128_si256(m, 1));
	return 0;
#else
	int ret = 0;
	ret |= aes128_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= aes128_vaes_enc(ctx2, plainText2, cipherText2);
	return ret;
#endif
}

int aes128_vaes_enc_x4(const rijndael_vaes_ctx_aes128 *ctx1, const rijndael_vaes_ctx_aes128 *ctx2,
	const rijndael_vaes_ctx_aes128 *ctx3, const rijndael_vaes_ctx_aes128 *ctx4,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	const uint8_t plainText3[16], const uint8_t plainText4[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16],
	uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	if ((ctx1 == NULL) || (ctx1->rtype != AES128) || (ctx2 == NULL) || (ctx2->rtype != AES128) ||
	    (ctx3 == NULL) || (ctx3->rtype != AES128) || (ctx4 == NULL) || (ctx4->rtype != AES128)) {
		return -1;
	}
#if defined(RIJNDAEL_VAES256) && defined(RIJNDAEL_VAES_XN_128BIT_CHAINS)
	{
		unsigned int r;
		__m128i m0 = _mm_loadu_si128((const __m128i*)plainText1);
		__m128i m1 = _mm_loadu_si128((const __m128i*)plainText2);
		__m128i m2 = _mm_loadu_si128((const __m128i*)plainText3);
		__m128i m3 = _mm_loadu_si128((const __m128i*)plainText4);
		m0 = _mm_xor_si128(m0, ctx1->rk[0]);
		m1 = _mm_xor_si128(m1, ctx2->rk[0]);
		m2 = _mm_xor_si128(m2, ctx3->rk[0]);
		m3 = _mm_xor_si128(m3, ctx4->rk[0]);
		for (r = 1; r < 10; r++) {
			m0 = _mm_aesenc_si128(m0, ctx1->rk[r]);
			m1 = _mm_aesenc_si128(m1, ctx2->rk[r]);
			m2 = _mm_aesenc_si128(m2, ctx3->rk[r]);
			m3 = _mm_aesenc_si128(m3, ctx4->rk[r]);
		}
		m0 = _mm_aesenclast_si128(m0, ctx1->rk[10]);
		m1 = _mm_aesenclast_si128(m1, ctx2->rk[10]);
		m2 = _mm_aesenclast_si128(m2, ctx3->rk[10]);
		m3 = _mm_aesenclast_si128(m3, ctx4->rk[10]);
		_mm_storeu_si128((__m128i*)cipherText1, m0);
		_mm_storeu_si128((__m128i*)cipherText2, m1);
		_mm_storeu_si128((__m128i*)cipherText3, m2);
		_mm_storeu_si128((__m128i*)cipherText4, m3);
		return 0;
	}
#elif defined(RIJNDAEL_VAES256)
	unsigned int r;
	__m256i m0 = _mm256_set_m128i(_mm_loadu_si128((const __m128i*)plainText2), _mm_loadu_si128((const __m128i*)plainText1));
	__m256i m1 = _mm256_set_m128i(_mm_loadu_si128((const __m128i*)plainText4), _mm_loadu_si128((const __m128i*)plainText3));
	m0 = _mm256_xor_si256(m0, _VAES_PACK_AES128_RK(ctx1, ctx2, 0));
	m1 = _mm256_xor_si256(m1, _VAES_PACK_AES128_RK(ctx3, ctx4, 0));
	for (r = 1; r < 10; r++) {
		m0 = _mm256_aesenc_epi128(m0, _VAES_PACK_AES128_RK(ctx1, ctx2, r));
		m1 = _mm256_aesenc_epi128(m1, _VAES_PACK_AES128_RK(ctx3, ctx4, r));
	}
	m0 = _mm256_aesenclast_epi128(m0, _VAES_PACK_AES128_RK(ctx1, ctx2, 10));
	m1 = _mm256_aesenclast_epi128(m1, _VAES_PACK_AES128_RK(ctx3, ctx4, 10));
	_mm_storeu_si128((__m128i*)cipherText1, _mm256_castsi256_si128(m0));
	_mm_storeu_si128((__m128i*)cipherText2, _mm256_extracti128_si256(m0, 1));
	_mm_storeu_si128((__m128i*)cipherText3, _mm256_castsi256_si128(m1));
	_mm_storeu_si128((__m128i*)cipherText4, _mm256_extracti128_si256(m1, 1));
	return 0;
#else
	int ret = 0;
	ret |= aes128_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= aes128_vaes_enc(ctx2, plainText2, cipherText2);
	ret |= aes128_vaes_enc(ctx3, plainText3, cipherText3);
	ret |= aes128_vaes_enc(ctx4, plainText4, cipherText4);
	return ret;
#endif
}

int aes128_vaes_enc_x8(const rijndael_vaes_ctx_aes128 *ctx1, const rijndael_vaes_ctx_aes128 *ctx2,
	const rijndael_vaes_ctx_aes128 *ctx3, const rijndael_vaes_ctx_aes128 *ctx4,
	const rijndael_vaes_ctx_aes128 *ctx5, const rijndael_vaes_ctx_aes128 *ctx6,
	const rijndael_vaes_ctx_aes128 *ctx7, const rijndael_vaes_ctx_aes128 *ctx8,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	const uint8_t plainText3[16], const uint8_t plainText4[16],
	const uint8_t plainText5[16], const uint8_t plainText6[16],
	const uint8_t plainText7[16], const uint8_t plainText8[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16],
	uint8_t cipherText3[16], uint8_t cipherText4[16],
	uint8_t cipherText5[16], uint8_t cipherText6[16],
	uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	if ((ctx1 == NULL) || (ctx1->rtype != AES128) || (ctx2 == NULL) || (ctx2->rtype != AES128) ||
	    (ctx3 == NULL) || (ctx3->rtype != AES128) || (ctx4 == NULL) || (ctx4->rtype != AES128) ||
	    (ctx5 == NULL) || (ctx5->rtype != AES128) || (ctx6 == NULL) || (ctx6->rtype != AES128) ||
	    (ctx7 == NULL) || (ctx7->rtype != AES128) || (ctx8 == NULL) || (ctx8->rtype != AES128)) {
		return -1;
	}
#if defined(RIJNDAEL_VAES256) && defined(RIJNDAEL_VAES_XN_128BIT_CHAINS)
	{
		unsigned int r;
		__m128i m0 = _mm_loadu_si128((const __m128i*)plainText1);
		__m128i m1 = _mm_loadu_si128((const __m128i*)plainText2);
		__m128i m2 = _mm_loadu_si128((const __m128i*)plainText3);
		__m128i m3 = _mm_loadu_si128((const __m128i*)plainText4);
		__m128i m4 = _mm_loadu_si128((const __m128i*)plainText5);
		__m128i m5 = _mm_loadu_si128((const __m128i*)plainText6);
		__m128i m6 = _mm_loadu_si128((const __m128i*)plainText7);
		__m128i m7 = _mm_loadu_si128((const __m128i*)plainText8);
		m0 = _mm_xor_si128(m0, ctx1->rk[0]);
		m1 = _mm_xor_si128(m1, ctx2->rk[0]);
		m2 = _mm_xor_si128(m2, ctx3->rk[0]);
		m3 = _mm_xor_si128(m3, ctx4->rk[0]);
		m4 = _mm_xor_si128(m4, ctx5->rk[0]);
		m5 = _mm_xor_si128(m5, ctx6->rk[0]);
		m6 = _mm_xor_si128(m6, ctx7->rk[0]);
		m7 = _mm_xor_si128(m7, ctx8->rk[0]);
		for (r = 1; r < 10; r++) {
			m0 = _mm_aesenc_si128(m0, ctx1->rk[r]);
			m1 = _mm_aesenc_si128(m1, ctx2->rk[r]);
			m2 = _mm_aesenc_si128(m2, ctx3->rk[r]);
			m3 = _mm_aesenc_si128(m3, ctx4->rk[r]);
			m4 = _mm_aesenc_si128(m4, ctx5->rk[r]);
			m5 = _mm_aesenc_si128(m5, ctx6->rk[r]);
			m6 = _mm_aesenc_si128(m6, ctx7->rk[r]);
			m7 = _mm_aesenc_si128(m7, ctx8->rk[r]);
		}
		m0 = _mm_aesenclast_si128(m0, ctx1->rk[10]);
		m1 = _mm_aesenclast_si128(m1, ctx2->rk[10]);
		m2 = _mm_aesenclast_si128(m2, ctx3->rk[10]);
		m3 = _mm_aesenclast_si128(m3, ctx4->rk[10]);
		m4 = _mm_aesenclast_si128(m4, ctx5->rk[10]);
		m5 = _mm_aesenclast_si128(m5, ctx6->rk[10]);
		m6 = _mm_aesenclast_si128(m6, ctx7->rk[10]);
		m7 = _mm_aesenclast_si128(m7, ctx8->rk[10]);
		_mm_storeu_si128((__m128i*)cipherText1, m0);
		_mm_storeu_si128((__m128i*)cipherText2, m1);
		_mm_storeu_si128((__m128i*)cipherText3, m2);
		_mm_storeu_si128((__m128i*)cipherText4, m3);
		_mm_storeu_si128((__m128i*)cipherText5, m4);
		_mm_storeu_si128((__m128i*)cipherText6, m5);
		_mm_storeu_si128((__m128i*)cipherText7, m6);
		_mm_storeu_si128((__m128i*)cipherText8, m7);
		return 0;
	}
#elif defined(RIJNDAEL_VAES256)
	unsigned int r;
	__m256i m0 = _mm256_set_m128i(_mm_loadu_si128((const __m128i*)plainText2), _mm_loadu_si128((const __m128i*)plainText1));
	__m256i m1 = _mm256_set_m128i(_mm_loadu_si128((const __m128i*)plainText4), _mm_loadu_si128((const __m128i*)plainText3));
	__m256i m2 = _mm256_set_m128i(_mm_loadu_si128((const __m128i*)plainText6), _mm_loadu_si128((const __m128i*)plainText5));
	__m256i m3 = _mm256_set_m128i(_mm_loadu_si128((const __m128i*)plainText8), _mm_loadu_si128((const __m128i*)plainText7));
	m0 = _mm256_xor_si256(m0, _VAES_PACK_AES128_RK(ctx1, ctx2, 0));
	m1 = _mm256_xor_si256(m1, _VAES_PACK_AES128_RK(ctx3, ctx4, 0));
	m2 = _mm256_xor_si256(m2, _VAES_PACK_AES128_RK(ctx5, ctx6, 0));
	m3 = _mm256_xor_si256(m3, _VAES_PACK_AES128_RK(ctx7, ctx8, 0));
	for (r = 1; r < 10; r++) {
		m0 = _mm256_aesenc_epi128(m0, _VAES_PACK_AES128_RK(ctx1, ctx2, r));
		m1 = _mm256_aesenc_epi128(m1, _VAES_PACK_AES128_RK(ctx3, ctx4, r));
		m2 = _mm256_aesenc_epi128(m2, _VAES_PACK_AES128_RK(ctx5, ctx6, r));
		m3 = _mm256_aesenc_epi128(m3, _VAES_PACK_AES128_RK(ctx7, ctx8, r));
	}
	m0 = _mm256_aesenclast_epi128(m0, _VAES_PACK_AES128_RK(ctx1, ctx2, 10));
	m1 = _mm256_aesenclast_epi128(m1, _VAES_PACK_AES128_RK(ctx3, ctx4, 10));
	m2 = _mm256_aesenclast_epi128(m2, _VAES_PACK_AES128_RK(ctx5, ctx6, 10));
	m3 = _mm256_aesenclast_epi128(m3, _VAES_PACK_AES128_RK(ctx7, ctx8, 10));
	_mm_storeu_si128((__m128i*)cipherText1, _mm256_castsi256_si128(m0));
	_mm_storeu_si128((__m128i*)cipherText2, _mm256_extracti128_si256(m0, 1));
	_mm_storeu_si128((__m128i*)cipherText3, _mm256_castsi256_si128(m1));
	_mm_storeu_si128((__m128i*)cipherText4, _mm256_extracti128_si256(m1, 1));
	_mm_storeu_si128((__m128i*)cipherText5, _mm256_castsi256_si128(m2));
	_mm_storeu_si128((__m128i*)cipherText6, _mm256_extracti128_si256(m2, 1));
	_mm_storeu_si128((__m128i*)cipherText7, _mm256_castsi256_si128(m3));
	_mm_storeu_si128((__m128i*)cipherText8, _mm256_extracti128_si256(m3, 1));
	return 0;
#else
	int ret = 0;
	ret |= aes128_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= aes128_vaes_enc(ctx2, plainText2, cipherText2);
	ret |= aes128_vaes_enc(ctx3, plainText3, cipherText3);
	ret |= aes128_vaes_enc(ctx4, plainText4, cipherText4);
	ret |= aes128_vaes_enc(ctx5, plainText5, cipherText5);
	ret |= aes128_vaes_enc(ctx6, plainText6, cipherText6);
	ret |= aes128_vaes_enc(ctx7, plainText7, cipherText7);
	ret |= aes128_vaes_enc(ctx8, plainText8, cipherText8);
	return ret;
#endif
}

#ifdef RIJNDAEL_VAES256
#undef _VAES_PACK_AES128_RK
#endif

int aes256_vaes_enc_x2(const rijndael_vaes_ctx_aes256 *ctx1, const rijndael_vaes_ctx_aes256 *ctx2,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	int ret = 0;
	ret |= aes256_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= aes256_vaes_enc(ctx2, plainText2, cipherText2);
	return ret;
}

int aes256_vaes_enc_x4(const rijndael_vaes_ctx_aes256 *ctx1, const rijndael_vaes_ctx_aes256 *ctx2,
	const rijndael_vaes_ctx_aes256 *ctx3, const rijndael_vaes_ctx_aes256 *ctx4,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	const uint8_t plainText3[16], const uint8_t plainText4[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16],
	uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	int ret = 0;
	ret |= aes256_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= aes256_vaes_enc(ctx2, plainText2, cipherText2);
	ret |= aes256_vaes_enc(ctx3, plainText3, cipherText3);
	ret |= aes256_vaes_enc(ctx4, plainText4, cipherText4);
	return ret;
}

int aes256_vaes_enc_x8(const rijndael_vaes_ctx_aes256 *ctx1, const rijndael_vaes_ctx_aes256 *ctx2,
	const rijndael_vaes_ctx_aes256 *ctx3, const rijndael_vaes_ctx_aes256 *ctx4,
	const rijndael_vaes_ctx_aes256 *ctx5, const rijndael_vaes_ctx_aes256 *ctx6,
	const rijndael_vaes_ctx_aes256 *ctx7, const rijndael_vaes_ctx_aes256 *ctx8,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	const uint8_t plainText3[16], const uint8_t plainText4[16],
	const uint8_t plainText5[16], const uint8_t plainText6[16],
	const uint8_t plainText7[16], const uint8_t plainText8[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16],
	uint8_t cipherText3[16], uint8_t cipherText4[16],
	uint8_t cipherText5[16], uint8_t cipherText6[16],
	uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	int ret = 0;
	ret |= aes256_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= aes256_vaes_enc(ctx2, plainText2, cipherText2);
	ret |= aes256_vaes_enc(ctx3, plainText3, cipherText3);
	ret |= aes256_vaes_enc(ctx4, plainText4, cipherText4);
	ret |= aes256_vaes_enc(ctx5, plainText5, cipherText5);
	ret |= aes256_vaes_enc(ctx6, plainText6, cipherText6);
	ret |= aes256_vaes_enc(ctx7, plainText7, cipherText7);
	ret |= aes256_vaes_enc(ctx8, plainText8, cipherText8);
	return ret;
}

int rijndael256_vaes_enc_x2(const rijndael_vaes_ctx_rijndael256 *ctx1, const rijndael_vaes_ctx_rijndael256 *ctx2,
	const uint8_t plainText1[32], const uint8_t plainText2[32],
	uint8_t cipherText1[32], uint8_t cipherText2[32]) {
#ifdef RIJNDAEL_VAES256
	if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256) ||
	    (ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256)) {
		return -1;
	}
	__m256i m1 = _mm256_loadu_si256((const __m256i *)(const void *)plainText1);
	__m256i m2 = _mm256_loadu_si256((const __m256i *)(const void *)plainText2);
	DO_ENC_BLOCK_VAES256_AOS_256_256_x2(m1, m2, ctx1->rk, ctx2->rk);
	_mm256_storeu_si256((__m256i *)(void *)cipherText1, m1);
	_mm256_storeu_si256((__m256i *)(void *)cipherText2, m2);
	return 0;
#else
	int ret = 0;
	ret |= rijndael256_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= rijndael256_vaes_enc(ctx2, plainText2, cipherText2);
	return ret;
#endif
}

int rijndael256_vaes_enc_x4(const rijndael_vaes_ctx_rijndael256 *ctx1, const rijndael_vaes_ctx_rijndael256 *ctx2,
	const rijndael_vaes_ctx_rijndael256 *ctx3, const rijndael_vaes_ctx_rijndael256 *ctx4,
	const uint8_t plainText1[32], const uint8_t plainText2[32],
	const uint8_t plainText3[32], const uint8_t plainText4[32],
	uint8_t cipherText1[32], uint8_t cipherText2[32],
	uint8_t cipherText3[32], uint8_t cipherText4[32]) {
#ifdef RIJNDAEL_VAES256
	if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256) ||
	    (ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256) ||
	    (ctx3 == NULL) || (ctx3->rtype != RIJNDAEL_256_256) ||
	    (ctx4 == NULL) || (ctx4->rtype != RIJNDAEL_256_256)) {
		return -1;
	}
	__m256i m1 = _mm256_loadu_si256((const __m256i *)(const void *)plainText1);
	__m256i m2 = _mm256_loadu_si256((const __m256i *)(const void *)plainText2);
	__m256i m3 = _mm256_loadu_si256((const __m256i *)(const void *)plainText3);
	__m256i m4 = _mm256_loadu_si256((const __m256i *)(const void *)plainText4);
	DO_ENC_BLOCK_VAES256_AOS_256_256_x4(m1, m2, m3, m4,
		ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk);
	_mm256_storeu_si256((__m256i *)(void *)cipherText1, m1);
	_mm256_storeu_si256((__m256i *)(void *)cipherText2, m2);
	_mm256_storeu_si256((__m256i *)(void *)cipherText3, m3);
	_mm256_storeu_si256((__m256i *)(void *)cipherText4, m4);
	return 0;
#else
	int ret = 0;
	ret |= rijndael256_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= rijndael256_vaes_enc(ctx2, plainText2, cipherText2);
	ret |= rijndael256_vaes_enc(ctx3, plainText3, cipherText3);
	ret |= rijndael256_vaes_enc(ctx4, plainText4, cipherText4);
	return ret;
#endif
}

int rijndael256_vaes_enc_x8(const rijndael_vaes_ctx_rijndael256 *ctx1, const rijndael_vaes_ctx_rijndael256 *ctx2,
	const rijndael_vaes_ctx_rijndael256 *ctx3, const rijndael_vaes_ctx_rijndael256 *ctx4,
	const rijndael_vaes_ctx_rijndael256 *ctx5, const rijndael_vaes_ctx_rijndael256 *ctx6,
	const rijndael_vaes_ctx_rijndael256 *ctx7, const rijndael_vaes_ctx_rijndael256 *ctx8,
	const uint8_t plainText1[32], const uint8_t plainText2[32],
	const uint8_t plainText3[32], const uint8_t plainText4[32],
	const uint8_t plainText5[32], const uint8_t plainText6[32],
	const uint8_t plainText7[32], const uint8_t plainText8[32],
	uint8_t cipherText1[32], uint8_t cipherText2[32],
	uint8_t cipherText3[32], uint8_t cipherText4[32],
	uint8_t cipherText5[32], uint8_t cipherText6[32],
	uint8_t cipherText7[32], uint8_t cipherText8[32]) {
#ifdef RIJNDAEL_VAES256
	if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256) ||
	    (ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256) ||
	    (ctx3 == NULL) || (ctx3->rtype != RIJNDAEL_256_256) ||
	    (ctx4 == NULL) || (ctx4->rtype != RIJNDAEL_256_256) ||
	    (ctx5 == NULL) || (ctx5->rtype != RIJNDAEL_256_256) ||
	    (ctx6 == NULL) || (ctx6->rtype != RIJNDAEL_256_256) ||
	    (ctx7 == NULL) || (ctx7->rtype != RIJNDAEL_256_256) ||
	    (ctx8 == NULL) || (ctx8->rtype != RIJNDAEL_256_256)) {
		return -1;
	}
	__m256i m1 = _mm256_loadu_si256((const __m256i *)(const void *)plainText1);
	__m256i m2 = _mm256_loadu_si256((const __m256i *)(const void *)plainText2);
	__m256i m3 = _mm256_loadu_si256((const __m256i *)(const void *)plainText3);
	__m256i m4 = _mm256_loadu_si256((const __m256i *)(const void *)plainText4);
	__m256i m5 = _mm256_loadu_si256((const __m256i *)(const void *)plainText5);
	__m256i m6 = _mm256_loadu_si256((const __m256i *)(const void *)plainText6);
	__m256i m7 = _mm256_loadu_si256((const __m256i *)(const void *)plainText7);
	__m256i m8 = _mm256_loadu_si256((const __m256i *)(const void *)plainText8);
	DO_ENC_BLOCK_VAES256_AOS_256_256_x8(m1, m2, m3, m4, m5, m6, m7, m8,
		ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk,
		ctx5->rk, ctx6->rk, ctx7->rk, ctx8->rk);
	_mm256_storeu_si256((__m256i *)(void *)cipherText1, m1);
	_mm256_storeu_si256((__m256i *)(void *)cipherText2, m2);
	_mm256_storeu_si256((__m256i *)(void *)cipherText3, m3);
	_mm256_storeu_si256((__m256i *)(void *)cipherText4, m4);
	_mm256_storeu_si256((__m256i *)(void *)cipherText5, m5);
	_mm256_storeu_si256((__m256i *)(void *)cipherText6, m6);
	_mm256_storeu_si256((__m256i *)(void *)cipherText7, m7);
	_mm256_storeu_si256((__m256i *)(void *)cipherText8, m8);
	return 0;
#else
	int ret = 0;
	ret |= rijndael256_vaes_enc(ctx1, plainText1, cipherText1);
	ret |= rijndael256_vaes_enc(ctx2, plainText2, cipherText2);
	ret |= rijndael256_vaes_enc(ctx3, plainText3, cipherText3);
	ret |= rijndael256_vaes_enc(ctx4, plainText4, cipherText4);
	ret |= rijndael256_vaes_enc(ctx5, plainText5, cipherText5);
	ret |= rijndael256_vaes_enc(ctx6, plainText6, cipherText6);
	ret |= rijndael256_vaes_enc(ctx7, plainText7, cipherText7);
	ret |= rijndael256_vaes_enc(ctx8, plainText8, cipherText8);
	return ret;
#endif
}

/* ======== Compound contexts (xN_xN and setkey_xN) ======== */

#ifdef RIJNDAEL_VAES256

#define STORE_AES128_KEY_PAIR(ctx, round, k0, k1) \
	_mm256_storeu_si256((__m256i *)(void *)(ctx)->rk[(round)], \
		_mm256_set_m128i((k1), (k0)))

#define EXPAND_AES128_KEY_PAIR(ctx, round, rcon, k0, k1) \
	do { \
		(k0) = AES_128_KEY_EXP_VAES((k0), (rcon)); \
		(k1) = AES_128_KEY_EXP_VAES((k1), (rcon)); \
		STORE_AES128_KEY_PAIR((ctx), (round), (k0), (k1)); \
	} while (0)

#define EXPAND_AES128_KEY_QUAD(ctx, round, rcon, k0, k1, k2, k3) \
	do { \
		(k0) = AES_128_KEY_EXP_VAES((k0), (rcon)); \
		(k1) = AES_128_KEY_EXP_VAES((k1), (rcon)); \
		(k2) = AES_128_KEY_EXP_VAES((k2), (rcon)); \
		(k3) = AES_128_KEY_EXP_VAES((k3), (rcon)); \
		STORE_AES128_KEY_PAIR(&(ctx)->ctx[0], (round), (k0), (k1)); \
		STORE_AES128_KEY_PAIR(&(ctx)->ctx[1], (round), (k2), (k3)); \
	} while (0)

#define EXPAND_AES128_KEY_OCTET(ctx, round, rcon, k0, k1, k2, k3, k4, k5, k6, k7) \
	do { \
		(k0) = AES_128_KEY_EXP_VAES((k0), (rcon)); \
		(k1) = AES_128_KEY_EXP_VAES((k1), (rcon)); \
		(k2) = AES_128_KEY_EXP_VAES((k2), (rcon)); \
		(k3) = AES_128_KEY_EXP_VAES((k3), (rcon)); \
		(k4) = AES_128_KEY_EXP_VAES((k4), (rcon)); \
		(k5) = AES_128_KEY_EXP_VAES((k5), (rcon)); \
		(k6) = AES_128_KEY_EXP_VAES((k6), (rcon)); \
		(k7) = AES_128_KEY_EXP_VAES((k7), (rcon)); \
		STORE_AES128_KEY_PAIR(&(ctx)->ctx[0], (round), (k0), (k1)); \
		STORE_AES128_KEY_PAIR(&(ctx)->ctx[1], (round), (k2), (k3)); \
		STORE_AES128_KEY_PAIR(&(ctx)->ctx[2], (round), (k4), (k5)); \
		STORE_AES128_KEY_PAIR(&(ctx)->ctx[3], (round), (k6), (k7)); \
	} while (0)

#define LOAD_AES128_PAIR(pt0, pt1) \
	_mm256_set_m128i( \
		_mm_loadu_si128((const __m128i *)(const void *)(pt1)), \
		_mm_loadu_si128((const __m128i *)(const void *)(pt0)))

#define STORE_AES128_PAIR(ct0, ct1, m) \
	do { \
		_mm_storeu_si128((__m128i *)(void *)(ct0), _mm256_castsi256_si128(m)); \
		_mm_storeu_si128((__m128i *)(void *)(ct1), _mm256_extracti128_si256((m), 1)); \
	} while (0)

#define LOAD_AES128_PAIR_RK(ctx, round) \
	_mm256_loadu_si256((const __m256i *)(const void *)(ctx)->rk[(round)])

#ifdef RIJNDAEL_VAES_GFNI_KEYSCHED
#ifndef __GFNI__
#error "RIJNDAEL_VAES_GFNI_KEYSCHED requires compiler GFNI support"
#endif

#define LOAD_AES128_KEY_PAIR(key0, key1) \
	_mm256_set_m128i( \
		_mm_loadu_si128((const __m128i *)(const void *)(key1)), \
		_mm_loadu_si128((const __m128i *)(const void *)(key0)))

#define STORE_AES128_PACKED_KEY(ctx, round, key) \
	_mm256_storeu_si256((__m256i *)(void *)(ctx)->rk[(round)], (key))

/* Apply SubWord(RotWord(last_word)) and the AES affine transform to both
 * 128-bit lanes at once. 0xf1e3c78f1f3e7cf8 is the GFNI matrix for the
 * AES S-box; the immediate 0x63 is its affine constant. */
static inline __m256i aes128_vaes_expand_key_pair_gfni(__m256i key, uint8_t rcon) {
	const __m256i rotword_broadcast = _mm256_setr_epi8(
		13, 14, 15, 12, 13, 14, 15, 12,
		13, 14, 15, 12, 13, 14, 15, 12,
		13, 14, 15, 12, 13, 14, 15, 12,
		13, 14, 15, 12, 13, 14, 15, 12);
	const __m256i aes_sbox_matrix =
		_mm256_set1_epi64x((long long)UINT64_C(0xf1e3c78f1f3e7cf8));
	__m256i assist;

	assist = _mm256_shuffle_epi8(key, rotword_broadcast);
	assist = _mm256_gf2p8affineinv_epi64_epi8(
		assist, aes_sbox_matrix, 0x63);
	/* One Rcon byte per repeated 32-bit word. */
	assist = _mm256_xor_si256(assist, _mm256_set1_epi32((int)rcon));
	key = _mm256_xor_si256(key, _mm256_slli_si256(key, 4));
	key = _mm256_xor_si256(key, _mm256_slli_si256(key, 4));
	key = _mm256_xor_si256(key, _mm256_slli_si256(key, 4));
	return _mm256_xor_si256(key, assist);
}

#define EXPAND_AES128_GFNI_PAIR(ctx, round, rcon, key) \
	do { \
		(key) = aes128_vaes_expand_key_pair_gfni((key), (rcon)); \
		STORE_AES128_PACKED_KEY((ctx), (round), (key)); \
	} while (0)

#define EXPAND_AES128_GFNI_QUAD(ctx, round, rcon, key0, key1) \
	do { \
		(key0) = aes128_vaes_expand_key_pair_gfni((key0), (rcon)); \
		(key1) = aes128_vaes_expand_key_pair_gfni((key1), (rcon)); \
		STORE_AES128_PACKED_KEY(&(ctx)->ctx[0], (round), (key0)); \
		STORE_AES128_PACKED_KEY(&(ctx)->ctx[1], (round), (key1)); \
	} while (0)

#define EXPAND_AES128_GFNI_OCTET(ctx, round, rcon, key0, key1, key2, key3) \
	do { \
		(key0) = aes128_vaes_expand_key_pair_gfni((key0), (rcon)); \
		(key1) = aes128_vaes_expand_key_pair_gfni((key1), (rcon)); \
		(key2) = aes128_vaes_expand_key_pair_gfni((key2), (rcon)); \
		(key3) = aes128_vaes_expand_key_pair_gfni((key3), (rcon)); \
		STORE_AES128_PACKED_KEY(&(ctx)->ctx[0], (round), (key0)); \
		STORE_AES128_PACKED_KEY(&(ctx)->ctx[1], (round), (key1)); \
		STORE_AES128_PACKED_KEY(&(ctx)->ctx[2], (round), (key2)); \
		STORE_AES128_PACKED_KEY(&(ctx)->ctx[3], (round), (key3)); \
	} while (0)

WEAK int aes128_vaes_setkey_enc_x2(rijndael_vaes_ctx_aes128_x2 *ctx,
	const uint8_t key1[16], const uint8_t key2[16]) {
	__m256i key;

	if ((ctx == NULL) || (key1 == NULL) || (key2 == NULL)) {
		return -1;
	}
	key = LOAD_AES128_KEY_PAIR(key1, key2);
	ctx->rtype = AES128;
	STORE_AES128_PACKED_KEY(ctx, 0, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  1, 0x01, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  2, 0x02, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  3, 0x04, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  4, 0x08, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  5, 0x10, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  6, 0x20, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  7, 0x40, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  8, 0x80, key);
	EXPAND_AES128_GFNI_PAIR(ctx,  9, 0x1b, key);
	EXPAND_AES128_GFNI_PAIR(ctx, 10, 0x36, key);
	return 0;
}

WEAK int aes128_vaes_setkey_enc_x4(rijndael_vaes_ctx_aes128_x4 *ctx,
	const uint8_t key1[16], const uint8_t key2[16],
	const uint8_t key3[16], const uint8_t key4[16]) {
	__m256i key0, key1_pair;

	if ((ctx == NULL) || (key1 == NULL) || (key2 == NULL) ||
	    (key3 == NULL) || (key4 == NULL)) {
		return -1;
	}
	key0 = LOAD_AES128_KEY_PAIR(key1, key2);
	key1_pair = LOAD_AES128_KEY_PAIR(key3, key4);
	ctx->ctx[0].rtype = AES128;
	ctx->ctx[1].rtype = AES128;
	STORE_AES128_PACKED_KEY(&ctx->ctx[0], 0, key0);
	STORE_AES128_PACKED_KEY(&ctx->ctx[1], 0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  1, 0x01, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  2, 0x02, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  3, 0x04, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  4, 0x08, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  5, 0x10, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  6, 0x20, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  7, 0x40, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  8, 0x80, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx,  9, 0x1b, key0, key1_pair);
	EXPAND_AES128_GFNI_QUAD(ctx, 10, 0x36, key0, key1_pair);
	return 0;
}

WEAK int aes128_vaes_setkey_enc_x8(rijndael_vaes_ctx_aes128_x8 *ctx,
	const uint8_t key1[16], const uint8_t key2[16],
	const uint8_t key3[16], const uint8_t key4[16],
	const uint8_t key5[16], const uint8_t key6[16],
	const uint8_t key7[16], const uint8_t key8[16]) {
	__m256i key0, key1_pair, key2_pair, key3_pair;

	if ((ctx == NULL) || (key1 == NULL) || (key2 == NULL) ||
	    (key3 == NULL) || (key4 == NULL) || (key5 == NULL) ||
	    (key6 == NULL) || (key7 == NULL) || (key8 == NULL)) {
		return -1;
	}
	key0 = LOAD_AES128_KEY_PAIR(key1, key2);
	key1_pair = LOAD_AES128_KEY_PAIR(key3, key4);
	key2_pair = LOAD_AES128_KEY_PAIR(key5, key6);
	key3_pair = LOAD_AES128_KEY_PAIR(key7, key8);
	ctx->ctx[0].rtype = AES128;
	ctx->ctx[1].rtype = AES128;
	ctx->ctx[2].rtype = AES128;
	ctx->ctx[3].rtype = AES128;
	STORE_AES128_PACKED_KEY(&ctx->ctx[0], 0, key0);
	STORE_AES128_PACKED_KEY(&ctx->ctx[1], 0, key1_pair);
	STORE_AES128_PACKED_KEY(&ctx->ctx[2], 0, key2_pair);
	STORE_AES128_PACKED_KEY(&ctx->ctx[3], 0, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  1, 0x01, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  2, 0x02, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  3, 0x04, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  4, 0x08, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  5, 0x10, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  6, 0x20, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  7, 0x40, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  8, 0x80, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx,  9, 0x1b, key0, key1_pair, key2_pair, key3_pair);
	EXPAND_AES128_GFNI_OCTET(ctx, 10, 0x36, key0, key1_pair, key2_pair, key3_pair);
	return 0;
}

#undef EXPAND_AES128_GFNI_OCTET
#undef EXPAND_AES128_GFNI_QUAD

/* ---- Rijndael-256-256 key schedule, GFNI, two keys per ymm ----------------
 * Same rationale as the AES-128 case above: AESKEYGENASSIST has no ymm form, so
 * it cannot batch two independent schedules; VGF2P8AFFINEINVQB does, so a pair
 * runs lane-parallel (lane 0 = key1, lane 1 = key2).
 *
 * Nk=8, so the schedule alternates two SubWord flavours - the only structural
 * difference with AES-128:
 *   even round keys: SubWord(RotWord(w7)) ^ Rcon   (KEY_256_ASSIST_1_VAES)
 *   odd  round keys: SubWord(w3), no rotation, no Rcon (KEY_256_ASSIST_2_VAES)
 * Both are the same affine-inverse; only the byte shuffle and the Rcon differ.
 * The scalar reference reads those out of AESKEYGENASSIST with _mm_shuffle_epi32
 * 0xff and 0xaa respectively, which is what the two masks below reproduce.
 *
 * The three "x ^= x << 4" steps are the cumulative word chain: applied to the
 * running value rather than to a saved copy they yield k ^ k<<4 ^ k<<8 ^ k<<12,
 * exactly what the reference builds with its temp4 ladder. */
#define RIJNDAEL256_GFNI_ROTWORD_BCAST \
	_mm256_setr_epi8(13, 14, 15, 12, 13, 14, 15, 12, \
	                 13, 14, 15, 12, 13, 14, 15, 12, \
	                 13, 14, 15, 12, 13, 14, 15, 12, \
	                 13, 14, 15, 12, 13, 14, 15, 12)
#define RIJNDAEL256_GFNI_WORD_BCAST \
	_mm256_setr_epi8(12, 13, 14, 15, 12, 13, 14, 15, \
	                 12, 13, 14, 15, 12, 13, 14, 15, \
	                 12, 13, 14, 15, 12, 13, 14, 15, \
	                 12, 13, 14, 15, 12, 13, 14, 15)

static inline __m256i rijndael256_vaes_subword_gfni(__m256i src, __m256i shuf) {
	const __m256i aes_sbox_matrix =
		_mm256_set1_epi64x((long long)UINT64_C(0xf1e3c78f1f3e7cf8));
	return _mm256_gf2p8affineinv_epi64_epi8(
		_mm256_shuffle_epi8(src, shuf), aes_sbox_matrix, 0x63);
}

static inline __m256i rijndael256_vaes_chain_gfni(__m256i x, __m256i assist) {
	x = _mm256_xor_si256(x, _mm256_slli_si256(x, 4));
	x = _mm256_xor_si256(x, _mm256_slli_si256(x, 4));
	x = _mm256_xor_si256(x, _mm256_slli_si256(x, 4));
	return _mm256_xor_si256(x, assist);
}

static inline void rijndael256_vaes_setkey_pair_gfni(uint8_t rk[30][32],
	const uint8_t key1[32], const uint8_t key2[32]) {
	static const uint8_t rcon[14] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
	                                  0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d };
	__m256i t1, t3, assist;
	unsigned int r;

	t1 = _mm256_set_m128i(_mm_loadu_si128((const __m128i *)(const void *)key2),
	                      _mm_loadu_si128((const __m128i *)(const void *)key1));
	t3 = _mm256_set_m128i(_mm_loadu_si128((const __m128i *)(const void *)(key2 + 16)),
	                      _mm_loadu_si128((const __m128i *)(const void *)(key1 + 16)));
	_mm256_storeu_si256((__m256i *)(void *)rk[0], t1);
	_mm256_storeu_si256((__m256i *)(void *)rk[1], t3);

	for (r = 0; r < 14; r++) {
		assist = rijndael256_vaes_subword_gfni(t3, RIJNDAEL256_GFNI_ROTWORD_BCAST);
		/* One Rcon byte per repeated 32-bit word, as in the AES-128 case. */
		assist = _mm256_xor_si256(assist, _mm256_set1_epi32((int)rcon[r]));
		t1 = rijndael256_vaes_chain_gfni(t1, assist);
		_mm256_storeu_si256((__m256i *)(void *)rk[2 + (2 * r)], t1);

		assist = rijndael256_vaes_subword_gfni(t1, RIJNDAEL256_GFNI_WORD_BCAST);
		t3 = rijndael256_vaes_chain_gfni(t3, assist);
		_mm256_storeu_si256((__m256i *)(void *)rk[3 + (2 * r)], t3);
	}
}

#undef RIJNDAEL256_GFNI_ROTWORD_BCAST
#undef RIJNDAEL256_GFNI_WORD_BCAST
#undef EXPAND_AES128_GFNI_PAIR
#undef STORE_AES128_PACKED_KEY
#undef LOAD_AES128_KEY_PAIR

#else

WEAK int aes128_vaes_setkey_enc_x2(rijndael_vaes_ctx_aes128_x2 *ctx,
	const uint8_t key1[16], const uint8_t key2[16]) {
	__m128i k0, k1;

	if ((ctx == NULL) || (key1 == NULL) || (key2 == NULL)) {
		return -1;
	}
	k0 = _mm_loadu_si128((const __m128i *)(const void *)key1);
	k1 = _mm_loadu_si128((const __m128i *)(const void *)key2);
	ctx->rtype = AES128;
	STORE_AES128_KEY_PAIR(ctx, 0, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  1, 0x01, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  2, 0x02, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  3, 0x04, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  4, 0x08, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  5, 0x10, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  6, 0x20, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  7, 0x40, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  8, 0x80, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx,  9, 0x1b, k0, k1);
	EXPAND_AES128_KEY_PAIR(ctx, 10, 0x36, k0, k1);
	return 0;
}

WEAK int aes128_vaes_setkey_enc_x4(rijndael_vaes_ctx_aes128_x4 *ctx,
	const uint8_t key1[16], const uint8_t key2[16],
	const uint8_t key3[16], const uint8_t key4[16]) {
	__m128i k0, k1, k2, k3;

	if ((ctx == NULL) || (key1 == NULL) || (key2 == NULL) ||
	    (key3 == NULL) || (key4 == NULL)) {
		return -1;
	}
	k0 = _mm_loadu_si128((const __m128i *)(const void *)key1);
	k1 = _mm_loadu_si128((const __m128i *)(const void *)key2);
	k2 = _mm_loadu_si128((const __m128i *)(const void *)key3);
	k3 = _mm_loadu_si128((const __m128i *)(const void *)key4);
	ctx->ctx[0].rtype = AES128;
	ctx->ctx[1].rtype = AES128;
	STORE_AES128_KEY_PAIR(&ctx->ctx[0], 0, k0, k1);
	STORE_AES128_KEY_PAIR(&ctx->ctx[1], 0, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  1, 0x01, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  2, 0x02, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  3, 0x04, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  4, 0x08, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  5, 0x10, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  6, 0x20, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  7, 0x40, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  8, 0x80, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx,  9, 0x1b, k0, k1, k2, k3);
	EXPAND_AES128_KEY_QUAD(ctx, 10, 0x36, k0, k1, k2, k3);
	return 0;
}

WEAK int aes128_vaes_setkey_enc_x8(rijndael_vaes_ctx_aes128_x8 *ctx,
	const uint8_t key1[16], const uint8_t key2[16],
	const uint8_t key3[16], const uint8_t key4[16],
	const uint8_t key5[16], const uint8_t key6[16],
	const uint8_t key7[16], const uint8_t key8[16]) {
	__m128i k0, k1, k2, k3, k4, k5, k6, k7;

	if ((ctx == NULL) || (key1 == NULL) || (key2 == NULL) ||
	    (key3 == NULL) || (key4 == NULL) || (key5 == NULL) ||
	    (key6 == NULL) || (key7 == NULL) || (key8 == NULL)) {
		return -1;
	}
	k0 = _mm_loadu_si128((const __m128i *)(const void *)key1);
	k1 = _mm_loadu_si128((const __m128i *)(const void *)key2);
	k2 = _mm_loadu_si128((const __m128i *)(const void *)key3);
	k3 = _mm_loadu_si128((const __m128i *)(const void *)key4);
	k4 = _mm_loadu_si128((const __m128i *)(const void *)key5);
	k5 = _mm_loadu_si128((const __m128i *)(const void *)key6);
	k6 = _mm_loadu_si128((const __m128i *)(const void *)key7);
	k7 = _mm_loadu_si128((const __m128i *)(const void *)key8);
	ctx->ctx[0].rtype = AES128;
	ctx->ctx[1].rtype = AES128;
	ctx->ctx[2].rtype = AES128;
	ctx->ctx[3].rtype = AES128;
	STORE_AES128_KEY_PAIR(&ctx->ctx[0], 0, k0, k1);
	STORE_AES128_KEY_PAIR(&ctx->ctx[1], 0, k2, k3);
	STORE_AES128_KEY_PAIR(&ctx->ctx[2], 0, k4, k5);
	STORE_AES128_KEY_PAIR(&ctx->ctx[3], 0, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  1, 0x01, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  2, 0x02, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  3, 0x04, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  4, 0x08, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  5, 0x10, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  6, 0x20, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  7, 0x40, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  8, 0x80, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx,  9, 0x1b, k0, k1, k2, k3, k4, k5, k6, k7);
	EXPAND_AES128_KEY_OCTET(ctx, 10, 0x36, k0, k1, k2, k3, k4, k5, k6, k7);
	return 0;
}

#endif /* RIJNDAEL_VAES_GFNI_KEYSCHED */

WEAK int aes128_vaes_enc_x2_x2(const rijndael_vaes_ctx_aes128_x2 *ctx,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16]) {
	__m256i m;
	unsigned int i;

	if ((ctx == NULL) || (ctx->rtype != AES128)) {
		return -1;
	}
	m = LOAD_AES128_PAIR(plainText1, plainText2);
	m = _mm256_xor_si256(m, LOAD_AES128_PAIR_RK(ctx, 0));
	for (i = 1; i < 10; i++) {
		m = _mm256_aesenc_epi128(m, LOAD_AES128_PAIR_RK(ctx, i));
	}
	m = _mm256_aesenclast_epi128(m, LOAD_AES128_PAIR_RK(ctx, 10));
	STORE_AES128_PAIR(cipherText1, cipherText2, m);
	return 0;
}

WEAK int aes128_vaes_enc_x4_x4(const rijndael_vaes_ctx_aes128_x4 *ctx,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	const uint8_t plainText3[16], const uint8_t plainText4[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16],
	uint8_t cipherText3[16], uint8_t cipherText4[16]) {
	__m256i m0, m1;
	unsigned int i;

	if ((ctx == NULL) || (ctx->ctx[0].rtype != AES128) ||
	    (ctx->ctx[1].rtype != AES128)) {
		return -1;
	}
	m0 = LOAD_AES128_PAIR(plainText1, plainText2);
	m1 = LOAD_AES128_PAIR(plainText3, plainText4);
	m0 = _mm256_xor_si256(m0, LOAD_AES128_PAIR_RK(&ctx->ctx[0], 0));
	m1 = _mm256_xor_si256(m1, LOAD_AES128_PAIR_RK(&ctx->ctx[1], 0));
	for (i = 1; i < 10; i++) {
		m0 = _mm256_aesenc_epi128(m0, LOAD_AES128_PAIR_RK(&ctx->ctx[0], i));
		m1 = _mm256_aesenc_epi128(m1, LOAD_AES128_PAIR_RK(&ctx->ctx[1], i));
	}
	m0 = _mm256_aesenclast_epi128(m0, LOAD_AES128_PAIR_RK(&ctx->ctx[0], 10));
	m1 = _mm256_aesenclast_epi128(m1, LOAD_AES128_PAIR_RK(&ctx->ctx[1], 10));
	STORE_AES128_PAIR(cipherText1, cipherText2, m0);
	STORE_AES128_PAIR(cipherText3, cipherText4, m1);
	return 0;
}

WEAK int aes128_vaes_enc_x8_x8(const rijndael_vaes_ctx_aes128_x8 *ctx,
	const uint8_t plainText1[16], const uint8_t plainText2[16],
	const uint8_t plainText3[16], const uint8_t plainText4[16],
	const uint8_t plainText5[16], const uint8_t plainText6[16],
	const uint8_t plainText7[16], const uint8_t plainText8[16],
	uint8_t cipherText1[16], uint8_t cipherText2[16],
	uint8_t cipherText3[16], uint8_t cipherText4[16],
	uint8_t cipherText5[16], uint8_t cipherText6[16],
	uint8_t cipherText7[16], uint8_t cipherText8[16]) {
	__m256i m0, m1, m2, m3;
	unsigned int i;

	if ((ctx == NULL) || (ctx->ctx[0].rtype != AES128) ||
	    (ctx->ctx[1].rtype != AES128) || (ctx->ctx[2].rtype != AES128) ||
	    (ctx->ctx[3].rtype != AES128)) {
		return -1;
	}
	m0 = LOAD_AES128_PAIR(plainText1, plainText2);
	m1 = LOAD_AES128_PAIR(plainText3, plainText4);
	m2 = LOAD_AES128_PAIR(plainText5, plainText6);
	m3 = LOAD_AES128_PAIR(plainText7, plainText8);
	m0 = _mm256_xor_si256(m0, LOAD_AES128_PAIR_RK(&ctx->ctx[0], 0));
	m1 = _mm256_xor_si256(m1, LOAD_AES128_PAIR_RK(&ctx->ctx[1], 0));
	m2 = _mm256_xor_si256(m2, LOAD_AES128_PAIR_RK(&ctx->ctx[2], 0));
	m3 = _mm256_xor_si256(m3, LOAD_AES128_PAIR_RK(&ctx->ctx[3], 0));
	for (i = 1; i < 10; i++) {
		m0 = _mm256_aesenc_epi128(m0, LOAD_AES128_PAIR_RK(&ctx->ctx[0], i));
		m1 = _mm256_aesenc_epi128(m1, LOAD_AES128_PAIR_RK(&ctx->ctx[1], i));
		m2 = _mm256_aesenc_epi128(m2, LOAD_AES128_PAIR_RK(&ctx->ctx[2], i));
		m3 = _mm256_aesenc_epi128(m3, LOAD_AES128_PAIR_RK(&ctx->ctx[3], i));
	}
	m0 = _mm256_aesenclast_epi128(m0, LOAD_AES128_PAIR_RK(&ctx->ctx[0], 10));
	m1 = _mm256_aesenclast_epi128(m1, LOAD_AES128_PAIR_RK(&ctx->ctx[1], 10));
	m2 = _mm256_aesenclast_epi128(m2, LOAD_AES128_PAIR_RK(&ctx->ctx[2], 10));
	m3 = _mm256_aesenclast_epi128(m3, LOAD_AES128_PAIR_RK(&ctx->ctx[3], 10));
	STORE_AES128_PAIR(cipherText1, cipherText2, m0);
	STORE_AES128_PAIR(cipherText3, cipherText4, m1);
	STORE_AES128_PAIR(cipherText5, cipherText6, m2);
	STORE_AES128_PAIR(cipherText7, cipherText8, m3);
	return 0;
}

#undef LOAD_AES128_PAIR_RK
#undef STORE_AES128_PAIR
#undef LOAD_AES128_PAIR
#undef EXPAND_AES128_KEY_OCTET
#undef EXPAND_AES128_KEY_QUAD
#undef EXPAND_AES128_KEY_PAIR
#undef STORE_AES128_KEY_PAIR

#else
MAKE_GENERIC_SETKEY_XX_IMPL(aes128, vaes, 16, 16)
MAKE_PARALLEL_FUNCS_XX_IMPL(aes128, vaes, 16)
#endif

MAKE_GENERIC_SETKEY_XX_IMPL(aes256, vaes, 32, 16)
MAKE_PARALLEL_FUNCS_XX_IMPL(aes256, vaes, 16)

#ifdef RIJNDAEL_VAES256

#define LOAD_RIJNDAEL256_PAIR_HALF(pt1, pt2, off) \
	_mm256_set_m128i( \
		_mm_loadu_si128((const __m128i *)(const void *)((pt2) + (off))), \
		_mm_loadu_si128((const __m128i *)(const void *)((pt1) + (off))))

#define STORE_RIJNDAEL256_PAIR_HALF(ct1, ct2, off, m) \
	do { \
		_mm_storeu_si128((__m128i *)(void *)((ct1) + (off)), _mm256_castsi256_si128(m)); \
		_mm_storeu_si128((__m128i *)(void *)((ct2) + (off)), _mm256_extracti128_si256((m), 1)); \
	} while (0)

WEAK int rijndael256_vaes_setkey_enc_x2(rijndael_vaes_ctx_rijndael256_x2 *ctx,
	const uint8_t key1[32], const uint8_t key2[32]) {
#ifdef RIJNDAEL_VAES_GFNI_KEYSCHED
	if ((ctx == NULL) || (key1 == NULL) || (key2 == NULL)) {
		return -1;
	}
	/* Lane-parallel schedule: no per-key scalar pass and no repack, hence no
	 * intermediate flat copy to wipe either. x4/x8 route through here. */
	ctx->rtype = RIJNDAEL_256_256;
	rijndael256_vaes_setkey_pair_gfni(ctx->rk, key1, key2);
	return 0;
#else
	rijndael_vaes_ctx_rijndael256 ctx1, ctx2;
	int ret = -1;
	unsigned int i;

	if (ctx == NULL) {
		return -1;
	}
	ret = rijndael256_vaes_setkey_enc(&ctx1, key1);
	if (ret) {
		goto err;
	}
	ret = rijndael256_vaes_setkey_enc(&ctx2, key2);
	if (ret) {
		goto err;
	}
	ctx->rtype = RIJNDAEL_256_256;
	for (i = 0; i < 30; i++) {
		_mm256_storeu_si256((__m256i *)(void *)ctx->rk[i],
			_mm256_set_m128i(ctx2.rk[i], ctx1.rk[i]));
	}
	ret = 0;
err:
	/* ctx1/ctx2 held flat (non-interleaved) copies of the round keys before
	 * being repacked into ctx->rk; wipe them. */
	rijndael_cleanse(&ctx1, sizeof(ctx1));
	rijndael_cleanse(&ctx2, sizeof(ctx2));
	return ret;
#endif /* RIJNDAEL_VAES_GFNI_KEYSCHED */
}

WEAK int rijndael256_vaes_setkey_enc_x4(rijndael_vaes_ctx_rijndael256_x4 *ctx,
	const uint8_t key1[32], const uint8_t key2[32],
	const uint8_t key3[32], const uint8_t key4[32]) {
	int ret;

	if (ctx == NULL) {
		return -1;
	}
	ret = rijndael256_vaes_setkey_enc_x2(&ctx->ctx[0], key1, key2);
	if (ret) {
		return ret;
	}
	return rijndael256_vaes_setkey_enc_x2(&ctx->ctx[1], key3, key4);
}

WEAK int rijndael256_vaes_setkey_enc_x8(rijndael_vaes_ctx_rijndael256_x8 *ctx,
	const uint8_t key1[32], const uint8_t key2[32],
	const uint8_t key3[32], const uint8_t key4[32],
	const uint8_t key5[32], const uint8_t key6[32],
	const uint8_t key7[32], const uint8_t key8[32]) {
	int ret;

	if (ctx == NULL) {
		return -1;
	}
	ret = rijndael256_vaes_setkey_enc_x2(&ctx->ctx[0], key1, key2);
	if (ret) {
		return ret;
	}
	ret = rijndael256_vaes_setkey_enc_x2(&ctx->ctx[1], key3, key4);
	if (ret) {
		return ret;
	}
	ret = rijndael256_vaes_setkey_enc_x2(&ctx->ctx[2], key5, key6);
	if (ret) {
		return ret;
	}
	return rijndael256_vaes_setkey_enc_x2(&ctx->ctx[3], key7, key8);
}

WEAK int rijndael256_vaes_enc_x2_x2(const rijndael_vaes_ctx_rijndael256_x2 *ctx,
	const uint8_t plainText1[32], const uint8_t plainText2[32],
	uint8_t cipherText1[32], uint8_t cipherText2[32]) {
	if ((ctx == NULL) || (ctx->rtype != RIJNDAEL_256_256)) {
		return -1;
	}
	__m256i ml = LOAD_RIJNDAEL256_PAIR_HALF(plainText1, plainText2, 0);
	__m256i mr = LOAD_RIJNDAEL256_PAIR_HALF(plainText1, plainText2, 16);
	DO_ENC_BLOCK_VAES256_PACKED_256_256(ml, mr, ctx->rk);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText1, cipherText2, 0, ml);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText1, cipherText2, 16, mr);
	return 0;
}

WEAK int rijndael256_vaes_enc_x4_x4(const rijndael_vaes_ctx_rijndael256_x4 *ctx,
	const uint8_t plainText1[32], const uint8_t plainText2[32],
	const uint8_t plainText3[32], const uint8_t plainText4[32],
	uint8_t cipherText1[32], uint8_t cipherText2[32],
	uint8_t cipherText3[32], uint8_t cipherText4[32]) {
	if ((ctx == NULL) ||
	    (ctx->ctx[0].rtype != RIJNDAEL_256_256) ||
	    (ctx->ctx[1].rtype != RIJNDAEL_256_256)) {
		return -1;
	}
	__m256i ml1 = LOAD_RIJNDAEL256_PAIR_HALF(plainText1, plainText2, 0);
	__m256i mr1 = LOAD_RIJNDAEL256_PAIR_HALF(plainText1, plainText2, 16);
	__m256i ml2 = LOAD_RIJNDAEL256_PAIR_HALF(plainText3, plainText4, 0);
	__m256i mr2 = LOAD_RIJNDAEL256_PAIR_HALF(plainText3, plainText4, 16);
	DO_ENC_BLOCK_VAES256_PACKED_256_256_x2(ml1, mr1, ml2, mr2,
		ctx->ctx[0].rk, ctx->ctx[1].rk);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText1, cipherText2, 0, ml1);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText1, cipherText2, 16, mr1);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText3, cipherText4, 0, ml2);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText3, cipherText4, 16, mr2);
	return 0;
}

WEAK int rijndael256_vaes_enc_x8_x8(const rijndael_vaes_ctx_rijndael256_x8 *ctx,
	const uint8_t plainText1[32], const uint8_t plainText2[32],
	const uint8_t plainText3[32], const uint8_t plainText4[32],
	const uint8_t plainText5[32], const uint8_t plainText6[32],
	const uint8_t plainText7[32], const uint8_t plainText8[32],
	uint8_t cipherText1[32], uint8_t cipherText2[32],
	uint8_t cipherText3[32], uint8_t cipherText4[32],
	uint8_t cipherText5[32], uint8_t cipherText6[32],
	uint8_t cipherText7[32], uint8_t cipherText8[32]) {
	if ((ctx == NULL) ||
	    (ctx->ctx[0].rtype != RIJNDAEL_256_256) ||
	    (ctx->ctx[1].rtype != RIJNDAEL_256_256) ||
	    (ctx->ctx[2].rtype != RIJNDAEL_256_256) ||
	    (ctx->ctx[3].rtype != RIJNDAEL_256_256)) {
		return -1;
	}
	__m256i ml1 = LOAD_RIJNDAEL256_PAIR_HALF(plainText1, plainText2, 0);
	__m256i mr1 = LOAD_RIJNDAEL256_PAIR_HALF(plainText1, plainText2, 16);
	__m256i ml2 = LOAD_RIJNDAEL256_PAIR_HALF(plainText3, plainText4, 0);
	__m256i mr2 = LOAD_RIJNDAEL256_PAIR_HALF(plainText3, plainText4, 16);
	__m256i ml3 = LOAD_RIJNDAEL256_PAIR_HALF(plainText5, plainText6, 0);
	__m256i mr3 = LOAD_RIJNDAEL256_PAIR_HALF(plainText5, plainText6, 16);
	__m256i ml4 = LOAD_RIJNDAEL256_PAIR_HALF(plainText7, plainText8, 0);
	__m256i mr4 = LOAD_RIJNDAEL256_PAIR_HALF(plainText7, plainText8, 16);
	DO_ENC_BLOCK_VAES256_PACKED_256_256_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4,
		ctx->ctx[0].rk, ctx->ctx[1].rk, ctx->ctx[2].rk, ctx->ctx[3].rk);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText1, cipherText2, 0, ml1);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText1, cipherText2, 16, mr1);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText3, cipherText4, 0, ml2);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText3, cipherText4, 16, mr2);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText5, cipherText6, 0, ml3);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText5, cipherText6, 16, mr3);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText7, cipherText8, 0, ml4);
	STORE_RIJNDAEL256_PAIR_HALF(cipherText7, cipherText8, 16, mr4);
	return 0;
}

#else
MAKE_GENERIC_SETKEY_XX_IMPL(rijndael256, vaes, 32, 32)
MAKE_PARALLEL_FUNCS_XX_IMPL(rijndael256, vaes, 32)
#endif

/* ======== ECB key schedule ======== */

int aes128_vaes_setkey_enc_ecb(rijndael_vaes_ctx_aes128_ecb *ctx, const uint8_t key[16]) {
	return aes128_vaes_setkey_enc(ctx, key);
}
int aes256_vaes_setkey_enc_ecb(rijndael_vaes_ctx_aes256_ecb *ctx, const uint8_t key[32]) {
	return aes256_vaes_setkey_enc(ctx, key);
}
int rijndael256_vaes_setkey_enc_ecb(rijndael_vaes_ctx_rijndael256_ecb *ctx, const uint8_t key[32]) {
	return rijndael256_vaes_setkey_enc(ctx, key);
}

/* ======== ECB encryption: VAES-512 -> VAES-256 -> scalar tail ======== */

#ifdef RIJNDAEL_VAES256
/* Same-key ECB tail on nv = 1, 2 or 3 ymm registers (2, 4 or 6 blocks).
 */
static inline void aes128_vaes_ecb_samekey_tail(const __m128i *rk, unsigned int nv,
        const uint8_t *in, uint8_t *out) {
	__m256i m[3];
	__m256i kr;
	unsigned int j, r;

	for (j = 0; j < nv; j++) {
		m[j] = _mm256_loadu_si256((const __m256i *)(const void *)&in[j * 32]);
	}
	kr = _mm256_broadcastsi128_si256(rk[0]);
	for (j = 0; j < nv; j++) {
		m[j] = _mm256_xor_si256(m[j], kr);
	}
	for (r = 1; r < 10; r++) {
		kr = _mm256_broadcastsi128_si256(rk[r]);
		for (j = 0; j < nv; j++) {
			m[j] = _mm256_aesenc_epi128(m[j], kr);
		}
	}
	kr = _mm256_broadcastsi128_si256(rk[10]);
	for (j = 0; j < nv; j++) {
		m[j] = _mm256_aesenclast_epi128(m[j], kr);
	}
	for (j = 0; j < nv; j++) {
		_mm256_storeu_si256((__m256i *)(void *)&out[j * 32], m[j]);
	}
}
#endif /* RIJNDAEL_VAES256 */

/* AES-128 ECB: VAES-512 x4-zmm (16 blocks) -> VAES-256 x4-ymm (8 blocks) -> single-block tail */
int aes128_vaes_enc_ecb(const rijndael_vaes_ctx_aes128_ecb *ctx, uint32_t nblocks, const uint8_t *in, uint8_t *out) {
	int ret = -1;
	uint32_t i = 0;
	if ((ctx == NULL) || (ctx->rtype != AES128)) { goto err; }
#ifdef RIJNDAEL_VAES512
	for (; i + 16 <= nblocks; i += 16) {
		__m512i m0, m1, m2, m3;
		m0 = _mm512_loadu_si512((const __m512i *)&in[(i+ 0)*16]);
		m1 = _mm512_loadu_si512((const __m512i *)&in[(i+ 4)*16]);
		m2 = _mm512_loadu_si512((const __m512i *)&in[(i+ 8)*16]);
		m3 = _mm512_loadu_si512((const __m512i *)&in[(i+12)*16]);
		DO_ENC_BLOCK_128_128_VAES512_ECB_x4(m0, m1, m2, m3, ctx->rk);
		_mm512_storeu_si512((__m512i *)&out[(i+ 0)*16], m0);
		_mm512_storeu_si512((__m512i *)&out[(i+ 4)*16], m1);
		_mm512_storeu_si512((__m512i *)&out[(i+ 8)*16], m2);
		_mm512_storeu_si512((__m512i *)&out[(i+12)*16], m3);
	}
#endif
#ifdef RIJNDAEL_VAES256
	for (; i + 8 <= nblocks; i += 8) {
		__m256i m0, m1, m2, m3;
		m0 = _mm256_loadu_si256((const __m256i *)&in[(i+0)*16]);
		m1 = _mm256_loadu_si256((const __m256i *)&in[(i+2)*16]);
		m2 = _mm256_loadu_si256((const __m256i *)&in[(i+4)*16]);
		m3 = _mm256_loadu_si256((const __m256i *)&in[(i+6)*16]);
		DO_ENC_BLOCK_128_128_VAES256_ECB_x4(m0, m1, m2, m3, ctx->rk);
		_mm256_storeu_si256((__m256i *)&out[(i+0)*16], m0);
		_mm256_storeu_si256((__m256i *)&out[(i+2)*16], m1);
		_mm256_storeu_si256((__m256i *)&out[(i+4)*16], m2);
		_mm256_storeu_si256((__m256i *)&out[(i+6)*16], m3);
	}
#endif
#ifdef RIJNDAEL_VAES256
	/* Remainder (0-7 blocks): same-key ymm tail, no per-round key re-packing.
	 * Even block counts are covered entirely here; an odd leftover block falls
	 * through to the single-block loop below. */
	if ((nblocks - i) >= 6) {
		aes128_vaes_ecb_samekey_tail(ctx->rk, 3, &in[i*16], &out[i*16]);
		i += 6;
	} else if ((nblocks - i) >= 4) {
		aes128_vaes_ecb_samekey_tail(ctx->rk, 2, &in[i*16], &out[i*16]);
		i += 4;
	} else if ((nblocks - i) >= 2) {
		aes128_vaes_ecb_samekey_tail(ctx->rk, 1, &in[i*16], &out[i*16]);
		i += 2;
	}
#endif
	/* Unbounded (not a single "if"): when RIJNDAEL_VAES256 is not defined, the
	 * x8-block loop above is compiled out and i is still 0 here, so this must
	 * be able to walk the whole remaining range, not just a single block. */
	for (; i < nblocks; i++) {
		ret = aes128_vaes_enc(ctx, &in[i*16], &out[i*16]);
		if (ret) { goto err; }
	}
	ret = 0;
err:
	return ret;
}

/* AES-256 ECB: VAES-512 x4-zmm (16 blocks) -> VAES-256 x4-ymm (8 blocks) -> single-block tail */
int aes256_vaes_enc_ecb(const rijndael_vaes_ctx_aes256_ecb *ctx, uint32_t nblocks, const uint8_t *in, uint8_t *out) {
	int ret = -1;
	uint32_t i = 0;
	if ((ctx == NULL) || (ctx->rtype != AES256)) { goto err; }
#ifdef RIJNDAEL_VAES512
	for (; i + 16 <= nblocks; i += 16) {
		__m512i m0, m1, m2, m3;
		m0 = _mm512_loadu_si512((const __m512i *)&in[(i+ 0)*16]);
		m1 = _mm512_loadu_si512((const __m512i *)&in[(i+ 4)*16]);
		m2 = _mm512_loadu_si512((const __m512i *)&in[(i+ 8)*16]);
		m3 = _mm512_loadu_si512((const __m512i *)&in[(i+12)*16]);
		DO_ENC_BLOCK_128_256_VAES512_ECB_x4(m0, m1, m2, m3, ctx->rk);
		_mm512_storeu_si512((__m512i *)&out[(i+ 0)*16], m0);
		_mm512_storeu_si512((__m512i *)&out[(i+ 4)*16], m1);
		_mm512_storeu_si512((__m512i *)&out[(i+ 8)*16], m2);
		_mm512_storeu_si512((__m512i *)&out[(i+12)*16], m3);
	}
#endif
#ifdef RIJNDAEL_VAES256
	for (; i + 8 <= nblocks; i += 8) {
		__m256i m0, m1, m2, m3;
		m0 = _mm256_loadu_si256((const __m256i *)&in[(i+0)*16]);
		m1 = _mm256_loadu_si256((const __m256i *)&in[(i+2)*16]);
		m2 = _mm256_loadu_si256((const __m256i *)&in[(i+4)*16]);
		m3 = _mm256_loadu_si256((const __m256i *)&in[(i+6)*16]);
		DO_ENC_BLOCK_128_256_VAES256_ECB_x4(m0, m1, m2, m3, ctx->rk);
		_mm256_storeu_si256((__m256i *)&out[(i+0)*16], m0);
		_mm256_storeu_si256((__m256i *)&out[(i+2)*16], m1);
		_mm256_storeu_si256((__m256i *)&out[(i+4)*16], m2);
		_mm256_storeu_si256((__m256i *)&out[(i+6)*16], m3);
	}
#endif
	for (; i < nblocks; i++) {
		ret = aes256_vaes_enc(ctx, &in[i*16], &out[i*16]);
		if (ret) { goto err; }
	}
	ret = 0;
err:
	return ret;
}

/* Rijndael-256 ECB: VAES-512 x2-zmm (4 blocks) -> VAES-256 x4-ymm (4 blocks) -> single-block tail */
int rijndael256_vaes_enc_ecb(const rijndael_vaes_ctx_rijndael256_ecb *ctx, uint32_t nblocks, const uint8_t *in, uint8_t *out) {
	int ret = -1;
	uint32_t i = 0;
	if ((ctx == NULL) || (ctx->rtype != RIJNDAEL_256_256)) { goto err; }
#ifdef RIJNDAEL_VAES512
	for (; i + 4 <= nblocks; i += 4) {
		__m128i l0, l1, l2, l3, r0, r1, r2, r3;
		l0 = _mm_loadu_si128((const __m128i *)&in[(i+0)*32]);
		r0 = _mm_loadu_si128((const __m128i *)&in[(i+0)*32 + 16]);
		l1 = _mm_loadu_si128((const __m128i *)&in[(i+1)*32]);
		r1 = _mm_loadu_si128((const __m128i *)&in[(i+1)*32 + 16]);
		l2 = _mm_loadu_si128((const __m128i *)&in[(i+2)*32]);
		r2 = _mm_loadu_si128((const __m128i *)&in[(i+2)*32 + 16]);
		l3 = _mm_loadu_si128((const __m128i *)&in[(i+3)*32]);
		r3 = _mm_loadu_si128((const __m128i *)&in[(i+3)*32 + 16]);
		__m512i zml = _mm512_inserti64x4(_mm512_castsi256_si512(_mm256_set_m128i(l1, l0)), _mm256_set_m128i(l3, l2), 1);
		__m512i zmr = _mm512_inserti64x4(_mm512_castsi256_si512(_mm256_set_m128i(r1, r0)), _mm256_set_m128i(r3, r2), 1);
		DO_ENC_BLOCK_256_256_VAES512_ECB_x4(zml, zmr, ctx->rk);
		_mm_storeu_si128((__m128i *)&out[(i+0)*32],      _mm512_extracti32x4_epi32(zml, 0));
		_mm_storeu_si128((__m128i *)&out[(i+0)*32 + 16], _mm512_extracti32x4_epi32(zmr, 0));
		_mm_storeu_si128((__m128i *)&out[(i+1)*32],      _mm512_extracti32x4_epi32(zml, 1));
		_mm_storeu_si128((__m128i *)&out[(i+1)*32 + 16], _mm512_extracti32x4_epi32(zmr, 1));
		_mm_storeu_si128((__m128i *)&out[(i+2)*32],      _mm512_extracti32x4_epi32(zml, 2));
		_mm_storeu_si128((__m128i *)&out[(i+2)*32 + 16], _mm512_extracti32x4_epi32(zmr, 2));
		_mm_storeu_si128((__m128i *)&out[(i+3)*32],      _mm512_extracti32x4_epi32(zml, 3));
		_mm_storeu_si128((__m128i *)&out[(i+3)*32 + 16], _mm512_extracti32x4_epi32(zmr, 3));
	}
#endif
#ifdef RIJNDAEL_VAES256
	for (; i + 8 <= nblocks; i += 8) {
		__m128i l0, l1, l2, l3, l4, l5, l6, l7;
		__m128i r0, r1, r2, r3, r4, r5, r6, r7;
		l0 = _mm_loadu_si128((const __m128i *)&in[(i+0)*32]);     r0 = _mm_loadu_si128((const __m128i *)&in[(i+0)*32 + 16]);
		l1 = _mm_loadu_si128((const __m128i *)&in[(i+1)*32]);     r1 = _mm_loadu_si128((const __m128i *)&in[(i+1)*32 + 16]);
		l2 = _mm_loadu_si128((const __m128i *)&in[(i+2)*32]);     r2 = _mm_loadu_si128((const __m128i *)&in[(i+2)*32 + 16]);
		l3 = _mm_loadu_si128((const __m128i *)&in[(i+3)*32]);     r3 = _mm_loadu_si128((const __m128i *)&in[(i+3)*32 + 16]);
		l4 = _mm_loadu_si128((const __m128i *)&in[(i+4)*32]);     r4 = _mm_loadu_si128((const __m128i *)&in[(i+4)*32 + 16]);
		l5 = _mm_loadu_si128((const __m128i *)&in[(i+5)*32]);     r5 = _mm_loadu_si128((const __m128i *)&in[(i+5)*32 + 16]);
		l6 = _mm_loadu_si128((const __m128i *)&in[(i+6)*32]);     r6 = _mm_loadu_si128((const __m128i *)&in[(i+6)*32 + 16]);
		l7 = _mm_loadu_si128((const __m128i *)&in[(i+7)*32]);     r7 = _mm_loadu_si128((const __m128i *)&in[(i+7)*32 + 16]);
		__m256i yml  = _mm256_set_m128i(l1, l0); __m256i ymr  = _mm256_set_m128i(r1, r0);
		__m256i yml2 = _mm256_set_m128i(l3, l2); __m256i ymr2 = _mm256_set_m128i(r3, r2);
		__m256i yml3 = _mm256_set_m128i(l5, l4); __m256i ymr3 = _mm256_set_m128i(r5, r4);
		__m256i yml4 = _mm256_set_m128i(l7, l6); __m256i ymr4 = _mm256_set_m128i(r7, r6);
		DO_ENC_BLOCK_256_256_VAES256_ECB_x8(yml, ymr, yml2, ymr2, yml3, ymr3, yml4, ymr4, ctx->rk);
		_mm_storeu_si128((__m128i *)&out[(i+0)*32],      _mm256_castsi256_si128(yml));
		_mm_storeu_si128((__m128i *)&out[(i+0)*32 + 16], _mm256_castsi256_si128(ymr));
		_mm_storeu_si128((__m128i *)&out[(i+1)*32],      _mm256_extracti128_si256(yml, 1));
		_mm_storeu_si128((__m128i *)&out[(i+1)*32 + 16], _mm256_extracti128_si256(ymr, 1));
		_mm_storeu_si128((__m128i *)&out[(i+2)*32],      _mm256_castsi256_si128(yml2));
		_mm_storeu_si128((__m128i *)&out[(i+2)*32 + 16], _mm256_castsi256_si128(ymr2));
		_mm_storeu_si128((__m128i *)&out[(i+3)*32],      _mm256_extracti128_si256(yml2, 1));
		_mm_storeu_si128((__m128i *)&out[(i+3)*32 + 16], _mm256_extracti128_si256(ymr2, 1));
		_mm_storeu_si128((__m128i *)&out[(i+4)*32],      _mm256_castsi256_si128(yml3));
		_mm_storeu_si128((__m128i *)&out[(i+4)*32 + 16], _mm256_castsi256_si128(ymr3));
		_mm_storeu_si128((__m128i *)&out[(i+5)*32],      _mm256_extracti128_si256(yml3, 1));
		_mm_storeu_si128((__m128i *)&out[(i+5)*32 + 16], _mm256_extracti128_si256(ymr3, 1));
		_mm_storeu_si128((__m128i *)&out[(i+6)*32],      _mm256_castsi256_si128(yml4));
		_mm_storeu_si128((__m128i *)&out[(i+6)*32 + 16], _mm256_castsi256_si128(ymr4));
		_mm_storeu_si128((__m128i *)&out[(i+7)*32],      _mm256_extracti128_si256(yml4, 1));
		_mm_storeu_si128((__m128i *)&out[(i+7)*32 + 16], _mm256_extracti128_si256(ymr4, 1));
	}
	for (; i + 4 <= nblocks; i += 4) {
		__m128i l0, l1, l2, l3, r0, r1, r2, r3;
		l0 = _mm_loadu_si128((const __m128i *)&in[(i+0)*32]);
		r0 = _mm_loadu_si128((const __m128i *)&in[(i+0)*32 + 16]);
		l1 = _mm_loadu_si128((const __m128i *)&in[(i+1)*32]);
		r1 = _mm_loadu_si128((const __m128i *)&in[(i+1)*32 + 16]);
		l2 = _mm_loadu_si128((const __m128i *)&in[(i+2)*32]);
		r2 = _mm_loadu_si128((const __m128i *)&in[(i+2)*32 + 16]);
		l3 = _mm_loadu_si128((const __m128i *)&in[(i+3)*32]);
		r3 = _mm_loadu_si128((const __m128i *)&in[(i+3)*32 + 16]);
		__m256i yml  = _mm256_set_m128i(l1, l0);
		__m256i ymr  = _mm256_set_m128i(r1, r0);
		__m256i yml2 = _mm256_set_m128i(l3, l2);
		__m256i ymr2 = _mm256_set_m128i(r3, r2);
		DO_ENC_BLOCK_256_256_VAES256_ECB_x4(yml, ymr, yml2, ymr2, ctx->rk);
		_mm_storeu_si128((__m128i *)&out[(i+0)*32],      _mm256_castsi256_si128(yml));
		_mm_storeu_si128((__m128i *)&out[(i+0)*32 + 16], _mm256_castsi256_si128(ymr));
		_mm_storeu_si128((__m128i *)&out[(i+1)*32],      _mm256_extracti128_si256(yml, 1));
		_mm_storeu_si128((__m128i *)&out[(i+1)*32 + 16], _mm256_extracti128_si256(ymr, 1));
		_mm_storeu_si128((__m128i *)&out[(i+2)*32],      _mm256_castsi256_si128(yml2));
		_mm_storeu_si128((__m128i *)&out[(i+2)*32 + 16], _mm256_castsi256_si128(ymr2));
		_mm_storeu_si128((__m128i *)&out[(i+3)*32],      _mm256_extracti128_si256(yml2, 1));
		_mm_storeu_si128((__m128i *)&out[(i+3)*32 + 16], _mm256_extracti128_si256(ymr2, 1));
	}
#endif
	for (; i < nblocks; i++) {
		ret = rijndael256_vaes_enc(ctx, &in[i*32], &out[i*32]);
		if (ret) { goto err; }
	}
	ret = 0;
err:
	return ret;
}


#else /* !RIJNDAEL_VAES */
/*
 * Dummy definition to avoid the empty translation unit ISO C warning
 */
typedef int dummy;
#endif /* RIJNDAEL_VAES */
