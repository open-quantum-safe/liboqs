//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <fp.h>
#include <encoding.h>
#include <rng.h>

static void gmp_add_fp(const ff_Params *p, const mp a, const mp b, mp c) {
  mpz_add(c, a, b);
  mpz_mod(c, c, p->mod);
}

static void gmp_constant_fp(const ff_Params *p, unsigned long a, mp b) {
  mpz_set_ui(b, a);
}

static void gmp_copy_fp(const ff_Params *p, mp dst, const mp src) {
  mpz_set(dst, src);
}

static int gmp_isequal_fp(const ff_Params *p, const mp a, const mp b) {
  return !mpz_cmp(a, b);
}

static void gmp_invert_fp(const ff_Params *p, const mp a, mp b) {
  mpz_invert(b, a, p->mod);
}

static int gmp_isBitSet_fp(const ff_Params *p, const mp a, const unsigned long index) {
  return mpz_tstbit(a, index);
}

static int gmp_isConstant(const ff_Params *p, const mp a, const size_t constant) {
  return !mpz_cmp_ui(a, constant);
}

static void gmp_multiply(const ff_Params *p, const mp a, const mp b, mp c) {
  mpz_mul(c, a, b);
  mpz_mod(c, c, p->mod);
}

static void gmp_negative(const ff_Params *p, const mp a, mp b) {
  mpz_neg(b, a);
  //gmp_printf("neg: %Zx\n", b);
  mpz_mod(b, b, p->mod);
}

static void gmp_pow(const ff_Params *p, const mp a, const mp b, mp c) {
  mpz_powm(c, a, b, p->mod);
}

static int nist_rand(const ff_Params *p, mp a) {
  size_t bytes = mp_sizeinbase(p->mod, 2);
  unsigned char arr[bytes];
  randombytes(arr, bytes);
  mp_import(a, bytes, -1, 1, 1, 0, arr);
  mpz_mod(a, a, p->mod);
  return 0;
}

static void gmp_square(const ff_Params *d, const mp a, mp b) {
  mpz_mul(b, a, a);
  mpz_mod(b, b, d->mod);
}

static void gmp_subtract(const ff_Params *p, const mp a, const mp b, mp c) {
  mpz_sub(c, a, b);
  mpz_mod(c, c, p->mod);
}

static void gmp_unity(const ff_Params *p, mp b) {
  mpz_set_ui(b, 1);
}

static void gmp_zero(const ff_Params *p, mp a) {
  mpz_set_ui(a, 0);
}

static void gmp_init(const ff_Params *p, mp a) {
  mpz_init(a);
}

static void gmp_clear(const ff_Params* p, mp a) {
  mpz_clear(a);
}

void mp_import(mp rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op) {
  mpz_import (rop, count, order, size, endian, nails, op);
}

void mp_export(void *rop, size_t *countp, int order, size_t size, int endian, size_t nails, const mpz_t op) {
  mpz_export(rop, countp, order, size, endian, nails, op);
}

size_t mp_sizeinbase(const mp a, int base) {
  return mpz_sizeinbase(a, base);
}

void mp_pow(const unsigned long a, const unsigned long b, mp c) {
  mpz_ui_pow_ui(c, a, b);
}

void mp_mod(const mp a, const mp mod, mp b) {
  mpz_mod(b, a, mod);
}


void
set_gmp_fp_params(ff_Params *params) {
  params->init =            gmp_init;
  params->add =             gmp_add_fp;
  params->clear =           gmp_clear;
  params->constant =        gmp_constant_fp;
  params->copy =            gmp_copy_fp;
  params->isEqual =         gmp_isequal_fp;
  params->invert =          gmp_invert_fp;
  params->isBitSet =        gmp_isBitSet_fp;
  params->isConstant =      gmp_isConstant;
  params->multiply =        gmp_multiply;
  params->negative =        gmp_negative;
  params->pow =             gmp_pow;
  params->rand =            nist_rand;
  params->square =          gmp_square;
  params->subtract =        gmp_subtract;
  params->unity =           gmp_unity;
  params->zero =            gmp_zero;
};

void fp_Init(const ff_Params* p, mp a) {
  p->init(p, a);
}

void fp_Add(const ff_Params *p, const mp a, const mp b, mp c) {
  p->add(p, a, b, c);
}

void fp_Clear(const ff_Params *p, mp a) {
  p->clear(p, a);
}

void fp_Constant(const ff_Params *p, unsigned long a, mp b) {
  p->constant(p, a, b);
}

void fp_Copy(const ff_Params *p, mp dst, const mp src) {
  p->copy(p, dst, src);
}

int fp_IsEqual(const ff_Params *p, const mp a, const mp b) {
  return p->isEqual(p, a, b);
}

void fp_Invert(const ff_Params *p, const mp a, mp b) {
  p->invert(p, a, b);
}

int fp_IsBitSet(const ff_Params *p, const mp a, const unsigned long index) {
  return p->isBitSet(p, a, index);
}

int fp_IsConstant(const ff_Params *p, const mp a, const size_t constant) {
  return p->isConstant(p, a, constant);
}

void fp_Multiply(const ff_Params *p, const mp a, const mp b, mp c) {
  p->multiply(p, a, b, c);
}

void fp_Negative(const ff_Params *p, const mp a, mp b) {
  p->negative(p, a, b);
}

void fp_Pow(const ff_Params *p, const mp a, const mp b, mp c) {
  p->pow(p, a, b, c);
}

void fp_Rand(const ff_Params *p, mp a) {
  p->rand(p, a);
}

void fp_Square(const ff_Params *p, const mp a, mp b) {
  p->square(p, a, b);
}

void fp_Subtract(const ff_Params *p, const mp a, const mp b, mp c) {
  p->subtract(p, a, b, c);
}

void fp_Unity(const ff_Params *p, mp b) {
  p->unity(p, b);
}

void fp_Zero(const ff_Params *p, mp a) {
  p->zero(p, a);
}

void fp_ImportHex(const char *hexStr, mp a) {
  mpz_set_str(a, hexStr, 0);
}