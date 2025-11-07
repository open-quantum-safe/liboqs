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

#ifdef OQS_ENABLE_SIG_STFL_XMSS
#include <oqs/sig_stfl_xmss.h>
#endif // OQS_ENABLE_SIG_STFL_XMSS

#ifdef OQS_ENABLE_SIG_STFL_LMS
#include <oqs/sig_stfl_lms.h>
#endif // OQS_ENABLE_SIG_STFL_LMS

OQS_API const char *OQS_SIG_STFL_alg_identifier(size_t i) {

	const char *a[OQS_SIG_STFL_algs_length] = {
		// XMSS
		OQS_SIG_STFL_alg_xmss_sha256_h10,
		OQS_SIG_STFL_alg_xmss_sha256_h16,
		OQS_SIG_STFL_alg_xmss_sha256_h20,
		OQS_SIG_STFL_alg_xmss_shake128_h10,
		OQS_SIG_STFL_alg_xmss_shake128_h16,
		OQS_SIG_STFL_alg_xmss_shake128_h20,
		OQS_SIG_STFL_alg_xmss_sha512_h10,
		OQS_SIG_STFL_alg_xmss_sha512_h16,
		OQS_SIG_STFL_alg_xmss_sha512_h20,
		OQS_SIG_STFL_alg_xmss_shake256_h10,
		OQS_SIG_STFL_alg_xmss_shake256_h16,
		OQS_SIG_STFL_alg_xmss_shake256_h20,
		OQS_SIG_STFL_alg_xmss_sha256_h10_192,
		OQS_SIG_STFL_alg_xmss_sha256_h16_192,
		OQS_SIG_STFL_alg_xmss_sha256_h20_192,
		OQS_SIG_STFL_alg_xmss_shake256_h10_192,
		OQS_SIG_STFL_alg_xmss_shake256_h16_192,
		OQS_SIG_STFL_alg_xmss_shake256_h20_192,
		OQS_SIG_STFL_alg_xmss_shake256_h10_256,
		OQS_SIG_STFL_alg_xmss_shake256_h16_256,
		OQS_SIG_STFL_alg_xmss_shake256_h20_256,
		OQS_SIG_STFL_alg_xmssmt_sha256_h20_2,
		OQS_SIG_STFL_alg_xmssmt_sha256_h20_4,
		OQS_SIG_STFL_alg_xmssmt_sha256_h40_2,
		OQS_SIG_STFL_alg_xmssmt_sha256_h40_4,
		OQS_SIG_STFL_alg_xmssmt_sha256_h40_8,
		OQS_SIG_STFL_alg_xmssmt_sha256_h60_3,
		OQS_SIG_STFL_alg_xmssmt_sha256_h60_6,
		OQS_SIG_STFL_alg_xmssmt_sha256_h60_12,
		OQS_SIG_STFL_alg_xmssmt_shake128_h20_2,
		OQS_SIG_STFL_alg_xmssmt_shake128_h20_4,
		OQS_SIG_STFL_alg_xmssmt_shake128_h40_2,
		OQS_SIG_STFL_alg_xmssmt_shake128_h40_4,
		OQS_SIG_STFL_alg_xmssmt_shake128_h40_8,
		OQS_SIG_STFL_alg_xmssmt_shake128_h60_3,
		OQS_SIG_STFL_alg_xmssmt_shake128_h60_6,
		OQS_SIG_STFL_alg_xmssmt_shake128_h60_12,
		// LMS
		OQS_SIG_STFL_alg_lms_sha256_h5_w1,
		OQS_SIG_STFL_alg_lms_sha256_h5_w2,
		OQS_SIG_STFL_alg_lms_sha256_h5_w4,
		OQS_SIG_STFL_alg_lms_sha256_h5_w8,

		OQS_SIG_STFL_alg_lms_sha256_h10_w1,
		OQS_SIG_STFL_alg_lms_sha256_h10_w2,
		OQS_SIG_STFL_alg_lms_sha256_h10_w4,
		OQS_SIG_STFL_alg_lms_sha256_h10_w8,

		OQS_SIG_STFL_alg_lms_sha256_h15_w1,
		OQS_SIG_STFL_alg_lms_sha256_h15_w2,
		OQS_SIG_STFL_alg_lms_sha256_h15_w4,
		OQS_SIG_STFL_alg_lms_sha256_h15_w8,

		OQS_SIG_STFL_alg_lms_sha256_h20_w1,
		OQS_SIG_STFL_alg_lms_sha256_h20_w2,
		OQS_SIG_STFL_alg_lms_sha256_h20_w4,
		OQS_SIG_STFL_alg_lms_sha256_h20_w8,

		OQS_SIG_STFL_alg_lms_sha256_h25_w1,
		OQS_SIG_STFL_alg_lms_sha256_h25_w2,
		OQS_SIG_STFL_alg_lms_sha256_h25_w4,
		OQS_SIG_STFL_alg_lms_sha256_h25_w8,

		//2-Level LMS
		OQS_SIG_STFL_alg_lms_sha256_h5_w8_h5_w8,
		OQS_SIG_STFL_alg_lms_sha256_h10_w4_h5_w8,

		OQS_SIG_STFL_alg_lms_sha256_h10_w8_h5_w8,
		OQS_SIG_STFL_alg_lms_sha256_h10_w2_h10_w2,
		OQS_SIG_STFL_alg_lms_sha256_h10_w4_h10_w4,
		OQS_SIG_STFL_alg_lms_sha256_h10_w8_h10_w8,

		OQS_SIG_STFL_alg_lms_sha256_h15_w8_h5_w8,
		OQS_SIG_STFL_alg_lms_sha256_h15_w8_h10_w8,
		OQS_SIG_STFL_alg_lms_sha256_h15_w8_h15_w8,

		OQS_SIG_STFL_alg_lms_sha256_h20_w8_h5_w8,
		OQS_SIG_STFL_alg_lms_sha256_h20_w8_h10_w8,
		OQS_SIG_STFL_alg_lms_sha256_h20_w8_h15_w8,
		OQS_SIG_STFL_alg_lms_sha256_h20_w8_h20_w8,
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
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10_192
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16_192
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20_192
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_192
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_192
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_192
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_256
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_256
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_256
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12
		return 1;
#else
		return 0;
#endif
	}
//#ifdef OQS_ENABLE_SIG_STFL_LMS
	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8
		return 1;
#else
		return 0;
#endif
	}

	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8
		return 1;
#else
		return 0;
#endif
	}

	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8
		return 1;
#else
		return 0;
#endif
	}

	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w8
		return 1;
#else
		return 0;
#endif
	}
	//2-Level LMS
	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w8_h5_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4_h5_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8_h5_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w2_h10_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w2_h10_w2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4_h10_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4_h10_w4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8_h10_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h5_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h10_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h15_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h5_w8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h20_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w1
		return 1;
#else
		return 0;
#endif
	}
//#endif //OQS_ENABLE_SIG_STFL_LMS
	else {
		return 0;
	}
}

OQS_API OQS_SIG_STFL *OQS_SIG_STFL_new(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10
		return OQS_SIG_STFL_alg_xmss_sha256_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
		return OQS_SIG_STFL_alg_xmss_sha256_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
		return OQS_SIG_STFL_alg_xmss_sha256_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10
		return OQS_SIG_STFL_alg_xmss_shake128_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
		return OQS_SIG_STFL_alg_xmss_shake128_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
		return OQS_SIG_STFL_alg_xmss_shake128_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10
		return OQS_SIG_STFL_alg_xmss_sha512_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
		return OQS_SIG_STFL_alg_xmss_sha512_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
		return OQS_SIG_STFL_alg_xmss_sha512_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10
		return OQS_SIG_STFL_alg_xmss_shake256_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
		return OQS_SIG_STFL_alg_xmss_shake256_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
		return OQS_SIG_STFL_alg_xmss_shake256_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10_192
		return OQS_SIG_STFL_alg_xmss_sha256_h10_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16_192
		return OQS_SIG_STFL_alg_xmss_sha256_h16_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20_192
		return OQS_SIG_STFL_alg_xmss_sha256_h20_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_192
		return OQS_SIG_STFL_alg_xmss_shake256_h10_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_192
		return OQS_SIG_STFL_alg_xmss_shake256_h16_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_192
		return OQS_SIG_STFL_alg_xmss_shake256_h20_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_256
		return OQS_SIG_STFL_alg_xmss_shake256_h10_256_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_256
		return OQS_SIG_STFL_alg_xmss_shake256_h16_256_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_256
		return OQS_SIG_STFL_alg_xmss_shake256_h20_256_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2
		return OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4
		return OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
		return OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4
		return OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8
		return OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
		return OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6
		return OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12
		return OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2
		return OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4
		return OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
		return OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4
		return OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8
		return OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
		return OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6
		return OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12
		return OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_new();
#else
		return NULL;
#endif
	}
//#ifdef OQS_ENABLE_SIG_STFL_LMS
	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w1
		return OQS_SIG_STFL_alg_lms_sha256_h5_w1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w2
		return OQS_SIG_STFL_alg_lms_sha256_h5_w2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w4
		return OQS_SIG_STFL_alg_lms_sha256_h5_w4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w8
		return OQS_SIG_STFL_alg_lms_sha256_h5_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w1
		return OQS_SIG_STFL_alg_lms_sha256_h10_w1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w2
		return OQS_SIG_STFL_alg_lms_sha256_h10_w2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4
		return OQS_SIG_STFL_alg_lms_sha256_h10_w4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8
		return OQS_SIG_STFL_alg_lms_sha256_h10_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w1
		return OQS_SIG_STFL_alg_lms_sha256_h15_w1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w2
		return OQS_SIG_STFL_alg_lms_sha256_h15_w2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w4
		return OQS_SIG_STFL_alg_lms_sha256_h15_w4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8
		return OQS_SIG_STFL_alg_lms_sha256_h15_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w1
		return OQS_SIG_STFL_alg_lms_sha256_h20_w1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w2
		return OQS_SIG_STFL_alg_lms_sha256_h20_w2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w4
		return OQS_SIG_STFL_alg_lms_sha256_h20_w4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8
		return OQS_SIG_STFL_alg_lms_sha256_h20_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w1
		return OQS_SIG_STFL_alg_lms_sha256_h25_w1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w2
		return OQS_SIG_STFL_alg_lms_sha256_h25_w2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w4
		return OQS_SIG_STFL_alg_lms_sha256_h25_w4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w8
		return OQS_SIG_STFL_alg_lms_sha256_h25_w8_new();
#else
		return NULL;
#endif
	}
//2-Level LMS
	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w8_h5_w8
		return OQS_SIG_STFL_alg_lms_sha256_h5_w8_h5_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4_h5_w8
		return OQS_SIG_STFL_alg_lms_sha256_h10_w4_h5_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8_h5_w8
		return OQS_SIG_STFL_alg_lms_sha256_h10_w8_h5_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w2_h10_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w2_h10_w2
		return OQS_SIG_STFL_alg_lms_sha256_h10_w2_h10_w2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4_h10_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4_h10_w4
		return OQS_SIG_STFL_alg_lms_sha256_h10_w4_h10_w4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8_h10_w8
		return OQS_SIG_STFL_alg_lms_sha256_h10_w8_h10_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h5_w8
		return OQS_SIG_STFL_alg_lms_sha256_h15_w8_h5_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h10_w8
		return OQS_SIG_STFL_alg_lms_sha256_h15_w8_h10_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h15_w8
		return OQS_SIG_STFL_alg_lms_sha256_h15_w8_h15_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h5_w8
		return OQS_SIG_STFL_alg_lms_sha256_h20_w8_h5_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h10_w8
		return OQS_SIG_STFL_alg_lms_sha256_h20_w8_h10_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h15_w8
		return OQS_SIG_STFL_alg_lms_sha256_h20_w8_h15_w8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h20_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h20_w8
		return OQS_SIG_STFL_alg_lms_sha256_h20_w8_h20_w8_new();
#else
		return NULL;
#endif
	}
//#endif //OQS_ENABLE_SIG_STFL_LMS
	else {
		return NULL;
	}
}

OQS_API OQS_STATUS OQS_SIG_STFL_keypair(const OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key) {
#ifndef OQS_ALLOW_STFL_KEY_AND_SIG_GEN
	(void)sig;
	(void)public_key;
	(void)secret_key;
	return OQS_ERROR;
#else
	if (sig == NULL || sig->keypair == NULL || sig->keypair(public_key, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
	return OQS_ERROR;
#endif
}

OQS_API OQS_STATUS OQS_SIG_STFL_sign(const OQS_SIG_STFL *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message,
                                     size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key) {
#ifndef OQS_ALLOW_STFL_KEY_AND_SIG_GEN
	(void)sig;
	(void)signature;
	(void)signature_len;
	(void)message;
	(void)message_len;
	(void)secret_key;
	return OQS_ERROR;
#else
	if (sig == NULL || sig->sign == NULL || sig->sign(signature, signature_len, message, message_len, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
#endif
}

OQS_API OQS_STATUS OQS_SIG_STFL_verify(const OQS_SIG_STFL *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (sig == NULL || sig->verify == NULL || sig->verify(message, message_len, signature, signature_len, public_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_STFL_sigs_remaining(const OQS_SIG_STFL *sig, unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
#ifndef OQS_ALLOW_STFL_KEY_AND_SIG_GEN
	(void)sig;
	(void)remain;
	(void)secret_key;
	return OQS_ERROR;
#else
	if (sig == NULL || sig->sigs_remaining == NULL || sig->sigs_remaining(remain, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
#endif //OQS_ALLOW_STFL_KEY_AND_SIG_GEN 
}

OQS_API OQS_STATUS OQS_SIG_STFL_sigs_total(const OQS_SIG_STFL *sig, unsigned long long *max, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
#ifndef OQS_ALLOW_STFL_KEY_AND_SIG_GEN
	(void)sig;
	(void)max;
	(void)secret_key;
	return OQS_ERROR;
#else
	if (sig == NULL || sig->sigs_total == NULL || sig->sigs_total(max, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
#endif //OQS_ALLOW_STFL_KEY_AND_SIG_GEN
}

OQS_API void OQS_SIG_STFL_free(OQS_SIG_STFL *sig) {
	OQS_MEM_insecure_free(sig);
}

// ================================= OQS_SIG_STFL_SECRET_KEY FUNCTION ===============================================

OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SIG_STFL_SECRET_KEY_new(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10
		return OQS_SECRET_KEY_XMSS_SHA256_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
		return OQS_SECRET_KEY_XMSS_SHA256_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
		return OQS_SECRET_KEY_XMSS_SHA256_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10
		return OQS_SECRET_KEY_XMSS_SHAKE128_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
		return OQS_SECRET_KEY_XMSS_SHAKE128_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
		return OQS_SECRET_KEY_XMSS_SHAKE128_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10
		return OQS_SECRET_KEY_XMSS_SHA512_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
		return OQS_SECRET_KEY_XMSS_SHA512_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
		return OQS_SECRET_KEY_XMSS_SHA512_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10
		return OQS_SECRET_KEY_XMSS_SHAKE256_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
		return OQS_SECRET_KEY_XMSS_SHAKE256_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
		return OQS_SECRET_KEY_XMSS_SHAKE256_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10_192
		return OQS_SECRET_KEY_XMSS_SHA256_H10_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16_192
		return OQS_SECRET_KEY_XMSS_SHA256_H16_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20_192
		return OQS_SECRET_KEY_XMSS_SHA256_H20_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_192
		return OQS_SECRET_KEY_XMSS_SHAKE256_H10_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_192
		return OQS_SECRET_KEY_XMSS_SHAKE256_H16_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_192)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_192
		return OQS_SECRET_KEY_XMSS_SHAKE256_H20_192_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10_256
		return OQS_SECRET_KEY_XMSS_SHAKE256_H10_256_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16_256
		return OQS_SECRET_KEY_XMSS_SHAKE256_H16_256_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20_256)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20_256
		return OQS_SECRET_KEY_XMSS_SHAKE256_H20_256_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2
		return OQS_SECRET_KEY_XMSSMT_SHA256_H20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4
		return OQS_SECRET_KEY_XMSSMT_SHA256_H20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
		return OQS_SECRET_KEY_XMSSMT_SHA256_H40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4
		return OQS_SECRET_KEY_XMSSMT_SHA256_H40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8
		return OQS_SECRET_KEY_XMSSMT_SHA256_H40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
		return OQS_SECRET_KEY_XMSSMT_SHA256_H60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6
		return OQS_SECRET_KEY_XMSSMT_SHA256_H60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12
		return OQS_SECRET_KEY_XMSSMT_SHA256_H60_12_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_12_new();
#else
		return NULL;
#endif
	}
//#ifdef OQS_ENABLE_SIG_STFL_LMS
	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w1
		return OQS_SECRET_KEY_LMS_SHA256_H5_W1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w2
		return OQS_SECRET_KEY_LMS_SHA256_H5_W2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w4
		return OQS_SECRET_KEY_LMS_SHA256_H5_W4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w8
		return OQS_SECRET_KEY_LMS_SHA256_H5_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w1
		return OQS_SECRET_KEY_LMS_SHA256_H10_W1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w2
		return OQS_SECRET_KEY_LMS_SHA256_H10_W2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4
		return OQS_SECRET_KEY_LMS_SHA256_H10_W4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8
		return OQS_SECRET_KEY_LMS_SHA256_H10_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w1
		return OQS_SECRET_KEY_LMS_SHA256_H15_W1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w2
		return OQS_SECRET_KEY_LMS_SHA256_H15_W2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w4
		return OQS_SECRET_KEY_LMS_SHA256_H15_W4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8
		return OQS_SECRET_KEY_LMS_SHA256_H15_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w1
		return OQS_SECRET_KEY_LMS_SHA256_H20_W1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w2
		return OQS_SECRET_KEY_LMS_SHA256_H20_W2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w4
		return OQS_SECRET_KEY_LMS_SHA256_H20_W4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8
		return OQS_SECRET_KEY_LMS_SHA256_H20_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w1
		return OQS_SECRET_KEY_LMS_SHA256_H25_W1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w2
		return OQS_SECRET_KEY_LMS_SHA256_H25_W2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w4
		return OQS_SECRET_KEY_LMS_SHA256_H25_W4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h25_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h25_w4
		return OQS_SECRET_KEY_LMS_SHA256_H25_W8_new();
#else
		return NULL;
#endif
	}
//2-Level LMS
	else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h5_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h5_w8_h5_w8
		return OQS_SECRET_KEY_LMS_SHA256_H5_W8_H5_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8_h5_w8
		return OQS_SECRET_KEY_LMS_SHA256_H10_W8_H5_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w2_h10_w2)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w2_h10_w2
		return OQS_SECRET_KEY_LMS_SHA256_H10_W2_H10_W2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4_h10_w4)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4_h10_w4
		return OQS_SECRET_KEY_LMS_SHA256_H10_W4_H10_W4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w4_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w4_h5_w8
		return OQS_SECRET_KEY_LMS_SHA256_H10_W4_H5_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h10_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8_h10_w8
		return OQS_SECRET_KEY_LMS_SHA256_H10_W8_H10_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h5_w8
		return OQS_SECRET_KEY_LMS_SHA256_H15_W8_H5_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h10_w8_h10_w8
		return OQS_SECRET_KEY_LMS_SHA256_H15_W8_H10_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h15_w8_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h15_w8_h15_w8
		return OQS_SECRET_KEY_LMS_SHA256_H15_W8_H15_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h5_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h5_w8
		return OQS_SECRET_KEY_LMS_SHA256_H20_W8_H5_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h10_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h10_w8
		return OQS_SECRET_KEY_LMS_SHA256_H20_W8_H10_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h15_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h15_w8
		return OQS_SECRET_KEY_LMS_SHA256_H20_W8_H15_W8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_h20_w8_h20_w8)) {
#ifdef OQS_ENABLE_SIG_STFL_lms_sha256_h20_w8_h20_w8
		return OQS_SECRET_KEY_LMS_SHA256_H20_W8_H20_W8_new();
#else
		return NULL;
#endif
	}
//#endif //OQS_ENABLE_SIG_STFL_LMS
	else {
		return NULL;
	}
}

OQS_API void OQS_SIG_STFL_SECRET_KEY_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL || sk->free_key == NULL) {
		return;
	}

	/* Call object specific free */
	sk->free_key(sk);

	/* Free sk object */
	OQS_MEM_secure_free(sk, sizeof(*sk));
}

OQS_API void OQS_SIG_STFL_SECRET_KEY_SET_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context) {
	if (sk == NULL || sk->set_scrt_key_store_cb == NULL) {
		return;
	}
	sk->set_scrt_key_store_cb(sk, store_cb, context);
}

/* Convert secret key object to byte string */
OQS_API OQS_STATUS OQS_SIG_STFL_SECRET_KEY_serialize(uint8_t **sk_buf_ptr, size_t *sk_buf_len, const OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL || sk_buf_len == NULL || sk_buf_ptr == NULL || sk->serialize_key == NULL) {
		return OQS_ERROR;
	}

	return sk->serialize_key(sk_buf_ptr, sk_buf_len, sk);
}

/* Insert secret key byte string in an Stateful secret key object */
OQS_API OQS_STATUS OQS_SIG_STFL_SECRET_KEY_deserialize(OQS_SIG_STFL_SECRET_KEY *sk, const uint8_t *sk_buf, const size_t sk_buf_len, void *context) {
	if (sk == NULL || sk_buf == NULL || sk->deserialize_key == NULL) {
		return OQS_ERROR;
	}

	return sk->deserialize_key(sk, sk_buf, sk_buf_len, context);
}

/*  OQS_SIG_STFL_SECRET_KEY_SET_lock callback function*/
OQS_API void OQS_SIG_STFL_SECRET_KEY_SET_lock(OQS_SIG_STFL_SECRET_KEY *sk, lock_key lock) {
	if (sk == NULL) {
		return;
	}
	sk->lock_key = lock;
}

/*  OQS_SIG_STFL_SECRET_KEY_SET_unlock callback function */
OQS_API void OQS_SIG_STFL_SECRET_KEY_SET_unlock(OQS_SIG_STFL_SECRET_KEY *sk, unlock_key unlock) {
	if (sk == NULL) {
		return;
	}
	sk->unlock_key = unlock;
}

/*  OQS_SIG_STFL_SECRET_KEY_SET_mutex */
OQS_API void OQS_SIG_STFL_SECRET_KEY_SET_mutex(OQS_SIG_STFL_SECRET_KEY *sk, void *mutex) {
	if (sk == NULL) {
		return;
	}
	sk->mutex = mutex;
}

/* OQS_SIG_STFL_SECRET_KEY_lock  */
OQS_STATUS OQS_SIG_STFL_SECRET_KEY_lock(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return OQS_ERROR;
	}
	if (sk->lock_key == NULL) {
		return OQS_SUCCESS;
	}

	// Try to lock the private key but the mutex is unset.
	if (sk->mutex == NULL) {
		return OQS_ERROR;
	}

	return (sk->lock_key(sk->mutex));
}

/* OQS_SIG_STFL_SECRET_KEY_unlock */
OQS_STATUS OQS_SIG_STFL_SECRET_KEY_unlock(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return OQS_ERROR;
	}
	if (sk->unlock_key == NULL) {
		return OQS_SUCCESS;
	}

	// Try to unlock the private key but the mutex is unset.
	if (sk->mutex == NULL) {
		return OQS_ERROR;
	}

	return (sk->unlock_key(sk->mutex));
}
