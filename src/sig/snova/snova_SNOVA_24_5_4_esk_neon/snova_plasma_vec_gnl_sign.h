/**
 * CT version for any l_SNOVA.
 */

#ifndef PLASMA_GNL_VECTOR_SIGN_H
#define PLASMA_GNL_VECTOR_SIGN_H

#define GAUSS_ROW (m_SNOVA * lsq_SNOVA + 1)
#define GAUSS_ROW32 ((GAUSS_ROW + VEC_LENGTH - 1) / VEC_LENGTH)
#define GAUSS_ROW_mult32 (GAUSS_ROW32 * VEC_LENGTH)


void calc_LR_J_vtl(
    uint8_t L_J[m_SNOVA][alpha_SNOVA][rank][vtl_v_len * VEC_LENGTH],
    uint8_t R_tr_J[m_SNOVA][alpha_SNOVA][rank][vtl_v_len * VEC_LENGTH],
    Aalpha_t Aalpha,
    Balpha_t Balpha,
    Qalpha1_t Qalpha1,
    Qalpha2_t Qalpha2,
    gf16m_t *X_in_GF16Matrix) {

	alignas(VEC_LENGTH) VECTOR X_J[rank][vtl_v_len];
	alignas(VEC_LENGTH) VECTOR X_tr_J[rank][vtl_v_len];

	jogressMatrix_vector((VECTOR *)X_J, (uint8_t *)X_in_GF16Matrix, 1, v_SNOVA);
	jogressTrMatrix_vector((VECTOR *)X_tr_J, (uint8_t *)X_in_GF16Matrix, 1, v_SNOVA);

	// calc LR
	for (int mi = 0; mi < m_SNOVA; ++mi) {
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			int mi_prime = i_prime_inv(mi, alpha);

			alignas(VEC_LENGTH) VECTOR AxS_256[rank][vtl_v_len] = {0};
			alignas(VEC_LENGTH) VECTOR AxS_tr_256[rank][vtl_v_len] = {0};
			alignas(VEC_LENGTH) VECTOR Q2xS_256[rank][vtl_v_len] = {0};
			alignas(VEC_LENGTH) VECTOR Q2xS_tr_256[rank][vtl_v_len] = {0};
			alignas(VEC_LENGTH) VECTOR L_tr_J_256[rank][vtl_v_len] = {0};

			for (int ni = 0; ni < rank; ++ni) {
				for (int nj = 0; nj < rank; ++nj) {
					VECTOR k1 = mtk2_16[get_gf16m(Aalpha[mi_prime][alpha], ni, nj)];
					VECTOR k2 = mtk2_16[get_gf16m(Qalpha2[mi_prime][alpha], ni, nj)];
					for (int nk = 0; nk < vtl_v_len; ++nk) {
						AxS_256[ni][nk] ^= VEC_SHUFFLE(k1, X_tr_J[nj][nk]);
						Q2xS_256[ni][nk] ^= VEC_SHUFFLE(k2, X_J[nj][nk]);
					}
				}
			}
			jogressMatrixTr_vector((VECTOR *)AxS_tr_256, (VECTOR *)AxS_256, 1, v_SNOVA);
			jogressMatrixTr_vector((VECTOR *)Q2xS_tr_256, (VECTOR *)Q2xS_256, 1, v_SNOVA);

			for (int ni = 0; ni < rank; ++ni) {
				for (int nj = 0; nj < rank; ++nj) {
					VECTOR k1 = mtk2_16[get_gf16m(Qalpha1[mi_prime][alpha], ni, nj)];
					VECTOR k2 = mtk2_16[get_gf16m(Balpha[mi_prime][alpha], nj, ni)];
					for (int nk = 0; nk < vtl_v_len; ++nk) {
						VECTOR *R_tr_J_256 = (VECTOR *)R_tr_J[mi][alpha][ni];
						L_tr_J_256[ni][nk] ^= VEC_SHUFFLE(k1, AxS_tr_256[nj][nk]);
						R_tr_J_256[nk] ^= VEC_SHUFFLE(k2, Q2xS_tr_256[nj][nk]);
					}
				}
			}
			jogressMatrixTr_vector((VECTOR *)L_J[mi][alpha], (VECTOR *)L_tr_J_256, 1, v_SNOVA);
		}
	}
}

/**
 * Computes signature
 */
int sign_digest_core_gnl_vtl(uint8_t *pt_signature, const uint8_t *digest,
                             uint64_t bytes_digest, uint8_t *array_salt,
                             Aalpha_t Aalpha, Balpha_t Balpha, Qalpha1_t Qalpha1,
                             Qalpha2_t Qalpha2, T12_t T12, F11_t F11, F12_t F12,
                             F21_t F21, const uint8_t pt_public_key_seed[seed_length_public],
                             const uint8_t pt_private_key_seed[seed_length_private]) {
	alignas(VEC_LENGTH) uint8_t Gauss[m_SNOVA * lsq_SNOVA][GAUSS_ROW_mult32];

	gf16m_t X_in_GF16Matrix[n_SNOVA] = {0};
	gf16m_t Fvv_in_GF16Matrix[m_SNOVA];
	gf16_t hash_in_GF16[m_SNOVA * lsq_SNOVA];
	gf16m_t signature_in_GF16Matrix[n_SNOVA];

	uint8_t signed_hash[bytes_hash];
	uint8_t vinegar_in_byte[(v_SNOVA * lsq_SNOVA + 1) >> 1];
	int flag_redo = 1;
	uint8_t num_sign = 0;

	createSignedHash(digest, bytes_digest, pt_public_key_seed, array_salt, signed_hash);

	// put hash value in GF16 array
	convert_bytes_to_GF16s(signed_hash, hash_in_GF16, GF16s_hash);

	do {
		memset(Gauss, 0, sizeof(Gauss));
		num_sign++;
		if (num_sign == 255) {
			// Probability of getting here is about 2^{-1020}
			memset(pt_signature, 0, bytes_sig_with_salt);
			return -1;
		}
		flag_redo = 0;
		// put hash value in the last column of Gauss matrix
		for (int index = 0; index < (m_SNOVA * lsq_SNOVA); index++) {
			Gauss[index][m_SNOVA * lsq_SNOVA] = hash_in_GF16[index];
		}
		// generate the vinegar value
		Keccak_HashInstance hashInstance;
		Keccak_HashInitialize_SHAKE256(&hashInstance);
		Keccak_HashUpdate(&hashInstance, pt_private_key_seed, 8 * seed_length_private);
		Keccak_HashUpdate(&hashInstance, digest, 8 * bytes_digest);
		Keccak_HashUpdate(&hashInstance, array_salt, 8 * bytes_salt);
		Keccak_HashUpdate(&hashInstance, &num_sign, 8);
		Keccak_HashFinal(&hashInstance, NULL);
		Keccak_HashSqueeze(&hashInstance, vinegar_in_byte, 8 * ((v_SNOVA * lsq_SNOVA + 1) >> 1));

		convert_bytes_to_GF16s(vinegar_in_byte, (uint8_t *)X_in_GF16Matrix, v_SNOVA * lsq_SNOVA);

		alignas(VEC_LENGTH) uint8_t L_J[m_SNOVA][alpha_SNOVA][rank][vtl_v_len * VEC_LENGTH] = {0};
		alignas(VEC_LENGTH) uint8_t R_tr_J[m_SNOVA][alpha_SNOVA][rank][vtl_v_len * VEC_LENGTH] = {0};

		calc_LR_J_vtl(L_J, R_tr_J, Aalpha, Balpha, Qalpha1, Qalpha2, X_in_GF16Matrix);

		for (int mi = 0; mi < m_SNOVA; ++mi) {
			gf16m_set_zero(Fvv_in_GF16Matrix[mi]);
		}
		for (int mi = 0; mi < m_SNOVA; ++mi) {
			VECTOR F11_J_256[v_SNOVA * rank][vtl_v_len] = {0};
			jogressMatrix_vector((VECTOR *)F11_J_256, (uint8_t *)F11[mi], v_SNOVA, v_SNOVA);

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
				int mi_prime = i_prime_inv(mi, alpha);
				VECTOR LJxF11J_256[rank][vtl_v_len] = {0};
				for (int vi = 0; vi < rank; ++vi) {
					for (int vj = 0; vj < v_SNOVA * rank; ++vj) {
						VECTOR k = vtl_ct_multtab(L_J[mi][alpha][vi][vj]);
						for (int vk = 0; vk < vtl_v_len; ++vk) {
							LJxF11J_256[vi][vk] ^= VEC_SHUFFLE(k, F11_J_256[vj][vk]);
						}
					}
				}

				uint8_t LJxF11JxRJ[rank][rank] = {0};
				for (int vi = 0; vi < rank; ++vi) {
					for (int vj = 0; vj < rank; ++vj) {
						VECTOR *R_tr_J_256 = (VECTOR *)R_tr_J[mi][alpha][vj];
						VECTOR tmp_256 = {0};
						for (int vk = 0; vk < vtl_v_len; ++vk) {
							gf16_32_mul_32_add((LJxF11J_256[vi] + vk), (R_tr_J_256 + vk), (&tmp_256));
						}

						LJxF11JxRJ[vi][vj] ^= horizontal_xor_256(tmp_256);
					}
				}

				// hash_in_GF16Matrix[mi] += LJxPJxRJ
				for (int ni = 0; ni < rank; ++ni) {
					for (int nj = 0; nj < rank; ++nj) {
						gf16_t t = get_gf16m(Fvv_in_GF16Matrix[mi_prime], ni, nj);
						set_gf16m(Fvv_in_GF16Matrix[mi_prime], ni, nj, t ^ LJxF11JxRJ[ni][nj]);
					}
				}
			}
		}

		// add to the last column of Gauss matrix
		for (int i = 0; i < m_SNOVA; ++i) {
			for (int j = 0; j < rank; ++j) {
				for (int k = 0; k < rank; ++k) {
					int index1 = i * lsq_SNOVA + j * rank + k;
					int index2 = m_SNOVA * lsq_SNOVA;
					Gauss[index1][index2] = gf16_get_add(Gauss[index1][index2], get_gf16m(Fvv_in_GF16Matrix[i], j, k));
				}
			}
		}

		// compute the coefficients of Xo and put into Gauss matrix and compute
		// the coefficients of Xo^t and add into Gauss matrix
		alignas(VEC_LENGTH) uint8_t Temp1[o_SNOVA * l_SNOVA * lsq_SNOVA * vtl_mainRow_x_rank(o_SNOVA)] = {0};
		alignas(VEC_LENGTH) uint8_t Temp2[o_SNOVA * l_SNOVA * lsq_SNOVA * vtl_mainRow_x_rank(o_SNOVA)] = {0};

		for (int mi = 0; mi < m_SNOVA; ++mi) {
			gf16m_t F21_vo[v_SNOVA][o_SNOVA];
			VECTOR F12_J_256[v_SNOVA * rank][vtl_o_len] = {0};
			VECTOR F21_vo_tr_J_256[v_SNOVA * rank][vtl_o_len] = {0};

			// swap F21  v <-> o
			for (int oi = 0; oi < o_SNOVA; ++oi) {
				for (int vi = 0; vi < v_SNOVA; ++vi) {
					gf16m_clone(F21_vo[vi][oi], F21[mi][oi][vi]);
				}
			}

			jogressMatrix_vector((VECTOR *)F12_J_256, (uint8_t *)F12[mi], v_SNOVA, o_SNOVA);
			jogressTrMatrix_vector((VECTOR *)F21_vo_tr_J_256, (uint8_t *)F21_vo, v_SNOVA, o_SNOVA);

			for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
				int mi_prime_inv = i_prime_inv(mi, alpha);

				VECTOR LJxF12J_256[rank][vtl_o_len] = {0};

				for (int vi = 0; vi < rank; ++vi) {
					for (int vj = 0; vj < v_SNOVA * rank; ++vj) {
						VECTOR k = vtl_ct_multtab(L_J[mi][alpha][vi][vj]);
						for (int oi = 0; oi < vtl_o_len; ++oi) {
							LJxF12J_256[vi][oi] ^= VEC_SHUFFLE(k, F12_J_256[vj][oi]);
						}
					}
				}

				VECTOR LJxF12J_tr_256[rank][vtl_o_len] = {0};
				VECTOR LJxF12JxQJ_tr_256[rank][vtl_o_len] = {0};

				jogressMatrixTr_vector((VECTOR *)LJxF12J_tr_256, (VECTOR *)LJxF12J_256, 1, o_SNOVA);

				for (int ri = 0; ri < rank; ++ri) {
					for (int rj = 0; rj < rank; ++rj) {
						VECTOR k = mtk2_16[get_gf16m(Qalpha2[mi_prime_inv][alpha], rj, ri)];
						for (int oi = 0; oi < vtl_o_len; ++oi) {
							LJxF12JxQJ_tr_256[ri][oi] ^= VEC_SHUFFLE(k, LJxF12J_tr_256[rj][oi]);
						}
					}
				}

				for (int ti2 = 0; ti2 < l_SNOVA; ++ti2)
					for (int tj2 = 0; tj2 < l_SNOVA; ++tj2) {
						VECTOR k = mtk2_16[Balpha[mi_prime_inv][alpha][tj2 * rank + ti2]];
						VECTOR *Temp1_256 = (VECTOR *)Temp1;
						VECTOR *AJ_256 = (VECTOR *)LJxF12JxQJ_tr_256;

						for (int tj1 = 0; tj1 < l_SNOVA; tj1++)
							for (int toi = 0; toi < vtl_mainRow_x_rank32(o_SNOVA); toi++) {
								Temp1_256[((mi_prime_inv * lsq_SNOVA + ti2 * rank + tj2) * l_SNOVA + tj1) * vtl_mainRow_x_rank32(o_SNOVA) + toi] ^=
								    VEC_SHUFFLE(k, AJ_256[(tj1 * vtl_mainRow_x_rank32(o_SNOVA)) + toi]);
							}
					}

				// ------- ^^^ F12    F22 vvv -------
				VECTOR F21JxRJ_256[rank][vtl_o_len] = {0};
				VECTOR RTRJ_x_F21TRJ_256[rank][vtl_o_len] = {0};

				for (int vi = 0; vi < rank; ++vi) {
					for (int vj = 0; vj < v_SNOVA * rank; ++vj) {
						VECTOR k = vtl_ct_multtab(R_tr_J[mi][alpha][vi][vj]);
						for (int oi = 0; oi < vtl_o_len; ++oi) {
							RTRJ_x_F21TRJ_256[vi][oi] ^= VEC_SHUFFLE(k, F21_vo_tr_J_256[vj][oi]);
						}
					}
				}
				jogressMatrixTr_vector((VECTOR *)F21JxRJ_256, (VECTOR *)RTRJ_x_F21TRJ_256, 1, o_SNOVA);

				VECTOR Q1xF21JxRJ_256[rank][vtl_o_len] = {0};
				for (int ri = 0; ri < rank; ++ri) {
					for (int rj = 0; rj < rank; ++rj) {
						VECTOR k = mtk2_16[get_gf16m(Qalpha1[mi_prime_inv][alpha], ri, rj)];
						for (int oi = 0; oi < vtl_o_len; ++oi) {
							Q1xF21JxRJ_256[ri][oi] ^= VEC_SHUFFLE(k, F21JxRJ_256[rj][oi]);
						}
					}
				}

				for (int ti1 = 0; ti1 < l_SNOVA; ++ti1)
					for (int tj2 = 0; tj2 < l_SNOVA; ++tj2) {
						VECTOR k = mtk2_16[Aalpha[mi_prime_inv][alpha][ti1 * rank + tj2]];
						VECTOR *Temp2_256 = (VECTOR *)Temp2;
						VECTOR *AJ_256 = (VECTOR *)Q1xF21JxRJ_256;

						for (int tj1 = 0; tj1 < l_SNOVA; ++tj1)
							for (int toi = 0; toi < vtl_mainRow_x_rank32(o_SNOVA); toi++) {
								Temp2_256[((mi_prime_inv * lsq_SNOVA + ti1 * l_SNOVA + tj2) * l_SNOVA + tj1) * vtl_mainRow_x_rank32(o_SNOVA) + toi] ^=
								    VEC_SHUFFLE(k, AJ_256[(tj1 * vtl_mainRow_x_rank32(o_SNOVA)) + toi]);
							}
					}
			}
		}

		for (int mi_prime_inv = 0; mi_prime_inv < o_SNOVA; ++mi_prime_inv)
			for (int ti1 = 0; ti1 < l_SNOVA; ++ti1)
				for (int ti2 = 0; ti2 < l_SNOVA; ++ti2)
					for (int tj1 = 0; tj1 < l_SNOVA; ++tj1)
						for (int oi = 0; oi < o_SNOVA; ++oi)
							for (int tj2 = 0; tj2 < l_SNOVA; ++tj2)
								Gauss[mi_prime_inv * lsq_SNOVA + ti1 * rank + ti2][oi * lsq_SNOVA + tj1 * rank + tj2] ^=
								    Temp1[((mi_prime_inv * lsq_SNOVA + ti2 * rank + tj2) * l_SNOVA + tj1) * vtl_mainRow_x_rank(o_SNOVA) + (oi * rank + ti1)];

		for (int mi_prime_inv = 0; mi_prime_inv < o_SNOVA; ++mi_prime_inv)
			for (int oi = 0; oi < o_SNOVA; ++oi)
				for (int ti1 = 0; ti1 < l_SNOVA; ++ti1)
					for (int ti2 = 0; ti2 < l_SNOVA; ++ti2)
						for (int tj1 = 0; tj1 < l_SNOVA; ++tj1)
							for (int tj2 = 0; tj2 < l_SNOVA; ++tj2)
								Gauss[mi_prime_inv * lsq_SNOVA + ti1 * rank + ti2][oi * lsq_SNOVA + tj1 * rank + tj2] ^=
								    Temp2[((mi_prime_inv * lsq_SNOVA + ti1 * l_SNOVA + tj2) * l_SNOVA + tj1) * vtl_mainRow_x_rank(o_SNOVA) + (oi * rank + ti2)];

		// Gaussian elimination in constant time
		for (int mi2 = 0; mi2 < m_SNOVA * lsq_SNOVA; ++mi2) {
			int swap = ct_gf16_is_not_zero(Gauss[mi2][mi2]) - 1;
			for (int j2 = mi2 + 1; j2 < m_SNOVA * lsq_SNOVA; ++j2) {
#if __ARM_NEON
				int8x16_t swap256 = vld1q_dup_s8((int8_t *)&swap);
#else
				VECTOR swap256 = _mm256_set1_epi32(swap);
#endif
				VECTOR *gdest = (VECTOR *)&Gauss[mi2][0];
				VECTOR *gsource = (VECTOR *)&Gauss[j2][0];
				for (int k2 = 0; k2 < GAUSS_ROW32; ++k2) {
					gdest[k2] ^= gsource[k2] & swap256;
				}

				swap = ct_gf16_is_not_zero(Gauss[mi2][mi2]) - 1;
			}
			flag_redo |= swap;

			// Constant time GF16 inverse
			VECTOR res256[1];
			VECTOR g256[1];
#if __ARM_NEON
			g256[0] = vld1q_dup_u8(&Gauss[mi2][mi2]);
#else
			g256[0] = _mm256_set1_epi8(Gauss[mi2][mi2]);
#endif
			res256[0] = VEC_SHUFFLE(vector_inv_table, g256[0]);
			uint8_t t_GF16 = VEC_BYTE(res256, 0);

			int kstart = (mi2 / VEC_LENGTH) * VEC_LENGTH;
			for (int k = kstart; k < GAUSS_ROW_mult32; k += VEC_LENGTH) {
				gf16_32_mul_k((VECTOR *)(Gauss[mi2] + k), t_GF16, (VECTOR *)(Gauss[mi2] + k));
			}

			for (int j2 = mi2 + 1; j2 < m_SNOVA * lsq_SNOVA; ++j2) {
				t_GF16 = Gauss[j2][mi2];
				for (int k2 = kstart; k2 < GAUSS_ROW_mult32; k2 += VEC_LENGTH) {
					gf16_32_mul_k_add((VECTOR *)(Gauss[mi2] + k2), t_GF16, (VECTOR *)(Gauss[j2] + k2));
				}
			}
		}

		if (!flag_redo) {
			SNOVA_CLEAR(Fvv_in_GF16Matrix);
			SNOVA_CLEAR(R_tr_J);
			SNOVA_CLEAR(Temp1);
			SNOVA_CLEAR(Temp2);
		}
	} while (flag_redo);

	alignas(VEC_LENGTH) uint8_t solution[GAUSS_ROW_mult32] = {0};
	uint32_t xSolution[m_SNOVA * lsq_SNOVA] = {0};
	uint8_t t_GF16 = 0;
	uint8_t Gauss_last_col;

	for (int mil2 = m_SNOVA * lsq_SNOVA - 1; mil2 >= 0; --mil2) {
		Gauss_last_col = Gauss[mil2][m_SNOVA * lsq_SNOVA];
		VECTOR t_GF16_256 = {0};

		Gauss[mil2][m_SNOVA * lsq_SNOVA] = 0;

		int kstart = ((mil2 + 1) / VEC_LENGTH) * VEC_LENGTH;
		for (int k2 = kstart; k2 < GAUSS_ROW_mult32; k2 += VEC_LENGTH) {
			gf16_32_mul_32_add((VECTOR *)(&Gauss[mil2][k2]), (VECTOR *)(&solution[k2]), &t_GF16_256);
		}

		uint8_t *t_GF16_256_8_ptr = (uint8_t *)(&t_GF16_256);
		for (int k2 = 0; k2 < VEC_LENGTH; k2++) {
			t_GF16 ^= t_GF16_256_8_ptr[k2];
		}

		solution[mil2] = Gauss_last_col ^ t_GF16;
		t_GF16 = 0;
		snova_set_zero(&Gauss_last_col, sizeof(Gauss_last_col));
	}

	for (int mi2 = m_SNOVA * lsq_SNOVA - 1; mi2 >= 0; --mi2) {
		xSolution[mi2] = gf16_from_nibble(solution[mi2]);
	}

	uint32_t xT12[v_SNOVA][o_SNOVA][lsq_SNOVA] = {0};

	for (int dj = 0; dj < v_SNOVA; ++dj)
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int idx = 0; idx < lsq_SNOVA; ++idx) {
				xT12[dj][dk][idx] = gf16_from_nibble(T12[dj][dk][idx]);
			}

	// Establish Signature
	uint32_t xSig[lsq_SNOVA] = {0};
	for (int dj = 0; dj < v_SNOVA; ++dj) {
		for (int dk = 0; dk < o_SNOVA; ++dk)
			for (int i1 = 0; i1 < l_SNOVA; ++i1)
				for (int j1 = 0; j1 < l_SNOVA; ++j1)
					for (int k1 = 0; k1 < l_SNOVA; ++k1) {
						xSig[i1 * l_SNOVA + j1] ^= xT12[dj][dk][i1 * l_SNOVA + k1] * xSolution[dk * lsq_SNOVA + k1 * l_SNOVA + j1];
					}

		for (int idx = 0; idx < lsq_SNOVA; ++idx) {
			signature_in_GF16Matrix[dj][idx] = X_in_GF16Matrix[dj][idx] ^ gf16_to_nibble(xSig[idx]);
		}

		memset(xSig, 0, sizeof(xSig));
	}

	for (int index = 0; index < o_SNOVA; ++index)
		for (int idx = 0; idx < lsq_SNOVA; ++idx) {
			signature_in_GF16Matrix[v_SNOVA + index][idx] = solution[index * lsq_SNOVA + idx];
		}

	// output signature
	convert_GF16s_to_bytes(pt_signature, (gf16_t *)signature_in_GF16Matrix, n_SNOVA * lsq_SNOVA);
	for (int i1 = 0; i1 < bytes_salt; ++i1) {
		pt_signature[bytes_signature + i1] = array_salt[i1];
	}

	// Cleanup
	SNOVA_CLEAR(Gauss);
	SNOVA_CLEAR(X_in_GF16Matrix);
	SNOVA_CLEAR(hash_in_GF16);
	SNOVA_CLEAR(signature_in_GF16Matrix);
	SNOVA_CLEAR(signed_hash);
	SNOVA_CLEAR(vinegar_in_byte);

	SNOVA_CLEAR(xSolution);
	SNOVA_CLEAR(xT12);

	return 0;
}

#endif
