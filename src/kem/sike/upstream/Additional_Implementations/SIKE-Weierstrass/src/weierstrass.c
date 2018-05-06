//
// SIKE-Weierstrass
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <weierstrass.h>

void weier_curve_init(ff_Params* p, weier_curve_t* curve) {
  weier_pt_init(p, &curve->P);
  weier_pt_init(p, &curve->Q);
  fp2_Init(p, &curve->a);
  fp2_Init(p, &curve->b);

  curve->ffData = p;
}

void weier_curve_clear(const ff_Params* p, weier_curve_t* curve) {
  weier_pt_clear(p, &curve->P);
  weier_pt_clear(p, &curve->Q);

  fp2_Clear(p, &curve->a);
  fp2_Clear(p, &curve->b);
}

void weier_pt_init(const ff_Params* p, weier_pt_t* pt) {
  fp2_Init(p, &pt->x);
  fp2_Init(p, &pt->y);
}

void weier_pt_clear(const ff_Params* p, weier_pt_t* pt) {
  fp2_Clear(p, &pt->x);
  fp2_Clear(p, &pt->y);
}


void weier_pt_copy(const ff_Params* p, const weier_pt_t* src, weier_pt_t* dst) {
  fp2_Copy(p, &src->x, &dst->x);
  fp2_Copy(p, &src->y, &dst->y);
}

/* infinity is represented as a point with (0, 0) */
void weier_set_inf_affine(const weier_curve_t *data, weier_pt_t *P) {
  const ff_Params *p = data->ffData;

  fp2_Set( p, &P->x, 0, 0 );
  fp2_Set( p, &P->y, 0, 0 );
}

/* returns 1 for True, 0 for False */
int weier_is_inf_affine(const weier_curve_t *data, const weier_pt_t *P) {
  return fp2_IsConst( data->ffData, &P->y, 0, 0 );
}

int weier_double_and_add(const weier_curve_t *data, const mp k, const weier_pt_t *P, weier_pt_t *Q, int msb) {

  int i;

  const ff_Params *p = data->ffData;
  weier_pt_t kP = { 0 };
  weier_pt_init(p, &kP);

  weier_set_inf_affine(data, &kP);

  for (i = msb - 1; i >= 0; i--) {
    weier_xDBL(data, &kP, &kP);
    if (p->isBitSet(p, k, i)) {
      weier_xADD(data, &kP, P, &kP);
    }
  }

  weier_pt_copy(p, &kP, Q);

  weier_pt_clear(p, &kP);
  return 0;
}

int weier_xDBL(const weier_curve_t *data, const weier_pt_t *P, weier_pt_t *R) {
  int status = 0;
  const ff_Params *p = data->ffData;

  fp2 u  = { 0 };
  fp2 v  = { 0 };
  fp2 tx = { 0 };

  fp2_Init(p, &u);
  fp2_Init(p, &v);
  fp2_Init(p, &tx);

  if (weier_is_inf_affine(data, P) ) {
    weier_set_inf_affine(data, R);
  } else {
    fp2_Copy(p, &P->x, &tx);


    fp2_Square    ( p, &P->x, &v );         /* v = x1^2                       */
    fp2_Add       ( p, &v, &v, &u  );       /* u = 2x1^2                      */
    fp2_Add       ( p, &v, &u, &u  );       /* u = 3x^2                       */
    fp2_Add       ( p, &u, &data->a, &v );  /* v = 3x^2 + a                   */

    fp2_Add       ( p, &P->y, &P->y, &u );  /* u = 2y1                        */
    fp2_Invert    ( p, &u, &u );            /* u = 1/(2y)                     */

    fp2_Multiply  ( p, &v, &u, &v );        /* v = lambda                     */
    fp2_Square    ( p, &v, &u );            /* u = lambda^2                   */
    fp2_Sub  ( p, &u, &P->x, &u );          /* u = lambda^2 - x1              */
    fp2_Sub  ( p, &u, &P->x, &R->x );       /* R->x = x3 (= lambda^2 - 2x1)   */

    fp2_Sub  ( p, &tx, &R->x, &u );         /* u = x1 - x3                    */
    fp2_Multiply  ( p, &v, &u, &v );        /* v = lambda*(x1 - x3)           */
    fp2_Sub  ( p, &v, &P->y, &R->y );       /* R->y = y3 (= lambda*(x1-x3)-y1 */

  }

  fp2_Clear(p, &u);
  fp2_Clear(p, &v);
  fp2_Clear(p, &tx);

  return status;
}


int weier_xADD(const weier_curve_t *data, const weier_pt_t *P, const weier_pt_t *Q, weier_pt_t *R) {

  int status = 0;
  const ff_Params *p = data->ffData;

  fp2 u = { 0 };
  fp2 v = { 0 };
  fp2 lambda = { 0 };
  fp2 tx = { 0 };

  fp2_Init(p, &u);
  fp2_Init(p, &v);
  fp2_Init(p, &lambda);
  fp2_Init(p, &tx);


  if (weier_is_inf_affine(data, P) ) {
    weier_pt_copy(p, Q, R);
  }
  else if (weier_is_inf_affine(data, Q) ) {
    weier_pt_copy(p, P, R);
  }
  else if ( fp2_IsEqual( p, &P->x, &Q->x) && fp2_IsEqual( p, &P->y, &Q->y ) ) {
    /* P == Q */
    weier_xDBL(data, P, R);
  }
  else if ( fp2_IsEqual( p, &P->x, &Q->x) && fp2_IsEqual(p, &P->y, &u) ) {
    /* P == -Q */
    weier_set_inf_affine(data, R);
  }
  else { /* P != Q or -Q  */

    fp2_Copy(p, &P->x, &tx);

    fp2_Sub  ( p, &P->x, &Q->x, &u );    /* u = x1 - x2                 */
    fp2_Sub  ( p, &P->y, &Q->y, &v );    /* v = y1 - y2                 */
    fp2_Invert    ( p, &u, &u );         /* u = 1/(x1 - x2)             */
    fp2_Multiply  ( p, &u, &v, &lambda );/* lambda = (y1-y2)/(x1-x2)    */
    fp2_Square    ( p, &lambda, &u );    /* u = lambda^2 = (y1-y2)/(x1-x2)               */

    fp2_Sub  ( p, &u, &P->x, &v);        /* v = lambda^2 - x1 = (y1-y2)/(x1-x2) - x1 */
    fp2_Sub  ( p, &v, &Q->x, &R->x);     /* u = x3 = lambda^2 - x1 - x2 = (y1-y2)/(x1-x2) - x1 - x2 */

    fp2_Sub  ( p, &tx, &R->x, &v );      /* v = x1 - x3                 */
    fp2_Multiply  ( p, &lambda, &v, &u );/* u = lambda*(x1 - x3)        */
    fp2_Sub  ( p, &u, &P->y, &R->y );    /* R->y = y3                   */

  }

  fp2_Clear(p, &u);
  fp2_Clear(p, &v);
  fp2_Clear(p, &lambda);
  fp2_Clear(p, &tx);

  return status;

}

// j-inv = 1728 * 4A^3 / (4A^3 + 27B^2)
void weier_j_inv(const weier_curve_t* curve, fp2* j_inv) {
  const ff_Params* p = curve->ffData;
  fp2 tmp1 = { 0 }, tmp2 = { 0 }, tmp3 = { 0 }, tmp4 = { 0 };
  fp2_Init(p, &tmp1);
  fp2_Init(p, &tmp2);
  fp2_Init(p, &tmp3);
  fp2_Init(p, &tmp4);

  fp2_Square(p, &curve->a, &tmp1); // a^2
  fp2_Multiply(p, &curve->a, &tmp1, &tmp1); // a^3
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // 2a^3
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // tmp1 = 4a^3

  fp2_Square(p, &curve->b, &tmp2); // tmp2 = b^2
  fp2_Add(p, &tmp2, &tmp2, &tmp4); // tmp4 = 2b^2
  fp2_Add(p, &tmp4, &tmp4, &tmp3); // tmp3 = 4b^2
  fp2_Add(p, &tmp2, &tmp4, &tmp2); // tmp2 = 3b^2
  fp2_Add(p, &tmp3, &tmp3, &tmp3); // tmp3 = 8b^2
  fp2_Add(p, &tmp3, &tmp3, &tmp4); // tmp4 = 16b^2
  fp2_Add(p, &tmp4, &tmp3, &tmp4); // tmp4 = 16b^2 + 8b^2 = 24b^2
  fp2_Add(p, &tmp4, &tmp2, &tmp2); // tmp2 = 24b^2 + 3b^2 = 27b^2

  fp2_Add(p, &tmp1, &tmp2, &tmp2); // tmp2 = 4a^3 + 27b^2
  fp2_Invert(p, &tmp2, &tmp2); // tmp2 = 1 / (4a^3 + 27b^2)
  fp2_Multiply(p, &tmp1, &tmp2, &tmp1); // tmp1 = 4a^3 / (4a^3 + 27b^2)

  // 1278 * tmp1 = (1024 + 512 + 128 + 64) * tmp1
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // 2
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // 4
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // 8
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // 16
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // 32
  fp2_Add(p, &tmp1, &tmp1, &tmp1); // 64 -> keep
  fp2_Add(p, &tmp1, &tmp1, &tmp2); // 128 -> keep
  fp2_Add(p, &tmp2, &tmp2, &tmp3); // 256
  fp2_Add(p, &tmp3, &tmp3, &tmp3); // 512 -> keep
  fp2_Add(p, &tmp3, &tmp3, &tmp4); // 1024
  fp2_Add(p, &tmp4, &tmp3, &tmp4); // 1024 + 512
  fp2_Add(p, &tmp4, &tmp2, &tmp4); // 1024 + 512 + 128
  fp2_Add(p, &tmp4, &tmp1, j_inv); // 1024 + 512 + 128 + 64

  fp2_Clear(p, &tmp1);
  fp2_Clear(p, &tmp2);
  fp2_Clear(p, &tmp3);
  fp2_Clear(p, &tmp4);
}
