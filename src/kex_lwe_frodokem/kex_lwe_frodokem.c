#include <oqs/kex_lwe_frodokem.h>
#include <oqs/common.h>

OQS_KEX *OQS_KEX_lwe_frodokem_640_aes_new() {

    OQS_KEX *kex = malloc(sizeof(OQS_KEX));
    if (kex == NULL) {
        return NULL;
    }
    kex->rand = NULL;
    kex->method_name = "LWE FrodoKEM-640-AES";
	kex->estimated_classical_security = 128;
	kex->estimated_quantum_security = 64;
	kex->seed = NULL;
	kex->seed_len = 0;
	kex->named_parameters = "640 AES";
	kex->params = NULL;
	kex->ctx = NULL;
	kex->alice_0 = OQS_KEX_lwe_frodokem_640_aes_alice_0;
	kex->bob = OQS_KEX_lwe_frodokem_640_aes_bob;
	kex->alice_1 = OQS_KEX_lwe_frodokem_640_aes_alice_1;
	kex->alice_priv_free = OQS_KEX_lwe_frodokem_alice_priv_free;
	kex->free = OQS_KEX_lwe_frodokem_free;
	return kex;
}

// FrodoKEM-640-AES wrapper

#include "upstream/api_frodo640.h"
int oqs_kex_lwe_frodokem_640_aes_keypair(unsigned char *pk, unsigned char *sk);
int oqs_kex_lwe_frodokem_640_aes_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int oqs_kex_lwe_frodokem_640_aes_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

int OQS_KEX_lwe_frodokem_640_aes_alice_0(UNUSED OQS_KEX *k, void **alice_priv, UNUSED uint8_t **alice_msg, UNUSED size_t *alice_msg_len) {
    *alice_priv = NULL;
    *alice_msg = NULL;
    int ret = OQS_ERROR;
    *alice_priv = malloc(CRYPTO_SECRETKEYBYTES);
    if (*alice_priv == NULL) {
        goto err;
    }
    *alice_msg = malloc(CRYPTO_PUBLICKEYBYTES);
    if (*alice_msg == NULL) {
        goto err;
    }
    ret = oqs_kex_lwe_frodokem_640_aes_keypair(*alice_msg, *alice_priv);
    if (ret == 0) {
        *alice_msg_len = CRYPTO_PUBLICKEYBYTES;
        ret = OQS_SUCCESS;
        goto done;
    } else {
        ret = OQS_ERROR;
        goto err;
    }
err:
    free(*alice_priv);
    free(*alice_msg);
done:
    return ret;
}

int OQS_KEX_lwe_frodokem_640_aes_bob(UNUSED OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len) {
    if (alice_msg_len != CRYPTO_PUBLICKEYBYTES) {
        return OQS_ERROR;
    }
    *bob_msg = NULL;
    *key = NULL;
    int ret = OQS_ERROR;
    *bob_msg = malloc(CRYPTO_CIPHERTEXTBYTES);
    if (*bob_msg == NULL) {
        goto err;
    }
    *key = malloc(CRYPTO_BYTES);
    if (*key == NULL) {
        goto err;
    }
    ret = oqs_kex_lwe_frodokem_640_aes_enc(*bob_msg, *key, alice_msg);
    if (ret == 0) {
        *bob_msg_len = CRYPTO_CIPHERTEXTBYTES;
        *key_len = CRYPTO_BYTES;
        ret = OQS_SUCCESS;
        goto done;
    } else {
        ret = OQS_ERROR;
        goto err;
    }
err:
    free(*bob_msg);
    free(*key);
done:
    return ret;
}

int OQS_KEX_lwe_frodokem_640_aes_alice_1(UNUSED OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len) {
    if (bob_msg_len != CRYPTO_CIPHERTEXTBYTES) {
        return OQS_ERROR;
    }
    *key = NULL;
    int ret = OQS_ERROR;
    *key = malloc(CRYPTO_BYTES);
    if (*key == NULL) {
        goto err;
    }
    ret = oqs_kex_lwe_frodokem_640_aes_dec(*key, bob_msg, alice_priv);
    if (ret == 0) {
        *key_len = CRYPTO_BYTES;
        ret = OQS_SUCCESS;
        goto done;
    } else {
        ret = OQS_ERROR;
        goto err;
    }
err:
    free(*key);
done:
    return ret;
}

// FrodoKEM-640-cSHAKE wrapper

#include "upstream/api_frodo640.h"
int oqs_kex_lwe_frodokem_640_cshake_keypair(unsigned char *pk, unsigned char *sk);
int oqs_kex_lwe_frodokem_640_cshake_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
int oqs_kex_lwe_frodokem_640_cshake_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

OQS_KEX *OQS_KEX_lwe_frodokem_640_cshake_new() {

    OQS_KEX *kex = malloc(sizeof(OQS_KEX));
    if (kex == NULL) {
        return NULL;
    }
    kex->rand = NULL;
    kex->method_name = "LWE FrodoKEM-640-cSHAKE";
	kex->estimated_classical_security = 128;
	kex->estimated_quantum_security = 64;
	kex->seed = NULL;
	kex->seed_len = 0;
	kex->named_parameters = "640 cSHAKE";
	kex->params = NULL;
	kex->ctx = NULL;
	kex->alice_0 = OQS_KEX_lwe_frodokem_640_cshake_alice_0;
	kex->bob = OQS_KEX_lwe_frodokem_640_cshake_bob;
	kex->alice_1 = OQS_KEX_lwe_frodokem_640_cshake_alice_1;
	kex->alice_priv_free = OQS_KEX_lwe_frodokem_alice_priv_free;
	kex->free = OQS_KEX_lwe_frodokem_free;
	return kex;
}

int OQS_KEX_lwe_frodokem_640_cshake_alice_0(UNUSED OQS_KEX *k, UNUSED void **alice_priv, UNUSED uint8_t **alice_msg, UNUSED size_t *alice_msg_len) {
    *alice_priv = NULL;
    *alice_msg = NULL;
    int ret = OQS_ERROR;
    *alice_priv = malloc(CRYPTO_SECRETKEYBYTES);
    if (*alice_priv == NULL) {
        goto err;
    }
    *alice_msg = malloc(CRYPTO_PUBLICKEYBYTES);
    if (*alice_msg == NULL) {
        goto err;
    }
    ret = oqs_kex_lwe_frodokem_640_cshake_keypair(*alice_msg, *alice_priv);
    if (ret == 0) {
        *alice_msg_len = CRYPTO_PUBLICKEYBYTES;
        ret = OQS_SUCCESS;
        goto done;
    } else {
        ret = OQS_ERROR;
        goto err;
    }
err:
    free(*alice_priv);
    free(*alice_msg);
done:
    return ret;
}

int OQS_KEX_lwe_frodokem_640_cshake_bob(UNUSED OQS_KEX *k, UNUSED const uint8_t *alice_msg, UNUSED const size_t alice_msg_len, UNUSED uint8_t **bob_msg, UNUSED size_t *bob_msg_len, UNUSED uint8_t **key, UNUSED size_t *key_len) {
    if (alice_msg_len != CRYPTO_PUBLICKEYBYTES) {
        return OQS_ERROR;
    }
    *bob_msg = NULL;
    *key = NULL;
    int ret = OQS_ERROR;
    *bob_msg = malloc(CRYPTO_CIPHERTEXTBYTES);
    if (*bob_msg == NULL) {
        goto err;
    }
    *key = malloc(CRYPTO_BYTES);
    if (*key == NULL) {
        goto err;
    }
    ret = oqs_kex_lwe_frodokem_640_cshake_enc(*bob_msg, *key, alice_msg);
    if (ret == 0) {
        *bob_msg_len = CRYPTO_CIPHERTEXTBYTES;
        *key_len = CRYPTO_BYTES;
        ret = OQS_SUCCESS;
        goto done;
    } else {
        ret = OQS_ERROR;
        goto err;
    }
err:
    free(*bob_msg);
    free(*key);
done:
    return ret;
}

int OQS_KEX_lwe_frodokem_640_cshake_alice_1(UNUSED OQS_KEX *k, UNUSED const void *alice_priv, UNUSED const uint8_t *bob_msg, UNUSED const size_t bob_msg_len, UNUSED uint8_t **key, UNUSED size_t *key_len) {
    if (bob_msg_len != CRYPTO_CIPHERTEXTBYTES) {
        return OQS_ERROR;
    }
    *key = NULL;
    int ret = OQS_ERROR;
    *key = malloc(CRYPTO_BYTES);
    if (*key == NULL) {
        goto err;
    }
    ret = oqs_kex_lwe_frodokem_640_cshake_dec(*key, bob_msg, alice_priv);
    if (ret == 0) {
        *key_len = CRYPTO_BYTES;
        ret = OQS_SUCCESS;
        goto done;
    } else {
        ret = OQS_ERROR;
        goto err;
    }
err:
    free(*key);
done:
    return ret;
}

void OQS_KEX_lwe_frodokem_alice_priv_free(UNUSED OQS_KEX *k, void *alice_priv) {
    free(alice_priv);
}

void OQS_KEX_lwe_frodokem_free(OQS_KEX *k) {
    free(k);
}
