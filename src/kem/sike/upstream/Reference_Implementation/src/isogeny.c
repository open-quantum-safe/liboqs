//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <isogeny.h>

void eval_3_iso(const ff_Params *p,
                const mont_pt_t *P3,
                const mont_pt_t *P,
                mont_pt_t *isoP) {

  const fp2* x3 = &P3->x;
  const fp2* x = &P->x;
  const fp2* y = &P->y;

  // xx:=x*(x*x3-1)^2/(x-x3)^2;
  // yy:=y*(x*x3-1)*(x^2*x3-3*x*x3^2+x+x3)/(x-x3)^3;

  fp2 t1 = { 0 }, t2 = { 0 }, t3 = { 0 }, t4 = { 0 }, one = { 0 };
  fp2_Init(p, &t1);
  fp2_Init(p, &t2);
  fp2_Init(p, &t3);
  fp2_Init(p, &t4);
  fp2_Init_set(p, &one, 1, 0);

  fp2_Square(p, x, &t1);             // t1 = x^2
  fp2_Multiply(p, &t1, x3, &t1);     // t1 = x^2*x3
  fp2_Square(p, x3, &t2);            // t2 = x3^2
  fp2_Multiply(p, x, &t2, &t2);      // t2 = x*x3^2
  fp2_Add(p, &t2, &t2, &t3);         // t3 = 2*x*x3^2
  fp2_Add(p, &t2, &t3, &t2);         // t2 = 3*x*x3^2
  fp2_Sub(p, &t1, &t2, &t1);         // t1 = x^2*x3-3*x*x3^2
  fp2_Add(p, &t1, x, &t1);           // t1 = x^2*x3-3*x*x3^2+x
  fp2_Add(p, &t1, x3, &t1);          // t1 = x^2*x3-3*x*x3^2+x+x3

  fp2_Sub(p, x, x3, &t2);            // t2 = x-x3
  fp2_Invert(p, &t2, &t2);           // t2 = 1/(x-x3)
  fp2_Square(p, &t2, &t3);           // t3 = 1/(x-x3)^2
  fp2_Multiply(p, &t2, &t3, &t2);    // t2 = 1/(x-x3)^3

  fp2_Multiply(p, x, x3, &t4);       // t4 = x*x3
  fp2_Sub(p, &t4, &one, &t4);        // t4 = x*x3-1

  fp2_Multiply(p, &t4, &t1, &t1);    // t1 = (x*x3-1)*(x^2*x3-3*x*x3^2+x+x3)
  fp2_Multiply(p, &t1, &t2, &t1);    // t1 = (x*x3-1)*(x^2*x3-3*x*x3^2+x+x3)/(x-x3)^3

  fp2_Square(p, &t4, &t2);           // t2 = (x*x3-1)^2
  fp2_Multiply(p, &t2, &t3, &t2);    // t2 = (x*x3-1)^2/(x-x3)^2

  fp2_Multiply(p, x, &t2, &isoP->x); // xx = x*(x*x3-1)^2/(x-x3)^2
  fp2_Multiply(p, y, &t1, &isoP->y); // yy = y*(x*x3-1)*(x^2*x3-3*x*x3^2+x+x3)/(x-x3)^3

  fp2_Clear(p, &t1);
  fp2_Clear(p, &t2);
  fp2_Clear(p, &t3);
  fp2_Clear(p, &t4);
  fp2_Clear(p, &one);
}

void curve_3_iso(const ff_Params *p, const mont_pt_t *P3, const mont_curve_int_t *E, mont_curve_int_t *isoE) {

  // aa:=(a*x3-6*x3^2+6)*x3;
  // bb:=b*x3^2;

  const fp2* x3 = &P3->x;
  const fp2* a = &E->a;
  const fp2* b = &E->b;
  fp2* aa = &isoE->a;
  fp2* bb = &isoE->b;

  fp2 t1 = { 0 }, t2 = { 0 };
  fp2_Init(p, &t1);
  fp2_Init(p, &t2);

  fp2_Square(p, x3, &t1);         // t1 = x3^2
  fp2_Multiply(p, b, &t1, bb);    // bb = b*x3^2

  fp2_Add(p, &t1, &t1, &t1);      // t1 = 2*x3^2
  fp2_Add(p, &t1, &t1, &t2);      // t2 = 4*x3^2
  fp2_Add(p, &t1, &t2, &t1);      // t1 = 6*x3^2
  fp2_Set(p, &t2, 6, 0);          // t2 = 6
  fp2_Sub(p, &t1, &t2, &t1);      // t1 = 6*x3^2-6
  fp2_Multiply(p, a, x3, &t2);    // t2 = a*x3
  fp2_Sub(p, &t2, &t1, &t1);      // t1 = a*x3-6*x3^2+6
  fp2_Multiply(p, &t1, x3, aa);   // aa = (a*x3-6*x3^2+6)*x3

  fp2_Clear(p, &t1);
  fp2_Clear(p, &t2);
}

void eval_4_iso(const ff_Params *p,
                const mont_pt_t *P4,
                const mont_pt_t *P,
                mont_pt_t *isoP) {

  const fp2* x4 = &P4->x;
  const fp2* x = &P->x;
  const fp2* y = &P->y;
  fp2* xx = &isoP->x;
  fp2* yy = &isoP->y;

  fp2 t1 = { 0 }, t2 = { 0 }, t3 = { 0 }, t4 = { 0 }, t5 = { 0 }, one = { 0 };
  fp2_Init(p, &t1);
  fp2_Init(p, &t2);
  fp2_Init(p, &t3);
  fp2_Init(p, &t4);
  fp2_Init(p, &t5);
  fp2_Init_set(p, &one, 1, 0);

  // xx:=-(x*x4^2+x-2*x4)*x*(x*x4-1)^2/((x-x4)^2*(2*x*x4-x4^2-1));
  // yy:=-2*y*x4^2*(x*x4-1)*(x^4*x4^2-4*x^3*x4^3+2*x^2*x4^4+x^4-4*x^3*x4+10*x^2*x4^2-4*x*x4^3-4*x*x4+x4^2+1)/((x-x4)^3*(2*x*x4-x4^2-1)^2);

  fp2_Square(p, x, &t1);             // t1 = x^2
  fp2_Square(p, &t1, &t2);           // t2 = x^4
  fp2_Square(p, x4, &t3);            // t3 = x4^2
  fp2_Multiply(p, &t2, &t3, &t4);    // t4 = x^4*x4^2
  fp2_Add(p, &t2, &t4, &t2);         // t2 = x^4+x^4*x4^2
  fp2_Multiply(p, &t1, &t3, &t4);    // t4 = x^2*x4^2
  fp2_Add(p, &t4, &t4, &t4);         // t4 = 2*x^2*x4^2
  fp2_Add(p, &t4, &t4, &t5);         // t5 = 4*x^2*x4^2
  fp2_Add(p, &t5, &t5, &t5);         // t5 = 8*x^2*x4^2
  fp2_Add(p, &t4, &t5, &t4);         // t4 = 10*x^2*x4^2
  fp2_Add(p, &t2, &t4, &t2);         // t2 = x^4+x^4*x4^2+10*x^2*x4^2
  fp2_Multiply(p, &t3, &t3, &t4);    // t4 = x4^4
  fp2_Multiply(p, &t1, &t4, &t5);    // t5 = x^2*x4^4
  fp2_Add(p, &t5, &t5, &t5);         // t5 = 2*x^2*x4^4
  fp2_Add(p, &t2, &t5, &t2);         // t2 = x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4
  fp2_Multiply(p, &t1, x, &t1);      // t1 = x^3
  fp2_Multiply(p, x4, &t3, &t4);     // t4 = x4^3
  fp2_Multiply(p, &t1, &t4, &t5);    // t5 = x^3*x4^3
  fp2_Add(p, &t5, &t5, &t5);         // t5 = 2*x^3*x4^3
  fp2_Add(p, &t5, &t5, &t5);         // t5 = 4*x^3*x4^3
  fp2_Sub(p, &t2, &t5, &t2);         // t2 = x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3
  fp2_Multiply(p, &t1, x4, &t1);     // t1 = x^3*x4
  fp2_Add(p, &t1, &t1, &t1);         // t1 = 2*x^3*x4
  fp2_Add(p, &t1, &t1, &t1);         // t1 = 4*x^3*x4
  fp2_Sub(p, &t2, &t1, &t1);         // t1 = x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4
  fp2_Multiply(p, x, &t4, &t2);      // t2 = x*x4^3
  fp2_Add(p, &t2, &t2, &t2);         // t2 = 2*x*x4^3
  fp2_Add(p, &t2, &t2, &t2);         // t2 = 4*x*x4^3
  fp2_Sub(p, &t1, &t2, &t1);         // t1 = x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3
  fp2_Add(p, &t1, &t3, &t1);         // t1 = x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2
  fp2_Add(p, &t1, &one, &t1);        // t1 = x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1
  fp2_Multiply(p, x, x4, &t2);       // t2 = x*x4
  fp2_Sub(p, &t2, &one, &t4);        // t4 = x*x4-1
  fp2_Add(p, &t2, &t2, &t2);         // t2 = 2*x*x4
  fp2_Add(p, &t2, &t2, &t5);         // t5 = 4*x*x4
  fp2_Sub(p, &t1, &t5, &t1);         // t1 = x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1-4*x*x4

  fp2_Multiply(p, &t4, &t1, &t1);    // t1 = (x*x4-1)*(x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1-4*x*x4)
  fp2_Multiply(p, &t3, &t1, &t1);    // t1 = x4^2*(x*x4-1)*(x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1-4*x*x4)
  fp2_Multiply(p, y, &t1, &t1);      // t1 = x4^2*(x*x4-1)*(x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1-4*x*x4)
  fp2_Add(p, &t1, &t1, &t1);         // t1 = 2*x4^2*(x*x4-1)*(x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1-4*x*x4)
  fp2_Negative(p, &t1, yy);          // yy = -2*x4^2*(x*x4-1)*(x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1-4*x*x4)

  fp2_Sub(p, &t2, &t3, &t2);         // t2 = 2*x*x4-x4^2
  fp2_Sub(p, &t2, &one, &t1);        // t1 = 2*x*x4-x4^2-1
  fp2_Sub(p, x, x4, &t2);            // t2 = x-x4
  fp2_Multiply(p, &t2, &t1, &t1);    // t1 = (x-x4)*(2*x*x4-x4^2-1)
  fp2_Square(p, &t1, &t5);           // t5 = (x-x4)^2*(2*x*x4-x4^2-1)^2
  fp2_Multiply(p, &t5, &t2, &t5);    // t5 = (x-x4)^3*(2*x*x4-x4^2-1)^2
  fp2_Invert(p, &t5, &t5);           // t5 = 1/((x-x4)^3*(2*x*x4-x4^2-1)^2)
  fp2_Multiply(p, yy, &t5, yy);      // yy = -2*x4^2*(x*x4-1)*(x^4+x^4*x4^2+10*x^2*x4^2+2*x^2*x4^4-4*x^3*x4^3-4*x^3*x4-4*x*x4^3+x4^2+1-4*x*x4)/((x-x4)^3*(2*x*x4-x4^2-1)^2)

  fp2_Multiply(p, &t1, &t2, &t1);    // t1 = (x-x4)^2*(2*x*x4-x4^2-1)
  fp2_Invert(p, &t1, &t1);           // t1 = 1/((x-x4)^2*(2*x*x4-x4^2-1))
  fp2_Square(p, &t4, &t4);           // t4 = (x*x4-1)^2
  fp2_Multiply(p, &t1, &t4, &t1);    // t1 = (x*x4-1)^2/((x-x4)^2*(2*x*x4-x4^2-1))
  fp2_Multiply(p, x, &t1, &t1);      // t1 = x*(x*x4-1)^2/((x-x4)^2*(2*x*x4-x4^2-1))
  fp2_Multiply(p, x, &t3, &t2);      // t2 = x*x4^2
  fp2_Add(p, &t2, x, &t2);           // t2 = x*x4^2+x
  fp2_Add(p, x4, x4, &t3);           // t3 = 2*x4
  fp2_Sub(p, &t2, &t3, &t2);         // t2 = x*x4^2+x-2*x4
  fp2_Negative(p, &t2, &t2);         // t2 = -(x*x4^2+x-2*x4)
  fp2_Multiply(p, &t1, &t2, xx);     // xx = -(x*x4^2+x-2*x4)*x*(x*x4-1)^2/((x-x4)^2*(2*x*x4-x4^2-1))

  fp2_Clear(p, &t1);
  fp2_Clear(p, &t2);
  fp2_Clear(p, &t3);
  fp2_Clear(p, &t4);
  fp2_Clear(p, &t5);
  fp2_Clear(p, &one);

}

void curve_4_iso(const ff_Params *p, const mont_pt_t *P4, const mont_curve_int_t *E, mont_curve_int_t *isoE) {

  const fp2 *x4 = &P4->x;
  //const fp2 *a = &E->coeff.a;
  const fp2 *b = &E->b;
  fp2 *aa = &isoE->a;
  fp2 *bb = &isoE->b;

  fp2 t1 = {0}, t2 = {0};
  fp2_Init(p, &t1);
  fp2_Init(p, &t2);

  // aa:=4*x4^4-2;
  // bb:=-(1/2)*x4*(x4^2+1)*b = -(1/2)*(x4^3+x4)*b;

  fp2_Square(p, x4, &t1);         // t1 = x4^2
  fp2_Square(p, &t1, aa);         // aa = x4^4
  fp2_Add(p, aa, aa, aa);         // aa = 2*x4^4
  fp2_Add(p, aa, aa, aa);         // aa = 4*x4^4
  fp2_Set(p, &t2, 2, 0);          // t2 = 2
  fp2_Sub(p, aa, &t2, aa);        // aa = 4*x4^4-2

  fp2_Multiply(p, x4, &t1, &t1);  // t1 = x4^3
  fp2_Add(p, &t1, x4, &t1);       // t1 = x4^3+x4
  fp2_Multiply(p, &t1, b, &t1);   // t1 = (x4^3+x4)*b
  fp2_Invert(p, &t2, &t2);        // t2 = 1/2 -> precompute
  fp2_Negative(p, &t2, &t2);      // t2 = -(1/2)
  fp2_Multiply(p, &t2, &t1, bb);

  fp2_Clear(p, &t1);
  fp2_Clear(p, &t2);
}

void iso_2_e(const ff_Params *p,
             int e,
             const mont_curve_int_t *E, mont_pt_t *S,
             const mont_pt_t *P1, const mont_pt_t *P2,
             mont_curve_int_t *isoE,
             mont_pt_t *isoP1,    mont_pt_t *isoP2) {

  int p1Eval = P1 != NULL, p2Eval = P2 != NULL;

  mont_pt_t T = { 0 };
  mont_pt_init(p, &T);
  mont_curve_copy(p, E, isoE);

  if (p1Eval)
    mont_pt_copy(p, P1, isoP1);

  if (p2Eval)
    mont_pt_copy(p, P2, isoP2);

  for (int i = e - 2; i >= 0; i -= 2) {
    xDBLe(E, S, i, &T);
    curve_4_iso(p, &T, isoE, isoE);

    eval_4_iso(p, &T, S, S);

    if (p1Eval)
      eval_4_iso(p, &T, isoP1, isoP1);

    if (p2Eval)
      eval_4_iso(p, &T, isoP2, isoP2);
  }

  mont_pt_clear(p, &T);
}

void iso_3_e(const ff_Params *p,
             int e,
             const mont_curve_int_t *E, mont_pt_t *S,
             const mont_pt_t *P1, const mont_pt_t *P2,
             mont_curve_int_t *isoE,
             mont_pt_t *isoP1,    mont_pt_t *isoP2) {

  int p1Eval = P1 != NULL, p2Eval = P2 != NULL;

  mont_pt_t T = { 0 };
  mont_pt_init(p, &T);
  mont_curve_copy(p, E, isoE);

  if (p1Eval)
    mont_pt_copy(p, P1, isoP1);

  if (p2Eval)
    mont_pt_copy(p, P2, isoP2);

  for (int i = e - 1; i >= 0; --i) {
    xTPLe(E, S, i, &T);
    curve_3_iso(p, &T, isoE, isoE);

    eval_3_iso(p, &T, S, S);

    if (p1Eval)
      eval_3_iso(p, &T, isoP1, isoP1);

    if (p2Eval)
      eval_3_iso(p, &T, isoP2, isoP2);
  }

  mont_pt_clear(p, &T);
}
