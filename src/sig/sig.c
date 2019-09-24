#include <assert.h>
#include <stdlib.h>
#if defined(_WIN32)
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include <oqs/oqs.h>

OQS_API const char *OQS_SIG_alg_identifier(size_t i) {
	// EDIT-WHEN-ADDING-SIG
	const char *a[OQS_SIG_algs_length] = {
	    OQS_SIG_alg_default,
	    ///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALG_IDENTIFIER_START
	    OQS_SIG_alg_dilithium_2, OQS_SIG_alg_dilithium_3, OQS_SIG_alg_dilithium_4,
	    OQS_SIG_alg_mqdss_31_48, OQS_SIG_alg_mqdss_31_64,
	    OQS_SIG_alg_sphincs_haraka_128f_robust, OQS_SIG_alg_sphincs_haraka_128f_simple, OQS_SIG_alg_sphincs_haraka_128s_robust, OQS_SIG_alg_sphincs_haraka_128s_simple, OQS_SIG_alg_sphincs_haraka_192f_robust, OQS_SIG_alg_sphincs_haraka_192f_simple, OQS_SIG_alg_sphincs_haraka_192s_robust, OQS_SIG_alg_sphincs_haraka_192s_simple, OQS_SIG_alg_sphincs_haraka_256f_robust, OQS_SIG_alg_sphincs_haraka_256f_simple, OQS_SIG_alg_sphincs_haraka_256s_robust, OQS_SIG_alg_sphincs_haraka_256s_simple, OQS_SIG_alg_sphincs_sha256_128f_robust, OQS_SIG_alg_sphincs_sha256_128f_simple, OQS_SIG_alg_sphincs_sha256_128s_robust, OQS_SIG_alg_sphincs_sha256_128s_simple, OQS_SIG_alg_sphincs_sha256_192f_robust, OQS_SIG_alg_sphincs_sha256_192f_simple, OQS_SIG_alg_sphincs_sha256_192s_robust, OQS_SIG_alg_sphincs_sha256_192s_simple, OQS_SIG_alg_sphincs_sha256_256f_robust, OQS_SIG_alg_sphincs_sha256_256f_simple, OQS_SIG_alg_sphincs_sha256_256s_robust, OQS_SIG_alg_sphincs_sha256_256s_simple, OQS_SIG_alg_sphincs_shake256_128f_robust, OQS_SIG_alg_sphincs_shake256_128f_simple, OQS_SIG_alg_sphincs_shake256_128s_robust, OQS_SIG_alg_sphincs_shake256_128s_simple, OQS_SIG_alg_sphincs_shake256_192f_robust, OQS_SIG_alg_sphincs_shake256_192f_simple, OQS_SIG_alg_sphincs_shake256_192s_robust, OQS_SIG_alg_sphincs_shake256_192s_simple, OQS_SIG_alg_sphincs_shake256_256f_robust, OQS_SIG_alg_sphincs_shake256_256f_simple, OQS_SIG_alg_sphincs_shake256_256s_robust, OQS_SIG_alg_sphincs_shake256_256s_simple,
	    ///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALG_IDENTIFIER_END
	    OQS_SIG_alg_picnic_L1_FS, OQS_SIG_alg_picnic_L1_UR, OQS_SIG_alg_picnic_L3_FS, OQS_SIG_alg_picnic_L3_UR, OQS_SIG_alg_picnic_L5_FS, OQS_SIG_alg_picnic_L5_UR, OQS_SIG_alg_picnic2_L1_FS, OQS_SIG_alg_picnic2_L3_FS, OQS_SIG_alg_picnic2_L5_FS,
	    OQS_SIG_alg_qTesla_p_I, OQS_SIG_alg_qTesla_p_III};
	if (i >= OQS_SIG_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}

OQS_API int OQS_SIG_alg_count() {
	return OQS_SIG_algs_length;
}

OQS_API int OQS_SIG_alg_is_enabled(const char *method_name) {
	if (method_name == NULL) {
		return 0;
	}
	if (0 == strcasecmp(method_name, OQS_SIG_alg_default)) {
		return OQS_SIG_alg_is_enabled(OQS_SIG_DEFAULT);
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ENABLED_CASE_START
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_dilithium_2)) {
#ifdef OQS_ENABLE_SIG_dilithium_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_dilithium_3)) {
#ifdef OQS_ENABLE_SIG_dilithium_3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_dilithium_4)) {
#ifdef OQS_ENABLE_SIG_dilithium_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mqdss_31_48)) {
#ifdef OQS_ENABLE_SIG_mqdss_31_48
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mqdss_31_64)) {
#ifdef OQS_ENABLE_SIG_mqdss_31_64
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192s_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256f_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256f_simple
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256s_robust
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256s_simple
		return 1;
#else
		return 0;
#endif
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ENABLED_CASE_END
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L1_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L1_FS
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L1_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L1_UR
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L3_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L3_FS
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L3_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L3_UR
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L5_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L5_FS
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L5_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L5_UR
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic2_L1_FS)) {
#ifdef OQS_ENABLE_SIG_picnic2_L1_FS
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic2_L3_FS)) {
#ifdef OQS_ENABLE_SIG_picnic2_L3_FS
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic2_L5_FS)) {
#ifdef OQS_ENABLE_SIG_picnic2_L5_FS
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTesla_p_I)) {
#ifdef OQS_ENABLE_SIG_qTesla_p_I
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTesla_p_III)) {
#ifdef OQS_ENABLE_SIG_qTesla_p_III
		return 1;
#else
		return 0;
#endif
		// EDIT-WHEN-ADDING-SIG
	} else {
		return 0;
	}
}

OQS_API OQS_SIG *OQS_SIG_new(const char *method_name) {
	if (method_name == NULL) {
		return NULL;
	}
	if (0 == strcasecmp(method_name, OQS_SIG_alg_default)) {
		return OQS_SIG_new(OQS_SIG_DEFAULT);
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_NEW_CASE_START
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_dilithium_2)) {
#ifdef OQS_ENABLE_SIG_dilithium_2
		return OQS_SIG_dilithium_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_dilithium_3)) {
#ifdef OQS_ENABLE_SIG_dilithium_3
		return OQS_SIG_dilithium_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_dilithium_4)) {
#ifdef OQS_ENABLE_SIG_dilithium_4
		return OQS_SIG_dilithium_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mqdss_31_48)) {
#ifdef OQS_ENABLE_SIG_mqdss_31_48
		return OQS_SIG_mqdss_31_48_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_mqdss_31_64)) {
#ifdef OQS_ENABLE_SIG_mqdss_31_64
		return OQS_SIG_mqdss_31_64_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128f_robust
		return OQS_SIG_sphincs_haraka_128f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128f_simple
		return OQS_SIG_sphincs_haraka_128f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128s_robust
		return OQS_SIG_sphincs_haraka_128s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_128s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_128s_simple
		return OQS_SIG_sphincs_haraka_128s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192f_robust
		return OQS_SIG_sphincs_haraka_192f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192f_simple
		return OQS_SIG_sphincs_haraka_192f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192s_robust
		return OQS_SIG_sphincs_haraka_192s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_192s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_192s_simple
		return OQS_SIG_sphincs_haraka_192s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256f_robust
		return OQS_SIG_sphincs_haraka_256f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256f_simple
		return OQS_SIG_sphincs_haraka_256f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256s_robust
		return OQS_SIG_sphincs_haraka_256s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_haraka_256s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_haraka_256s_simple
		return OQS_SIG_sphincs_haraka_256s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128f_robust
		return OQS_SIG_sphincs_sha256_128f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128f_simple
		return OQS_SIG_sphincs_sha256_128f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128s_robust
		return OQS_SIG_sphincs_sha256_128s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_128s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_128s_simple
		return OQS_SIG_sphincs_sha256_128s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192f_robust
		return OQS_SIG_sphincs_sha256_192f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192f_simple
		return OQS_SIG_sphincs_sha256_192f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192s_robust
		return OQS_SIG_sphincs_sha256_192s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_192s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_192s_simple
		return OQS_SIG_sphincs_sha256_192s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256f_robust
		return OQS_SIG_sphincs_sha256_256f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256f_simple
		return OQS_SIG_sphincs_sha256_256f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256s_robust
		return OQS_SIG_sphincs_sha256_256s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_sha256_256s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_sha256_256s_simple
		return OQS_SIG_sphincs_sha256_256s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128f_robust
		return OQS_SIG_sphincs_shake256_128f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128f_simple
		return OQS_SIG_sphincs_shake256_128f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128s_robust
		return OQS_SIG_sphincs_shake256_128s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_128s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_128s_simple
		return OQS_SIG_sphincs_shake256_128s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192f_robust
		return OQS_SIG_sphincs_shake256_192f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192f_simple
		return OQS_SIG_sphincs_shake256_192f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192s_robust
		return OQS_SIG_sphincs_shake256_192s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_192s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_192s_simple
		return OQS_SIG_sphincs_shake256_192s_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256f_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256f_robust
		return OQS_SIG_sphincs_shake256_256f_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256f_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256f_simple
		return OQS_SIG_sphincs_shake256_256f_simple_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256s_robust)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256s_robust
		return OQS_SIG_sphincs_shake256_256s_robust_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_sphincs_shake256_256s_simple)) {
#ifdef OQS_ENABLE_SIG_sphincs_shake256_256s_simple
		return OQS_SIG_sphincs_shake256_256s_simple_new();
#else
		return NULL;
#endif
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_NEW_CASE_END
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L1_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L1_FS
		return OQS_SIG_picnic_L1_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L1_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L1_UR
		return OQS_SIG_picnic_L1_UR_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L3_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L3_FS
		return OQS_SIG_picnic_L3_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L3_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L3_UR
		return OQS_SIG_picnic_L3_UR_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L5_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L5_FS
		return OQS_SIG_picnic_L5_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L5_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L5_UR
		return OQS_SIG_picnic_L5_UR_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic2_L1_FS)) {
#ifdef OQS_ENABLE_SIG_picnic2_L1_FS
		return OQS_SIG_picnic2_L1_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic2_L3_FS)) {
#ifdef OQS_ENABLE_SIG_picnic2_L3_FS
		return OQS_SIG_picnic2_L3_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic2_L5_FS)) {
#ifdef OQS_ENABLE_SIG_picnic2_L5_FS
		return OQS_SIG_picnic2_L5_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTesla_p_I)) {
#ifdef OQS_ENABLE_SIG_qTesla_p_I
		return OQS_SIG_qTesla_p_I_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTesla_p_III)) {
#ifdef OQS_ENABLE_SIG_qTesla_p_III
		return OQS_SIG_qTesla_p_III_new();
#else
		return NULL;
#endif
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

OQS_API OQS_STATUS OQS_SIG_verify(const OQS_SIG *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (sig == NULL || sig->verify(message, message_len, signature, signature_len, public_key) != OQS_SUCCESS) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API void OQS_SIG_free(OQS_SIG *sig) {
	OQS_MEM_insecure_free(sig);
}
