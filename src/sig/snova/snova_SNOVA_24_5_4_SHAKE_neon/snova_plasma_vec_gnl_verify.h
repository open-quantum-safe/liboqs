/**
 * VTL version for any l_SNOVA.
 */

#ifndef PLASMA_GNL_VECTOR_VERIFY_H
#define PLASMA_GNL_VECTOR_VERIFY_H

static inline void evaluation_vector_vtl(
    gf16m_t *restrict hash_in_GF16Matrix,
    const public_key_expand *restrict pkx,
    gf16m_t *restrict signature_in_GF16Matrix
) {

	VECTOR S_J_256[rank][vtl_len];
	VECTOR S_tr_J_256[rank][vtl_len];
	VECTOR L_J_nibble256[m_SNOVA * alpha_SNOVA * rank_next32 * vtl_len] = {0};
	VECTOR R_tr_J_256[m_SNOVA][alpha_SNOVA][rank][vtl_len] = {0};

	jogressMatrix_vector((VECTOR *)S_J_256, (uint8_t *)signature_in_GF16Matrix, 1, n_SNOVA);
	jogressTrMatrix_vector((VECTOR *)S_tr_J_256, (uint8_t *)signature_in_GF16Matrix, 1, n_SNOVA);

	// calc LR
	for (int mi = 0; mi < m_SNOVA; ++mi) {
		VECTOR AxS_tr_256[alpha_SNOVA_next2][rank][vtl_len] = {0};
		VECTOR Q2xS_tr_256[alpha_SNOVA_next2][rank][vtl_len] = {0};

		// A and Q2, vtl 2 alpha set end 0
		gf16m_t Aalpha[alpha_SNOVA_next2] = {0};
		gf16m_t Qalpha2[alpha_SNOVA_next2] = {0};
		memcpy((uint8_t *)Aalpha, (uint8_t *)(pkx->map1.Aalpha[mi]), sizeof(pkx->map1.Aalpha[mi]));
		memcpy((uint8_t *)Qalpha2, (uint8_t *)(pkx->map1.Qalpha2[mi]), sizeof(pkx->map1.Qalpha2[mi]));

		for (int alpha = 0; alpha < alpha_SNOVA_next2; alpha += 2) {
			VECTOR AxS_256[rank][vtl_len] = {0};
			VECTOR Q2xS_256[rank][vtl_len] = {0};

			// use vtl (2 alpha x vtl_len)
			for (int ni = 0; ni < rank; ++ni) {
				for (int nj = 0; nj < rank; ++nj) {
					VECTOR k1_lh = mtk2_16[get_gf16m(Aalpha[alpha], ni, nj) | (get_gf16m(Aalpha[alpha + 1], ni, nj) << 4)];
					VECTOR k2_lh = mtk2_16[get_gf16m(Qalpha2[alpha], ni, nj) | (get_gf16m(Qalpha2[alpha + 1], ni, nj) << 4)];
					for (int nk = 0; nk < vtl_len; ++nk) {
						AxS_256[ni][nk] ^= VEC_SHUFFLE(k1_lh, S_tr_J_256[nj][nk]);
						Q2xS_256[ni][nk] ^= VEC_SHUFFLE(k2_lh, S_J_256[nj][nk]);
					}
				}
			}

			jogressMatrixTr_vector((VECTOR *)AxS_tr_256[alpha], (VECTOR *)AxS_256, 1, n_SNOVA);
			jogressMatrixTr_vector((VECTOR *)Q2xS_tr_256[alpha], (VECTOR *)Q2xS_256, 1, n_SNOVA);

			// nibble splite
			for (int ni = 0; ni < rank; ++ni) {
				for (int nk = 0; nk < vtl_len; ++nk) {
					AxS_tr_256[alpha + 1][ni][nk] = (AxS_tr_256[alpha][ni][nk] >> 4) & l_mask;
					AxS_tr_256[alpha][ni][nk] &= l_mask;

					Q2xS_tr_256[alpha + 1][ni][nk] = (Q2xS_tr_256[alpha][ni][nk] >> 4) & l_mask;
					Q2xS_tr_256[alpha][ni][nk] &= l_mask;
				}
			}
		}

		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			int mj = i_prime(mi, alpha);
			VECTOR L_tr_J_256[rank][vtl_len] = {0};
			// use vtl (2 ni x vtl_len)  **PS. If rank is odd, the last value is handled separately.**
			for (int ni = 0; ni < rank_floor2; ni += 2) {
				for (int nj = 0; nj < rank; ++nj) {
					VECTOR k1_lh = mtk2_16[get_gf16m(pkx->map1.Qalpha1[mi][alpha], ni, nj) ^ (get_gf16m(pkx->map1.Qalpha1[mi][alpha], ni + 1, nj) << 4)];
					VECTOR k2_lh = mtk2_16[get_gf16m(pkx->map1.Balpha[mi][alpha], nj, ni) ^ (get_gf16m(pkx->map1.Balpha[mi][alpha], nj, ni + 1) << 4)];
					for (int nk = 0; nk < vtl_len; ++nk) {
						L_tr_J_256[ni][nk] ^= VEC_SHUFFLE(k1_lh, AxS_tr_256[alpha][nj][nk]);
						R_tr_J_256[mj][alpha][ni][nk] ^= VEC_SHUFFLE(k2_lh, Q2xS_tr_256[alpha][nj][nk]);
					}
				}
			}

#if rank % 2    //  rank is odd, use vl no vtl(vectorized look-up), 
			for (int nj = 0; nj < rank; ++nj) {
				VECTOR k1_lh = mtk2_16[get_gf16m(pkx->map1.Qalpha1[mi][alpha], rank_floor2, nj)];
				VECTOR k2_lh = mtk2_16[get_gf16m(pkx->map1.Balpha[mi][alpha], nj, rank_floor2)];
				for (int nk = 0; nk < vtl_len; ++nk) {
					L_tr_J_256[rank_floor2][nk] ^= VEC_SHUFFLE(k1_lh, AxS_tr_256[alpha][nj][nk]);
					R_tr_J_256[mj][alpha][rank_floor2][nk] ^= VEC_SHUFFLE(k2_lh, Q2xS_tr_256[alpha][nj][nk]);
				}
			}
#endif

			// nibble splite
			for (int ni = 0; ni < rank_floor2; ni += 2) {
				for (int nk = 0; nk < vtl_len; ++nk) {
					L_tr_J_256[ni + 1][nk] = (L_tr_J_256[ni][nk] >> 4) & l_mask;
					L_tr_J_256[ni][nk] &= l_mask;

					R_tr_J_256[mj][alpha][ni + 1][nk] = (R_tr_J_256[mj][alpha][ni][nk] >> 4) & l_mask;
					R_tr_J_256[mj][alpha][ni][nk] &= l_mask;
				}
			}

			alignas(VEC_LENGTH) uint8_t L_J[rank_next2][vtl_len * VEC_LENGTH] = {0};
			jogressMatrixTr_vector((VECTOR *)L_J, (VECTOR *)L_tr_J_256, 1, n_SNOVA);

			// nibble L_J
			for (int ni = 0; ni < rank; ni += 2) {
				VECTOR *L_J_l_256 = (VECTOR *)L_J[ni];
				VECTOR *L_J_h_256 = (VECTOR *)L_J[ni + 1];
				VECTOR *L_J_256 = L_J_nibble256 + ((mj * alpha_SNOVA + alpha) * rank_next32 + ni / 2) * vtl_len;
				for (int nk = 0; nk < vtl_len; ++nk) {
					L_J_256[nk] = L_J_l_256[nk] ^ (L_J_h_256[nk] << 4);
				}
			}
		}
	}

	VECTOR h_256[m_SNOVA][rank][rank] = {0};
	for (int mi = 0; mi < m_SNOVA; ++mi) {

		// clac P
		gf16m_t P[n_SNOVA][n_SNOVA] = {0};
		VECTOR P_J_256[n_SNOVA * rank][vtl_len] = {0};
		/*
		         V        O
		     +--------+--------+
		     |        |        |
		   V |  P11   |  P12   |
		     |        |        |
		     +--------+--------+   = P[n_SNOVA][n_SNOVA]
		     |        |        |
		   O |  P21   |  P22   |
		     |        |        |
		     +--------+--------+
		*/
		for (int ni = 0; ni < v_SNOVA; ++ni) {
			memcpy((uint8_t *)P[ni], (uint8_t *)pkx->map1.P11[mi][ni], sizeof(pkx->map1.P11[mi][ni]));
			memcpy((uint8_t *)(P[ni] + v_SNOVA), (uint8_t *)pkx->map1.P12[mi][ni], sizeof(pkx->map1.P11[mi][ni]));

		}
		for (int ni = v_SNOVA; ni < n_SNOVA; ++ni) {
			memcpy((uint8_t *)P[ni], (uint8_t *)pkx->map1.P21[mi][ni - v_SNOVA], sizeof(pkx->map1.P21[mi][ni - v_SNOVA]));
			memcpy((uint8_t *)(P[ni] + v_SNOVA), (uint8_t *)pkx->P22[mi][ni - v_SNOVA], sizeof(pkx->P22[mi][ni - v_SNOVA]));
		}
		jogressMatrix_vector((VECTOR *)P_J_256, (uint8_t *)P, n_SNOVA, n_SNOVA);

		// evaluation start!!
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			int mi_prime = i_prime_inv(mi, alpha);
			VECTOR LJxPJ_256[rank][vtl_len] = {0};
			VECTOR LJxPJ_256_nibble[rank_next32][vtl_len] = {0};

			// LJ x PJ, Main loop (n_SNOVA^2 * rank^3 times)
			for (int nk = 0; nk < n_SNOVA * rank; ++nk) {
				for (int ni = 0; ni < rank_next32; ++ni) {
					VECTOR k_lh = mtk2_16[VEC_BYTE(L_J_nibble256, ((mi * alpha_SNOVA + alpha) * rank_next32 + ni) * vtl_len * VEC_LENGTH + nk)];
					for (int nj = 0; nj < vtl_len; ++nj) {
						LJxPJ_256_nibble[ni][nj] ^= VEC_SHUFFLE(k_lh, P_J_256[nk][nj]);
					}
				}
			}

			// nibble splite, **PS. If rank is odd, the last value is handled separately.**
			for (int ni = 0; ni < rank_floor2; ni += 2) {
				VECTOR *LJxPJ_256_nibble_l = LJxPJ_256[ni];
				VECTOR *LJxPJ_256_nibble_h = LJxPJ_256[ni + 1];
				for (int nj = 0; nj < vtl_len; ++nj) {
					LJxPJ_256_nibble_l[nj] = LJxPJ_256_nibble[ni / 2][nj] & l_mask;
					LJxPJ_256_nibble_h[nj] = ((LJxPJ_256_nibble[ni / 2][nj] >> 4) & l_mask);
				}
			}

#if rank % 2 // rank is odd
			for (int nj = 0; nj < vtl_len; ++nj) {
				LJxPJ_256[rank_floor2][nj] = LJxPJ_256_nibble[rank_floor2 / 2][nj];
			}
#endif

			// (LJ x PJ) x RJ, Secondary loop (n_SNOVA * rank^3 times)
			for (int nk = 0; nk < vtl_len; ++nk) {
				for (int ni = 0; ni < rank; ++ni) {
					for (int nj = 0; nj < rank; ++nj) {
						gf16_32_mul_32_add((LJxPJ_256[ni] + nk), (R_tr_J_256[mi][alpha][nj] + nk), (&h_256[mi_prime][ni][nj]));
					}
				}
			}
		}
	}

	// set hash_in_GF16Matrix
	for (int mi = 0; mi < m_SNOVA; ++mi) {
		for (int ni = 0; ni < rank; ++ni) {
			for (int nj = 0; nj < rank; ++nj) {
				set_gf16m(hash_in_GF16Matrix[mi], ni, nj, horizontal_xor_256(h_256[mi][ni][nj]));
			}
		}
	}
}

int verify_signture_vtl_core(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature, const public_key_expand *pkx) {
	uint8_t hash_in_bytes[bytes_hash];
	uint8_t signed_hash[bytes_hash];
	const uint8_t *pt_salt = pt_signature + bytes_signature;

	gf16m_t hash_in_GF16Matrix[m_SNOVA];
	gf16m_t signature_in_GF16Matrix[n_SNOVA];

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
	evaluation_vector_vtl(hash_in_GF16Matrix, pkx, signature_in_GF16Matrix);
	convert_GF16s_to_bytes(hash_in_bytes, (gf16_t *)hash_in_GF16Matrix, m_SNOVA * lsq_SNOVA);

	int result = 0;
	for (int i = 0; i < bytes_hash; ++i) {
		if (hash_in_bytes[i] != signed_hash[i]) {
			result = -1;
			break;
		}
	}

	return result;
}

/**
 * Regular entry point for verification
 */
int verify_signture_vtl(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature, const uint8_t *pk) {
	public_key_expand pkx;
	expand_public_core(&pkx, pk);
	return verify_signture_vtl_core(pt_digest, bytes_digest, pt_signature, &pkx);
}

int verify_signture_pkx_vtl(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature, const uint8_t *pkx_pck) {
	public_key_expand pkx_unpck;
	pkx_unpack(&pkx_unpck, (public_key_expand_pack *)pkx_pck);
	return verify_signture_vtl_core(pt_digest, bytes_digest, pt_signature, &pkx_unpck);
}

#endif
