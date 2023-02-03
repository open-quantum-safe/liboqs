// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#define MESSAGE_LEN 50

#define MAGIC_VALUE_BYTES 31
struct magic_value {
    uint8_t val[MAGIC_VALUE_BYTES];
};

/*OQS_STATUS lock_sk_key(OQS_SECRET_KEY *sk) {
    return sk != NULL ? OQS_SUCCESS : OQS_ERROR;
}

OQS_STATUS release_sk_key(OQS_SECRET_KEY *sk) {
    return sk != NULL ? OQS_SUCCESS : OQS_ERROR;
}
static OQS_STATUS do_nothing_save(const OQS_SECRET_KEY *sk) {
    return sk != NULL ? OQS_SUCCESS : OQS_ERROR;
}*/

static OQS_STATUS sig_stfl_test_correctness(void) {

    OQS_SIG_STFL *sig = NULL;
    uint8_t *public_key = NULL;
    OQS_SECRET_KEY *secret_key = NULL;

    uint8_t message[MAGIC_VALUE_BYTES + MESSAGE_LEN + MAGIC_VALUE_BYTES] = {0};

    uint8_t *signature = NULL;
    size_t signature_len = 0;

    OQS_STATUS rc;

    struct magic_value magic;
    OQS_randombytes(magic.val, MAGIC_VALUE_BYTES);

    const char *alg_name = "XMSS-SHA2_10_256";

    sig = OQS_SIG_STFL_new(alg_name);
    if (sig == NULL) {
        fprintf(stderr, "ERROR: OQS_SIG_STFL_new failed\n");
        goto error;
    }

    secret_key = OQS_SECRET_KEY_new(alg_name);
    if (secret_key == NULL) {
        fprintf(stderr, "ERROR: OQS_SECRET_KEY_new failed\n");
        goto error;
    }
    secret_key->lock_key = NULL;
    secret_key->release_key = NULL;
    secret_key->save_secret_key = NULL;

    public_key = malloc(MAGIC_VALUE_BYTES + sig->length_public_key + MAGIC_VALUE_BYTES);
    if (public_key == NULL) {
        fprintf(stderr, "ERROR: malloc failed!\n");
        goto error;
    }
    memcpy(public_key, magic.val, MAGIC_VALUE_BYTES);
    memcpy(public_key + MAGIC_VALUE_BYTES + sig->length_public_key, magic.val, MAGIC_VALUE_BYTES);

    signature = malloc(MAGIC_VALUE_BYTES + sig->length_signature + MAGIC_VALUE_BYTES);
    if (signature == NULL) {
        fprintf(stderr, "ERROR: malloc failed!\n");
        goto error;
    }
    memcpy(signature, magic.val, MAGIC_VALUE_BYTES);
    memcpy(signature + MAGIC_VALUE_BYTES + sig->length_signature, magic.val, MAGIC_VALUE_BYTES);

    rc = OQS_SIG_STFL_keypair(sig, public_key + MAGIC_VALUE_BYTES, secret_key);
    if (rc != OQS_SUCCESS) {
        fprintf(stderr, "ERROR: OQS_SIG_STFL_keypair failed\n");
        goto error;
    }

    memcpy(message, magic.val, MAGIC_VALUE_BYTES);
    OQS_randombytes(message + MAGIC_VALUE_BYTES, MESSAGE_LEN);
    memcpy(message + MAGIC_VALUE_BYTES + MESSAGE_LEN, magic.val, MAGIC_VALUE_BYTES);

    rc = OQS_SIG_STFL_sign(sig, signature + MAGIC_VALUE_BYTES, &signature_len, message + MAGIC_VALUE_BYTES, MESSAGE_LEN, secret_key);
    if (rc != OQS_SUCCESS) {
        fprintf(stderr, "ERROR: OQS_SIG_STFL_sign failed\n");
        goto error;
    }

    rc = OQS_SIG_STFL_verify(sig, message + MAGIC_VALUE_BYTES, MESSAGE_LEN, signature + MAGIC_VALUE_BYTES, signature_len, public_key + MAGIC_VALUE_BYTES);
    if (rc != OQS_SUCCESS) {
        fprintf(stderr, "ERROR: Signature verification error.\n");
        goto error;
    }

    printf("SUCCESS!\n");
    rc = OQS_SUCCESS;
    goto cleanup;

error:
    rc = OQS_ERROR;

cleanup:
    if (public_key) {
        free(public_key);
    }
    if (signature) {
        free(signature);
    }
    if (sig) {
        OQS_SIG_STFL_free(sig);
    }
    if(secret_key) {
        OQS_SECRET_KEY_free(secret_key);
    }

    return rc;
}

int main(void) {
    OQS_STATUS rc = sig_stfl_test_correctness();
    if (rc != OQS_SUCCESS) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

