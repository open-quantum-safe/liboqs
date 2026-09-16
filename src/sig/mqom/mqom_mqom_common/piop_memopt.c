#include "piop.h"
#include "xof.h"
#include "piop_cache.h"
#include "benchmark.h"
#include "expand_mq.h"
#include "piop_memopt.h"
#include "piop_common.h"

static int ComputePz_multiple(uint32_t nb_reps, const field_ext_elt x0[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], field_ext_elt z0[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], field_ext_elt z1[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]) {
	int ret = -1;
	uint32_t i, j, e;
	ExpandEquations_ctx EEctx = { 0 };

	/* Only use rows for A_hat and b_hat to save memory */
	field_ext_elt A_hat_row[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	/* NOTE: we reuse the A_hat_row memory slot to save memory */
	field_ext_elt *b_hat_row = A_hat_row;

	field_ext_elt t0[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt t1[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt t1_x0[PIOP_NB_PARALLEL_REPETITIONS_SIGN];

	const field_ext_elt* x0_ptr[PIOP_NB_PARALLEL_REPETITIONS_SIGN];
	field_ext_elt* t0_ptr[PIOP_NB_PARALLEL_REPETITIONS_SIGN];

	field_ext_elt z_0i, z_1i;
	field_ext_elt t0_x;

	if(nb_reps > PIOP_NB_PARALLEL_REPETITIONS_SIGN) {
		ret = -1;
		goto err;
	}

	for (e = 0; e < nb_reps; e++) {
		x0_ptr[e] = x0[e];
	}

	/* Compute the equations expansion in a streaming way to save memory */
	ret = ExpandEquations_memopt_init(mseed_eq, &EEctx);
	ERR(ret, err);

	for (i = 0; i < MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i++) {
		for (j = 0; j < MQOM3_PARAM_MQ_N; j++) {
			__BENCHMARK_START__(BS_PIOP_EXPAND_MQ);
			/* Extract row from A_hat */
			ret = ExpandEquations_memopt_update(&EEctx, A_hat_row);
			ERR(ret, err);
			__BENCHMARK_STOP__(BS_PIOP_EXPAND_MQ);
			/* Compute t1, common to all tau repetitions */
			__BENCHMARK_START__(BS_PIOP_COMPUTE_T1);
			t1[j] = field_ext_base_vect_mult(A_hat_row, x, j + 1);
			__BENCHMARK_STOP__(BS_PIOP_COMPUTE_T1);
			/* Compute t0, different for each tau repetition */
			__BENCHMARK_START__(BS_PIOP_MAT_MUL_EXT);
			for (e = 0; e < nb_reps; e++) {
				t0_ptr[e] = &t0[e][j];
			}
			field_ext_vect_mult_multiple_public(t0_ptr, A_hat_row, x0_ptr, j + 1, nb_reps);
			__BENCHMARK_STOP__(BS_PIOP_MAT_MUL_EXT);
		}
		/* Finish t1 computation with b_hat_row */
		__BENCHMARK_START__(BS_PIOP_EXPAND_MQ);
		ret = ExpandEquations_memopt_update(&EEctx, b_hat_row);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_PIOP_EXPAND_MQ);
		__BENCHMARK_START__(BS_PIOP_COMPUTE_T1);
		field_ext_vect_add(t1, b_hat_row, t1, MQOM3_PARAM_MQ_N);
		__BENCHMARK_STOP__(BS_PIOP_COMPUTE_T1);
		/* Compute the rest */
		__BENCHMARK_START__(BS_PIOP_COMPUTE_PZI);
		for (e = 0; e < nb_reps; e++) {
			t1_x0[e] = field_ext_vect_mult(t1, x0[e], MQOM3_PARAM_MQ_N); /* t1^T x0[e] */
		}
		for (e = 0; e < nb_reps; e++) {
			t0_x = field_ext_base_vect_mult(t0[e], x, MQOM3_PARAM_MQ_N);   /* t0^T x[e] */
			field_ext_vect_add(&t0_x, &t1_x0[e], &z_1i, 1);
			field_ext_vect_pack(z_1i, z1[e], i);
		}
		for (e = 0; e < nb_reps; e++) {
			z_0i = field_ext_vect_mult(t0[e], x0[e], MQOM3_PARAM_MQ_N);
			field_ext_vect_pack(z_0i, z0[e], i);
		}
		__BENCHMARK_STOP__(BS_PIOP_COMPUTE_PZI);
	}

	ret = 0;
err:
	ExpandEquations_memopt_final(&EEctx);
	/* t0/t1/t1_x0/t0_x are derived directly from the secret witness x
	 * (e.g. t1 = A_i.x + b_i is a linear equation in x); z_0i/z_1i mix in
	 * the per-signature blinding x0. Not otherwise cleansed by the caller. */
	mqom_cleanse(t0, sizeof(t0));
	mqom_cleanse(t1, sizeof(t1));
	mqom_cleanse(t1_x0, sizeof(t1_x0));
	mqom_cleanse(&t0_x, sizeof(t0_x));
	mqom_cleanse(&z_0i, sizeof(z_0i));
	mqom_cleanse(&z_1i, sizeof(z_1i));
	return ret;
}

int ComputePAlpha_memopt(const uint8_t com[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], const field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;
	uint32_t i, z;
	field_ext_elt Gamma[MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];

	field_ext_elt z0[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], z1[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt tmp[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];

	for (e = 0; e < MQOM3_PARAM_TAU; e+=PIOP_NB_PARALLEL_REPETITIONS_SIGN) {
		uint32_t nb_reps = (e + PIOP_NB_PARALLEL_REPETITIONS_SIGN <= MQOM3_PARAM_TAU) ? PIOP_NB_PARALLEL_REPETITIONS_SIGN : MQOM3_PARAM_TAU - e;
		ret = ComputePz_multiple(nb_reps, &x0[e], x, mseed_eq, z0, z1);
		ERR(ret, err);
		for(z = 0; z < nb_reps; z++){
			/* Gamma^{[e+z]} = XOF_8(com[e+z])  - per-execution batching challenge (v3) */
			__BENCHMARK_START__(BS_PIOP_EXPAND_BATCHING_MAT);
			ret = ExpandBatchingChallenge_memopt(com[e+z], Gamma);
			ERR(ret, err);
			__BENCHMARK_STOP__(BS_PIOP_EXPAND_BATCHING_MAT);
			__BENCHMARK_START__(BS_PIOP_BATCH_AND_MASK);
			for (i = 0; i < MQOM3_PARAM_ETA; i++) {
				field_ext_vect_pack(
				    field_ext_vect_mult(Gamma[i], z0[z], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
				    tmp, i
				);
			}
			field_ext_vect_add(tmp, u0[e+z], alpha0[e+z], MQOM3_PARAM_ETA);
			for (i = 0; i < MQOM3_PARAM_ETA; i++) {
				field_ext_vect_pack(
				    field_ext_vect_mult(Gamma[i], z1[z], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
				    tmp, i
				);
			}
			field_ext_vect_add(tmp, u1[e+z], alpha1[e+z], MQOM3_PARAM_ETA);
			__BENCHMARK_STOP__(BS_PIOP_BATCH_AND_MASK);
		}
	}

	ret = 0;
err:
	/* z0/z1 (P_z evaluations) and tmp (Gamma-batched masked evaluation) are
	 * derived from the secret witness x via ComputePz_multiple(); wipe them. */
	mqom_cleanse(z0, sizeof(z0));
	mqom_cleanse(z1, sizeof(z1));
	mqom_cleanse(tmp, sizeof(tmp));
	return ret;
}

int ComputePAlpha_partial_memopt(uint32_t nb_reps, const uint8_t com[][MQOM3_PARAM_DIGEST_SIZE], const field_ext_elt x0[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt u0[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const field_ext_elt u1[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], field_ext_elt alpha0[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;
	uint32_t i;
	field_ext_elt Gamma[MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];

	if(nb_reps > PIOP_NB_PARALLEL_REPETITIONS_SIGN) {
		ret = -1;
		goto err;
	}

	field_ext_elt z0[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], z1[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt tmp[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	ret = ComputePz_multiple(nb_reps, x0, x, mseed_eq, z0, z1);
	ERR(ret, err);
	for (e = 0; e < nb_reps; e++) {
		/* Gamma^{[e]} = XOF_8(com[e])  - per-execution batching challenge (v3) */
		__BENCHMARK_START__(BS_PIOP_EXPAND_BATCHING_MAT);
		ret = ExpandBatchingChallenge_memopt(com[e], Gamma);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_PIOP_EXPAND_BATCHING_MAT);
		__BENCHMARK_START__(BS_PIOP_BATCH_AND_MASK);
		for (i = 0; i < MQOM3_PARAM_ETA; i++) {
			field_ext_vect_pack(
			    field_ext_vect_mult(Gamma[i], z0[e], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
			    tmp, i
			);
		}
		field_ext_vect_add(tmp, u0[e], alpha0[e], MQOM3_PARAM_ETA);
		for (i = 0; i < MQOM3_PARAM_ETA; i++) {
			field_ext_vect_pack(
			    field_ext_vect_mult(Gamma[i], z1[e], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
			    tmp, i
			);
		}
		field_ext_vect_add(tmp, u1[e], alpha1[e], MQOM3_PARAM_ETA);
		__BENCHMARK_STOP__(BS_PIOP_BATCH_AND_MASK);
	}

	ret = 0;
err:
	/* z0/z1 (P_z evaluations) and tmp (Gamma-batched masked evaluation) are
	 * derived from the secret witness x via ComputePz_multiple(); wipe them.
	 * (z0/z1/tmp are declared after the early nb_reps guard above, so this
	 * runs safely with zeroed-but-uninitialized data on that early-exit path.) */
	mqom_cleanse(z0, sizeof(z0));
	mqom_cleanse(z1, sizeof(z1));
	mqom_cleanse(tmp, sizeof(tmp));
	return ret;
}

/***************************************************************/
/***************************************************************/

static int ComputePzEval_multiple(uint32_t nb_reps, const field_ext_elt r[], const field_ext_elt v_x[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], const field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], field_ext_elt v_z[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]) {
	int ret = -1;
	uint32_t i, j, e;
	ExpandEquations_ctx EEctx = { 0 };

	/* Only use rows for A_hat and b_hat to save memory */
	field_ext_elt A_hat_row[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	/* NOTE: we reuse the A_hat_row memory slot to save memory */
	field_ext_elt *b_hat_row = A_hat_row;

	field_ext_elt v_t[PIOP_NB_PARALLEL_REPETITIONS_VERIFY][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt tmp[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt y_r2[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt v_zi;

	const field_ext_elt* vx_ptr[PIOP_NB_PARALLEL_REPETITIONS_VERIFY];
	field_ext_elt* vt_ptr[PIOP_NB_PARALLEL_REPETITIONS_VERIFY];

	if(nb_reps > PIOP_NB_PARALLEL_REPETITIONS_VERIFY) {
		ret = -1;
		goto err;
	}

	for (e = 0; e < nb_reps; e++) {
		vx_ptr[e] = v_x[e];
	}

	/* Compute the equations expansion in a streaming way to save memory */
	ret = ExpandEquations_memopt_init(mseed_eq, &EEctx);
	ERR(ret, err);

	for (i = 0; i < MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i++) {
		/* Perform operations row by row for A_hat */
		for (j = 0; j < MQOM3_PARAM_MQ_N; j++) {
			ret = ExpandEquations_memopt_update(&EEctx, A_hat_row);
			ERR(ret, err);
			for (e = 0; e < nb_reps; e++) {
				vt_ptr[e] = &v_t[e][j];
			}
			field_ext_vect_mult_multiple_public(vt_ptr, A_hat_row, vx_ptr, j + 1, nb_reps);
		}
		/* Generate and add b_hat row */
		ret = ExpandEquations_memopt_update(&EEctx, b_hat_row);
		ERR(ret, err);
		for (e = 0; e < nb_reps; e++) {
			field_ext_constant_vect_mult(r[e], b_hat_row, tmp, MQOM3_PARAM_MQ_N);
			field_ext_vect_add(v_t[e], tmp, v_t[e], MQOM3_PARAM_MQ_N);
			/* Compute v_{z,i} = P_{z,i}(r) = v_t^T v_x - y_i r^2 */
			v_zi = field_ext_vect_mult(v_t[e], v_x[e], MQOM3_PARAM_MQ_N);
			field_ext_vect_pack(v_zi, v_z[e], i);
		}
	}
	for (e = 0; e < nb_reps; e++) {
		field_ext_elt r2 = field_ext_mult(r[e], r[e]);
		field_ext_constant_vect_mult(r2, y, y_r2, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU);
		field_ext_vect_add(v_z[e], y_r2, v_z[e], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU);
	}

	ret = 0;
err:
	ExpandEquations_memopt_final(&EEctx);
	return ret;
}


int RecomputePAlpha_memopt(const uint8_t com[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], const field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;
	uint32_t i;
	field_ext_elt Gamma[MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];

	field_ext_elt v_z[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt v_alpha[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt r[MQOM3_PARAM_TAU];
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		r[e] = get_evaluation_point(i_star[e]);
	}
	for (e = 0; e < MQOM3_PARAM_TAU; e+=PIOP_NB_PARALLEL_REPETITIONS_VERIFY) {
		uint32_t nb_reps = (e + PIOP_NB_PARALLEL_REPETITIONS_VERIFY <= MQOM3_PARAM_TAU) ? PIOP_NB_PARALLEL_REPETITIONS_VERIFY : MQOM3_PARAM_TAU - e;
		ret = ComputePzEval_multiple(nb_reps, &r[e], &x_eval[e], mseed_eq, y, &v_z[e]);
		ERR(ret, err);
	}
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		/* Gamma^{[e]} = XOF_8(com[e])  - per-execution batching challenge (v3) */
		ret = ExpandBatchingChallenge_memopt(com[e], Gamma);
		ERR(ret, err);
		for (i = 0; i < MQOM3_PARAM_ETA; i++) {
			field_ext_vect_pack(
			    field_ext_vect_mult(Gamma[i], v_z[e], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
			    v_alpha, i
			);
		}
		field_ext_vect_add(v_alpha, u_eval[e], v_alpha, MQOM3_PARAM_ETA);
		field_ext_constant_vect_mult(r[e], alpha1[e], alpha0[e], MQOM3_PARAM_ETA);
		field_ext_vect_add(v_alpha, alpha0[e], alpha0[e], MQOM3_PARAM_ETA);
	}

	ret = 0;
err:
	return ret;
}

int RecomputePAlpha_partial_memopt(uint32_t nb_reps, const uint8_t com[][MQOM3_PARAM_DIGEST_SIZE], const field_ext_elt alpha1[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const uint16_t i_star[], const field_ext_elt x_eval[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt u_eval[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], const field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], field_ext_elt alpha0[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;
	uint32_t i;
	field_ext_elt Gamma[MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];

	if(nb_reps > PIOP_NB_PARALLEL_REPETITIONS_VERIFY) {
		ret = -1;
		goto err;
	}

	field_ext_elt v_z[PIOP_NB_PARALLEL_REPETITIONS_VERIFY][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt v_alpha[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt r[PIOP_NB_PARALLEL_REPETITIONS_VERIFY] = { 0 };
	for (e = 0; e < nb_reps; e++) {
		r[e] = get_evaluation_point(i_star[e]);
	}
	ret = ComputePzEval_multiple(nb_reps, r, x_eval, mseed_eq, y, v_z);
	ERR(ret, err);
	for (e = 0; e < nb_reps; e++) {
		/* Gamma^{[e]} = XOF_8(com[e])  - per-execution batching challenge (v3) */
		ret = ExpandBatchingChallenge_memopt(com[e], Gamma);
		ERR(ret, err);
		for (i = 0; i < MQOM3_PARAM_ETA; i++) {
			field_ext_vect_pack(
			    field_ext_vect_mult(Gamma[i], v_z[e], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
			    v_alpha, i
			);
		}
		field_ext_vect_add(v_alpha, u_eval[e], v_alpha, MQOM3_PARAM_ETA);
		field_ext_constant_vect_mult(r[e], alpha1[e], alpha0[e], MQOM3_PARAM_ETA);
		field_ext_vect_add(v_alpha, alpha0[e], alpha0[e], MQOM3_PARAM_ETA);
	}

	ret = 0;
err:
	return ret;
}
