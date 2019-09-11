/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: API header file for P610 using compression
*********************************************************************************************/  

#ifndef P610_COMPRESSED_API_H
#define P610_COMPRESSED_API_H
    

/*********************** Key encapsulation mechanism API ***********************/

#define CRYPTO_SECRETKEYBYTES     336      // MSG_BYTES + SECRETKEY_A_BYTES + CRYPTO_PUBLICKEYBYTES bytes
#define CRYPTO_PUBLICKEYBYTES     273      // 3*ORDER_B_ENCODED_BYTES + FP2_ENCODED_BYTES + 2 bytes for shared elligator
#define CRYPTO_BYTES               24
#define CRYPTO_CIPHERTEXTBYTES    297      // COMPRESSED_CHUNK_CT + MSG_BYTES bytes     

// Algorithm name
#define CRYPTO_ALGNAME "SIKEp610_compressed"  

// SIKE's key generation
// It produces a private key sk and computes the public key pk.
// Outputs: secret key sk (CRYPTO_SECRETKEYBYTES = 336 bytes)
//          public key pk (CRYPTO_PUBLICKEYBYTES = 273 bytes) 
int crypto_kem_keypair_SIKEp610_compressed(unsigned char *pk, unsigned char *sk);

// SIKE's encapsulation
// Input:   public key pk         (CRYPTO_PUBLICKEYBYTES = 273 bytes)
// Outputs: shared secret ss      (CRYPTO_BYTES = 24 bytes)
//          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = 297 bytes) 
int crypto_kem_enc_SIKEp610_compressed(unsigned char *ct, unsigned char *ss, const unsigned char *pk);

// SIKE's decapsulation
// Input:   secret key sk         (CRYPTO_SECRETKEYBYTES = 336 bytes)
//          ciphertext message ct (CRYPTO_CIPHERTEXTBYTES = 297 bytes) 
// Outputs: shared secret ss      (CRYPTO_BYTES = 24 bytes)
int crypto_kem_dec_SIKEp610_compressed(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);


// Encoding of keys for KEM-based isogeny system "SIKEp610" (wire format):
// ----------------------------------------------------------------------
// Elements over GF(p610) are encoded in 77 octets in little endian format (i.e., the least significant octet is located in the lowest memory address). 
// Elements (a+b*i) over GF(p610^2), where a and b are defined over GF(p610), are encoded as {a, b}, with a in the lowest memory portion.
//
// Private keys sk consist of the concatenation of a 24-byte random value, a value in the range [0, 2^305-1] and the public key pk. In the SIKE API, 
// private keys are encoded in 336 octets in little endian format. 
// Public keys pk consist of 3 values of length OBOB_BITS, one element in GF(p610^2) and 2 bytes. In the SIKE API, pk is encoded in 273 octets.
// Ciphertexts ct consist of the concatenation of 3 values of length OALICE_BITS, one element in GF(p610^2), 2 bytes and a 24-byte value. In the SIKE API, 
// ct is encoded in 273 + 24 = 297 octets.  
// Shared keys ss consist of a value of 24 octets.


/*********************** Ephemeral key exchange API ***********************/

#define SIDH_SECRETKEYBYTES_A    39
#define SIDH_SECRETKEYBYTES_B    38
#define SIDH_PUBLICKEYBYTES     273
#define SIDH_BYTES              154 

// SECURITY NOTE: SIDH supports ephemeral Diffie-Hellman key exchange. It is NOT secure to use it with static keys.
// See "On the Security of Supersingular Isogeny Cryptosystems", S.D. Galbraith, C. Petit, B. Shani and Y.B. Ti, in ASIACRYPT 2016, 2016.
// Extended version available at: http://eprint.iacr.org/2016/859     

// Generation of Alice's secret key 
// Outputs random value in [0, 2^305 - 1] to be used as Alice's private key
void random_mod_order_A_SIDHp610(unsigned char* random_digits);

// Generation of Bob's secret key 
// Outputs random value in [0, 2^Floor(Log(2,3^192)) - 1] to be used as Bob's private key
void random_mod_order_B_SIDHp610(unsigned char* random_digits);

// Alice's ephemeral public key generation
// Input:  a private key PrivateKeyA in the range [0, 2^305 - 1], stored in 39 bytes. 
// Output: the public key PublicKeyA consisting of 3 GF(p610^2) elements encoded in 273 bytes.
int EphemeralKeyGeneration_A_SIDHp610_Compressed(const unsigned char* PrivateKeyA, unsigned char* PublicKeyA);

// Bob's ephemeral key-pair generation
// It produces a private key PrivateKeyB and computes the public key PublicKeyB.
// The private key is an integer in the range [0, 2^Floor(Log(2,3^192)) - 1], stored in 38 bytes.  
// The public key consists of 3 GF(p610^2) elements encoded in 273 bytes.
int EphemeralKeyGeneration_B_SIDHp610_Compressed(const unsigned char* PrivateKeyB, unsigned char* PublicKeyB);

// Alice's ephemeral shared secret computation
// It produces a shared secret key SharedSecretA using her secret key PrivateKeyA and Bob's public key PublicKeyB
// Inputs: Alice's PrivateKeyA is an integer in the range [0, 2^305 - 1], stored in 39 bytes. 
//         Bob's PublicKeyB consists of 3 GF(p610^2) elements encoded in 273 bytes.
// Output: a shared secret SharedSecretA that consists of one element in GF(p610^2) encoded in 154 bytes.
int EphemeralSecretAgreement_A_SIDHp610_Compressed(const unsigned char* PrivateKeyA, const unsigned char* PublicKeyB, unsigned char* SharedSecretA);

// Bob's ephemeral shared secret computation
// It produces a shared secret key SharedSecretB using his secret key PrivateKeyB and Alice's public key PublicKeyA
// Inputs: Bob's PrivateKeyB is an integer in the range [0, 2^Floor(Log(2,3^192)) - 1], stored in 38 bytes.  
//         Alice's PublicKeyA consists of 3 GF(p610^2) elements encoded in 273 bytes.
// Output: a shared secret SharedSecretB that consists of one element in GF(p610^2) encoded in 154 bytes. 
int EphemeralSecretAgreement_B_SIDHp610_Compressed(const unsigned char* PrivateKeyB, const unsigned char* PublicKeyA, unsigned char* SharedSecretB);


// Encoding of keys for KEX-based isogeny system "SIDHp610" (wire format):
// ----------------------------------------------------------------------
// Elements over GF(p610) are encoded in 77 octets in little endian format (i.e., the least significant octet is located in the lowest memory address). 
// Elements (a+b*i) over GF(p610^2), where a and b are defined over GF(p610), are encoded as {a, b}, with a in the lowest memory portion.
//
// Private keys PrivateKeyA and PrivateKeyB can have values in the range [0, 2^305-1] adn [0, 2^Floor(Log(2,3^192)) - 1], resp. In the SIDH API, 
// Alice's and Bob's private keys are encoded in 39 and 38 octets, resp., in little endian format. 
// Public keys pk consist of 3 values of length OBOB_BITS, one element in GF(p610^2) and 2 bytes. In the SIKE API, pk is encoded in 273 octets.
// Shared keys SharedSecretA and SharedSecretB consist of one element in GF(p610^2). In the SIDH API, they are encoded in 154 octets.


#endif
