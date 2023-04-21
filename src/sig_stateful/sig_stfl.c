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

OQS_API const char *OQS_SIG_STFL_alg_identifier(size_t i) {

	const char *a[OQS_SIG_algs_length] = {
		OQS_SIG_STFL_alg_xmss_sha256_h10,
		OQS_SIG_STFL_alg_xmss_sha256_h16,
		OQS_SIG_STFL_alg_xmss_sha256_h20,
		OQS_SIG_STFL_alg_xmss_sha512_h10,
		OQS_SIG_STFL_alg_xmss_sha512_h16,
		OQS_SIG_STFL_alg_xmss_sha512_h20,
		OQS_SIG_STFL_alg_xmss_shake256_h10,
		OQS_SIG_STFL_alg_xmss_shake256_h16,
		OQS_SIG_STFL_alg_xmss_shake256_h20,
		OQS_SIG_STFL_alg_xmss_shake512_h10,
		OQS_SIG_STFL_alg_xmss_shake512_h16,
		OQS_SIG_STFL_alg_xmss_shake512_h20,
		OQS_SIG_STFL_alg_xmssmt_sha256_2_h20,
		OQS_SIG_STFL_alg_xmssmt_sha256_4_h20,
		OQS_SIG_STFL_alg_xmssmt_sha256_2_h40,
		OQS_SIG_STFL_alg_xmssmt_sha256_4_h40,
		OQS_SIG_STFL_alg_xmssmt_sha256_8_h40,
		OQS_SIG_STFL_alg_xmssmt_sha256_3_h60,
		OQS_SIG_STFL_alg_xmssmt_sha256_6_h60,
		OQS_SIG_STFL_alg_xmssmt_sha256_12_h60,

		OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1,
		OQS_SIG_STFL_alg_lms_sha256_n32_h5_w2,
		OQS_SIG_STFL_alg_lms_sha256_n32_h5_w4,
		OQS_SIG_STFL_alg_lms_sha256_n32_h5_w8,

		OQS_SIG_STFL_alg_lms_sha256_n32_h10_w1,
		OQS_SIG_STFL_alg_lms_sha256_n32_h10_w2,
		OQS_SIG_STFL_alg_lms_sha256_n32_h10_w4,
		OQS_SIG_STFL_alg_lms_sha256_n32_h10_w8,

		OQS_SIG_STFL_alg_lms_sha256_n32_h15_w1,
		OQS_SIG_STFL_alg_lms_sha256_n32_h15_w2,
		OQS_SIG_STFL_alg_lms_sha256_n32_h15_w4,
		OQS_SIG_STFL_alg_lms_sha256_n32_h15_w8,

		OQS_SIG_STFL_alg_lms_sha256_n32_h20_w1,
		OQS_SIG_STFL_alg_lms_sha256_n32_h20_w2,
		OQS_SIG_STFL_alg_lms_sha256_n32_h20_w4,
		OQS_SIG_STFL_alg_lms_sha256_n32_h20_w8,

		OQS_SIG_STFL_alg_lms_sha256_n32_h25_w1,
		OQS_SIG_STFL_alg_lms_sha256_n32_h25_w2,
		OQS_SIG_STFL_alg_lms_sha256_n32_h25_w4,
		OQS_SIG_STFL_alg_lms_sha256_n32_h25_w8,
		OQS_SIG_STFL_alg_hss_sha256
	};

	if (i >= OQS_SIG_STFL_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}


OQS_API int OQS_SIG_STFL_alg_count(void) {
	return OQS_SIG_STFL_algs_length;
}


OQS_API int OQS_SIG_STFL_alg_is_enabled(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_2_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_4_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_2_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H40
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_4_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H40
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_8_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_8_H40
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_3_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_3_H60
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_6_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_6_H60
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_12_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_12_H60
		return 1;
#else
		return 0;
#endif
#ifdef OQS_ENABLE_SIG_STFL_LMS
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w2)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w4)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w8)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w1)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w2)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w4)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w8)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w1)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w2)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w4)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w8)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w1)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w2)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w4)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w8)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w1)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w2)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w4)) {
        return 1;
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w8)) {
        return 1;
#endif
#ifdef OQS_ENABLE_SIG_STFL_HSS
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_hss_sha256)) {
        return 1;
#endif
	}
	// EDIT-WHEN ADDING MORE XMSS/XMSS^MT ALGS
	else {
		return 0;
	}
}


OQS_API OQS_SIG_STFL *OQS_SIG_STFL_new(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H10
		return OQS_SIG_STFL_alg_xmss_sha256_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H16
		return OQS_SIG_STFL_alg_xmss_sha256_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H20
		return OQS_SIG_STFL_alg_xmss_sha256_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H10
		return OQS_SIG_STFL_alg_xmss_sha512_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H16
		return OQS_SIG_STFL_alg_xmss_sha512_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H20
		return OQS_SIG_STFL_alg_xmss_sha512_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H10
		return OQS_SIG_STFL_alg_xmss_shake256_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H20
		return OQS_SIG_STFL_alg_xmss_shake256_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H20
		return OQS_SIG_STFL_alg_xmss_shake256_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H10
		return OQS_SIG_STFL_alg_xmss_shake512_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H16
		return OQS_SIG_STFL_alg_xmss_shake512_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H20
		return OQS_SIG_STFL_alg_xmss_shake512_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_2_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H20
		return OQS_SIG_STFL_alg_xmssmt_sha256_2_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_4_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H20
		return OQS_SIG_STFL_alg_xmssmt_sha256_4_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_2_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H40
		return OQS_SIG_STFL_alg_xmssmt_sha256_2_h40_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_4_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H40
		return OQS_SIG_STFL_alg_xmssmt_sha256_4_h40_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_8_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_8_H40
		return OQS_SIG_STFL_alg_xmssmt_sha256_8_h40_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_3_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_3_H60
		return OQS_SIG_STFL_alg_xmssmt_sha256_3_h60_new();
#else
		return NULL;
#endif
	}  else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_6_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_6_H60
		return OQS_SIG_STFL_alg_xmssmt_sha256_6_h60_new();
#else
		return NULL;
#endif
	}  else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_12_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_12_H60
		return OQS_SIG_STFL_alg_xmssmt_sha256_12_h60_new();
#else
		return NULL;
#endif
#ifdef OQS_ENABLE_SIG_STFL_LMS
	}  else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1)) {
	    return OQS_SIG_STFL_alg_lms_sha256_h5_w1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w2)) {
        return OQS_SIG_STFL_alg_lms_sha256_h5_w2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w4)) {
        return OQS_SIG_STFL_alg_lms_sha256_h5_w4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w8)) {
        return OQS_SIG_STFL_alg_lms_sha256_h5_w8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w1)) {
        return OQS_SIG_STFL_alg_lms_sha256_h10_w1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w2)) {
        return OQS_SIG_STFL_alg_lms_sha256_h10_w2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w4)) {
        return OQS_SIG_STFL_alg_lms_sha256_h10_w4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w8)) {
        return OQS_SIG_STFL_alg_lms_sha256_h10_w8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w1)) {
        return OQS_SIG_STFL_alg_lms_sha256_h15_w1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w2)) {
        return OQS_SIG_STFL_alg_lms_sha256_h15_w2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w4)) {
        return OQS_SIG_STFL_alg_lms_sha256_h15_w4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w8)) {
        return OQS_SIG_STFL_alg_lms_sha256_h15_w8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w1)) {
        return OQS_SIG_STFL_alg_lms_sha256_h20_w1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w2)) {
        return OQS_SIG_STFL_alg_lms_sha256_h20_w2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w4)) {
        return OQS_SIG_STFL_alg_lms_sha256_h20_w4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w8)) {
        return OQS_SIG_STFL_alg_lms_sha256_h20_w8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w1)) {
        return OQS_SIG_STFL_alg_lms_sha256_h25_w1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w2)) {
        return OQS_SIG_STFL_alg_lms_sha256_h25_w2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w4)) {
        return OQS_SIG_STFL_alg_lms_sha256_h25_w4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w8)) {
        return OQS_SIG_STFL_alg_lms_sha256_h25_w8_new();
#endif //OQS_ENABLE_SIG_STFL_LMS
#ifdef OQS_ENABLE_SIG_STFL_HSS
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_hss_sha256)) {
        return NULL;
#endif
	} else {
		return NULL;
	}
}


OQS_API OQS_STATUS OQS_SIG_STFL_keypair(const OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->keypair == NULL || sig->keypair(public_key, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_STFL_sign(const OQS_SIG_STFL *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->sign == NULL || sig->sign(signature, signature_len, message, message_len, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_STFL_verify(const OQS_SIG_STFL *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (sig == NULL || sig->verify == NULL || sig->verify(message, message_len, signature, signature_len, public_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_SECRET_KEY *OQS_SIG_STFL_derive_subkey(const OQS_SIG_STFL *sig, OQS_SECRET_KEY *master_key, const unsigned long long number_of_signatures) {
	if (sig == NULL || master_key == NULL || number_of_signatures == 0L) {
		return NULL;
	};
	return sig->derive_subkey(master_key, number_of_signatures);
}

OQS_API void OQS_SIG_STFL_free(OQS_SIG_STFL *sig) {
	OQS_MEM_insecure_free(sig);
}



// ================================= OQS_SECRET_KEY FUNCTION ===============================================


OQS_API OQS_SECRET_KEY *OQS_SECRET_KEY_new(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H10
		return OQS_SECRET_KEY_XMSS_SHA256_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H16
		return OQS_SECRET_KEY_XMSS_SHA256_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H20
		return OQS_SECRET_KEY_XMSS_SHA256_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H10
		return OQS_SECRET_KEY_XMSS_SHA512_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H16
		return OQS_SECRET_KEY_XMSS_SHA512_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA512_H20
		return OQS_SECRET_KEY_XMSS_SHA512_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H10
		return OQS_SECRET_KEY_XMSS_SHAKE256_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H20
		return OQS_SECRET_KEY_XMSS_SHAKE256_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE256_H20
		return OQS_SECRET_KEY_XMSS_SHAKE256_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H10
		return OQS_SECRET_KEY_XMSS_SHAKE512_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H16
		return OQS_SECRET_KEY_XMSS_SHAKE512_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHAKE512_H20
		return OQS_SECRET_KEY_XMSS_SHAKE512_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_2_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H20
		return OQS_SECRET_KEY_XMSSMT_SHA256_2_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_4_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H20
		return OQS_SECRET_KEY_XMSSMT_SHA256_4_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_2_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_2_H40
		return OQS_SECRET_KEY_XMSSMT_SHA256_2_H40_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_4_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_4_H40
		return OQS_SECRET_KEY_XMSSMT_SHA256_4_H40_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_8_h40)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_8_H40
		return OQS_SECRET_KEY_XMSSMT_SHA256_8_H40_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_3_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_3_H60
		return OQS_SECRET_KEY_XMSSMT_SHA256_3_H60_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_6_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_6_H60
		return OQS_SECRET_KEY_XMSSMT_SHA256_6_H60_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_12_h60)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSSMT_SHA256_12_H60
		return OQS_SECRET_KEY_XMSSMT_SHA256_12_H60_new();
#else
		return NULL;

#endif
#ifdef OQS_ENABLE_SIG_STFL_LMS
    }  else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1)) {
        return OQS_SECRET_KEY_LMS_SHA256_H5_W1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w2)) {
        return OQS_SECRET_KEY_LMS_SHA256_H5_W2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w4)) {
        return OQS_SECRET_KEY_LMS_SHA256_H5_W4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w8)) {
        return OQS_SECRET_KEY_LMS_SHA256_H5_W8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w1)) {
        return OQS_SECRET_KEY_LMS_SHA256_H10_W1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w2)) {
        return OQS_SECRET_KEY_LMS_SHA256_H10_W2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w4)) {
        return OQS_SECRET_KEY_LMS_SHA256_H10_W4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h10_w8)) {
        return OQS_SECRET_KEY_LMS_SHA256_H10_W8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w1)) {
        return OQS_SECRET_KEY_LMS_SHA256_H15_W1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w2)) {
        return OQS_SECRET_KEY_LMS_SHA256_H15_W2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w4)) {
        return OQS_SECRET_KEY_LMS_SHA256_H15_W4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h15_w8)) {
        return OQS_SECRET_KEY_LMS_SHA256_H15_W8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w1)) {
        return OQS_SECRET_KEY_LMS_SHA256_H20_W1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w2)) {
        return OQS_SECRET_KEY_LMS_SHA256_H20_W2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w4)) {
        return OQS_SECRET_KEY_LMS_SHA256_H20_W4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h20_w8)) {
        return OQS_SECRET_KEY_LMS_SHA256_H20_W8_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w1)) {
        return OQS_SECRET_KEY_LMS_SHA256_H25_W1_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w2)) {
        return OQS_SECRET_KEY_LMS_SHA256_H25_W2_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w4)) {
        return OQS_SECRET_KEY_LMS_SHA256_H25_W4_new();
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h25_w8)) {
        return OQS_SECRET_KEY_LMS_SHA256_H25_W8_new();
#endif //OQS_ENABLE_SIG_STFL_LMS
#ifdef OQS_ENABLE_SIG_STFL_HSS
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_hss_sha256)) {
        return NULL;
#endif
     }
     else {
		return NULL;
	}
}


OQS_API void OQS_SECRET_KEY_free(OQS_SECRET_KEY *sk) {
    if (sk == NULL) return;
	OQS_MEM_secure_free(sk->secret_key, sk->length_secret_key);
	OQS_MEM_secure_free(sk, sizeof(sk));
}
