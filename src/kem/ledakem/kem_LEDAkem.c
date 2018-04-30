#include <stdlib.h>

#include <oqs/kem_LEDAkem.h>


OQS_KEM *OQS_KEM_ledakem_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
#if (defined(OQS_ENABLE_KEM_ledakem_C1_N02) || \
  defined(OQS_ENABLE_KEM_ledakem_C1_N03) || \
  defined(OQS_ENABLE_KEM_ledakem_C1_N04))
	kem->claimed_nist_level = 1;
#elif (defined(OQS_ENABLE_KEM_ledakem_C3_N02) || \
  defined(OQS_ENABLE_KEM_ledakem_C3_N03) || \
  defined(OQS_ENABLE_KEM_ledakem_C3_N04))
	kem->claimed_nist_level = 3;
#elif (defined(OQS_ENABLE_KEM_ledakem_C5_N02) || \
  defined(OQS_ENABLE_KEM_ledakem_C5_N03) || \
  defined(OQS_ENABLE_KEM_ledakem_C5_N04))
	kem->claimed_nist_level = 5; 
#endif 
	kem->ind_cca = true;
  
#if defined(OQS_ENABLE_KEM_ledakem_C1_N02)
	kem->method_name = "ledakem_C1_N02";
	kem->length_public_key = OQS_KEM_ledakem_C1_N02_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C1_N02_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C1_N02_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C1_N02_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C1_N03)
	kem->method_name = "ledakem_C1_N03";
	kem->length_public_key = OQS_KEM_ledakem_C1_N03_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C1_N03_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C1_N03_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C1_N03_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C1_N04)
	kem->method_name = "ledakem_C1_N04";
	kem->length_public_key = OQS_KEM_ledakem_C1_N04_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C1_N04_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C1_N04_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C1_N04_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C3_N02)
	kem->method_name = "ledakem_C3_N02";
	kem->length_public_key = OQS_KEM_ledakem_C3_N02_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C3_N02_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C3_N02_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C3_N02_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C3_N03)
	kem->method_name = "ledakem_C3_N03";
	kem->length_public_key = OQS_KEM_ledakem_C3_N03_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C3_N03_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C3_N03_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C3_N03_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C3_N04)
	kem->method_name = "ledakem_C3_N04";
	kem->length_public_key = OQS_KEM_ledakem_C3_N04_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C3_N04_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C3_N04_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C3_N04_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C5_N02)
	kem->method_name = "ledakem_C5_N02";
	kem->length_public_key = OQS_KEM_ledakem_C5_N02_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C5_N02_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C5_N02_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C5_N02_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C5_N03)
	kem->method_name = "ledakem_C5_N03";
	kem->length_public_key = OQS_KEM_ledakem_C5_N03_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C5_N03_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C5_N03_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C5_N03_length_shared_secret;
#elif defined(OQS_ENABLE_KEM_ledakem_C5_N04)
	kem->method_name = "ledakem_C5_N04";
	kem->length_public_key = OQS_KEM_ledakem_C5_N04_length_public_key;
	kem->length_secret_key = OQS_KEM_ledakem_C5_N04_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ledakem_C5_N04_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ledakem_C5_N04_length_shared_secret;
#endif

	kem->keypair = OQS_KEM_ledakem_keypair;
	kem->encaps = OQS_KEM_ledakem_encaps;
	kem->decaps = OQS_KEM_ledakem_decaps;

	return kem;
}

