//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <sidh.h>
#include <encoding.h>
#include <api_generic.h>

// SIKE's key generation. Generic version, Supports all parameter sets.
// It produces a private key sk and computes the public key pk.
// Outputs: secret key sk
//          public key pk
int crypto_kem_keypair_generic(const sike_params_t *params, unsigned char *pk, unsigned char *sk) {
  ff_Params* p = params->EB.ffData;

  sike_private_key sk3;
  sike_public_key_t pk3 = { 0 };
  unsigned char s[params->msg_bytes];

  fp_Init(p, sk3);
  public_key_init(p, &pk3);

  sike_kem_keygen(params, &pk3, sk3, s);

  pktoos(params, &pk3, pk, BOB);
  sktoos(params, BOB, s, sk3, &pk3, sk);

  fp_Clear(p, sk3);
  public_key_clear(p, &pk3);
  return 0;
}

// SIKE's encapsulation Generic version, Supports all parameter sets.
// Input:   public key pk
// Outputs: shared secret ss
//          ciphertext message ct (this is c0 and c1 encoded)
int crypto_kem_enc_generic(const sike_params_t *params,
                           unsigned char *ct,
                           unsigned char *ss,
                           const unsigned char *pk) {

  int rc = 0;

  ff_Params* p = params->EB.ffData;
  sike_public_key_t c0 = { 0 };
  sike_public_key_t pk3 = { 0 };
  unsigned char c1[params->msg_bytes];

  public_key_init(p, &c0);
  public_key_init(p, &pk3);

  rc = ostopk(params, BOB, pk, &pk3);
  if ( rc ) goto end;

  sike_kem_encaps(params, &pk3, &c0, c1, ss);

  encapstoos(params, &c0, c1, ct);

end:
  public_key_clear(p, &c0);
  public_key_clear(p, &pk3);
  return rc;

}

// SIKE's decapsulation
// Input:   secret key sk
//          ciphertext message ct
// Outputs: shared secret ss
int crypto_kem_dec_generic(const sike_params_t *params,
                           unsigned char *ss,
                           const unsigned char *ct,
                           const unsigned char *sk) {

  ff_Params* p = params->EB.ffData;
  sike_private_key sk3;
  sike_public_key_t pk3 = { 0 };
  sike_public_key_t c0 = { 0 };
  unsigned char s[params->msg_bytes];
  unsigned char c1[params->msg_bytes];

  fp_Init(p, sk3);
  public_key_init(p, &pk3);
  public_key_init(p, &c0);

  ostosk(params, BOB, sk, s, sk3, &pk3);

  ostoencaps(params, ct, &c0, c1);

  sike_kem_decaps(params, &pk3, sk3, &c0, c1, s, ss);

end:
  fp_Clear(p, sk3);
  public_key_clear(p, &pk3);
  public_key_clear(p, &c0);
  return 0;
}
