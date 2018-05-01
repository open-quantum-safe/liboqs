//
// SIKE-Weierstrass
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//
// Based on https://github.com/Microsoft/PQCrypto-SIDH
//

#include <sike_params.h>
#include <stdio.h>
#include "test_arith.h"

#define TEST_LOOPS 1

static int test_fp(const sike_params_t *params) {
  int rc = 0, n;

  const ff_Params *p = params->EA.ffData;

  mp a, b, c, d, e, f;

  fp_Init(p, a);
  fp_Init(p, b);
  fp_Init(p, c);
  fp_Init(p, d);
  fp_Init(p, e);
  fp_Init(p, f);


  printf(
    "\n--------------------------------------------------------------------------------------------------------\n\n");
  printf("Testing field arithmetic over GF(p): \n\n");

  // Field addition over the prime p751
  rc = 0;
  for (n = 0; n < TEST_LOOPS; n++) {
    fp_Rand(p, a);
    fp_Rand(p, b);
    fp_Rand(p, c);
    fp_Rand(p, d);
    fp_Rand(p, e);
    fp_Rand(p, f);

    fp_Add(p, a, b, d);
    fp_Add(p, d, c, e);                 // e = (a+b)+c
    fp_Add(p, b, c, d);
    fp_Add(p, d, a, f);                 // f = a+(b+c)
    if (!fp_IsEqual(p, e, f)) {
      rc = 1;
      break;
    }

    fp_Add(p, a, b, d);                                     // d = a+b
    fp_Add(p, b, a, e);                                     // e = b+a
    if (!fp_IsEqual(p, d, e)) {
      rc = 1;
      break;
    }

    fp_Zero(p, b);
    fp_Add(p, a, b, d);                                     // d = a+0
    if (!fp_IsEqual(p, a, d)) {
      rc = 1;
      break;
    }

    fp_Zero(p, b);
    fp_Copy(p, a, d);
    fp_Negative(p, d, d);
    fp_Add(p, a, d, e);                                     // e = a+(-a)
    if (!fp_IsEqual(p, e, b)) {
      rc = 1;
      break;
    }
  }
  if (!rc) printf("  GF(p) addition tests ............................................ PASSED");
  else {
    printf("  GF(p) addition tests... FAILED");
    printf("\n");
    goto end;
  }
  printf("\n");

  // Field subtraction over the prime p751
  rc = 0;
  for (n = 0; n < TEST_LOOPS; n++) {
    fp_Rand(p, a);
    fp_Rand(p, b);
    fp_Rand(p, c);
    fp_Rand(p, d);
    fp_Rand(p, e);
    fp_Rand(p, f);

    fp_Subtract(p, a, b, d);
    fp_Subtract(p, d, c, e);                 // e = (a-b)-c
    fp_Add(p, b, c, d);
    fp_Subtract(p, a, d, f);                 // f = a-(b+c)
    if (!fp_IsEqual(p, e, f)) {
      rc = 1;
      break;
    }

    fp_Subtract(p, a, b, d);                                     // d = a-b
    fp_Subtract(p, b, a, e);
    fp_Negative(p, e, e);                                           // e = -(b-a)
    if (!fp_IsEqual(p, d, e)) {
      rc = 1;
      break;
    }

    fp_Zero(p, b);
    fp_Subtract(p, a, b, d);                                     // d = a-0
    if (!fp_IsEqual(p, a, d)) {
      rc = 1;
      break;
    }

    fp_Zero(p, b);
    fp_Copy(p, a, d);
    fp_Subtract(p, a, d, e);                                     // e = a+(-a)
    if (!fp_IsEqual(p, e, b)) {
      rc = 1;
      break;
    }
  }
  if (!rc) printf("  GF(p) subtraction tests ......................................... PASSED");
  else {
    printf("  GF(p) subtraction tests... FAILED");
    printf("\n");
    goto end;
  }
  printf("\n");

  // Field multiplication over the prime p751
  rc = 0;
  for (n = 0; n < TEST_LOOPS; n++) {
    fp_Rand(p, a);
    fp_Rand(p, b);
    fp_Rand(p, c);

    fp_Multiply(p, a, b, d);
    fp_Multiply(p, d, c, e);                          // e = (a*b)*c
    fp_Multiply(p, b, c, d);
    fp_Multiply(p, d, a, f);                          // f = a*(b*c)
    if (!fp_IsEqual(p, e, f)) {
      rc = 1;
      break;
    }

    fp_Add(p, b, c, d);
    fp_Multiply(p, a, d, e);                               // e = a*(b+c)
    fp_Multiply(p, a, b, d);
    fp_Multiply(p, a, c, f);
    fp_Add(p, d, f, f);    // f = a*b+a*c
    if (!fp_IsEqual(p, e, f)) {
      rc = 1;
      break;
    }

    fp_Multiply(p, a, b, d);                                                      // d = a*b
    fp_Multiply(p, b, a, e);                                                      // e = b*a
    if (!fp_IsEqual(p, d, e)) {
      rc = 1;
      break;
    }

    fp_Constant(p, 1, b);
    fp_Multiply(p, a, b, d);                                                      // d = a*1
    if (!fp_IsEqual(p, a, d)) {
      rc = 1;
      break;
    }

    fp_Zero(p, b);
    fp_Multiply(p, a, b, d);                                                      // d = a*0
    if (!fp_IsEqual(p, b, d)) {
      rc = 1;
      break;
    }
  }
  if (!rc) printf("  GF(p) multiplication tests ...................................... PASSED");
  else {
    printf("  GF(p) multiplication tests... FAILED");
    printf("\n");
    goto end;
  }
  printf("\n");

  // Field squaring over the prime p751
  rc = 0;
  for (n = 0; n < TEST_LOOPS; n++) {
    fp_Rand(p, a);

    fp_Square(p, a, b);                                 // b = a^2
    fp_Multiply(p, a, a, c);                             // c = a*a
    if (!fp_IsEqual(p, b, c)) {
      rc = 1;
      break;
    }

    fp_Zero(p, a);
    fp_Square(p, a, d);                                 // d = 0^2
    if (!fp_IsEqual(p, a, d)) {
      rc = 1;
      break;
    }
  }
  if (!rc) printf("  GF(p) squaring tests............................................. PASSED");
  else {
    printf("  GF(p) squaring tests... FAILED");
    printf("\n");
    goto end;
  }
  printf("\n");

  // Field inversion over the prime p751
  rc = 0;
  for (n = 0; n < TEST_LOOPS; n++) {
    fp_Rand(p, a);
    fp_Constant(p, 1, d);
    fp_Copy(p, a, b);
    fp_Invert(p, a, a);
    fp_Multiply(p, a, b, c);                             // c = a*a^-1
    if (!fp_IsEqual(p, c, d)) {
      rc = 1;
      break;
    }

    fp_Rand(p, a);
    fp_Copy(p, a, b);
    fp_Invert(p, b, b);                                     // a = a^-1 by exponentiation
    fp_Invert(p, b, b);                                     // a = a^-1 by exponentiation
    if (!fp_IsEqual(p, a, b)) {
      rc = 1;
      break;
    }
  }
  if (!rc) printf("  GF(p) inversion tests............................................ PASSED");
  else {
    printf("  GF(p) inversion tests... FAILED");
    printf("\n");
    goto end;
  }
  printf("\n");


  end:
  fp_Clear(p, a);
  fp_Clear(p, b);
  fp_Clear(p, c);
  fp_Clear(p, d);
  fp_Clear(p, e);
  fp_Clear(p, f);

  return rc;
}

static int test_fp2(const sike_params_t* params)
{ // Tests for the quadratic extension field arithmetic
  int rc = 0;

  const ff_Params *p = params->EA.ffData;

  int n;
  fp2 a, b, c, d, e, f, g, h, i, j;

  fp2_Init(p, &a);
  fp2_Init(p, &b);
  fp2_Init(p, &c);
  fp2_Init(p, &d);
  fp2_Init(p, &e);
  fp2_Init(p, &f);
  fp2_Init(p, &g);
  fp2_Init(p, &h);
  fp2_Init(p, &i);
  fp2_Init(p, &j);

  printf("\n--------------------------------------------------------------------------------------------------------\n\n");
  printf("Testing quadratic extension arithmetic over GF(p751^2): \n\n");

  // Addition over GF
  rc = 0;
  for (n=0; n<TEST_LOOPS; n++)
  {
    fp2_Rand(p, &a); fp2_Rand(p, &b); fp2_Rand(p, &c); fp2_Rand(p, &d); fp2_Rand(p, &e); fp2_Rand(p, &f);

    fp2_Add(p, &a, &b, &d); fp2_Add(p, &d, &c, &e);                 // e = (a+b)+c
    fp2_Add(p, &b, &c, &d); fp2_Add(p, &d, &a, &f);                 // f = a+(b+c)
    if (!fp2_IsEqual(p, &e,&f)) { rc = 1; break; }

    fp2_Add(p, &a, &b, &d);                                     // d = a+b
    fp2_Add(p, &b, &a, &e);                                     // e = b+a
    if (!fp2_IsEqual(p, &d,&e)) { rc = 1; break; }

    fp2_Set(p, &b, 0, 0);
    fp2_Add(p, &a, &b, &d);                                     // d = a+0
    if (!fp2_IsEqual(p, &a,&d)) { rc = 1; break; }

    fp2_Set(p, &b, 0, 0);
    fp2_Copy(p, &a, &d);
    fp2_Negative(p, &d, &d);
    fp2_Add(p, &a, &d, &e);                                     // e = a+(-a)
    if (!fp2_IsEqual(p, &e,&b)) { rc = 1; break; }
  }
  if (!rc) printf("  GF(p^2) addition tests .......................................... PASSED");
  else {
    printf("  GF(p^2) addition tests... FAILED"); printf("\n");
    goto end;
  }
  printf("\n");

  // Subtraction over GF
  rc = 0;
  for (n=0; n<TEST_LOOPS; n++)
  {
    fp2_Rand(p, &a); fp2_Rand(p, &b); fp2_Rand(p, &c); fp2_Rand(p, &d); fp2_Rand(p, &e); fp2_Rand(p, &f);

    fp2_Sub(p, &a, &b, &d); fp2_Sub(p, &d, &c, &e);                 // e = (a-b)-c
    fp2_Add(p,&b, &c, &d); fp2_Sub(p, &a, &d, &f);                 // f = a-(b+c)
    if (!fp2_IsEqual(p, &e,&f)) { rc = 1; break; }

    fp2_Sub(p, &a, &b, &d);                                     // d = a-b
    fp2_Sub(p, &b, &a, &e);
    fp2_Negative(p, &e, &e);                                           // e = -(b-a)
    if (!fp2_IsEqual(p, &d, &e)) { rc = 1; break; }

    fp2_Set(p, &b, 0, 0);
    fp2_Sub(p, &a, &b, &d);                                     // d = a-0
    if (!fp2_IsEqual(p, &a, &d)) { rc = 1; break; }

    fp2_Set(p, &b, 0, 0);
    fp2_Copy(p, &a, &d);
    fp2_Sub(p, &a, &d, &e);                                     // e = a+(-a)
    if (!fp2_IsEqual(p, &e, &b)) { rc = 1; break; }
  }
  if (!rc) printf("  GF(p^2) subtraction tests ....................................... PASSED");
  else { printf("  GF(p^2) subtraction tests... FAILED"); printf("\n"); goto end; }
  printf("\n");

  // Multiplication over GF
  rc = 0;
  for (n=0; n<TEST_LOOPS; n++)
  {
    fp2_Rand(p, &a); fp2_Rand(p, &b); fp2_Rand(p, &c);

    fp2_Multiply(p, &a, &b, &d); fp2_Multiply(p, &d, &c, &e);                          // e = (a*b)*c
    fp2_Multiply(p, &b, &c, &d); fp2_Multiply(p, &d, &a, &f);                          // f = a*(b*c)
    if (!fp2_IsEqual(p, &e, &f)) { rc = 1; break; }

    fp2_Add(p, &b, &c, &d); fp2_Multiply(p, &a, &d, &e);                               // e = a*(b+c)
    fp2_Multiply(p, &a, &b, &d); fp2_Multiply(p, &a, &c, &f); fp2_Add(p, &d, &f, &f);   // f = a*b+a*c
    if (!fp2_IsEqual(p, &e, &f)) { rc = 1; break; }

    fp2_Multiply(p, &a, &b, &d);                                                      // d = a*b
    fp2_Multiply(p, &b, &a, &e);                                                      // e = b*a
    if (!fp2_IsEqual(p, &d, &e)) { rc = 1; break; }

    fp2_Set(p, &b, 1, 0);
    fp2_Multiply(p, &a, &b, &d);                                                      // d = a*1
    if (!fp2_IsEqual(p, &a, &d)) { rc = 1; break; }

    fp2_Set(p, &b, 0, 0);
    fp2_Multiply(p, &a, &b, &d);                                                      // d = a*0
    if (!fp2_IsEqual(p, &b, &d)) { rc = 1; break; }
  }
  if (!rc) printf("  GF(p^2) multiplication tests .................................... PASSED");
  else { printf("  GF(p^2) multiplication tests... FAILED"); printf("\n"); goto end; }
  printf("\n");

  // Squaring over GF
  rc = 0;
  for (n=0; n<TEST_LOOPS; n++)
  {
    fp2_Rand(p, &a);

    fp2_Square(p, &a, &b);                                 // b = a^2
    fp2_Multiply(p, &a, &a, &c);                             // c = a*a
    if (!fp2_IsEqual(p, &b, &c)) { rc = 1; break; }

    fp2_Set(p, &a, 0, 0);
    fp2_Square(p, &a, &d);                                 // d = 0^2
    if (!fp2_IsEqual(p, &a, &d)) { rc = 1; break; }
  }
  if (!rc) printf("  GF(p^2) squaring tests........................................... PASSED");
  else { printf("  GF(p^2) squaring tests... FAILED"); printf("\n"); goto end; }
  printf("\n");

  // Inversion over GF
  rc = 0;
  for (n=0; n<TEST_LOOPS; n++)
  {
    fp2_Rand(p, &a);

    fp2_Set(p, &d, 1, 0);
    fp2_Copy(p, &a, &b);
    fp2_Invert(p, &a, &a);
    fp2_Multiply(p, &a, &b, &c);                             // c = a*a^-1
    if (!fp2_IsEqual(p, &c, &d)) { rc = 1; break; }

    fp2_Rand(p, &a);

    fp2_Copy(p, &a, &b);
    fp2_Invert(p, &a, &a);                                 // a = a^-1 with exponentiation
    fp2_Invert(p, &b, &b);                                 // a = a^-1 with exponentiation
    if (!fp2_IsEqual(p, &a, &b)) { rc = 1; break; }
  }
  if (!rc) printf("  GF(p^2) inversion tests.......................................... PASSED");
  else { printf("  GF(p^2) inversion tests... FAILED"); printf("\n"); goto end; }
  printf("\n");

  // Sqrt over GF
  rc = 0;
  for (n = 0; n < 100; ++n)
  {
    fp2_Rand(p, &a);
    fp2_Square(p, &a, &b);
    fp2_Sqrt(p, &b, &c, 0);
    fp2_Sqrt(p, &b, &d, 1);

    if (!fp2_IsEqual(p, &a, &c) && !fp2_IsEqual(p, &a, &d)) { rc = 1; break; }
  }
  if (!rc) printf("  GF(p^2) sqrt tests............................................... PASSED");
  else { printf("  GF(p^2) sqrt tests... FAILED"); printf("\n"); goto end; }
  printf("\n");

  end:
  fp2_Clear(p, &a);
  fp2_Clear(p, &b);
  fp2_Clear(p, &c);
  fp2_Clear(p, &d);
  fp2_Clear(p, &e);
  fp2_Clear(p, &f);

  return rc;
}

int test_arith(const sike_params_t *params) {

  int rc = 0;
  rc = test_fp(params);
  if ( rc ) goto end;

  rc = test_fp2(params);
  if ( rc ) goto end;

  end:
  return rc;
}