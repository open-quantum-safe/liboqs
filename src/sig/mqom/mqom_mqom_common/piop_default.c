#include "piop.h"
#include "xof.h"
#include "piop_cache.h"
#include "benchmark.h"
#include "expand_mq.h"
#include "piop_common.h"

/* Batch size for Gamma pre-computation: matches the available parallel XOF width. */
#if defined(USE_XOF_X8)
#define PIOP_GAMMA_BATCH 8
#elif defined(USE_XOF_X4)
#define PIOP_GAMMA_BATCH 4
#else
#define PIOP_GAMMA_BATCH 1
#endif

/* Some useful types definition */
/* NOTE: we use multi-dimensional array types to ease usage of indices.
 * While we can use pure local variables, these become too large to fit the stack
 * and heap allocation is needed. */
typedef field_ext_elt (*MatrixSetMQ)[MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
typedef field_ext_elt (*VectorSetMQ)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];

static inline void compute_t1(const field_ext_elt A_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt b_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt t1[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], uint32_t i, piop_cache *cache) {
	if (is_entry_active_piop_cache(cache, i)) {
		get_entry_piop_cache(cache, i, t1);
	} else {
		field_ext_base_mat_mult((field_ext_elt*)A_hat[i], x, t1, MQOM3_PARAM_MQ_N, TRI_INF);
		field_ext_vect_add(t1, b_hat[i], t1, MQOM3_PARAM_MQ_N);
		set_entry_piop_cache(cache, i, t1);
	}

	return;
}


static int ComputePz(const field_ext_elt x0_e[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt A_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt b_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt z0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], field_ext_elt z1[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], piop_cache *cache) {
	int ret = -1;
	uint32_t i;

	field_ext_elt t0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt t1[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt z_0i, z_1i;
	field_ext_elt t0_x, t1_x0;

	for (i = 0; i < MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i++) {
		/* Compute P_t(X) = t_0 + t_1 X = A_i P_x(X) + b_i X */
		__BENCHMARK_START__(BS_PIOP_MAT_MUL_EXT);
		field_ext_mat_mult((field_ext_elt*)A_hat[i], x0_e, t0, MQOM3_PARAM_MQ_N, TRI_INF);
		__BENCHMARK_STOP__(BS_PIOP_MAT_MUL_EXT);
		__BENCHMARK_START__(BS_PIOP_COMPUTE_T1);
		compute_t1(A_hat, x, b_hat, t1, i, cache);
		__BENCHMARK_STOP__(BS_PIOP_COMPUTE_T1);

		/* Compute P_{z,i}(X) = z_{0,i} + z_{1,i} X = P_t(X)^T P_x(X) - y_i X^2 */
		__BENCHMARK_START__(BS_PIOP_COMPUTE_PZI);
		z_0i = field_ext_vect_mult(t0, x0_e, MQOM3_PARAM_MQ_N);
		t1_x0 = field_ext_vect_mult(t1, x0_e, MQOM3_PARAM_MQ_N);      /* t1^T x0[e] */
		t0_x = field_ext_base_vect_mult(t0, x, MQOM3_PARAM_MQ_N);     /* t0^T x */
		field_ext_vect_add(&t0_x, &t1_x0, &z_1i, 1);
		field_ext_vect_pack(z_0i, z0, i);
		field_ext_vect_pack(z_1i, z1, i);
		__BENCHMARK_STOP__(BS_PIOP_COMPUTE_PZI);
	}

	ret = 0;
	/* t0/t1/t0_x/t1_x0 are values derived directly from the secret witness x
	 * (e.g. t1 = A_i.x + b_i is a linear equation in x); z_0i/z_1i mix in the
	 * per-signature blinding x0_e. None of this is otherwise cleansed by the
	 * caller, so wipe it here before returning. */
	mqom_cleanse(t0, sizeof(t0));
	mqom_cleanse(t1, sizeof(t1));
	mqom_cleanse(&t0_x, sizeof(t0_x));
	mqom_cleanse(&t1_x0, sizeof(t1_x0));
	mqom_cleanse(&z_0i, sizeof(z_0i));
	mqom_cleanse(&z_1i, sizeof(z_1i));
	return ret;
}

int ComputePAlpha_default(const uint8_t com[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], const field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;
	field_ext_elt *_A_hat = NULL;
	field_ext_elt *_b_hat = NULL;

	/* Initialize the PIOP cache for t1 (MQ_M / MU entries) */
	piop_cache *t1_cache = init_piop_cache(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU);

	/* Expand the public matrices */
	_A_hat = (field_ext_elt*)mqom_malloc((MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * MQOM3_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_A_hat == NULL) {
		ret = -1;
		goto err;
	}
	_b_hat = (field_ext_elt*)mqom_malloc((MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_b_hat == NULL) {
		ret = -1;
		goto err;
	}
	MatrixSetMQ A_hat = (MatrixSetMQ)_A_hat;
	VectorSetMQ b_hat = (VectorSetMQ)_b_hat;

	__BENCHMARK_START__(BS_PIOP_EXPAND_MQ);
	ret = ExpandEquations(mseed_eq, A_hat, b_hat);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_PIOP_EXPAND_MQ);

	uint32_t i, z, nb;
	field_ext_elt Gamma_batch[PIOP_GAMMA_BATCH][MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt z0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], z1[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt tmp[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	/* Gamma^{[e]} = XOF_8(com[e])  - per-execution batching challenge (v3) */
	for (e = 0; e < MQOM3_PARAM_TAU; e += PIOP_GAMMA_BATCH) {
		const uint8_t *com_group[PIOP_GAMMA_BATCH];
		nb = MQOM3_PARAM_TAU - e;
		if (nb > PIOP_GAMMA_BATCH) { nb = PIOP_GAMMA_BATCH; }
		for (z = 0; z < nb; z++) { com_group[z] = com[e + z]; }
		__BENCHMARK_START__(BS_PIOP_EXPAND_BATCHING_MAT);
		ret = ExpandBatchingChallenge_batch(nb, com_group, Gamma_batch);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_PIOP_EXPAND_BATCHING_MAT);
		for (z = 0; z < nb; z++) {
			ret = ComputePz(x0[e + z], x, (const field_ext_elt (*)[MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])A_hat, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])b_hat, z0, z1, t1_cache);
			ERR(ret, err);
			__BENCHMARK_START__(BS_PIOP_BATCH_AND_MASK);
			for (i = 0; i < MQOM3_PARAM_ETA; i++) {
				field_ext_vect_pack(
				    field_ext_vect_mult(Gamma_batch[z][i], z0, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
				    tmp, i
				);
			}
			field_ext_vect_add(tmp, u0[e + z], alpha0[e + z], MQOM3_PARAM_ETA);
			for (i = 0; i < MQOM3_PARAM_ETA; i++) {
				field_ext_vect_pack(
				    field_ext_vect_mult(Gamma_batch[z][i], z1, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
				    tmp, i
				);
			}
			field_ext_vect_add(tmp, u1[e + z], alpha1[e + z], MQOM3_PARAM_ETA);
			__BENCHMARK_STOP__(BS_PIOP_BATCH_AND_MASK);
		}
	}

	ret = 0;
err:
	destroy_piop_cache(t1_cache);
	if (_A_hat) {
		mqom_free(_A_hat, (MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * MQOM3_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	if (_b_hat) {
		mqom_free(_b_hat, (MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	/* z0/z1 (P_z evaluations) and tmp (Gamma-batched masked evaluation) are
	 * derived from the secret witness x via ComputePz(); wipe them here. */
	mqom_cleanse(z0, sizeof(z0));
	mqom_cleanse(z1, sizeof(z1));
	mqom_cleanse(tmp, sizeof(tmp));
	return ret;
}

/***************************************************************/
/***************************************************************/

static int ComputePzEval(field_ext_elt r, const field_ext_elt v_x[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt A_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt b_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], field_ext_elt v_z[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]) {
	int ret = -1;
	uint32_t i;

	field_ext_elt v_t[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt tmp[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt v_zi;

	field_ext_elt r2 = field_ext_mult(r, r);
	field_ext_elt y_r2[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_constant_vect_mult(r2, y, y_r2, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU);

	for (i = 0; i < MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i++) {
		/* Compute v_t = P_t(r) = A_i P_x(r) + b_i r */
		field_ext_mat_mult((field_ext_elt*)A_hat[i], v_x, tmp, MQOM3_PARAM_MQ_N, TRI_INF);
		field_ext_constant_vect_mult(r, b_hat[i], v_t, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(v_t, tmp, v_t, MQOM3_PARAM_MQ_N);

		/* Compute v_{z,i} = P_{z,i}(r) = v_t^T v_r - y_i r^2 */
		v_zi = field_ext_vect_mult(v_t, v_x, MQOM3_PARAM_MQ_N);
		field_ext_vect_pack(v_zi, v_z, i);
	}
	field_ext_vect_add(v_z, y_r2, v_z, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU);

	ret = 0;
	return ret;
}

int RecomputePAlpha_default(const uint8_t com[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], const field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], const field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;
	uint32_t e;
	field_ext_elt *_A_hat = NULL;
	field_ext_elt *_b_hat = NULL;

	/* Expand the public matrices */
	_A_hat = (field_ext_elt*)mqom_malloc((MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * MQOM3_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_A_hat == NULL) {
		ret = -1;
		goto err;
	}
	_b_hat = (field_ext_elt*)mqom_malloc((MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	if (_b_hat == NULL) {
		ret = -1;
		goto err;
	}
	MatrixSetMQ A_hat = (MatrixSetMQ)_A_hat;
	VectorSetMQ b_hat = (VectorSetMQ)_b_hat;

	ret = ExpandEquations(mseed_eq, A_hat, b_hat);
	ERR(ret, err);

	uint32_t i, z, nb;
	field_ext_elt Gamma_batch[PIOP_GAMMA_BATCH][MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt v_z[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt v_alpha[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	/* Gamma^{[e]} = XOF_8(com[e])  - per-execution batching challenge (v3) */
	for (e = 0; e < MQOM3_PARAM_TAU; e += PIOP_GAMMA_BATCH) {
		const uint8_t *com_group[PIOP_GAMMA_BATCH];
		nb = MQOM3_PARAM_TAU - e;
		if (nb > PIOP_GAMMA_BATCH) { nb = PIOP_GAMMA_BATCH; }
		for (z = 0; z < nb; z++) { com_group[z] = com[e + z]; }
		ret = ExpandBatchingChallenge_batch(nb, com_group, Gamma_batch);
		ERR(ret, err);
		for (z = 0; z < nb; z++) {
			field_ext_elt r = get_evaluation_point(i_star[e + z]);
			ret = ComputePzEval(r, x_eval[e + z], (const field_ext_elt (*)[MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])A_hat, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])b_hat, y, v_z);
			ERR(ret, err);
			for (i = 0; i < MQOM3_PARAM_ETA; i++) {
				field_ext_vect_pack(
				    field_ext_vect_mult(Gamma_batch[z][i], v_z, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU),
				    v_alpha, i
				);
			}
			field_ext_vect_add(v_alpha, u_eval[e + z], v_alpha, MQOM3_PARAM_ETA);
			field_ext_constant_vect_mult(r, alpha1[e + z], alpha0[e + z], MQOM3_PARAM_ETA);
			field_ext_vect_add(v_alpha, alpha0[e + z], alpha0[e + z], MQOM3_PARAM_ETA);
		}
	}

	ret = 0;
err:
	if (_A_hat) {
		mqom_free(_A_hat, (MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * MQOM3_PARAM_MQ_N * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	if (_b_hat) {
		mqom_free(_b_hat, (MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) * FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
	}
	return ret;
}
