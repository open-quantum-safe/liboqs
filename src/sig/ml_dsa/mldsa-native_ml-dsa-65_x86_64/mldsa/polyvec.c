/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#include <stdint.h>
#include <string.h>

#include "common.h"
#include "debug.h"
#include "poly.h"
#include "polyvec.h"

#if !defined(MLD_USE_NATIVE_NTT_CUSTOM_ORDER)
/* This namespacing is not done at the top to avoid a naming conflict
 * with native backends, which are currently not yet namespaced. */
#define mld_poly_permute_bitrev_to_custom \
  MLD_NAMESPACE(mld_poly_permute_bitrev_to_custom)

static MLD_INLINE void mld_poly_permute_bitrev_to_custom(int32_t data[MLDSA_N])
{
  ((void)data);
}
#endif /* !MLD_USE_NATIVE_NTT_CUSTOM_ORDER */


MLD_INTERNAL_API
void mld_polyvec_matrix_expand(mld_polyvecl mat[MLDSA_K],
                               const uint8_t rho[MLDSA_SEEDBYTES])
{
  unsigned int i, j;
  /*
   * We generate four separate seed arrays rather than a single one to work
   * around limitations in CBMC function contracts dealing with disjoint slices
   * of the same parent object.
   */

  MLD_ALIGN uint8_t seed_ext[4][MLD_ALIGN_UP(MLDSA_SEEDBYTES + 2)];

  for (j = 0; j < 4; j++)
  __loop__(
    assigns(j, object_whole(seed_ext))
    invariant(j <= 4)
  )
  {
    mld_memcpy(seed_ext[j], rho, MLDSA_SEEDBYTES);
  }
  /* Sample 4 matrix entries a time. */
  for (i = 0; i < (MLDSA_K * MLDSA_L / 4) * 4; i += 4)
  __loop__(
    assigns(i, j, object_whole(seed_ext), memory_slice(mat, MLDSA_K * sizeof(mld_polyvecl)))
    invariant(i <= (MLDSA_K * MLDSA_L / 4) * 4 && i % 4 == 0)
    /* vectors 0 .. i / MLDSA_L are completely sampled */
    invariant(forall(k1, 0, i / MLDSA_L, forall(l1, 0, MLDSA_L,
      array_bound(mat[k1].vec[l1].coeffs, 0, MLDSA_N, 0, MLDSA_Q))))
    /* last vector is sampled up to i % MLDSA_L */
    invariant(forall(k2, i / MLDSA_L, i / MLDSA_L + 1, forall(l2, 0, i % MLDSA_L,
      array_bound(mat[k2].vec[l2].coeffs, 0, MLDSA_N, 0, MLDSA_Q))))
  )
  {
    for (j = 0; j < 4; j++)
    __loop__(
      assigns(j, object_whole(seed_ext))
      invariant(j <= 4)
    )
    {
      uint8_t x = (i + j) / MLDSA_L;
      uint8_t y = (i + j) % MLDSA_L;

      seed_ext[j][MLDSA_SEEDBYTES + 0] = y;
      seed_ext[j][MLDSA_SEEDBYTES + 1] = x;
    }

    mld_poly_uniform_4x(&mat[i / MLDSA_L].vec[i % MLDSA_L],
                        &mat[(i + 1) / MLDSA_L].vec[(i + 1) % MLDSA_L],
                        &mat[(i + 2) / MLDSA_L].vec[(i + 2) % MLDSA_L],
                        &mat[(i + 3) / MLDSA_L].vec[(i + 3) % MLDSA_L],
                        seed_ext);
  }

  /* For MLDSA_K=6, MLDSA_L=5, process the last two entries individually */
  while (i < MLDSA_K * MLDSA_L)
  __loop__(
    assigns(i, object_whole(seed_ext), memory_slice(mat, MLDSA_K * sizeof(mld_polyvecl)))
    invariant(i <= MLDSA_K * MLDSA_L)
    /* vectors 0 .. i / MLDSA_L are completely sampled */
    invariant(forall(k1, 0, i / MLDSA_L, forall(l1, 0, MLDSA_L,
      array_bound(mat[k1].vec[l1].coeffs, 0, MLDSA_N, 0, MLDSA_Q))))
    /* last vector is sampled up to i % MLDSA_L */
    invariant(forall(k2, i / MLDSA_L, i / MLDSA_L + 1, forall(l2, 0, i % MLDSA_L,
      array_bound(mat[k2].vec[l2].coeffs, 0, MLDSA_N, 0, MLDSA_Q))))
  )
  {
    uint8_t x = i / MLDSA_L;
    uint8_t y = i % MLDSA_L;
    mld_poly *this_poly = &mat[i / MLDSA_L].vec[i % MLDSA_L];

    seed_ext[0][MLDSA_SEEDBYTES + 0] = y;
    seed_ext[0][MLDSA_SEEDBYTES + 1] = x;

    mld_poly_uniform(this_poly, seed_ext[0]);
    i++;
  }

  /*
   * The public matrix is generated in NTT domain. If the native backend
   * uses a custom order in NTT domain, permute A accordingly.
   */
  for (i = 0; i < MLDSA_K; i++)
  {
    for (j = 0; j < MLDSA_L; j++)
    {
      mld_poly_permute_bitrev_to_custom(mat[i].vec[j].coeffs);
    }
  }

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(seed_ext, sizeof(seed_ext));
}

MLD_INTERNAL_API
void mld_polyvec_matrix_pointwise_montgomery(mld_polyveck *t,
                                             const mld_polyvecl mat[MLDSA_K],
                                             const mld_polyvecl *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(v->vec, MLDSA_L, MLDSA_N, MLD_NTT_BOUND);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(t, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k0, 0, i,
                     array_abs_bound(t->vec[k0].coeffs, 0, MLDSA_N, MLDSA_Q)))
  )
  {
    mld_polyvecl_pointwise_acc_montgomery(&t->vec[i], &mat[i], v);
  }

  mld_assert_abs_bound_2d(t->vec, MLDSA_K, MLDSA_N, MLDSA_Q);
}

/**************************************************************/
/************ Vectors of polynomials of length MLDSA_L **************/
/**************************************************************/
MLD_INTERNAL_API
void mld_polyvecl_uniform_gamma1(mld_polyvecl *v,
                                 const uint8_t seed[MLDSA_CRHBYTES],
                                 uint16_t nonce)
{
  nonce = MLDSA_L * nonce;
#if MLDSA_L == 4
  mld_poly_uniform_gamma1_4x(&v->vec[0], &v->vec[1], &v->vec[2], &v->vec[3],
                             seed, nonce, nonce + 1, nonce + 2, nonce + 3);
#elif MLDSA_L == 5
  mld_poly_uniform_gamma1_4x(&v->vec[0], &v->vec[1], &v->vec[2], &v->vec[3],
                             seed, nonce, nonce + 1, nonce + 2, nonce + 3);
  mld_poly_uniform_gamma1(&v->vec[4], seed, nonce + 4);
#elif MLDSA_L == 7
  mld_poly_uniform_gamma1_4x(&v->vec[0], &v->vec[1], &v->vec[2],
                             &v->vec[3 /* irrelevant */], seed, nonce,
                             nonce + 1, nonce + 2, 0xFF /* irrelevant */);
  mld_poly_uniform_gamma1_4x(&v->vec[3], &v->vec[4], &v->vec[5], &v->vec[6],
                             seed, nonce + 3, nonce + 4, nonce + 5, nonce + 6);
#endif /* MLDSA_L == 7 */

  mld_assert_bound_2d(v->vec, MLDSA_L, MLDSA_N, -(MLDSA_GAMMA1 - 1),
                      MLDSA_GAMMA1 + 1);
}

MLD_INTERNAL_API
void mld_polyvecl_reduce(mld_polyvecl *v)
{
  unsigned int i;
  mld_assert_bound_2d(v->vec, MLDSA_L, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX);

  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyvecl)))
    invariant(i <= MLDSA_L)
    invariant(forall(k0, i, MLDSA_L, forall(k1, 0, MLDSA_N, v->vec[k0].coeffs[k1] == loop_entry(*v).vec[k0].coeffs[k1])))
    invariant(forall(k2, 0, i,
      array_bound(v->vec[k2].coeffs, 0, MLDSA_N, -REDUCE32_RANGE_MAX, REDUCE32_RANGE_MAX))))
  {
    mld_poly_reduce(&v->vec[i]);
  }

  mld_assert_bound_2d(v->vec, MLDSA_L, MLDSA_N, -REDUCE32_RANGE_MAX,
                      REDUCE32_RANGE_MAX);
}

/* Reference: We use destructive version (output=first input) to avoid
 *            reasoning about aliasing in the CBMC specification */
MLD_INTERNAL_API
void mld_polyvecl_add(mld_polyvecl *u, const mld_polyvecl *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    assigns(i, memory_slice(u, sizeof(mld_polyvecl)))
    invariant(i <= MLDSA_L)
    invariant(forall(k0, i, MLDSA_L,
              forall(k1, 0, MLDSA_N, u->vec[k0].coeffs[k1] == loop_entry(*u).vec[k0].coeffs[k1])))
    invariant(forall(k4, 0, i, forall(k5, 0, MLDSA_N, u->vec[k4].coeffs[k5] == loop_entry(*u).vec[k4].coeffs[k5] + v->vec[k4].coeffs[k5])))
    invariant(forall(k6, 0, i, array_bound(u->vec[k6].coeffs, 0, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX)))
  )
  {
    mld_poly_add(&u->vec[i], &v->vec[i]);
  }
  mld_assert_bound_2d(u->vec, MLDSA_L, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX);
}

MLD_INTERNAL_API
void mld_polyvecl_ntt(mld_polyvecl *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(v->vec, MLDSA_L, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyvecl)))
    invariant(i <= MLDSA_L)
    invariant(forall(k0, i, MLDSA_L, forall(k1, 0, MLDSA_N, v->vec[k0].coeffs[k1] == loop_entry(*v).vec[k0].coeffs[k1])))
    invariant(forall(k1, 0, i, array_abs_bound(v->vec[k1].coeffs, 0, MLDSA_N, MLD_NTT_BOUND))))
  {
    mld_poly_ntt(&v->vec[i]);
  }

  mld_assert_abs_bound_2d(v->vec, MLDSA_L, MLDSA_N, MLD_NTT_BOUND);
}

MLD_INTERNAL_API
void mld_polyvecl_invntt_tomont(mld_polyvecl *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(v->vec, MLDSA_L, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyvecl)))
    invariant(i <= MLDSA_L)
    invariant(forall(k0, i, MLDSA_L, forall(k1, 0, MLDSA_N, v->vec[k0].coeffs[k1] == loop_entry(*v).vec[k0].coeffs[k1])))
    invariant(forall(k1, 0, i, array_abs_bound(v->vec[k1].coeffs, 0, MLDSA_N, MLD_INTT_BOUND))))
  {
    mld_poly_invntt_tomont(&v->vec[i]);
  }

  mld_assert_abs_bound_2d(v->vec, MLDSA_L, MLDSA_N, MLD_INTT_BOUND);
}

MLD_INTERNAL_API
void mld_polyvecl_pointwise_poly_montgomery(mld_polyvecl *r, const mld_poly *a,
                                            const mld_polyvecl *v)
{
  unsigned int i;
  mld_assert_abs_bound(a->coeffs, MLDSA_N, MLD_NTT_BOUND);
  mld_assert_abs_bound_2d(v->vec, MLDSA_L, MLDSA_N, MLD_NTT_BOUND);

  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    assigns(i, memory_slice(r, sizeof(mld_polyvecl)))
    invariant(i <= MLDSA_L)
    invariant(forall(k2, 0, i, array_abs_bound(r->vec[k2].coeffs, 0, MLDSA_N, MLDSA_Q)))
  )
  {
    mld_poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
  }

  mld_assert_abs_bound_2d(r->vec, MLDSA_L, MLDSA_N, MLDSA_Q);
}

MLD_INTERNAL_API
void mld_polyvecl_pointwise_acc_montgomery(mld_poly *w, const mld_polyvecl *u,
                                           const mld_polyvecl *v)
{
  unsigned int i, j;
  mld_assert_bound_2d(u->vec, MLDSA_L, MLDSA_N, 0, MLDSA_Q);
  mld_assert_abs_bound_2d(v->vec, MLDSA_L, MLDSA_N, MLD_NTT_BOUND);
  /* The first input is bounded by [0, Q-1] inclusive
   * The second input is bounded by [-9Q+1, 9Q-1] inclusive . Hence, we can
   * safely accumulate in 64-bits without intermediate reductions as
   * MLDSA_L * (MLD_NTT_BOUND-1) * (Q-1) < INT64_MAX
   *
   * The worst case is ML-DSA-87: 7 * (9Q-1) * (Q-1) < 2**52
   * (and likewise for negative values)
   */

  for (i = 0; i < MLDSA_N; i++)
  __loop__(
    assigns(i, j, object_whole(w))
    invariant(i <= MLDSA_N)
    invariant(array_abs_bound(w->coeffs, 0, i, MLDSA_Q))
  )
  {
    int64_t t = 0;
    int32_t r;
    for (j = 0; j < MLDSA_L; j++)
    __loop__(
      assigns(j, t)
      invariant(j <= MLDSA_L)
      invariant(t >= -(int64_t)j*(MLDSA_Q - 1)*(MLD_NTT_BOUND - 1))
      invariant(t <= (int64_t)j*(MLDSA_Q - 1)*(MLD_NTT_BOUND - 1))
    )
    {
      t += (int64_t)u->vec[j].coeffs[i] * v->vec[j].coeffs[i];
    }

    r = mld_montgomery_reduce(t);
    w->coeffs[i] = r;
  }

  mld_assert_abs_bound(w->coeffs, MLDSA_N, MLDSA_Q);
}

MLD_INTERNAL_API
uint32_t mld_polyvecl_chknorm(const mld_polyvecl *v, int32_t bound)
{
  unsigned int i;
  uint32_t t = 0;
  mld_assert_bound_2d(v->vec, MLDSA_L, MLDSA_N, -REDUCE32_RANGE_MAX,
                      REDUCE32_RANGE_MAX);

  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    invariant(i <= MLDSA_L)
    invariant(t == 0 || t == 0xFFFFFFFF)
    invariant((t == 0) == forall(k1, 0, i, array_abs_bound(v->vec[k1].coeffs, 0, MLDSA_N, bound)))
  )
  {
    /* Reference: Leaks which polynomial violates the bound via a conditional.
     * We are more conservative to reduce the number of declassifications in
     * constant-time testing.
     */
    t |= mld_poly_chknorm(&v->vec[i], bound);
  }
  return t;
}

/**************************************************************/
/************ Vectors of polynomials of length MLDSA_K **************/
/**************************************************************/
MLD_INTERNAL_API
void mld_polyveck_reduce(mld_polyveck *v)
{
  unsigned int i;
  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k0, i, MLDSA_K, forall(k1, 0, MLDSA_N, v->vec[k0].coeffs[k1] == loop_entry(*v).vec[k0].coeffs[k1])))
    invariant(forall(k2, 0, i,
      array_bound(v->vec[k2].coeffs, 0, MLDSA_N, -REDUCE32_RANGE_MAX, REDUCE32_RANGE_MAX)))
  )
  {
    mld_poly_reduce(&v->vec[i]);
  }

  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, -REDUCE32_RANGE_MAX,
                      REDUCE32_RANGE_MAX);
}

MLD_INTERNAL_API
void mld_polyveck_caddq(mld_polyveck *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(v->vec, MLDSA_K, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k0, i, MLDSA_K, forall(k1, 0, MLDSA_N, v->vec[k0].coeffs[k1] == loop_entry(*v).vec[k0].coeffs[k1])))
    invariant(forall(k1, 0, i, array_bound(v->vec[k1].coeffs, 0, MLDSA_N, 0, MLDSA_Q))))
  {
    mld_poly_caddq(&v->vec[i]);
  }

  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, 0, MLDSA_Q);
}

/* Reference: We use destructive version (output=first input) to avoid
 *            reasoning about aliasing in the CBMC specification */
MLD_INTERNAL_API
void mld_polyveck_add(mld_polyveck *u, const mld_polyveck *v)
{
  unsigned int i;

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(u, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k0, i, MLDSA_K,
              forall(k1, 0, MLDSA_N, u->vec[k0].coeffs[k1] == loop_entry(*u).vec[k0].coeffs[k1])))
    invariant(forall(k4, 0, i, forall(k5, 0, MLDSA_N, u->vec[k4].coeffs[k5] == loop_entry(*u).vec[k4].coeffs[k5] + v->vec[k4].coeffs[k5])))
    invariant(forall(k6, 0, i, array_bound(u->vec[k6].coeffs, 0, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX)))
  )
  {
    mld_poly_add(&u->vec[i], &v->vec[i]);
  }
  mld_assert_bound_2d(u->vec, MLDSA_L, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX);
}

MLD_INTERNAL_API
void mld_polyveck_sub(mld_polyveck *u, const mld_polyveck *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(u->vec, MLDSA_K, MLDSA_N, MLDSA_Q);
  mld_assert_abs_bound_2d(v->vec, MLDSA_K, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(u, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k0, 0, i,
                     array_bound(u->vec[k0].coeffs, 0, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX)))
    invariant(forall(k1, i, MLDSA_K,
             forall(n1, 0, MLDSA_N, u->vec[k1].coeffs[n1] == loop_entry(*u).vec[k1].coeffs[n1]))))
  {
    mld_poly_sub(&u->vec[i], &v->vec[i]);
  }

  mld_assert_bound_2d(u->vec, MLDSA_K, MLDSA_N, INT32_MIN, REDUCE32_DOMAIN_MAX);
}

MLD_INTERNAL_API
void mld_polyveck_shiftl(mld_polyveck *v)
{
  unsigned int i;
  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, 0, 1 << 10);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k1, 0, i, array_bound(v->vec[k1].coeffs, 0, MLDSA_N, 0, MLDSA_Q)))
    invariant(forall(k1, i, MLDSA_K,
             forall(n1, 0, MLDSA_N, v->vec[k1].coeffs[n1] == loop_entry(*v).vec[k1].coeffs[n1])))
  )
  {
    mld_poly_shiftl(&v->vec[i]);
  }

  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, 0, MLDSA_Q);
}

MLD_INTERNAL_API
void mld_polyveck_ntt(mld_polyveck *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(v->vec, MLDSA_K, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k0, i, MLDSA_K, forall(k1, 0, MLDSA_N, v->vec[k0].coeffs[k1] == loop_entry(*v).vec[k0].coeffs[k1])))
    invariant(forall(k1, 0, i, array_abs_bound(v->vec[k1].coeffs, 0, MLDSA_N, MLD_NTT_BOUND))))
  {
    mld_poly_ntt(&v->vec[i]);
  }
  mld_assert_abs_bound_2d(v->vec, MLDSA_K, MLDSA_N, MLD_NTT_BOUND);
}

MLD_INTERNAL_API
void mld_polyveck_invntt_tomont(mld_polyveck *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(v->vec, MLDSA_K, MLDSA_N, MLDSA_Q);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(v, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k0, i, MLDSA_K, forall(k1, 0, MLDSA_N, v->vec[k0].coeffs[k1] == loop_entry(*v).vec[k0].coeffs[k1])))
    invariant(forall(k1, 0, i, array_abs_bound(v->vec[k1].coeffs, 0, MLDSA_N, MLD_INTT_BOUND))))
  {
    mld_poly_invntt_tomont(&v->vec[i]);
  }

  mld_assert_abs_bound_2d(v->vec, MLDSA_K, MLDSA_N, MLD_INTT_BOUND);
}

MLD_INTERNAL_API
void mld_polyveck_pointwise_poly_montgomery(mld_polyveck *r, const mld_poly *a,
                                            const mld_polyveck *v)
{
  unsigned int i;
  mld_assert_abs_bound_2d(v->vec, MLDSA_K, MLDSA_N, MLD_NTT_BOUND);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(r, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k2, 0, i, array_abs_bound(r->vec[k2].coeffs, 0, MLDSA_N, MLDSA_Q)))
  )
  {
    mld_poly_pointwise_montgomery(&r->vec[i], a, &v->vec[i]);
  }
  mld_assert_abs_bound_2d(r->vec, MLDSA_K, MLDSA_N, MLDSA_Q);
}

MLD_INTERNAL_API
uint32_t mld_polyveck_chknorm(const mld_polyveck *v, int32_t bound)
{
  unsigned int i;
  uint32_t t = 0;
  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, -REDUCE32_RANGE_MAX,
                      REDUCE32_RANGE_MAX);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    invariant(i <= MLDSA_K)
    invariant(t == 0 || t == 0xFFFFFFFF)
    invariant((t == 0) == forall(k1, 0, i, array_abs_bound(v->vec[k1].coeffs, 0, MLDSA_N, bound)))
  )
  {
    /* Reference: Leaks which polynomial violates the bound via a conditional.
     * We are more conservative to reduce the number of declassifications in
     * constant-time testing.
     */
    t |= mld_poly_chknorm(&v->vec[i], bound);
  }

  return t;
}

MLD_INTERNAL_API
void mld_polyveck_power2round(mld_polyveck *v1, mld_polyveck *v0,
                              const mld_polyveck *v)
{
  unsigned int i;
  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, 0, MLDSA_Q);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(v0, sizeof(mld_polyveck)), memory_slice(v1, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k1, 0, i, array_bound(v0->vec[k1].coeffs, 0, MLDSA_N, -(MLD_2_POW_D/2)+1, (MLD_2_POW_D/2)+1)))
    invariant(forall(k2, 0, i, array_bound(v1->vec[k2].coeffs, 0, MLDSA_N, 0, ((MLDSA_Q - 1) / MLD_2_POW_D) + 1)))
  )
  {
    mld_poly_power2round(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }

  mld_assert_bound_2d(v0->vec, MLDSA_K, MLDSA_N, -(MLD_2_POW_D / 2) + 1,
                      (MLD_2_POW_D / 2) + 1);
  mld_assert_bound_2d(v1->vec, MLDSA_K, MLDSA_N, 0,
                      ((MLDSA_Q - 1) / MLD_2_POW_D) + 1);
}

MLD_INTERNAL_API
void mld_polyveck_decompose(mld_polyveck *v1, mld_polyveck *v0,
                            const mld_polyveck *v)
{
  unsigned int i;
  mld_assert_bound_2d(v->vec, MLDSA_K, MLDSA_N, 0, MLDSA_Q);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(v0, sizeof(mld_polyveck)), memory_slice(v1, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k1, 0, i,
                     array_bound(v1->vec[k1].coeffs, 0, MLDSA_N, 0, (MLDSA_Q-1)/(2*MLDSA_GAMMA2))))
    invariant(forall(k2, 0, i,
                     array_abs_bound(v0->vec[k2].coeffs, 0, MLDSA_N, MLDSA_GAMMA2+1)))
  )
  {
    mld_poly_decompose(&v1->vec[i], &v0->vec[i], &v->vec[i]);
  }

  mld_assert_bound_2d(v1->vec, MLDSA_K, MLDSA_N, 0,
                      (MLDSA_Q - 1) / (2 * MLDSA_GAMMA2));
  mld_assert_abs_bound_2d(v0->vec, MLDSA_K, MLDSA_N, MLDSA_GAMMA2 + 1);
}

MLD_INTERNAL_API
unsigned int mld_polyveck_make_hint(mld_polyveck *h, const mld_polyveck *v0,
                                    const mld_polyveck *v1)
{
  unsigned int i, s = 0;

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, s, object_whole(h))
    invariant(i <= MLDSA_K)
    invariant(s <= i * MLDSA_N)
    invariant(forall(k1, 0, i, array_bound(h->vec[k1].coeffs, 0, MLDSA_N, 0, 2)))
  )
  {
    s += mld_poly_make_hint(&h->vec[i], &v0->vec[i], &v1->vec[i]);
  }

  mld_assert_bound_2d(h->vec, MLDSA_K, MLDSA_N, 0, 2);
  return s;
}

MLD_INTERNAL_API
void mld_polyveck_use_hint(mld_polyveck *w, const mld_polyveck *u,
                           const mld_polyveck *h)
{
  unsigned int i;
  mld_assert_bound_2d(u->vec, MLDSA_K, MLDSA_N, 0, MLDSA_Q);
  mld_assert_bound_2d(h->vec, MLDSA_K, MLDSA_N, 0, 2);

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, memory_slice(w, sizeof(mld_polyveck)))
    invariant(i <= MLDSA_K)
    invariant(forall(k2, 0, i,
                     array_bound(w->vec[k2].coeffs, 0, MLDSA_N, 0,
                                 (MLDSA_Q - 1) / (2 * MLDSA_GAMMA2))))
  )
  {
    mld_poly_use_hint(&w->vec[i], &u->vec[i], &h->vec[i]);
  }

  mld_assert_bound_2d(w->vec, MLDSA_K, MLDSA_N, 0,
                      (MLDSA_Q - 1) / (2 * MLDSA_GAMMA2));
}

MLD_INTERNAL_API
void mld_polyveck_pack_w1(uint8_t r[MLDSA_K * MLDSA_POLYW1_PACKEDBYTES],
                          const mld_polyveck *w1)
{
  unsigned int i;
  mld_assert_bound_2d(w1->vec, MLDSA_K, MLDSA_N, 0,
                      (MLDSA_Q - 1) / (2 * MLDSA_GAMMA2));

  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, object_whole(r))
    invariant(i <= MLDSA_K)
  )
  {
    mld_polyw1_pack(&r[i * MLDSA_POLYW1_PACKEDBYTES], &w1->vec[i]);
  }
}

MLD_INTERNAL_API
void mld_polyveck_pack_eta(uint8_t r[MLDSA_K * MLDSA_POLYETA_PACKEDBYTES],
                           const mld_polyveck *p)
{
  unsigned int i;
  mld_assert_abs_bound_2d(p->vec, MLDSA_K, MLDSA_N, MLDSA_ETA + 1);
  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, object_whole(r))
    invariant(i <= MLDSA_K)
  )
  {
    mld_polyeta_pack(&r[i * MLDSA_POLYETA_PACKEDBYTES], &p->vec[i]);
  }
}

MLD_INTERNAL_API
void mld_polyvecl_pack_eta(uint8_t r[MLDSA_L * MLDSA_POLYETA_PACKEDBYTES],
                           const mld_polyvecl *p)
{
  unsigned int i;
  mld_assert_abs_bound_2d(p->vec, MLDSA_L, MLDSA_N, MLDSA_ETA + 1);
  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    assigns(i, object_whole(r))
    invariant(i <= MLDSA_L)
  )
  {
    mld_polyeta_pack(&r[i * MLDSA_POLYETA_PACKEDBYTES], &p->vec[i]);
  }
}

MLD_INTERNAL_API
void mld_polyvecl_pack_z(uint8_t r[MLDSA_L * MLDSA_POLYZ_PACKEDBYTES],
                         const mld_polyvecl *p)
{
  unsigned int i;
  mld_assert_bound_2d(p->vec, MLDSA_L, MLDSA_N, -(MLDSA_GAMMA1 - 1),
                      MLDSA_GAMMA1 + 1);
  for (i = 0; i < MLDSA_L; ++i)
  __loop__(
    assigns(i, object_whole(r))
    invariant(i <= MLDSA_L)
  )
  {
    mld_polyz_pack(&r[i * MLDSA_POLYZ_PACKEDBYTES], &p->vec[i]);
  }
}

MLD_INTERNAL_API
void mld_polyveck_pack_t0(uint8_t r[MLDSA_K * MLDSA_POLYT0_PACKEDBYTES],
                          const mld_polyveck *p)
{
  unsigned int i;
  mld_assert_bound_2d(p->vec, MLDSA_K, MLDSA_N, -(1 << (MLDSA_D - 1)) + 1,
                      (1 << (MLDSA_D - 1)) + 1);
  for (i = 0; i < MLDSA_K; ++i)
  __loop__(
    assigns(i, object_whole(r))
    invariant(i <= MLDSA_K)
  )
  {
    mld_polyt0_pack(&r[i * MLDSA_POLYT0_PACKEDBYTES], &p->vec[i]);
  }
}

MLD_INTERNAL_API
void mld_polyvecl_unpack_eta(
    mld_polyvecl *p, const uint8_t r[MLDSA_L * MLDSA_POLYETA_PACKEDBYTES])
{
  unsigned int i;
  for (i = 0; i < MLDSA_L; ++i)
  {
    mld_polyeta_unpack(&p->vec[i], r + i * MLDSA_POLYETA_PACKEDBYTES);
  }

  mld_assert_bound_2d(p->vec, MLDSA_L, MLDSA_N, MLD_POLYETA_UNPACK_LOWER_BOUND,
                      MLDSA_ETA + 1);
}

MLD_INTERNAL_API
void mld_polyvecl_unpack_z(mld_polyvecl *z,
                           const uint8_t r[MLDSA_L * MLDSA_POLYZ_PACKEDBYTES])
{
  unsigned int i;
  for (i = 0; i < MLDSA_L; ++i)
  {
    mld_polyz_unpack(&z->vec[i], r + i * MLDSA_POLYZ_PACKEDBYTES);
  }

  mld_assert_bound_2d(z->vec, MLDSA_L, MLDSA_N, -(MLDSA_GAMMA1 - 1),
                      MLDSA_GAMMA1 + 1);
}

MLD_INTERNAL_API
void mld_polyveck_unpack_eta(
    mld_polyveck *p, const uint8_t r[MLDSA_K * MLDSA_POLYETA_PACKEDBYTES])
{
  unsigned int i;
  for (i = 0; i < MLDSA_K; ++i)
  {
    mld_polyeta_unpack(&p->vec[i], r + i * MLDSA_POLYETA_PACKEDBYTES);
  }

  mld_assert_bound_2d(p->vec, MLDSA_K, MLDSA_N, MLD_POLYETA_UNPACK_LOWER_BOUND,
                      MLDSA_ETA + 1);
}

MLD_INTERNAL_API
void mld_polyveck_unpack_t0(mld_polyveck *p,
                            const uint8_t r[MLDSA_K * MLDSA_POLYT0_PACKEDBYTES])
{
  unsigned int i;
  for (i = 0; i < MLDSA_K; ++i)
  {
    mld_polyt0_unpack(&p->vec[i], r + i * MLDSA_POLYT0_PACKEDBYTES);
  }

  mld_assert_bound_2d(p->vec, MLDSA_K, MLDSA_N, -(1 << (MLDSA_D - 1)) + 1,
                      (1 << (MLDSA_D - 1)) + 1);
}
