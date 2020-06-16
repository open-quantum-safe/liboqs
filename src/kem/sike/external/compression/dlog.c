/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: Pohlig-Hellman with optimal strategy
*
* SPDX-License-Identifier: MIT
*********************************************************************************************/

static void from_base(int *D, digit_t *r, int Dlen, int base) { // Convert a number in base "base": (D[k-1]D[k-2]...D[1]D[0])_base < 2^(NWORDS_ORDER*RADIX)into decimal
	// Output: r = D[k-1]*base^(k-1) + ... + D[1]*base + D[0]
	digit_t ell[NWORDS_ORDER] = {0}, digit[NWORDS_ORDER] = {0}, temp[NWORDS_ORDER] = {0};
	int ellw;

	ell[0] = base;
	r[0] = D[Dlen - 1] * ell[0];
	for (int i = Dlen - 2; i >= 1; i--) {
		ellw = base;
		digit[0] = (digit_t) D[i];
		mp_add(r, digit, r, NWORDS_ORDER);
		if ((base & (int) 0x1) == 0x0) {
			while (ellw > 1) {
				mp_add(r, r, r, NWORDS_ORDER);
				ellw /= 2;
			}
		} else {
			while (ellw > 1) {
				mp_add(r, r, temp, NWORDS_ORDER);
				mp_add(r, temp, r, NWORDS_ORDER);
				ellw /= 3;
			}
		}
	}
	digit[0] = (digit_t) D[0];
	mp_add(r, digit, r, NWORDS_ORDER);
}

static void Traverse_w_div_e(const f2elm_t r, int j, int k, int z, const unsigned int *P, const f2elm_t *T, int *D, int Dlen, int ell, int w) { // Traverse a Pohlig-Hellman optimal strategy to solve a discrete log in a group of order ell^e
	// The leaves of the tree will be used to recover the digits which are numbers from 0 to ell^w-1
	// Assume the integer w divides the exponent e
	f2elm_t rp = {0};

	if (z > 1) {
		int t = P[z];
		fp2copy(r, rp);
		for (int i = 0; i < z - t; i++) {
			if ((ell & 1) == 0) {
				for (int ii = 0; ii < w; ii++) {
					sqr_Fp2_cycl(rp, (digit_t *) &Montgomery_one);
				}
			} else {
				for (int ii = 0; ii < w; ii++) {
					cube_Fp2_cycl(rp, (digit_t *) &Montgomery_one);
				}
			}
		}
		Traverse_w_div_e(rp, j + (z - t), k, t, P, T, D, Dlen, ell, w);

		fp2copy(r, rp);
		for (int h = k; h < k + t; h++) {
			fp2mul_mont(rp, (const felm_t *) &T[(j + h) * ell][2 * D[h]], rp);
		}
		Traverse_w_div_e(rp, j, k + t, z - t, P, T, D, Dlen, ell, w);
	} else {
		fp2_conj(r, rp);
		fp2correction(rp);
		for (int t = 0; t < ell; t++) {
			if (memcmp((unsigned char *) &T[(Dlen - 1) * ell][2 * t], rp, 2 * NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
				D[k] = t;
				break;
			}
		}
	}
}

static void Traverse_w_notdiv_e(const f2elm_t r, int j, int k, int z, const unsigned int *P, const f2elm_t *T1, const f2elm_t *T2, int *D, int Dlen, int ell, int ellw, int ell_emodw, int w, int e) { // Traverse a Pohlig-Hellman optimal strategy to solve a discrete log in a group of order ell^e
	// Leaves are used to recover the digits which are numbers from 0 to ell^w-1 except by the last leaf that gives a digit between 0 and ell^(e mod w)
	// Assume w does not divide the exponent e
	f2elm_t rp = {0};

	if (z > 1) {
		int t = P[z], goleft;
		fp2copy(r, rp);

		goleft = (j > 0) ? w * (z - t) : (e % w) + w * (z - t - 1);
		for (int i = 0; i < goleft; i++) {
			if ((ell & 1) == 0) {
				sqr_Fp2_cycl(rp, (digit_t *) &Montgomery_one);
			} else {
				cube_Fp2_cycl(rp, (digit_t *) &Montgomery_one);
			}
		}

		Traverse_w_notdiv_e(rp, j + (z - t), k, t, P, T1, T2, D, Dlen, ell, ellw, ell_emodw, w, e);

		fp2copy(r, rp);
		for (int h = k; h < k + t; h++) {
			if (j > 0) {
				fp2mul_mont(rp, (const felm_t *) &T2[ellw * (j + h)][2 * D[h]], rp);
			} else {
				fp2mul_mont(rp, (const felm_t *) &T1[ellw * (j + h)][2 * D[h]], rp);
			}
		}

		Traverse_w_notdiv_e(rp, j, k + t, z - t, P, T1, T2, D, Dlen, ell, ellw, ell_emodw, w, e);
	} else {
		fp2_conj(r, rp);
		fp2correction(rp);
		if (!(j == 0 && k == Dlen - 1)) {
			for (int t = 0; t < ellw; t++) {
				if (memcmp((unsigned char *) &T2[ellw * (Dlen - 1)][2 * t], rp, 2 * NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
					D[k] = t;
					break;
				}
			}
		} else {
			for (int t = 0; t < ell_emodw; t++) {
				if (memcmp((unsigned char *) &T1[ellw * (Dlen - 1)][2 * t], rp, 2 * NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
					D[k] = t;
					break;
				}
			}
		}
	}
}

static void solve_dlog(const f2elm_t r, int *D, digit_t *d, int ell) { // Computes the discrete log of input r = g^d where g = e(P,Q)^ell^e, and P,Q are torsion generators in the initial curve
	// Return the integer d
	if (ell == 2) {
#if (OALICE_BITS % W_2 == 0)
		Traverse_w_div_e(r, 0, 0, PLEN_2 - 1, ph2_path, (const f2elm_t *) &ph2_T, D, DLEN_2, ELL2_W, W_2);
#else
		Traverse_w_notdiv_e(r, 0, 0, PLEN_2 - 1, ph2_path, (const f2elm_t *) &ph2_T1, (const f2elm_t *) &ph2_T2, D, DLEN_2, ell, ELL2_W, ELL2_EMODW, W_2, OALICE_BITS);
#endif
		from_base(D, d, DLEN_2, ELL2_W);
	} else if (ell == 3) {
#if (OBOB_EXPON % W_3 == 0)
		Traverse_w_div_e(r, 0, 0, PLEN_3 - 1, ph3_path, (const f2elm_t *) &ph3_T, D, DLEN_3, ELL3_W, W_3);
#else
		Traverse_w_notdiv_e(r, 0, 0, PLEN_3 - 1, ph3_path, (const f2elm_t *) &ph3_T1, (const f2elm_t *) &ph3_T2, D, DLEN_3, ell, ELL3_W, ELL3_EMODW, W_3, OBOB_EXPON);
#endif
		from_base(D, d, DLEN_3, ELL3_W);
	}
}

static void ph2(const point_full_proj_t phiP, const point_full_proj_t phiQ, const point_t PS, const point_t QS, const f2elm_t A, digit_t *c0, digit_t *d0, digit_t *c1, digit_t *d1) { // Computes the 4 coefficients of the change of basis matrix between the bases {phiP,phiQ} and {PS, QS}
	// Assume both bases generate the full 2^eA torsion
	f2elm_t n[4] = {0};
	int D[DLEN_2];

	// Compute the four pairings
	Tate_4_pairings_2_torsion(phiP, phiQ, PS, QS, A, n);

	solve_dlog(n[0], D, d0, 2);
	solve_dlog(n[2], D, c0, 2);
	mp_sub((digit_t *) Alice_order, c0, c0, NWORDS_ORDER);
	solve_dlog(n[1], D, d1, 2);
	solve_dlog(n[3], D, c1, 2);
	mp_sub((digit_t *) Alice_order, c1, c1, NWORDS_ORDER);
}

static void ph3(point_full_proj_t phiP, point_full_proj_t phiQ, point_full_proj_t PS, point_full_proj_t QS, f2elm_t A, digit_t *c0, digit_t *d0, digit_t *c1, digit_t *d1) { // Computes the 4 coefficients of the change of basis matrix between the bases {phiP,phiQ} and {PS, QS}
	// Assume both bases generate the full 3^eA torsion
	f2elm_t n[4] = {0};
	int D[DLEN_3];

	// Compute the four pairings
	Tate_4_pairings_3_torsion(phiP, phiQ, PS, QS, A, n);

	solve_dlog(n[0], D, d0, 3);
	solve_dlog(n[2], D, c0, 3);
	mp_sub((digit_t *) Bob_order, c0, c0, NWORDS_ORDER);
	solve_dlog(n[1], D, d1, 3);
	solve_dlog(n[3], D, c1, 3);
	mp_sub((digit_t *) Bob_order, c1, c1, NWORDS_ORDER);
}
