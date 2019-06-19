#include "api.h"
#include "niederreiter.h"
#include "randombytes.h"
#include "rng.h"

#include <string.h>

static void pack_pk(uint8_t *pk_bytes, publicKeyNiederreiter_t *pk) {
    size_t i;
    for (i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_tobytes(pk_bytes + i * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B,
                pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT);
    }
}

static void unpack_pk(publicKeyNiederreiter_t *pk, const uint8_t *pk_bytes) {
    size_t i;
    for (i = 0; i < N0 - 1; i++) {
        PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_frombytes(pk->Mtr + i * NUM_DIGITS_GF2X_ELEMENT,
                pk_bytes + i * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B);
    }
}

static void pack_ct(uint8_t *sk_bytes, DIGIT *ct) {
    PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_tobytes(sk_bytes, ct);
}

static void unpack_ct(DIGIT *ct, const uint8_t *ct_bytes) {
    PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_frombytes(ct, ct_bytes);
}

static void pack_error(uint8_t *error_bytes, DIGIT *error_digits) {
    size_t i;
    for (i = 0; i < N0; i++) {
        PQCLEAN_LEDAKEMLT12_LEAKTIME_gf2x_tobytes(error_bytes + i * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B,
                error_digits + i * NUM_DIGITS_GF2X_ELEMENT);
    }
}

/* Generates a keypair - pk is the public key and sk is the secret key. */
int PQCLEAN_LEDAKEMLT12_LEAKTIME_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    AES_XOF_struct niederreiter_keys_expander;
    publicKeyNiederreiter_t pk_nie;

    randombytes(((privateKeyNiederreiter_t *)sk)->prng_seed, TRNG_BYTE_LENGTH);
    PQCLEAN_LEDAKEMLT12_LEAKTIME_seedexpander_from_trng(&niederreiter_keys_expander, ((privateKeyNiederreiter_t *)sk)->prng_seed);
    PQCLEAN_LEDAKEMLT12_LEAKTIME_niederreiter_keygen(&pk_nie, (privateKeyNiederreiter_t *) sk, &niederreiter_keys_expander);

    pack_pk(pk, &pk_nie);

    return 0;
}

/* Encrypt - pk is the public key, ct is a key encapsulation message
  (ciphertext), ss is the shared secret.*/
int PQCLEAN_LEDAKEMLT12_LEAKTIME_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
    AES_XOF_struct niederreiter_encap_key_expander;
    unsigned char encapsulated_key_seed[TRNG_BYTE_LENGTH];
    DIGIT error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    uint8_t error_bytes[N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B];
    DIGIT syndrome[NUM_DIGITS_GF2X_ELEMENT];
    publicKeyNiederreiter_t pk_nie;

    randombytes(encapsulated_key_seed, TRNG_BYTE_LENGTH);
    unpack_pk(&pk_nie, pk);

    PQCLEAN_LEDAKEMLT12_LEAKTIME_seedexpander_from_trng(&niederreiter_encap_key_expander, encapsulated_key_seed);
    PQCLEAN_LEDAKEMLT12_LEAKTIME_rand_circulant_blocks_sequence(error_vector, &niederreiter_encap_key_expander);
    pack_error(error_bytes, error_vector);
    HASH_FUNCTION(ss, error_bytes, (N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B));
    PQCLEAN_LEDAKEMLT12_LEAKTIME_niederreiter_encrypt(syndrome, &pk_nie, error_vector);

    pack_ct(ct, syndrome);

    return 0;
}


/* Decrypt - ct is a key encapsulation message (ciphertext), sk is the private
   key, ss is the shared secret */
int PQCLEAN_LEDAKEMLT12_LEAKTIME_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    DIGIT decoded_error_vector[N0 * NUM_DIGITS_GF2X_ELEMENT];
    uint8_t decoded_error_bytes[N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B];
    DIGIT syndrome[NUM_DIGITS_GF2X_ELEMENT];

    unpack_ct(syndrome, ct);
    PQCLEAN_LEDAKEMLT12_LEAKTIME_niederreiter_decrypt(decoded_error_vector, (privateKeyNiederreiter_t *)sk, syndrome);
    pack_error(decoded_error_bytes, decoded_error_vector);
    HASH_FUNCTION(ss, decoded_error_bytes, (N0 * NUM_DIGITS_GF2X_ELEMENT * DIGIT_SIZE_B));

    return 0;
}
