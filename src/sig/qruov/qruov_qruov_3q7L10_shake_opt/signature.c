#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "qruov.h"
#include "qrop.h"
#include "rng.h"
#include "util.h"

int crypto_sign_signature(unsigned char *sig,
                          size_t *siglen,
                          const unsigned char *m,
                          size_t mlen,
                          const unsigned char *sk)
{
    int ret = -1;

    SEED_PK seed_pk;
    SEED_SK seed_sk, seed_y, seed_r, seed_sol;
    SALT sig_r;
    SIG_S sig_s;

    *siglen = 0;

    memcpy(seed_sk, sk, SEED_SK_LEN);
    prepare_seed_pk_from_seed(seed_pk, seed_sk);

    randombytes(seed_y, SEED_SK_LEN);
    randombytes(seed_r, SEED_SK_LEN);
    randombytes(seed_sol, SEED_SK_LEN);

    qruov_init();

    if (qruov_sign(seed_pk, seed_sk,
                   seed_y, seed_r, seed_sol,
                   m, mlen,
                   sig_r, sig_s) != 0) {
        goto cleanup;
    }

    memcpy(sig, sig_r, SALT_LEN);
    store_fq(sig + SALT_LEN, sig_s, SIG_S_COEFF_LEN);

    *siglen = QRUOV_SIG_LEN;
    ret = 0;

cleanup:
    secure_zero(seed_sk, sizeof(seed_sk));
    secure_zero(seed_y, sizeof(seed_y));
    secure_zero(seed_r, sizeof(seed_r));
    secure_zero(seed_sol, sizeof(seed_sol));

    return ret;
}

int crypto_sign_verify(const unsigned char *sig,
                       size_t siglen,
                       const unsigned char *m,
                       size_t mlen,
                       const unsigned char *pk)
{
    SEED_PK seed_pk;
    SALT sig_r;
    SIG_S sig_s;

    if (siglen != QRUOV_SIG_LEN) {
        return -1;
    }

    memcpy(seed_pk, pk, SEED_PK_LEN);
    memcpy(sig_r, sig, SALT_LEN);

    if (!load_fq_checked(sig_s,
                         sig + SALT_LEN,
                         SIG_S_COEFF_LEN)) {
        return -1;
    }

    qruov_init();

    return qruov_verify(seed_pk,
                        pk + SEED_PK_LEN,
                        m,
                        mlen,
                        sig_r,
                        sig_s) ? 0 : -1;
}
