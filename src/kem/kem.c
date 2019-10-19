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
	    OQS_KEM_alg_bike1_l1_cpa, OQS_KEM_alg_bike1_l3_cpa, OQS_KEM_alg_bike1_l1_fo, OQS_KEM_alg_bike1_l3_fo,
	    ///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALG_IDENTIFIER_START
	    OQS_KEM_alg_kyber_512, OQS_KEM_alg_kyber_768, OQS_KEM_alg_kyber_1024, OQS_KEM_alg_kyber_512_90s, OQS_KEM_alg_kyber_768_90s, OQS_KEM_alg_kyber_1024_90s,
	    OQS_KEM_alg_newhope_512cca, OQS_KEM_alg_newhope_1024cca,
	    OQS_KEM_alg_ntru_hps2048509, OQS_KEM_alg_ntru_hps2048677, OQS_KEM_alg_ntru_hps4096821, OQS_KEM_alg_ntru_hrss701,
	    OQS_KEM_alg_saber_lightsaber, OQS_KEM_alg_saber_saber, OQS_KEM_alg_saber_firesaber,
	    ///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_ALG_IDENTIFIER_END
	    OQS_KEM_alg_frodokem_640_aes, OQS_KEM_alg_frodokem_640_shake, OQS_KEM_alg_frodokem_976_aes, OQS_KEM_alg_frodokem_976_shake, OQS_KEM_alg_frodokem_1344_aes, OQS_KEM_alg_frodokem_1344_shake,
	    OQS_KEM_alg_sidh_p434, OQS_KEM_alg_sidh_p503, OQS_KEM_alg_sidh_p610, OQS_KEM_alg_sidh_p751,
	    OQS_KEM_alg_sidh_p434_compressed, OQS_KEM_alg_sidh_p503_compressed, OQS_KEM_alg_sidh_p610_compressed, OQS_KEM_alg_sidh_p751_compressed,
	    OQS_KEM_alg_sike_p434, OQS_KEM_alg_sike_p503, OQS_KEM_alg_sike_p610, OQS_KEM_alg_sike_p751,
	    OQS_KEM_alg_sike_p434_compressed, OQS_KEM_alg_sike_p503_compressed, OQS_KEM_alg_sike_p610_compressed, OQS_KEM_alg_sike_p751_compressed,
	};
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
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l1_cpa)) {
#ifdef OQS_ENABLE_KEM_bike1_l1_cpa
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l3_cpa)) {
#ifdef OQS_ENABLE_KEM_bike1_l3_cpa
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l1_fo)) {
#ifdef OQS_ENABLE_KEM_bike1_l1_fo
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l3_fo)) {
#ifdef OQS_ENABLE_KEM_bike1_l3_fo
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
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_512_90s)) {
#ifdef OQS_ENABLE_KEM_kyber_512_90s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_768_90s)) {
#ifdef OQS_ENABLE_KEM_kyber_768_90s
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_1024_90s)) {
#ifdef OQS_ENABLE_KEM_kyber_1024_90s
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
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p434_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p434_compressed
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p503_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p503_compressed
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p610_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p610_compressed
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p751_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p751_compressed
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p434_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p434_compressed
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p503_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p503_compressed
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p610_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p610_compressed
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p751_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p751_compressed
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
	if (0 == strcasecmp(method_name, OQS_KEM_alg_default)) {
		return OQS_KEM_new(OQS_KEM_DEFAULT);
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l1_cpa)) {
#ifdef OQS_ENABLE_KEM_bike1_l1_cpa
		return OQS_KEM_bike1_l1_cpa_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l3_cpa)) {
#ifdef OQS_ENABLE_KEM_bike1_l3_cpa
		return OQS_KEM_bike1_l3_cpa_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l1_fo)) {
#ifdef OQS_ENABLE_KEM_bike1_l1_fo
		return OQS_KEM_bike1_l1_fo_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l3_fo)) {
#ifdef OQS_ENABLE_KEM_bike1_l3_fo
		return OQS_KEM_bike1_l3_fo_new();
#else
		return NULL;
#endif
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_NEW_CASE_START
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_512)) {
#ifdef OQS_ENABLE_KEM_kyber_512
		return OQS_KEM_kyber_512_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_768)) {
#ifdef OQS_ENABLE_KEM_kyber_768
		return OQS_KEM_kyber_768_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_1024)) {
#ifdef OQS_ENABLE_KEM_kyber_1024
		return OQS_KEM_kyber_1024_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_512_90s)) {
#ifdef OQS_ENABLE_KEM_kyber_512_90s
		return OQS_KEM_kyber_512_90s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_768_90s)) {
#ifdef OQS_ENABLE_KEM_kyber_768_90s
		return OQS_KEM_kyber_768_90s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber_1024_90s)) {
#ifdef OQS_ENABLE_KEM_kyber_1024_90s
		return OQS_KEM_kyber_1024_90s_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_newhope_512cca)) {
#ifdef OQS_ENABLE_KEM_newhope_512cca
		return OQS_KEM_newhope_512cca_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_newhope_1024cca)) {
#ifdef OQS_ENABLE_KEM_newhope_1024cca
		return OQS_KEM_newhope_1024cca_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hps2048509)) {
#ifdef OQS_ENABLE_KEM_ntru_hps2048509
		return OQS_KEM_ntru_hps2048509_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hps2048677)) {
#ifdef OQS_ENABLE_KEM_ntru_hps2048677
		return OQS_KEM_ntru_hps2048677_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hps4096821)) {
#ifdef OQS_ENABLE_KEM_ntru_hps4096821
		return OQS_KEM_ntru_hps4096821_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ntru_hrss701)) {
#ifdef OQS_ENABLE_KEM_ntru_hrss701
		return OQS_KEM_ntru_hrss701_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_lightsaber)) {
#ifdef OQS_ENABLE_KEM_saber_lightsaber
		return OQS_KEM_saber_lightsaber_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_saber)) {
#ifdef OQS_ENABLE_KEM_saber_saber
		return OQS_KEM_saber_saber_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_firesaber)) {
#ifdef OQS_ENABLE_KEM_saber_firesaber
		return OQS_KEM_saber_firesaber_new();
#else
		return NULL;
#endif
		///// OQS_COPY_FROM_PQCLEAN_FRAGMENT_NEW_CASE_END
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_640_aes)) {
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
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p434)) {
#ifdef OQS_ENABLE_KEM_sidh_p434
		return OQS_KEM_sidh_p434_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p503)) {
#ifdef OQS_ENABLE_KEM_sidh_p503
		return OQS_KEM_sidh_p503_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p610)) {
#ifdef OQS_ENABLE_KEM_sidh_p610
		return OQS_KEM_sidh_p610_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p751)) {
#ifdef OQS_ENABLE_KEM_sidh_p751
		return OQS_KEM_sidh_p751_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p434)) {
#ifdef OQS_ENABLE_KEM_sike_p434
		return OQS_KEM_sike_p434_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p503)) {
#ifdef OQS_ENABLE_KEM_sike_p503
		return OQS_KEM_sike_p503_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p610)) {
#ifdef OQS_ENABLE_KEM_sike_p610
		return OQS_KEM_sike_p610_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p751)) {
#ifdef OQS_ENABLE_KEM_sike_p751
		return OQS_KEM_sike_p751_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p434_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p434_compressed
		return OQS_KEM_sidh_p434_compressed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p503_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p503_compressed
		return OQS_KEM_sidh_p503_compressed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p610_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p610_compressed
		return OQS_KEM_sidh_p610_compressed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sidh_p751_compressed)) {
#ifdef OQS_ENABLE_KEM_sidh_p751_compressed
		return OQS_KEM_sidh_p751_compressed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p434_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p434_compressed
		return OQS_KEM_sike_p434_compressed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p503_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p503_compressed
		return OQS_KEM_sike_p503_compressed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p610_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p610_compressed
		return OQS_KEM_sike_p610_compressed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p751_compressed)) {
#ifdef OQS_ENABLE_KEM_sike_p751_compressed
		return OQS_KEM_sike_p751_compressed_new();
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
