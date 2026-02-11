// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include <oqs/oqs.h>

OQS_API const char *OQS_SIG_alg_identifier(size_t i) {
	// EDIT-WHEN-ADDING-SIG
	const char *a[OQS_SIG_algs_length] = {
		///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ALG_IDENTIFIER_START
		OQS_SIG_alg_ml_dsa_44,
		OQS_SIG_alg_ml_dsa_65,
		OQS_SIG_alg_ml_dsa_87,
		OQS_SIG_alg_falcon_512,
		OQS_SIG_alg_falcon_1024,
		OQS_SIG_alg_falcon_padded_512,
		OQS_SIG_alg_falcon_padded_1024,
		OQS_SIG_alg_mayo_1,
		OQS_SIG_alg_mayo_2,
		OQS_SIG_alg_mayo_3,
		OQS_SIG_alg_mayo_5,
		OQS_SIG_alg_cross_rsdp_128_balanced,
		OQS_SIG_alg_cross_rsdp_128_fast,
		OQS_SIG_alg_cross_rsdp_128_small,
		OQS_SIG_alg_cross_rsdp_192_balanced,
		OQS_SIG_alg_cross_rsdp_192_fast,
		OQS_SIG_alg_cross_rsdp_192_small,
		OQS_SIG_alg_cross_rsdp_256_balanced,
		OQS_SIG_alg_cross_rsdp_256_fast,
		OQS_SIG_alg_cross_rsdp_256_small,
		OQS_SIG_alg_cross_rsdpg_128_balanced,
		OQS_SIG_alg_cross_rsdpg_128_fast,
		OQS_SIG_alg_cross_rsdpg_128_small,
		OQS_SIG_alg_cross_rsdpg_192_balanced,
		OQS_SIG_alg_cross_rsdpg_192_fast,
		OQS_SIG_alg_cross_rsdpg_192_small,
		OQS_SIG_alg_cross_rsdpg_256_balanced,
		OQS_SIG_alg_cross_rsdpg_256_fast,
		OQS_SIG_alg_cross_rsdpg_256_small,
		OQS_SIG_alg_uov_ov_Is,
		OQS_SIG_alg_uov_ov_Ip,
		OQS_SIG_alg_uov_ov_III,
		OQS_SIG_alg_uov_ov_V,
		OQS_SIG_alg_uov_ov_Is_pkc,
		OQS_SIG_alg_uov_ov_Ip_pkc,
		OQS_SIG_alg_uov_ov_III_pkc,
		OQS_SIG_alg_uov_ov_V_pkc,
		OQS_SIG_alg_uov_ov_Is_pkc_skc,
		OQS_SIG_alg_uov_ov_Ip_pkc_skc,
		OQS_SIG_alg_uov_ov_III_pkc_skc,
		OQS_SIG_alg_uov_ov_V_pkc_skc,
		OQS_SIG_alg_snova_SNOVA_24_5_4,
		OQS_SIG_alg_snova_SNOVA_24_5_4_SHAKE,
		OQS_SIG_alg_snova_SNOVA_24_5_4_esk,
		OQS_SIG_alg_snova_SNOVA_24_5_4_SHAKE_esk,
		OQS_SIG_alg_snova_SNOVA_37_17_2,
		OQS_SIG_alg_snova_SNOVA_25_8_3,
		OQS_SIG_alg_snova_SNOVA_56_25_2,
		OQS_SIG_alg_snova_SNOVA_49_11_3,
		OQS_SIG_alg_snova_SNOVA_37_8_4,
		OQS_SIG_alg_snova_SNOVA_24_5_5,
		OQS_SIG_alg_snova_SNOVA_60_10_4,
		OQS_SIG_alg_snova_SNOVA_29_6_5,///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ALG_IDENTIFIER_END
		///// OQS_COPY_FROM_SLH_DSA_FRAGMENT_ALGID_START
		OQS_SIG_alg_slh_dsa_pure_sha2_128s,
		OQS_SIG_alg_slh_dsa_pure_sha2_128f,
		OQS_SIG_alg_slh_dsa_pure_sha2_192s,
		OQS_SIG_alg_slh_dsa_pure_sha2_192f,
		OQS_SIG_alg_slh_dsa_pure_sha2_256s,
		OQS_SIG_alg_slh_dsa_pure_sha2_256f,
		OQS_SIG_alg_slh_dsa_pure_shake_128s,
		OQS_SIG_alg_slh_dsa_pure_shake_128f,
		OQS_SIG_alg_slh_dsa_pure_shake_192s,
		OQS_SIG_alg_slh_dsa_pure_shake_192f,
		OQS_SIG_alg_slh_dsa_pure_shake_256s,
		OQS_SIG_alg_slh_dsa_pure_shake_256f,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_256f,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_128s,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_128f,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_192s,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_192f,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_256s,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_256f,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_128s,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_128f,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_192s,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_192f,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_256s,
		OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_256f,
		///// OQS_COPY_FROM_SLH_DSA_FRAGMENT_ALGID_END
	};
	if (i >= OQS_SIG_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}

OQS_API int OQS_SIG_alg_count(void) {
	return OQS_SIG_algs_length;
}

OQS_API int OQS_SIG_alg_is_enabled(const char *method_name) {
	if (method_name == NULL) {
		return 0;
	}
	if (0) {
		///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ENABLED_CASE_START
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_ml_dsa_44)) {
#ifdef OQS_ENABLE_SIG_ml_dsa_44
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_ml_dsa_65)) {
#ifdef OQS_ENABLE_SIG_ml_dsa_65
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_ml_dsa_87)) {
#ifdef OQS_ENABLE_SIG_ml_dsa_87
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_512)) {
#ifdef OQS_ENABLE_SIG_falcon_512
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_1024)) {
#ifdef OQS_ENABLE_SIG_falcon_1024
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_padded_512)) {
#ifdef OQS_ENABLE_SIG_falcon_padded_512
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_padded_1024)) {
#ifdef OQS_ENABLE_SIG_falcon_padded_1024
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_1)) {
#ifdef OQS_ENABLE_SIG_mayo_1
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_2)) {
#ifdef OQS_ENABLE_SIG_mayo_2
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_3)) {
#ifdef OQS_ENABLE_SIG_mayo_3
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_5)) {
#ifdef OQS_ENABLE_SIG_mayo_5
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_128_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_128_balanced
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_128_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_128_fast
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_128_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_128_small
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_192_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_192_balanced
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_192_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_192_fast
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_192_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_192_small
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_256_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_256_balanced
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_256_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_256_fast
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_256_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_256_small
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_128_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_128_balanced
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_128_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_128_fast
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_128_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_128_small
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_192_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_192_balanced
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_192_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_192_fast
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_192_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_192_small
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_256_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_256_balanced
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_256_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_256_fast
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_256_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_256_small
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Is)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Is
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Ip)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Ip
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_III)) {
#ifdef OQS_ENABLE_SIG_uov_ov_III
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_V)) {
#ifdef OQS_ENABLE_SIG_uov_ov_V
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Is_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Is_pkc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Ip_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Ip_pkc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_III_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_III_pkc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_V_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_V_pkc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Is_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Is_pkc_skc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Ip_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Ip_pkc_skc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_III_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_III_pkc_skc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_V_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_V_pkc_skc
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4_SHAKE)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4_SHAKE
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4_esk)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4_esk
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4_SHAKE_esk)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4_SHAKE_esk
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_37_17_2)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_37_17_2
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_25_8_3)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_25_8_3
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_56_25_2)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_56_25_2
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_49_11_3)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_49_11_3
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_37_8_4)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_37_8_4
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_5)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_5
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_60_10_4)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_60_10_4
		return 1;
#else
		return 0;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_29_6_5)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_29_6_5
		return 1;
#else
		return 0;
#endif
///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ENABLED_CASE_END
///// OQS_COPY_FROM_SLH_DSA_FRAGMENT_ENABLED_START
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_256f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_128s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_128f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_192s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_192f
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_256s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_256f
		return 1;
#else
		return 0;
#endif
		///// OQS_COPY_FROM_SLH_DSA_FRAGMENT_ENABLED_END
	} else {
		return 0;
	}
}

OQS_API OQS_SIG *OQS_SIG_new(const char *method_name) {
	if (method_name == NULL) {
		return NULL;
	}
	if (0) {
		///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_NEW_CASE_START
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_ml_dsa_44)) {
#ifdef OQS_ENABLE_SIG_ml_dsa_44
		return OQS_SIG_ml_dsa_44_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_ml_dsa_65)) {
#ifdef OQS_ENABLE_SIG_ml_dsa_65
		return OQS_SIG_ml_dsa_65_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_ml_dsa_87)) {
#ifdef OQS_ENABLE_SIG_ml_dsa_87
		return OQS_SIG_ml_dsa_87_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_512)) {
#ifdef OQS_ENABLE_SIG_falcon_512
		return OQS_SIG_falcon_512_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_1024)) {
#ifdef OQS_ENABLE_SIG_falcon_1024
		return OQS_SIG_falcon_1024_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_padded_512)) {
#ifdef OQS_ENABLE_SIG_falcon_padded_512
		return OQS_SIG_falcon_padded_512_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_falcon_padded_1024)) {
#ifdef OQS_ENABLE_SIG_falcon_padded_1024
		return OQS_SIG_falcon_padded_1024_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_1)) {
#ifdef OQS_ENABLE_SIG_mayo_1
		return OQS_SIG_mayo_1_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_2)) {
#ifdef OQS_ENABLE_SIG_mayo_2
		return OQS_SIG_mayo_2_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_3)) {
#ifdef OQS_ENABLE_SIG_mayo_3
		return OQS_SIG_mayo_3_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mayo_5)) {
#ifdef OQS_ENABLE_SIG_mayo_5
		return OQS_SIG_mayo_5_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_128_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_128_balanced
		return OQS_SIG_cross_rsdp_128_balanced_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_128_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_128_fast
		return OQS_SIG_cross_rsdp_128_fast_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_128_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_128_small
		return OQS_SIG_cross_rsdp_128_small_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_192_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_192_balanced
		return OQS_SIG_cross_rsdp_192_balanced_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_192_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_192_fast
		return OQS_SIG_cross_rsdp_192_fast_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_192_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_192_small
		return OQS_SIG_cross_rsdp_192_small_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_256_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_256_balanced
		return OQS_SIG_cross_rsdp_256_balanced_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_256_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_256_fast
		return OQS_SIG_cross_rsdp_256_fast_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdp_256_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdp_256_small
		return OQS_SIG_cross_rsdp_256_small_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_128_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_128_balanced
		return OQS_SIG_cross_rsdpg_128_balanced_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_128_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_128_fast
		return OQS_SIG_cross_rsdpg_128_fast_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_128_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_128_small
		return OQS_SIG_cross_rsdpg_128_small_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_192_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_192_balanced
		return OQS_SIG_cross_rsdpg_192_balanced_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_192_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_192_fast
		return OQS_SIG_cross_rsdpg_192_fast_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_192_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_192_small
		return OQS_SIG_cross_rsdpg_192_small_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_256_balanced)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_256_balanced
		return OQS_SIG_cross_rsdpg_256_balanced_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_256_fast)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_256_fast
		return OQS_SIG_cross_rsdpg_256_fast_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_cross_rsdpg_256_small)) {
#ifdef OQS_ENABLE_SIG_cross_rsdpg_256_small
		return OQS_SIG_cross_rsdpg_256_small_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Is)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Is
		return OQS_SIG_uov_ov_Is_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Ip)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Ip
		return OQS_SIG_uov_ov_Ip_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_III)) {
#ifdef OQS_ENABLE_SIG_uov_ov_III
		return OQS_SIG_uov_ov_III_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_V)) {
#ifdef OQS_ENABLE_SIG_uov_ov_V
		return OQS_SIG_uov_ov_V_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Is_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Is_pkc
		return OQS_SIG_uov_ov_Is_pkc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Ip_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Ip_pkc
		return OQS_SIG_uov_ov_Ip_pkc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_III_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_III_pkc
		return OQS_SIG_uov_ov_III_pkc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_V_pkc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_V_pkc
		return OQS_SIG_uov_ov_V_pkc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Is_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Is_pkc_skc
		return OQS_SIG_uov_ov_Is_pkc_skc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_Ip_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_Ip_pkc_skc
		return OQS_SIG_uov_ov_Ip_pkc_skc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_III_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_III_pkc_skc
		return OQS_SIG_uov_ov_III_pkc_skc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_uov_ov_V_pkc_skc)) {
#ifdef OQS_ENABLE_SIG_uov_ov_V_pkc_skc
		return OQS_SIG_uov_ov_V_pkc_skc_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4
		return OQS_SIG_snova_SNOVA_24_5_4_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4_SHAKE)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4_SHAKE
		return OQS_SIG_snova_SNOVA_24_5_4_SHAKE_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4_esk)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4_esk
		return OQS_SIG_snova_SNOVA_24_5_4_esk_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_4_SHAKE_esk)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_4_SHAKE_esk
		return OQS_SIG_snova_SNOVA_24_5_4_SHAKE_esk_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_37_17_2)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_37_17_2
		return OQS_SIG_snova_SNOVA_37_17_2_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_25_8_3)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_25_8_3
		return OQS_SIG_snova_SNOVA_25_8_3_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_56_25_2)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_56_25_2
		return OQS_SIG_snova_SNOVA_56_25_2_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_49_11_3)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_49_11_3
		return OQS_SIG_snova_SNOVA_49_11_3_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_37_8_4)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_37_8_4
		return OQS_SIG_snova_SNOVA_37_8_4_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_24_5_5)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_24_5_5
		return OQS_SIG_snova_SNOVA_24_5_5_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_60_10_4)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_60_10_4
		return OQS_SIG_snova_SNOVA_60_10_4_new();
#else
		return NULL;
#endif

	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_snova_SNOVA_29_6_5)) {
#ifdef OQS_ENABLE_SIG_snova_SNOVA_29_6_5
		return OQS_SIG_snova_SNOVA_29_6_5_new();
#else
		return NULL;
#endif
///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_NEW_CASE_END
///// OQS_COPY_FROM_SLH_DSA_FRAGMENT_SIGNEW_START
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_128s
		return OQS_SIG_slh_dsa_pure_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_128f
		return OQS_SIG_slh_dsa_pure_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_192s
		return OQS_SIG_slh_dsa_pure_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_192f
		return OQS_SIG_slh_dsa_pure_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_256s
		return OQS_SIG_slh_dsa_pure_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_sha2_256f
		return OQS_SIG_slh_dsa_pure_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_128s
		return OQS_SIG_slh_dsa_pure_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_128f
		return OQS_SIG_slh_dsa_pure_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_192s
		return OQS_SIG_slh_dsa_pure_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_192f
		return OQS_SIG_slh_dsa_pure_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_256s
		return OQS_SIG_slh_dsa_pure_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_pure_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_pure_shake_256f
		return OQS_SIG_slh_dsa_pure_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha2_224_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha2_224_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha2_224_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha2_224_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha2_224_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha2_224_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha2_224_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha2_224_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_224_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_224_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha2_224_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha2_256_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha2_256_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha2_256_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha2_256_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha2_256_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha2_256_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha2_256_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha2_256_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_256_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha2_256_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha2_384_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha2_384_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha2_384_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha2_384_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha2_384_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha2_384_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha2_384_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha2_384_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_384_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_384_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha2_384_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha2_512_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha2_512_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha2_512_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha2_512_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha2_512_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha2_512_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha2_512_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha2_512_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha2_512_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_224_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_224_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha2_512_224_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha2_512_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha2_512_256_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha2_512_256_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha3_224_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha3_224_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha3_224_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha3_224_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha3_224_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha3_224_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha3_224_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha3_224_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_224_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_224_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha3_224_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha3_256_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha3_256_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha3_256_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha3_256_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha3_256_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha3_256_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha3_256_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha3_256_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_256_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha3_256_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha3_384_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha3_384_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha3_384_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha3_384_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha3_384_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha3_384_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha3_384_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha3_384_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_384_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_384_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha3_384_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_128s
		return OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_128f
		return OQS_SIG_slh_dsa_sha3_512_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_192s
		return OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_192f
		return OQS_SIG_slh_dsa_sha3_512_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_256s
		return OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_sha2_256f
		return OQS_SIG_slh_dsa_sha3_512_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_128s
		return OQS_SIG_slh_dsa_sha3_512_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_128f
		return OQS_SIG_slh_dsa_sha3_512_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_192s
		return OQS_SIG_slh_dsa_sha3_512_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_192f
		return OQS_SIG_slh_dsa_sha3_512_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_256s
		return OQS_SIG_slh_dsa_sha3_512_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_sha3_512_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_sha3_512_prehash_shake_256f
		return OQS_SIG_slh_dsa_sha3_512_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_128s
		return OQS_SIG_slh_dsa_shake_128_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_128f
		return OQS_SIG_slh_dsa_shake_128_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_192s
		return OQS_SIG_slh_dsa_shake_128_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_192f
		return OQS_SIG_slh_dsa_shake_128_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_256s
		return OQS_SIG_slh_dsa_shake_128_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_sha2_256f
		return OQS_SIG_slh_dsa_shake_128_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_128s
		return OQS_SIG_slh_dsa_shake_128_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_128f
		return OQS_SIG_slh_dsa_shake_128_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_192s
		return OQS_SIG_slh_dsa_shake_128_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_192f
		return OQS_SIG_slh_dsa_shake_128_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_256s
		return OQS_SIG_slh_dsa_shake_128_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_128_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_128_prehash_shake_256f
		return OQS_SIG_slh_dsa_shake_128_prehash_shake_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_128s
		return OQS_SIG_slh_dsa_shake_256_prehash_sha2_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_128f
		return OQS_SIG_slh_dsa_shake_256_prehash_sha2_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_192s
		return OQS_SIG_slh_dsa_shake_256_prehash_sha2_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_192f
		return OQS_SIG_slh_dsa_shake_256_prehash_sha2_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_256s
		return OQS_SIG_slh_dsa_shake_256_prehash_sha2_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_sha2_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_sha2_256f
		return OQS_SIG_slh_dsa_shake_256_prehash_sha2_256f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_128s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_128s
		return OQS_SIG_slh_dsa_shake_256_prehash_shake_128s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_128f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_128f
		return OQS_SIG_slh_dsa_shake_256_prehash_shake_128f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_192s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_192s
		return OQS_SIG_slh_dsa_shake_256_prehash_shake_192s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_192f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_192f
		return OQS_SIG_slh_dsa_shake_256_prehash_shake_192f_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_256s)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_256s
		return OQS_SIG_slh_dsa_shake_256_prehash_shake_256s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_slh_dsa_shake_256_prehash_shake_256f)) {
#ifdef OQS_ENABLE_SIG_slh_dsa_shake_256_prehash_shake_256f
		return OQS_SIG_slh_dsa_shake_256_prehash_shake_256f_new();
#else
		return NULL;
#endif
		///// OQS_COPY_FROM_SLH_DSA_FRAGMENT_SIGNEW_END
		// EDIT-WHEN-ADDING-SIG
	} else {
		return NULL;
	}
}

OQS_API OQS_STATUS OQS_SIG_keypair(const OQS_SIG *sig, uint8_t *public_key, uint8_t *secret_key) {
	if (sig == NULL || sig->keypair(public_key, secret_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_sign(const OQS_SIG *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	if (sig == NULL || sig->sign(signature, signature_len, message, message_len, secret_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_sign_with_ctx_str(const OQS_SIG *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *secret_key) {
	if (sig == NULL || sig->sign_with_ctx_str(signature, signature_len, message, message_len, ctx_str, ctx_str_len, secret_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_verify(const OQS_SIG *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (sig == NULL || sig->verify(message, message_len, signature, signature_len, public_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_verify_with_ctx_str(const OQS_SIG *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *public_key) {
	if (sig == NULL || sig->verify_with_ctx_str(message, message_len, signature, signature_len, ctx_str, ctx_str_len, public_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API bool OQS_SIG_supports_ctx_str(const char *alg_name) {
	OQS_SIG *sig = OQS_SIG_new(alg_name);
	if (sig == NULL) {
		return false;
	}
	bool result = sig->sig_with_ctx_support;
	OQS_SIG_free(sig);
	return result;
}

OQS_API void OQS_SIG_free(OQS_SIG *sig) {
	OQS_MEM_insecure_free(sig);
}
