/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: Torsion basis generation for compression
*********************************************************************************************/


static void get_2_torsion_entangled_basis_compression(const f2elm_t A, point_t S1, point_t S2, unsigned char *bit, unsigned char *entry) 
{ // Build an entangled basis for E[2^m].
  // At first glance this is similar to the Elligator 2 technique, but the field element u is a *square* here, not a non-square.
    unsigned int i, index, isSqrA = 0;
    felm_t r = {0}, s, z, alpha, twoalphainv, beta;
    f2elm_t t, u, u0, one = {0}, *t_ptr;
    felm_t *x1 = (felm_t*) S1->x, *y1 = (felm_t*) S1->y, *x2 = (felm_t*) S2->x, *y2 = (felm_t*) S2->y;
     
    fpcopy((digit_t*)&Montgomery_one, one[0]);    
    copy_words((const digit_t *)u0_entang, u0[0], 2*NWORDS_FIELD);
    copy_words((const digit_t *)u_entang, u[0], 2*NWORDS_FIELD);

    // Select the correct table
    if (is_sqr_fp2(A, s)) {
        t_ptr = (f2elm_t *)table_v_qnr; 
        isSqrA = 1;
    } else {
        t_ptr = (f2elm_t *)table_v_qr;
    }
    *bit = (unsigned char)isSqrA;  // This bit is transmitted along with the PK to speedup decompression

    index = 0;
    do {
        fp2mul_mont(A, (felm_t *)*t_ptr++, x1); // x1 =  A*v
        fp2neg(x1);                             // x1 = -A*v
        fp2add(x1, A, t);
        fp2mul_mont(x1, t, t);
        fpadd(t[0], one[0], t[0]);
        fp2mul_mont(x1, t, t);                  // t = x1^3 + A*x1^2 + x1 = x1(x1(x1 + A) + 1)
        index += 2;
    } while (!is_sqr_fp2(t, s));
    *entry = ((unsigned char)index - 2)/2;  // This table entry will also be transmitted along with the PubKey to speedup decompression
    
    if (isSqrA)
        copy_words((const digit_t *)table_r_qnr[(index-2)/2], r, NWORDS_FIELD);
    else
        copy_words((const digit_t *)table_r_qr[(index-2)/2], r, NWORDS_FIELD);
    
    // Finish sqrt computation for y1 = sqrt(x1^3+A*x1^2+x1)
    fpadd(t[0],s,z);
    fpdiv2(z,z);
    fpcopy(z,alpha);
    for (i = 0; i < OALICE_BITS - 2; i++) {         
        fpsqr_mont(alpha, alpha);
    }
    for (i = 0; i < OBOB_EXPON; i++) {
        fpsqr_mont(alpha, s);                                         
        fpmul_mont(alpha, s, alpha);     // alpha = z^((p+1)/4)                                 
    }

    fpadd(alpha,alpha,twoalphainv); 
    fpinv_mont_bingcd(twoalphainv);   
    fpmul_mont(t[1],twoalphainv,beta);
    fpsqr_mont(alpha, twoalphainv);
    fpcorrection(twoalphainv);
    fpcorrection(z);   
    if (memcmp(twoalphainv, z, NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
        fpcopy(alpha,y1[0]);
        fpcopy(beta,y1[1]);
    } else {
        fpneg(beta);
        fpcopy(beta,y1[0]);
        fpneg(alpha);        
        fpcopy(alpha,y1[1]);
    }   
    fp2add(x1, A, x2);
    fp2neg(x2);                  // x2 = A*v - A
    fp2mul_mont(u0,y1,y2);   
    fpmul_mont(r,y2[0],y2[0]);
    fpmul_mont(r,y2[1],y2[1]);   // y2 = u0*r*y1
}


static void get_2_torsion_entangled_basis_decompression(const f2elm_t A, point_t S1, point_t S2, unsigned char isASqr, unsigned char entry) 
{ // Build an entangled basis for E[2^m] during decompression using the the entry and table already computed during compression
    unsigned int i;
    felm_t r = {0}, s, z, alpha, twoalphainv, beta;
    f2elm_t t, u, u0, one = {0}, *t_ptr;
    felm_t *x1 = (felm_t*) S1->x, *y1 = (felm_t*) S1->y, *x2 = (felm_t*) S2->x, *y2 = (felm_t*) S2->y;
     
    fpcopy((digit_t*)&Montgomery_one, one[0]);
    
    copy_words((const digit_t *)u0_entang, u0[0], 2*NWORDS_FIELD);
    copy_words((const digit_t *)u_entang, u[0], 2*NWORDS_FIELD);

    // Select the table
    t_ptr = isASqr ? (f2elm_t *)table_v_qnr : (f2elm_t *)table_v_qr;

    fp2mul_mont(A, t_ptr[entry], x1); // x1 =  A*v
    fp2neg(x1);                       // x1 = -A*v
    fp2add(x1, A, t);
    fp2mul_mont(x1, t, t);
    fpadd(t[0], one[0], t[0]);
    fp2mul_mont(x1, t, t);            // t = x1^3 + A*x1^2 + x1 = x1(x1(x1 + A) + 1)
    is_sqr_fp2(t, s);
    
    if (isASqr)
        copy_words((const digit_t *)table_r_qnr[entry], r, NWORDS_FIELD);
    else
        copy_words((const digit_t *)table_r_qr[entry], r, NWORDS_FIELD);
    
    // Finish sqrt computation for y1 = sqrt(x1^3+A*x1^2+x1)
    fpadd(t[0],s,z);
    fpdiv2(z,z);
    fpcopy(z,alpha);
    for (i = 0; i < OALICE_BITS - 2; i++) {         
        fpsqr_mont(alpha, alpha);
    }
    for (i = 0; i < OBOB_EXPON; i++) {
        fpsqr_mont(alpha, s);                                         
        fpmul_mont(alpha, s, alpha);     // alpha = z^((p+1)/4)                                 
    }

    fpadd(alpha,alpha,twoalphainv);   
    fpinv_mont_bingcd(twoalphainv);   
    fpmul_mont(t[1],twoalphainv,beta);
    fpsqr_mont(alpha, twoalphainv);
    fpcorrection(twoalphainv);
    fpcorrection(z);
    if (memcmp((unsigned char*)twoalphainv, (unsigned char*)z, NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
        fpcopy(alpha,y1[0]);
        fpcopy(beta,y1[1]);
    } else {
        fpneg(beta);
        fpcopy(beta,y1[0]);
        fpneg(alpha);        
        fpcopy(alpha,y1[1]);
    }
    fp2add(x1, A, x2);
    fp2neg(x2);                  // x2 = A*v - A
    fp2mul_mont(u0,y1,y2);   
    fpmul_mont(r,y2[0],y2[0]);
    fpmul_mont(r,y2[1],y2[1]);   // y2 = u0*r*y1
}


static void sqrtinv2(const f2elm_t v, const f2elm_t z, f2elm_t s, f2elm_t invz)
{ // Compute the square root of a field element v in F_{p^2} and invert a field element z.
    int j;
    felm_t az, bz, av, bv, Nz, Nv, av2, bv2, r, t, x, y, kk;
    f2elm_t conjz, zero = {0};
    
    fpsqr_mont(z[0],az);
    fpsqr_mont(z[1],bz);
    fpadd(az,bz,Nz);    
    fpcopy(v[0],av);
    fpcopy(v[1],bv);

    if (memcmp(bv,zero[0],NBITS_TO_NBYTES(NBITS_FIELD)) != 0) {
        fpsqr_mont(av,av2);
        fpsqr_mont(bv,bv2);
        fpadd(av2,bv2,Nv);

        fpcopy(Nv,r);
        for (j = 0; j < OALICE_BITS - 2; j++)
            fpsqr_mont(r,r);
        for (j = 0; j < OBOB_EXPON; j++) {
            fpsqr_mont(r,t);
            fpmul_mont(r,t,r);
        }        
        fpsqr_mont(r,t);
        fpcorrection(t);
        fpcorrection(Nv);
        if (memcmp(t,Nv,NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
            mp_add(av,r,r,NWORDS_FIELD);            
            
            if ((r[0] & 0x01) != 0)
                mp_add(r,(digit_t*)&PRIME,r,NWORDS_FIELD);
            
            mp_shiftr1(r,NWORDS_FIELD);

            fpcopy(r,x);
            for (j = 0; j < OALICE_BITS - 2; j++)
                fpsqr_mont(x,x);
            for (j = 0; j < OBOB_EXPON; j++) {
                fpsqr_mont(x,t);
                fpmul_mont(x,t,x);
            }        
            
            if (memcmp(Nz,zero[0],NBITS_TO_NBYTES(NBITS_FIELD)) != 0) {
                fpmul_mont(x,Nz,t);
                fpadd(t,t,t);
                fpinv_mont_bingcd(t); 
                fpcopy(t,kk);
                fpmul_mont(bv,Nz,y);
                fpmul_mont(y,kk,y);
                
                fpsqr_mont(x,t);
                fpcorrection(t);
                fpcorrection(r);
                if (memcmp(t,r,NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
                    fpcopy(x,s[0]);
                    fpcopy(y,s[1]);
                } else {
                    fpcopy(y,s[0]);
                    fpcopy(x,s[1]);                    
                }
                fp2_conj(z,conjz);
                fpmul_mont(x,kk,t);
                fpadd(t,t,t);
                fpmul_mont(t,conjz[0],invz[0]);
                fpmul_mont(t,conjz[1],invz[1]);
            } else {
                fpadd(x,x,t);
                fpinv_mont_bingcd(t);
                fpmul_mont(bv,t,y);                
                fpsqr_mont(x,t);
                if (memcmp(t,r,NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
                    fpcopy(x,s[0]);
                    fpcopy(y,s[1]);
                } else {
                    fpcopy(y,s[0]);
                    fpcopy(x,s[1]);                    
                }
                fp2copy(zero,invz);                
            }

        } else {
            fp2copy(zero,s);
            if (memcmp(Nz,zero[0],NBITS_TO_NBYTES(NBITS_FIELD)) != 0) {
                fpinv_mont_bingcd(Nz);
                fp2_conj(z,conjz);
                fpmul_mont(Nz,conjz[0],invz[0]);
                fpmul_mont(Nz,conjz[1],invz[1]);                
            } else {
                fp2copy(zero,invz);
            }
        }
    } else {
        fpcopy(av,r);
        for (j = 0; j < OALICE_BITS - 2; j++)
            fpsqr_mont(r,r);
        for (j = 0; j < OBOB_EXPON; j++) {
            fpsqr_mont(r,t);
            fpmul_mont(r,t,r);
        }            
        fpsqr_mont(r,t);
        if (memcmp(t,av,NBITS_TO_NBYTES(NBITS_FIELD)) == 0) {
            fpcopy(r,s[0]);
            fpcopy(zero[0],s[1]);
        } else {
            fpcopy(zero[0],s[0]);
            fpcopy(r,s[1]);
        }

        if (memcmp(Nz,zero[0],NBITS_TO_NBYTES(NBITS_FIELD)) != 0) {
            fp2_conj(z,conjz);
            fpinv_mont_bingcd(Nz);
            fpmul_mont(Nz,conjz[0],invz[0]);
            fpmul_mont(Nz,conjz[1],invz[1]);            
        } else {
            fp2copy(zero,invz);
        }
    }
}


static void BasePoint3n(f2elm_t A, unsigned int *r, point_proj_t P, point_proj_t Q)
{ // xz-only construction of a point of order 3^n in the Montgomery curve y^2 = x^3 + A*x^2 + x from base counter r.
  // This is essentially the Elligator 2 technique coupled with cofactor multiplication and LI checking.
    int i;
    felm_t a2, b2, N, temp0, temp1;
    f2elm_t A2, A24, two = {0}, x, y2, one_fp2 = {0}, *t_ptr;
    point_proj_t S;
    
    fpcopy((digit_t*)&Montgomery_one, one_fp2[0]);
    fp2div2(A,A2);
    fpcopy(one_fp2[0],two[0]);
    fpadd(two[0], two[0], two[0]);
    fp2add(A,two,A24);
    fp2div2(A24,A24);
    fp2div2(A24,A24);

    t_ptr = (f2elm_t *)&v_3_torsion[*r];    
    do {
        *r += 1;        
        fp2mul_mont(A,(felm_t*)t_ptr++,x); // x = A*v; v := 1/(1 + U*r^2) table lookup
        fp2neg(x);                         // x = -A*v;
        fp2add(A,x,y2);                    // y2 = x + A
        fp2mul_mont(y2, x, y2);            // y2 = x*(x + A)
        fpadd(y2[0], one_fp2[0], y2[0]);   // y2 = x(x + A) + 1
        fp2mul_mont(x,y2,y2);              // y2 = x*(x^2 + Ax + 1);
        fpsqr_mont(y2[0],a2);
        fpsqr_mont(y2[1],b2);
        fpadd(a2,b2,N);                    // N := norm(y2);

        fpcopy(N,temp0);
        for (i = 0; i < OALICE_BITS - 2; i++) {    
            fpsqr_mont(temp0, temp0);
        }
        for (i = 0; i < OBOB_EXPON; i++) {
            fpsqr_mont(temp0, temp1);
            fpmul_mont(temp0, temp1, temp0);
        }
        fpsqr_mont(temp0,temp1);  // z = N^((p + 1) div 4);
        fpcorrection(temp1);
        fpcorrection(N);
        if (memcmp(temp1,N,NBITS_TO_NBYTES(NBITS_FIELD)) != 0) {
            fp2neg(x);
            fp2sub(x,A,x);        // x = -x - A;
        }
        fp2copy(x,S->X);
        fp2copy(one_fp2,S->Z);
        Double(S,P,A24,OALICE_BITS);   // x, z := Double(A24, x, 1, eA);
        xTPLe_fast(P,Q,A2,OBOB_EXPON-1);  // t, w := Triple(A_2, x, z, eB-1);
        fp2correction(Q->X);
        fp2correction(Q->Z);
    }   while (is_felm_zero(Q->Z[0]) && is_felm_zero(Q->Z[1]));
}


static void BasePoint3n_decompression(f2elm_t A, const unsigned char r, point_proj_t P)
{ // Deterministic xz-only construction of a point of order 3^n in the Montgomery curve y^2 = x^3 + A*x^2 + x from counter r1.
  // Notice that the Elligator 2 counter r was generated beforehand during key compression without linear independence testing
    int i;
    felm_t a2, b2, N, temp0, temp1;
    f2elm_t A2, A24, two = {0}, x, y2, one_fp2 = {0};
    point_proj_t S;
    
    fpcopy((digit_t*)&Montgomery_one, one_fp2[0]);
    fp2div2(A,A2);
    fpcopy((digit_t*)&Montgomery_one,two[0]);
    fpadd(two[0], two[0], two[0]);
    fp2add(A,two,A24);
    fp2div2(A24,A24);
    fp2div2(A24,A24);

    if ( (r-1) < TABLE_V3_LEN)
        fp2mul_mont(A, (felm_t*)&v_3_torsion[r-1], x);  // x =  A*v;
    else // If the index r is out of range, just use a default (0)
        fp2mul_mont(A, (felm_t*)&v_3_torsion[0], x);  // x =  A*v0;
    fp2neg(x);                                      // x = -A*v;
    fp2add(x,A,y2);                                 // y2 = x + A             
    fp2mul_mont(x,y2,y2);                           // y2 = x*(x + A)
    fpadd(y2[0], one_fp2[0], y2[0]);                // y2 = x(x + A) + 1
    fp2mul_mont(x, y2, y2);                         // y2 = x*(x^2 + A*x + 1);    
    fpsqr_mont(y2[0],a2);
    fpsqr_mont(y2[1],b2);
    fpadd(a2,b2,N);                                 // N := Fp!norm(y2);

    fpcopy(N,temp0);
    for (i = 0; i < OALICE_BITS - 2; i++) {    
        fpsqr_mont(temp0, temp0);
    }
    for (i = 0; i < OBOB_EXPON; i++) {
        fpsqr_mont(temp0, temp1);
        fpmul_mont(temp0, temp1, temp0);
    }
    fpsqr_mont(temp0,temp1);  // z = N^((p + 1) div 4);
    fpcorrection(temp1);
    fpcorrection(N);
    if (memcmp(temp1,N,NBITS_TO_NBYTES(NBITS_FIELD)) != 0) {
        fp2neg(x);
        fp2sub(x,A,x);        // x = -x - A;
    }
    fp2copy(x,S->X);
    fp2copy(one_fp2,S->Z);
    Double(S,P,A24,OALICE_BITS);   // x, z := Double(A24, x, 1, eA);
}


static void BuildOrdinaryE3nBasis(f2elm_t A, point_full_proj_t R1, point_full_proj_t R2, unsigned int rs[2])
{ // Generate a basis for the 3^eB torsion
    unsigned int r = 0;
    f2elm_t t2w1, t1w2;
    point_proj_t P, Q, R, S;
    
    // 1st basis point:
    BasePoint3n(A,&r,P,Q);    
    rs[0] = r;
    
    // 2nd basis point:
    do {
        BasePoint3n(A,&r,R,S);
        fp2mul_mont(S->X,Q->Z,t2w1);
        fp2mul_mont(Q->X,S->Z,t1w2);
        fp2correction(t2w1);
        fp2correction(t1w2);
    } while (memcmp(t2w1[0],t1w2[0],NBITS_TO_NBYTES(NBITS_FIELD)) == 0 && memcmp(t2w1[1],t1w2[1],NBITS_TO_NBYTES(NBITS_FIELD)) == 0); // Pr[t2/w2 == t1/w1] = 1/4: E[loop length] = 4/3
    rs[1] = r;

    // NB: ideally the following point completions could share one inversion at the cost of 3 products, but this is not implemented here.
    CompleteMPoint(A,P,R1);    
    CompleteMPoint(A,R,R2);      
}


static void BuildOrdinaryE3nBasis_decompression(f2elm_t A, point_full_proj_t R1, point_full_proj_t R2, const unsigned char r1, const unsigned char r2)
{ // Generate a basis for the 3^eB torsion using shared information
    point_proj_t P, R;
    
    // 1st basis point:
    BasePoint3n_decompression(A,r1,P);
    // 2nd basis point:
    BasePoint3n_decompression(A,r2,R);
    
    // NB: ideally the following point completions could share one inversion at the cost of 3 products, but this is not implemented here.
    CompleteMPoint(A,P,R1);    
    CompleteMPoint(A,R,R2);   
}
 
