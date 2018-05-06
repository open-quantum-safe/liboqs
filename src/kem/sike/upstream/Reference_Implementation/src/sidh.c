//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <sike_params.h>
#include <sidh.h>
#include <isogeny.h>
#include <stdlib.h>
#include <random.h>
#include <encoding.h>

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

  ff_Params *p = NULL;

  unsigned long e, msb = 0;
  const mont_curve_int_t *E;
  const mont_pt_t *Po, *Qo;
  void (*iso_e)(const ff_Params *, int, const mont_curve_int_t *, mont_pt_t *,
                const mont_pt_t *, const mont_pt_t *,
                mont_curve_int_t *, mont_pt_t *, mont_pt_t *);

  if (party == ALICE) {
    p = params->EA.ffData;
    e = params->eA;
    msb = params->msbA;
    E = &params->EA;
    Po = &params->EB.P;
    Qo = &params->EB.Q;
    iso_e = iso_2_e;
  } else {
    p = params->EB.ffData;
    e = params->eB;
    msb = params->msbB;
    E = &params->EB;
    Po = &params->EA.P;
    Qo = &params->EA.Q;
    iso_e = iso_3_e;
  }

  mont_pt_t S = { 0 };
  mont_pt_init(p, &S);

  mont_curve_int_t pkInt = { 0 };
  mont_curve_init(p, &pkInt);


  // Generate kernel
  // S:=P2+SK_2*Q2;
  mont_double_and_add(E, sk, &E->Q, &S, (int) msb);
  xADD(E, &E->P, &S, &S);

  fp2_Set(p, &pkInt.a, 0, 0);
  fp2_Set(p, &pkInt.b, 1, 0);
  mont_pt_copy(p, Po, &pkInt.P);
  mont_pt_copy(p, Qo, &pkInt.Q);

  iso_e(p, (int) e, &pkInt, &S, &pkInt.P, &pkInt.Q, &pkInt, &pkInt.P, &pkInt.Q);

  get_xR(p, &pkInt, pk);

  mont_pt_clear(p, &S);
  mont_curve_clear(p, &pkInt);

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

  unsigned long e, msb;

  mont_curve_int_t E = { 0 };

  void (*iso_e)(const ff_Params *, int, const mont_curve_int_t *, mont_pt_t *,
                const mont_pt_t *, const mont_pt_t *,
                mont_curve_int_t *, mont_pt_t *, mont_pt_t *);

  if (party == ALICE) {
    p = params->EA.ffData;
    e = params->eA;
    msb = params->msbA;
    iso_e = iso_2_e;
  } else {
    p = params->EB.ffData;
    e = params->eB;
    msb = params->msbB;
    iso_e = iso_3_e;
  }

  mont_curve_init(p, &E);
  get_yP_yQ_A_B(p, pkO, &E);

  mont_pt_t S = { 0 };
  mont_pt_init(p, &S);

  // Generate kernel
  //S:=phiP2+SK_2*phiQ2
  mont_double_and_add(&E, skI, &E.Q, &S, (int) msb);
  xADD(&E, &E.P, &S, &S);

  iso_e(p, (int) e, &E, &S, NULL, NULL, &E, NULL, NULL);

  j_inv(p, &E, secret);

  mont_curve_clear(p, &E);
  mont_pt_clear(p, &S);
}
