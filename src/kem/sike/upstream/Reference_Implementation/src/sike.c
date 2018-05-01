//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <sike_params.h>
#include <fips202.h>
#include <encoding.h>
#include <stdlib.h>
#include <string.h>
#include <rng.h>

static const uint16_t sike_G = (uint16_t) 0;
static const uint16_t sike_H = (uint16_t) 1;
static const uint16_t pke_N  = (uint16_t) 2;

/**
 * Function f: cSHAKE with parameter pke_N
 */
static void function_F(const unsigned char* m, size_t mLen, unsigned char* F, size_t fLen) {
  cshake256_simple(F, fLen, pke_N, m, mLen);
}

/**
 * Function g: cSHAKE with parameter sike_G
 */
static void function_G(const unsigned char* m, size_t mLen, unsigned char* G, size_t gLen) {
  cshake256_simple(G, gLen, sike_G, m, mLen);
}

/**
 * Function h: cSHAKE with parameter sike_H
 */
static void function_H(const unsigned char* m, size_t mLen, unsigned char* H, size_t hLen) {
  cshake256_simple(H, hLen, sike_H, m, mLen);
}

/**
 * SIKE PKE encryption
 *
 * For B:
 * - c_0 == PK_3 <- B's keygen function using PK_3, SK_3
 * - j <- Shared secret (j-invariant) using PK_2, SK_3
 * - h <- F(j)
 * - c_1 <- h + m
 * - return (c_0, c_1)
 * -
 * @param params SIKE parameters
 * @param pk3 Public key of the other party
 * @param m message
 * @param sk2 Own private key
 * @param c0 First component of encryption
 * @param c1 Second component of encryption
 */
void sike_pke_enc(const sike_params_t* params,
                  const sike_public_key_t* pk3,
                  const sike_msg* m,
                  const sike_private_key sk2,
                  sike_public_key_t* c0,
                  unsigned char* c1 )
{
  ff_Params* p = params->EB.ffData;

  fp2 j = { 0 };
  unsigned char* jEnc = NULL;
  size_t jEncLen = 0;
  unsigned char h[params->msg_bytes];

  fp2_Init(p, &j);

  // c0 <- isogen_2(sk_2)
  sidh_isogen(params, c0, sk2, ALICE);

  // j <- isoex_2(pk3, sk2)
  sidh_isoex(params, pk3, sk2, ALICE, &j);

  fp2toos_alloc(params, &j, &jEnc, &jEncLen);

  // h <- F(j)
  function_F(jEnc, jEncLen, h, params->msg_bytes);

  // c1 <- h ^ m
  for (int i = 0; i < params->msg_bytes; ++i)
    c1[i] = h[i] ^ m[i];

  // cleanup
  clear_free(h, params->msg_bytes, MEM_NOT_FREE);
  fp2_Clear(p, &j);
  clear_free(jEnc, jEncLen, MEM_FREE);
}

/**
 * SIKE PKE decryption
 *
 * For B:
 * - B's keygen function using PK_2, SK_3, evaluating on B's curve
 * - Shared secret (j-invariant),
 *
 * @param params SIKE parameters
 * @param sk3 Own private key
 * @param c0 First component of encryption
 * @param c1 Second component of encryption
 * @param m Recovered message
 */
void sike_pke_dec(const sike_params_t* params,
                  const sike_private_key sk3,
                  const sike_public_key_t* c0,
                  const unsigned char* c1,
                  sike_msg* m) {
  const ff_Params* p = params->EA.ffData;
  fp2 j = { 0 };
  unsigned char h[params->msg_bytes];
  unsigned char* jEnc = NULL;
  size_t jEncLen = 0;

  fp2_Init(p, &j);

  // j <- isoex_3(c0, sk3)
  sidh_isoex(params, c0, sk3, BOB, &j);

  fp2toos_alloc(params, &j, &jEnc, &jEncLen);

  // h <- F(j)
  function_F(jEnc, jEncLen, h, params->msg_bytes);

  // c1 = h ^ m
  for (int i = 0; i < params->msg_bytes; ++i)
    m[i] = h[i] ^ c1[i];

  // cleanup
  clear_free(h, params->msg_bytes, MEM_NOT_FREE);
  fp2_Clear(p, &j);
  clear_free(jEnc, jEncLen, MEM_FREE);
}


/**
 * SIKE KEM key generation (KeyGen)
 *
 * @param params SIKE parameters
 * @param pk3 public key
 * @param sk3 private key
 * @param s   SIKE parameter s
 */
void sike_kem_keygen(const sike_params_t *params,
                     sike_public_key_t *pk3,
                     sike_private_key sk3,
                     unsigned char *s) {
  randombytes(s, params->msg_bytes);

  sidh_sk_keygen(params, BOB, sk3);
  sidh_isogen(params, pk3, sk3, BOB);
}

/**
 * SIKE KEM Encapsulation
 *
 * For B:
 * - m <- random(0,1)^l
 * - r <- G(m || pk3)
 * - (c0, c1) <- Enc(pk3, m, r)
 * - K <- H(m || (c0, c1))
 *
 * @param params SIKE parameters
 * @param pk3 Other party's public key
 * @param c0 First component of encryption
 * @param c1 Second component of encryption
 * @param K key (do not share with other party)
 */
void sike_kem_encaps(const sike_params_t *params,
                     const sike_public_key_t *pk3,
                     sike_public_key_t *c0,
                     unsigned char *c1,
                     unsigned char *K) {
  const ff_Params* p = params->EA.ffData;
  size_t rLen = BITS_TO_BYTES_CEIL(params->msbA);
  unsigned char* r = calloc(rLen, 1);

  sike_private_key rDec;
  fp_Init(p, rDec);

  // space for (m || pk3)
  size_t mPk3EncLen = params->msg_bytes + pktoos_len(params, BOB);
  unsigned char mPk3Enc[mPk3EncLen];
  unsigned char* m = mPk3Enc, *pk3Enc = mPk3Enc + params->msg_bytes;

  // m <- random(0,1)^l
  randombytes(m, params->msg_bytes);

  pktoos(params, pk3, pk3Enc, BOB);

  // r <- G(m || pk3)
  function_G(mPk3Enc, mPk3EncLen, r, rLen);


  ostoi(r, BITS_TO_BYTES_CEIL(params->msbA), rDec);
  mp_mod(rDec, params->ordA, rDec);

  // (c0, c1) <- Enc(pk3, m, r)
  sike_pke_enc(params, pk3, m, rDec, c0, c1);

  size_t mCEncLen = params->msg_bytes + encapstoos_len(params);
  unsigned char mCEnc[mCEncLen];
  unsigned char* cEnc = mCEnc + params->msg_bytes;
  memcpy(mCEnc, m, params->msg_bytes);
  encapstoos(params, c0, c1, cEnc);

  // K <- H(m || (c0, c1))
  function_H(mCEnc, mCEncLen, K, params->crypto_bytes);

  // cleanup
  clear_free(r, rLen, MEM_FREE);
  fp_Clear(p, rDec);
}

/**
 * SIKE KEM Decapsulation
 *
 * For B:
 * - m'  <- Dec(sk3, (c0, c1))
 * - r'  <- G(m' || pk3)
 * - c0' <- isogen_2(r')
 * - if (c0' == c0) K <- H(m' || (c0, c1))
 * - else           K <- H(s || (c0, c1))
 * @param params SIKE parameters
 * @param pk3 Own public key
 * @param sk3 Own private key
 * @param c0 First component of the encryption
 * @param c1 Second component of the encrytion
 * @param s SIKE parameter `s`
 * @param K decapsulated keys
 *
 */
void sike_kem_decaps(const sike_params_t *params,
                     const sike_public_key_t *pk3,
                     const sike_private_key sk3,
                     const sike_public_key_t *c0,
                     const unsigned char *c1,
                     const unsigned char *s,
                     unsigned char *K) {
  ff_Params* p = params->EA.ffData;

  // space for (m || pk3)
  size_t mPk3EncLen = params->msg_bytes + pktoos_len(params, BOB);
  unsigned char mPk3Enc[mPk3EncLen];
  unsigned char* m = mPk3Enc, *pk3Enc = mPk3Enc + params->msg_bytes;
  memset(m, 0, params->msg_bytes);

  // m' <- Dec(sk3, (c0, c1))
  sike_pke_dec(params, sk3, c0, c1, m);

  size_t rLen = BITS_TO_BYTES_CEIL(params->msbA);
  unsigned char* r = calloc(rLen, 1);

  sike_private_key rDec;
  fp_Init(p, rDec);

  pktoos(params, pk3, pk3Enc, BOB);

  // r' <- G(m' || pk3)
  function_G(mPk3Enc, mPk3EncLen, r, rLen);

  ostoi(r, rLen, rDec);
  mp_mod(rDec, params->ordA, rDec);

  sike_public_key_t c0Prime = { 0 };
  public_key_init(p, &c0Prime);

  // c0' <- isogen_2(r')
  sidh_isogen(params, &c0Prime, rDec, ALICE);

  unsigned char *c0PrimeEnc = NULL, *c0Enc = NULL;
  size_t c0PrimeEncLen = 0, c0EncLen = 0;
  pktoos_alloc(params, &c0Prime, &c0PrimeEnc, &c0PrimeEncLen, ALICE);
  pktoos_alloc(params, c0, &c0Enc, &c0EncLen, ALICE);

  size_t mCEncLen = params->msg_bytes + encapstoos_len(params);
  unsigned char mCEnc[mCEncLen];
  unsigned char* cEnc = mCEnc + params->msg_bytes;
  encapstoos(params, c0, c1, cEnc);

  int eq = !memcmp(c0Enc, c0PrimeEnc, c0PrimeEncLen);
  if (eq) {
    // K <- H(m' || (c0, c1))
    memcpy(mCEnc, m, params->msg_bytes);
    function_H(mCEnc, mCEncLen, K, params->crypto_bytes);
  } else {
    // K <- H(s || (c0, c1))
    memcpy(mCEnc, s, params->msg_bytes);
    function_H(mCEnc, mCEncLen, K, params->crypto_bytes);
  }

  // cleanup
  clear_free(r, rLen, MEM_FREE);
  fp_Clear(p, rDec);
  public_key_clear(p, &c0Prime);
  clear_free(c0PrimeEnc, c0PrimeEncLen, MEM_FREE);
  clear_free(c0Enc, c0EncLen, MEM_FREE);
  clear_free(mPk3Enc, mPk3EncLen, MEM_NOT_FREE);
}
