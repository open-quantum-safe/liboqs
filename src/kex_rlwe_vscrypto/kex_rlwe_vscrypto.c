
// liboqs wrapper around refactored vscrypto ringlwe code

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include <oqs/kex.h>
#include <oqs/rand.h>

#include "kex_rlwe_vscrypto.h"
#include "vscrypto_kex.h"
#include "vscrypto_defs.h"

#define UNUSED __attribute__ ((unused))

typedef enum {
  VSCRING_NONE,
  VSCRING_337_32353,
  VSCRING_433_35507,
  VSCRING_541_41117,
  VSCRING_631_44171,
  VSCRING_739_47297,
  VSCRING_821_49261
} ring_id_t;

extern ring_ctx_t *GLOBAL(ring_ctx_337_32353)(void);
extern ring_ctx_t *GLOBAL(ring_ctx_433_35507)(void);
extern ring_ctx_t *GLOBAL(ring_ctx_541_41117)(void);
extern ring_ctx_t *GLOBAL(ring_ctx_631_44171)(void);
extern ring_ctx_t *GLOBAL(ring_ctx_739_47297)(void);
extern ring_ctx_t *GLOBAL(ring_ctx_821_49261)(void);

OQS_KEX *OQS_KEX_rlwe_vscrypto_new(OQS_RAND *rand, const char *named_parameters) {
  ring_id_t ring_id = VSCRING_NONE;
  OQS_KEX *k;

  // Which ring from the "EMP" paper (Even More Practical Key Exchanges
  // for the Internet using Lattice Cryptography) ?
  //
  if (!named_parameters) return NULL;
  if (!strcmp(named_parameters,"EMP_I"))   ring_id = VSCRING_337_32353;
  if (!strcmp(named_parameters,"EMP_II"))  ring_id = VSCRING_433_35507;
  if (!strcmp(named_parameters,"EMP_III")) ring_id = VSCRING_541_41117;
  if (!strcmp(named_parameters,"EMP_IV"))  ring_id = VSCRING_631_44171;
  if (!strcmp(named_parameters,"EMP_V"))   ring_id = VSCRING_739_47297;
  if (!strcmp(named_parameters,"EMP_VI"))  ring_id = VSCRING_821_49261;
  if (ring_id == VSCRING_NONE) return NULL;

  if (!(k=malloc(sizeof(OQS_KEX)))) return NULL;
  if (!(k->method_name=malloc(100))) { free(k); return NULL; }

  switch (ring_id) {
    case VSCRING_337_32353:
      k->ctx = GLOBAL(ring_ctx_337_32353)();
      k->estimated_classical_security = 102;
      k->estimated_quantum_security = 93;
      break;
    case VSCRING_433_35507:
      k->ctx = GLOBAL(ring_ctx_433_35507)();
      k->estimated_classical_security = 132;
      k->estimated_quantum_security = 118;
      break;
    case VSCRING_541_41117:
      k->ctx = GLOBAL(ring_ctx_541_41117)();
      k->estimated_classical_security = 165;
      k->estimated_quantum_security = 146;
      break;
    case VSCRING_631_44171:
      k->ctx = GLOBAL(ring_ctx_631_44171)();
      k->estimated_classical_security = 194;
      k->estimated_quantum_security = 171;
      break;
    case VSCRING_739_47297:
      k->ctx = GLOBAL(ring_ctx_739_47297)();
      k->estimated_classical_security = 231;
      k->estimated_quantum_security = 202;
      break;
    case VSCRING_821_49261:
      k->ctx = GLOBAL(ring_ctx_821_49261)();
      k->estimated_classical_security = 259;
      k->estimated_quantum_security = 226;
      break;
    default:
      assert(0);   // should never happen
  }

  sprintf(k->method_name,"RLWE_VSCRYPTO_%s",named_parameters);
  k->seed = NULL;
  k->seed_len = 0;
  k->named_parameters = NULL;
  k->rand = rand;
  k->params = NULL;
  k->alice_0 = &OQS_KEX_rlwe_vscrypto_alice_0;
  k->bob = &OQS_KEX_rlwe_vscrypto_bob;
  k->alice_1 = &OQS_KEX_rlwe_vscrypto_alice_1;
  k->alice_priv_free = &OQS_KEX_rlwe_vscrypto_alice_priv_free;
  k->free = &OQS_KEX_rlwe_vscrypto_free;

  return k;

}

// packing to/from bytes because vscrypto uses different datatypes
// (should really modify vscrypto to remove the need for this overhead)
static void u8_to_ringelt(const uint8_t *u8, RINGELT *re, int nr_ringelts) {
  int i;
  for (i=0; i<nr_ringelts; i++) re[i] = 256*u8[2*i] + u8[2*i+1];
  return;
}
static void ringelt_to_u8(RINGELT *re, uint8_t *u8, int nr_ringelts) {
  int i;
  for (i=0; i<nr_ringelts; i++) { u8[2*i] = re[i]>>8; u8[2*i+1] = re[i]&0xff; }
  return;
}
static void u64_to_bits(uint64_t *u64, uint8_t *packed_bits, int nr_bits) {
  int i;
  for (i=0; 8*i<nr_bits; i++) packed_bits[i] = 0xff & (u64[i/8]>>(8*(i&7)));
  return;
}
static void bits_to_u64(const uint8_t *packed_bits, uint64_t *u64, int nr_bits) {
  int i;
  for (i=0; 64*i<nr_bits; i++) u64[i] = 0;
  for (i=0; 8*i<nr_bits; i++) u64[i/8] |= ((uint64_t)packed_bits[i]) << (8*(i&7));
  return;
}

// ~~~
// generate fresh private & public values
// ~~~
int OQS_KEX_rlwe_vscrypto_alice_0(OQS_KEX *k, void **alice_priv_void, uint8_t **alice_msg, size_t *alice_msg_len) {
  ring_ctx_t *ctx = k->ctx;
  RINGELT **alice_priv = (RINGELT**)alice_priv_void;
  const unsigned nr_ringelts = NR_PUBPRIV_RINGELTS(ctx);
  RINGELT kem1_s[2*nr_ringelts], kem1_b[nr_ringelts];
  int i;

  assert(ctx->prime_q<65536);

  // get space for public/private key pair
  if (!(*alice_msg=malloc(2*nr_ringelts))) return 0;
  if (!(*alice_priv=malloc(nr_ringelts*sizeof(RINGELT)))) { free(*alice_msg); *alice_msg = 0; return 0; }
  *alice_msg_len = 2*nr_ringelts;

  // get noise, private and public from vscrypto ringlwe
  GLOBAL(KEM1_Generate)(ctx,kem1_s,kem1_b,k->rand);

  // return to the caller
  ringelt_to_u8(kem1_b,*alice_msg,nr_ringelts);
  for (i=0; i<(int)nr_ringelts; i++) (*alice_priv)[i] = kem1_s[i+nr_ringelts];

  return 1;
}

// ~~~
// given Alice's public, generate Bob's private & public and
// reconciliation data to form the response
// ~~~
int OQS_KEX_rlwe_vscrypto_bob(OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) {
  ring_ctx_t *ctx = k->ctx;
  const unsigned nr_ringelts = NR_PUBPRIV_RINGELTS(ctx);
  const unsigned nr_bits = NR_RECKEY_BITS(ctx);
  RINGELT kem1_b[nr_ringelts], kem1_u[nr_ringelts];
  uint64_t kem1_cr_v[ctx->muwords], kem1_mu[ctx->muwords];
  uint8_t *rec;

  assert(ctx->prime_q<65536);
  assert(alice_msg_len==2*nr_ringelts);

  // allocate: 2*nr_ringelts bytes for Bob's public
  //           (nr_bits+7)/8 bytes for reconciliation data
  //           (nr_bits+7)/8 bytes for shared key
  if (!(*bob_msg=malloc(2*nr_ringelts+(nr_bits+7)/8))) return 0;
  if (!(*key=malloc((nr_bits+7)/8))) { free(*bob_msg); *bob_msg = 0; return 0; }
  *bob_msg_len = 2*nr_ringelts + (nr_bits+7)/8;
  rec = 2*nr_ringelts + *bob_msg;  // tail end of *bob_msg
  *key_len = (nr_bits+7)/8;

  // get public, reconciliation data and key from vscrypto rlwe
  u8_to_ringelt(alice_msg,kem1_b,nr_ringelts);
  GLOBAL(KEM1_Encapsulate)(ctx,kem1_u,kem1_cr_v,kem1_mu,kem1_b,k->rand);

  // return to the caller
  ringelt_to_u8(kem1_u,*bob_msg,nr_ringelts);
  u64_to_bits(kem1_cr_v,rec,nr_bits);
  u64_to_bits(kem1_mu,*key,nr_bits);

  return 1;
}

int OQS_KEX_rlwe_vscrypto_alice_1(OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) {
  ring_ctx_t *ctx = k->ctx;
  const unsigned nr_ringelts = NR_PUBPRIV_RINGELTS(ctx);
  const unsigned nr_bits = NR_RECKEY_BITS(ctx);
  const uint8_t *bob_rec = 2*nr_ringelts + bob_msg;
  uint64_t kem1_mu[ctx->muwords], kem1_cr_v[ctx->muwords];
  RINGELT kem1_u[nr_ringelts];
  const RINGELT *kem1_s_1 = alice_priv;

  assert(ctx->prime_q<65536);
  assert(bob_msg_len==2*nr_ringelts+(nr_bits+7)/8);

  // allocate space for shared key
  if (!(*key=malloc((nr_bits+7)/8))) return 0;
  *key_len = (nr_bits+7)/8;

  // generate Alice's session key
  u8_to_ringelt(bob_msg,kem1_u,nr_ringelts);
  bits_to_u64(bob_rec,kem1_cr_v,nr_bits);
  GLOBAL(KEM1_Decapsulate)(ctx,kem1_mu,kem1_u,kem1_s_1,kem1_cr_v);

  // return to the caller
  u64_to_bits(kem1_mu,*key,nr_bits);

  return 1;
}

void OQS_KEX_rlwe_vscrypto_alice_priv_free(UNUSED OQS_KEX *k, void *alice_priv) {
  if (alice_priv) free(alice_priv);
  return;
}

void OQS_KEX_rlwe_vscrypto_free(OQS_KEX *k) {
  if (!k) return;
  free(k->method_name);
  k->method_name = NULL;
  free(k);
  return;
}

