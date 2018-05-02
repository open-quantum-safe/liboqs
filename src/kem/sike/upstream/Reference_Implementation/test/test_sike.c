//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <encoding.h>
#include <api_generic.h>
#include <memory.h>
#include "test_sike.h"
#include "printing.h"
#include "test_extras.h"
#include <stdio.h>
#include <random.h>
#include <sike_params.h>

int test_sidh(const char* name, const sike_params_t* params) {
  int rc = 0;

  ff_Params* p = params->EA.ffData;

  mp privateKey_A, privateKey_B;
  sike_public_key_t publicKey_A = { 0 }, publicKey_B = { 0 };
  fp2 shared_sec_A = { 0 }, shared_sec_B = { 0 };

  fp2_Init(p, &shared_sec_A);
  fp2_Init(p, &shared_sec_B);

  unsigned char *enc_sec_A = NULL, *enc_sec_B = NULL;
  size_t enc_sec_A_len = 0, enc_sec_B_len = 0;

  p->init(p, privateKey_A);
  p->init(p, privateKey_B);

  public_key_init(p, &publicKey_A);
  public_key_init(p, &publicKey_B);


  sidh_sk_keygen(params, ALICE, privateKey_A);
  sidh_isogen(params, &publicKey_A, privateKey_A, ALICE);

  printFp2("pkA.Px = ", &publicKey_A.xP);
  printFp2("pkA.Qx = ", &publicKey_A.xQ);
  printFp2("pkA.Rx = ", &publicKey_A.xR);

  sidh_sk_keygen(params, BOB, privateKey_B);
  sidh_isogen(params, &publicKey_B, privateKey_B, BOB);

  printFp2("pkB.Px = ", &publicKey_B.xP);
  printFp2("pkB.Qx = ", &publicKey_B.xQ);
  printFp2("pkB.Rx = ", &publicKey_B.xR);

  gmp_printf("skA = %Zx\n", privateKey_A);

  sidh_isoex(params, &publicKey_B, privateKey_A, ALICE, &shared_sec_A);

  printFp2("Alice ss: ", &shared_sec_A);

  gmp_printf("skB = %Zx\n", privateKey_B);

  sidh_isoex(params, &publicKey_A, privateKey_B, BOB, &shared_sec_B);

  printFp2("Bob ss: ", &shared_sec_B);

  fp2toos_alloc(params, &shared_sec_A, &enc_sec_A, &enc_sec_A_len);
  fp2toos_alloc(params, &shared_sec_B, &enc_sec_B, &enc_sec_B_len);




  if (enc_sec_A_len == 0 || enc_sec_B_len == 0 || enc_sec_A_len != enc_sec_B_len) {
    printf("Lengths invalid: enc_sec_A_len == %zu, enc_sec_B_len == %zu\n", enc_sec_A_len, enc_sec_B_len);
    rc = 1;
  }

  if (memcmp(enc_sec_A, enc_sec_B, enc_sec_A_len)) {
    printf("Shared secrets don't match...\n");
    print_hex("A", enc_sec_A, enc_sec_A_len);
    print_hex("B", enc_sec_B, enc_sec_B_len);

    rc = 1;
  }

  if (!rc) {
    printf("succeeded:\n");
    print_hex("A", enc_sec_A, enc_sec_A_len);
    print_hex("B", enc_sec_B, enc_sec_B_len);
  }

  end:
  p->clear(p, privateKey_A);
  p->clear(p, privateKey_B);
  public_key_clear(p, &publicKey_A);
  public_key_clear(p, &publicKey_B);

  fp2_Clear(p, &shared_sec_A);
  fp2_Clear(p, &shared_sec_B);

  free(enc_sec_A);
  free(enc_sec_B);

  return rc;
}

int test_pke(const char* name, const sike_params_t* params) {

  int rc = 0;

  ff_Params* p = params->EA.ffData;

  sike_private_key sk2, sk3;
  sike_public_key_t pk3 = { 0 };
  sike_public_key_t c0 = { 0 };
  unsigned char c1[params->msg_bytes];

  public_key_init(p, &c0);
  public_key_init(p, &pk3);

  unsigned char m[params->msg_bytes];

  unsigned char mRecovered[params->msg_bytes];

  randombytes(m, params->msg_bytes);

  for (int i = 0; i < params->msg_bytes; ++i) {
    mRecovered[i] = 0;
    c1[i] = 0;
  }

  p->init(p, sk2);
  p->init(p, sk3);

  sidh_sk_keygen(params, ALICE, sk2);

  print_hex("msg = ", m, params->msg_bytes);

  sidh_sk_keygen(params, BOB, sk3);

  sidh_isogen(params, &pk3, sk3, BOB);

  sike_pke_enc(params, &pk3, m, sk2, &c0, c1);

  sike_pke_dec(params, sk3, &c0, c1, mRecovered);

  print_hex("c1              = ", c1, params->msg_bytes);
  print_hex("msg             = ", m, params->msg_bytes);
  print_hex("msg (recovered) = ", mRecovered, params->msg_bytes);

  int eq = !memcmp(m, mRecovered, params->msg_bytes);
  if (!eq) rc = 1;

  public_key_clear(p, &c0);
  public_key_clear(p, &pk3);

  end:
  return rc;
}

int test_sike(const char* name, const sike_params_t* params, int rnd) {

  int rc = 0;

  size_t pkLen = pktoos_len(params, BOB);
  size_t skLen = sktoos_len(params, BOB);
  size_t ctLen = encapstoos_len(params);
  size_t ssLen = params->crypto_bytes;

  unsigned char* pk3    = calloc(pkLen, 1);
  unsigned char* sk3    = calloc(skLen, 1);
  unsigned char* ct     = calloc(ctLen, 1);
  unsigned char* ss     = calloc(ssLen, 1);
  unsigned char* ss_rec = calloc(ssLen, 1);

  crypto_kem_keypair_generic(params, pk3, sk3);

  printf("KEM keypair:\n");
  print_hex("pk = ", pk3, pkLen);
  print_hex("sk = ", sk3, skLen);

  rc = crypto_kem_enc_generic(params, ct, ss, pk3);
  if ( rc ) goto end;

  printf("KEM enc:\n");
  print_hex("ct = ", ct, ctLen);
  print_hex("ss = ", ss, ssLen);

  rc = crypto_kem_dec_generic(params, ss_rec, ct, sk3);
  if ( rc ) goto end;

  printf("KEM dec:\n");
  print_hex("ss_decaps = ", ss_rec, ssLen);

  int eq = !memcmp(ss, ss_rec, params->crypto_bytes);

  if (!eq) rc = 1;

end:
  free(pk3);
  free(sk3);
  free(ct);
  free(ss);
  free(ss_rec);
  return rc;
}

int test_sike_int(const char* name, const sike_params_t* params) {

  int rc = 0;

  ff_Params* p = params->EA.ffData;

  sike_private_key sk3;
  sike_public_key_t pk3 = { 0 };
  sike_public_key_t c0 = { 0 };
  unsigned char c1    [params->msg_bytes];
  unsigned char K     [params->crypto_bytes];
  unsigned char K_rec [params->crypto_bytes];
  unsigned char s     [params->msg_bytes];

  public_key_init(p, &c0);
  public_key_init(p, &pk3);

  p->init(p, sk3);

  sidh_sk_keygen(params, BOB, sk3);

  sidh_isogen(params, &pk3, sk3, BOB);

  sike_kem_encaps(params, &pk3, &c0, c1, K);

  print_hex("K             = ", K, params->crypto_bytes);

  sike_kem_decaps(params, &pk3, sk3, &c0, c1, s, K_rec);

  //print_hex("ct            = ", ctEnc, ctLen);
  //print_hex("c1            = ", c1, params->crypto_bytes);
  print_hex("K (recovered) = ", K_rec, params->crypto_bytes);

  int eq = !memcmp(K, K_rec, params->crypto_bytes);
  if (!eq) rc = 1;

  public_key_clear(p, &c0);
  public_key_clear(p, &pk3);

end:
  return rc;
}

int test_sike_speedy(const char* name, const sike_params_t* params, int runs) {

  int rc = 0;
  unsigned int i;

  int64_t cycles, cycles1, cycles2;

  size_t pkLen = pktoos_len(params, BOB);
  size_t skLen = sktoos_len(params, BOB);
  size_t ctLen = encapstoos_len(params);
  size_t ssLen = params->crypto_bytes;

  unsigned char* pk3    = calloc(pkLen, 1);
  unsigned char* sk3    = calloc(skLen, 1);
  unsigned char* ct     = calloc(ctLen, 1);
  unsigned char* ss     = calloc(ssLen, 1);
  unsigned char* ss_rec = calloc(ssLen, 1);

  printf("Performance of %s (avg. of %d runs):\n", name, runs);

  cycles = 0;
  for (i = 0; i < runs; ++i) {
    cycles1 = cpucycles();
    crypto_kem_keypair_generic(params, pk3, sk3);
    cycles2 = cpucycles();

    cycles = cycles + (cycles2 - cycles1);
  }
  printf("  Key generation runs in ....................................... %10ld ", (cycles / runs));
  print_unit

  cycles = 0;
  for (i = 0; i < runs; ++i) {
    cycles1 = cpucycles();
    rc = crypto_kem_enc_generic(params, ct, ss, pk3);
    cycles2 = cpucycles();
    if ( rc ) goto end;

    cycles = cycles + (cycles2 - cycles1);
  }
  printf("  Encapsulation runs in ........................................ %10ld ", (cycles / runs));
  print_unit

  cycles = 0;
  for (i = 0; i < runs; ++i) {
    cycles1 = cpucycles();
    rc = crypto_kem_dec_generic(params, ss_rec, ct, sk3);
    cycles2 = cpucycles();
    if ( rc ) goto end;

    cycles = cycles + (cycles2 - cycles1);
  }
  printf("  Decapsulation runs in ........................................ %10ld ", (cycles / runs));
  print_unit

end:
  free(pk3);
  free(sk3);
  free(ct);
  free(ss);
  free(ss_rec);
  return rc;
}

int test_sike_speedy_int(const char *name, const sike_params_t *params, int runs) {

  int rc = 0;
  unsigned int i;

  int64_t cycles, cycles1, cycles2;

  ff_Params* p = params->EA.ffData;

  sike_private_key sk3;
  sike_public_key_t pk3 = { 0 };
  sike_public_key_t c0 = { 0 };
  unsigned char c1    [params->crypto_bytes];
  unsigned char K     [params->crypto_bytes];
  unsigned char K_rec [params->crypto_bytes];
  unsigned char s     [params->crypto_bytes];

  printf("Performance of %s - internal API (avg. of %d runs):\n", name, runs);

  public_key_init(p, &c0);
  public_key_init(p, &pk3);

  p->init(p, sk3);

  sidh_sk_keygen(params, BOB, sk3);

  cycles = 0;
  for (i = 0; i < runs; ++i) {
    cycles1 = cpucycles();
    sidh_isogen(params, &pk3, sk3, BOB);
    cycles2 = cpucycles();
    cycles = cycles + (cycles2 - cycles1);
  }
  printf("  Key generation runs in ....................................... %10ld ", (cycles / runs));
  print_unit

  cycles = 0;
  for (i = 0; i < runs; ++i) {
    cycles1 = cpucycles();
    sike_kem_encaps(params, &pk3, &c0, c1, K);
    cycles2 = cpucycles();
    cycles = cycles + (cycles2 - cycles1);
  }
  printf("  Encapsulation runs in ........................................ %10ld ", (cycles / runs));
  print_unit

  cycles = 0;
  for (i = 0; i < runs; ++i) {
    cycles1 = cpucycles();
    sike_kem_decaps(params, &pk3, sk3, &c0, c1, s, K_rec);
    cycles2 = cpucycles();
    cycles = cycles + (cycles2 - cycles1);
  }
  printf("  Decapsulation runs in ........................................ %10ld ", (cycles / runs));
  print_unit

  public_key_clear(p, &c0);
  public_key_clear(p, &pk3);

  return rc;
}
