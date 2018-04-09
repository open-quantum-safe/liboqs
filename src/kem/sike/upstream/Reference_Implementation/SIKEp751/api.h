//
//  api.h
//
//  Created by Bassham, Lawrence E (Fed) on 9/6/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//


//   This is a sample 'api.h' for use 'sign.c'

#ifndef api_h
#define api_h

#define CRYPTO_SECRETKEYBYTES     644    // MSG_BYTES + SECRETKEY_B_BYTES + CRYPTO_PUBLICKEYBYTES bytes
#define CRYPTO_PUBLICKEYBYTES     564
#define CRYPTO_BYTES               24
#define CRYPTO_CIPHERTEXTBYTES    596    // CRYPTO_PUBLICKEYBYTES + MSG_BYTES bytes

// Algorithm name
#define CRYPTO_ALGNAME "SIKEp751"

// SIKE's key generation
// It produces a private key sk and computes the public key pk.
// Outputs: secret key sk (CRYPTO_SECRETKEYBYTES = 644 bytes)
//          public key pk (CRYPTO_PUBLICKEYBYTES = 564 bytes)
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

// SIKE's encapsulation
// Input:   public key pk         (CRYPTO_PUBLICKEYBYTES = 564 bytes)
// Outputs: shared secret ss      (CRYPTO_BYTES = 24 bytes)
//          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = 596 bytes)
int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

// SIKE's decapsulation
// Input:   secret key sk         (CRYPTO_SECRETKEYBYTES = 644 bytes)
//          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = 596 bytes)
// Outputs: shared secret ss      (CRYPTO_BYTES = 24 bytes)
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#endif /* api_h */
