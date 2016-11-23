/********************************************************************************************
* SIDH: an efficient supersingular isogeny-based cryptography library for Diffie-Hellman key 
*       exchange providing 128 bits of quantum security and 192 bits of classical security.
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
*
* Abstract: elliptic curve and isogeny functions
*
*********************************************************************************************/ 

#include "SIDH_internal.h"


void j_inv(f2elm_t A, f2elm_t C, f2elm_t jinv)
{ // Computes the j-invariant of a Montgomery curve with projective constant.
  // Input: A,C in GF(p^2).
  // Output: j=256*(A^2-3*C^2)^3/(C^4*(A^2-4*C^2)), which is j-invariant of Montgomery curve B*y^2=x^3+(A/C)*x^2+x or (equivalently) j-invariant of B'*y^2=C*x^3+A*x^2+C*x.
    f2elm_t t0, t1;
    
    fp2sqr751_mont(A, jinv);                           // jinv = A^2        
    fp2sqr751_mont(C, t1);                             // t1 = C^2
    fp2add751(t1, t1, t0);                             // t0 = t1+t1
    fp2sub751(jinv, t0, t0);                           // t0 = jinv-t0
    fp2sub751(t0, t1, t0);                             // t0 = t0-t1
    fp2sub751(t0, t1, jinv);                           // jinv = t0-t1
    fp2sqr751_mont(t1, t1);                            // t1 = t1^2
    fp2mul751_mont(jinv, t1, jinv);                    // jinv = jinv*t1
    fp2add751(t0, t0, t0);                             // t0 = t0+t0
    fp2add751(t0, t0, t0);                             // t0 = t0+t0
    fp2sqr751_mont(t0, t1);                            // t1 = t0^2
    fp2mul751_mont(t0, t1, t0);                        // t0 = t0*t1
    fp2add751(t0, t0, t0);                             // t0 = t0+t0
    fp2add751(t0, t0, t0);                             // t0 = t0+t0
    fp2inv751_mont(jinv);                              // jinv = 1/jinv 
    fp2mul751_mont(jinv, t0, jinv);                    // jinv = t0*jinv
}


void xDBLADD(point_proj_t P, point_proj_t Q, f2elm_t xPQ, f2elm_t A24)
{ // Simultaneous doubling and differential addition.
  // Input: projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, affine difference xPQ=x(P-Q) and Montgomery curve constant A24=(A+2)/4.
  // Output: projective Montgomery points P <- 2*P = (X2P:Z2P) such that x(2P)=X2P/Z2P, and Q <- P+Q = (XQP:ZQP) such that = x(Q+P)=XQP/ZQP. 
    f2elm_t t0, t1, t2;

    fp2add751(P->X, P->Z, t0);                         // t0 = XP+ZP
    fp2sub751(P->X, P->Z, t1);                         // t1 = XP-ZP
    fp2sqr751_mont(t0, P->X);                          // XP = (XP+ZP)^2
    fp2sub751(Q->X, Q->Z, t2);                         // t2 = XQ-ZQ
    fp2add751(Q->X, Q->Z, Q->X);                       // XQ = XQ+ZQ
    fp2mul751_mont(t0, t2, t0);                        // t0 = (XP+ZP)*(XQ-ZQ)
    fp2sqr751_mont(t1, P->Z);                          // ZP = (XP-ZP)^2
    fp2mul751_mont(t1, Q->X, t1);                      // t1 = (XP-ZP)*(XQ+ZQ)
    fp2sub751(P->X, P->Z, t2);                         // t2 = (XP+ZP)^2-(XP-ZP)^2
    fp2mul751_mont(P->X, P->Z, P->X);                  // XP = (XP+ZP)^2*(XP-ZP)^2
    fp2mul751_mont(t2, A24, Q->X);                     // XQ = A24*[(XP+ZP)^2-(XP-ZP)^2]
    fp2sub751(t0, t1, Q->Z);                           // ZQ = (XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)
    fp2add751(Q->X, P->Z, P->Z);                       // ZP = A24*[(XP+ZP)^2-(XP-ZP)^2]+(XP-ZP)^2
    fp2add751(t0, t1, Q->X);                           // XQ = (XP+ZP)*(XQ-ZQ)+(XP-ZP)*(XQ+ZQ)
    fp2mul751_mont(P->Z, t2, P->Z);                    // ZP = [A24*[(XP+ZP)^2-(XP-ZP)^2]+(XP-ZP)^2]*[(XP+ZP)^2-(XP-ZP)^2]
    fp2sqr751_mont(Q->Z, Q->Z);                        // ZQ = [(XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)]^2
    fp2sqr751_mont(Q->X, Q->X);                        // XQ = [(XP+ZP)*(XQ-ZQ)+(XP-ZP)*(XQ+ZQ)]^2
    fp2mul751_mont(Q->Z, xPQ, Q->Z);                   // ZQ = xPQ*[(XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)]^2
}


void xDBL(point_proj_t P, point_proj_t Q, f2elm_t A24, f2elm_t C24)
{ // Doubling of a Montgomery point in projective coordinates (X:Z).
  // Input: projective Montgomery x-coordinates P = (X1:Z1), where x1=X1/Z1 and Montgomery curve constant A24/C24=(A/C+2)/4.
  // Output: projective Montgomery x-coordinates Q = 2*P = (X2:Z2).
    f2elm_t t0, t1;
    
    fp2sub751(P->X, P->Z, t0);                         // t0 = X1-Z1
    fp2add751(P->X, P->Z, t1);                         // t1 = X1+Z1
    fp2sqr751_mont(t0, t0);                            // t0 = (X1-Z1)^2 
    fp2sqr751_mont(t1, t1);                            // t1 = (X1+Z1)^2 
    fp2mul751_mont(C24, t0, Q->Z);                     // Z2 = C24*(X1-Z1)^2   
    fp2mul751_mont(t1, Q->Z, Q->X);                    // X2 = C24*(X1-Z1)^2*(X1+Z1)^2
    fp2sub751(t1, t0, t1);                             // t1 = (X1+Z1)^2-(X1-Z1)^2 
    fp2mul751_mont(A24, t1, t0);                       // t0 = A24*[(X1+Z1)^2-(X1-Z1)^2]
    fp2add751(Q->Z, t0, Q->Z);                         // Z2 = A24*[(X1+Z1)^2-(X1-Z1)^2] + C24*(X1-Z1)^2
    fp2mul751_mont(Q->Z, t1, Q->Z);                    // Z2 = [A24*[(X1+Z1)^2-(X1-Z1)^2] + C24*(X1-Z1)^2]*[(X1+Z1)^2-(X1-Z1)^2]
}


void xDBLe(point_proj_t P, point_proj_t Q, f2elm_t A, f2elm_t C, int e)
{ // Computes [2^e](X:Z) on Montgomery curve with projective constant via e repeated doublings.
  // Input: projective Montgomery x-coordinates P = (XP:ZP), such that xP=XP/ZP and Montgomery curve constant A/C.
  // Output: projective Montgomery x-coordinates P <- (2^e)*P.
    f2elm_t A24num, A24den;
    int i;
    
    fp2add751(C, C, A24num);                           
    fp2add751(A24num, A24num, A24den);                    
    fp2add751(A24num, A, A24num); 
    copy_words((digit_t*)P, (digit_t*)Q, 2*2*NWORDS_FIELD);

    for (i = 0; i < e; i++) {
        xDBL(Q, Q, A24num, A24den);
    }
}


void xADD(point_proj_t P, point_proj_t Q, f2elm_t xPQ)
{ // Differential addition.
  // Input: projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, and affine difference xPQ=x(P-Q).
  // Output: projective Montgomery point P <- P+Q = (XQP:ZQP) such that = x(Q+P)=XQP/ZQP. 
    f2elm_t t0, t1;
    
    fp2add751(P->X, P->Z, t0);                         // t0 = XP+ZP
    fp2sub751(P->X, P->Z, t1);                         // t1 = XP-ZP
    fp2sub751(Q->X, Q->Z, P->X);                       // XP = XQ-ZQ
    fp2add751(Q->X, Q->Z, P->Z);                       // ZP = XQ+ZQ
    fp2mul751_mont(t0, P->X, t0);                      // t0 = (XP+ZP)*(XQ-ZQ)                           
    fp2mul751_mont(t1, P->Z, t1);                      // t1 = (XP-ZP)*(XQ+ZQ)
    fp2sub751(t0, t1, P->Z);                           // ZP = (XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)
    fp2add751(t0, t1, P->X);                           // XP = (XP+ZP)*(XQ-ZQ)+(XP-ZP)*(XQ+ZQ)
    fp2sqr751_mont(P->Z, P->Z);                        // ZP = [(XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)]^2
    fp2sqr751_mont(P->X, P->X);                        // XP = [(XP+ZP)*(XQ-ZQ)+(XP-ZP)*(XQ+ZQ)]^2
    fp2mul751_mont(P->Z, xPQ, P->Z);                   // ZP = xPQ*[(XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)]^2
}


void xDBL_basefield(point_basefield_proj_t P, point_basefield_proj_t Q)
{ // Doubling of a Montgomery point in projective coordinates (X:Z) over the base field.
  // Input: projective Montgomery x-coordinates P = (X1:Z1), where x1=X1/Z1 and Montgomery curve constant A24/C24=(A/C+2)/4.
  // Output: projective Montgomery x-coordinates Q = 2*P = (X2:Z2).
    felm_t t0, t1;

    // NOTE: this function is fixed for A24=1, C24=2
    
    fpsub751(P->X, P->Z, t0);                          // t0 = X1-Z1
    fpadd751(P->X, P->Z, t1);                          // t1 = X1+Z1
    fpsqr751_mont(t0, t0);                             // t0 = (X1-Z1)^2 
    fpsqr751_mont(t1, t1);                             // t1 = (X1+Z1)^2   
    fpadd751(t0, t0, Q->Z);                            // Z2 = C24*(X1-Z1)^2 
    fpmul751_mont(t1, Q->Z, Q->X);                     // X2 = C24*(X1-Z1)^2*(X1+Z1)^2
    fpsub751(t1, t0, t1);                              // t1 = (X1+Z1)^2-(X1-Z1)^2 
    fpadd751(Q->Z, t1, Q->Z);                          // Z2 = A24*[(X1+Z1)^2-(X1-Z1)^2] + C24*(X1-Z1)^2
    fpmul751_mont(Q->Z, t1, Q->Z);                     // Z2 = [A24*[(X1+Z1)^2-(X1-Z1)^2] + C24*(X1-Z1)^2]*[(X1+Z1)^2-(X1-Z1)^2]
}


void xDBLADD_basefield(point_basefield_proj_t P, point_basefield_proj_t Q, felm_t xPQ, felm_t A24)
{ // Simultaneous doubling and differential addition over the base field.
  // Input: projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, affine difference xPQ=x(P-Q) and Montgomery curve constant A24=(A+2)/4.
  // Output: projective Montgomery points P <- 2*P = (X2P:Z2P) such that x(2P)=X2P/Z2P, and Q <- P+Q = (XQP:ZQP) such that = x(Q+P)=XQP/ZQP. 
    felm_t t0, t1, t2;

    // NOTE: this function is fixed for C24=2

    fpadd751(P->X, P->Z, t0);                          // t0 = XP+ZP
    fpsub751(P->X, P->Z, t1);                          // t1 = XP-ZP
    fpsqr751_mont(t0, P->X);                           // XP = (XP+ZP)^2
    fpsub751(Q->X, Q->Z, t2);                          // t2 = XQ-ZQ
    fpadd751(Q->X, Q->Z, Q->X);                        // XQ = XQ+ZQ
    fpmul751_mont(t0, t2, t0);                         // t0 = (XP+ZP)*(XQ-ZQ)
    fpsqr751_mont(t1, P->Z);                           // ZP = (XP-ZP)^2
    fpmul751_mont(t1, Q->X, t1);                       // t1 = (XP-ZP)*(XQ+ZQ)
    fpsub751(P->X, P->Z, t2);                          // t2 = (XP+ZP)^2-(XP-ZP)^2

    if (A24[0] == 1) {
        fpadd751(P->Z, P->Z, P->Z);                    // ZP = C24*(XP-ZP)^2
        fpmul751_mont(P->X, P->Z, P->X);               // XP = C24*(XP+ZP)^2*(XP-ZP)^2
        fpadd751(t2, P->Z, P->Z);                      // ZP = A24*[(XP+ZP)^2-(XP-ZP)^2]+C24*(XP-ZP)^2
    } else {
        fpmul751_mont(P->X, P->Z, P->X);               // XP = (XP+ZP)^2*(XP-ZP)^2
        fpmul751_mont(A24, t2, Q->X);                  // XQ = A24*[(XP+ZP)^2-(XP-ZP)^2]
        fpadd751(P->Z, Q->X, P->Z);                    // ZP = A24*[(XP+ZP)^2-(XP-ZP)^2]+C24*(XP-ZP)^2
    }
    
    fpsub751(t0, t1, Q->Z);                            // ZQ = (XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)
    fpadd751(t0, t1, Q->X);                            // XQ = (XP+ZP)*(XQ-ZQ)+(XP-ZP)*(XQ+ZQ)
    fpmul751_mont(P->Z, t2, P->Z);                     // ZP = [A24*[(XP+ZP)^2-(XP-ZP)^2]+C24*(XP-ZP)^2]*[(XP+ZP)^2-(XP-ZP)^2]
    fpsqr751_mont(Q->Z, Q->Z);                         // ZQ = [(XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)]^2
    fpsqr751_mont(Q->X, Q->X);                         // XQ = [(XP+ZP)*(XQ-ZQ)+(XP-ZP)*(XQ+ZQ)]^2
    fpmul751_mont(Q->Z, xPQ, Q->Z);                    // ZQ = xPQ*[(XP+ZP)*(XQ-ZQ)-(XP-ZP)*(XQ+ZQ)]^2
}


void ladder(felm_t x, digit_t* m, point_basefield_proj_t P, point_basefield_proj_t Q, felm_t A24, unsigned int order_bits, unsigned int order_fullbits, PCurveIsogenyStruct CurveIsogeny)
{ // The Montgomery ladder
  // Inputs: the affine x-coordinate of a point P on E: B*y^2=x^3+A*x^2+x, 
  //         scalar m
  //         curve constant A24 = (A+2)/4
  //         order_bits = subgroup order bitlength
  //         order_fullbits = smallest multiple of 32 larger than the order bitlength
  // Output: Q = m*(x:1)
  // CurveIsogeny must be set up in advance using SIDH_curve_initialize().
    unsigned int bit = 0, owords = NBITS_TO_NWORDS(order_fullbits);
    digit_t mask;
    int i;

    // Initializing with the points (1:0) and (x:1)
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)P->X);
    fpzero751(P->Z);
    fpcopy751(x, Q->X);
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)Q->Z);

    for (i = order_fullbits-order_bits; i > 0; i--) {
        mp_shiftl1(m, owords);
    }
    
    for (i = order_bits; i > 0; i--) {
        bit = (unsigned int)(m[owords-1] >> (RADIX-1));
        mp_shiftl1(m, owords);
        mask = 0-(digit_t)bit;

        swap_points_basefield(P, Q, mask);
        xDBLADD_basefield(P, Q, x, A24);           // If bit=0 then P <- 2*P and Q <- P+Q, 
        swap_points_basefield(P, Q, mask);         // else if bit=1 then Q <- 2*Q and P <- P+Q
    }
}


SIDH_CRYPTO_STATUS BigMont_ladder(unsigned char* x, digit_t* m, unsigned char* xout, PCurveIsogenyStruct CurveIsogeny)
{ // BigMont's scalar multiplication using the Montgomery ladder
  // Inputs: x, the affine x-coordinate of a point P on BigMont: y^2=x^3+A*x^2+x, 
  //         scalar m.
  // Output: xout, the affine x-coordinate of m*(x:1)
  // CurveIsogeny must be set up in advance using SIDH_curve_initialize().
    point_basefield_proj_t P1, P2;
    digit_t scalar[BIGMONT_NWORDS_ORDER];
    felm_t X, A24 = {0};

    A24[0] = (digit_t)CurveIsogeny->BigMont_A24; 
    to_mont(A24, A24);                               // Conversion to Montgomery representation
    to_mont((digit_t*)x, X);
    
    copy_words(m, scalar, BIGMONT_NWORDS_ORDER);
    ladder(X, scalar, P1, P2, A24, BIGMONT_NBITS_ORDER, BIGMONT_MAXBITS_ORDER, CurveIsogeny);

    fpinv751_mont(P1->Z);
    fpmul751_mont(P1->X, P1->Z, (digit_t*)xout);
    from_mont((digit_t*)xout, (digit_t*)xout);       // Conversion to standard representation

    return SIDH_CRYPTO_SUCCESS;
}


SIDH_CRYPTO_STATUS secret_pt(point_basefield_t P, digit_t* m, unsigned int AliceOrBob, point_proj_t R, PCurveIsogenyStruct CurveIsogeny)
{ // Computes key generation entirely in the base field by exploiting a 1-dimensional Montgomery ladder in the trace zero subgroup and 
  // recovering the y-coordinate for the addition. All operations in the base field GF(p).
  // Input:  The scalar m, point P = (x,y) on E in the base field subgroup and Q = (x1,y1*i) on E in the trace-zero subgroup. 
  //         x,y,x1,y1 are all in the base field.          
  // Output: R = (RX0+RX1*i)/RZ0 (the x-coordinate of P+[m]Q).
    unsigned int nbits;
    point_basefield_t Q;
    point_basefield_proj_t S, T;
    digit_t *X0 = (digit_t*)S->X, *Z0 = (digit_t*)S->Z, *X1 = (digit_t*)T->X, *Z1 = (digit_t*)T->Z;
    digit_t *x  = (digit_t*)P->x, *y  = (digit_t*)P->y, *x1 = (digit_t*)Q->x, *y1 = (digit_t*)Q->y;
    digit_t scalar[SIDH_NWORDS_ORDER];
    felm_t t0, t1, t2, A24 = {0};
    digit_t *RX0 = (digit_t*)R->X[0], *RX1 = (digit_t*)R->X[1], *RZ0 = (digit_t*)R->Z[0], *RZ1 = (digit_t*)R->Z[1];

    fpcopy751(P->x, Q->x);                         // Q = (-XP,YP)
    fpcopy751(P->y, Q->y);
    fpneg751(Q->x);

    if (AliceOrBob == ALICE) {
        nbits = CurveIsogeny->oAbits;
    } else if (AliceOrBob == BOB) {
        nbits = CurveIsogeny->oBbits;
    } else {
        return SIDH_CRYPTO_ERROR_INVALID_PARAMETER;
    }
        
    // Setting curve constant to one (in standard representation), used in xDBLADD_basefield() in the ladder computation
    A24[0] = 1;
    copy_words(m, scalar, SIDH_NWORDS_ORDER);
    ladder(Q->x, scalar, S, T, A24, nbits, CurveIsogeny->owordbits, CurveIsogeny);
    
    //RX0 := (2*y*y1*Z0^2*Z1 + Z1*(X0*x1+Z0)*(X0+x1*Z0) - X1*(X0-x1*Z0)^2)*(2*y*y1*Z0^2*Z1 - Z1*(X0*x1+Z0)*(X0+x1*Z0) + X1*(X0-x1*Z0)^2) - 4*y1^2*Z0*Z1^2*(X0+x*Z0)*(X0-x*Z0)^2;
    //RX1 := 4*y*y1*Z0^2*Z1*(Z1*(X0*x1+Z0)*(X0+x1*Z0) - X1*(X0-x1*Z0)^2);
    //RZ0 := 4*y1^2*Z0^2*Z1^2*(X0-x*Z0)^2;

    fpmul751_mont(x1, Z0, RX1);
    fpmul751_mont(X0, x1, RX0);
    fpsub751(X0, RX1, t0);
    fpadd751(X0, RX1, RX1);
    fpsqr751_mont(t0, t0);
    fpadd751(RX0, Z0, RX0);
    fpmul751_mont(t0, X1, t0);
    fpmul751_mont(RX0, RX1, RX0);
    fpmul751_mont(y1, Z1, t2);
    fpmul751_mont(y, Z0, t1);
    fpadd751(t2, t2, t2);
    fpmul751_mont(t2, Z0, RX1);
    fpmul751_mont(RX0, Z1, RX0);
    fpsub751(RX0, t0, RX0);
    fpmul751_mont(t1, RX1, t1);
    fpsqr751_mont(RX1, t0);
    fpmul751_mont(t2, RX1, t2);
    fpmul751_mont(t1, RX0, RX1);
    fpadd751(t1, RX0, RZ0);
    fpadd751(RX1, RX1, RX1);
    fpsub751(t1, RX0, t1);
    fpmul751_mont(x, Z0, RX0);
    fpmul751_mont(t1, RZ0, t1);
    fpsub751(X0, RX0, RZ0);
    fpadd751(X0, RX0, RX0);
    fpsqr751_mont(RZ0, RZ0);
    fpmul751_mont(t2, RX0, t2);
    fpmul751_mont(t2, RZ0, t2);
    fpmul751_mont(RZ0, t0, RZ0);
    fpsub751(t1, t2, RX0);
    fpzero751(RZ1);

    return SIDH_CRYPTO_SUCCESS;
}


SIDH_CRYPTO_STATUS ladder_3_pt(f2elm_t xP, f2elm_t xQ, f2elm_t xPQ, digit_t* m, unsigned int AliceOrBob, point_proj_t W, f2elm_t A, PCurveIsogenyStruct CurveIsogeny)
{ // Computes P+[m]Q via x-only arithmetic. Algorithm by De Feo, Jao and Plut.
  // Input:  three affine points xP,xQ,xPQ and Montgomery constant A.
  // Output: projective Montgomery x-coordinates of x(P+[m]Q)=WX/WZ
    point_proj_t U = {0}, V = {0};
    f2elm_t A24, A24num, constant1 = {0}, constant2;
    felm_t temp_scalar;
    unsigned int bit = 0, nbits, fullbits = CurveIsogeny->owordbits;
    digit_t mask;
    int i;

    if (AliceOrBob == ALICE) {
        nbits = CurveIsogeny->oAbits;
    } else if (AliceOrBob == BOB) {
        nbits = CurveIsogeny->oBbits;
    } else {
        return SIDH_CRYPTO_ERROR_INVALID_PARAMETER;
    }
   
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, constant1[0]);
    fp2add751(constant1, constant1, constant1);                  // constant = 2
    fp2add751(A, constant1, A24num);
    fp2div2_751(A24num, A24);  
    fp2div2_751(A24, A24);
    
    // Initializing with the points (1:0), (xQ:1) and (xP:1)
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)U->X);
    fp2copy751(xQ, V->X);
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)V->Z);
    fp2copy751(xP, W->X);
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)W->Z);
    fpzero751(W->Z[1]);
    fpcopy751(m, temp_scalar);
    
    for (i = fullbits-nbits; i > 0; i--) {
        mp_shiftl1(temp_scalar, SIDH_NWORDS_ORDER);
    }
    
    for (i = nbits; i > 0; i--) {
        bit = (unsigned int)(temp_scalar[SIDH_NWORDS_ORDER-1] >> (RADIX-1));
        mp_shiftl1(temp_scalar, SIDH_NWORDS_ORDER);
        mask = 0 - (digit_t)bit;

        swap_points(W, U, mask);
        swap_points(U, V, mask);
        select_f2elm(xP, xQ, constant1, mask);
        select_f2elm(xQ, xPQ, constant2, mask);
        xADD(W, U, constant1);                     // If bit=0 then W <- W+U, U <- 2*U and V <- U+V, 
        xDBLADD(U, V, constant2, A24);             // else if bit=1 then U <- U+V, V <- 2*V and W <- V+W
        swap_points(U, V, mask);
        swap_points(W, U, mask);
    }

    return SIDH_CRYPTO_SUCCESS;
}


void get_4_isog(point_proj_t P, f2elm_t A, f2elm_t C, f2elm_t* coeff)
{ // Computes the corresponding 4-isogeny of a projective Montgomery point (X4:Z4) of order 4.
  // Input:  projective point of order four P = (X4:Z4).
  // Output: the 4-isogenous Montgomery curve with projective coefficient A/C and the 5 coefficients 
  //         that are used to evaluate the isogeny at a point in eval_4_isog().
    
    fp2add751(P->X, P->Z, coeff[0]);                   // coeff[0] = X4+Z4
    fp2sqr751_mont(P->X, coeff[3]);                    // coeff[3] = X4^2
    fp2sqr751_mont(P->Z, coeff[4]);                    // coeff[4] = Z4^2
    fp2sqr751_mont(coeff[0], coeff[0]);                // coeff[0] = (X4+Z4)^2
    fp2add751(coeff[3], coeff[4], coeff[1]);           // coeff[1] = X4^2+Z4^2
    fp2sub751(coeff[3], coeff[4], coeff[2]);           // coeff[2] = X4^2-Z4^2
    fp2sqr751_mont(coeff[3], coeff[3]);                // coeff[3] = X4^4
    fp2sqr751_mont(coeff[4], coeff[4]);                // coeff[4] = Z4^4
    fp2add751(coeff[3], coeff[3], A);                  // A = 2*X4^4
    fp2sub751(coeff[0], coeff[1], coeff[0]);           // coeff[0] = 2*X4*Z4 = (X4+Z4)^2 - (X4^2+Z4^2)
    fp2sub751(A, coeff[4], A);                         // A = 2*X4^4-Z4^4
    fp2copy751(coeff[4], C);                           // C = Z4^4
    fp2add751(A, A, A);                                // A = 2(2*X4^4-Z4^4)
}


void eval_4_isog(point_proj_t P, f2elm_t* coeff)
{ // Evaluates the isogeny at the point (X:Z) in the domain of the isogeny, given a 4-isogeny phi defined 
  // by the 5 coefficients in coeff (computed in the function four_isogeny_from_projective_kernel()).
  // Inputs: the coefficients defining the isogeny, and the projective point P = (X:Z).
  // Output: the projective point P = phi(P) = (X:Z) in the codomain. 
    f2elm_t t0, t1;

    fp2mul751_mont(P->X, coeff[0], P->X);              // X = coeff[0]*X
    fp2mul751_mont(P->Z, coeff[1], t0);                // t0 = coeff[1]*Z
    fp2sub751(P->X, t0, P->X);                         // X = X-t0
    fp2mul751_mont(P->Z, coeff[2], P->Z);              // Z = coeff[2]*Z
    fp2sub751(P->X, P->Z, t0);                         // t0 = X-Z
    fp2mul751_mont(P->Z, P->X, P->Z);                  // Z = X*Z
    fp2sqr751_mont(t0, t0);                            // t0 = t0^2
    fp2add751(P->Z, P->Z, P->Z);                       // Z = Z+Z
    fp2add751(P->Z, P->Z, P->Z);                       // Z = Z+Z
    fp2add751(P->Z, t0, P->X);                         // X = t0+Z
    fp2mul751_mont(P->Z, t0, P->Z);                    // Z = t0*Z
    fp2mul751_mont(P->Z, coeff[4], P->Z);              // Z = coeff[4]*Z
    fp2mul751_mont(t0, coeff[4], t0);                  // t0 = t0*coeff[4]
    fp2mul751_mont(P->X, coeff[3], t1);                // t1 = X*coeff[3]
    fp2sub751(t0, t1, t0);                             // t0 = t0-t1
    fp2mul751_mont(P->X, t0, P->X);                    // X = X*t0
}


void first_4_isog(point_proj_t P, f2elm_t A, f2elm_t Aout, f2elm_t Cout, PCurveIsogenyStruct CurveIsogeny)
{ // Computes first 4-isogeny computed by Alice.
  // Inputs: projective point P = (X4:Z4) and curve constant A.
  // Output: the projective point P = (X4:Z4) in the codomain and isogenous curve constant Aout/Cout.  
    f2elm_t t0 = {0}, t1, t2;
    
    fpcopy751(CurveIsogeny->Montgomery_one, t0[0]); 
    fpadd751(t0[0], t0[0], t0[0]);                     // t0 = 2 (in Montgomery domain)
    fp2sub751(A, t0, Cout);                            // Cout = A-2
    fpadd751(t0[0], t0[0], t1[0]);                     
    fpadd751(t0[0], t1[0], t0[0]);                     // t0 = 6 (in Montgomery domain)
    fp2add751(P->X, P->Z, t1);                         // t1 = X+Z
    fp2sub751(P->X, P->Z, t2);                         // t2 = X-Z
    fp2sqr751_mont(t1, t1);                            // t1 = (X+Z)^2
    fp2add751(A, t0, Aout);                            // A = A+6
    fp2mul751_mont(P->X, P->Z, P->Z);                  // Z = X*Z
    fp2neg751(P->Z);                                   // Z = -X*Z
    fp2sqr751_mont(t2, t2);                            // t2 = (X-Z)^2
    fp2mul751_mont(P->Z, Cout, P->Z);                  // Z = -C*X*Z
    fp2add751(Aout, Aout, Aout);                       // Aout = 2*A+12
    fp2sub751(t1, P->Z, P->X);                         // X = (X+Z)^2+C*X*Z
    fp2mul751_mont(P->Z, t2, P->Z);                    // Z = -C*X*Z*(X-Z)^2
    fp2mul751_mont(P->X, t1, P->X);                    // X = (X+Z)^2*[(X+Z)^2+C*X*Z]
}


void xTPL(point_proj_t P, point_proj_t Q, f2elm_t A24, f2elm_t C24)
{ // Tripling of a Montgomery point in projective coordinates (X:Z).
  // Input: projective Montgomery x-coordinates P = (X:Z), where x=X/Z and Montgomery curve constant A/C.
  // Output: projective Montgomery x-coordinates Q = 3*P = (X3:Z3).
    f2elm_t t0, t1, t2, t3, t4, t5;

    fp2sub751(P->X, P->Z, t2);                         // t2 = X-Z           
    fp2add751(P->X, P->Z, t3);                         // t3 = X+Z 
    fp2sqr751_mont(t2, t0);                            // t0 = t2^2 
    fp2sqr751_mont(t3, t1);                            // t1 = t3^2 
    fp2mul751_mont(t0, C24, t4);                       // t4 = C24*t0 
    fp2mul751_mont(t1, t4, t5);                        // t5 = t4*t1
    fp2sub751(t1, t0, t1);                             // t1 = t1-t0 
    fp2mul751_mont(A24, t1, t0);                       // t0 = A24*t1
    fp2add751(t4, t0, t4);                             // t4 = t4+t0
    fp2mul751_mont(t1, t4, t4);                        // t4 = t4*t1
    fp2add751(t5, t4, t0);                             // t0 = t5+t4
    fp2sub751(t5, t4, t1);                             // t1 = t5-t4
    fp2mul751_mont(t0, t2, t0);                        // t0 = t2*t0
    fp2mul751_mont(t1, t3, t1);                        // t1 = t3*t1
    fp2sub751(t0, t1, t4);                             // t4 = t0-t1
    fp2add751(t0, t1, t5);                             // t5 = t0+t1
    fp2sqr751_mont(t4, t4);                            // t4 = t4^2
    fp2sqr751_mont(t5, t5);                            // t5 = t5^2
    fp2mul751_mont(P->X, t4, t4);                      // t4 = X*t4
    fp2mul751_mont(P->Z, t5, Q->X);                    // X3 = Z*t5
    fp2copy751(t4, Q->Z);                              // Z3 = t4
}


void xTPLe(point_proj_t P, point_proj_t Q, f2elm_t A, f2elm_t C, int e)
{ // Computes [3^e](X:Z) on Montgomery curve with projective constant via e repeated triplings.
  // Input: projective Montgomery x-coordinates P = (XP:ZP), such that xP=XP/ZP and Montgomery curve constant A/C.
  // Output: projective Montgomery x-coordinates P <- (3^e)*P.
    f2elm_t A24, C24;
    int i;
    
    fp2add751(C, C, A24);                           
    fp2add751(A24, A24, C24);                    
    fp2add751(A24, A, A24);       
    copy_words((digit_t*)P, (digit_t*)Q, 2*2*NWORDS_FIELD);

    for (i = 0; i < e; i++) {
        xTPL(Q, Q, A24, C24);
    }
}


void get_3_isog(point_proj_t P, f2elm_t A, f2elm_t C)
{ // Computes the corresponding 3-isogeny of a projective Montgomery point (X3:Z3) of order 3.
  // Input:  projective point of order three P = (X3:Z3).
  // Output: the 3-isogenous Montgomery curve with projective coefficient A/C. 
    f2elm_t t0, t1;

    fp2sqr751_mont(P->X, t0);                          // t0 = X^2
    fp2add751(t0, t0, t1);                             // t1 = 2*t0
    fp2add751(t0, t1, t0);                             // t0 = t0+t1
    fp2sqr751_mont(P->Z, t1);                          // t1 = Z^2
    fp2sqr751_mont(t1, A);                             // A = t1^2
    fp2add751(t1, t1, t1);                             // t1 = 2*t1
    fp2add751(t1, t1, C);                              // C = 2*t1
    fp2sub751(t0, t1, t1);                             // t1 = t0-t1
    fp2mul751_mont(t0, t1, t1);                        // t1 = t0*t1
    fp2sub751(A, t1, A);                               // A = A-t1 
    fp2sub751(A, t1, A);                               // A = A-t1 
    fp2sub751(A, t1, A);                               // A = A-t1     
    fp2mul751_mont(P->X, P->Z, t1);                    // t1 = X*Z    // ms trade-off possible (1 mul for 1sqr + 1add + 2sub)
    fp2mul751_mont(C, t1, C);                          // C = C*t1
}


void eval_3_isog(point_proj_t P, point_proj_t Q)
{ // Computes the 3-isogeny R=phi(X:Z), given projective point (X3:Z3) of order 3 on a Montgomery curve and a point P = (X:Z).
  // Inputs: projective points P = (X3:Z3) and Q = (X:Z).
  // Output: the projective point R = phi(Q) = (XX:ZZ). 
    f2elm_t t0, t1, t2;

    fp2mul751_mont(P->X, Q->X, t0);                  // t0 = X3*X
    fp2mul751_mont(P->Z, Q->X, t1);                  // t1 = Z3*X
    fp2mul751_mont(P->Z, Q->Z, t2);                  // t2 = Z3*Z
    fp2sub751(t0, t2, t0);                           // t0 = X3*X-Z3*Z          
    fp2mul751_mont(P->X, Q->Z, t2);                  // t2 = X3*Z
    fp2sub751(t1, t2, t1);                           // t1 = Z3*X-X3*Z
    fp2sqr751_mont(t0, t0);                          // t0 = (X3*X-Z3*Z)^2
    fp2sqr751_mont(t1, t1);                          // t1 = (Z3*X-X3*Z)^2
    fp2mul751_mont(Q->X, t0, Q->X);                  // X = X*(X3*X-Z3*Z)^2        
    fp2mul751_mont(Q->Z, t1, Q->Z);                  // Z = Z*(Z3*X-X3*Z)^2
}


void inv_3_way(f2elm_t z1, f2elm_t z2, f2elm_t z3)
{ // 3-way simultaneous inversion
  // Input:  z1,z2,z3
  // Output: 1/z1,1/z2,1/z3 (override inputs).
    f2elm_t t0, t1, t2, t3;

    fp2mul751_mont(z1, z2, t0);                      // t0 = z1*z2
    fp2mul751_mont(z3, t0, t1);                      // t1 = z1*z2*z3
    fp2inv751_mont(t1);                              // t1 = 1/(z1*z2*z3)
    fp2mul751_mont(z3, t1, t2);                      // t2 = 1/(z1*z2) 
    fp2mul751_mont(t2, z2, t3);                      // t3 = 1/z1
    fp2mul751_mont(t2, z1, z2);                      // z2 = 1/z2
    fp2mul751_mont(t0, t1, z3);                      // z3 = 1/z3
    fp2copy751(t3, z1);                              // z1 = 1/z1
}


void distort_and_diff(felm_t xP, point_proj_t D, PCurveIsogenyStruct CurveIsogeny)
{ // Computing the point (x(Q-P),z(Q-P))
  // Input:  coordinate xP of point P=(xP,yP)
  // Output: the point D = (x(Q-P),z(Q-P)), where Q=tau(P).
    felm_t one;

    fpcopy751(CurveIsogeny->Montgomery_one, one);
    fpsqr751_mont(xP, D->X[0]);	                     // XD = xP^2
    fpadd751(D->X[0], one, D->X[0]);                 // XD = XD+1
    fpcopy751(D->X[0], D->X[1]);                     // XD = XD*i
    fpzero751(D->X[0]);          
    fpadd751(xP, xP, D->Z[0]);                       // ZD = xP+xP
}


void get_A(f2elm_t xP, f2elm_t xQ, f2elm_t xR, f2elm_t A, PCurveIsogenyStruct CurveIsogeny)
{ // Given the x-coordinates of P, Q, and R, returns the value A corresponding to the Montgomery curve E_A: y^2=x^3+A*x^2+x such that R=Q-P on E_A.
  // Input:  the x-coordinates xP, xQ, and xR of the points P, Q and R.
  // Output: the coefficient A corresponding to the curve E_A: y^2=x^3+A*x^2+x.
    f2elm_t t0, t1, one = {0};
    
    fpcopy751(CurveIsogeny->Montgomery_one, one[0]);
    fp2add751(xP, xQ, t1);                           // t1 = xP+xQ
    fp2mul751_mont(xP, xQ, t0);                      // t0 = xP*xQ
    fp2mul751_mont(xR, t1, A);                       // A = xR*t1
    fp2add751(t0, A, A);                             // A = A+t0
    fp2mul751_mont(t0, xR, t0);                      // t0 = t0*xR
    fp2sub751(A, one, A);                            // A = A-1
    fp2add751(t0, t0, t0);                           // t0 = t0+t0
    fp2add751(t1, xR, t1);                           // t1 = t1+xR
    fp2add751(t0, t0, t0);                           // t0 = t0+t0
    fp2sqr751_mont(A, A);                            // A = A^2
    fp2inv751_mont(t0);                              // t0 = 1/t0
    fp2mul751_mont(A, t0, A);                        // A = A*t0
    fp2sub751(A, t1, A);                             // Afinal = A-t1
}