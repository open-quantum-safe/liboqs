/**
 * \file kem.h
 * \brief Key encapsulation mechanisms
 *
 * The file `src/kem/example_kem.c` contains two examples on using the OQS_KEM API.
 *
 * The first example uses the individual scheme's algorithms directly and uses
 * no dynamic memory allocation -- all buffers are allocated on the stack, with
 * sizes indicated using preprocessor macros.  Since algorithms can be disabled at
 * compile-time, the programmer should wrap the code in \#ifdefs.
 *
 * The second example uses an OQS_KEM object to use an algorithm specified at
 * runtime.  Therefore it uses dynamic memory allocation -- all buffers must be
 * malloc'ed by the programmer, with sizes indicated using the corresponding length
 * member of the OQS_KEM object in question.  Since algorithms can be disabled at
 * compile-time, the programmer should check that the OQS_KEM object is not `NULL`.
 */

#ifndef __OQS_KEM_H
#define __OQS_KEM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

/** Algorithm identifier for default KEM algorithm. */
#define OQS_KEM_alg_default "DEFAULT"
/** Algorithm identifier for FrodoKEM-640-AES KEM. */
#define OQS_KEM_alg_frodokem_640_aes "FrodoKEM-640-AES"
/** Algorithm identifier for FrodoKEM-640-cSHAKE KEM. */
#define OQS_KEM_alg_frodokem_640_cshake "FrodoKEM-640-cSHAKE"
/** Algorithm identifier for FrodoKEM-976-AES KEM. */
#define OQS_KEM_alg_frodokem_976_aes "FrodoKEM-976-AES"
/** Algorithm identifier for FrodoKEM-976-cSHAKE KEM. */
#define OQS_KEM_alg_frodokem_976_cshake "FrodoKEM-976-cSHAKE"
/** Algorithm identifier for NewHope512-CCA-KEM KEM. */
#define OQS_KEM_alg_newhope_512_cca_kem "NewHope512-CCA-KEM"
/** Algorithm identifier for NewHope1024-CCA-KEM KEM. */
#define OQS_KEM_alg_newhope_1024_cca_kem "NewHope1024-CCA-KEM"
/** Algorithm identifier for Kyber512 KEM. */
#define OQS_KEM_alg_kyber512 "Kyber512"
/** Algorithm identifier for Kyber768 KEM. */
#define OQS_KEM_alg_kyber768 "Kyber768"
/** Algorithm identifier for Kyber1024 KEM. */
#define OQS_KEM_alg_kyber1024 "Kyber1024"
/** Algorithm identifier for BIKE1 L1 KEM. */
#define OQS_KEM_alg_bike1_l1 "BIKE1-L1"
/** Algorithm identifier for BIKE1 L3 KEM. */
#define OQS_KEM_alg_bike1_l3 "BIKE1-L3"
/** Algorithm identifier for BIKE1 L5 KEM. */
#define OQS_KEM_alg_bike1_l5 "BIKE1-L5"
/** Algorithm identifier for BIKE2 L1 KEM. */
#define OQS_KEM_alg_bike2_l1 "BIKE2-L1"
/** Algorithm identifier for BIKE2 L3 KEM. */
#define OQS_KEM_alg_bike2_l3 "BIKE2-L3"
/** Algorithm identifier for BIKE2 L5 KEM. */
#define OQS_KEM_alg_bike2_l5 "BIKE2-L5"
/** Algorithm identifier for BIKE3 L1 KEM. */
#define OQS_KEM_alg_bike3_l1 "BIKE3-L1"
/** Algorithm identifier for BIKE3 L3 KEM. */
#define OQS_KEM_alg_bike3_l3 "BIKE3-L3"
/** Algorithm identifier for BIKE3 L5 KEM. */
#define OQS_KEM_alg_bike3_l5 "BIKE3-L5"
/** Algorithm identifier for Sike p503 KEM. */
#define OQS_KEM_alg_sike_p503 "Sike-p503"
/** Algorithm identifier for Sike p751 KEM. */
#define OQS_KEM_alg_sike_p751 "Sike-p751"
/** Algorithm identifier for BIG_QUAKE_1. */
#define OQS_KEM_alg_BIG_QUAKE_1 "BIG_QUAKE_1"
/** Algorithm identifier for BIG_QUAKE_3. */
#define OQS_KEM_alg_BIG_QUAKE_3 "BIG_QUAKE_3"
/** Algorithm identifier for BIG_QUAKE_5. */
#define OQS_KEM_alg_BIG_QUAKE_5 "BIG_QUAKE_5"
/** Algorithm identifier for LEDA KEM SL=1 N0=2. */
#define OQS_KEM_alg_ledakem_C1_N02 "LEDAKEM_C1_N02"
/** Algorithm identifier for LEDA KEM SL=1 N0=3. */
#define OQS_KEM_alg_ledakem_C1_N03 "LEDAKEM_C1_N03"
/** Algorithm identifier for LEDA KEM SL=1 N0=4. */
#define OQS_KEM_alg_ledakem_C1_N04 "LEDAKEM_C1_N04"
/** Algorithm identifier for LEDA KEM SL=3 N0=2. */
#define OQS_KEM_alg_ledakem_C3_N02 "LEDAKEM_C3_N02"
/** Algorithm identifier for LEDA KEM SL=3 N0=3. */
#define OQS_KEM_alg_ledakem_C3_N03 "LEDAKEM_C3_N03"
/** Algorithm identifier for LEDA KEM SL=3 N0=4. */
#define OQS_KEM_alg_ledakem_C3_N04 "LEDAKEM_C3_N04"
/** Algorithm identifier for LEDA KEM SL=5 N0=2. */
#define OQS_KEM_alg_ledakem_C5_N02 "LEDAKEM_C5_N02"
/** Algorithm identifier for LEDA KEM SL=5 N0=3. */
#define OQS_KEM_alg_ledakem_C5_N03 "LEDAKEM_C5_N03"
/** Algorithm identifier for LEDA KEM SL=5 N0=4. */
#define OQS_KEM_alg_ledakem_C5_N04 "LEDAKEM_C5_N04"
/** Algorithm identifier for LightSaber-KEM. */
#define OQS_KEM_alg_saber_light_saber_kem "LightSaber-KEM"
/** Algorithm identifier for Saber-KEM. */
#define OQS_KEM_alg_saber_saber_kem "Saber-KEM"
/** Algorithm identifier for FireSaber-KEM. */
#define OQS_KEM_alg_saber_fire_saber_kem "FireSaber-KEM"
/** Algorithm identifier for Lima-2p-1024 CCA KEM. */
#define OQS_KEM_alg_lima_2p_1024_cca_kem "Lima-2p-1024-CCA-KEM"
/** Algorithm identifier for Lima-2p-2048 CCA KEM. */
#define OQS_KEM_alg_lima_2p_2048_cca_kem "Lima-2p-2048-CCA-KEM"
/** Algorithm identifier for Lima-sp-1018 CCA KEM. */
#define OQS_KEM_alg_lima_sp_1018_cca_kem "Lima-sp-1018-CCA-KEM"
/** Algorithm identifier for Lima-sp-1306 CCA KEM. */
#define OQS_KEM_alg_lima_sp_1306_cca_kem "Lima-sp-1306-CCA-KEM"
/** Algorithm identifier for Lima-sp-1822 CCA KEM. */
#define OQS_KEM_alg_lima_sp_1822_cca_kem "Lima-sp-1822-CCA-KEM"
/** Algorithm identifier for Lima-sp-2062 CCA KEM. */
#define OQS_KEM_alg_lima_sp_2062_cca_kem "Lima-sp-2062-CCA-KEM"
/** Algorithm identifier for Titanium Std CCA KEM */
#define OQS_KEM_alg_titanium_cca_std_kem "Titanium-CCA-std"
/** Algorithm identifier for Titanium hi CCA KEM */
#define OQS_KEM_alg_titanium_cca_hi_kem "Titanium-CCA-hi"
/** Algorithm identifier for Titanium med CCA KEM */
#define OQS_KEM_alg_titanium_cca_med_kem "Titanium-CCA-med"
/** Algorithm identifier for Titanium supet CCA KEM */
#define OQS_KEM_alg_titanium_cca_super_kem "Titanium-CCA-super"

// EDIT-WHEN-ADDING-KEM
/** Number of algorithm identifiers above. */
#define OQS_KEM_algs_length 46

/**
 * Returns identifiers for available key encapsulation mechanisms in liboqs.  Used with OQS_KEM_new.
 *
 * Note that algorithm identifiers are present in this list even when the algorithm is disabled
 * at compile time.
 *
 * @param[in] i Index of the algorithm identifier to return, 0 <= i < OQS_KEM_algs_length
 * @return Algorithm identifier as a string, or NULL.
 */
OQS_API const char *OQS_KEM_alg_identifier(size_t i);

/**
 * Key encapsulation mechanism object
 */
typedef struct OQS_KEM {

	/** Printable string representing the name of the key encapsulation mechanism. */
	const char *method_name;

	/**
	 * Printable string representing the version of the cryptographic algorithm.
	 *
	 * Implementations with the same method_name and same alg_version will be interoperable.
	 * See README.md for information about algorithm compatibility.
	 */
	const char *alg_version;

	/** The NIST security level (1, 2, 3, 4, 5) claimed in this algorithm's original NIST submission. */
	uint8_t claimed_nist_level;

	/** Whether the KEM offers IND-CCA security (TRUE) or IND-CPA security (FALSE). */
	bool ind_cca;

	/** The (maximum) length, in bytes, of public keys for this KEM. */
	size_t length_public_key;
	/** The (maximum) length, in bytes, of secret keys for this KEM. */
	size_t length_secret_key;
	/** The (maximum) length, in bytes, of ciphertexts for this KEM. */
	size_t length_ciphertext;
	/** The (maximum) length, in bytes, of shared secrets for this KEM. */
	size_t length_shared_secret;

	// clang-format off

	/**
	 * Keypair generation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `public_key` and
	 * `secret_key`, based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_KEM_*_length_*`.
	 *
	 * @param[out] public_key The public key represented as a byte string.
	 * @param[out] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*keypair)(uint8_t *public_key, uint8_t *secret_key);

	/**
	 * Encapsulation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `ciphertext` and
	 * `shared_secret`, based on the `length_*` members in this object or the per-scheme
	 * compile-time macros `OQS_KEM_*_length_*`.
	 *
	 * @param[out] ciphertext The ciphertext (encapsulation) represented as a byte string.
	 * @param[out] shared_secret The shared secret represented as a byte string.
	 * @param[in] public_key The public key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*encaps)(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);

	/**
	 * Decapsulation algorithm.
	 *
	 * Caller is responsible for allocating sufficient memory for `shared_secret`, based
	 * on the `length_*` members in this object or the per-scheme compile-time macros
	 * `OQS_KEM_*_length_*`.
	 *
	 * @param[out] shared_secret The shared secret represented as a byte string.
	 * @param[in] ciphertext The ciphertext (encapsulation) represented as a byte string.
	 * @param[in] secret_key The secret key represented as a byte string.
	 * @return OQS_SUCCESS or OQS_ERROR
	 */
	OQS_STATUS (*decaps)(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);
	// clang-format on

} OQS_KEM;

/**
 * Constructs an OQS_KEM object for a particular algorithm.
 *
 * Callers should always check whether the return value is `NULL`, which indicates either than an
 * invalid algorithm name was provided, or that the requested algorithm was disabled at compile-time.
 *
 * @param[in] method_name Name of the desired algorithm; one of the names in `OQS_KEM_algs`.
 * @return An OQS_KEM for the particular algorithm, or `NULL` if the algorithm has been disabled at compile-time.
 */
OQS_API OQS_KEM *OQS_KEM_new(const char *method_name);

/**
 * Keypair generation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `public_key` and
 * `secret_key`, based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] public_key The public key represented as a byte string.
 * @param[out] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_KEM_keypair(const OQS_KEM *kem, uint8_t *public_key, uint8_t *secret_key);

/**
 * Encapsulation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `ciphertext` and
 * `shared_secret`, based on the `length_*` members in this object or the per-scheme
 * compile-time macros `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] ciphertext The ciphertext (encapsulation) represented as a byte string.
 * @param[out] shared_secret The shared secret represented as a byte string.
 * @param[in] public_key The public key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_KEM_encaps(const OQS_KEM *kem, uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key);

/**
 * Decapsulation algorithm.
 *
 * Caller is responsible for allocating sufficient memory for `shared_secret`, based
 * on the `length_*` members in this object or the per-scheme compile-time macros
 * `OQS_KEM_*_length_*`.
 *
 * @param[in] kem The OQS_KEM object representing the KEM.
 * @param[out] shared_secret The shared secret represented as a byte string.
 * @param[in] ciphertext The ciphertext (encapsulation) represented as a byte string.
 * @param[in] secret_key The secret key represented as a byte string.
 * @return OQS_SUCCESS or OQS_ERROR
 */
OQS_API OQS_STATUS OQS_KEM_decaps(const OQS_KEM *kem, uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key);

/**
 * Frees an OQS_KEM object that was constructed by OQS_KEM_new.
 *
 * @param[in] kem The OQS_KEM object to free.
 */
OQS_API void OQS_KEM_free(OQS_KEM *kem);

#include <oqs/kem_frodokem.h>
#include <oqs/kem_newhopenist.h>
#include <oqs/kem_kyber.h>
#include <oqs/kem_ledakem.h>
#include <oqs/kem_bike.h>
#include <oqs/kem_sike.h>
#include <oqs/kem_BIGQUAKE.h>
#include <oqs/kem_saber.h>
#include <oqs/kem_lima.h>
#include <oqs/kem_titanium.h>
// EDIT-WHEN-ADDING-KEM

#endif // __OQS_KEM_H
