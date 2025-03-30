#ifndef PLASMA_OPTION_H
#define PLASMA_OPTION_H

#if OPTIMISATION == 2
#include "snova_plasma_vec.h"
#include "snova_plasma_vec_gnl_sign.h"
#include "snova_plasma_vec_gnl_verify.h"
#elif OPTIMISATION == 1
#include "snova_opt.h"
#else
#error
#endif

// ---
#if OPTIMISATION >= 2
#define gen_P22 gen_P22_vtl
#define gen_F gen_F_vtl
#define sign_digest_core sign_digest_core_gnl_vtl
#define verify_core verify_signture_vtl
#define verify_pkx_core verify_signture_pkx_vtl
#elif OPTIMISATION == 1
#define gen_F gen_F_opt
#define gen_P22 gen_P22_opt
#define sign_digest_core sign_digest_core_opt
#define verify_core verify_signture_opt
#define verify_pkx_core verify_signture_pkx_opt
#endif

static void snova_plasma_init(void) {
	static int first_plasma_time = 1;
	if (first_plasma_time) {
		first_plasma_time = 0;
#if OPTIMISATION == 2
		init_vector_table();
#endif
	}
}

#endif
