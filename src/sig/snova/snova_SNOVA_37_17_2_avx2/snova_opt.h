/**
 * Optimized version in plain C. The compiler may use vectorized instructions if available.
 */

#ifndef SNOVA_OPT_H
#define SNOVA_OPT_H

/**
 * Generate private key (F part)
 */
void gen_F_opt(map_group2 *map2, map_group1 *map1, T12_t T12) {
	uint32_t xF11[m_SNOVA * lsq_SNOVA] = {0};
	uint32_t xT12[v_SNOVA * o_SNOVA * lsq_SNOVA] = {0};
	uint32_t xtemp0[m_SNOVA * o_SNOVA * l_SNOVA * l_SNOVA];
	uint32_t xtemp1[m_SNOVA * o_SNOVA * l_SNOVA * l_SNOVA];

	memcpy(map2->F11, map1->P11, m_SNOVA * v_SNOVA * v_SNOVA * lsq_SNOVA);
	memcpy(map2->F12, map1->P12, m_SNOVA * v_SNOVA * o_SNOVA * lsq_SNOVA);
	memcpy(map2->F21, map1->P21, m_SNOVA * o_SNOVA * v_SNOVA * lsq_SNOVA);

	// F12

	for (int dj = 0; dj < v_SNOVA; ++dj)
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					xT12[((dj * l_SNOVA + i1) * o_SNOVA + dk) * l_SNOVA + j1] =
					    gf16_from_nibble(T12[dj][dk][i1 * l_SNOVA + j1]);
				}

	for (int di = 0; di < v_SNOVA; di++) {
		// uint32_t xtemp[m_SNOVA * o_SNOVA * l_SNOVA * l_SNOVA] = {0};
		SNOVA_CLEAR(xtemp0);
		for (int dk = 0; dk < v_SNOVA; dk++) {
			for (int mi = 0; mi < m_SNOVA; mi++)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						xF11[j1 * m_SNOVA * l_SNOVA + mi * l_SNOVA + i1] =
						    gf16_from_nibble(map1->P11[mi][di][dk][i1 * l_SNOVA + j1]);

			for (int dj = 0; dj < o_SNOVA; ++dj)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int mi = 0; mi < m_SNOVA; ++mi)
						for (int i1 = 0; i1 < l_SNOVA; ++i1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xtemp0[(dj * l_SNOVA + j1) * m_SNOVA * l_SNOVA + mi * l_SNOVA + i1] ^=
								    xF11[k1 * m_SNOVA * l_SNOVA + mi * l_SNOVA + i1] *
								    xT12[(dk * l_SNOVA + k1) * o_SNOVA * l_SNOVA + (dj * l_SNOVA + j1)];
		}

		for (int mi = 0; mi < m_SNOVA; ++mi)
			for (int dj = 0; dj < o_SNOVA; ++dj)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						map2->F12[mi][di][dj][i1 * l_SNOVA + j1] ^=
						    gf16_to_nibble(xtemp0[(dj * l_SNOVA + j1) * m_SNOVA * l_SNOVA + mi * l_SNOVA + i1]);
	}

	// Repeat F21

	for (int dj = 0; dj < v_SNOVA; ++dj)
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					xT12[((dj * l_SNOVA + j1) * o_SNOVA + dk) * l_SNOVA + i1] =
					    gf16_from_nibble(T12[dj][dk][i1 * l_SNOVA + j1]);
				}

	for (int di = 0; di < v_SNOVA; di++) {
		// uint32_t xtemp[m_SNOVA * o_SNOVA * l_SNOVA * l_SNOVA] = {0};
		SNOVA_CLEAR(xtemp1);
		for (int dk = 0; dk < v_SNOVA; dk++) {
			for (int mi = 0; mi < m_SNOVA; mi++)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						xF11[i1 * m_SNOVA * l_SNOVA + mi * l_SNOVA + j1] =
						    gf16_from_nibble(map1->P11[mi][dk][di][i1 * l_SNOVA + j1]);

			for (int dj = 0; dj < o_SNOVA; ++dj)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int mi = 0; mi < m_SNOVA; ++mi)
						for (int i1 = 0; i1 < l_SNOVA; ++i1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xtemp1[(dj * l_SNOVA + j1) * m_SNOVA * l_SNOVA + mi * l_SNOVA + i1] ^=
								    xF11[k1 * m_SNOVA * l_SNOVA + mi * l_SNOVA + i1] *
								    xT12[(dk * l_SNOVA + k1) * o_SNOVA * l_SNOVA + (dj * l_SNOVA + j1)];
		}

		for (int mi = 0; mi < m_SNOVA; ++mi)
			for (int dj = 0; dj < o_SNOVA; ++dj)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						map2->F21[mi][dj][di][i1 * l_SNOVA + j1] ^=
						    gf16_to_nibble(xtemp1[(dj * l_SNOVA + i1) * m_SNOVA * l_SNOVA + mi * l_SNOVA + j1]);
	}

	SNOVA_CLEAR(xF11);
	SNOVA_CLEAR(xT12);
}

/**
 * Generate public key (P22 part)
 * @param outP22 - output
 * @param T12 - input
 * @param P21 - input
 * @param F12 - input
 */
void gen_P22_opt(P22_byte_t outP22, T12_t T12, P21_t P21, F12_t F12) {
	uint32_t xT12[v_SNOVA * o_SNOVA * lsq_SNOVA];
	uint32_t xF12[v_SNOVA * o_SNOVA * lsq_SNOVA];
	uint32_t xP21[o_SNOVA * v_SNOVA * lsq_SNOVA];
	P22_t P22 = {0};

	for (int di = 0; di < v_SNOVA; ++di)
		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					xT12[(di * o_SNOVA + dj) * lsq_SNOVA + i1 * l_SNOVA + j1] =
					    gf16_from_nibble(T12[di][dj][i1 * l_SNOVA + j1]);

	for (int mi = 0; mi < m_SNOVA; ++mi) {
		uint32_t xP22[o_SNOVA * o_SNOVA * lsq_SNOVA] = {0};

		for (int di = 0; di < v_SNOVA; ++di)
			for (int dk = 0; dk < o_SNOVA; ++dk)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						xF12[(di * o_SNOVA + dk) * lsq_SNOVA + i1 * l_SNOVA + j1] =
						    gf16_from_nibble(F12[mi][di][dk][i1 * l_SNOVA + j1]);

		for (int di = 0; di < v_SNOVA; ++di)
			for (int dj = 0; dj < o_SNOVA; ++dj)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						xP21[(dj * v_SNOVA + di) * lsq_SNOVA + i1 * l_SNOVA + j1] =
						    gf16_from_nibble(P21[mi][dj][di][i1 * l_SNOVA + j1]);

		for (int di = 0; di < v_SNOVA; ++di)
			for (int dj = 0; dj < o_SNOVA; ++dj)
				for (int dk = 0; dk < o_SNOVA; ++dk)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xP22[(dj * o_SNOVA + dk) * lsq_SNOVA + i1 * l_SNOVA + j1] ^=
								    xF12[(di * o_SNOVA + dk) * lsq_SNOVA + k1 * l_SNOVA + j1] *
								    xT12[(di * o_SNOVA + dj) * lsq_SNOVA + i1 * l_SNOVA + k1];

		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int di = 0; di < v_SNOVA; ++di)
				for (int dk = 0; dk < o_SNOVA; ++dk)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xP22[(dj * o_SNOVA + dk) * lsq_SNOVA + i1 * l_SNOVA + j1] ^=
								    xP21[(dj * v_SNOVA + di) * lsq_SNOVA + i1 * l_SNOVA + k1] *
								    xT12[(di * o_SNOVA + dk) * lsq_SNOVA + k1 * l_SNOVA + j1];

		for (int dj = 0; dj < o_SNOVA; ++dj)
			for (int dk = 0; dk < o_SNOVA; ++dk)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						P22[mi][dj][dk][i1 * l_SNOVA + j1] =
						    gf16_to_nibble(xP22[((dj * o_SNOVA + dk) * l_SNOVA + i1) * l_SNOVA + j1]);
	}

	convert_GF16s_to_bytes(outP22, (uint8_t *)P22, m_SNOVA * o_SNOVA * o_SNOVA * lsq_SNOVA);
}

/**
 * Computes signature
 */
int sign_digest_core_opt(uint8_t *pt_signature, const uint8_t *digest, uint64_t bytes_digest, uint8_t *array_salt,
                         Aalpha_t Aalpha, Balpha_t Balpha, Qalpha1_t Qalpha1, Qalpha2_t Qalpha2, T12_t T12, F11_t F11,
                         F12_t F12, F21_t F21, const uint8_t pt_public_key_seed[seed_length_public],
                         const uint8_t pt_private_key_seed[seed_length_private]) {
	uint8_t vinegar_gf16[n_SNOVA][lsq_SNOVA] = {0};
	uint32_t xVinegar_gf16[n_SNOVA][lsq_SNOVA] = {0};
	uint32_t temp_xgf16 = 0;

	uint32_t xSolution[m_SNOVA * lsq_SNOVA] = {0};

	uint8_t hash_in_GF16[m_SNOVA * lsq_SNOVA];
	uint8_t signature_in_GF16Matrix[n_SNOVA][lsq_SNOVA];
	uint8_t signed_hash[bytes_hash];

	int flag_redo = 1;
	uint8_t num_sign = 0;

	memset(pt_signature, 0, (bytes_signature + bytes_salt));

	createSignedHash(digest, bytes_digest, pt_public_key_seed, array_salt, signed_hash);
	convert_bytes_to_GF16s(signed_hash, hash_in_GF16, GF16s_hash);

	// Try to find a solution

	uint32_t xT12[v_SNOVA][o_SNOVA][lsq_SNOVA] = {0};
	uint32_t xGauss[m_SNOVA * lsq_SNOVA][m_SNOVA * lsq_SNOVA + 1] = {0};

	for (int dj = 0; dj < v_SNOVA; ++dj)
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int idx = 0; idx < lsq_SNOVA; ++idx) {
				xT12[dj][dk][idx] = gf16_from_nibble(T12[dj][dk][idx]);
			}

	uint32_t xAalpha[m_SNOVA][alpha_SNOVA * lsq_SNOVA] = {0};
	uint32_t xBalpha[m_SNOVA][alpha_SNOVA * lsq_SNOVA] = {0};
	uint32_t xQalpha1[m_SNOVA][alpha_SNOVA * lsq_SNOVA] = {0};
	uint32_t xQalpha2[m_SNOVA][alpha_SNOVA * lsq_SNOVA] = {0};

	for (int mi = 0; mi < m_SNOVA; ++mi)
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			int pj_prime = i_prime(mi, alpha);
			for (int idx = 0; idx < lsq_SNOVA; ++idx) {
				xQalpha1[pj_prime][alpha * lsq_SNOVA + idx] = gf16_from_nibble(Qalpha1[mi][alpha][idx]);
				xQalpha2[pj_prime][alpha * lsq_SNOVA + idx] = gf16_from_nibble(Qalpha2[mi][alpha][idx]);
				xAalpha[pj_prime][alpha * lsq_SNOVA + idx] = gf16_from_nibble(Aalpha[mi][alpha][idx]);
				xBalpha[pj_prime][alpha * lsq_SNOVA + idx] = gf16_from_nibble(Balpha[mi][alpha][idx]);
			}
		}

	do {
		// Prepare

		uint32_t xF11[v_SNOVA * l_SNOVA] = {0};
		uint32_t xF12[v_SNOVA * l_SNOVA] = {0};
		uint32_t xF21[v_SNOVA * l_SNOVA] = {0};

		uint8_t vinegar_in_byte[(v_SNOVA * lsq_SNOVA + 1) >> 1] = {0};

		uint32_t xLeft[alpha_SNOVA * v_SNOVA * l_SNOVA * l_SNOVA] = {0};
		uint32_t xRight[alpha_SNOVA * v_SNOVA * l_SNOVA * l_SNOVA] = {0};
		uint32_t xFvv_in_GF16Matrix[m_SNOVA][l_SNOVA][l_SNOVA] = {0};

		uint32_t xtemp_int[o_SNOVA * alpha_SNOVA * l_SNOVA * l_SNOVA];
		uint32_t xTemp[m_SNOVA][o_SNOVA][l_SNOVA][l_SNOVA][l_SNOVA][l_SNOVA] = {0};
		uint32_t xTemp_lr[o_SNOVA][alpha_SNOVA][lsq_SNOVA];

		num_sign++;
		if (num_sign == 255) {
			// Probability of getting here is about 2^{-1020}
			memset(pt_signature, 0, bytes_sig_with_salt);
			return -1;
		}
		flag_redo = 0;

		// generate the vinegar value
		Keccak_HashInstance hashInstance;
		Keccak_HashInitialize_SHAKE256(&hashInstance);
		Keccak_HashUpdate(&hashInstance, pt_private_key_seed, 8 * seed_length_private);
		Keccak_HashUpdate(&hashInstance, digest, 8 * bytes_digest);
		Keccak_HashUpdate(&hashInstance, array_salt, 8 * bytes_salt);
		Keccak_HashUpdate(&hashInstance, &num_sign, 8);
		Keccak_HashFinal(&hashInstance, NULL);
		Keccak_HashSqueeze(&hashInstance, vinegar_in_byte, 8 * ((v_SNOVA * lsq_SNOVA + 1) >> 1));

		convert_bytes_to_GF16s(vinegar_in_byte, (uint8_t *)vinegar_gf16, v_SNOVA * lsq_SNOVA);
		for (int jdx = 0; jdx < v_SNOVA; ++jdx)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					xVinegar_gf16[jdx][i1 * l_SNOVA + j1] = gf16_from_nibble(vinegar_gf16[jdx][i1 * l_SNOVA + j1]);
				}

		// evaluate the vinegar part of central map
		// 4 * V * L^5

		for (int mi = 0; mi < m_SNOVA; ++mi) {
			uint32_t xTemp_Q[alpha_SNOVA][v_SNOVA][lsq_SNOVA] = {0};

			memset(xtemp_int, 0, sizeof(xtemp_int));
			memset(xTemp_lr, 0, sizeof(xTemp_lr));

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
				for (int jdx = 0; jdx < v_SNOVA; ++jdx)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xTemp_Q[alpha][jdx][i1 * l_SNOVA + j1] ^=
								    xVinegar_gf16[jdx][k1 * l_SNOVA + i1] * xQalpha1[mi][alpha * lsq_SNOVA + k1 * l_SNOVA + j1];

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
				for (int jdx = 0; jdx < v_SNOVA; ++jdx)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1) {
							xTemp_Q[alpha][jdx][i1 * l_SNOVA + j1] &= 0x49249249;
						}

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
				for (int jdx = 0; jdx < v_SNOVA; ++jdx)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xLeft[alpha * l_SNOVA * v_SNOVA * l_SNOVA + i1 * v_SNOVA * l_SNOVA + jdx * l_SNOVA + j1] ^=
								    xAalpha[mi][alpha * lsq_SNOVA + i1 * l_SNOVA + k1] * xTemp_Q[alpha][jdx][k1 * l_SNOVA + j1];

			for (int idx = 0; idx < v_SNOVA * alpha_SNOVA * lsq_SNOVA; ++idx) {
				xLeft[idx] = gf16_reduce(xLeft[idx]);
			}

			// Same for right
			memset(xTemp_Q, 0, sizeof(xTemp_Q));

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
				for (int jdx = 0; jdx < v_SNOVA; ++jdx)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xTemp_Q[alpha][jdx][i1 * l_SNOVA + j1] ^=
								    xQalpha2[mi][alpha * lsq_SNOVA + i1 * l_SNOVA + k1] * xVinegar_gf16[jdx][k1 * l_SNOVA + j1];

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
				for (int jdx = 0; jdx < v_SNOVA; ++jdx)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1) {
							xTemp_Q[alpha][jdx][i1 * l_SNOVA + j1] &= 0x49249249;
						}

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
				for (int jdx = 0; jdx < v_SNOVA; ++jdx)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xRight[alpha * l_SNOVA * v_SNOVA * l_SNOVA + j1 * v_SNOVA * l_SNOVA + jdx * l_SNOVA + i1] ^=
								    xTemp_Q[alpha][jdx][i1 * l_SNOVA + k1] * xBalpha[mi][alpha * lsq_SNOVA + k1 * l_SNOVA + j1];

			for (int idx = 0; idx < v_SNOVA * alpha_SNOVA * lsq_SNOVA; ++idx) {
				xRight[idx] = gf16_reduce(xRight[idx]);
			}

			// Main multiplication

			// V^2 * O * L^5

			for (int kdx = 0; kdx < v_SNOVA; kdx++)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					uint32_t xtemp3[alpha_SNOVA * l_SNOVA] = {0};

					for (int jdx = 0; jdx < v_SNOVA; jdx++)
						for (int k1 = 0; k1 < l_SNOVA; ++k1) {
							xF11[jdx * l_SNOVA + k1] = gf16_from_nibble(F11[mi][jdx][kdx][k1 * l_SNOVA + j1]);
						}

					for (int alpha_i1 = 0; alpha_i1 < alpha_SNOVA * l_SNOVA; ++alpha_i1)
						for (int jdx_k1 = 0; jdx_k1 < v_SNOVA * l_SNOVA; ++jdx_k1) {
							xtemp3[alpha_i1] ^= xLeft[alpha_i1 * v_SNOVA * l_SNOVA + jdx_k1] * xF11[jdx_k1];
						}

					for (int idx = 0; idx < alpha_SNOVA * l_SNOVA; ++idx) {
						xtemp3[idx] &= 0x49249249;
					}

					for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
						for (int i1 = 0; i1 < l_SNOVA; ++i1)
							for (int j2 = 0; j2 < l_SNOVA; ++j2) {
								int mj = i_prime_inv(mi, alpha);
								xFvv_in_GF16Matrix[mj][i1][j2] ^=
								    xtemp3[alpha * l_SNOVA + i1] *
								    xRight[(alpha * l_SNOVA + j2) * v_SNOVA * l_SNOVA + kdx * l_SNOVA + j1];
							}
				}

			// compute the coefficients of Xo and put into Gauss matrix and compute
			// the coefficients of Xo^t and add into Gauss matrix
			//

			for (int kdx = 0; kdx < o_SNOVA; kdx++)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					for (int jdx = 0; jdx < v_SNOVA; jdx++)
						for (int k1 = 0; k1 < l_SNOVA; ++k1) {
							xF12[jdx * l_SNOVA + k1] = gf16_from_nibble(F12[mi][jdx][kdx][k1 * l_SNOVA + j1]);
						}

					for (int alpha_i1 = 0; alpha_i1 < alpha_SNOVA * l_SNOVA; ++alpha_i1)
						for (int jdk_k1 = 0; jdk_k1 < v_SNOVA * l_SNOVA; ++jdk_k1)
							xtemp_int[alpha_i1 * o_SNOVA * l_SNOVA + (0 * o_SNOVA + kdx) * l_SNOVA + j1] ^=
							    xLeft[alpha_i1 * v_SNOVA * l_SNOVA + jdk_k1] * xF12[jdk_k1];
				}

			for (int idx = 0; idx < alpha_SNOVA * l_SNOVA * o_SNOVA * l_SNOVA; ++idx) {
				xtemp_int[idx] = gf16_reduce(xtemp_int[idx]);
			}

			// Calculate Temp -> Gauss matrix
			// O^2 * L^5

			for (int kdx = 0; kdx < o_SNOVA; ++kdx)
				for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xTemp_lr[kdx][alpha][i1 * l_SNOVA + j1] ^=
								    xtemp_int[alpha * l_SNOVA * o_SNOVA * l_SNOVA + i1 * o_SNOVA * l_SNOVA +
								              0 * o_SNOVA * l_SNOVA + kdx * l_SNOVA + k1] *
								    xQalpha2[mi][alpha * lsq_SNOVA + k1 * l_SNOVA + j1];

			for (int kdx = 0; kdx < o_SNOVA; ++kdx)
				for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1) {
							xTemp_lr[kdx][alpha][i1 * l_SNOVA + j1] &= 0x49249249;
						}

			// Outer product
			// O^2 * L^6

			for (int kdx = 0; kdx < o_SNOVA; ++kdx)
				for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
					int mj = i_prime_inv(mi, alpha);
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int i2 = 0; i2 < l_SNOVA; ++i2)
								for (int j2 = 0; j2 < l_SNOVA; ++j2)
									xTemp[mj][kdx][i1][j2][i2][j1] ^= xTemp_lr[kdx][alpha][i1 * l_SNOVA + i2] *
									                                  xBalpha[mi][alpha * lsq_SNOVA + j2 * l_SNOVA + j1];
				}

			// Same for Right
			// O^2 * L^5
			memset(xtemp_int, 0, sizeof(xtemp_int));
			memset(xTemp_lr, 0, sizeof(xTemp_lr));

			for (int kdx = 0; kdx < o_SNOVA; kdx++)
				for (int i1 = 0; i1 < l_SNOVA; ++i1) {
					for (int jdx = 0; jdx < v_SNOVA; jdx++)
						for (int k1 = 0; k1 < l_SNOVA; ++k1) {
							xF21[jdx * l_SNOVA + k1] = gf16_from_nibble(F21[mi][kdx][jdx][i1 * l_SNOVA + k1]);
						}

					for (int alpha_j1 = 0; alpha_j1 < alpha_SNOVA * l_SNOVA; ++alpha_j1)
						for (int jdk_k1 = 0; jdk_k1 < v_SNOVA * l_SNOVA; ++jdk_k1)
							xtemp_int[alpha_j1 * o_SNOVA * l_SNOVA + (0 * o_SNOVA + kdx) * l_SNOVA + i1] ^=
							    xRight[alpha_j1 * v_SNOVA * l_SNOVA + jdk_k1] * xF21[jdk_k1];
				}

			for (int idx = 0; idx < alpha_SNOVA * l_SNOVA * o_SNOVA * l_SNOVA; ++idx) {
				xtemp_int[idx] = gf16_reduce(xtemp_int[idx]);
			}

			for (int kdx = 0; kdx < o_SNOVA; ++kdx)
				for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int k1 = 0; k1 < l_SNOVA; ++k1)
								xTemp_lr[kdx][alpha][i1 * l_SNOVA + j1] ^=
								    xtemp_int[alpha * l_SNOVA * o_SNOVA * l_SNOVA + j1 * o_SNOVA * l_SNOVA +
								              0 * o_SNOVA * l_SNOVA + kdx * l_SNOVA + k1] *
								    xQalpha1[mi][alpha * lsq_SNOVA + i1 * l_SNOVA + k1];

			for (int kdx = 0; kdx < o_SNOVA; ++kdx)
				for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1) {
							xTemp_lr[kdx][alpha][i1 * l_SNOVA + j1] &= 0x49249249;
						}

			// Outer product
			// O^2 * L^6

			for (int kdx = 0; kdx < o_SNOVA; ++kdx)
				for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
					int mj = i_prime_inv(mi, alpha);
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int j1 = 0; j1 < l_SNOVA; ++j1)
							for (int i2 = 0; i2 < l_SNOVA; ++i2)
								for (int j2 = 0; j2 < l_SNOVA; ++j2)
									xTemp[mj][kdx][i1][j2][i2][j1] ^= xAalpha[mi][alpha * lsq_SNOVA + i1 * l_SNOVA + j2] *
									                                  xTemp_lr[kdx][alpha][i2 * l_SNOVA + j1];
				}

			SNOVA_CLEAR(xLeft)
			SNOVA_CLEAR(xRight)
		}

		// Compose Gauss matrix
		// put hash value in the last column of Gauss matrix
		for (int index = 0; index < (m_SNOVA * lsq_SNOVA); index++) {
			xGauss[index][m_SNOVA * lsq_SNOVA] = gf16_from_nibble(hash_in_GF16[index]);
		}

		// Reorder xTemp
		for (int mi = 0; mi < m_SNOVA; ++mi)
			for (int kdx = 0; kdx < o_SNOVA; ++kdx)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						for (int i2 = 0; i2 < l_SNOVA; ++i2)
							for (int j2 = 0; j2 < l_SNOVA; ++j2) {
								xGauss[mi * lsq_SNOVA + i1 * l_SNOVA + j1][kdx * lsq_SNOVA + i2 * l_SNOVA + j2] =
								    gf16_reduce(xTemp[mi][kdx][i1][j2][i2][j1]);
							}

		// last column of Gauss matrix
		for (int mi = 0; mi < m_SNOVA; mi++)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1) {
					xGauss[mi * lsq_SNOVA + i1 * l_SNOVA + j1][m_SNOVA * lsq_SNOVA] ^=
					    gf16_reduce(xFvv_in_GF16Matrix[mi][i1][j1]);
				}

		// Gauss elimination in constant time
		for (int mi2 = 0; mi2 < m_SNOVA * lsq_SNOVA; ++mi2) {
			int swap = ct_xgf16_is_not_zero(xGauss[mi2][mi2]) - 1;
			for (int j2 = mi2 + 1; j2 < m_SNOVA * lsq_SNOVA; ++j2) {
				for (int k2 = mi2; k2 < m_SNOVA * lsq_SNOVA + 1; ++k2) {
					xGauss[mi2][k2] ^= xGauss[j2][k2] & swap;
				}
				swap = ct_xgf16_is_not_zero(xGauss[mi2][mi2]) - 1;
			}
			flag_redo |= swap;

			temp_xgf16 = gf16_inv(xGauss[mi2][mi2]);
			for (int k2 = mi2; k2 < m_SNOVA * lsq_SNOVA + 1; ++k2) {
				xGauss[mi2][k2] = gf16_reduce(xGauss[mi2][k2] * temp_xgf16);
			}

			for (int j2 = mi2 + 1; j2 < m_SNOVA * lsq_SNOVA; ++j2) {
				// Constant time version
				temp_xgf16 = ct_xgf16_is_not_zero(xGauss[j2][mi2]) * xGauss[j2][mi2];
				for (int k2 = mi2; k2 < m_SNOVA * lsq_SNOVA + 1; ++k2) {
					xGauss[j2][k2] = gf16_reduce(xGauss[j2][k2] ^ (xGauss[mi2][k2] * temp_xgf16));
				}
			}
		}

		// Cleanup
		if (!flag_redo) {
			SNOVA_CLEAR(xF11)
			SNOVA_CLEAR(xF12)
			SNOVA_CLEAR(xF21)
			SNOVA_CLEAR(vinegar_in_byte)
			SNOVA_CLEAR(xFvv_in_GF16Matrix)
			SNOVA_CLEAR(xtemp_int)
			SNOVA_CLEAR(xTemp)
			SNOVA_CLEAR(xTemp_lr)
		}
	} while (flag_redo);

	temp_xgf16 = 0;
	for (int mi2 = m_SNOVA * lsq_SNOVA - 1; mi2 >= 0; --mi2) {
		for (int k2 = mi2 + 1; k2 < m_SNOVA * lsq_SNOVA; ++k2) {
			temp_xgf16 ^= xGauss[mi2][k2] * xSolution[k2];
		}

		xSolution[mi2] = xGauss[mi2][m_SNOVA * lsq_SNOVA] ^ gf16_reduce(temp_xgf16);
		temp_xgf16 = 0;
	}

	for (int index = 0; index < o_SNOVA; ++index)
		for (int i1 = 0; i1 < l_SNOVA; ++i1)
			for (int j1 = 0; j1 < l_SNOVA; ++j1) {
				vinegar_gf16[index + v_SNOVA][i1 * l_SNOVA + j1] =
				    gf16_to_nibble(xSolution[index * lsq_SNOVA + i1 * l_SNOVA + j1]);
			}

	// Establish Signature

	uint32_t xSig[lsq_SNOVA] = {0};
	for (int dj = 0; dj < v_SNOVA; ++dj) {
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int k1 = 0; k1 < l_SNOVA; ++k1) {
						xSig[i1 * l_SNOVA + j1] ^=
						    xT12[dj][dk][i1 * l_SNOVA + k1] * xSolution[dk * lsq_SNOVA + k1 * l_SNOVA + j1];
					}

		for (int idx = 0; idx < lsq_SNOVA; ++idx) {
			signature_in_GF16Matrix[dj][idx] = vinegar_gf16[dj][idx] ^ gf16_to_nibble(xSig[idx]);
		}

		SNOVA_CLEAR(xSig)
	}

	for (int index = 0; index < o_SNOVA; ++index)
		for (int idx = 0; idx < lsq_SNOVA; ++idx) {
			signature_in_GF16Matrix[v_SNOVA + index][idx] = vinegar_gf16[v_SNOVA + index][idx];
		}

	// output signature
	convert_GF16s_to_bytes(pt_signature, (gf16_t *)signature_in_GF16Matrix, n_SNOVA * lsq_SNOVA);
	for (int i1 = 0; i1 < bytes_salt; ++i1) {
		pt_signature[bytes_signature + i1] = array_salt[i1];
	}

	// Cleanup
	SNOVA_CLEAR(vinegar_gf16)
	SNOVA_CLEAR(xVinegar_gf16)
	SNOVA_CLEAR(xSolution)
	SNOVA_CLEAR(hash_in_GF16)
	SNOVA_CLEAR(signature_in_GF16Matrix)
	SNOVA_CLEAR(signed_hash)
	SNOVA_CLEAR(xT12)
	SNOVA_CLEAR(xGauss)

	return 0;
}

/**
 * Verifies signature
 */
int verify_signture_opt_core(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature,
                             const public_key_expand *pkx) {
	uint8_t hash_in_bytes[bytes_hash];
	uint8_t signed_hash[bytes_hash];
	const uint8_t *pt_salt = pt_signature + bytes_signature;

	gf16m_t signature_in_GF16Matrix[n_SNOVA];
	uint32_t Xsignature_in_GF16Matrix[n_SNOVA][lsq_SNOVA];

	Keccak_HashInstance hashInstance;
	Keccak_HashInitialize_SHAKE256(&hashInstance);
	Keccak_HashUpdate(&hashInstance, pkx->pt_public_key_seed, 8 * seed_length_public);
	Keccak_HashUpdate(&hashInstance, pt_digest, 8 * bytes_digest);
	Keccak_HashUpdate(&hashInstance, pt_salt, 8 * bytes_salt);
	Keccak_HashFinal(&hashInstance, NULL);
	Keccak_HashSqueeze(&hashInstance, signed_hash, 8 * bytes_hash);

#if (o_SNOVA * l_SNOVA) & 0x1 == 1
	signed_hash[bytes_hash - 1] &= 0x0f;
#endif

	convert_bytes_to_GF16s(pt_signature, (gf16_t *)signature_in_GF16Matrix, GF16s_signature);
	// generate PRNG part of public key

	for (int jdx = 0; jdx < n_SNOVA; ++jdx)
		for (int i1 = 0; i1 < l_SNOVA; ++i1)
			for (int j1 = 0; j1 < l_SNOVA; ++j1) {
				Xsignature_in_GF16Matrix[jdx][i1 * l_SNOVA + j1] =
				    gf16_from_nibble(signature_in_GF16Matrix[jdx][i1 * l_SNOVA + j1]);
			}

	uint32_t res[m_SNOVA][lsq_SNOVA] = {0};

	// evaluate signature GF16Matrix array
	for (int mi = 0; mi < m_SNOVA; ++mi) {
		uint32_t xLeft[alpha_SNOVA][n_SNOVA][lsq_SNOVA] = {0};
		uint32_t xRight[alpha_SNOVA][n_SNOVA][lsq_SNOVA] = {0};

		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			uint32_t xAalpha[lsq_SNOVA];
			uint32_t xBalpha[lsq_SNOVA];
			uint32_t xQalpha1[lsq_SNOVA];
			uint32_t xQalpha2[lsq_SNOVA];

			int mi_prime = i_prime_inv(mi, alpha);
			for (int idx = 0; idx < lsq_SNOVA; ++idx) {
				xAalpha[idx] = gf16_from_nibble(pkx->map1.Aalpha[mi_prime][alpha][idx]);
				xBalpha[idx] = gf16_from_nibble(pkx->map1.Balpha[mi_prime][alpha][idx]);
				xQalpha1[idx] = gf16_from_nibble(pkx->map1.Qalpha1[mi_prime][alpha][idx]);
				xQalpha2[idx] = gf16_from_nibble(pkx->map1.Qalpha2[mi_prime][alpha][idx]);
			}

			uint32_t xTemp_Q[n_SNOVA][lsq_SNOVA];
			memset(xTemp_Q, 0, sizeof(xTemp_Q));

			// Left
			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						for (int k1 = 0; k1 < l_SNOVA; ++k1)
							xTemp_Q[jdx][i1 * l_SNOVA + j1] ^=
							    Xsignature_in_GF16Matrix[jdx][k1 * l_SNOVA + i1] * xQalpha1[k1 * l_SNOVA + j1];

			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1) {
						xTemp_Q[jdx][i1 * l_SNOVA + j1] &= 0x49249249;
					}

			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						for (int k1 = 0; k1 < l_SNOVA; ++k1)
							xLeft[alpha][jdx][i1 * l_SNOVA + j1] ^=
							    xAalpha[i1 * l_SNOVA + k1] * xTemp_Q[jdx][k1 * l_SNOVA + j1];

			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int idx = 0; idx < lsq_SNOVA; ++idx) {
					xLeft[alpha][jdx][idx] = gf16_reduce(xLeft[alpha][jdx][idx]);
				}

			// Right
			memset(xTemp_Q, 0, sizeof(xTemp_Q));

			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						for (int k1 = 0; k1 < l_SNOVA; ++k1)
							xTemp_Q[jdx][i1 * l_SNOVA + j1] ^=
							    xQalpha2[i1 * l_SNOVA + k1] * Xsignature_in_GF16Matrix[jdx][k1 * l_SNOVA + j1];

			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1) {
						xTemp_Q[jdx][i1 * l_SNOVA + j1] &= 0x49249249;
					}

			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int i1 = 0; i1 < l_SNOVA; ++i1)
					for (int j1 = 0; j1 < l_SNOVA; ++j1)
						for (int k1 = 0; k1 < l_SNOVA; ++k1)
							xRight[alpha][jdx][i1 * l_SNOVA + j1] ^=
							    xTemp_Q[jdx][i1 * l_SNOVA + k1] * xBalpha[k1 * l_SNOVA + j1];

			for (int jdx = 0; jdx < n_SNOVA; ++jdx)
				for (int idx = 0; idx < lsq_SNOVA; ++idx) {
					xRight[alpha][jdx][idx] = gf16_reduce(xRight[alpha][jdx][idx]);
				}
		}

#if l_SNOVA != 4

		// Main loop

		uint32_t Intermediate[alpha_SNOVA][lsq_SNOVA][n_SNOVA] = {0};

		for (int dj = 0; dj < n_SNOVA; ++dj) {
			uint32_t xPublic[lsq_SNOVA][n_SNOVA];

			if (dj < v_SNOVA) {
				for (int dk = 0; dk < v_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[idx][dk] = gf16_from_nibble(pkx->map1.P11[mi][dj][dk][idx]);
					}

				for (int dk = v_SNOVA; dk < n_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[idx][dk] = gf16_from_nibble(pkx->map1.P12[mi][dj][dk - v_SNOVA][idx]);
					}
			} else {
				for (int dk = 0; dk < v_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[idx][dk] = gf16_from_nibble(pkx->map1.P21[mi][dj - v_SNOVA][dk][idx]);
					}

				for (int dk = v_SNOVA; dk < n_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[idx][dk] = gf16_from_nibble(pkx->P22[mi][dj - v_SNOVA][dk - v_SNOVA][idx]);
					}
			}

			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
						for (int k1 = 0; k1 < l_SNOVA; ++k1)
							for (int dk = 0; dk < n_SNOVA; ++dk)
								Intermediate[alpha][i1 * l_SNOVA + j1][dk] ^=
								    xLeft[alpha][dj][i1 * l_SNOVA + k1] * xPublic[k1 * l_SNOVA + j1][dk];
		}

		// Reduce for next multiplication
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int dk = 0; dk < n_SNOVA; ++dk) {
						Intermediate[alpha][i1 * l_SNOVA + j1][dk] &= 0x49249249;
					}

		// Second loop
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			int mj = i_prime_inv(mi, alpha);
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int k1 = 0; k1 < l_SNOVA; ++k1)
						for (int dk = 0; dk < n_SNOVA; ++dk)
							res[mj][i1 * l_SNOVA + j1] ^=
							    Intermediate[alpha][i1 * l_SNOVA + k1][dk] * xRight[alpha][dk][k1 * l_SNOVA + j1];
		}

#else

		// Prepare for main loop

		uint32_t xPublic[n_SNOVA][lsq_SNOVA][n_SNOVA];
		for (int dj = 0; dj < n_SNOVA; ++dj) {
			if (dj < v_SNOVA) {
				for (int dk = 0; dk < v_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[dj][idx][dk] = gf16_from_nibble(pkx->map1.P11[mi][dj][dk][idx]);
					}

				for (int dk = v_SNOVA; dk < n_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[dj][idx][dk] = gf16_from_nibble(pkx->map1.P12[mi][dj][dk - v_SNOVA][idx]);
					}
			} else {
				for (int dk = 0; dk < v_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[dj][idx][dk] = gf16_from_nibble(pkx->map1.P21[mi][dj - v_SNOVA][dk][idx]);
					}

				for (int dk = v_SNOVA; dk < n_SNOVA; dk++)
					for (size_t idx = 0; idx < lsq_SNOVA; idx++) {
						xPublic[dj][idx][dk] = gf16_from_nibble(pkx->P22[mi][dj - v_SNOVA][dk - v_SNOVA][idx]);
					}
			}
		}

		// Main loop
		uint32_t Intermediate[alpha_SNOVA * lsq_SNOVA * n_SNOVA] = {0};

		for (int dj = 0; dj < n_SNOVA; ++dj)
			for (int dk = 0; dk < n_SNOVA; ++dk)
				for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
					for (int i1 = 0; i1 < l_SNOVA; ++i1)
						for (int k1 = 0; k1 < l_SNOVA; ++k1)
							for (int j1 = 0; j1 < l_SNOVA; ++j1)
								Intermediate[dk * alpha_SNOVA * lsq_SNOVA + alpha * lsq_SNOVA + i1 * l_SNOVA + j1] ^=
								    xLeft[alpha][dj][i1 * l_SNOVA + k1] * xPublic[dj][k1 * l_SNOVA + j1][dk];

		// Reduce for next multiplication
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int dk = 0; dk < n_SNOVA; ++dk) {
						Intermediate[dk * alpha_SNOVA * lsq_SNOVA + alpha * lsq_SNOVA + i1 * l_SNOVA + j1] &= 0x49249249;
					}

		// Second loop
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			int mj = i_prime_inv(mi, alpha);
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int k1 = 0; k1 < l_SNOVA; ++k1)
						for (int dk = 0; dk < n_SNOVA; ++dk)
							res[mj][i1 * l_SNOVA + k1] ^=
							    Intermediate[dk * alpha_SNOVA * lsq_SNOVA + alpha * lsq_SNOVA + i1 * l_SNOVA + j1] *
							    xRight[alpha][dk][j1 * l_SNOVA + k1];
		}
#endif
	}

	// Finish up
	for (int mi = 0; mi < m_SNOVA; ++mi)
		for (int i1 = 0; i1 < l_SNOVA; ++i1)
			for (int j1 = 0; j1 < l_SNOVA; ++j1) {
				((gf16_t *)signature_in_GF16Matrix)[mi * lsq_SNOVA + i1 * l_SNOVA + j1] =
				    gf16_to_nibble(res[mi][i1 * l_SNOVA + j1]);
			}

	convert_GF16s_to_bytes(hash_in_bytes, (gf16_t *)signature_in_GF16Matrix, m_SNOVA * lsq_SNOVA);

	int result = 0;
	for (int idx = 0; idx < bytes_hash; ++idx)
		if (hash_in_bytes[idx] != signed_hash[idx]) {
			result = -1;
			break;
		}

	return result;
}

int verify_signture_opt(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature, const uint8_t *pk) {
	public_key_expand pkx;
	expand_public_core(&pkx, pk);
	return verify_signture_opt_core(pt_digest, bytes_digest, pt_signature, &pkx);
}

int verify_signture_pkx_opt(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature,
                            const uint8_t *pkx_pck) {
	public_key_expand pkx_unpck;
	pkx_unpack(&pkx_unpck, (public_key_expand_pack *)pkx_pck);
	return verify_signture_opt_core(pt_digest, bytes_digest, pt_signature, &pkx_unpck);
}

#endif
