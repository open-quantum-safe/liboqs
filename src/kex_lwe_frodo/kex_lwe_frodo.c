#include <stdlib.h>
#include <string.h>
#if !defined(_WIN32)
#include <strings.h>
#include <unistd.h>
#endif

#include <oqs/common.h>
#include <oqs/kex.h>
#include <oqs/rand.h>

#include "kex_lwe_frodo.h"
#include "local.h"

#define LWE_DIV_ROUNDUP(x, y) (((x) + (y) -1) / y)

#include <stdio.h>

// pre-process code to obtain "recommended" functions
#include "recommended.h"
#define MACRIFY(NAME) NAME##_recommended
#include "kex_lwe_frodo_macrify.c"
static size_t alice_priv_key_len = PARAMS_N * PARAMS_NBAR * sizeof(uint16_t);
// undefine macros to avoid any confusion later
#include "recommended.h"
#undef MACRIFY

void OQS_KEX_lwe_frodo_alice_priv_free(UNUSED OQS_KEX *k, void *alice_priv) {
	OQS_MEM_secure_free(alice_priv, alice_priv_key_len);
}

void OQS_KEX_lwe_frodo_free(OQS_KEX *k) {
	if (!k) {
		return;
	}
	if (k->params) {
		struct oqs_kex_lwe_frodo_params *params = (struct oqs_kex_lwe_frodo_params *) k->params;
		free(params->cdf_table);
		params->cdf_table = NULL;
		free(params->seed);
		params->seed = NULL;
		free(params->param_name);
		params->param_name = NULL;
		free(k->params);
		k->params = NULL;
	}
	free(k->named_parameters);
	k->named_parameters = NULL;
	free(k->method_name);
	k->method_name = NULL;
	free(k);
}
