#include "lowmc.h"
#include "lowmc_pars.h"
#include "mzd_additional.h"


static void sbox_layer_bitsliced(mzd_local_t* out, mzd_local_t* in, mask_t const* mask) {
  mzd_and(out, in, mask->mask);

  mzd_local_t* buffer[6] = {NULL};
  mzd_local_init_multiple_ex(buffer, 6, 1, in->ncols, false);

  // a
  mzd_local_t* x0m = mzd_and(buffer[0], mask->x0, in);
  // b
  mzd_local_t* x1m = mzd_and(buffer[1], mask->x1, in);
  // c
  mzd_local_t* x2m = mzd_and(buffer[2], mask->x2, in);

  mzd_shift_left(x0m, x0m, 2);
  mzd_shift_left(x1m, x1m, 1);

  // b & c
  mzd_local_t* t0 = mzd_and(buffer[3], x1m, x2m);
  // c & a
  mzd_local_t* t1 = mzd_and(buffer[4], x0m, x2m);
  // a & b
  mzd_local_t* t2 = mzd_and(buffer[5], x0m, x1m);

  // (b & c) ^ a
  mzd_xor(t0, t0, x0m);

  // (c & a) ^ a ^ b
  mzd_xor(t1, t1, x0m);
  mzd_xor(t1, t1, x1m);

  // (a & b) ^ a ^ b ^c
  mzd_xor(t2, t2, x0m);
  mzd_xor(t2, t2, x1m);
  mzd_xor(t2, t2, x2m);

  mzd_shift_right(t0, t0, 2);
  mzd_shift_right(t1, t1, 1);

  mzd_xor(out, out, t2);
  mzd_xor(out, out, t0);
  mzd_xor(out, out, t1);

  mzd_local_free_multiple(buffer);
}


mzd_local_t* lowmc_call(lowmc_t const* lowmc, lowmc_key_t const* lowmc_key, mzd_local_t const* p) {
  if (p->ncols > lowmc->n) {
    return NULL;
  }
  if (p->nrows != 1) {
    return NULL;
  }

  mzd_local_t* x = mzd_local_init_ex(1, lowmc->n, false);
  mzd_local_t* y = mzd_local_init_ex(1, lowmc->n, false);

  mzd_local_copy(x, p);
#if defined(MUL_M4RI)
  mzd_addmul_vl(x, lowmc_key, lowmc->k0_lookup);
#else
  mzd_addmul_v(x, lowmc_key, lowmc->k0_matrix);
#endif

  lowmc_round_t const* round = lowmc->rounds;
  for (unsigned i = 0; i < lowmc->r; ++i, ++round) {
    {
      sbox_layer_bitsliced(y, x, &lowmc->mask);
    }
#if defined(MUL_M4RI)
    mzd_mul_vl(x, y, round->l_lookup);
#else
    mzd_mul_v(x, y, round->l_matrix);
#endif
    mzd_xor(x, x, round->constant);
    mzd_addmul_v(x, lowmc_key, round->k_matrix);
  }

  mzd_local_free(y);

  return x;
}
