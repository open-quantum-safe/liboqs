/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: API header file for P610 using compression
*********************************************************************************************/

#ifndef P610_COMPRESSED_API_H
#define P610_COMPRESSED_API_H

/*********************** Key encapsulation mechanism API ***********************/

// OQS note: size #defines moved to P610_compressed.c to avoid redefinitions across parameters

// SIKE's key generation
// It produces a private key sk and computes the public key pk.
// Outputs: secret key sk (CRYPTO_SECRETKEYBYTES = 491 bytes)
//          public key pk (CRYPTO_PUBLICKEYBYTES = 274 bytes)
int OQS_KEM_sike_p610_compressed_keypair(unsigned char *pk, unsigned char *sk);

// SIKE's encapsulation
// Input:   public key pk         (CRYPTO_PUBLICKEYBYTES = 274 bytes)
// Outputs: shared secret ss      (CRYPTO_BYTES = 24 bytes)
//          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = 336 bytes)
int OQS_KEM_sike_p610_compressed_encaps(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

// SIKE's decapsulation
// Input:   secret key sk         (CRYPTO_SECRETKEYBYTES = 491 bytes)
//          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = 336 bytes)
// Outputs: shared secret ss      (CRYPTO_BYTES = 24 bytes)
int OQS_KEM_sike_p610_compressed_decaps(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

// Encoding of keys for KEM-based isogeny system "SIKEp610" (wire format):
// ----------------------------------------------------------------------
// Elements over GF(p610) are encoded in 77 octets in little endian format (i.e., the least significant octet is located in the lowest memory address).
// Elements (a+b*i) over GF(p610^2), where a and b are defined over GF(p610), are encoded as {a, b}, with a in the lowest memory portion.
//
// Private keys sk consist of the concatenation of a 24-byte random value, a value in the range [0, 2^305-1] and the public key pk. In the SIKE API,
// private keys are encoded in 491 octets in little endian format.
// Public keys pk consist of 3 values of length OBOB_BITS, one element in GF(p610^2) and 2 bytes. In the SIKE API, pk is encoded in 274 octets.
// Ciphertexts ct consist of the concatenation of 3 values of length OALICE_BITS, one element in GF(p610^2), 2 bytes and a 24-byte value. In the SIKE API,
// ct is encoded in 4*39 + 154 + 2 + 24 = 336 octets.
// Shared keys ss consist of a value of 24 octets.

/*********************** Ephemeral key exchange API ***********************/

// SECURITY NOTE: SIDH supports ephemeral Diffie-Hellman key exchange. It is NOT secure to use it with static keys.
// See "On the Security of Supersingular Isogeny Cryptosystems", S.D. Galbraith, C. Petit, B. Shani and Y.B. Ti, in ASIACRYPT 2016, 2016.
// Extended version available at: http://eprint.iacr.org/2016/859

// Generation of Alice's secret key
// Outputs random value in [0, 2^305 - 1] to be used as Alice's private key
void oqs_kem_sidh_p610_compressed_random_mod_order_A(unsigned char *random_digits);

// Generation of Bob's secret key
// Outputs random value in [0, 2^Floor(Log(2,3^192)) - 1] to be used as Bob's private key
void oqs_kem_sidh_p610_compressed_random_mod_order_B(unsigned char *random_digits);

// Alice's ephemeral public key generation
// Input:  a private key PrivateKeyA in the range [0, 2^305 - 1], stored in 39 bytes.
// Output: the public key PublicKeyA consisting of 3 GF(p610^2) elements encoded in 274 bytes.
int oqs_kem_sidh_p610_compressed_EphemeralKeyGeneration_A(const unsigned char *PrivateKeyA, unsigned char *PublicKeyA);

// Bob's ephemeral key-pair generation
// It produces a private key PrivateKeyB and computes the public key PublicKeyB.
// The private key is an integer in the range [0, 2^Floor(Log(2,3^192)) - 1], stored in 38 bytes.
// The public key consists of 3 GF(p610^2) elements encoded in 274 bytes.
int oqs_kem_sidh_p610_compressed_EphemeralKeyGeneration_B(const unsigned char *PrivateKeyB, unsigned char *PublicKeyB);

// Alice's ephemeral shared secret computation
// It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
// Inputs: Alice's PrivateKeyA is an integer in the range [0, 2^305 - 1], stored in 39 bytes.
//         Bob's PublicKeyB consists of 3 GF(p610^2) elements encoded in 274 bytes.
// Output: a shared secret SharedSecretA that consists of one element in GF(p610^2) encoded in 154 bytes.
int oqs_kem_sidh_p610_compressed_EphemeralSecretAgreement_A(const unsigned char *PrivateKeyA, const unsigned char *PublicKeyB, unsigned char *SharedSecretA);

// Bob's ephemeral shared secret computation
// It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
// Inputs: Bob's PrivateKeyB is an integer in the range [0, 2^Floor(Log(2,3^192)) - 1], stored in 38 bytes.
//         Alice's PublicKeyA consists of 3 GF(p610^2) elements encoded in 274 bytes.
// Output: a shared secret SharedSecretB that consists of one element in GF(p610^2) encoded in 154 bytes.
int oqs_kem_sidh_p610_compressed_EphemeralSecretAgreement_B(const unsigned char *PrivateKeyB, const unsigned char *PublicKeyA, unsigned char *SharedSecretB);

// Encoding of keys for KEX-based isogeny system "SIDHp610" (wire format):
// ----------------------------------------------------------------------
// Elements over GF(p610) are encoded in 77 octets in little endian format (i.e., the least significant octet is located in the lowest memory address).
// Elements (a+b*i) over GF(p610^2), where a and b are defined over GF(p610), are encoded as {a, b}, with a in the lowest memory portion.
//
// Private keys PrivateKeyA and PrivateKeyB can have values in the range [0, 2^305-1] adn [0, 2^Floor(Log(2,3^192)) - 1], resp. In the SIDH API,
// Alice's and Bob's private keys are encoded in 39 and 38 octets, resp., in little endian format.
// Public keys pk consist of 3 values of length OBOB_BITS, one element in GF(p610^2) and 2 bytes. In the SIKE API, pk is encoded in 274 octets.
// Shared keys SharedSecretA and SharedSecretB consist of one element in GF(p610^2). In the SIDH API, they are encoded in 154 octets.

#endif
