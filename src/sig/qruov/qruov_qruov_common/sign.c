#include <string.h>
#include "qruov.h"
#include "qrop.h"
#include "util.h"
#include "rng.h"

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{
    SEED_PK seed_pk;
    SEED_SK seed_sk;
    randombytes(seed_sk, SEED_SK_LEN);
    prepare_seed_pk_from_seed(seed_pk, seed_sk);
    qruov_init();
    memcpy(pk, seed_pk, SEED_PK_LEN);
    qruov_keygen(seed_pk, seed_sk, pk + SEED_PK_LEN);
    memcpy(sk, seed_sk, SEED_SK_LEN);
    secure_zero(seed_sk, sizeof(seed_sk));
    return 0;
}

int crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk)
{
    int ret = -1;
    SEED_PK seed_pk;
    SEED_SK seed_sk, seed_y, seed_r, seed_sol;
    SALT sig_r;
    SIG_S sig_s;
    *smlen = 0;
    memcpy(seed_sk, sk, SEED_SK_LEN);
    prepare_seed_pk_from_seed(seed_pk, seed_sk);
    randombytes(seed_y, SEED_SK_LEN);
    randombytes(seed_r, SEED_SK_LEN);
    randombytes(seed_sol, SEED_SK_LEN);
    qruov_init();
    if (qruov_sign(seed_pk, seed_sk, seed_y, seed_r, seed_sol, m, mlen, sig_r, sig_s) != 0) goto cleanup;
    memmove(sm + QRUOV_SIG_LEN, m, mlen);
    memcpy(sm, sig_r, SALT_LEN);
    store_fq(sm + SALT_LEN, sig_s, SIG_S_COEFF_LEN);
    *smlen = QRUOV_SIG_LEN + mlen;
    ret = 0;
cleanup:
    secure_zero(seed_sk, sizeof(seed_sk));
    secure_zero(seed_y, sizeof(seed_y));
    secure_zero(seed_r, sizeof(seed_r));
    secure_zero(seed_sol, sizeof(seed_sol));
    return ret;
}

int crypto_sign_open(unsigned char *m, unsigned long long *mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk)
{
    SEED_PK seed_pk;
    SALT sig_r;
    SIG_S sig_s;
    *mlen = 0;
    if (smlen < QRUOV_SIG_LEN) {
        return -1;
    }
    memcpy(seed_pk, pk, SEED_PK_LEN);
    memcpy(sig_r, sm, SALT_LEN);
    if (!load_fq_checked(sig_s, sm + SALT_LEN, SIG_S_COEFF_LEN)) {
        return -1;
    }
    qruov_init();
    if (!qruov_verify(seed_pk, pk + SEED_PK_LEN, sm + QRUOV_SIG_LEN, smlen - QRUOV_SIG_LEN, sig_r, sig_s)) {
        return -1;
    }
    *mlen = smlen - QRUOV_SIG_LEN;
    memmove(m, sm + QRUOV_SIG_LEN, *mlen);
    return 0;
}
