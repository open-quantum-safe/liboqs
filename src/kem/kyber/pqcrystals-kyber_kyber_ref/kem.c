#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "kem.h"
#include "indcpa.h"
#include "verify.h"
#include "symmetric.h"
#include "randombytes.h"
extern param params[];
/*************************************************
* Name:        crypto_kem_keypair
*
* Description: Generates public and private key
*              for CCA-secure Kyber key encapsulation mechanism
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
*                (an already allocated array of KYBER_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/


int crypto_kem_keypair_512(uint8_t *pk,
                       uint8_t *sk)
{
  int8_t security_level = 0;
  size_t i;
  indcpa_keypair(pk, sk, security_level);
  for(i=0;i<(size_t)params[security_level].KYBER_INDCPA_PUBLICKEYBYTES;i++)
    sk[i+params[security_level].KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
  hash_h(sk+params[security_level].KYBER_SECRETKEYBYTES-2*params[security_level].KYBER_SYMBYTES, pk, params[security_level].KYBER_PUBLICKEYBYTES);
  /* Value z for pseudo-random output on reject */
  randombytes(sk+params[security_level].KYBER_SECRETKEYBYTES-params[security_level].KYBER_SYMBYTES, params[security_level].KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_keypair_768(uint8_t *pk,
                       uint8_t *sk)
{
  int8_t security_level = 1;
  size_t i;
  indcpa_keypair(pk, sk,security_level);
  for(i=0;i<(size_t)params[security_level].KYBER_INDCPA_PUBLICKEYBYTES;i++)
    sk[i+params[security_level].KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
  hash_h(sk+params[security_level].KYBER_SECRETKEYBYTES-2*params[security_level].KYBER_SYMBYTES, pk, params[security_level].KYBER_PUBLICKEYBYTES);
  /* Value z for pseudo-random output on reject */
  randombytes(sk+params[security_level].KYBER_SECRETKEYBYTES-params[security_level].KYBER_SYMBYTES, params[security_level].KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_keypair_1024(uint8_t *pk,
                       uint8_t *sk)
{
  int8_t security_level = 2;
  size_t i;
  indcpa_keypair(pk, sk,security_level);
  for(i=0;i<(size_t)params[security_level].KYBER_INDCPA_PUBLICKEYBYTES;i++)
    sk[i+params[security_level].KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
  hash_h(sk+params[security_level].KYBER_SECRETKEYBYTES-2*params[security_level].KYBER_SYMBYTES, pk, params[security_level].KYBER_PUBLICKEYBYTES);
  /* Value z for pseudo-random output on reject */
  randombytes(sk+params[security_level].KYBER_SECRETKEYBYTES-params[security_level].KYBER_SYMBYTES, params[security_level].KYBER_SYMBYTES);
  return 0;
}

/*************************************************
* Name:        crypto_kem_enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - uint8_t *ct: pointer to output cipher text
*                (an already allocated array of KYBER_CIPHERTEXTBYTES bytes)
*              - uint8_t *ss: pointer to output shared secret
*                (an already allocated array of KYBER_SSBYTES bytes)
*              - const uint8_t *pk: pointer to input public key
*                (an already allocated array of KYBER_PUBLICKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
int crypto_kem_enc_512(uint8_t *ct,
                   uint8_t *ss,
                   const uint8_t *pk)
{
  int8_t security_level =0;
  uint8_t buf[2*params[security_level].KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*params[security_level].KYBER_SYMBYTES];

  randombytes(buf, params[security_level].KYBER_SYMBYTES);
  /* Don't release system RNG output */
  hash_h(buf, buf, params[security_level].KYBER_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  hash_h(buf+params[security_level].KYBER_SYMBYTES, pk, params[security_level].KYBER_PUBLICKEYBYTES);
  hash_g(kr, buf, 2*params[security_level].KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc(ct, buf, pk, kr+params[security_level].KYBER_SYMBYTES,security_level);

  /* overwrite coins in kr with H(c) */
  hash_h(kr+params[security_level].KYBER_SYMBYTES, ct, params[security_level].KYBER_CIPHERTEXTBYTES);
  /* hash concatenation of pre-k and H(c) to k */
  kdf(ss, params[security_level].KYBER_SSBYTES,kr, 2*params[security_level].KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_enc_768(uint8_t *ct,
                   uint8_t *ss,
                   const uint8_t *pk)
{
  int8_t security_level =1;
  uint8_t buf[2*params[security_level].KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*params[security_level].KYBER_SYMBYTES];

  randombytes(buf, params[security_level].KYBER_SYMBYTES);
  /* Don't release system RNG output */
  hash_h(buf, buf, params[security_level].KYBER_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  hash_h(buf+params[security_level].KYBER_SYMBYTES, pk, params[security_level].KYBER_PUBLICKEYBYTES);
  hash_g(kr, buf, 2*params[security_level].KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc(ct, buf, pk, kr+params[security_level].KYBER_SYMBYTES,security_level);

  /* overwrite coins in kr with H(c) */
  hash_h(kr+params[security_level].KYBER_SYMBYTES, ct, params[security_level].KYBER_CIPHERTEXTBYTES);
  /* hash concatenation of pre-k and H(c) to k */
  kdf(ss, params[security_level].KYBER_SSBYTES,kr, 2*params[security_level].KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_enc_1024(uint8_t *ct,
                   uint8_t *ss,
                   const uint8_t *pk)
{
  int8_t security_level =2;
  uint8_t buf[2*params[security_level].KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*params[security_level].KYBER_SYMBYTES];

  randombytes(buf, params[security_level].KYBER_SYMBYTES);
  /* Don't release system RNG output */
  hash_h(buf, buf, params[security_level].KYBER_SYMBYTES);

  /* Multitarget countermeasure for coins + contributory KEM */
  hash_h(buf+params[security_level].KYBER_SYMBYTES, pk, params[security_level].KYBER_PUBLICKEYBYTES);
  hash_g(kr, buf, 2*params[security_level].KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc(ct, buf, pk, kr+params[security_level].KYBER_SYMBYTES,security_level);

  /* overwrite coins in kr with H(c) */
  hash_h(kr+params[security_level].KYBER_SYMBYTES, ct, params[security_level].KYBER_CIPHERTEXTBYTES);
  /* hash concatenation of pre-k and H(c) to k */
  kdf(ss, params[security_level].KYBER_SSBYTES,kr, 2*params[security_level].KYBER_SYMBYTES);
  return 0;
}

/*************************************************
* Name:        crypto_kem_dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   - uint8_t *ss: pointer to output shared secret
*                (an already allocated array of KYBER_SSBYTES bytes)
*              - const uint8_t *ct: pointer to input cipher text
*                (an already allocated array of KYBER_CIPHERTEXTBYTES bytes)
*              - const uint8_t *sk: pointer to input private key
*                (an already allocated array of KYBER_SECRETKEYBYTES bytes)
*
* Returns 0.
*
* On failure, ss will contain a pseudo-random value.
**************************************************/
int crypto_kem_dec_512(uint8_t *ss,
                   const uint8_t *ct,
                   const uint8_t *sk)
{
  int8_t security_level =0;
  size_t i;
  int fail;
  uint8_t buf[2*params[security_level].KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*params[security_level].KYBER_SYMBYTES];
  uint8_t cmp[params[security_level].KYBER_CIPHERTEXTBYTES];
  const uint8_t *pk = sk+params[security_level].KYBER_INDCPA_SECRETKEYBYTES;

  indcpa_dec(buf, ct, sk,security_level);

  /* Multitarget countermeasure for coins + contributory KEM */
  for(i=0;i<(size_t)params[security_level].KYBER_SYMBYTES;i++)
    buf[params[security_level].KYBER_SYMBYTES+i] = sk[params[security_level].KYBER_SECRETKEYBYTES-2*params[security_level].KYBER_SYMBYTES+i];
  hash_g(kr, buf, 2*params[security_level].KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc(cmp, buf, pk, kr+params[security_level].KYBER_SYMBYTES, security_level);

  fail = verify(ct, cmp, params[security_level].KYBER_CIPHERTEXTBYTES);

  /* overwrite coins in kr with H(c) */
  hash_h(kr+params[security_level].KYBER_SYMBYTES, ct, params[security_level].KYBER_CIPHERTEXTBYTES);

  /* Overwrite pre-k with z on re-encryption failure */
  cmov(kr, sk+params[security_level].KYBER_SECRETKEYBYTES-params[security_level].KYBER_SYMBYTES, params[security_level].KYBER_SYMBYTES, fail);

  /* hash concatenation of pre-k and H(c) to k */
  kdf(ss,params[security_level].KYBER_SSBYTES, kr, 2*params[security_level].KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_dec_768(uint8_t *ss,
                   const uint8_t *ct,
                   const uint8_t *sk)
{
  int8_t security_level = 1;
  size_t i;
  int fail;
  uint8_t buf[2*params[security_level].KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*params[security_level].KYBER_SYMBYTES];
  uint8_t cmp[params[security_level].KYBER_CIPHERTEXTBYTES];
  const uint8_t *pk = sk+params[security_level].KYBER_INDCPA_SECRETKEYBYTES;

  indcpa_dec(buf, ct, sk,security_level);

  /* Multitarget countermeasure for coins + contributory KEM */
  for(i=0;i<(size_t)params[security_level].KYBER_SYMBYTES;i++)
    buf[params[security_level].KYBER_SYMBYTES+i] = sk[params[security_level].KYBER_SECRETKEYBYTES-2*params[security_level].KYBER_SYMBYTES+i];
  hash_g(kr, buf, 2*params[security_level].KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc(cmp, buf, pk, kr+params[security_level].KYBER_SYMBYTES, security_level);

  fail = verify(ct, cmp, params[security_level].KYBER_CIPHERTEXTBYTES);

  /* overwrite coins in kr with H(c) */
  hash_h(kr+params[security_level].KYBER_SYMBYTES, ct, params[security_level].KYBER_CIPHERTEXTBYTES);

  /* Overwrite pre-k with z on re-encryption failure */
  cmov(kr, sk+params[security_level].KYBER_SECRETKEYBYTES-params[security_level].KYBER_SYMBYTES, params[security_level].KYBER_SYMBYTES, fail);

  /* hash concatenation of pre-k and H(c) to k */
  kdf(ss,params[security_level].KYBER_SSBYTES, kr, 2*params[security_level].KYBER_SYMBYTES);
  return 0;
}

int crypto_kem_dec_1024(uint8_t *ss,
                   const uint8_t *ct,
                   const uint8_t *sk)
{
  int8_t security_level =2;
  size_t i;
  int fail;
  uint8_t buf[2*params[security_level].KYBER_SYMBYTES];
  /* Will contain key, coins */
  uint8_t kr[2*params[security_level].KYBER_SYMBYTES];
  uint8_t cmp[params[security_level].KYBER_CIPHERTEXTBYTES];
  const uint8_t *pk = sk+params[security_level].KYBER_INDCPA_SECRETKEYBYTES;

  indcpa_dec(buf, ct, sk,security_level);

  /* Multitarget countermeasure for coins + contributory KEM */
  for(i=0;i<(size_t)params[security_level].KYBER_SYMBYTES;i++)
    buf[params[security_level].KYBER_SYMBYTES+i] = sk[params[security_level].KYBER_SECRETKEYBYTES-2*params[security_level].KYBER_SYMBYTES+i];
  hash_g(kr, buf, 2*params[security_level].KYBER_SYMBYTES);

  /* coins are in kr+KYBER_SYMBYTES */
  indcpa_enc(cmp, buf, pk, kr+params[security_level].KYBER_SYMBYTES, security_level);

  fail = verify(ct, cmp, params[security_level].KYBER_CIPHERTEXTBYTES);

  /* overwrite coins in kr with H(c) */
  hash_h(kr+params[security_level].KYBER_SYMBYTES, ct, params[security_level].KYBER_CIPHERTEXTBYTES);

  /* Overwrite pre-k with z on re-encryption failure */
  cmov(kr, sk+params[security_level].KYBER_SECRETKEYBYTES-params[security_level].KYBER_SYMBYTES, params[security_level].KYBER_SYMBYTES, fail);

  /* hash concatenation of pre-k and H(c) to k */
  kdf(ss,params[security_level].KYBER_SSBYTES, kr, 2*params[security_level].KYBER_SYMBYTES);
  return 0;
}
