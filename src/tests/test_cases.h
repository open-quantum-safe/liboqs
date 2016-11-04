#ifndef TEST_CASES_H
#define TEST_CASES_H
#include <oqs/kex.h>

struct kex_testcase {
	enum OQS_KEX_alg_name alg_name;
	unsigned char *seed;
	size_t seed_len;
	char *named_parameters;
	char *id;
	int run;
};

/* Add new testcases here */
struct kex_testcase kex_testcases[] = {
	{ OQS_KEX_alg_rlwe_bcns15, NULL, 0, NULL, "rlwe_bcns15", 0 },
	{ OQS_KEX_alg_rlwe_newhope, NULL, 0, NULL, "rlwe_newhope", 0 },
	{ OQS_KEX_alg_lwe_frodo, (unsigned char *) "01234567890123456", 16, "recommended", "lwe_frodo_recommended", 0 },
};

#endif


