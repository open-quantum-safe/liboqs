//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <fp2.h>

/** Adds two elements in a quadratic extension field

@see `fp2_Add()`
*/
void
fp2_Add( const ff_Params* p, const fp2* a, const fp2* b, fp2* c )
{
  fp_Add(p, a->x0, b->x0, c->x0);
  fp_Add(p, a->x1, b->x1, c->x1);
}

/**
 * Deinitializes an extension field element
 * @param p
 * @param fp2
 */
void fp2_Clear( const ff_Params* p, fp2* fp2)
{
  fp_Clear(p, fp2->x0);
  fp_Clear(p, fp2->x1);
}

/** Copies an extension field element

@see `fp2_Copy()`
*/
void
fp2_Copy( const ff_Params* p, const fp2* a, fp2* b )
{
  fp_Copy( p, b->x0, a->x0 );
  fp_Copy( p, b->x1, a->x1 );
}

/** Initializes an extension field element

@see `fp2_Init()`
*/
void
fp2_Init( const ff_Params* p, fp2* fp2 )
{
  fp_Init(p, fp2->x0);
  fp_Init(p, fp2->x1);
}

int
fp2_IsEqual( const ff_Params* p, const fp2* a1, const fp2* a2 )
{
  return fp_IsEqual(p, a1->x0, a2->x0) && fp_IsEqual(p, a1->x1, a2->x1);
}

/** Initializes an extension field elements and sets it to integer values

@see `fp2_Init_set()`
*/
void
fp2_Init_set( const ff_Params* p, fp2* fp2, unsigned long x0, unsigned long x1 )
{
  fp2_Init(p, fp2);
  fp_Constant(p, x0, fp2->x0);
  fp_Constant(p, x1, fp2->x1);
}

/** Initializes an extension field elements and sets it to integer values

@see `fp2_Init_set()`
*/
void
fp2_Set( const ff_Params* p, fp2* fp2, unsigned long x0, unsigned long x1 )
{
  fp_Constant(p, x0, fp2->x0);
  fp_Constant(p, x1, fp2->x1);
}

/** Inverts a quadratic extension field element

@see `fp2_Invert()`
*/
void
fp2_Invert( const ff_Params* p, const fp2* a, fp2* b )
{
  mp mul0;
  mp mul1;
  fp_Init(p, mul0);
  fp_Init(p, mul1);

  fp_Square( p, a->x0, mul0 );
  fp_Square( p, a->x1, mul1 );

  fp_Add( p, mul0, mul1, mul0 );
  fp_Invert( p, mul0, mul0 );

  fp_Negative( p, a->x1, mul1 );

  fp_Multiply( p, a->x0, mul0, b->x0 );
  fp_Multiply( p, mul1, mul0, b->x1 );

  fp_Clear(p, mul0);
  fp_Clear(p, mul1);
}

/** Multiplies two elements in a quadratic extension field

@see `fp2_Multiply()`
*/
void
fp2_Multiply( const ff_Params* p,
              const fp2*  a,
              const fp2*  b,
              fp2*  c )
{
  mp mul0;
  mp mul1;
  mp adda;
  mp addb;

  fp_Init(p, mul0);
  fp_Init(p, mul1);
  fp_Init(p, adda);
  fp_Init(p, addb);

  fp_Multiply( p, a->x0, b->x0, mul0 );
  fp_Multiply( p, a->x1, b->x1, mul1 );

  fp_Add( p, a->x0, a->x1, adda );
  fp_Add( p, b->x0, b->x1, addb );

  fp_Subtract( p, mul0, mul1, c->x0 );

  fp_Add( p, mul0, mul1, mul0 );
  fp_Multiply( p, adda, addb, mul1 );

  fp_Subtract( p, mul1, mul0, c->x1 );

  fp_Clear(p, mul0);
  fp_Clear(p, mul1);
  fp_Clear(p, adda);
  fp_Clear(p, addb);
}

/** Negative of a quadratic extension field element

@see `fp2_Negative()`
*/
void
fp2_Negative( const ff_Params* p, const fp2* a, fp2* b )
{
  fp_Negative( p, a->x0, b->x0 );
  fp_Negative( p, a->x1, b->x1 );
}

/** Squares a quadratic extension field element

@see `fp2_Square()`
*/
void
fp2_Square( const ff_Params* p, const fp2* a, fp2* b )
{
  /*
   * This is more efficient
   *
  mp t1, t2, t3;
  fp_Init(p, t1);
  fp_Init(p, t2);
  fp_Init(p, t3);

  fp_Add(p, a->x0, a->x1, t1);
  fp_Subtract(p, a->x0, a->x1, t2);
  fp_Add(p, a->x0, a->x0, t3);
  fp_Multiply(p, t1, t2, b->x0);
  fp_Multiply(p, t3, a->x1, b->x1);

  fp_Clear(p, t1);
  fp_Clear(p, t2);
  fp_Clear(p, t3);
  */

  fp2_Multiply( p, a, a, b );
}

/** Subtracts two elements in a quadratic extension field

@see `fp2_Sub()`
*/
void
fp2_Sub( const ff_Params* p, const fp2* a, const fp2* b, fp2* c )
{
  fp_Subtract( p, a->x0, b->x0, c->x0 );
  fp_Subtract( p, a->x1, b->x1, c->x1 );
}

/** Adds two elements in a quadratic extension field.
    The two elements are expected to have degree 1

@see `fp2_Add()`
*/
void
fp2_AddDeg1( const ff_Params* p, const fp2* a, const fp2* b, fp2* c)
{
  fp_Add( p, a->x0, b->x0, c->x0 );
}

void
fp2_Rand( const ff_Params* p, fp2* a )
{
  fp_Rand(p, a->x0);
  fp_Rand(p, a->x1);
}

int
fp2_IsConst( const ff_Params* p, const fp2* a, unsigned long x0, unsigned long x1 ) {
  return p->isConstant(p, a->x0, x0) && p->isConstant(p, a->x1, x1);
}

/*
int
fp2_Pow( const ff_Params* p, const fp2* a, const mp b, fp2* c )
{
  fp2 tmp = { 0 };
  fp2_Init(p, &tmp);
  fp2_Unity(p, &tmp);

  size_t msb = mp_sizeinbase(b, 2);

  for (int i = (int) msb - 1; i >= 0; --i) {
    fp2_Square(p, &tmp, &tmp);
    if (p->isBitSet(p, b, i)) {
      fp2_Multiply(p, &tmp, a, &tmp);
    }
  }

  fp2_Copy(p, &tmp, c);

  fp2_Clear(p, &tmp);
}
 */


/**
 *
 * SIKE primes satisfy p % 4 == 1
 *
 * Using 3.1 of
 * Efficient compression of SIDH public keys
 * // Craig Costello, David Jao, Patrick Longa, Michael Naehrig, Joost Renes,
 * // David Urbanik, EUROCRYPT 2017.      * Then sqrt(a) = a^((p+1)/4)
 *
 * @param p
 * @param a
 * @param b
 * @return
 */
void
fp2_Sqrt( const ff_Params* p, const fp2* a, fp2* b, int sol)
{
  mp t0, t1, t2, t3, p14, p34, inv2;
  fp_Init(p, t0);
  fp_Init(p, t1);
  fp_Init(p, t2);
  fp_Init(p, t3);
  fp_Init(p, p14);
  fp_Init(p, p34);
  fp_Init(p, inv2);

  fp_Constant(p, 2, inv2);

  // (p + 1) / 4
  mpz_add_ui(p14, p->mod, 1);
  mpz_tdiv_q_2exp(p14, p14, 2);

  // (p - 3) / 4
  mpz_sub_ui(p34, p->mod, 3);
  mpz_tdiv_q_2exp(p34, p34, 2);

  fp_Invert(p, inv2, inv2);
  fp_Square(p, a->x0, t0);
  fp_Square(p, a->x1, t1);
  fp_Add(p, t0, t1, t0);
  fp_Pow(p, t0, p14, t1);
  fp_Add(p, a->x0, t1, t0);
  fp_Multiply(p, t0, inv2, t0);
  //p->half(p, t0);
  fp_Pow(p, t0, p34, t2);

  //fp_Multiply(p, t0, t2, t1);
  fp_Pow(p, t0, p14, t1);

  fp_Multiply(p, t2, a->x1, t2);
  fp_Multiply(p, t2, inv2, t2);
  fp_Square(p, t1, t3);

  if (fp_IsEqual(p, t3, t0)) {
    if (sol == 0) {
      fp_Copy(p, b->x0, t1);
      fp_Copy(p, b->x1, t2);
    } else if (sol == 1) {
      fp_Negative(p, t1, b->x0);
      fp_Negative(p, t2, b->x1);
    }
  } else {
    if (sol == 0) {
      fp_Copy(p, b->x0, t2);
      fp_Negative(p, t1, b->x1);
    } else {
      fp_Negative(p, t2, b->x0);
      fp_Copy(p, b->x1, t1);
    }
  }

  fp_Clear(p, t0);
  fp_Clear(p, t1);
  fp_Clear(p, t2);
  fp_Clear(p, t3);
  fp_Clear(p, p14);
  fp_Clear(p, p34);
  fp_Clear(p, inv2);
}
