/*************************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: ephemeral supersingular isogeny Diffie-Hellman key exchange (SIDH) using compression
**************************************************************************************************/ 

#include <string.h>

#define COMPRESSION 0
#define DECOMPRESSION 1


static void init_basis(digit_t *gen, f2elm_t XP, f2elm_t XQ, f2elm_t XR)
{ // Initialization of basis points
    
    fpcopy(gen,                  XP[0]);
    fpcopy(gen +   NWORDS_FIELD, XP[1]);
    fpcopy(gen + 2*NWORDS_FIELD, XQ[0]);
    fpcopy(gen + 3*NWORDS_FIELD, XQ[1]);
    fpcopy(gen + 4*NWORDS_FIELD, XR[0]);
    fpcopy(gen + 5*NWORDS_FIELD, XR[1]);
}


static void FormatPrivKey_B(unsigned char *skB) 
{
    skB[SECRETKEY_B_BYTES-2] &= MASK3_BOB;
    skB[SECRETKEY_B_BYTES-1] &= MASK2_BOB;    // Clear necessary bits so that 3*ephemeralsk is still less than Bob_order
    mul3(skB);                                // Multiply ephemeralsk by 3        
}


void random_mod_order_A(unsigned char* random_digits)
{  // Generation of Alice's secret key  
   // Outputs random value in [0, 2^eA - 1]

    memset(random_digits, 0, SECRETKEY_A_BYTES);
    OQS_randombytes(random_digits, SECRETKEY_A_BYTES);
    random_digits[0] &= 0xFE;                            // Make private scalar even    
    random_digits[SECRETKEY_A_BYTES-1] &= MASK_ALICE;    // Masking last byte     
}


void random_mod_order_B(unsigned char* random_digits)
{  // Generation of Bob's secret key  
   // Outputs random value in [0, 2^Floor(Log(2, oB)) - 1]
    
    OQS_randombytes(random_digits, SECRETKEY_B_BYTES);
    FormatPrivKey_B(random_digits);
}


static void Ladder3pt_dual(const point_proj_t *Rs, const digit_t* m, const unsigned int AliceOrBob, point_proj_t R, const f2elm_t A24)
{ // Project 3-point ladder
    point_proj_t R0 = {0}, R2 = {0};
    digit_t mask;
    int i, nbits, bit, swap, prevbit = 0;

    if (AliceOrBob == ALICE) {
        nbits = OALICE_BITS;
    } else {
        nbits = OBOB_BITS;
    }

    fp2copy(Rs[1]->X, R0->X);
    fp2copy(Rs[1]->Z, R0->Z);
    fp2copy(Rs[2]->X, R2->X);
    fp2copy(Rs[2]->Z, R2->Z);
    fp2copy(Rs[0]->X, R->X);
    fp2copy(Rs[0]->Z, R->Z);

    // Main loop
    for (i = 0; i < nbits; i++) {
        bit = (m[i >> LOG2RADIX] >> (i & (RADIX-1))) & 1;
        swap = bit ^ prevbit;
        prevbit = bit;
        mask = 0 - (digit_t)swap;

        swap_points(R, R2, mask);
        xDBLADD(R0, R2, R->X, A24);
        fp2mul_mont(R2->X, R->Z, R2->X);
    }
    swap = 0 ^ prevbit;
    mask = 0 - (digit_t)swap;
    swap_points(R, R2, mask);
}


static void Elligator2(const f2elm_t a24, const unsigned int r, f2elm_t x, unsigned char *bit, const unsigned char COMPorDEC)
{ // Generate an x-coordinate of a point on curve with (affine) coefficient a24 
  // Use the counter r
    int i;
    felm_t one_fp, a2, b2, N, temp0, temp1;
    f2elm_t A, y2, *t_ptr;

    fpcopy((digit_t*)&Montgomery_one, one_fp);
    fp2add(a24, a24, A);
    fpsub(A[0], one_fp, A[0]);
    fp2add(A, A, A);                       // A = 4*a24-2 

    // Elligator computation
    t_ptr = (f2elm_t *)&v_3_torsion[r];    
    fp2mul_mont(A, (felm_t*)t_ptr, x);     // x = A*v; v := 1/(1 + U*r^2) table lookup
    fp2neg(x);                             // x = -A*v;
    
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


static void get_4_isog_dual(const point_proj_t P, f2elm_t A24, f2elm_t C24, f2elm_t* coeff)
{
    fp2sub(P->X, P->Z, coeff[1]);
    fp2add(P->X, P->Z, coeff[2]);
    fp2sqr_mont(P->Z, coeff[4]);
    fp2add(coeff[4], coeff[4], coeff[0]);
    fp2sqr_mont(coeff[0], C24);
    fp2add(coeff[0], coeff[0], coeff[0]);
    fp2sqr_mont(P->X, coeff[3]);
    fp2add(coeff[3], coeff[3], A24);
    fp2sqr_mont(A24, A24);
}

#if (OALICE_BITS % 2 == 1)

static void eval_dual_2_isog(const f2elm_t X2, const f2elm_t Z2, point_proj_t P)
{
    f2elm_t t0;

    fp2add(P->X, P->Z, t0);
    fp2sub(P->X, P->Z, P->Z);
    fp2sqr_mont(t0, t0);
    fp2sqr_mont(P->Z, P->Z);
    fp2sub(t0, P->Z, P->Z);
    fp2mul_mont(X2, P->Z, P->Z);
    fp2mul_mont(Z2, t0, P->X);
}

#endif

static void eval_final_dual_2_isog(point_proj_t P)
{
    f2elm_t t0, t1;
    felm_t t2;

    fp2add(P->X, P->Z, t0);
    fp2mul_mont(P->X, P->Z, t1);
    fp2sqr_mont(t0, P->X);
    fpcopy((P->X)[0], t2);
    fpcopy((P->X)[1], (P->X)[0]);
    fpcopy(t2, (P->X)[1]);
    fpneg((P->X)[1]);
    fp2add(t1, t1, P->Z);
    fp2add(P->Z, P->Z, P->Z);
}


static void eval_dual_4_isog_shared(const f2elm_t X4pZ4, const f2elm_t X42, const f2elm_t Z42, f2elm_t* coeff)
{
    fp2sub(X42, Z42, coeff[0]);
    fp2add(X42, Z42, coeff[1]);
    fp2sqr_mont(X4pZ4, coeff[2]);
    fp2sub(coeff[2], coeff[1], coeff[2]);
}


static void eval_dual_4_isog(const f2elm_t A24, const f2elm_t C24, const f2elm_t* coeff, point_proj_t P)
{
    f2elm_t t0, t1, t2, t3;

    fp2add(P->X, P->Z, t0);
    fp2sub(P->X, P->Z, t1);
    fp2sqr_mont(t0, t0);
    fp2sqr_mont(t1, t1);
    fp2sub(t0, t1, t2);
    fp2sub(C24, A24, t3);
    fp2mul_mont(t2, t3, t3);
    fp2mul_mont(C24, t0, t2);
    fp2sub(t2, t3, t2);
    fp2mul_mont(t2, t0, P->X);
    fp2mul_mont(t3, t1, P->Z);
    fp2mul_mont(coeff[0], P->X, P->X);
    fp2mul_mont(coeff[1], P->Z, t0);
    fp2add(P->X, t0, P->X);
    fp2mul_mont(coeff[2], P->Z, P->Z);
}


static void eval_full_dual_4_isog(const f2elm_t As[][5], point_proj_t P)
{
    // First all 4-isogenies
    for(unsigned int i = 0; i < MAX_Alice; i++) {
        eval_dual_4_isog(As[MAX_Alice-i][0], As[MAX_Alice-i][1], *(As+MAX_Alice-i-1)+2, P);
    }
#if (OALICE_BITS % 2 == 1)
    eval_dual_2_isog(As[MAX_Alice][2], As[MAX_Alice][3], P);
#endif
    eval_final_dual_2_isog(P);    // to A = 0
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
    else if (memcmp(gX[0][1], (digit_t*)g_R_S_im, (size_t)nbytes) == 0)    // = g_R3_S3
        alpha = 1;
    else    // = g_R3_S3^2
        alpha = 2;

    if (memcmp(gX[1][1], zero, (size_t)nbytes) == 0)    // = 1
        beta = 0;
    else if (memcmp(gX[1][1], (digit_t*)g_R_S_im, (size_t)nbytes) == 0)    // = g_R3_S3
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


static void FullIsogeny_A_dual(unsigned char* PrivateKeyA, f2elm_t As[][5], f2elm_t a24, unsigned int sike)
{
  // Input:  a private key PrivateKeyA in the range [0, 2^eA - 1]. 
  // Output: the public key PublicKeyA consisting of 3 elements in GF(p^2) which are encoded by removing leading 0 bytes.
    point_proj_t R, pts[MAX_INT_POINTS_ALICE];
    f2elm_t XPA, XQA, XRA, coeff[5], A24 = {0}, C24 = {0}, A = {0};
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_ALICE], npts = 0, ii = 0;
    digit_t SecretKeyA[NWORDS_ORDER] = {0};

    // Initialize basis points
    init_basis((digit_t*)A_gen, XPA, XQA, XRA);

    // Initialize constants: A24 = A+2C, C24 = 4C, where A=6, C=1
    fpcopy((digit_t*)&Montgomery_one, A24[0]);
    fp2add(A24, A24, A24);
    fp2add(A24, A24, C24);
    fp2add(A24, C24, A);
    fp2add(C24, C24, A24);
    
    // Retrieve kernel point
    decode_to_digits(PrivateKeyA, SecretKeyA, SECRETKEY_A_BYTES, NWORDS_ORDER);
    LADDER3PT(XPA, XQA, XRA, SecretKeyA, ALICE, R, A);
    fp2inv_mont(R->Z);
    fp2mul_mont(R->X,R->Z,R->X);
    fpcopy((digit_t*)&Montgomery_one, R->Z[0]);
    fpzero(R->Z[1]); 
    if (sike == 1)
        fp2_encode(R->X, &PrivateKeyA[SECRETKEY_A_BYTES + CRYPTO_PUBLICKEYBYTES]);  // privA ||= x(KA) = x(PA + sk_A*QA)
    
#if (OALICE_BITS % 2 == 1)
    point_proj_t S;

    xDBLe(R, S, A24, C24, (int)(OALICE_BITS-1));
    get_2_isog(S, A24, C24);
    eval_2_isog(R, S);
    fp2copy(S->X, As[MAX_Alice][2]);
    fp2copy(S->Z, As[MAX_Alice][3]);
#endif

    // Traverse tree
    index = 0;
    for (row = 1; row < MAX_Alice; row++) {
        while (index < MAX_Alice-row) {
            fp2copy(R->X, pts[npts]->X);
            fp2copy(R->Z, pts[npts]->Z);
            pts_index[npts++] = index;
            m = strat_Alice[ii++];
            xDBLe(R, R, A24, C24, (int)(2*m));
            index += m;
        }

        fp2copy(A24, As[row-1][0]);
        fp2copy(C24, As[row-1][1]);
        get_4_isog_dual(R, A24, C24, coeff);
        for (i = 0; i < npts; i++) {
            eval_4_isog(pts[i], coeff);
        }
        eval_dual_4_isog_shared(coeff[2], coeff[3], coeff[4], *(As+row-1)+2);

        fp2copy(pts[npts-1]->X, R->X);
        fp2copy(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }

    fp2copy(A24, As[MAX_Alice-1][0]);
    fp2copy(C24, As[MAX_Alice-1][1]);
    get_4_isog_dual(R, A24, C24, coeff);
    eval_dual_4_isog_shared(coeff[2], coeff[3], coeff[4], *(As+MAX_Alice-1)+2);
    fp2copy(A24, As[MAX_Alice][0]);
    fp2copy(C24, As[MAX_Alice][1]);
    fp2inv_mont_bingcd(C24);
    fp2mul_mont(A24, C24, a24);
}


static void Dlogs3_dual(const f2elm_t *f, int *D, digit_t *d0, digit_t *c0, digit_t *d1, digit_t *c1)
{
    solve_dlog(f[0], D, d0, 3);    
    solve_dlog(f[2], D, c0, 3);
    solve_dlog(f[1], D, d1, 3);     
    solve_dlog(f[3], D, c1, 3);    
    mp_sub((digit_t*)Bob_order, c0, c0, NWORDS_ORDER);    
    mp_sub((digit_t*)Bob_order, c1, c1, NWORDS_ORDER);  
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


static void PKADecompression_dual(const unsigned char* SecretKeyB, const unsigned char* CompressedPKA, point_proj_t R, f2elm_t A)
{
    unsigned char bit, rs[3];
    f2elm_t A24;
    point_proj_t Rs[3] = {0};
    digit_t t1[NWORDS_ORDER] = {0}, t2[NWORDS_ORDER] = {0}, t3[NWORDS_ORDER] = {0}, t4[NWORDS_ORDER] = {0};
    digit_t vone[NWORDS_ORDER] = {0}, temp[NWORDS_ORDER] = {0}, SKin[NWORDS_ORDER] = {0};
    
    fp2_decode(&CompressedPKA[3*ORDER_B_ENCODED_BYTES], A);
    vone[0] = 1;
    to_Montgomery_mod_order(vone, vone, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);  // Converting to Montgomery representation
    
    bit = CompressedPKA[3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES] >> 7;
    memcpy(rs, &CompressedPKA[3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES], 3);
    rs[0] &= 0x7F;

    fpadd(A[0], (digit_t*)Montgomery_one, A24[0]);
    fpcopy(A[1], A24[1]);
    fpadd(A24[0], (digit_t*)Montgomery_one, A24[0]);
    fp2div2(A24, A24);
    fp2div2(A24, A24);

    BuildOrdinary3nBasis_Decomp_dual(A24, Rs, rs, rs[2]);
    fpcopy((digit_t*)Montgomery_one, (Rs[0]->Z)[0]);
    fpcopy((digit_t*)Montgomery_one, (Rs[1]->Z)[0]);

    swap_points(Rs[0], Rs[1], 0-(digit_t)bit);
    decode_to_digits(SecretKeyB, SKin, SECRETKEY_B_BYTES, NWORDS_ORDER);    
    to_Montgomery_mod_order(SKin, t1, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);    // Converting to Montgomery representation 
    decode_to_digits(&CompressedPKA[0], temp, ORDER_B_ENCODED_BYTES, NWORDS_ORDER);    
    to_Montgomery_mod_order(temp, t2, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
    decode_to_digits(&CompressedPKA[ORDER_B_ENCODED_BYTES], temp, ORDER_B_ENCODED_BYTES, NWORDS_ORDER);    
    to_Montgomery_mod_order(temp, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
    decode_to_digits(&CompressedPKA[2*ORDER_B_ENCODED_BYTES], temp, ORDER_B_ENCODED_BYTES, NWORDS_ORDER);    
    to_Montgomery_mod_order(temp, t4, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
    if (bit == 0) {    
        Montgomery_multiply_mod_order(t1, t3, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        mp_add(t3, vone, t3, NWORDS_ORDER);
        Montgomery_inversion_mod_order_bingcd(t3, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
        Montgomery_multiply_mod_order(t1, t4, t4, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        mp_add(t2, t4, t4, NWORDS_ORDER);
        Montgomery_multiply_mod_order(t3, t4, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(t3, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);    // Converting back from Montgomery representation        
        Ladder3pt_dual(Rs,t3,BOB,R,A24);
    } else {   
        Montgomery_multiply_mod_order(t1, t4, t4, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        mp_add(t4, vone, t4, NWORDS_ORDER);
        Montgomery_inversion_mod_order_bingcd(t4, t4, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
        Montgomery_multiply_mod_order(t1, t3, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        mp_add(t2, t3, t3, NWORDS_ORDER);
        Montgomery_multiply_mod_order(t3, t4, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(t3, t3, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);    // Converting back from Montgomery representation 
        Ladder3pt_dual(Rs,t3,BOB,R,A24);
    }
    Double(R, R, A24, OALICE_BITS);    // x, z := Double(A24, x, 1, eA);
}


static void Compress_PKA_dual(digit_t *d0, digit_t *c0, digit_t *d1, digit_t *c1, f2elm_t a24, unsigned int *rs, unsigned char *CompressedPKA)
{
    unsigned int bit;
    digit_t temp[NWORDS_ORDER] = {0}, inv[NWORDS_ORDER];
    f2elm_t A;

    fp2add(a24,a24,A);
    fp2add(A,A,A);
    fpsub(A[0],(digit_t*)Montgomery_one,A[0]);
    fpsub(A[0],(digit_t*)Montgomery_one,A[0]);    // 4*a24-2
    
    bit = mod3(d1);
    to_Montgomery_mod_order(c0, c0, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);   // Converting to Montgomery representation
    to_Montgomery_mod_order(c1, c1, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
    to_Montgomery_mod_order(d0, d0, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
    to_Montgomery_mod_order(d1, d1, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
    
    if (bit != 0) {  // Storing [d1*c0inv, c1*c0inv, d0*c0inv] and setting bit "NBITS_ORDER" to 0   
        Montgomery_inversion_mod_order_bingcd(d1, inv, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
        Montgomery_neg(d0, (digit_t*)&Bob_order);
        Montgomery_multiply_mod_order(d0, inv, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(temp, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);                    // Converting back from Montgomery representation
        encode_to_bytes(temp, &CompressedPKA[0], ORDER_B_ENCODED_BYTES);
        Montgomery_neg(c1, (digit_t*)&Bob_order);
        Montgomery_multiply_mod_order(c1, inv, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(temp, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        encode_to_bytes(temp, &CompressedPKA[ORDER_B_ENCODED_BYTES], ORDER_B_ENCODED_BYTES);
        Montgomery_multiply_mod_order(c0, inv, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(temp, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        encode_to_bytes(temp, &CompressedPKA[2*ORDER_B_ENCODED_BYTES], ORDER_B_ENCODED_BYTES);
        CompressedPKA[3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES] = 0x00;
    } else {  // Storing [d1*d0inv, c1*d0inv, c0*d0inv] and setting bit "NBITS_ORDER" to 1
        Montgomery_inversion_mod_order_bingcd(d0, inv, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2, (digit_t*)&Montgomery_RB1);
        Montgomery_neg(d1, (digit_t*)&Bob_order);
        Montgomery_multiply_mod_order(d1, inv, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(temp, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);                     // Converting back from Montgomery representation         
        encode_to_bytes(temp, &CompressedPKA[0], ORDER_B_ENCODED_BYTES); 
        Montgomery_multiply_mod_order(c1, inv, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(temp, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        encode_to_bytes(temp, &CompressedPKA[ORDER_B_ENCODED_BYTES], ORDER_B_ENCODED_BYTES); 
        Montgomery_neg(c0, (digit_t*)&Bob_order);
        Montgomery_multiply_mod_order(c0, inv, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        from_Montgomery_mod_order(temp, temp, (digit_t*)Bob_order, (digit_t*)&Montgomery_RB2);
        encode_to_bytes(temp, &CompressedPKA[2*ORDER_B_ENCODED_BYTES], ORDER_B_ENCODED_BYTES);
        CompressedPKA[3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES] = 0x80;
    }

    fp2_encode(A, &CompressedPKA[3*ORDER_B_ENCODED_BYTES]);    
    CompressedPKA[3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES] |= (unsigned char)rs[0];
    CompressedPKA[3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES + 1] = (unsigned char)rs[1];
    CompressedPKA[3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES + 2] = (unsigned char)rs[2];
}


static int EphemeralKeyGeneration_A_extended(unsigned char* PrivateKeyA, unsigned char* CompressedPKA)
{ // Alice's ephemeral public key generation using compression -- SIKE protocol
  // Output: PrivateKeyA[MSG_BYTES + SECRETKEY_A_BYTES] <- x(K_A) where K_A = PA + sk_A*Q_A 
    unsigned int rs[3];
    int D[DLEN_3];
    f2elm_t a24, As[MAX_Alice+1][5], f[4];
    digit_t c0[NWORDS_ORDER] = {0}, d0[NWORDS_ORDER] = {0}, c1[NWORDS_ORDER] = {0}, d1[NWORDS_ORDER] = {0}; 
    point_full_proj_t Rs[2];

    FullIsogeny_A_dual(PrivateKeyA, As, a24, 1);
    BuildOrdinary3nBasis_dual(a24, As, Rs, rs, &rs[2]);
    Tate3_pairings(Rs, f);
    Dlogs3_dual(f, D, d0, c0, d1, c1);
    Compress_PKA_dual(d0, c0, d1, c1, a24, rs, CompressedPKA);
    return 0;
}


int EphemeralKeyGeneration_A(const unsigned char* PrivateKeyA, unsigned char* CompressedPKA)
{ // Alice's ephemeral public key generation using compression -- SIDH protocol
  // Output: PrivateKeyA[MSG_BYTES + SECRETKEY_A_BYTES] <- x(K_A) where K_A = PA + sk_A*Q_A 
    unsigned int rs[3];
    int D[DLEN_3];
    f2elm_t a24, As[MAX_Alice+1][5], f[4];
    digit_t c0[NWORDS_ORDER] = {0}, d0[NWORDS_ORDER] = {0}, c1[NWORDS_ORDER] = {0}, d1[NWORDS_ORDER] = {0}; 
    point_full_proj_t Rs[2];

    FullIsogeny_A_dual((unsigned char*)PrivateKeyA, As, a24, 0);
    BuildOrdinary3nBasis_dual(a24, As, Rs, rs, &rs[2]);
    Tate3_pairings(Rs, f);
    Dlogs3_dual(f, D, d0, c0, d1, c1);
    Compress_PKA_dual(d0, c0, d1, c1, a24, rs, CompressedPKA);
    return 0;
}


int EphemeralSecretAgreement_B(const unsigned char* PrivateKeyB, const unsigned char* PKA, unsigned char* SharedSecretB)
{ // Bob's ephemeral shared secret computation using compression
  // It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's decompressed data point_R and param_A
  // Inputs: Bob's PrivateKeyB is an integer in the range [1, oB-1], where oB = 3^OBOB_EXP. 
  //         Alice's decompressed data consists of point_R in (X:Z) coordinates and the curve parameter param_A in GF(p^2).
  // Output: a shared secret SharedSecretB that consists of one element in GF(p^2). 
    unsigned int i, ii = 0, row, m, index = 0, pts_index[MAX_INT_POINTS_BOB], npts = 0;
    f2elm_t A24plus = {0}, A24minus = {0};
    point_proj_t R, pts[MAX_INT_POINTS_BOB];
    f2elm_t jinv, A, coeff[3];
    f2elm_t param_A = {0};

    PKADecompression_dual(PrivateKeyB, PKA, R, param_A);
    
    fp2copy((felm_t*)param_A, A);    
    fpadd((digit_t*)&Montgomery_one, (digit_t*)&Montgomery_one, A24minus[0]);
    fp2add(A, A24minus, A24plus);
    fp2sub(A, A24minus, A24minus);
        
    // Traverse tree
    index = 0;
    for (row = 1; row < MAX_Bob; row++) {
        while (index < MAX_Bob-row) {
            fp2copy(R->X, pts[npts]->X);
            fp2copy(R->Z, pts[npts]->Z);
            pts_index[npts++] = index;
            m = strat_Bob[ii++];
            xTPLe(R, R, A24minus, A24plus, (int)m);
            index += m;
        }
        get_3_isog(R, A24minus, A24plus, coeff);

        for (i = 0; i < npts; i++) {
            eval_3_isog(pts[i], coeff);
        } 

        fp2copy(pts[npts-1]->X, R->X);
        fp2copy(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }
     
    get_3_isog(R, A24minus, A24plus, coeff);
    fp2add(A24plus, A24minus, A);
    fp2add(A, A, A);
    fp2sub(A24plus, A24minus, A24plus);
    j_inv(A, A24plus, jinv);    
    fp2_encode(jinv, SharedSecretB);    // Format shared secret
      
    return 0;
}


static void BuildEntangledXonly(const f2elm_t A, point_proj_t *R, unsigned char *qnr, unsigned char *ind)
{
    felm_t s;
    f2elm_t *t_ptr, r, t;

    // Select the correct table
    if (is_sqr_fp2(A,  s)) {
        t_ptr = (f2elm_t *)table_v_qnr; 
        *qnr = 1;
    } else {
        t_ptr = (f2elm_t *)table_v_qr;
        *qnr = 0;
    }

    // Get x0
    *ind = 0;
    do {
        fp2mul_mont(A,  (felm_t *)*t_ptr++,  R[0]->X);    // R[0]->X =  A*v
        fp2neg(R[0]->X);                                  // R[0]->X = -A*v
        fp2add(R[0]->X,  A,  t);
        fp2mul_mont(R[0]->X,  t,  t);
        fpadd(t[0],  (digit_t*)Montgomery_one,  t[0]);
        fp2mul_mont(R[0]->X,  t,  t);                     // t = R[0]->X^3 + A*R[0]->X^2 + R[0]->X
        *ind += 1;
    } while (!is_sqr_fp2(t,  s));
    *ind -= 1;

    if (*qnr)
        fpcopy((digit_t*)table_r_qnr[*ind], r[0]);
    else
        fpcopy((digit_t*)table_r_qr[*ind], r[0]);

    // Get x1
    fp2add(R[0]->X, A, R[1]->X);
    fp2neg(R[1]->X);    // R[1]->X = -R[0]->X-A

    // Get difference x2,  z2
    fp2sub(R[0]->X, R[1]->X, R[2]->Z);
    fp2sqr_mont(R[2]->Z, R[2]->Z);

    fpcopy(r[0], r[1]);    // (1+i)*ind
    fpadd((digit_t*)Montgomery_one, r[0], r[0]);
    fp2sqr_mont(r, r);
    fp2mul_mont(t, r, R[2]->X);
}


static void RecoverY(const f2elm_t A, const point_proj_t *xs, point_full_proj_t *Rs)
{
    f2elm_t t0, t1, t2, t3, t4;

    fp2mul_mont(xs[2]->X, xs[1]->Z, t0);
    fp2mul_mont(xs[1]->X, xs[2]->Z, t1);
    fp2mul_mont(xs[1]->X, xs[2]->X, t2);
    fp2mul_mont(xs[1]->Z, xs[2]->Z, t3);
    fp2sqr_mont(xs[1]->X, t4);
    fp2sqr_mont(xs[1]->Z, Rs[1]->X);
    fp2sub(t2, t3, Rs[1]->Y);
    fp2mul_mont(xs[1]->X, Rs[1]->Y, Rs[1]->Y);
    fp2add(t4, Rs[1]->X, t4);
    fp2mul_mont(xs[2]->Z, t4, t4);
    fp2mul_mont(A, t1, Rs[1]->X);
    fp2sub(t0, t1, Rs[1]->Z);

    fp2mul_mont(Rs[0]->X, Rs[1]->Z, t0);
    fp2add(t2, Rs[1]->X, t1);
    fp2add(t1, t1, t1);
    fp2sub(t0, t1, t0);
    fp2mul_mont(xs[1]->Z, t0, t0);
    fp2sub(t0, t4, t0);
    fp2mul_mont(Rs[0]->X, t0, t0);
    fp2add(t0, Rs[1]->Y, Rs[1]->Y);
    fp2mul_mont(Rs[0]->Y, t3, t0);
    fp2mul_mont(xs[1]->X, t0, Rs[1]->X);
    fp2add(Rs[1]->X, Rs[1]->X, Rs[1]->X);
    fp2mul_mont(xs[1]->Z, t0, Rs[1]->Z);
    fp2add(Rs[1]->Z, Rs[1]->Z, Rs[1]->Z);

    fp2inv_mont_bingcd(Rs[1]->Z);
    fp2mul_mont(Rs[1]->X, Rs[1]->Z, Rs[1]->X);
    fp2mul_mont(Rs[1]->Y, Rs[1]->Z, Rs[1]->Y);
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
    fpadd(A6[0], t0, A6[0]);    // A6 = 6 

    CompleteMPoint(A6, xs[0], Rs[0]);
    RecoverY(A6, xs, Rs);
}


static void FullIsogeny_B_dual(const unsigned char* PrivateKeyB, f2elm_t Ds[][2], f2elm_t A)
{ // Bob's ephemeral public key generation
  // Input:  a private key PrivateKeyB in the range [0, 2^Floor(Log(2,oB)) - 1]. 
  // Output: the public key PublicKeyB consisting of 3 elements in GF(p^2) which are encoded by removing leading 0 bytes.
    point_proj_t R = {0}, Q3 = {0}, pts[MAX_INT_POINTS_BOB];
    f2elm_t XPB, XQB, XRB, coeff[3], A24plus = {0}, A24minus = {0};
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_BOB], npts = 0, ii = 0;
    digit_t SecretKeyB[NWORDS_ORDER] = {0};

    // Initialize basis points
    init_basis((digit_t*)B_gen, XPB, XQB, XRB);
    fpcopy((digit_t*)XQB3, (Q3->X)[0]);
    fpcopy((digit_t*)XQB3 + NWORDS_FIELD, (Q3->X)[1]);
    fpcopy((digit_t*)Montgomery_one, (Q3->Z)[0]);

    // Initialize constants: A24minus = A-2C, A24plus = A+2C, where A=6, C=1
    fpcopy((digit_t*)&Montgomery_one, A24plus[0]);
    fp2add(A24plus, A24plus, A24plus);
    fp2add(A24plus, A24plus, A24minus);
    fp2add(A24plus, A24minus, A);
    fp2add(A24minus, A24minus, A24plus);
    
    // Retrieve kernel point
    decode_to_digits(PrivateKeyB, SecretKeyB, SECRETKEY_B_BYTES, NWORDS_ORDER);    
    LADDER3PT(XPB, XQB, XRB, SecretKeyB, BOB, R, A);
    
    // Traverse tree
    index = 0;
    for (row = 1; row < MAX_Bob; row++) {
        while (index < MAX_Bob-row) {
            fp2copy(R->X, pts[npts]->X);
            fp2copy(R->Z, pts[npts]->Z);
            pts_index[npts++] = index;
            m = strat_Bob[ii++];
            xTPLe(R, R, A24minus, A24plus, (int)m);
            index += m;
        } 
        get_3_isog(R, A24minus, A24plus, coeff);
        for (i = 0; i < npts; i++) {
            eval_3_isog(pts[i], coeff);
        }     
        eval_3_isog(Q3, coeff);    // Kernel of dual 
        fp2sub(Q3->X,Q3->Z,Ds[row-1][0]);
        fp2add(Q3->X,Q3->Z,Ds[row-1][1]);

        fp2copy(pts[npts-1]->X, R->X);
        fp2copy(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }
    get_3_isog(R, A24minus, A24plus, coeff);
    eval_3_isog(Q3, coeff);    // Kernel of dual 
    fp2sub(Q3->X, Q3->Z, Ds[MAX_Bob-1][0]);
    fp2add(Q3->X, Q3->Z, Ds[MAX_Bob-1][1]);

    fp2add(A24plus, A24minus, A);
    fp2sub(A24plus, A24minus, A24plus);
    fp2inv_mont_bingcd(A24plus);
    fp2mul_mont(A24plus, A, A);
    fp2add(A, A, A);    // A = 2*(A24plus+A24mins)/(A24plus-A24minus) 
}


static void Dlogs2_dual(const f2elm_t *f, int *D, digit_t *d0, digit_t *c0, digit_t *d1, digit_t *c1)
{
    solve_dlog(f[0], D, d0, 2);
    solve_dlog(f[2], D, c0, 2);
    solve_dlog(f[1], D, d1, 2);
    solve_dlog(f[3], D, c1, 2);
    mp_sub((digit_t*)Alice_order, c0, c0, NWORDS_ORDER);
    mp_sub((digit_t*)Alice_order, c1, c1, NWORDS_ORDER);
}


static void BuildEntangledXonly_Decomp(const f2elm_t A, point_proj_t *R, unsigned char qnr, unsigned char ind)
{
    f2elm_t *t_ptr, r, t;
    
    // Select the correct table
    if ( qnr == 1 )
        t_ptr = (f2elm_t *)table_v_qnr; 
    else
        t_ptr = (f2elm_t *)table_v_qr;

    if (ind >= TABLE_V_LEN/2)
        ind = 0;
    // Get x0 
    fp2mul_mont(A, t_ptr[ind], R[0]->X);    // x1 =  A*v
    fp2neg(R[0]->X);                        // R[0]->X = -A*v
    fp2add(R[0]->X, A, t);
    fp2mul_mont(R[0]->X, t, t);
    fpadd(t[0], (digit_t*)Montgomery_one, t[0]);
    fp2mul_mont(R[0]->X, t, t);             // t = R[0]->X^3 + A*R[0]->X^2 + R[0]->X

    if (qnr == 1)
        fpcopy((digit_t*)table_r_qnr[ind], r[0]);
    else
        fpcopy((digit_t*)table_r_qr[ind], r[0]);

    // Get x1 
    fp2add(R[0]->X, A, R[1]->X);
    fp2neg(R[1]->X);    // R[1]->X = -R[0]->X-A

    // Get difference x2,z2 
    fp2sub(R[0]->X, R[1]->X, R[2]->Z);
    fp2sqr_mont(R[2]->Z, R[2]->Z);

    fpcopy(r[0],r[1]); // (1+i)*ind 
    fpadd((digit_t*)Montgomery_one, r[0], r[0]);
    fp2sqr_mont(r, r);
    fp2mul_mont(t, r, R[2]->X);
}


static void PKBDecompression_extended(const unsigned char* SecretKeyA, const unsigned char* CompressedPKB, point_proj_t R, f2elm_t A, unsigned char* tphiBKA_t)
{ // Bob's PK decompression -- SIKE protocol
    uint64_t mask = (digit_t)(-1);
    unsigned char qnr, ind;
    f2elm_t A24,  Adiv2 = {0};
    digit_t tmp1[2*NWORDS_ORDER] = {0}, tmp2[2*NWORDS_ORDER] = {0}, inv[NWORDS_ORDER] = {0}, scal[2*NWORDS_ORDER] = {0};
    digit_t SKin[NWORDS_ORDER] = {0}, a0[NWORDS_ORDER] = {0}, a1[NWORDS_ORDER] = {0}, b0[NWORDS_ORDER] = {0}, b1[NWORDS_ORDER] = {0};
    point_proj_t Rs[3] = {0};

    mask >>= (MAXBITS_ORDER - OALICE_BITS);

    fp2_decode(&CompressedPKB[4*ORDER_A_ENCODED_BYTES], A);
    qnr = CompressedPKB[4*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES] & 0x01;
    ind = CompressedPKB[4*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES + 1];

    BuildEntangledXonly_Decomp(A, Rs, qnr, ind);
    fpcopy((digit_t*)Montgomery_one, (Rs[0]->Z)[0]);
    fpcopy((digit_t*)Montgomery_one, (Rs[1]->Z)[0]);
    
    fpadd(A[0], (digit_t*)Montgomery_one, A24[0]);
    fpcopy(A[1], A24[1]);
    fpadd(A24[0], (digit_t*)Montgomery_one, A24[0]);
    fp2div2(A24, A24);
    fp2div2(A24, A24);
       
    decode_to_digits(SecretKeyA, SKin, SECRETKEY_A_BYTES, NWORDS_ORDER);       
    decode_to_digits(&CompressedPKB[0], a0, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
    decode_to_digits(&CompressedPKB[ORDER_A_ENCODED_BYTES], b0, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
    decode_to_digits(&CompressedPKB[2*ORDER_A_ENCODED_BYTES], a1, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
    decode_to_digits(&CompressedPKB[3*ORDER_A_ENCODED_BYTES], b1, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);

    if ( (a0[0] & 1) == 1) {
        multiply((digit_t*)SKin, b1, tmp1, NWORDS_ORDER);
        mp_add(tmp1, b0, tmp1, NWORDS_ORDER);
        tmp1[NWORDS_ORDER-1] &= (digit_t)mask;        
        multiply((digit_t*)SKin, a1, tmp2, NWORDS_ORDER);
        mp_add(tmp2, a0, tmp2, NWORDS_ORDER);
        tmp2[NWORDS_ORDER-1] &= (digit_t)mask; 
        inv_mod_orderA(tmp2, inv);
        multiply(tmp1, inv, scal, NWORDS_ORDER);
        scal[NWORDS_ORDER-1] &= (digit_t)mask;
        Ladder3pt_dual(Rs, scal, ALICE, R, A24);         
    } else {
        multiply((digit_t*)SKin, a1, tmp1, NWORDS_ORDER);
        mp_add(tmp1, a0, tmp1, NWORDS_ORDER);
        tmp1[NWORDS_ORDER-1] &= (digit_t)mask; 
        multiply((digit_t*)SKin, b1, tmp2, NWORDS_ORDER);
        mp_add(tmp2, b0, tmp2, NWORDS_ORDER);
        tmp2[NWORDS_ORDER-1] &= (digit_t)mask;
        inv_mod_orderA(tmp2, inv);
        multiply(inv, tmp1, scal, NWORDS_ORDER);
        scal[NWORDS_ORDER-1] &= (digit_t)mask;
        swap_points(Rs[0], Rs[1], 0-(digit_t)1);
        Ladder3pt_dual(Rs, scal, ALICE, R, A24);            
    }        
    
    fp2div2(A,Adiv2);
    xTPLe_fast(R, R, Adiv2, OBOB_EXPON);    
    
    fp2_encode(R->X, tphiBKA_t);
    fp2_encode(R->Z, &tphiBKA_t[FP2_ENCODED_BYTES]);
    encode_to_bytes(inv, &tphiBKA_t[2*FP2_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
}


static void Compress_PKB_dual_extended(digit_t *d0, digit_t *c0, digit_t *d1, digit_t *c1, f2elm_t A, unsigned char qnr, unsigned char ind, unsigned char *CompressedPKB)
{ // Bob's PK compression -- SIKE protocol
    uint64_t mask = (digit_t)(-1);
    digit_t tmp[2*NWORDS_ORDER] = {0}, D[2*NWORDS_ORDER] = {0}, Dinv[2*NWORDS_ORDER] = {0};
    
    mask >>= (MAXBITS_ORDER - OALICE_BITS);
    
    multiply(c0, d1, tmp, NWORDS_ORDER);
    multiply(c1, d0, D, NWORDS_ORDER);
    Montgomery_neg(D, (digit_t*)Alice_order);
    mp_add(tmp, D, D, NWORDS_ORDER);
    D[NWORDS_ORDER-1] &= (digit_t)mask;         
    inv_mod_orderA(D, Dinv);        
    
    multiply(d1, Dinv, tmp, NWORDS_ORDER); // a0' = 3^n * d1 / D
    tmp[NWORDS_ORDER-1] &= (digit_t)mask;
    encode_to_bytes(tmp, CompressedPKB, ORDER_A_ENCODED_BYTES);
    
    Montgomery_neg(d0, (digit_t*)Alice_order);
    multiply(d0, Dinv, tmp, NWORDS_ORDER); // b0' = 3^n * (- d0 / D)
    tmp[NWORDS_ORDER-1] &= (digit_t)mask; 
    encode_to_bytes(tmp, &CompressedPKB[ORDER_A_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
    
    Montgomery_neg(c1, (digit_t*)Alice_order);
    multiply(c1, Dinv, tmp, NWORDS_ORDER); // a1' = 3^n * (- c1 / D)
    tmp[NWORDS_ORDER-1] &= (digit_t)mask;
    encode_to_bytes(tmp, &CompressedPKB[2*ORDER_A_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
    
    multiply(c0, Dinv, tmp, NWORDS_ORDER); // b1' = 3^n * (c0 / D)
    tmp[NWORDS_ORDER-1] &= (digit_t)mask;
    encode_to_bytes(tmp, &CompressedPKB[3*ORDER_A_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
    
    fp2_encode(A, &CompressedPKB[4*ORDER_A_ENCODED_BYTES]);
    CompressedPKB[4*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES] = qnr;
    CompressedPKB[4*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES + 1] = ind;
}


static void PKBDecompression(const unsigned char* SecretKeyA, const unsigned char* CompressedPKB, point_proj_t R, f2elm_t A)
{ // Bob's PK decompression -- SIDH protocol
    uint64_t mask = (digit_t)(-1);
    unsigned char bit,qnr,ind;
    f2elm_t A24;
    digit_t tmp1[2*NWORDS_ORDER] = {0}, tmp2[2*NWORDS_ORDER] = {0}, vone[2*NWORDS_ORDER] = {0};
    digit_t SKin[NWORDS_ORDER] = {0}, comp_temp[NWORDS_ORDER] = {0};
    point_proj_t Rs[3] = {0};

    mask >>= (MAXBITS_ORDER - OALICE_BITS);
    vone[0] = 1;

    fp2_decode(&CompressedPKB[3*ORDER_A_ENCODED_BYTES], A);
    bit = CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES] >> 7;
    qnr = CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES] & 0x1;
    ind = CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES + 1];

    // Rebuild the basis 
    BuildEntangledXonly_Decomp(A,Rs,qnr,ind);
    fpcopy((digit_t*)Montgomery_one, (Rs[0]->Z)[0]);
    fpcopy((digit_t*)Montgomery_one, (Rs[1]->Z)[0]);
    
    fpadd(A[0], (digit_t*)Montgomery_one, A24[0]);
    fpcopy(A[1], A24[1]);
    fpadd(A24[0], (digit_t*)Montgomery_one, A24[0]);
    fp2div2(A24, A24);
    fp2div2(A24, A24);
       
    decode_to_digits(SecretKeyA, SKin, SECRETKEY_A_BYTES, NWORDS_ORDER);
    swap_points(Rs[0], Rs[1], 0-(digit_t)bit);
    if (bit == 0) {
        decode_to_digits(&CompressedPKB[ORDER_A_ENCODED_BYTES], comp_temp, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
        multiply((digit_t*)SKin, comp_temp, tmp1, NWORDS_ORDER);
        mp_add(tmp1, vone, tmp1, NWORDS_ORDER);
        tmp1[NWORDS_ORDER-1] &= (digit_t)mask;
        inv_mod_orderA(tmp1, tmp2);
        decode_to_digits(&CompressedPKB[2*ORDER_A_ENCODED_BYTES], comp_temp, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
        multiply((digit_t*)SKin, comp_temp, tmp1, NWORDS_ORDER);
        decode_to_digits(&CompressedPKB[0], comp_temp, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
        mp_add(&comp_temp[0], tmp1, tmp1, NWORDS_ORDER);
        multiply(tmp1, tmp2, vone, NWORDS_ORDER);
        vone[NWORDS_ORDER-1] &= (digit_t)mask;
        Ladder3pt_dual(Rs,vone,ALICE,R,A24);
    } else {
        decode_to_digits(&CompressedPKB[2*ORDER_A_ENCODED_BYTES], comp_temp, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
        multiply((digit_t*)SKin, comp_temp, tmp1, NWORDS_ORDER);
        mp_add(tmp1, vone, tmp1, NWORDS_ORDER);
        tmp1[NWORDS_ORDER-1] &= (digit_t)mask;
        inv_mod_orderA(tmp1, tmp2);
        decode_to_digits(&CompressedPKB[ORDER_A_ENCODED_BYTES], comp_temp, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
        multiply((digit_t*)SKin, comp_temp, tmp1, NWORDS_ORDER);
        decode_to_digits(&CompressedPKB[0], comp_temp, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
        mp_add(&comp_temp[0], tmp1, tmp1, NWORDS_ORDER);
        multiply(tmp1, tmp2, vone, NWORDS_ORDER);
        vone[NWORDS_ORDER-1] &= (digit_t)mask;
        Ladder3pt_dual(Rs,vone,ALICE,R,A24);
    }
    fp2div2(A,A24);
    xTPLe_fast(R, R, A24, OBOB_EXPON);
}


static void Compress_PKB_dual(digit_t *d0, digit_t *c0, digit_t *d1, digit_t *c1, f2elm_t A, unsigned char qnr, unsigned char ind, unsigned char *CompressedPKB)
{ // Bob's PK compression -- SIDH protocol
    digit_t tmp[2*NWORDS_ORDER], inv[NWORDS_ORDER];
    if ((d1[0] & 1) == 1) {  // Storing [-d0*d1^-1 = b1*a0^-1, -c1*d1^-1 = a1*a0^-1, c0*d1^-1 = b0*a0^-1] and setting bit384 to 0
        inv_mod_orderA(d1, inv);
        Montgomery_neg(d0, (digit_t*)Alice_order);
        multiply(d0, inv, tmp, NWORDS_ORDER);
        encode_to_bytes(tmp, &CompressedPKB[0], ORDER_A_ENCODED_BYTES);
        CompressedPKB[ORDER_A_ENCODED_BYTES-1] &= MASK_ALICE;
        Montgomery_neg(c1, (digit_t*)Alice_order);
        multiply(c1, inv, tmp, NWORDS_ORDER);
        encode_to_bytes(tmp, &CompressedPKB[ORDER_A_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
        CompressedPKB[2*ORDER_A_ENCODED_BYTES-1] &= MASK_ALICE;
        multiply(c0, inv, tmp, NWORDS_ORDER);
        encode_to_bytes(tmp, &CompressedPKB[2*ORDER_A_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
        CompressedPKB[3*ORDER_A_ENCODED_BYTES-1] &= MASK_ALICE;
        CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES] = 0x00;
    } else {  // Storing [ -d1*d0^-1 = b1*b0inv, c1*d0^-1 = a1*b0inv, -c0*d0^-1 = a0*b0inv] and setting bit384 to 1
        inv_mod_orderA(d0, inv);
        Montgomery_neg(d1, (digit_t*)Alice_order);
        multiply(d1, inv, tmp, NWORDS_ORDER);
        encode_to_bytes(tmp, &CompressedPKB[0], ORDER_A_ENCODED_BYTES);
        CompressedPKB[ORDER_A_ENCODED_BYTES - 1] &= MASK_ALICE;
        multiply(c1, inv, tmp, NWORDS_ORDER);
        encode_to_bytes(tmp, &CompressedPKB[ORDER_A_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
        CompressedPKB[2*ORDER_A_ENCODED_BYTES-1] &= MASK_ALICE;
        Montgomery_neg(c0, (digit_t*)Alice_order);
        multiply(c0, inv, tmp, NWORDS_ORDER);
        encode_to_bytes(tmp, &CompressedPKB[2*ORDER_A_ENCODED_BYTES], ORDER_A_ENCODED_BYTES);
        CompressedPKB[3*ORDER_A_ENCODED_BYTES-1] &= MASK_ALICE;
        CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES] = 0x80;
    }
    
    fp2_encode(A, &CompressedPKB[3*ORDER_A_ENCODED_BYTES]);  
    CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES] |= qnr;
    CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES + 1] = ind;
    CompressedPKB[3*ORDER_A_ENCODED_BYTES + FP2_ENCODED_BYTES + 2] = 0;
}


static int EphemeralKeyGeneration_B_extended(const unsigned char* PrivateKeyB, unsigned char* CompressedPKB, unsigned int sike)
{ // Bob's ephemeral public key generation using compression -- SIKE protocol
    unsigned char qnr, ind;
    int D[DLEN_2] = {0};
    digit_t c0[NWORDS_ORDER] = {0}, d0[NWORDS_ORDER] = {0}, c1[NWORDS_ORDER] = {0}, d1[NWORDS_ORDER] = {0}; 
    f2elm_t Ds[MAX_Bob][2] = {0}, f[4] = {0}, A = {0};
    point_full_proj_t Rs[2] = {0};
    point_t Pw, Qw;

    FullIsogeny_B_dual(PrivateKeyB, Ds, A);
    BuildOrdinary2nBasis_dual(A, Ds, Rs, &qnr, &ind);  // Generate a basis in E_A and pulls it back to E_A6. Rs[0] and Rs[1] affinized.

    // Maps from y^2 = x^3 + 6x^2 + x into y^2 = x^3 -11x + 14
    fpadd((digit_t*)Montgomery_one, (Rs[0]->X)[0], (Rs[0]->X)[0]);
    fpadd((digit_t*)Montgomery_one, (Rs[0]->X)[0], (Rs[0]->X)[0]);  // Weierstrass form 
    fpadd((digit_t*)Montgomery_one, (Rs[1]->X)[0], (Rs[1]->X)[0]);
    fpadd((digit_t*)Montgomery_one, (Rs[1]->X)[0], (Rs[1]->X)[0]);  // Weierstrass form 

    fpcopy((digit_t*)A_basis_zero + 0*NWORDS_FIELD, Pw->x[0]);
    fpcopy((digit_t*)A_basis_zero + 1*NWORDS_FIELD, Pw->x[1]);
    fpcopy((digit_t*)A_basis_zero + 2*NWORDS_FIELD, Pw->y[0]);
    fpcopy((digit_t*)A_basis_zero + 3*NWORDS_FIELD, Pw->y[1]);
    fpcopy((digit_t*)A_basis_zero + 4*NWORDS_FIELD, Qw->x[0]);
    fpcopy((digit_t*)A_basis_zero + 5*NWORDS_FIELD, Qw->x[1]);
    fpcopy((digit_t*)A_basis_zero + 6*NWORDS_FIELD, Qw->y[0]);
    fpcopy((digit_t*)A_basis_zero + 7*NWORDS_FIELD, Qw->y[1]);
    Tate2_pairings(Pw, Qw, Rs, f);
    fp2correction(f[0]);
    fp2correction(f[1]);
    fp2correction(f[2]);
    fp2correction(f[3]);

    Dlogs2_dual(f, D, d0, c0, d1, c1);
    if (sike == 1)
        Compress_PKB_dual_extended(d0, c0, d1, c1, A, qnr, ind, CompressedPKB);  
    else
        Compress_PKB_dual(d0, c0, d1, c1, A, qnr, ind, CompressedPKB);    

    return 0;
}


int EphemeralKeyGeneration_B(const unsigned char* PrivateKeyB, unsigned char* CompressedPKB)
{ // Bob's ephemeral public key generation using compression -- SIDH protocol

    return EphemeralKeyGeneration_B_extended(PrivateKeyB, CompressedPKB, 0);
}


static int EphemeralSecretAgreement_A_extended(const unsigned char* PrivateKeyA, const unsigned char* PKB, unsigned char* SharedSecretA, unsigned int sike)
{ // Alice's ephemeral shared secret computation using compression -- SIKE protocol
    unsigned int i, ii = 0, row, m, index = 0, pts_index[MAX_INT_POINTS_ALICE], npts = 0;
    f2elm_t A24plus = {0}, C24 = {0};
    point_proj_t R, pts[MAX_INT_POINTS_ALICE];
    f2elm_t jinv, coeff[5], A;
    f2elm_t param_A = {0};

    if (sike == 1)
        PKBDecompression_extended(PrivateKeyA, PKB, R, param_A, SharedSecretA+FP2_ENCODED_BYTES);
    else
        PKBDecompression(PrivateKeyA, PKB, R, param_A);
    
    fp2copy(param_A, A);    
    fpadd((digit_t*)&Montgomery_one, (digit_t*)&Montgomery_one, C24[0]);
    fp2add(A, C24, A24plus);
    fpadd(C24[0], C24[0], C24[0]);    

#if (OALICE_BITS % 2 == 1)
    point_proj_t S;

    xDBLe(R, S, A24plus, C24, (int)(OALICE_BITS-1));
    get_2_isog(S, A24plus, C24);
    eval_2_isog(R, S);
#endif

    // Traverse tree
    index = 0;
    for (row = 1; row < MAX_Alice; row++) {
        while (index < MAX_Alice-row) {
            fp2copy(R->X, pts[npts]->X);
            fp2copy(R->Z, pts[npts]->Z);
            pts_index[npts++] = index;
            m = strat_Alice[ii++];
            xDBLe(R, R, A24plus, C24, (int)(2*m));
            index += m;
        }
        get_4_isog(R, A24plus, C24, coeff);

        for (i = 0; i < npts; i++) {
            eval_4_isog(pts[i], coeff);
        }

        fp2copy(pts[npts-1]->X, R->X);
        fp2copy(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }

    get_4_isog(R, A24plus, C24, coeff);
    fp2add(A24plus, A24plus, A24plus);
    fp2sub(A24plus, C24, A24plus);
    fp2add(A24plus, A24plus, A24plus);
    j_inv(A24plus, C24, jinv);    
    fp2_encode(jinv, SharedSecretA);    // Format shared secret
    
    return 0;
}


int EphemeralSecretAgreement_A(const unsigned char* PrivateKeyA, const unsigned char* PKB, unsigned char* SharedSecretA)
{ // Alice's ephemeral shared secret computation using compression -- SIDH protocol
  // It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's decompressed data point_R and param_A
  // Inputs: Alice's PrivateKeyA is an even integer in the range [2, oA-2], where oA = 2^OALICE_BITS. 
  //         Bob's decompressed data consists of point_R in (X:Z) coordinates and the curve parameter param_A in GF(p^2).
  // Output: a shared secret SharedSecretA that consists of one element in GF(p^2).

    return EphemeralSecretAgreement_A_extended(PrivateKeyA, PKB, SharedSecretA, 0);
}


static int8_t validate_ciphertext(const unsigned char* ephemeralsk_, const unsigned char* CompressedPKB, const unsigned char* xKA, const unsigned char* tphiBKA_t)
{ // If ct validation passes returns 0, otherwise returns -1.
    point_proj_t phis[3] = {0}, R, S, pts[MAX_INT_POINTS_BOB];
    f2elm_t XPB, XQB, XRB, coeff[3], A24plus = {0}, A24minus = {0}, A = {0}, comp1 = {0}, comp2 = {0}, one = {0};
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_BOB], npts = 0, ii = 0;
    digit_t temp[NWORDS_ORDER] = {0};
    digit_t sk[NWORDS_ORDER] = {0};    
                
    fpcopy((digit_t*)&Montgomery_one, one[0]);    
    
    // Initialize basis points
    init_basis((digit_t*)B_gen, XPB, XQB, XRB);

    fp2_decode(xKA, phis[0]->X);
    fpcopy((digit_t*)&Montgomery_one, phis[0]->Z[0]); // phi[0] <- PA + skA*QA    

    // Initialize constants: A24minus = A-2C, A24plus = A+2C, where A=6, C=1
    fpcopy((digit_t*)&Montgomery_one, A24plus[0]);
    fp2add(A24plus, A24plus, A24plus);
    fp2add(A24plus, A24plus, A24minus);  // A24minus = 4
    fp2add(A24plus, A24minus, A);        // A = 6
    fp2add(A24minus, A24minus, A24plus); // A24plus = 8
    
    // Retrieve kernel point
    decode_to_digits(ephemeralsk_, sk, SECRETKEY_B_BYTES, NWORDS_ORDER);
    LADDER3PT(XPB, XQB, XRB, sk, BOB, R, A);
    
    // Traverse tree
    index = 0;
    for (row = 1; row < MAX_Bob; row++) {
        while (index < MAX_Bob-row) {
            fp2copy(R->X, pts[npts]->X);
            fp2copy(R->Z, pts[npts]->Z);
            pts_index[npts++] = index;
            m = strat_Bob[ii++];
            xTPLe(R, R, A24minus, A24plus, (int)m);
            index += m;
        } 
        get_3_isog(R, A24minus, A24plus, coeff);
        for (i = 0; i < npts; i++) {
            eval_3_isog(pts[i], coeff);
        }         
        eval_3_isog(phis[0], coeff);

        fp2copy(pts[npts-1]->X, R->X);
        fp2copy(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }    
    get_3_isog(R, A24minus, A24plus, coeff);         
    eval_3_isog(phis[0], coeff);  // phis[0] <- phiB(PA + skA*QA)

    fp2_decode(&CompressedPKB[4*ORDER_A_ENCODED_BYTES], A);
    
    // Single equation check: t*(phiP + skA*phiQ) =? t*3^n*((a0+skA*a1)*S1 + (b0+skA*b1)*S2) for t in {(a0+skA*a1)^-1, (b0+skA*b1)^-1}
    
    fp2_decode(tphiBKA_t, S->X);
    fp2_decode(&tphiBKA_t[FP2_ENCODED_BYTES], S->Z);  // Recover t*3^n*((a0+skA*a1)*S1 + (b0+skA*b1)*S2)    
    decode_to_digits(&tphiBKA_t[2*FP2_ENCODED_BYTES], temp, ORDER_A_ENCODED_BYTES, NWORDS_ORDER);
    Ladder(phis[0], temp, A, OALICE_BITS, R);         // t*(phiP + skA*phiQ)
        
    fp2mul_mont(R->X, S->Z, comp1);
    fp2mul_mont(R->Z, S->X, comp2);             
    return (cmp_f2elm(comp1, comp2));
}
