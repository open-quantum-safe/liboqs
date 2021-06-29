/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: Torsion basis generation for compression
*********************************************************************************************/


#define COMPRESSION 0
#define DECOMPRESSION 1


static void Elligator2(const f2elm_t a24, const unsigned int r, f2elm_t x, unsigned char *bit, const unsigned char COMPorDEC)
{ // Generate an x-coordinate of a point on curve with (affine) coefficient a24 
  // Use a precomputed Elligator table of size TABLE_V3_LEN and switch to online computations if table runs out of elements.
  // Use the counter r
    int i;
    felm_t one_fp, a2, b2, N, temp0, temp1, rmonty = {0}, *U;
    f2elm_t A, y2, *t_ptr, v;

    fpcopy((digit_t*)&Montgomery_one, one_fp);
    fp2add(a24, a24, A);
    fpsub(A[0], one_fp, A[0]);
    fp2add(A, A, A);                          // A = 4*a24-2 

    // Elligator computation    
    if (r < TABLE_V3_LEN) {
        t_ptr = (f2elm_t *)&v_3_torsion[r];    
        fp2copy((felm_t*)t_ptr, v);
    } else { // Compute v = 1/(1+U*r^2)
        U = (felm_t *)U3;
        rmonty[0] = r;
        to_mont(rmonty, rmonty);
        fpsqr_mont(rmonty, rmonty);
        fpmul_mont(U[0], rmonty, v[0]);
        fpmul_mont(U[1], rmonty, v[1]);
        fpadd(v[0], (digit_t*)&Montgomery_one, v[0]);
        fp2inv_mont_bingcd(v);
    }
    fp2mul_mont(A, v, x);     // x = A*v; v := 1/(1 + U*r^2) table lookup
    fp2neg(x);                // x = -A*v;
    
    if (COMPorDEC == COMPRESSION) {
        fp2add(A, x, y2);                      // y2 = x + A
        fp2mul_mont(y2,  x,  y2);              // y2 = x*(x + A)
        fpadd(y2[0],  one_fp,  y2[0]);         // y2 = x(x + A) + 1
        fp2mul_mont(x, y2, y2);                // y2 = x*(x^2 + Ax + 1);
        fpsqr_mont(y2[0], a2);
        fpsqr_mont(y2[1], b2);
        fpadd(a2, b2, N);                      // N := norm(y2);

        fpcopy(N, temp0);
        for (i = 0; i < OALICE_BITS - 2; i++) {    
            fpsqr_mont(temp0,  temp0);
        }
        for (i = 0; i < OBOB_EXPON; i++) {
            fpsqr_mont(temp0,  temp1);
            fpmul_mont(temp0,  temp1,  temp0);
        }
        fpsqr_mont(temp0, temp1);              // z = N^((p + 1) div 4);
        fpcorrection(temp1);
        fpcorrection(N);
        if (memcmp(temp1, N, NBITS_TO_NBYTES(NBITS_FIELD)) != 0) {
            fp2neg(x);
            fp2sub(x, A, x);                   // x = -x - A;
            if (COMPorDEC == COMPRESSION)
                *bit = 1;        
        }
    } else {
        if (*bit) {
            fp2neg(x);
            fp2sub(x,A,x);                              // x = -x - A;
        }       
    }
}


static void TripleAndParabola_proj(const point_full_proj_t R, f2elm_t l1x, f2elm_t l1z)
{
    fp2sqr_mont(R->X, l1z);
    fp2add(l1z, l1z, l1x);
    fp2add(l1x, l1z, l1x);
    fpadd(l1x[0], (digit_t*)&Montgomery_one, l1x[0]);
    fp2add(R->Y, R->Y, l1z);
}


static void Tate3_proj(const point_full_proj_t P, const point_full_proj_t Q, f2elm_t gX, f2elm_t gZ)
{
    f2elm_t t0, l1x;

    TripleAndParabola_proj(P, l1x, gZ);
    fp2sub(Q->X, P->X, gX);
    fp2mul_mont(l1x, gX, gX);
    fp2sub(P->Y, Q->Y, t0);
    fp2mul_mont(gZ, t0, t0);
    fp2add(gX, t0, gX);
}


static void FinalExpo3_2way(f2elm_t *gX, f2elm_t *gZ)
{
    unsigned int i, j;
    f2elm_t f_[2], finv[2];

    for(i = 0; i < 2; i++) {
        fp2copy(gZ[i], f_[i]);
        fpneg(f_[i][1]);    // Conjugate
        fp2mul_mont(gX[i], f_[i], f_[i]);
    }
    mont_n_way_inv(f_,2,finv);
    for(i = 0; i < 2; i++) {
        fpneg(gX[i][1]);
        fp2mul_mont(gX[i], gZ[i], gX[i]);
        fp2mul_mont(gX[i], finv[i], gX[i]);
        for(j = 0; j < OALICE_BITS; j++)
            fp2sqr_mont(gX[i], gX[i]);
        for(j = 0; j < OBOB_EXPON-1; j++)
            cube_Fp2_cycl(gX[i], (digit_t*)&Montgomery_one);
    }
}


static void FinalExpo3(f2elm_t gX, f2elm_t gZ)
{
    unsigned int i;
    f2elm_t f_;

    fp2copy(gZ, f_);
    fpneg(f_[1]);
    fp2mul_mont(gX, f_, f_);
    fp2inv_mont_bingcd(f_);
    fpneg(gX[1]);
    fp2mul_mont(gX,gZ, gX);
    fp2mul_mont(gX,f_, gX);
    for(i = 0; i < OALICE_BITS; i++)
        fp2sqr_mont(gX, gX);
    for(i = 0; i < OBOB_EXPON-1; i++)
        cube_Fp2_cycl(gX, (digit_t*)Montgomery_one);
}


static void make_positive(f2elm_t x)
{
    unsigned long long nbytes = NBITS_TO_NBYTES(NBITS_FIELD);
    felm_t zero = {0};

    from_fp2mont(x, x);
    if (memcmp(x[0], zero, (size_t)nbytes) != 0) {
        if ((x[0][0] & 1) == 1)
            fp2neg(x);
    } else {
        if ((x[1][0] & 1) == 1)
            fp2neg(x);
    }
    to_fp2mont(x, x);
}


static bool FirstPoint_dual(const point_proj_t P, point_full_proj_t R, unsigned char *ind)
{
    point_full_proj_t R3,S3;
    f2elm_t gX[2],gZ[2];
    felm_t zero = {0};
    unsigned long long nbytes = NBITS_TO_NBYTES(NBITS_FIELD);
    unsigned char alpha,beta;

    fpcopy((digit_t*)B_gen_3_tors + 0*NWORDS_FIELD, (R3->X)[0]);
    fpcopy((digit_t*)B_gen_3_tors + 1*NWORDS_FIELD, (R3->X)[1]);
    fpcopy((digit_t*)B_gen_3_tors + 2*NWORDS_FIELD, (R3->Y)[0]);
    fpcopy((digit_t*)B_gen_3_tors + 3*NWORDS_FIELD, (R3->Y)[1]);
    fpcopy((digit_t*)B_gen_3_tors + 4*NWORDS_FIELD, (S3->X)[0]);
    fpcopy((digit_t*)B_gen_3_tors + 5*NWORDS_FIELD, (S3->X)[1]);
    fpcopy((digit_t*)B_gen_3_tors + 6*NWORDS_FIELD, (S3->Y)[0]);
    fpcopy((digit_t*)B_gen_3_tors + 7*NWORDS_FIELD, (S3->Y)[1]);

    CompletePoint(P,R);
    Tate3_proj(R3,R,gX[0],gZ[0]);
    Tate3_proj(S3,R,gX[1],gZ[1]);
    FinalExpo3_2way(gX,gZ);

    // Do small DLog with respect to g_R3_S3
    fp2correction(gX[0]);
    fp2correction(gX[1]);
    if (memcmp(gX[0][1], zero, (size_t)nbytes) == 0)    // = 1
        alpha = 0;
    else if (memcmp(gX[0][1], g_R_S_im, (size_t)nbytes) == 0)    // = g_R3_S3
        alpha = 1;
    else    // = g_R3_S3^2
        alpha = 2;

    if (memcmp(gX[1][1], zero, (size_t)nbytes) == 0)    // = 1
        beta = 0;
    else if (memcmp(gX[1][1], g_R_S_im, (size_t)nbytes) == 0)    // = g_R3_S3
        beta = 1;
    else    // = g_R3_S3^2
        beta = 2;

    if (alpha == 0 && beta == 0)   // Not full order
        return false;

    // Return the 3-torsion point that R lies above
    if (alpha == 0)                // Lies above R3
        *ind = 0;
    else if (beta == 0)            // Lies above S3
        *ind = 1;
    else if (alpha + beta == 3)    // Lies above R3+S3
        *ind = 3;
    else                           // Lies above R3-S3
        *ind = 2;

    return true;
}


static bool SecondPoint_dual(const point_proj_t P, point_full_proj_t R, unsigned char ind)
{
    point_full_proj_t RS3;
    f2elm_t gX, gZ;
    felm_t zero = {0};
    unsigned long long nbytes = NBITS_TO_NBYTES(NBITS_FIELD);

    // Pair with 3-torsion point determined by first point
    fpcopy((digit_t*)B_gen_3_tors + (4*ind + 0)*NWORDS_FIELD, (RS3->X)[0]);
    fpcopy((digit_t*)B_gen_3_tors + (4*ind + 1)*NWORDS_FIELD, (RS3->X)[1]);
    fpcopy((digit_t*)B_gen_3_tors + (4*ind + 2)*NWORDS_FIELD, (RS3->Y)[0]);
    fpcopy((digit_t*)B_gen_3_tors + (4*ind + 3)*NWORDS_FIELD, (RS3->Y)[1]);

    CompletePoint(P, R);
    Tate3_proj(RS3, R, gX, gZ);
    FinalExpo3(gX, gZ);

    fp2correction(gX);
    if (memcmp(gX[1], zero, (size_t)nbytes) != 0)    // Not equal to 1
        return true;
    else
        return false;
}


static void FirstPoint3n(const f2elm_t a24, const f2elm_t As[][5], f2elm_t x, point_full_proj_t R, unsigned int *r, unsigned char *ind, unsigned char *bitEll)
{
    bool b = false;
    point_proj_t P;
    felm_t zero = {0};
    *r = 0;    

    while (!b) {        
        *bitEll = 0;
        Elligator2(a24, *r, x, bitEll, COMPRESSION);    // Get x-coordinate on curve a24

        fp2copy(x, P->X);
        fpcopy((digit_t*)&Montgomery_one, (P->Z)[0]);
        fpcopy(zero, (P->Z)[1]);
        eval_full_dual_4_isog(As, P);    // Move x over to A = 0

        b = FirstPoint_dual(P, R, ind);  // Compute DLog with 3-torsion points
        *r = *r + 1;
    }
}


static void SecondPoint3n(const f2elm_t a24, const f2elm_t As[][5], f2elm_t x, point_full_proj_t R, unsigned int *r, unsigned char ind, unsigned char *bitEll)
{
    bool b = false;
    point_proj_t P;
    felm_t zero = {0};

    while (!b) {
        *bitEll = 0;
        Elligator2(a24, *r, x, bitEll, COMPRESSION);

        fp2copy(x, P->X);
        fpcopy((digit_t*)&Montgomery_one, (P->Z)[0]);
        fpcopy(zero, (P->Z)[1]);
        eval_full_dual_4_isog(As, P);    // Move x over to A = 0

        b = SecondPoint_dual(P, R, ind);
        *r = *r + 1;
    }
}


static void makeDiff(const point_full_proj_t R, point_full_proj_t S, const point_proj_t D)
{
    f2elm_t t0, t1, t2;
    unsigned long long nbytes = NBITS_TO_NBYTES(NBITS_FIELD);

    fp2sub(R->X, S->X, t0);
    fp2sub(R->Y, S->Y, t1);
    fp2sqr_mont(t0, t0);
    fp2sqr_mont(t1, t1);
    fp2add(R->X, S->X, t2);
    fp2mul_mont(t0, t2, t2);
    fp2sub(t1, t2, t1);
    fp2mul_mont(D->Z, t1, t1);
    fp2mul_mont(D->X, t0, t0);
    fp2correction(t0);
    fp2correction(t1);
    if (memcmp(t0[0], t1[0], (size_t)nbytes) == 0 && memcmp(t0[1], t1[1], (size_t)nbytes) == 0)
        fp2neg(S->Y);
}


static void BiQuad_affine(const f2elm_t a24, const f2elm_t x0, const f2elm_t x1, point_proj_t R)
{
    f2elm_t Ap2, aa, bb, cc, t0, t1;

    fp2add(a24, a24, Ap2);
    fp2add(Ap2, Ap2, Ap2);    // Ap2 = a+2 = 4*a24

    fp2sub(x0, x1, aa);
    fp2sqr_mont(aa, aa);

    fp2mul_mont(x0, x1, cc);
    fpsub(cc[0], (digit_t*)Montgomery_one, cc[0]);
    fp2sqr_mont(cc, cc);

    fpsub(x0[0], (digit_t*)Montgomery_one, bb[0]);
    fpcopy(x0[1], bb[1]);
    fp2sqr_mont(bb, bb);
    fp2mul_mont(Ap2, x0, t0);
    fp2add(bb, t0, bb);
    fp2mul_mont(x1, bb, bb);
    fpsub(x1[0], (digit_t*)Montgomery_one, t0[0]);
    fpcopy(x1[1], t0[1]);
    fp2sqr_mont(t0, t0);
    fp2mul_mont(Ap2, x1, t1);
    fp2add(t0, t1, t0);
    fp2mul_mont(x0, t0, t0);
    fp2add(bb, t0, bb);
    fp2add(bb, bb, bb);

    fp2sqr_mont(bb, t0);
    fp2mul_mont(aa, cc, t1);
    fp2add(t1, t1, t1);
    fp2add(t1, t1, t1);
    fp2sub(t0, t1, t0);
    sqrt_Fp2(t0, t0);
    make_positive(t0);    // Make the sqrt "positive"
    fp2add(bb, t0, R->X);
    fp2add(aa, aa, R->Z);
}


static void BuildOrdinary3nBasis_dual(const f2elm_t a24, const f2elm_t As[][5], point_full_proj_t *R, unsigned int *r, unsigned int *bitsEll)
{
    point_proj_t D;
    f2elm_t xs[2];
    unsigned char ind, bit;

    FirstPoint3n(a24, As, xs[0], R[0], r, &ind, &bit);
    *bitsEll = (unsigned int)bit;
    *(r+1) = *r;
    SecondPoint3n(a24, As, xs[1], R[1], r+1, ind, &bit);
    *bitsEll |= ((unsigned int)bit << 1);

    // Get x-coordinate of difference
    BiQuad_affine(a24, xs[0], xs[1], D);
    eval_full_dual_4_isog(As, D);    // Move x over to A = 0
    makeDiff(R[0], R[1], D);
}


static void BuildOrdinary3nBasis_Decomp_dual(const f2elm_t A24, point_proj_t *Rs, unsigned char *r, const unsigned char bitsEll)
{
    unsigned char bitEll[2];
    
    bitEll[0] = bitsEll & 0x1;
    bitEll[1] = (bitsEll >> 1) & 0x1;    
    
    // Elligator2 both x-coordinates
    Elligator2(A24, (unsigned int)r[0]-1, Rs[0]->X, &bitEll[0], DECOMPRESSION);
    Elligator2(A24, (unsigned int)r[1]-1, Rs[1]->X, &bitEll[1], DECOMPRESSION);
    // Get x-coordinate of difference
    BiQuad_affine(A24, Rs[0]->X, Rs[1]->X, Rs[2]);
}


static void get2mPointonEA(const f2elm_t A, f2elm_t x, felm_t r, f2elm_t t, unsigned char *vqnr, unsigned char *ind) 
{// Given a Montgomery curve EA, find a point of order 2^m using precomputed tables of size TABLE_R_LEN and switch to online computations if table runs out of elements.
    f2elm_t *tv_ptr, v, tmp;
    felm_t *tr_ptr, *u;

    u = (felm_t *)u_entang;
    // Select the correct tables, i.e., if A is a QR then v must be QNR, and vice-versa
    if (is_sqr_fp2(A,  tmp[0])) {
        tv_ptr = (f2elm_t *)table_v_qnr; 
        tr_ptr = (felm_t *)table_r_qnr; 
        *vqnr = 1;
    } else {
        tv_ptr = (f2elm_t *)table_v_qr;
        tr_ptr = (felm_t *)table_r_qr; 
        *vqnr = 0;
    }

    *ind = 0;
    do {
        if (*ind <= TABLE_R_LEN-1) {
            fp2copy((felm_t *)*tv_ptr++, v);
            fpcopy(tr_ptr[*ind], r);
        } else {
            do {
                fpadd(r, (digit_t*)Montgomery_one, r);
                fpmul_mont(r, r, tmp[1]);
                fpmul_mont(u[0], tmp[1], tmp[0]);
                fpmul_mont(u[1], tmp[1], tmp[1]);
                fpadd(tmp[0], (digit_t*)Montgomery_one, tmp[0]);
                fp2inv_mont_bingcd(tmp);
                fp2copy(tmp, v);     // v = 1/(1 + u*r^2)
                *ind += 1; // store the number of attempts for r so that we skip them during decompression
            } while (is_sqr_fp2(v, tmp[0]) == *vqnr);
        }
        fp2mul_mont(A, v, x);   
        fp2neg(x);                   // x = -A*v
        fp2add(x, A, tmp);        
        fp2mul_mont(x, tmp, tmp); 
        fpadd(tmp[0], (digit_t*)Montgomery_one, tmp[0]); 
        fp2mul_mont(x, tmp, t);      // t = x^3 + A*x^2 + x
        if (*ind < TABLE_R_LEN)
            *ind += 1;
    } while (!is_sqr_fp2(t, tmp[0]));
    *ind -= 1;
}


static void BuildEntangledXonly(const f2elm_t A, point_proj_t *R, unsigned char *qnr, unsigned char *ind)
{
    f2elm_t r, t;

    get2mPointonEA(A, R[0]->X, r[0], t, qnr, ind);

     // Get x1 = -x-A
    fp2add(R[0]->X, A, R[1]->X);
    fp2neg(R[1]->X);

    // Get difference x2,  z2
    fp2sub(R[0]->X, R[1]->X, R[2]->Z);
    fp2sqr_mont(R[2]->Z, R[2]->Z);

    fpcopy(r[0], r[1]);    // (1+i)*r
    fpadd((digit_t*)Montgomery_one, r[0], r[0]);
    fp2sqr_mont(r, r);
    fp2mul_mont(t, r, R[2]->X);
}


static void BuildOrdinary2nBasis_dual(const f2elm_t A, const f2elm_t Ds[][2], point_full_proj_t *Rs, unsigned char *qnr, unsigned char *ind)
{
    unsigned int i;
    felm_t t0;
    f2elm_t A6 = {0};
    point_proj_t xs[3] = {0};

    // Generate x-only entangled basis 
    BuildEntangledXonly(A, xs, qnr, ind);

    fpcopy((digit_t*)Montgomery_one, (xs[0]->Z)[0]);
    fpcopy((digit_t*)Montgomery_one, (xs[1]->Z)[0]);

    // Move them back to A = 6 
    for(i = 0; i < MAX_Bob; i++) {
        eval_3_isog(xs[0], Ds[MAX_Bob-1-i]);
        eval_3_isog(xs[1], Ds[MAX_Bob-1-i]);
        eval_3_isog(xs[2], Ds[MAX_Bob-1-i]);
    }

    // Recover y-coordinates with a single sqrt on A = 6
    fpcopy((digit_t*)Montgomery_one, A6[0]);
    fpadd(A6[0], A6[0], t0);
    fpadd(t0, t0, A6[0]);
    fpadd(A6[0], t0, A6[0]);

    CompleteMPoint(A6, xs[0]->X, xs[0]->Z, Rs[0]);
    RecoverY(A6, xs, Rs);
}


static void getrvOf2mPoint_Decomp(const unsigned char vqnr, const unsigned char ind, felm_t r, f2elm_t v) 
{// Given a Montgomery curve EA, find Elligator values r, v leading to a point of order 2^m. 
 // Use precomputed tables of size TABLE_R_LEN and switch to online computations if table runs out of elements.
    f2elm_t *tv_ptr, tmp;
    felm_t *tr_ptr, *u;

    u = (felm_t *)u_entang;
    // Select the correct tables
    if (vqnr == 1) {
        tv_ptr = (f2elm_t *)table_v_qnr; 
        tr_ptr = (felm_t *)table_r_qnr; 
    } else {
        tv_ptr = (f2elm_t *)table_v_qr;
        tr_ptr = (felm_t *)table_r_qr; 
    }

    if (ind < TABLE_R_LEN) {
        fp2copy(tv_ptr[ind], v);
        fpcopy(tr_ptr[ind], r);
    } else {
        fpcopy(tr_ptr[TABLE_R_LEN-1], r);
        for (int k = 0; k < ind - TABLE_R_LEN + 1; k++)
            fpadd(r, (digit_t*)Montgomery_one, r);

        fpmul_mont(r, r, tmp[1]);
        fpmul_mont(u[0], tmp[1], tmp[0]);
        fpmul_mont(u[1], tmp[1], v[1]);
        fpadd(tmp[0], (digit_t*)Montgomery_one, v[0]); // v^-1 = (1 + u*r^2)
    }           
}


static void BuildEntangledXonly_Decomp(const f2elm_t A, point_proj_t *R, unsigned char qnr, unsigned char ind)
{
    f2elm_t r, t, v;

    getrvOf2mPoint_Decomp(qnr, ind, r[0], v);

    if (ind < TABLE_R_LEN) {
       // Get x0 = -A*v
        fp2mul_mont(A, v, R[0]->X);    
        fp2neg(R[0]->X);               
        fp2add(R[0]->X, A, t);
        fp2mul_mont(R[0]->X, t, t);
        fpadd(t[0], (digit_t*)Montgomery_one, t[0]);
        fp2mul_mont(R[0]->X, t, t);    // t = x0^3 + A*x0^2 + x0

        // Get x1 = -x0-A
        fp2add(R[0]->X, A, R[1]->X);
        fp2neg(R[1]->X);

        // Get difference x2,z2 
        fp2sub(R[0]->X, R[1]->X, R[2]->Z);
        fp2sqr_mont(R[2]->Z, R[2]->Z);

        fpcopy(r[0],r[1]); // (1+i)*r
        fpadd((digit_t*)Montgomery_one, r[0], r[0]);
        fp2sqr_mont(r, r);
        fp2mul_mont(t, r, R[2]->X);
    } else {
        // Get X0, Z0
        fp2copy(A, R[0]->X);
        fp2neg(R[0]->X);
        fp2copy(v, R[0]->Z);

        // Get X1, Z1 
        fp2copy(v, R[1]->X);
        fp2neg(R[1]->X);  
        fpadd((digit_t*)Montgomery_one, R[1]->X[0], R[1]->X[0]);
        fp2mul_mont(R[1]->X, A, R[1]->X);
        fp2copy(v, R[1]->Z);

        // Get difference X2,Z2 
        fpcopy(r[0],r[1]); // (1+i)*r 
        fpadd((digit_t*)Montgomery_one, r[0], r[0]);
        fp2sqr_mont(r, R[2]->X);
        fp2copy(v, r);
        fpsub(r[0], (digit_t*)Montgomery_one, r[0]);
        fp2sqr_mont(A, t);
        fp2mul_mont(t, r, r);
        fp2sqr_mont(v, t);
        fp2sub(r, t, r);
        fp2mul_mont(R[2]->X, r, R[2]->X);
        fp2mul_mont(A, v, R[2]->Z);
        fp2copy(v, r);
        fpsub(r[0], (digit_t*)Montgomery_one, r[0]);
        fpsub(r[0], (digit_t*)Montgomery_one, r[0]);
        fp2sqr_mont(r, r);
        fp2mul_mont(R[2]->Z, r, R[2]->Z);
    }
}
