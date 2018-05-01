//
// SIKE-Weierstrass
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <weierstrass.h>
#include <isogeny.h>

static void
weier_map_velu(const weier_curve_t* curve,
               const weier_pt_t *ker,
               const weier_pt_t *P,
               const fp2 *tK,
               const fp2 *uK,
               const weier_pt_t *gK,
                     weier_pt_t *mapP) {
  const ff_Params* p = curve->ffData;


  fp2 tmp1 = { 0 }, tmp2 = { 0 }, tmp3 = { 0 }, tmp4 = { 0 };
  fp2_Init(p, &tmp1);
  fp2_Init(p, &tmp2);
  fp2_Init(p, &tmp3);
  fp2_Init(p, &tmp4);


  if ( fp2_IsEqual(p, &ker->x, &P->x) && fp2_IsEqual(p, &ker->y, &P->y)) {
    weier_set_inf_affine(curve, mapP);
  } else {
    // xPi = P[0] + tK*(P[0]-K[0])^(-1) + uK*(P[0]-K[0])^(-2)
    // yPi = P[1] - uK*2*P[1]*(P[0]-K[0])^(-3) - (tK*(P[1]-K[1]) - gxK*gyK) * (P[0]-K[0])^(-2)

    fp2_Sub(p, &P->x, &ker->x, &tmp1);      // tmp1 = P[0]-K[0]
    fp2_Invert(p, &tmp1, &tmp2);            // tmp2 = (P[0]-K[0])^-1
    fp2_Square(p, &tmp2, &tmp3);            // tmp3 = (P[0]-K[0])^-2
    fp2_Multiply(p, &tmp2, &tmp3, &tmp4);   // tmp4 = (P[0]-K[0])^-3

    fp2_Multiply(p, tK, &tmp2, &tmp1);      // tmp1 = tk*tmp2 = tk*(P[0]-K[0])^-1
    fp2_Multiply(p, uK, &tmp3, &tmp2);      // tmp2 = uK * tmp3 = uk*(P[0]-K[0])^-2
    fp2_Add(p, &P->x, &tmp1, &mapP->x);     // mapP->x = P->x + tmp1 = P->x + tk*(P[0]-K[0])^-1
    fp2_Add(p, &mapP->x, &tmp2, &mapP->x);  // mapP->x = mapP->x + tmp2 = P->x + tk*(P[0]-K[0])^-1 + uk*(P[0]-K[0])^-2

    fp2_Sub(p, &P->y, &ker->y, &tmp2);      // tmp2 = P[1]-K[1]
    fp2_Multiply(p, &P->y, &tmp4, &tmp4);   // tmp4 = P->y * tmp4 = P[1]*(P[0]-K[0])^(-3)
    fp2_Multiply(p, uK, &tmp4, &tmp4);      // tmp4 = uK*P[1]*(P[0]-K[0])^(-3)
    fp2_Add(p, &tmp4, &tmp4, &tmp4);        // tmp4 = 2*uK*P[1]*(P[0]-K[0])^(-3)
    fp2_Sub(p, &P->y, &tmp4, &tmp4);        // tmp4 = P[1] - uK*2*P[1]*(P[0]-K[0])^(-3)
    fp2_Multiply(p, tK, &tmp2, &tmp2);      // tmp2 = tK * tmp2 = tK*(P[1]-K[1])
    fp2_Multiply(p, &gK->x, &gK->y, &tmp1); // tmp1 = gxK*gyK
    fp2_Sub(p, &tmp2, &tmp1, &tmp2);        // tmp2 = tmp2 - tmp1 = (tK*(P[1]-K[1]) - gxK*gyK)
    fp2_Multiply(p, &tmp2, &tmp3, &tmp2);   // tmp2 = tmp2 * tmp3 = (tK*(P[1]-K[1]) - gxK*gyK) * (P[0]-K[0])^-2
    fp2_Sub(p, &tmp4, &tmp2, &mapP->y);     // mapP->y = P[1] - uK*2*P[1]*(P[0]-K[0])^(-3) - (tK*(P[1]-K[1]) - gxK*gyK) * (P[0]-K[0])^-2
  }

  fp2_Clear(p, &tmp1);
  fp2_Clear(p, &tmp2);
  fp2_Clear(p, &tmp3);
  fp2_Clear(p, &tmp4);
}


void
weier_iso_2_3(const weier_curve_t *E,
              const weier_pt_t *ker,
              const weier_pt_t *P,
              const weier_pt_t *Q,
              const weier_pt_t *R,
                    weier_curve_t *EK,
                    weier_pt_t *isoP,
                    weier_pt_t *isoQ,
                    weier_pt_t *isoR ) {
  const ff_Params* p = E->ffData;

  weier_pt_t gK = { 0 };
  fp2 tK = { 0 }, uK = { 0 }, wK = { 0 };
  fp2 tmp = { 0 };

  weier_pt_init(p, &gK);

  fp2_Init(p, &tK);
  fp2_Init(p, &uK);
  fp2_Init(p, &wK);
  fp2_Init(p, &tmp);

  // gxK = 3 * (xK) ^2 + a
  fp2_Square(p, &ker->x, &gK.x);
  fp2_Add(p, &gK.x, &gK.x, &tmp); // 2
  fp2_Add(p, &gK.x, &tmp, &gK.x); // 3
  fp2_Add(p, &gK.x, &E->a, &gK.x);

  // gyK = -2 * yK
  fp2_Add(p, &ker->y, &ker->y, &gK.y);
  fp2_Negative(p, &gK.y, &gK.y);

  //if (yK == 0):
  //  tK = gxK
  //else:
  //  tK = 2 * gxK
  fp2_Copy(p, &gK.x, &tK);
  if (!fp2_IsConst(p, &ker->y, 0, 0)) {
    fp2_Add(p, &tK, &tK, &tK);
  }

  // uK = (gyK) ^2
  fp2_Square(p, &gK.y, &uK);

  // wK = uK + xK*tK
  fp2_Multiply(p, &ker->x, &tK, &wK);
  fp2_Add(p, &uK, &wK, &wK);

  // ai = a - 5*tK
  fp2_Add(p, &tK, &tK, &tmp); // 2tk
  fp2_Add(p, &tmp, &tmp, &tmp); // 4tk
  fp2_Add(p, &tmp, &tK, &tmp); // 5tk
  fp2_Sub(p, &E->a, &tmp, &EK->a);

  // bi = b - 7*wK
  fp2_Add(p, &wK, &wK, &tmp);   // 2wK
  fp2_Add(p, &tmp, &tmp, &tmp); // 4wK
  fp2_Add(p, &tmp, &tmp, &tmp); // 8wK
  fp2_Sub(p, &tmp, &wK, &tmp);  // 7wK
  fp2_Sub(p, &E->b, &tmp, &EK->b);

  EK->ffData = E->ffData;

  if (P) {
    weier_map_velu(EK, ker, P, &tK, &uK, &gK, isoP);
  }
  if (Q) {
    weier_map_velu(EK, ker, Q, &tK, &uK, &gK, isoQ);
  }
  if (R) {
    weier_map_velu(EK, ker, R, &tK, &uK, &gK, isoR);
  }

  weier_pt_clear(p, &gK);

  fp2_Clear(p, &tK);
  fp2_Clear(p, &uK);
  fp2_Clear(p, &wK);
  fp2_Clear(p, &tmp);
}
