#include "api.h"
#include "cmov.h"
#include "fips202.h"
#include "owcpa.h"
#include "params.h"
#include "randombytes.h"
#include "sample.h"

// API FUNCTIONS
int PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    uint8_t seed[NTRU_SAMPLE_FG_BYTES];

    randombytes(seed, NTRU_SAMPLE_FG_BYTES);
    PQCLEAN_NTRUHPS2048509_AVX2_owcpa_keypair(pk, sk, seed);

    randombytes(sk + NTRU_OWCPA_SECRETKEYBYTES, NTRU_PRFKEYBYTES);

    return 0;
}

int PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_enc(uint8_t *c, uint8_t *k, const uint8_t *pk) {
    poly r, m;
    uint8_t rm[NTRU_OWCPA_MSGBYTES];
    uint8_t rm_seed[NTRU_SAMPLE_RM_BYTES];

    randombytes(rm_seed, NTRU_SAMPLE_RM_BYTES);

    PQCLEAN_NTRUHPS2048509_AVX2_sample_rm(&r, &m, rm_seed);

    PQCLEAN_NTRUHPS2048509_AVX2_poly_S3_tobytes(rm, &r);
    PQCLEAN_NTRUHPS2048509_AVX2_poly_S3_tobytes(rm + NTRU_PACK_TRINARY_BYTES, &m);
    sha3_256(k, rm, NTRU_OWCPA_MSGBYTES);

    PQCLEAN_NTRUHPS2048509_AVX2_poly_Z3_to_Zq(&r);
    PQCLEAN_NTRUHPS2048509_AVX2_owcpa_enc(c, &r, &m, pk);

    return 0;
}

int PQCLEAN_NTRUHPS2048509_AVX2_crypto_kem_dec(uint8_t *k, const uint8_t *c, const uint8_t *sk) {
    int i, fail;
    uint8_t rm[NTRU_OWCPA_MSGBYTES];
    uint8_t buf[NTRU_PRFKEYBYTES + NTRU_CIPHERTEXTBYTES];

    fail = PQCLEAN_NTRUHPS2048509_AVX2_owcpa_dec(rm, c, sk);
    /* If fail = 0 then c = Enc(h, rm). There is no need to re-encapsulate. */
    /* See comment in PQCLEAN_NTRUHPS2048509_AVX2_owcpa_dec for details.                                */

    sha3_256(k, rm, NTRU_OWCPA_MSGBYTES);

    /* shake(secret PRF key || input ciphertext) */
    for (i = 0; i < NTRU_PRFKEYBYTES; i++) {
        buf[i] = sk[i + NTRU_OWCPA_SECRETKEYBYTES];
    }
    for (i = 0; i < NTRU_CIPHERTEXTBYTES; i++) {
        buf[NTRU_PRFKEYBYTES + i] = c[i];
    }
    sha3_256(rm, buf, NTRU_PRFKEYBYTES + NTRU_CIPHERTEXTBYTES);

    PQCLEAN_NTRUHPS2048509_AVX2_cmov(k, rm, NTRU_SHAREDKEYBYTES, (unsigned char) fail);

    return 0;
}
