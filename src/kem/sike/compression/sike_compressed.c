/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: supersingular isogeny key encapsulation (SIKE) protocol using compression
*********************************************************************************************/

#include <string.h>
#include <oqs/common.h>
#include <oqs/sha3.h>

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk) { // SIKE's key generation using compression
	// Outputs: secret key sk (CRYPTO_SECRETKEYBYTES = MSG_BYTES + SECRETKEY_A_BYTES + CRYPTO_PUBLICKEYBYTES bytes)
	//          public key pk_comp (CRYPTO_PUBLICKEYBYTES bytes)

	// Generate lower portion of secret key sk <- s||SK
	OQS_randombytes(sk, MSG_BYTES);
	random_mod_order_A(sk + MSG_BYTES); // Even random number

	// Generate public key pk
	EphemeralKeyGeneration_A(sk + MSG_BYTES, pk);

	// Append public key pk to secret key sk
	memcpy(&sk[MSG_BYTES + SECRETKEY_A_BYTES], pk, CRYPTO_PUBLICKEYBYTES);

	return 0;
}

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) { // SIKE's encapsulation using compression
	// Input:   public key pk              (CRYPTO_PUBLICKEYBYTES bytes)
	// Outputs: shared secret ss           (CRYPTO_BYTES bytes)
	//          ciphertext message ct      (CRYPTO_CIPHERTEXTBYTES = COMPRESSED_CHUNK_CT + MSG_BYTES bytes)
	unsigned char ephemeralsk[SECRETKEY_B_BYTES] = {0};
	unsigned char jinvariant[FP2_ENCODED_BYTES] = {0};
	unsigned char h[MSG_BYTES];
	unsigned char temp[CRYPTO_CIPHERTEXTBYTES + MSG_BYTES] = {0};

	// Generate ephemeralsk <- G(m||pk) mod oB
	OQS_randombytes(temp, MSG_BYTES);
	memcpy(&temp[MSG_BYTES], pk, CRYPTO_PUBLICKEYBYTES);
	OQS_SHA3_shake256(ephemeralsk, SECRETKEY_B_BYTES, temp, MSG_BYTES + CRYPTO_PUBLICKEYBYTES);
	FormatPrivKey_B(ephemeralsk);

	// Encrypt
	EphemeralKeyGeneration_B(ephemeralsk, ct);
	EphemeralSecretAgreement_B(ephemeralsk, pk, jinvariant);
	OQS_SHA3_shake256(h, MSG_BYTES, jinvariant, FP2_ENCODED_BYTES);
	for (int i = 0; i < MSG_BYTES; i++) {
		ct[i + COMPRESSED_CHUNK_CT] = temp[i] ^ h[i];
	}

	// Generate shared secret ss <- H(m||ct)
	memcpy(&temp[MSG_BYTES], ct, CRYPTO_CIPHERTEXTBYTES);
	OQS_SHA3_shake256(ss, CRYPTO_BYTES, temp, CRYPTO_CIPHERTEXTBYTES + MSG_BYTES);

	return 0;
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) { // SIKE's decapsulation using compression
	// Input:   secret key sk                         (CRYPTO_SECRETKEYBYTES = MSG_BYTES + SECRETKEY_A_BYTES + CRYPTO_PUBLICKEYBYTES bytes)
	//          compressed ciphertext message ct      (CRYPTO_CIPHERTEXTBYTES = COMPRESSED_CHUNK_CT + MSG_BYTES bytes)
	// Outputs: shared secret ss                      (CRYPTO_BYTES bytes)
	unsigned char ephemeralsk_[SECRETKEY_B_BYTES] = {0};
	unsigned char jinvariant_[FP2_ENCODED_BYTES] = {0}, h_[MSG_BYTES];
	unsigned char c0_comp_[COMPRESSED_CHUNK_CT] = {0};
	unsigned char temp[CRYPTO_CIPHERTEXTBYTES + MSG_BYTES] = {0};

	// Decrypt
	EphemeralSecretAgreement_A(sk + MSG_BYTES, ct, jinvariant_);
	OQS_SHA3_shake256(h_, MSG_BYTES, jinvariant_, FP2_ENCODED_BYTES);

	for (int i = 0; i < MSG_BYTES; i++) {
		temp[i] = ct[i + COMPRESSED_CHUNK_CT] ^ h_[i];
	}

	// Generate ephemeralsk_ <- G(m||pk) mod oB
	memcpy(&temp[MSG_BYTES], &sk[MSG_BYTES + SECRETKEY_A_BYTES], CRYPTO_PUBLICKEYBYTES);
	OQS_SHA3_shake256(ephemeralsk_, SECRETKEY_B_BYTES, temp, MSG_BYTES + CRYPTO_PUBLICKEYBYTES);
	FormatPrivKey_B(ephemeralsk_);

	// Generate shared secret ss <- H(m||ct) or output ss <- H(s||ct)
	EphemeralKeyGeneration_B(ephemeralsk_, c0_comp_);

	if (memcmp(c0_comp_, ct, COMPRESSED_CHUNK_CT) != 0) {
		memcpy(temp, sk, MSG_BYTES);
	}

	memcpy(&temp[MSG_BYTES], ct, CRYPTO_CIPHERTEXTBYTES);
	OQS_SHA3_shake256(ss, CRYPTO_BYTES, temp, CRYPTO_CIPHERTEXTBYTES + MSG_BYTES);

	return 0;
}
