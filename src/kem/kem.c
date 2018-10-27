#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include <oqs/oqs.h>

OQS_API const char *OQS_KEM_alg_identifier(size_t i) {
	// EDIT-WHEN-ADDING-KEM
	const char *a[OQS_KEM_algs_length] = {
	    OQS_KEM_alg_default,
	    OQS_KEM_alg_BIG_QUAKE_1, OQS_KEM_alg_BIG_QUAKE_3, OQS_KEM_alg_BIG_QUAKE_5,
	    OQS_KEM_alg_bike1_l1, OQS_KEM_alg_bike1_l3, OQS_KEM_alg_bike1_l5, OQS_KEM_alg_bike2_l1, OQS_KEM_alg_bike2_l3, OQS_KEM_alg_bike2_l5, OQS_KEM_alg_bike3_l1, OQS_KEM_alg_bike3_l3, OQS_KEM_alg_bike3_l5,
	    OQS_KEM_alg_frodokem_640_aes, OQS_KEM_alg_frodokem_640_cshake, OQS_KEM_alg_frodokem_976_aes, OQS_KEM_alg_frodokem_976_cshake,
	    OQS_KEM_alg_kyber512, OQS_KEM_alg_kyber768, OQS_KEM_alg_kyber1024,
	    OQS_KEM_alg_ledakem_C1_N02, OQS_KEM_alg_ledakem_C1_N03, OQS_KEM_alg_ledakem_C1_N04, OQS_KEM_alg_ledakem_C3_N02, OQS_KEM_alg_ledakem_C3_N03, OQS_KEM_alg_ledakem_C3_N04, OQS_KEM_alg_ledakem_C5_N02, OQS_KEM_alg_ledakem_C5_N03, OQS_KEM_alg_ledakem_C5_N04,
	    OQS_KEM_alg_lima_2p_1024_cca_kem, OQS_KEM_alg_lima_2p_2048_cca_kem, OQS_KEM_alg_lima_sp_1018_cca_kem, OQS_KEM_alg_lima_sp_1306_cca_kem, OQS_KEM_alg_lima_sp_1822_cca_kem, OQS_KEM_alg_lima_sp_2062_cca_kem,
	    OQS_KEM_alg_newhope_512_cca_kem, OQS_KEM_alg_newhope_1024_cca_kem,
	    OQS_KEM_alg_saber_light_saber_kem, OQS_KEM_alg_saber_saber_kem, OQS_KEM_alg_saber_fire_saber_kem,
	    OQS_KEM_alg_sike_p503, OQS_KEM_alg_sike_p751,
	    OQS_KEM_alg_titanium_cca_std_kem, OQS_KEM_alg_titanium_cca_hi_kem, OQS_KEM_alg_titanium_cca_med_kem, OQS_KEM_alg_titanium_cca_super_kem};
	if (i >= OQS_KEM_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}

OQS_API OQS_KEM *OQS_KEM_new(const char *method_name) {
	if (0 == strcasecmp(method_name, OQS_KEM_alg_default)) {
		return OQS_KEM_new(OQS_KEM_DEFAULT);
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_BIG_QUAKE_1)) {
#ifdef OQS_ENABLE_KEM_BIG_QUAKE_1
		return OQS_KEM_BIG_QUAKE_1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_BIG_QUAKE_3)) {
#ifdef OQS_ENABLE_KEM_BIG_QUAKE_3
		return OQS_KEM_BIG_QUAKE_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_BIG_QUAKE_5)) {
#ifdef OQS_ENABLE_KEM_BIG_QUAKE_5
		return OQS_KEM_BIG_QUAKE_5_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l1)) {
#ifdef OQS_ENABLE_KEM_bike1_l1
		return OQS_KEM_bike1_l1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l3)) {
#ifdef OQS_ENABLE_KEM_bike1_l3
		return OQS_KEM_bike1_l3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike1_l5)) {
#ifdef OQS_ENABLE_KEM_bike1_l5
		return OQS_KEM_bike1_l5_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike2_l1)) {
#ifdef OQS_ENABLE_KEM_bike2_l1
		return OQS_KEM_bike2_l1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike2_l3)) {
#ifdef OQS_ENABLE_KEM_bike2_l3
		return OQS_KEM_bike2_l3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike2_l5)) {
#ifdef OQS_ENABLE_KEM_bike2_l5
		return OQS_KEM_bike2_l5_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike3_l1)) {
#ifdef OQS_ENABLE_KEM_bike3_l1
		return OQS_KEM_bike3_l1_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike3_l3)) {
#ifdef OQS_ENABLE_KEM_bike3_l3
		return OQS_KEM_bike3_l3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_bike3_l5)) {
#ifdef OQS_ENABLE_KEM_bike3_l5
		return OQS_KEM_bike3_l5_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_640_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_640_aes
		return OQS_KEM_frodokem_640_aes_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_640_cshake)) {
#ifdef OQS_ENABLE_KEM_frodokem_640_cshake
		return OQS_KEM_frodokem_640_cshake_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_976_aes)) {
#ifdef OQS_ENABLE_KEM_frodokem_976_aes
		return OQS_KEM_frodokem_976_aes_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_frodokem_976_cshake)) {
#ifdef OQS_ENABLE_KEM_frodokem_976_cshake
		return OQS_KEM_frodokem_976_cshake_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber512)) {
#ifdef OQS_ENABLE_KEM_kyber512
		return OQS_KEM_kyber512_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber768)) {
#ifdef OQS_ENABLE_KEM_kyber768
		return OQS_KEM_kyber768_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_kyber1024)) {
#ifdef OQS_ENABLE_KEM_kyber1024
		return OQS_KEM_kyber1024_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C1_N02)) {
#if defined(OQS_ENABLE_KEM_ledakem_C1_N02)
		return OQS_KEM_ledakem_C1_N02_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C1_N03)) {
#if defined(OQS_ENABLE_KEM_ledakem_C1_N03)
		return OQS_KEM_ledakem_C1_N03_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C1_N04)) {
#if defined(OQS_ENABLE_KEM_ledakem_C1_N04)
		return OQS_KEM_ledakem_C1_N04_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C3_N02)) {
#if defined(OQS_ENABLE_KEM_ledakem_C3_N02)
		return OQS_KEM_ledakem_C3_N02_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C3_N03)) {
#if defined(OQS_ENABLE_KEM_ledakem_C3_N03)
		return OQS_KEM_ledakem_C3_N03_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C3_N04)) {
#if defined(OQS_ENABLE_KEM_ledakem_C3_N04)
		return OQS_KEM_ledakem_C3_N04_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C5_N02)) {
#ifdef OQS_ENABLE_KEM_ledakem_C5_N02
		return OQS_KEM_ledakem_C5_N02_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C5_N03)) {
#if defined(OQS_ENABLE_KEM_ledakem_C5_N03)
		return OQS_KEM_ledakem_C5_N03_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_ledakem_C5_N04)) {
#if defined(OQS_ENABLE_KEM_ledakem_C5_N04)
		return OQS_KEM_ledakem_C5_N04_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_lima_2p_1024_cca_kem)) {
#ifdef OQS_ENABLE_KEM_lima_2p_1024_cca_kem
		return OQS_KEM_lima_2p_1024_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_lima_2p_2048_cca_kem)) {
#ifdef OQS_ENABLE_KEM_lima_2p_2048_cca_kem
		return OQS_KEM_lima_2p_2048_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_lima_sp_1018_cca_kem)) {
#ifdef OQS_ENABLE_KEM_lima_sp_1018_cca_kem
		return OQS_KEM_lima_sp_1018_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_lima_sp_1306_cca_kem)) {
#ifdef OQS_ENABLE_KEM_lima_sp_1306_cca_kem
		return OQS_KEM_lima_sp_1306_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_lima_sp_1822_cca_kem)) {
#ifdef OQS_ENABLE_KEM_lima_sp_1822_cca_kem
		return OQS_KEM_lima_sp_1822_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_lima_sp_2062_cca_kem)) {
#ifdef OQS_ENABLE_KEM_lima_sp_2062_cca_kem
		return OQS_KEM_lima_sp_2062_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_newhope_512_cca_kem)) {
#ifdef OQS_ENABLE_KEM_newhope_512_cca_kem
		return OQS_KEM_newhope_512_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_newhope_1024_cca_kem)) {
#ifdef OQS_ENABLE_KEM_newhope_1024_cca_kem
		return OQS_KEM_newhope_1024_cca_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p503)) {
#ifdef OQS_ENABLE_KEM_sike_p503
		return OQS_KEM_sike_p503_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_sike_p751)) {
#ifdef OQS_ENABLE_KEM_sike_p751
		return OQS_KEM_sike_p751_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_light_saber_kem)) {
#ifdef OQS_ENABLE_KEM_saber_light_saber_kem
		return OQS_KEM_saber_light_saber_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_saber_kem)) {
#ifdef OQS_ENABLE_KEM_saber_saber_kem
		return OQS_KEM_saber_saber_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_saber_fire_saber_kem)) {
#ifdef OQS_ENABLE_KEM_saber_fire_saber_kem
		return OQS_KEM_saber_fire_saber_kem_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_titanium_cca_std_kem)) {
#ifdef OQS_ENABLE_KEM_titanium_cca_std_kem
		return OQS_KEM_titanium_cca_std_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_titanium_cca_med_kem)) {
#ifdef OQS_ENABLE_KEM_titanium_cca_med_kem
		return OQS_KEM_titanium_cca_med_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_titanium_cca_super_kem)) {
#ifdef OQS_ENABLE_KEM_titanium_cca_super_kem
		return OQS_KEM_titanium_cca_super_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_KEM_alg_titanium_cca_hi_kem)) {
#ifdef OQS_ENABLE_KEM_titanium_cca_hi_kem
		return OQS_KEM_titanium_cca_hi_new();
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
