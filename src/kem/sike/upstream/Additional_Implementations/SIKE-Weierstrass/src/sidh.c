//
// SIKE-Weierstrass
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <sike_params.h>
#include <sidh.h>
#include <isogeny.h>
#include <encoding.h>
#include <stdlib.h>
#include <random.h>

static void
lift_to_isogeny(const weier_curve_t *E,
                unsigned long l,
                unsigned long e,
                unsigned long msb,
                const weier_pt_t *Po,
                const weier_pt_t *Qo,
                weier_pt_t *R,
                weier_curve_t *Ec) {
  const ff_Params *p = E->ffData;
  weier_pt_t K = {0};

  weier_pt_init(p, &K);

  fp2_Copy(p, &E->a, &Ec->a);
  fp2_Copy(p, &E->b, &Ec->b);

  if (Po && Qo) {
    weier_pt_copy(p, Po, &Ec->P);
    weier_pt_copy(p, Qo, &Ec->Q);
  }
  mp factor;

  p->init(p, factor);

  for (int i = 0; i < e; ++i) {
    // K = (l^(e-1-i))*R
    mpz_ui_pow_ui(factor, l, e - 1 - i);
    weier_double_and_add(Ec, factor, R, &K, msb);

    weier_iso_2_3(Ec, &K, &Ec->P, &Ec->Q, R, Ec, &Ec->P, &Ec->Q, R);
  }

  p->clear(p, factor);
  weier_pt_clear(p, &K);
}

static int
kernel_gen(const weier_curve_t *curve, const weier_pt_t *P, const weier_pt_t *Q, const mp m, int msb, weier_pt_t *R) {
  const ff_Params *p = curve->ffData;

  weier_pt_t tmp = {0};

  weier_pt_init(p, &tmp);
  weier_double_and_add(curve, m, Q, &tmp, msb);

  weier_xADD(curve, P, &tmp, R);

  weier_pt_clear(p, &tmp);
  return 0;
}

/**
 * SIDH private-key generation
 *
 * Private keys are integers in the range:
 * Outputs random value in [0, 2^lA - 1] for ALICE
 * Outputs random value in [0, 2^Floor(Log(2,3^lB)) - 1] for BOB
 *
 * @param params SIKE parameters
 * @param party ALICE or BOB
 * @param sk Private key to be generated
 */
void sidh_sk_keygen(const sike_params_t* params,
                    party_t party,
                    sike_private_key sk) {

  size_t bytes = 0;
  unsigned char* arr = 0;

  if (party == ALICE) {
    // Outputs random value in [0, 2^lA - 1]
    size_t orderLen = mp_sizeinbase(params->ordA, 2);
    bytes = BITS_TO_BYTES_CEIL(orderLen);
    arr = malloc(bytes);
    randombytes(arr, bytes);
    ostoi(arr, bytes, sk);
    mpz_mod(sk, sk, params->ordA);
  } else {
    // Outputs random value in [0, 2^Floor(Log(2,3^lB)) - 1]
    size_t orderLen = mp_sizeinbase(params->ordB, 2);
    bytes = BITS_TO_BYTES_CEIL(orderLen - 1);
    arr = malloc(bytes);
    randombytes(arr, bytes);
    ostoi(arr, bytes, sk);
    mp mod;
    mpz_init_set_ui(mod, 1);
    mpz_mul_2exp(mod, mod, orderLen - 1);
    mpz_mod(sk, sk, mod);
    mpz_clear(mod);
  }

  clear_free(arr, bytes, MEM_FREE);
}

/**
 * Isogen
 * (SIDH public-key generation)
 *
 * For A:
 * Given a private key m_A, a base curve E_0, generators P_A, Q_A and P_B, Q_B:
 * - generates kernel defining an isogeny: K = P_A + m_A*Q_A
 * - gets isogenous curve E_A
 * - evaluates P_B and Q_B under the isogeny: phi_A(P_B), phi_A(Q_B)
 * - Returns public key as E_A with generators phi_A(P_B), phi_A(Q_B)
 * For B:
 * Given a private key m_B, a base curve E_0, generators P_B, Q_B and P_A, Q_A:
 * - generates kernel defining an isogeny: K = P_B + m_B*Q_B
 * - gets isogenous curve E_B
 * - evaluates P_A and Q_A under the isogeny: phi_B(P_A), phi_B(Q_A)
 * - Returns public key as E_B with generators phi_B(P_A), phi_B(Q_A)
 *
 * @param params SIDH parameters
 * @param pk Public key to be generated
 * @param sk Private key, externally provided
 * @param party `ALICE` or `BOB`
 */
void sidh_isogen(const sike_params_t *params,
                 sike_public_key_t *pk,
                 const sike_private_key sk,
                 party_t party) {

  const ff_Params *p = NULL;

  unsigned long e, l, msb;
  const weier_curve_t *E;
  const weier_pt_t *Po, *Qo;
  weier_pt_t R = { 0 };

  if (party == ALICE) {
    p = params->EA.ffData;
    e = params->eA;
    l = params->lA;
    msb = params->msbA;
    E = &params->EA;
    Po = &params->EB.P;
    Qo = &params->EB.Q;
  } else {
    p = params->EB.ffData;
    e = params->eB;
    l = params->lB;
    msb = params->msbB;
    E = &params->EB;
    Po = &params->EA.P;
    Qo = &params->EA.Q;
  }

  weier_pt_init(p, &R);

  // R = P + m*Q
  // IsoGen
  kernel_gen(E, &E->P, &E->Q, sk, msb, &R);

  // Lift Pi, Qi to EC using R as kernel
  // IsoEx
  lift_to_isogeny(E, l, e, msb, Po, Qo, &R, pk);

  weier_pt_clear(p, &R);

}

/**
 * Isoex
 * (SIDH shared secret generation)
 *
 * For A:
 * Given a private key m_A, and B's public key: curve E_B, generators phi_B(P_A), phi_B(Q_A)
 * - generates kernel defining an isogeny: K = phi_B(P_A) + m_A*phi_B(Q_A)
 * - gets isogenous curve E_AB
 * - Shared secret is the j-invariant of E_AB
 * For B:
 * Given a private key m_B, and A's public key: curve E_A, generators phi_A(P_B), phi_A(Q_B)
 * - generates kernel defining an isogeny: K = phi_A(P_B) + m_B*phi_A(Q_B)
 * - gets isogenous curve E_BA
 * - Shared secret is the j-invariant of E_BA
 *
 * @param params SIDH parameters
 * @param pkO Public key of the other party
 * @param skI Own private key
 * @param party `ALICE` or `BOB`
 * @param secret Shared secret to be generated
 */
void sidh_isoex(const sike_params_t *params,
                const sike_public_key_t *pkO,
                const sike_private_key skI,
                party_t party,
                fp2 *secret) {
  ff_Params *p = NULL;

  unsigned long e, l, msb;
  const weier_curve_t *E = pkO;
  weier_curve_t Ec = { 0 };
  if (party == ALICE) {
    p = params->EA.ffData;
    e = params->eA;
    l = params->lA;
    msb = params->msbA;
  } else {
    p = params->EB.ffData;
    e = params->eB;
    l = params->lB;
    msb = params->msbB;
  }

  weier_pt_t R = {0};
  weier_pt_init(p, &R);

  weier_curve_init(p, &Ec);
  Ec.ffData = p;

  // R = P + m*Q
  kernel_gen(E, &E->P, &E->Q, skI, msb, &R);

  lift_to_isogeny(E, l, e, msb, NULL, NULL, &R, &Ec);

  weier_j_inv(&Ec, secret);

  weier_pt_clear(p, &R);
  weier_curve_clear(p, &Ec);
}