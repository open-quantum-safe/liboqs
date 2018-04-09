//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <montgomery.h>

void mont_curve_init(ff_Params* p, mont_curve_int_t* curve) {
  curve->ffData = p;

  mont_pt_init(p, &curve->P);
  mont_pt_init(p, &curve->Q);
  fp2_Init(p, &curve->a);
  fp2_Init(p, &curve->b);
}

void mont_curve_clear(const ff_Params* p, mont_curve_int_t* curve) {
  mont_pt_clear(p, &curve->P);
  mont_pt_clear(p, &curve->Q);
  fp2_Clear(p, &curve->a);
  fp2_Clear(p, &curve->b);
}

void mont_curve_copy(const ff_Params* p, const mont_curve_int_t* curve, mont_curve_int_t* curvecopy) {
  if (curve != curvecopy) {
    mont_pt_copy(p, &curve->P, &curvecopy->P);
    mont_pt_copy(p, &curve->Q, &curvecopy->Q);
    fp2_Copy(p, &curve->a, &curvecopy->a);
    fp2_Copy(p, &curve->b, &curvecopy->b);
  }
}

void mont_pt_init(const ff_Params* p, mont_pt_t* pt) {
  fp2_Init(p, &pt->x);
  fp2_Init(p, &pt->y);
}

void mont_pt_clear(const ff_Params* p, mont_pt_t* pt) {
  fp2_Clear(p, &pt->x);
  fp2_Clear(p, &pt->y);
}

void public_key_init(ff_Params* p, sike_public_key_t* pk) {
  pk->ffData = p;
  fp2_Init(p, &pk->xP);
  fp2_Init(p, &pk->xQ);
  fp2_Init(p, &pk->xR);
}

void public_key_clear(const ff_Params* p, sike_public_key_t* pk) {
  fp2_Clear(p, &pk->xP);
  fp2_Clear(p, &pk->xQ);
  fp2_Clear(p, &pk->xR);

}

void mont_pt_copy(const ff_Params* p, const mont_pt_t* src, mont_pt_t* dst) {
  if (src != dst) {
    fp2_Copy(p, &src->x, &dst->x);
    fp2_Copy(p, &src->y, &dst->y);
  }
}

/* infinity is represented as a point with (0, 0) */
static void mont_set_inf_affine(const mont_curve_int_t* curve, mont_pt_t *P) {

  const ff_Params *p = curve->ffData;

  fp2_Set( p, &P->x, 0, 0 );
  fp2_Set( p, &P->y, 0, 0 );
}

/* returns 1 for True, 0 for False */
static int mont_is_inf_affine(const mont_curve_int_t* curve, const mont_pt_t *P) {
  return fp2_IsConst( curve->ffData, &P->y, 0, 0 );
}

void mont_double_and_add(const mont_curve_int_t* curve, const mp k, const mont_pt_t* P, mont_pt_t *Q, int msb) {

  int i;

  const ff_Params *p = curve->ffData;
  mont_pt_t kP = { 0 };
  mont_pt_init(p, &kP);

  mont_set_inf_affine(curve, &kP);

  for (i = msb - 1; i >= 0; i--) {
    xDBL(curve, &kP, &kP);
    if (fp_IsBitSet(p, k, i)) {
      xADD(curve, &kP, P, &kP);
    }
  }

  mont_pt_copy(p, &kP, Q);

  mont_pt_clear(p, &kP);
}

/**
 * x3 = b*(3*x1^2+2*a*x1+1)2/(2*b*y1)2-a-x1-x1
 * y3 = (2*x1+x1+a)*(3*x12+2*a*x1+1)/(2*b*y1)-b*(3*x12+2*a*x1+1)3/(2*b*y1)3-y1
 * @param data
 * @param P
 * @param R
 * @param tmp
 * @return
 */
void xDBL(const mont_curve_int_t *curve, const mont_pt_t *P, mont_pt_t *R) {
  const ff_Params *p = curve->ffData;

  const fp2* a = &curve->a;
  const fp2* b = &curve->b;

  // x3 = b*(3*x1^2+2*a*x1+1)^2/(2*b*y1)^2-a-x1-x1
  // y3 = (2*x1+x1+a)*(3*x1^2+2*a*x1+1)/(2*b*y1)-b*(3*x1^2+2*a*x1+1)^3/(2*b*y1)^3-y1

  if (mont_is_inf_affine(curve, P) ) {
    mont_set_inf_affine(curve, R);
    return;
  }

  fp2 t0 = { 0 }, t1 = { 0 }, t2 = { 0 };
  fp2_Init(p, &t0);
  fp2_Init(p, &t1);
  fp2_Init(p, &t2);

  fp2_Set(p, &t2, 1, 0);                        // t2 = 1

  fp2_Square(p, &P->x, &t0);                    // t0 = x1^2
  fp2_Add(p, &t0, &t0, &t1);                    // t0 = 2*x1^2
  fp2_Add(p, &t0, &t1, &t0);                    // t0 = 3*x1^2

  fp2_Multiply(p, a, &P->x, &t1);               // t1 = a*x1
  fp2_Add(p, &t1, &t1, &t1);                    // t1 = 2*a*x1


  fp2_Add(p, &t0, &t1, &t0);                    // t0 = 3*x1^2+2*a*x1
  fp2_Add(p, &t0, &t2, &t0);                    // t0 = 3*x1^2+2*a*x1+1

  fp2_Multiply(p, b, &P->y, &t1);               // t1 = b*y1
  fp2_Add(p, &t1, &t1, &t1);                    // t1 = 2*b*y1
  fp2_Invert(p, &t1, &t1);                      // t1 = 1 / (2*b*y1)

  fp2_Multiply(p, &t0, &t1, &t0);               // t0 = (3*x1^2+2*a*x1+1) / (2*b*y1)

  fp2_Square(p, &t0, &t1);                      // t1 = (3*x1^2+2*a*x1+1)^2 / (2*b*y1)^2

  fp2_Multiply(p, b, &t1, &t2);                 // t2 = b*(3*x1^2+2*a*x1+1)^2 / (2*b*y1)^2
  fp2_Sub(p, &t2, a, &t2);                      // t2 = b*(3*x1^2+2*a*x1+1)^2 / (2*b*y1)^2 - a
  fp2_Sub(p, &t2, &P->x, &t2);                  // t2 = b*(3*x1^2+2*a*x1+1)^2 / (2*b*y1)^2 - a - x1
  fp2_Sub(p, &t2, &P->x, &t2);                  // t2 = b*(3*x1^2+2*a*x1+1)^2 / (2*b*y1)^2 - a - x1 - x1

  fp2_Multiply(p, &t0, &t1, &t1);               // t1 = (3*x1^2+2*a*x1+1)^3 / (2*b*y1)^3
  fp2_Multiply(p, b, &t1, &t1);                 // t1 = b*(3*x1^2+2*a*x1+1)^3 / (2*b*y1)^3
  fp2_Add(p, &t1, &P->y, &t1);                  // t1 = b*(3*x1^2+2*a*x1+1)^3 / (2*b*y1)^3 + y1

  fp2_Add(p, &P->x, &P->x, &R->y);              // x3 = 2*x1
  fp2_Add(p, &R->y, &P->x, &R->y);              // y3 = 2*x1+x1
  fp2_Add(p, &R->y, a, &R->y);                  // y3 = 2*x1+x1+a
  fp2_Multiply(p, &R->y, &t0, &R->y);           // y3 = (2*x1+x1+a)*(3*x1^2+2*a*x1+1)/(2*b*y1)
  fp2_Sub(p, &R->y, &t1, &R->y);                // y3 = (2*x1+x1+a)*(3*x1^2+2*a*x1+1)/(2*b*y1) - (b*(3*x1^2+2*a*x1+1)^3/(2*b*y1)^3 + y1)

  fp2_Copy(p, &t2, &R->x);                      // x3 = b*(3*x1^2+2*a*x1+1)^2 / (2*b*y1)^2 - a - x1 - x1

  fp2_Clear(p, &t0);
  fp2_Clear(p, &t1);
  fp2_Clear(p, &t2);

}

void xDBLe(const mont_curve_int_t *curve, const mont_pt_t *P, int e, mont_pt_t *R) {
  mont_pt_copy(curve->ffData, P, R);
  for (int j = 0; j < e; ++j)
    xDBL(curve, R, R);
}

void xADD(const mont_curve_int_t *curve, const mont_pt_t *P, const mont_pt_t *Q, mont_pt_t *R) {

  // x3 = b*(y2-y1)^2/(x2-x1)^2-a-x1-x2
  // y3 = (2*x1+x2+a)*(y2-y1)/(x2-x1)-b*(y2-y1)^3/(x2-x1)^3-y1
  // y3 = ((2*x1)+x2+a) * ((y2-y1)/(x2-x1)) - b*((y2-y1)^3/(x2-x1)^3) - y1

  const ff_Params *p = curve->ffData;

  const fp2* a = &curve->a;
  const fp2* b = &curve->b;

  fp2 t0 = { 0 }, t1 = { 0 }, t2 = { 0 };
  fp2_Init(p, &t0);
  fp2_Init(p, &t1);
  fp2_Init(p, &t2);

  fp2_Negative(p, &Q->y, &t0);

  if (mont_is_inf_affine(curve, P) ) {
    mont_pt_copy(p, Q, R);
  }
  else if (mont_is_inf_affine(curve, Q) ) {
    mont_pt_copy(p, P, R);
  }
  else if ( fp2_IsEqual( p, &P->x, &Q->x) && fp2_IsEqual( p, &P->y, &Q->y ) ) {
    /* P == Q */
    xDBL(curve, P, R);
  }
  else if ( fp2_IsEqual( p, &P->x, &Q->x) && fp2_IsEqual(p, &P->y, &t0) ) {
    /* P == -Q */
    mont_set_inf_affine(curve, R);
  }
  else {
    /* P != Q or -Q  */

    fp2_Sub(p, &Q->y, &P->y, &t0);               // t0 = y2-y1
    fp2_Sub(p, &Q->x, &P->x, &t1);               // t1 = x2-x1
    fp2_Invert(p, &t1, &t1);                     // t1 = 1/(x2-x1)
    fp2_Multiply(p, &t0, &t1, &t0);              // t0 = (y2-y1)/(x2-x1)

    fp2_Square(p, &t0, &t1);                     // t1 = (y2-y1)^2/(x2-x1)^2

    fp2_Add(p, &P->x, &P->x, &t2);               // t2 = 2*x1
    fp2_Add(p, &t2, &Q->x, &t2);                 // t2 = 2*x1+x2
    fp2_Add(p, &t2, a, &t2);                     // t2 = 2*x1+x2+a
    fp2_Multiply(p, &t2, &t0, &t2);              // t2 = (2*x1+x2+a)*(y2-y1)/(x2-x1)

    fp2_Multiply(p, &t0, &t1, &t0);              // t0 = (y2-y1)^3/(x2-x1)^3
    fp2_Multiply(p, b, &t0, &t0);                // t0 = b*(y2-y1)^3/(x2-x1)^3
    fp2_Add(p, &t0, &P->y, &t0);                 // t0 = b*(y2-y1)^3/(x2-x1)^3+y1

    fp2_Sub(p, &t2, &t0, &t0);                   // t0 = (2*x1+x2+a)*(y2-y1)/(x2-x1)-b*(y2-y1)^3/(x2-x1)^3-y1

    fp2_Multiply(p, b, &t1, &t1);                // t1 = b*(y2-y1)^2/(x2-x1)^2
    fp2_Sub(p, &t1, a, &t1);                     // t1 = b*(y2-y1)^2/(x2-x1)^2-a
    fp2_Sub(p, &t1, &P->x, &t1);                 // t1 = b*(y2-y1)^2/(x2-x1)^2-a-x1

    fp2_Sub(p, &t1, &Q->x, &R->x);               // x3 = b*(y2-y1)^2/(x2-x1)^2-a-x1-x2

    fp2_Copy(p, &t0, &R->y);                     // y3 = (2*x1+x2+a)*(y2-y1)/(x2-x1)-(b*(y2-y1)^3/(x2-x1)^3+y1)

  }

  fp2_Clear(p, &t0);
  fp2_Clear(p, &t1);
  fp2_Clear(p, &t2);
}

void xTPL(const mont_curve_int_t *curve, const mont_pt_t *P, mont_pt_t *R) {
  const ff_Params* p = curve->ffData;
  mont_pt_t T = { 0 };
  mont_pt_init(p, &T);

  xDBL(curve, P, &T);
  xADD(curve, P, &T, R);

  mont_pt_clear(p, &T);
}

/**
 * Repeated affine tripling.
 *
 * @param curve Underlying curve
 * @param P Point
 * @param e Repetitions
 * @param R Result R=3^e*P
 */
void xTPLe(const mont_curve_int_t *curve, const mont_pt_t *P, int e, mont_pt_t *R) {
  mont_pt_copy(curve->ffData, P, R);
  for (int j = 0; j < e; ++j)
    xTPL(curve, R, R);
}

/**
 * j-invariant:
 * 256*(a^2-3)^3/(a^2-4);
 * @param p
 * @param E
 * @param jinv
 */
void j_inv(const ff_Params *p, const mont_curve_int_t *E, fp2 *jinv) {
  const fp2 *a = &E->a;

  fp2 t0 = { 0 }, t1 = { 0 };
  fp2_Init(p, &t0);
  fp2_Init(p, &t1);

  fp2_Square(p, a, &t0);            // t0 = a^2
  fp2_Set(p, jinv, 3, 0);           // jinv = 3
  fp2_Sub(p, &t0, jinv, jinv);      // jinv = a^2-3
  fp2_Square(p, jinv, &t1);         // t1 = (a^2-3)^2
  fp2_Multiply(p, jinv, &t1, jinv); // jinv = (a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 2*(a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 4*(a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 8*(a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 16*(a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 32*(a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 64*(a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 128*(a^2-3)^3
  fp2_Add(p, jinv, jinv, jinv);     // jinv = 256*(a^2-3)^3

  fp2_Set(p, &t1, 4, 0);            // t1 = 4
  fp2_Sub(p, &t0, &t1, &t0);        // t0 = a^2-4
  fp2_Invert(p, &t0, &t0);          // t0 = 1/(a^2-4)
  fp2_Multiply(p, jinv, &t0, jinv); // jinv = 256*(a^2-3)^3/(a^2-4)

  fp2_Clear(p, &t0);
  fp2_Clear(p, &t1);
}

void get_xR(const ff_Params *p,
            const mont_curve_int_t *curve,
            sike_public_key_t *pk) {

  mont_pt_t R = { 0 };
  mont_pt_init(p, &R);

  mont_pt_copy(p, &curve->Q, &R);
  fp2_Negative(p, &R.y, &R.y);

  xADD(curve, &curve->P, &R, &R);
  fp2_Copy(p, &curve->P.x, &pk->xP);
  fp2_Copy(p, &curve->Q.x, &pk->xQ);
  fp2_Copy(p, &R.x, &pk->xR);

  mont_pt_clear(p, &R);
}

void get_yP_yQ_A_B(const ff_Params *p,
                   const sike_public_key_t *pk,
                   mont_curve_int_t *curve) {

  fp2* a = &curve->a;
  fp2* b = &curve->b;
  mont_pt_t* P = &curve->P;
  mont_pt_t* Q = &curve->Q;

  const fp2* xP = &pk->xP;
  const fp2* xQ = &pk->xQ;
  const fp2* xR = &pk->xR;

  mont_pt_t T = { 0 };
  mont_pt_init(p, &T);

  fp2 *t1 = &T.x, *t2 = &T.y;

  // a:=(1-xP*xQ-xP*xR-xQ*xR)^2/(4*xP*xQ*xR)-xP-xQ-xR;

  fp2_Multiply(p, xP, xQ, a);   // t1 = xP*xQ
  fp2_Multiply(p, a, xR, t1);   // t2 = xP*xQ*xR
  fp2_Add(p, t1, t1, t1);       // t2 = 2*xP*xQ*xR
  fp2_Add(p, t1, t1, t1);       // t2 = 4*xP*xQ*xR
  fp2_Invert(p, t1, t1);        // t2 = 1/(4*xP*xQ*xR)

  fp2_Set(p, t2, 1, 0);         // t3 = 1
  fp2_Sub(p, t2, a, a);         // t1 = 1-xP*xQ
  fp2_Multiply(p, xP, xR, t2);  // t3 = xP*xR
  fp2_Sub(p, a, t2, a);         // t1 = 1-xP*xQ-xP*xR
  fp2_Multiply(p, xQ, xR, t2);  // t3 = xQ*xR
  fp2_Sub(p, a, t2, a);         // t1 = 1-xP*xQ-xP*xR-xQ*xR
  fp2_Square(p, a, a);          // t1 = (1-xP*xQ-xP*xR-xQ*xR)^2

  fp2_Multiply(p, a, t1, a);    // t1 = (1-xP*xQ-xP*xR-xQ*xR)^2/(4*xP*xQ*xR)
  fp2_Sub(p, a, xP, a);         // t1 = (1-xP*xQ-xP*xR-xQ*xR)^2/(4*xP*xQ*xR)-xP
  fp2_Sub(p, a, xQ, a);         // t1 = (1-xP*xQ-xP*xR-xQ*xR)^2/(4*xP*xQ*xR)-xP-xQ
  fp2_Sub(p, a, xR, a);         // a = (1-xP*xQ-xP*xR-xQ*xR)^2/(4*xP*xQ*xR)-xP-xQ-xR


  fp2_Square(p, xP, t1);        // t1 = xP^2
  fp2_Multiply(p, xP, t1, t2);  // t2 = xP^3
  fp2_Multiply(p, a, t1, t1);   // t1 = a*xP^2
  fp2_Add(p, t2, t1, t1);       // t1 = xP^3+a*xP^2
  fp2_Add(p, t1, xP, t1);       // t1 = xP^3+a*xP^2+xP
  fp2_Sqrt(p, t1, &P->y, 0);    // yP = sqrt(xP^3+a*xP^2+xP)

  fp2_Square(p, xQ, t1);        // t1 = xQ^2
  fp2_Multiply(p, xQ, t1, t2);  // t2 = xQ^3
  fp2_Multiply(p, a, t1, t1);   // t1 = a*xQ^2
  fp2_Add(p, t2, t1, t1);       // t1 = xQ^3+a*xQ^2
  fp2_Add(p, t1, xQ, t1);       // t1 = xQ^3+a*xQ^2+xQ
  fp2_Sqrt(p, t1, &Q->y, 0);    // yQ = sqrt(xQ^3+a*xQ^2+xQ)

  fp2_Copy(p, xP, &P->x);
  fp2_Copy(p, xQ, &Q->x);

  fp2_Set(p, b, 1, 0);

  mont_pt_copy(p, Q, &T);
  fp2_Negative(p, &T.y, &T.y);
  xADD(curve, P, &T, &T);

  if (!fp2_IsEqual(p, &T.x, xR))
    fp2_Negative(p, &Q->y, &Q->y);

  mont_pt_clear(p, &T);
}
