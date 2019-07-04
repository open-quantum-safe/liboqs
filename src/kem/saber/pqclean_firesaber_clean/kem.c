#include "SABER_indcpa.h"
#include "SABER_params.h"
#include "fips202.h"
#include "randombytes.h"
#include "verify.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int PQCLEAN_FIRESABER_CLEAN_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    int i;

    // sk[0:SABER_INDCPA_SECRETKEYBYTES-1] <-- sk
    PQCLEAN_FIRESABER_CLEAN_indcpa_kem_keypair(pk, sk);

    // sk[SABER_INDCPA_SECRETKEYBYTES:SABER_INDCPA_SECRETKEYBYTES+SABER_INDCPA_SECRETKEYBYTES-1] <-- pk
    for (i = 0; i < SABER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + SABER_INDCPA_SECRETKEYBYTES] = pk[i];
    }

    // Then hash(pk) is appended.
    sha3_256(sk + SABER_SECRETKEYBYTES - 64, pk, SABER_INDCPA_PUBLICKEYBYTES);

    // Remaining part of sk contains a pseudo-random number.
    // This is output when check in crypto_kem_dec() fails.
    randombytes(sk + SABER_SECRETKEYBYTES - SABER_KEYBYTES, SABER_KEYBYTES );
    return (0);
}

int PQCLEAN_FIRESABER_CLEAN_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
    // Will contain key, coins
    unsigned char kr[64];
    unsigned char buf[64];

    randombytes(buf, 32);

    // BUF[0:31] <-- random message (will be used as the key for client) Note: hash doesnot release system RNG output
    sha3_256(buf, buf, 32);

    // BUF[32:63] <-- Hash(public key);  Multitarget countermeasure for coins + contributory KEM
    sha3_256(buf + 32, pk, SABER_INDCPA_PUBLICKEYBYTES);

    // kr[0:63] <-- Hash(buf[0:63]);
    sha3_512(kr, buf, 64);

    // K^ <-- kr[0:31]
    // noiseseed (r) <-- kr[32:63];
    // buf[0:31] contains message; kr[32:63] contains randomness r;
    PQCLEAN_FIRESABER_CLEAN_indcpa_kem_enc(buf, kr + 32, pk,  ct);

    sha3_256(kr + 32, ct, SABER_BYTES_CCA_DEC);

    // hash concatenation of pre-k and h(c) to k
    sha3_256(ss, kr, 64);

    return (0);
}


int PQCLEAN_FIRESABER_CLEAN_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    int i;
    unsigned char fail;
    unsigned char cmp[SABER_BYTES_CCA_DEC];
    unsigned char buf[64];

    // Will contain key, coins
    unsigned char kr[64];
    const unsigned char *pk = sk + SABER_INDCPA_SECRETKEYBYTES;

    // buf[0:31] <-- message
    PQCLEAN_FIRESABER_CLEAN_indcpa_kem_dec(sk, ct, buf);


    // Multitarget countermeasure for coins + contributory KEM
    // Save hash by storing h(pk) in sk
    for (i = 0; i < 32; i++) {
        buf[32 + i] = sk[SABER_SECRETKEYBYTES - 64 + i];
    }

    sha3_512(kr, buf, 64);

    PQCLEAN_FIRESABER_CLEAN_indcpa_kem_enc(buf, kr + 32, pk, cmp);


    fail = PQCLEAN_FIRESABER_CLEAN_verify(ct, cmp, SABER_BYTES_CCA_DEC);

    // overwrite coins in kr with h(c)
    sha3_256(kr + 32, ct, SABER_BYTES_CCA_DEC);

    PQCLEAN_FIRESABER_CLEAN_cmov(kr, sk + SABER_SECRETKEYBYTES - SABER_KEYBYTES, SABER_KEYBYTES, fail);

    // hash concatenation of pre-k and h(c) to k
    sha3_256(ss, kr, 64);

    return (0);
}
