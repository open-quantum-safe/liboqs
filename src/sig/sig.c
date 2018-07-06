#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include <oqs/oqs.h>

char *OQS_SIG_alg_identifier(size_t i) {
	// EDIT-WHEN-ADDING-SIG
	char *a[OQS_SIG_algs_length] = {
	    OQS_SIG_alg_default,
	    OQS_SIG_alg_picnic_L1_FS,
	    OQS_SIG_alg_picnic_L1_UR,
	    OQS_SIG_alg_picnic_L3_FS,
	    OQS_SIG_alg_picnic_L3_UR,
	    OQS_SIG_alg_picnic_L5_FS,
	    OQS_SIG_alg_picnic_L5_UR,
	    OQS_SIG_alg_qTESLA_I,
	    OQS_SIG_alg_qTESLA_III_size,
	    OQS_SIG_alg_qTESLA_III_speed,
	    OQS_SIG_alg_qTESLA_p_I,
	    OQS_SIG_alg_qTESLA_p_III
	};
	if (i >= OQS_SIG_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}

OQS_SIG *OQS_SIG_new(const char *method_name) {
	if (0 == strcasecmp(method_name, OQS_SIG_alg_default)) {
		return OQS_SIG_new(OQS_SIG_DEFAULT);
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L1_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L1_FS
		return OQS_SIG_picnic_L1_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L1_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L1_UR
		return OQS_SIG_picnic_L1_UR_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L3_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L3_FS
		return OQS_SIG_picnic_L3_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L3_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L3_UR
		return OQS_SIG_picnic_L3_UR_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L5_FS)) {
#ifdef OQS_ENABLE_SIG_picnic_L5_FS
		return OQS_SIG_picnic_L5_FS_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_picnic_L5_UR)) {
#ifdef OQS_ENABLE_SIG_picnic_L5_UR
		return OQS_SIG_picnic_L5_UR_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTESLA_I)) {
#ifdef OQS_ENABLE_SIG_qTESLA_I
		return OQS_SIG_qTESLA_I_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTESLA_III_size)) {
#ifdef OQS_ENABLE_SIG_qTESLA_III_size
		return OQS_SIG_qTESLA_III_size_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTESLA_III_speed)) {
#ifdef OQS_ENABLE_SIG_qTESLA_III_speed
		return OQS_SIG_qTESLA_III_speed_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTESLA_p_I)) {
#ifdef OQS_ENABLE_SIG_qTESLA_p_I
		return OQS_SIG_qTESLA_p_I_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_alg_qTESLA_p_III)) {
#ifdef OQS_ENABLE_SIG_qTESLA_p_III
		return OQS_SIG_qTESLA_p_III_new();
#else
		return NULL;
#endif
		// EDIT-WHEN-ADDING-SIG
	} else {
		return NULL;
	}
}

OQS_STATUS OQS_SIG_keypair(const OQS_SIG *sig, uint8_t *public_key, uint8_t *secret_key) {
  if (sig == NULL || sig->keypair(public_key, secret_key) != 0) {
    return OQS_ERROR;
  } else {
    return OQS_SUCCESS;
  }
}

OQS_STATUS OQS_SIG_sign(const OQS_SIG *sig, uint8_t *signed_message, size_t *signed_message_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
  if (sig == NULL || sig->sign(signed_message, signed_message_len, message, message_len, secret_key) != 0) {
    return OQS_ERROR;
  } else {
    return OQS_SUCCESS;
  }
}

OQS_STATUS OQS_SIG_sign_open(const OQS_SIG *sig, uint8_t *message, size_t *message_len, const uint8_t *signed_message, size_t signed_message_len, const uint8_t *public_key) {
  if (sig == NULL || sig->sign_open(message, message_len, signed_message, signed_message_len, public_key) != 0) {
    return OQS_ERROR;
  } else {
    return OQS_SUCCESS;
  }
}

void OQS_SIG_free(OQS_SIG *sig) {
	OQS_MEM_insecure_free(sig);
}
