//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include "api.h"
#include <sike_params.h>
#include <api_generic.h>

// SIKE's key generation
// It produces a private key sk and computes the public key pk.
// Outputs: secret key sk (CRYPTO_SECRETKEYBYTES = 818 bytes)
//          public key pk (CRYPTO_PUBLICKEYBYTES = 726 bytes)
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
  int rc = 0;
  sike_params_t params = { 0 };

  sike_setup_params(&SIKEp964, &params);

  rc = crypto_kem_keypair_generic(&params, pk, sk);
  if ( rc ) goto end;

end:
  sike_teardown_params(&params);
  if (rc) return -1;
  else return 0;
}

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
  int rc = 0;
  sike_params_t params = { 0 };

  sike_setup_params(&SIKEp964, &params);

  rc = crypto_kem_enc_generic(&params, ct, ss, pk);
  if ( rc ) goto end;

end:
  sike_teardown_params(&params);
  if (rc) return -1;
  else return 0;
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
  int rc = 0;
  sike_params_t params = { 0 };

  sike_setup_params(&SIKEp964, &params);

  rc = crypto_kem_dec_generic(&params, ss, ct, sk);
  if ( rc ) goto end;

end:
  sike_teardown_params(&params);
  if (rc) return -1;
  else return 0;
}
