/********************************************************************************************
* Supersingular Isogeny Key Encapsulation Library
*
* Abstract: supersingular isogeny key encapsulation (SIKE) protocol
*********************************************************************************************/ 

#include <string.h>
#include "P751_internal.h"
#include "sha3/fips202.h"


int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{ // SIKE's key generation
  // Outputs: secret key sk (CRYPTO_SECRETKEYBYTES = MSG_BYTES + SECRETKEY_B_BYTES + CRYPTO_PUBLICKEYBYTES bytes)
  //          public key pk (CRYPTO_PUBLICKEYBYTES bytes) 

    // Generate lower portion of secret key sk <- s||SK
    randombytes(sk, MSG_BYTES);
    random_mod_order_B(sk + MSG_BYTES);

    // Generate public key pk
    EphemeralKeyGeneration_B(sk + MSG_BYTES, pk);

    // Append public key pk to secret key sk
    memcpy(&sk[MSG_BYTES + SECRETKEY_B_BYTES], pk, CRYPTO_PUBLICKEYBYTES);

    return 0;
}


int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{ // SIKE's encapsulation
  // Input:   public key pk         (CRYPTO_PUBLICKEYBYTES bytes)
  // Outputs: shared secret ss      (CRYPTO_BYTES bytes)
  //          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = CRYPTO_PUBLICKEYBYTES + MSG_BYTES bytes) 
    const uint16_t G = 0;
    const uint16_t H = 1;
    const uint16_t P = 2;
    unsigned char ephemeralsk[SECRETKEY_A_BYTES];
    unsigned char jinvariant[FP2_ENCODED_BYTES];
    unsigned char h[MSG_BYTES];
    unsigned char temp[CRYPTO_CIPHERTEXTBYTES+MSG_BYTES];
    unsigned int i;

    // Generate ephemeralsk <- G(m||pk) mod oA 
    randombytes(temp, MSG_BYTES);
    memcpy(&temp[MSG_BYTES], pk, CRYPTO_PUBLICKEYBYTES);
    cshake256_simple(ephemeralsk, SECRETKEY_A_BYTES, G, temp, CRYPTO_PUBLICKEYBYTES+MSG_BYTES);
    ephemeralsk[SECRETKEY_A_BYTES - 1] &= MASK_ALICE;

    // Encrypt
    EphemeralKeyGeneration_A(ephemeralsk, ct);
    EphemeralSecretAgreement_A(ephemeralsk, pk, jinvariant);
    cshake256_simple(h, MSG_BYTES, P, jinvariant, FP2_ENCODED_BYTES);
    for (i = 0; i < MSG_BYTES; i++) ct[i + CRYPTO_PUBLICKEYBYTES] = temp[i] ^ h[i];

    // Generate shared secret ss <- H(m||ct)
    memcpy(&temp[MSG_BYTES], ct, CRYPTO_CIPHERTEXTBYTES);
    cshake256_simple(ss, CRYPTO_BYTES, H, temp, CRYPTO_CIPHERTEXTBYTES+MSG_BYTES);

    return 0;
}


int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{ // SIKE's decapsulation
  // Input:   secret key sk         (CRYPTO_SECRETKEYBYTES = MSG_BYTES + SECRETKEY_B_BYTES + CRYPTO_PUBLICKEYBYTES bytes)
  //          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = CRYPTO_PUBLICKEYBYTES + MSG_BYTES bytes) 
  // Outputs: shared secret ss      (CRYPTO_BYTES bytes)
    const uint16_t G = 0;
    const uint16_t H = 1;
    const uint16_t P = 2;
    unsigned char ephemeralsk_[SECRETKEY_A_BYTES];
    unsigned char jinvariant_[FP2_ENCODED_BYTES];
    unsigned char h_[MSG_BYTES];
    unsigned char c0_[CRYPTO_PUBLICKEYBYTES];
    unsigned char temp[CRYPTO_CIPHERTEXTBYTES+MSG_BYTES];
    unsigned int i;

    // Decrypt
    EphemeralSecretAgreement_B(sk + MSG_BYTES, ct, jinvariant_);
    cshake256_simple(h_, MSG_BYTES, P, jinvariant_, FP2_ENCODED_BYTES);
    for (i = 0; i < MSG_BYTES; i++) temp[i] = ct[i + CRYPTO_PUBLICKEYBYTES] ^ h_[i];

    // Generate ephemeralsk_ <- G(m||pk) mod oA
    memcpy(&temp[MSG_BYTES], &sk[MSG_BYTES + SECRETKEY_B_BYTES], CRYPTO_PUBLICKEYBYTES);
    cshake256_simple(ephemeralsk_, SECRETKEY_A_BYTES, G, temp, CRYPTO_PUBLICKEYBYTES+MSG_BYTES);
    ephemeralsk_[SECRETKEY_A_BYTES - 1] &= MASK_ALICE;
    
    // Generate shared secret ss <- H(m||ct) or output ss <- H(s||ct)
    EphemeralKeyGeneration_A(ephemeralsk_, c0_);
    if (memcmp(c0_, ct, CRYPTO_PUBLICKEYBYTES) != 0) {
        memcpy(temp, sk, MSG_BYTES);
    }
    memcpy(&temp[MSG_BYTES], ct, CRYPTO_CIPHERTEXTBYTES);
    cshake256_simple(ss, CRYPTO_BYTES, H, temp, CRYPTO_CIPHERTEXTBYTES+MSG_BYTES);

    return 0;
}