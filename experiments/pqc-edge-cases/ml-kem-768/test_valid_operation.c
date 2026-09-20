/*EC-01 Valid Operation*/
/*author : tanvi medapati*/
#include <oqs/oqs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    OQS_KEM *kem = NULL;
    uint8_t *public_key = NULL;
    uint8_t *secret_key = NULL;
    uint8_t *ciphertext = NULL;
    uint8_t *shared_secret_e = NULL;
    uint8_t *shared_secret_d = NULL;
    int exit_code = EXIT_FAILURE;
    OQS_init();
    kem = OQS_KEM_new(OQS_KEM_alg_ml_kem_768);
    if (kem == NULL) {
        fprintf(stderr, "ERROR : ML-KEM-768 is unavailable.\n");
        goto cleanup;
    }
    public_key = OQS_MEM_malloc(kem->length_public_key);
    secret_key = OQS_MEM_malloc(kem->length_secret_key);
    ciphertext = OQS_MEM_malloc(kem->length_ciphertext);
    shared_secret_e = OQS_MEM_malloc(kem->length_shared_secret);
    shared_secret_d = OQS_MEM_malloc(kem->length_shared_secret);
    if (public_key == NULL ||
        secret_key == NULL ||
        ciphertext == NULL ||
        shared_secret_e == NULL ||
        shared_secret_d == NULL) {
        fprintf(stderr, "ERROR : Memory allocation failed.\n");
        goto cleanup;
    }
    OQS_STATUS rc = OQS_KEM_keypair(
        kem,
        public_key,
        secret_key
    );
    printf("EC-01 keypair return : %d\n", rc);
    if (rc != OQS_SUCCESS) {
        goto cleanup;
    }
    rc = OQS_KEM_encaps(
        kem,
        ciphertext,
        shared_secret_e,
        public_key
    );

    printf("EC-01 encapsulation return : %d\n", rc);

    if (rc != OQS_SUCCESS) {
        goto cleanup;
    }

    rc = OQS_KEM_decaps(
        kem,
        shared_secret_d,
        ciphertext,
        secret_key
    );

    printf("EC-01 decapsulation return : %d\n", rc);

    if (rc != OQS_SUCCESS) {
        goto cleanup;
    }

    if (memcmp(
            shared_secret_e,
            shared_secret_d,
            kem->length_shared_secret
        ) == 0) {
        printf("EC-01 shared secrets : MATCH\n");
        exit_code = EXIT_SUCCESS;
    } else {
        printf("EC-01 shared secrets : MISMATCH\n");
    }

cleanup:
    if (kem != NULL) {
        OQS_MEM_secure_free(secret_key, kem->length_secret_key);
        OQS_MEM_secure_free(shared_secret_e, kem->length_shared_secret);
        OQS_MEM_secure_free(shared_secret_d, kem->length_shared_secret);

        OQS_MEM_insecure_free(public_key);
        OQS_MEM_insecure_free(ciphertext);

        OQS_KEM_free(kem);
    }

    OQS_destroy();

    return exit_code;
}
