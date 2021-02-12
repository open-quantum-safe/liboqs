/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: pairing computation for compression
*********************************************************************************************/


#define t_points  2


static void final_exponentiation_2_torsion(f2elm_t f, const f2elm_t finv, f2elm_t fout)
{ // The final exponentiation for pairings in the 2^eA-torsion group. Raising the value f to the power (p^2-1)/2^eA.
    felm_t one = {0};
    f2elm_t temp = {0};
    unsigned int i; 

    fpcopy((digit_t*)&Montgomery_one, one);
    
    // f = f^p
    fp2_conj(f, temp);
    fp2mul_mont(temp, finv, temp);              // temp = f^(p-1)

    for (i = 0; i < OBOB_EXPON; i++) {
        cube_Fp2_cycl(temp, one);
    }
    fp2copy(temp, fout);
}


static void final_exponentiation_3_torsion(f2elm_t f, const f2elm_t finv, f2elm_t fout)
{ // The final exponentiation for pairings in the 3-torsion group. Raising the value f to the power (p^2-1)/3^eB.
    felm_t one = {0};
    f2elm_t temp;
    unsigned int i; 

    fpcopy((digit_t*)&Montgomery_one, one);
    
    // f = f^p
    fp2_conj(f, temp); 
    fp2mul_mont(temp, finv, temp);              // temp = f^(p-1)

    for (i = 0; i < OALICE_BITS; i++) {
        sqr_Fp2_cycl(temp, one);
    }
    fp2copy(temp, fout);
}


void Tate3_pairings(point_full_proj_t *Qj, f2elm_t* f)
{
    felm_t *x, *y, *l1, *l2, *n1, *n2, *x2, *x23, *x2p3;
    f2elm_t xQ2s[t_points], finv[2*t_points], one = {0};
    f2elm_t t0, t1, t2, t3, t4, t5, g, h, tf;

    fpcopy((digit_t*)&Montgomery_one, one[0]);

    for (int j = 0; j < t_points; j++) {
        fp2copy(one, f[j]);
        fp2copy(one, f[j+t_points]);
        fp2sqr_mont(Qj[j]->X, xQ2s[j]);
    }

    for (int k = 0; k < OBOB_EXPON - 1; k++) {
        l1   = (felm_t*)T_tate3 + 6*k + 0;
        l2   = (felm_t*)T_tate3 + 6*k + 1;
        n1   = (felm_t*)T_tate3 + 6*k + 2;
        n2   = (felm_t*)T_tate3 + 6*k + 3;
        x23  = (felm_t*)T_tate3 + 6*k + 4;
        x2p3 = (felm_t*)T_tate3 + 6*k + 5;
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

            fp2sqr_mont(f[j+t_points], tf);
            fp2mul_mont(f[j+t_points], tf, f[j+t_points]);
            fp2mul_mont(f[j+t_points], g, f[j+t_points]);
        }
    }
    for (int j = 0; j < t_points; j++) {
        x  = (felm_t*)T_tate3 + 6*(OBOB_EXPON-1) + 0;
        y  = (felm_t*)T_tate3 + 6*(OBOB_EXPON-1) + 1;
        l1 = (felm_t*)T_tate3 + 6*(OBOB_EXPON-1) + 2;
        x2 = (felm_t*)T_tate3 + 6*(OBOB_EXPON-1) + 3;
        
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

        fp2sqr_mont(f[j+t_points], tf);
        fp2mul_mont(f[j+t_points], tf, f[j+t_points]);
        fp2mul_mont(f[j+t_points], g, f[j+t_points]);
    }

    // Final exponentiation:
    mont_n_way_inv(f, 2*t_points, finv);
    for (int j = 0; j < 2*t_points; j++) {
        final_exponentiation_3_torsion(f[j], finv[j], f[j]);
    }
}


void Tate2_pairings(const point_t P, const point_t Q, point_full_proj_t *Qj, f2elm_t* f)
{
    felm_t *x, *y, *x_, *y_, *l1;
    f2elm_t finv[2*t_points], one = {0};
    f2elm_t *x_first, *y_first, l1_first, t0, t1, g, h;
    
    fpcopy((digit_t*)&Montgomery_one, one[0]);

    for (int j = 0; j < t_points; j++) {
        fp2copy(one, f[j]);
        fp2copy(one, f[j+t_points]);
    }

    // Pairings with P
    x_first = (f2elm_t*)P->x;
    y_first = (f2elm_t*)P->y;

    x_ = (felm_t*)T_tate2_firststep_P + 0;
    y_ = (felm_t*)T_tate2_firststep_P + 1;
    fpcopy((digit_t*)T_tate2_firststep_P + 2*NWORDS_FIELD, l1_first[0]);         
    fpcopy((digit_t*)T_tate2_firststep_P + 3*NWORDS_FIELD, l1_first[1]);         
    
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
        x_ = (felm_t*)T_tate2_P + 3 * k + 0;
        y_ = (felm_t*)T_tate2_P + 3 * k + 1;
        l1 = (felm_t*)T_tate2_P + 3 * k + 2;
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
    x_first = (f2elm_t*)Q->x;
    y_first = (f2elm_t*)Q->y; 
    x_ = (felm_t*)T_tate2_firststep_Q + 0;
    y_ = (felm_t*)T_tate2_firststep_Q + 1;
    fpcopy(((felm_t*)T_tate2_firststep_Q)[2], l1_first[0]);
    fpcopy(((felm_t*)T_tate2_firststep_Q)[3], l1_first[1]);

    for (int j = 0; j < t_points; j++) {
        fp2sub(Qj[j]->X, *x_first, t0);
        fp2sub(Qj[j]->Y, *y_first, t1);
        fp2mul_mont(l1_first, t0, t0);
        fp2sub(t0, t1, g);

        fpsub(Qj[j]->X[0], *x_, h[0]);
        fpcopy(Qj[j]->X[1], h[1]);
        fpneg(h[1]);
        fp2mul_mont(g, h, g);

        fp2sqr_mont(f[j+t_points], f[j+t_points]);
        fp2mul_mont(f[j+t_points], g, f[j+t_points]);
    }
    x = x_;
    y = y_;
    
    for (int k = 0; k < OALICE_BITS - 2; k++) {
        x_ = (felm_t*)T_tate2_Q + 3*k + 0;
        y_ = (felm_t*)T_tate2_Q + 3*k + 1;
        l1 = (felm_t*)T_tate2_Q + 3*k + 2;
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

            fp2sqr_mont(f[j+t_points], f[j+t_points]);
            fp2mul_mont(f[j+t_points], g, f[j+t_points]);
        }
        x = x_;
        y = y_;
    }
    // Last iteration
    for (int j = 0; j < t_points; j++) {
        fpsub(Qj[j]->X[0], *x, g[0]);
        fpcopy(Qj[j]->X[1], g[1]);

        fp2sqr_mont(f[j+t_points], f[j+t_points]);
        fp2mul_mont(f[j+t_points], g, f[j+t_points]);
    }

    // Final exponentiation:
    mont_n_way_inv(f, 2*t_points, finv);
    for (int j = 0; j < 2*t_points; j++) {
        final_exponentiation_2_torsion(f[j], finv[j], f[j]);
    }
}

