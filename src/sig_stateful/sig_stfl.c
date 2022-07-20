// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include <oqs/oqs.h>

const char *OQS_SIG_STFL_alg_identifier(size_t i) {

    const char *a[OQS_SIG_algs_length] = {

        OQS_SIG_STFL_alg_xmss_sha256_h10,
        OQS_SIG_STFL_alg_xmss_sha256_h16,
        OQS_SIG_STFL_alg_xmss_sha256_h20,
        OQS_SIG_STFL_alg_xmss_sha512_h10,
        OQS_SIG_STFL_alg_xmss_sha512_h16,
        OQS_SIG_STFL_alg_xmss_sha512_h20,
        OQS_SIG_STFL_alg_xmss_shake256_h10,
        OQS_SIG_STFL_alg_xmss_shake256_h16,
        OQS_SIG_STFL_alg_xmss_shake256_h20,
        OQS_SIG_STFL_alg_xmss_shake512_h10,
        OQS_SIG_STFL_alg_xmss_shake512_h16,
        OQS_SIG_STFL_alg_xmss_shake512_h20,
    };

    if (i >= OQS_SIG_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}


int OQS_SIG_alg_count(void) {
    return OQS_SIG_algs_length;
}


int OQS_SIG_alg_is_enabled(const char *method_name) {
    if (method_name == NULL) {
        return 0;
    } 

    if (0) {    

    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H10
        return 1;
#else 
        return 0;
#endif
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H16
        return 1;
#else 
        return 0;
#endif
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H20
        return 1;
#else 
        return 0;
#endif
        // EDIT-WHEN ADDING MORE XMSS/XMSS^MT ALGS
    } else {
        return 0;
    }
}


OQS_SIG_STFL *OQS_SIG_STFL_new(const char *method_name) {
    if (method_name == NULL) {
        return NULL;
    } 

    if (0) {    

    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H10
        return OQS_SIG_STFL_alg_xmss_sha256_h10_new();
#else 
        return NULL;
#endif
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H16
        return OQS_SIG_STFL_alg_xmss_sha256_h16_new();
#else 
        return NULL;
#endif
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H20
        return OQS_SIG_STFL_alg_xmss_sha256_h20_new();
#else 
        return NULL;
#endif
        // EDIT-WHEN ADDING MORE XMSS/XMSS^MT ALGS
    } else {
        return NULL;
    }
}


int OQS_SIG_STFL_keypair(const OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->keypair(public_key, secret_key) != 0) {
		return -1;
	} else {
		return 0;
	}
}

int OQS_SIG_STFL_sign(const OQS_SIG_STFL *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->sign(signature, signature_len, message, message_len, secret_key) != 0) {
		return -1;
	} else {
		return 0;
	}
}

int OQS_SIG_STFL_verify(const OQS_SIG_STFL *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (sig == NULL || sig->verify(message, message_len, signature, signature_len, public_key) != 0) {
		return -1;
	} else {
		return 0;
	}
}


void OQS_SIG_STFL_free(OQS_SIG_STFL *sig) {
    free(sig);
}



// ================================= OQS_SECRET_KEY FUNCTION ===============================================



OQS_SECRET_KEY *OQS_SECRET_KEY_new(const char *method_name) {
    if (method_name == NULL) {
        return NULL;
    } 

    if (0) {    

    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H10
        return OQS_SECRET_KEY_XMSS_SHA256_H10_new();
#else 
        //return NULL;
#endif
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H16
        return OQS_SECRET_KEY_XMSS_SHA256_H16_new();
#else 
        //return NULL;
#endif
    } else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_XMSS_SHA256_H20
        return OQS_SECRET_KEY_XMSS_SHA256_H20_new();
#else 
        //return NULL;
#endif
    } else {
        return NULL;
    }
}


void OQS_SECRET_KEY_free(OQS_SECRET_KEY *sk) {
    free(sk->secret_key);
    free(sk);
}

// ==========================================================================================================