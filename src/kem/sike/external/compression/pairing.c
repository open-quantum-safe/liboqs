/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: pairing computation for compression
*********************************************************************************************/

#define t_points 2

static void get_A_compression(const f2elm_t xP, const f2elm_t xQ, const f2elm_t xR, f2elm_t A) { // Given the x-coordinates of P, Q, and R, returns the value A corresponding to the Montgomery curve E_A: y^2=x^3+A*x^2+x such that R=Q-P on E_A.
	// Input:  the x-coordinates xP, xQ, and xR of the points P, Q and R.
	// Output: the coefficient A corresponding to the curve E_A: y^2=x^3+A*x^2+x.
	f2elm_t t0, t1, one = {0};

	fpcopy((digit_t *) &Montgomery_one, one[0]);
	fp2add(xP, xQ, t1);      // t1 = xP+xQ
	fp2mul_mont(xP, xQ, t0); // t0 = xP*xQ
	fp2mul_mont(xR, t1, A);  // A = xR*t1
	fp2add(t0, A, A);        // A = A+t0
	fp2mul_mont(t0, xR, t0); // t0 = t0*xR
	fp2sub(A, one, A);       // A = A-1
	fp2add(t0, t0, t0);      // t0 = t0+t0
	fp2add(t1, xR, t1);      // t1 = t1+xR
	fp2add(t0, t0, t0);      // t0 = t0+t0
	fp2sqr_mont(A, A);       // A = A^2
	fp2inv_mont_bingcd(t0);  // t0 = 1/t0
	fp2mul_mont(A, t0, A);   // A = A*t0
	fp2sub(A, t1, A);        // Afinal = A-t1
}

static void recover_y(const publickey_t PK, point_full_proj_t phiP, point_full_proj_t phiQ, point_full_proj_t phiX, f2elm_t A) { // Recover the y-coordinates of the public key
	// The three resulting points are (simultaneously) correct up to sign
	f2elm_t tmp, phiXY, one = {0};

	fpcopy((digit_t *) &Montgomery_one, one[0]);
	get_A_compression(PK[0], PK[1], PK[2], A); // NOTE: don't have to compress this, can output in keygen

	fp2add(PK[2], A, tmp);
	fp2mul_mont(PK[2], tmp, tmp);
	fp2add(tmp, one, tmp);
	fp2mul_mont(PK[2], tmp, tmp); // tmp = PK[2]^3+A*PK[2]^2+PK[2];
	sqrt_Fp2(tmp, phiXY);
	fp2copy(PK[2], phiX->X);
	fp2copy(phiXY, phiX->Y);
	fp2copy(one, phiX->Z); // phiX = [PK[2],phiXY,1];

	recover_os(PK[1], one, PK[0], one, PK[2], phiXY, A, phiQ->X, phiQ->Y, phiQ->Z);
	fp2neg(phiXY);
	recover_os(PK[0], one, PK[1], one, PK[2], phiXY, A, phiP->X, phiP->Y, phiP->Z);
}

static void final_exponentiation_2_torsion(f2elm_t f, const f2elm_t finv, f2elm_t fout) { // The final exponentiation for pairings in the 2^eA-torsion group. Raising the value f to the power (p^2-1)/2^eA.
	felm_t one = {0};
	f2elm_t temp = {0};
	unsigned int i;

	fpcopy((digit_t *) &Montgomery_one, one);

	// f = f^p
	fp2_conj(f, temp);
	fp2mul_mont(temp, finv, temp); // temp = f^(p-1)

	for (i = 0; i < OBOB_EXPON; i++) {
		cube_Fp2_cycl(temp, one);
	}
	fp2copy(temp, fout);
}

static void Tate_pairings_2_torsion(const point_full_proj_t P, point_full_proj_t *Qj, f2elm_t a, f2elm_t *n) { // Compute the reduced Tate pairings e_{2^m}(P, Q_j) for the curve y^2 = x^3 + a*x + b:
	f2elm_t h[t_points], one = {0};
	f2elm_t X, Y, Z, X2, Y2, Y4, M, S, T, temp;
	f2elm_t Xp, Yp, Zp, Tp;
	f2elm_t L, W, g;

	fpcopy((digit_t *) &Montgomery_one, one[0]);
	fp2copy(P->X, X);
	fp2copy(P->Y, Y);
	fp2copy(P->Z, Z);
	fp2sqr_mont(Z, T);

	for (int j = 0; j < t_points; j++) {
		fp2copy(one, n[j]);
		fp2mul_mont(T, Qj[j]->X, temp);
		fp2sub(temp, X, h[j]);
	}

	for (int k = 0; k < OALICE_BITS; k++) {
		// Point doubling and line function construction:
		fp2sqr_mont(X, X2);
		fp2sqr_mont(Y, Y2);
		fp2sqr_mont(Y2, Y4);
		fp2sqr_mont(T, temp);
		fp2mul_mont(a, temp, temp);
		fp2add(X2, X2, M);
		fp2add(M, X2, M);
		fp2add(M, temp, M); // M = 3*X_2 + a*T^2
		fp2add(X, Y2, S);
		fp2sqr_mont(S, S);
		fp2sub(S, X2, S);
		fp2sub(S, Y4, S);
		fp2add(S, S, S); // S = 2*((X + Y2)^2 - X2 - Y4)
		fp2sqr_mont(M, temp);
		fp2add(S, S, Xp);
		fp2sub(temp, Xp, Xp); // Xp = M^2 - 2*S
		fp2sub(S, Xp, temp);
		fp2mul_mont(M, temp, temp);
		fp2shl(Y4, 3, Yp);
		fp2sub(temp, Yp, Yp); // Yp = M*(S - Xp) - 8*Y4
		fp2add(Y, Z, temp);
		fp2sqr_mont(temp, temp);
		fp2sub(temp, Y2, temp);
		fp2sub(temp, T, Zp);   // Zp = (Y + Z)^2 - Y2 - T
		fp2sqr_mont(Zp, Tp);   // Tp = Zp^2
		fp2mul_mont(Zp, T, L); // L = Zp*T
		fp2add(Y2, Y2, W);     // W = 2*Y2

		fp2correction(Zp);
		if (is_felm_zero(Zp[0]) && is_felm_zero(Zp[1])) { // Doubling exception for points in 2*E
			fp2copy(one, Xp);
			fp2copy(one, Yp);
		}

		// Line function evaluation and accumulation:
		for (int j = 0; j < t_points; j++) {
			if (!is_felm_zero(Zp[0]) || !is_felm_zero(Zp[1])) {
				fp2mul_mont(M, h[j], temp);
				fp2add(temp, W, temp);
				fp2mul_mont(L, Qj[j]->Y, g);
				fp2sub(temp, g, g); // g = M*hj + W - L*Y_{Qj}
				fp2mul_mont(Tp, Qj[j]->X, temp);
				fp2sub(temp, Xp, h[j]); // hj = Tp*X_{Qj} - Xp
				fp2_conj(h[j], temp);
				fp2mul_mont(temp, g, g); // g = g*hj^*
			} else {
				fp2copy(h[j], g);
			}
			fp2sqr_mont(n[j], n[j]);
			fp2mul_mont(n[j], g, n[j]);
		}
		fp2copy(Xp, X);
		fp2copy(Yp, Y);
		fp2copy(Zp, Z);
		fp2copy(Tp, T);
	}

	// Final exponentiation:
	mont_n_way_inv(n, t_points, h);
	for (int j = 0; j < t_points; j++) {
		fp2correction(Qj[j]->Z);
		if (is_felm_zero(Qj[j]->Z[0]) && is_felm_zero(Qj[j]->Z[1])) {
			fp2copy(one, n[j]);
		} else {
			final_exponentiation_2_torsion(n[j], h[j], n[j]);
		}
	}
}

static void Monty2Weier(const f2elm_t A, f2elm_t a, f2elm_t b) { // Convert a Montgomery curve EM: y^2 = x^3 + A*x^2 + x into its short Weierstrass form EW: v^2 = u^3 + a*u + b.
	f2elm_t one = {0}, temp, A2, AA;

	fpcopy((digit_t *) &Montgomery_one, one[0]);
	// a = 1 - A^2/3
	fp2sqr_mont(A, A2);
	fpmul_mont(A2[0], (digit_t *) threeinv, temp[0]);
	fpmul_mont(A2[1], (digit_t *) threeinv, temp[1]);
	fp2sub(one, temp, a);

	// b = (2*A^3 - 9*A)/27
	fp2add(A, A, AA);          // AA = 2*A
	fp2mul_mont(AA, A2, temp); // temp = 2A^3
	fp2add(AA, AA, A2);        // A2 = 4*A
	fp2add(A2, A2, A2);        // A2 = 8*A
	fp2add(A2, A, A2);         // A2 = 9*A
	fp2sub(temp, A2, b);       // b = 2A^3 - 9*A
	fpmul_mont((digit_t *) threeinv, (digit_t *) threeinv, temp[0]);
	fpmul_mont(temp[0], (digit_t *) threeinv, temp[0]);
	fpmul_mont(temp[0], b[0], b[0]);
	fpmul_mont(temp[0], b[1], b[1]);
}

static void PointMonty2Weier(const point_full_proj_t PM, point_full_proj_t PW, const f2elm_t A) { // Convert a point PM on a Montgomery curve y^2 = x^3 + A*x^2 + x into the corresponding point on its short Weierstrass form EW.
	f2elm_t zero = {0}, one = {0}, temp;

	fpcopy((digit_t *) &Montgomery_one, one[0]);

	if (is_felm_zero(PM->Z[0]) && is_felm_zero(PM->Z[1])) {
		fp2copy(zero, PW->X);
		fp2copy(one, PW->Y);
		fp2copy(zero, PW->Z);
		return;
	}
	fpmul_mont((digit_t *) threeinv, A[0], temp[0]);
	fpmul_mont((digit_t *) threeinv, A[1], temp[1]);

	// PW = EW(PM->X + A/3, PM->Y, 1)
	fp2add(temp, PM->X, PW->X);
	fp2copy(PM->Y, PW->Y);
	fp2copy(one, PW->Z);
}

static void Tate_4_pairings_2_torsion(const point_full_proj_t P, const point_full_proj_t Q, const point_t S1, const point_t S2, const f2elm_t A, f2elm_t *n) { // The doubling only 2-torsion Tate pairing of order 2^eA, consisting of the doubling only Miller loop and the final exponentiation.]
	// Computes 4 pairings at once: e(P, S1), e(P, S2), e(Q, S1), e(Q, S2).
	point_full_proj_t Qj[2], PW, QW, QjW[2];
	f2elm_t a, b, one = {0};

	fpcopy((digit_t *) &Montgomery_one, one[0]);
	Monty2Weier(A, a, b);

	// Assume S1 and S2 are normalized
	fp2copy(S1->x, Qj[0]->X);
	fp2copy(S1->y, Qj[0]->Y);
	fp2copy(one, Qj[0]->Z);
	fp2copy(S2->x, Qj[1]->X);
	fp2copy(S2->y, Qj[1]->Y);
	fp2copy(one, Qj[1]->Z);

	PointMonty2Weier(P, PW, A);
	PointMonty2Weier(Q, QW, A);
	PointMonty2Weier(Qj[0], QjW[0], A);
	PointMonty2Weier(Qj[1], QjW[1], A);

	Tate_pairings_2_torsion(PW, QjW, a, n);
	Tate_pairings_2_torsion(QW, QjW, a, n + 2);
}

static void final_exponentiation_3_torsion(f2elm_t f, const f2elm_t finv, f2elm_t fout) { // The final exponentiation for pairings in the 3-torsion group. Raising the value f to the power (p^2-1)/3^eB.
	felm_t one = {0};
	f2elm_t temp;
	unsigned int i;

	fpcopy((digit_t *) &Montgomery_one, one);

	// f = f^p
	fp2_conj(f, temp);
	fp2mul_mont(temp, finv, temp); // temp = f^(p-1)

	for (i = 0; i < OALICE_BITS; i++) {
		sqr_Fp2_cycl(temp, one);
	}
	fp2copy(temp, fout);
}

static void Tate_pairings_3_torsion(const point_full_proj_t P, point_full_proj_t *Qj, f2elm_t a, f2elm_t *n) { // The tripling only 3-torsion Tate pairing of order 3^eB, consisting of the tripling only Miller loop and the final exponentiation.
	// Computes 4 pairings at once: e(P, R1), e(P, R2), e(Q, R1), e(Q, R2).
	f2elm_t h[t_points], one = {0};
	f2elm_t X, Y, Z, X2, Y2, Y4, M, S, T;
	f2elm_t Xp, Yp, Zp, Tp, D, U, Up, Fp;
	f2elm_t L, W, Wp, g, T2, M2, F, F2, d;
	f2elm_t temp, temp1;

	fpcopy((digit_t *) &Montgomery_one, one[0]);
	fp2copy(P->X, X);
	fp2copy(P->Y, Y);
	fp2copy(P->Z, Z);
	fp2sqr_mont(Z, T);

	for (int j = 0; j < t_points; j++) {
		fp2copy(one, n[j]);
		fp2mul_mont(T, Qj[j]->X, temp);
		fp2sub(temp, X, h[j]);
	}

	for (int k = 0; k < OBOB_EXPON; k++) {
		// Point tripling and parabola function construction:
		fp2sqr_mont(X, X2);
		fp2sqr_mont(Y, Y2);
		fp2sqr_mont(Y2, Y4);
		fp2sqr_mont(T, T2);
		fp2add(X2, X2, temp);
		fp2add(temp, X2, temp);
		fp2mul_mont(a, T2, M);
		fp2add(temp, M, M); // M = 3*X2 + a*T2
		fp2sqr_mont(M, M2);
		fp2add(X, Y2, temp);
		fp2sqr_mont(temp, temp);
		fp2sub(temp, X2, temp);
		fp2sub(temp, Y4, D); // D = (X + Y2)^2 - X2 - Y4
		fp2add(D, D, temp);
		fp2add(temp, D, temp);
		fp2add(temp, temp, temp);
		fp2sub(temp, M2, F); // F = 6*D - M2
		fp2sqr_mont(F, F2);
		fp2add(Y2, Y2, W);
		fp2add(W, W, Wp);
		fp2shl(Y4, 4, S);
		fp2add(M, F, temp);
		fp2sqr_mont(temp, temp);
		fp2sub(temp, M2, temp);
		fp2sub(temp, F2, temp);
		fp2sub(temp, S, U); // U = (M + F)^2 - M2 - F2 - S
		fp2sub(S, U, Up);
		fp2mul_mont(X, F2, temp);
		fp2mul_mont(Wp, U, Xp);
		fp2sub(temp, Xp, Xp);
		fp2shl(Xp, 2, Xp); // Xp = 4*(X*F2 - Wp*U)
		fp2mul_mont(U, Up, temp);
		fp2mul_mont(F, F2, Yp);
		fp2sub(temp, Yp, Yp);
		fp2mul_mont(Y, Yp, Yp);
		fp2shl(Yp, 3, Yp); // Yp = 8*Y*(U*Up - F*F2)
		fp2add(Z, F, temp);
		fp2sqr_mont(temp, temp);
		fp2sub(temp, T, temp);
		fp2sub(temp, F2, Zp); // Zp = (Z + F)^2 - T - F2
		fp2sqr_mont(Zp, Tp);
		fp2add(Y, Z, temp);
		fp2sqr_mont(temp, temp);
		fp2sub(temp, Y2, temp);
		fp2sub(temp, T, temp);
		fp2mul_mont(temp, T, L);
		fp2add(F, F, Fp);

		fp2correction(Zp);
		if (is_felm_zero(Zp[0]) && is_felm_zero(Zp[1])) {
			fp2copy(one, Xp);
			fp2copy(one, Yp);
		}

		// Parabola function evaluation and accumulation
		for (int j = 0; j < t_points; j++) {
			fp2mul_mont(L, Qj[j]->Y, temp);
			fp2sub(W, temp, d);
			if (!is_felm_zero(Zp[0]) || !is_felm_zero(Zp[1])) {
				fp2mul_mont(M, h[j], temp);
				fp2add(d, temp, g); // g = (M*h + d)
				fp2mul_mont(Up, h[j], temp);
				fp2mul_mont(Fp, d, temp1);
				fp2add(temp, temp1, temp1);
				fp2mul_mont(temp1, g, g); // g = (M*h + d)*(Up*h + Fp*d)
				fp2mul_mont(Wp, h[j], temp);
				fp2add(F, temp, temp);
				fp2_conj(temp, temp);
				fp2mul_mont(temp, g, g); // g = (M*h + d)*(Up*h + Fp*d)*(Wp*h + F)^*
				fp2mul_mont(Tp, Qj[j]->X, temp);
				fp2sub(temp, Xp, h[j]); // h = Tp*X_{Qj} - Xp
				fp2_conj(h[j], temp);
				fp2mul_mont(temp, g, g);
			} else {
				fp2mul_mont(M, h[j], temp);
				fp2add(temp, d, g);
			}
			fp2sqr_mont(n[j], temp);
			fp2mul_mont(temp, n[j], n[j]);
			fp2mul_mont(g, n[j], n[j]);
		}
		fp2copy(Xp, X);
		fp2copy(Yp, Y);
		fp2copy(Zp, Z);
		fp2copy(Tp, T);
	}

	// Final exponentiation:
	mont_n_way_inv(n, t_points, h);
	for (int j = 0; j < t_points; j++) {
		fp2correction(Qj[j]->Z);
		if (is_felm_zero(Qj[j]->Z[0]) && is_felm_zero(Qj[j]->Z[1])) {
			fp2copy(one, n[j]);
		} else {
			final_exponentiation_3_torsion(n[j], h[j], n[j]);
		}
	}
}

static void Tate_4_pairings_3_torsion(const point_full_proj_t P, const point_full_proj_t Q, const point_full_proj_t R1, const point_full_proj_t R2, const f2elm_t A, f2elm_t *n) { // The tripling only 3-torsion Tate pairing of order 3^eB, consisting of the tripling only Miller loop and the final exponentiation.
	// Computes 4 pairings at once: e(P, S1), e(P, S2), e(Q, S1), e(Q,S2).
	point_full_proj_t Qj[2] = {0}, PW, QW, QjW[2] = {0};
	f2elm_t a, b;

	Monty2Weier(A, a, b);
	fp2copy(R1->X, Qj[0]->X);
	fp2copy(R1->Y, Qj[0]->Y);
	fp2copy(R1->Z, Qj[0]->Z);
	fp2copy(R2->X, Qj[1]->X);
	fp2copy(R2->Y, Qj[1]->Y);
	fp2copy(R2->Z, Qj[1]->Z);

	PointMonty2Weier(P, PW, A);
	PointMonty2Weier(Q, QW, A);
	PointMonty2Weier(Qj[0], QjW[0], A);
	PointMonty2Weier(Qj[1], QjW[1], A);
	Tate_pairings_3_torsion(PW, QjW, a, n);
	Tate_pairings_3_torsion(QW, QjW, a, n + 2);
}

static void Tate3_pairings(point_full_proj_t *Qj, f2elm_t *f) {
	felm_t *x, *y, *l1, *l2, *n1, *n2, *x2, *x23, *x2p3;
	f2elm_t xQ2s[t_points], finv[2 * t_points], one = {0};
	f2elm_t t0, t1, t2, t3, t4, t5, g, h, tf;

	fpcopy((digit_t *) &Montgomery_one, one[0]);

	for (int j = 0; j < t_points; j++) {
		fp2copy(one, f[j]);
		fp2copy(one, f[j + t_points]);
		fp2sqr_mont(Qj[j]->X, xQ2s[j]);
	}

	for (int k = 0; k < OBOB_EXPON - 1; k++) {
		l1 = (felm_t *) T_tate3 + 6 * k + 0;
		l2 = (felm_t *) T_tate3 + 6 * k + 1;
		n1 = (felm_t *) T_tate3 + 6 * k + 2;
		n2 = (felm_t *) T_tate3 + 6 * k + 3;
		x23 = (felm_t *) T_tate3 + 6 * k + 4;
		x2p3 = (felm_t *) T_tate3 + 6 * k + 5;
		for (int j = 0; j < t_points; j++) {
			fpmul_mont(Qj[j]->X[0], *l1, t0[0]);
			fpmul_mont(Qj[j]->X[1], *l1, t0[1]);
			fpmul_mont(Qj[j]->X[0], *l2, t2[0]);
			fpmul_mont(Qj[j]->X[1], *l2, t2[1]);
			fpadd(xQ2s[j][0], *x23, t4[0]);
			fpcopy(xQ2s[j][1], t4[1]);
			fpmul_mont(Qj[j]->X[0], *x2p3, t5[0]);
			fpmul_mont(Qj[j]->X[1], *x2p3, t5[1]);

			fp2sub(t0, Qj[j]->Y, t1);
			fpadd(t1[0], *n1, t1[0]);
			fp2sub(t2, Qj[j]->Y, t3);
			fpadd(t3[0], *n2, t3[0]);
			fp2mul_mont(t1, t3, g);
			fp2sub(t4, t5, h);
			fp2_conj(h, h);
			fp2mul_mont(g, h, g);

			fp2sqr_mont(f[j], tf);
			fp2mul_mont(f[j], tf, f[j]);
			fp2mul_mont(f[j], g, f[j]);

			fpsub(t0[1], Qj[j]->Y[0], t1[0]);
			fpadd(t0[0], Qj[j]->Y[1], t1[1]);
			fpneg(t1[1]);
			fpadd(t1[1], *n1, t1[1]);
			fpsub(t2[1], Qj[j]->Y[0], t3[0]);
			fpadd(t2[0], Qj[j]->Y[1], t3[1]);
			fpneg(t3[1]);
			fpadd(t3[1], *n2, t3[1]);

			fp2mul_mont(t1, t3, g);
			fp2add(t4, t5, h);
			fp2_conj(h, h);
			fp2mul_mont(g, h, g);

			fp2sqr_mont(f[j + t_points], tf);
			fp2mul_mont(f[j + t_points], tf, f[j + t_points]);
			fp2mul_mont(f[j + t_points], g, f[j + t_points]);
		}
	}
	for (int j = 0; j < t_points; j++) {
		x = (felm_t *) T_tate3 + 6 * (OBOB_EXPON - 1) + 0;
		y = (felm_t *) T_tate3 + 6 * (OBOB_EXPON - 1) + 1;
		l1 = (felm_t *) T_tate3 + 6 * (OBOB_EXPON - 1) + 2;
		x2 = (felm_t *) T_tate3 + 6 * (OBOB_EXPON - 1) + 3;

		fpsub(Qj[j]->X[0], *x, t0[0]);
		fpcopy(Qj[j]->X[1], t0[1]);
		fpmul_mont(*l1, t0[0], t1[0]);
		fpmul_mont(*l1, t0[1], t1[1]);
		fp2sub(t1, Qj[j]->Y, t2);
		fpadd(t2[0], *y, t2[0]);
		fp2mul_mont(t0, t2, g);
		fpsub(Qj[j]->X[0], *x2, h[0]);
		fpcopy(Qj[j]->X[1], h[1]);
		fpneg(h[1]);
		fp2mul_mont(g, h, g);

		fp2sqr_mont(f[j], tf);
		fp2mul_mont(f[j], tf, f[j]);
		fp2mul_mont(f[j], g, f[j]);

		fpadd(Qj[j]->X[0], *x, t0[0]);
		fpmul_mont(*l1, t0[0], t1[0]);
		fpsub(Qj[j]->Y[0], t1[1], t2[0]);
		fpadd(Qj[j]->Y[1], t1[0], t2[1]);
		fpsub(t2[1], *y, t2[1]);
		fp2mul_mont(t0, t2, g);
		fpadd(Qj[j]->X[0], *x2, h[0]);
		fp2mul_mont(g, h, g);

		fp2sqr_mont(f[j + t_points], tf);
		fp2mul_mont(f[j + t_points], tf, f[j + t_points]);
		fp2mul_mont(f[j + t_points], g, f[j + t_points]);
	}

	// Final exponentiation:
	mont_n_way_inv(f, 2 * t_points, finv);
	for (int j = 0; j < 2 * t_points; j++) {
		final_exponentiation_3_torsion(f[j], finv[j], f[j]);
	}
}

static void Tate2_pairings(const point_t P, const point_t Q, point_full_proj_t *Qj, f2elm_t *f) {
	felm_t *x, *y, *x_, *y_, *l1;
	f2elm_t finv[2 * t_points], one = {0};
	f2elm_t *x_first, *y_first, l1_first, t0, t1, g, h;

	fpcopy((digit_t *) &Montgomery_one, one[0]);

	for (int j = 0; j < t_points; j++) {
		fp2copy(one, f[j]);
		fp2copy(one, f[j + t_points]);
	}

	// Pairings with P
	x_first = (f2elm_t *) P->x;
	y_first = (f2elm_t *) P->y;

	x_ = (felm_t *) T_tate2_firststep_P + 0;
	y_ = (felm_t *) T_tate2_firststep_P + 1;
	fpcopy((digit_t *) T_tate2_firststep_P + 2 * NWORDS_FIELD, l1_first[0]);
	fpcopy((digit_t *) T_tate2_firststep_P + 3 * NWORDS_FIELD, l1_first[1]);

	for (int j = 0; j < t_points; j++) {
		fp2sub(Qj[j]->X, *x_first, t0);
		fp2sub(Qj[j]->Y, *y_first, t1);
		fp2mul_mont(l1_first, t0, t0);
		fp2sub(t0, t1, g);

		fpsub(Qj[j]->X[0], *x_, h[0]);
		fpcopy(Qj[j]->X[1], h[1]);
		fpneg(h[1]);
		fp2mul_mont(g, h, g);

		fp2sqr_mont(f[j], f[j]);
		fp2mul_mont(f[j], g, f[j]);
	}
	x = x_;
	y = y_;

	for (int k = 0; k < OALICE_BITS - 2; k++) {
		x_ = (felm_t *) T_tate2_P + 3 * k + 0;
		y_ = (felm_t *) T_tate2_P + 3 * k + 1;
		l1 = (felm_t *) T_tate2_P + 3 * k + 2;
		for (int j = 0; j < t_points; j++) {
			fpsub(*x, Qj[j]->X[0], t0[1]);
			fpmul_mont(*l1, t0[1], t0[1]);
			fpmul_mont(*l1, Qj[j]->X[1], t0[0]);
			fpsub(Qj[j]->Y[1], *y, t1[1]);
			fpsub(t0[1], t1[1], g[1]);
			fpsub(t0[0], Qj[j]->Y[0], g[0]);

			fpsub(Qj[j]->X[0], *x_, h[0]);
			fpcopy(Qj[j]->X[1], h[1]);
			fpneg(h[1]);
			fp2mul_mont(g, h, g);

			fp2sqr_mont(f[j], f[j]);
			fp2mul_mont(f[j], g, f[j]);
		}
		x = x_;
		y = y_;
	}
	for (int j = 0; j < t_points; j++) {
		fpsub(Qj[j]->X[0], *x, g[0]);
		fpcopy(Qj[j]->X[1], g[1]);
		fp2sqr_mont(f[j], f[j]);
		fp2mul_mont(f[j], g, f[j]);
	}

	// Pairings with Q
	x_first = (f2elm_t *) Q->x;
	y_first = (f2elm_t *) Q->y;
	x_ = (felm_t *) T_tate2_firststep_Q + 0;
	y_ = (felm_t *) T_tate2_firststep_Q + 1;
	fpcopy(((felm_t *) T_tate2_firststep_Q)[2], l1_first[0]);
	fpcopy(((felm_t *) T_tate2_firststep_Q)[3], l1_first[1]);

	for (int j = 0; j < t_points; j++) {
		fp2sub(Qj[j]->X, *x_first, t0);
		fp2sub(Qj[j]->Y, *y_first, t1);
		fp2mul_mont(l1_first, t0, t0);
		fp2sub(t0, t1, g);

		fpsub(Qj[j]->X[0], *x_, h[0]);
		fpcopy(Qj[j]->X[1], h[1]);
		fpneg(h[1]);
		fp2mul_mont(g, h, g);

		fp2sqr_mont(f[j + t_points], f[j + t_points]);
		fp2mul_mont(f[j + t_points], g, f[j + t_points]);
	}
	x = x_;
	y = y_;

	for (int k = 0; k < OALICE_BITS - 2; k++) {
		x_ = (felm_t *) T_tate2_Q + 3 * k + 0;
		y_ = (felm_t *) T_tate2_Q + 3 * k + 1;
		l1 = (felm_t *) T_tate2_Q + 3 * k + 2;
		for (int j = 0; j < t_points; j++) {
			fpsub(Qj[j]->X[0], *x, t0[0]);
			fpmul_mont(*l1, t0[0], t0[0]);
			fpmul_mont(*l1, Qj[j]->X[1], t0[1]);
			fpsub(Qj[j]->Y[0], *y, t1[0]);
			fpsub(t0[0], t1[0], g[0]);
			fpsub(t0[1], Qj[j]->Y[1], g[1]);

			fpsub(Qj[j]->X[0], *x_, h[0]);
			fpcopy(Qj[j]->X[1], h[1]);
			fpneg(h[1]);
			fp2mul_mont(g, h, g);

			fp2sqr_mont(f[j + t_points], f[j + t_points]);
			fp2mul_mont(f[j + t_points], g, f[j + t_points]);
		}
		x = x_;
		y = y_;
	}
	// Last iteration
	for (int j = 0; j < t_points; j++) {
		fpsub(Qj[j]->X[0], *x, g[0]);
		fpcopy(Qj[j]->X[1], g[1]);

		fp2sqr_mont(f[j + t_points], f[j + t_points]);
		fp2mul_mont(f[j + t_points], g, f[j + t_points]);
	}

	// Final exponentiation:
	mont_n_way_inv(f, 2 * t_points, finv);
	for (int j = 0; j < 2 * t_points; j++) {
		final_exponentiation_2_torsion(f[j], finv[j], f[j]);
	}
}
