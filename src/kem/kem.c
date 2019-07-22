#include <assert.h>
#include <stdlib.h>
#if defined(_WIN32)
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include <oqs/oqs.h>

OQS_API const char *OQS_KEM_alg_identifier(size_t i) {
	// EDIT-WHEN-ADDING-KEM
	const char *a[OQS_KEM_algs_length] = {
	    OQS_KEM_alg_default,
	    OQS_KEM_alg_frodokem_640_aes, OQS_KEM_alg_frodokem_640_shake, OQS_KEM_alg_frodokem_976_aes, OQS_KEM_alg_frodokem_976_shake, OQS_KEM_alg_frodokem_1344_aes, OQS_KEM_alg_frodokem_1344_shake};
	if (i >= OQS_KEM_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}

OQS_API int OQS_KEM_alg_count() {
	return OQS_KEM_algs_length;
}

OQS_API int OQS_KEM_alg_is_enabled(const char *method_name) {
	if (method_name == NULL) {
		return 0;
	}
	if (0 == strcasecmp(method_name, OQS_KEM_alg_default)) {
		return OQS_KEM_alg_is_enabled(OQS_KEM_DEFAULT);
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l1)) {
#ifdef OQS_ENABLE_KEM_bike1_l1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l3)) {
#ifdef OQS_ENABLE_KEM_bike1_l3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l5)) {
#ifdef OQS_ENABLE_KEM_bike1_l5
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike2_l1)) {
#ifdef OQS_ENABLE_KEM_bike2_l1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike2_l3)) {
#ifdef OQS_ENABLE_KEM_bike2_l3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike2_l5)) {
#ifdef OQS_ENABLE_KEM_bike2_l5
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike3_l1)) {
#ifdef OQS_ENABLE_KEM_bike3_l1
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike3_l3)) {
#ifdef OQS_ENABLE_KEM_bike3_l3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike3_l5)) {
#ifdef OQS_ENABLE_KEM_bike3_l5
		return 1;
#else
		return 0;
#endif
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ENABLED_CASE_START
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_512)) {
#ifdef OQS_ENABLE_KEM_kyber_512
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_768)) {
#ifdef OQS_ENABLE_KEM_kyber_768
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_1024)) {
#ifdef OQS_ENABLE_KEM_kyber_1024
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_newhope_512cca)) {
#ifdef OQS_ENABLE_KEM_newhope_512cca
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_newhope_1024cca)) {
#ifdef OQS_ENABLE_KEM_newhope_1024cca
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hps2048509)) {
#ifdef OQS_ENABLE_KEM_ntru_hps2048509
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hps2048677)) {
#ifdef OQS_ENABLE_KEM_ntru_hps2048677
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hps4096821)) {
#ifdef OQS_ENABLE_KEM_ntru_hps4096821
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hrss701)) {
#ifdef OQS_ENABLE_KEM_ntru_hrss701
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_lightsaber)) {
#ifdef OQS_ENABLE_KEM_saber_lightsaber
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_saber)) {
#ifdef OQS_ENABLE_KEM_saber_saber
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_firesaber)) {
#ifdef OQS_ENABLE_KEM_saber_firesaber
		return 1;
#else
		return 0;
#endif
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ENABLED_CASE_END
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_640_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_640_aes
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_640_shake)) {
#ifdef OQS_ENABLE_KEM_frodokem_640_shake
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_976_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_976_aes
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_976_shake)) {
#ifdef OQS_ENABLE_KEM_frodokem_976_shake
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_1344_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_1344_aes
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_1344_shake)) {
#ifdef OQS_ENABLE_KEM_frodokem_1344_shake
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p434)) {
#ifdef OQS_ENABLE_KEM_sidh_p434
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p503)) {
#ifdef OQS_ENABLE_KEM_sidh_p503
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p610)) {
#ifdef OQS_ENABLE_KEM_sidh_p610
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p751)) {
#ifdef OQS_ENABLE_KEM_sidh_p751
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p434)) {
#ifdef OQS_ENABLE_KEM_sike_p434
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p503)) {
#ifdef OQS_ENABLE_KEM_sike_p503
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p610)) {
#ifdef OQS_ENABLE_KEM_sike_p610
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p751)) {
#ifdef OQS_ENABLE_KEM_sike_p751
		return 1;
#else
		return 0;
#endif
		// EDIT-WHEN-ADDING-KEM
	} else {
		return 0;
	}
}

OQS_API OQS_KEM *OQS_KEM_new(const char *method_name) {
	if (method_name == NULL) {
		return NULL;
	}
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_NEW_CASE_END
	if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_640_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_640_aes
		return OQS_KEM_frodokem_640_aes_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_640_shake)) {
#ifdef OQS_ENABLE_KEM_frodokem_640_shake
		return OQS_KEM_frodokem_640_shake_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_976_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_976_aes
		return OQS_KEM_frodokem_976_aes_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_976_shake)) {
#ifdef OQS_ENABLE_KEM_frodokem_976_shake
		return OQS_KEM_frodokem_976_shake_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_1344_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_1344_aes
		return OQS_KEM_frodokem_1344_aes_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_1344_shake)) {
#ifdef OQS_ENABLE_KEM_frodokem_1344_shake
		return OQS_KEM_frodokem_1344_shake_new();
#else
		return NULL;
#endif
		// EDIT-WHEN-ADDING-KEM
	} else {
		return NULL;
	}
}

OQS_API OQS_STATUS OQS_KEM_keypair(const OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key) {
	if (kem == NULL) {
		return OQS_ERROR;
	} else {
		return kem->keypair(public_key, secret_key);
	}
}

OQS_API OQS_STATUS OQS_KEM_encaps(const OQS_KEM *kem, uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
	if (kem == NULL) {
		return OQS_ERROR;
	} else {
		return kem->encaps(ciphertext, shared_secret, public_key);
	}
}

OQS_API OQS_STATUS OQS_KEM_decaps(const OQS_KEM *kem, uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
	if (kem == NULL) {
		return OQS_ERROR;
	} else {
		return kem->decaps(shared_secret, ciphertext, secret_key);
	}
}

OQS_API void OQS_KEM_free(OQS_KEM *kem) {
	OQS_MEM_insecure_free(kem);
}
