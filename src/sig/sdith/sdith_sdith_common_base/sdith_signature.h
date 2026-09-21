#ifndef SDITH_SIGNATURE_H
#define SDITH_SIGNATURE_H

// This header contains the main SDiTH signature

#include "commons.h"

/** signature main parameters */
typedef struct signature_parameters_t {
  uint64_t lambda;
  uint64_t kappa;
  uint64_t tau;
  uint64_t target_topen;
  uint64_t proofow_w;
  uint64_t rsd_w;
  uint64_t rsd_n;
  uint64_t mux_depth;
  uint64_t mux_arities[8];
  proofow_variant_t proofow_variant;
} signature_parameters;

/** Default parameter sets */
EXPORT_DECL const signature_parameters CAT1_SHORT_PARAMETERS;
EXPORT_DECL const signature_parameters CAT3_SHORT_PARAMETERS;
EXPORT_DECL const signature_parameters CAT5_SHORT_PARAMETERS;
EXPORT_DECL const signature_parameters CAT1_FAST_PARAMETERS;
EXPORT_DECL const signature_parameters CAT3_FAST_PARAMETERS;
EXPORT_DECL const signature_parameters CAT5_FAST_PARAMETERS;
/** Default parameter sets for official variants */
EXPORT_DECL const signature_parameters CAT1_SHORT_CIPHERPOW_PARAMETERS;
EXPORT_DECL const signature_parameters CAT3_SHORT_CIPHERPOW_PARAMETERS;
EXPORT_DECL const signature_parameters CAT5_SHORT_CIPHERPOW_PARAMETERS;
EXPORT_DECL const signature_parameters CAT1_FAST_CIPHERPOW_PARAMETERS;
EXPORT_DECL const signature_parameters CAT3_FAST_CIPHERPOW_PARAMETERS;
EXPORT_DECL const signature_parameters CAT5_FAST_CIPHERPOW_PARAMETERS;

/** @brief Number of bytes of the secret key (in the sense keypair) */
EXPORT uint64_t sdith_secret_key_bytes(const signature_parameters* sig_params);
/** @brief Number of bytes of the public key */
EXPORT uint64_t sdith_public_key_bytes(const signature_parameters* sig_params);
/** @brief Number of bytes of a sdith signature */
EXPORT uint64_t sdith_signature_bytes(const signature_parameters* sig_params);

/**
 * @brief Number of pure entropy bytes to provide to the keygen algorithm
 * These bytes should be generated using a secure random generator, a random device, or something like /dev/urandom.
 * During tests purposes, fixing the entropy makes the algorithm deterministic
 */
EXPORT uint64_t sdith_keygen_entropy_bytes(const signature_parameters* sig_params);

/**
 * @brief size of the scratch space to pass to the keygen function
 * The signature function does not use any heap allocation: temporaries that are too large to fit
 * on the stack are going to be allocated in this scratch space.
 */
EXPORT uint64_t sdith_keygen_tmp_bytes(const signature_parameters* sig_params);

/**
 * @brief SDiTH Key generator
 * @param sig_params parameter set
 * @param skey        [out] secret key (i.e. keypair)
 * @param pkey        [out] public key (i.e. keypair)
 * @param entropy     [in] entropy bytes
 * @param tmp_space   [tmp] scratch space -- holds secret key material on
 *                    return (the raw solution): the caller must scrub it.
 *
 * Constant-time with respect to the generated secret key, save for the
 * rejection loop of the position sampler (whose trip count is independent of
 * the accepted positions). This holds only for the avx parameter table: the ref
 * ct helpers are the readable definitions, not hardened ones.
 */
EXPORT void sdith_keygen(const signature_parameters* sig_params,  //
                         void* skey,                              // out: secret key
                         void* pkey,                              // out: public key
                         const void* entropy,                     // in: entropy
                         uint8_t* tmp_space);

/**
 * @brief Number of pure entropy bytes to provide to the signature algorithm
 * These bytes should be generated using a secure random generator, a random device, or something like /dev/urandom.
 * During tests purposes, fixing the entropy makes the algorithm deterministic
 */
EXPORT uint64_t sdith_signature_entropy_bytes(const signature_parameters* sig_params);

/**
 * @brief size of the scratch space to pass to the signature function
 * The signature function does not use any heap allocation: temporaries that are too large to fit
 * on the stack are going to be allocated in this scratch space.
 */
EXPORT uint64_t sdith_signature_tmp_bytes(const signature_parameters* sig_params);

/**
 * SDiTH signature
 * @param sig_params    parameter set
 * @param signature     [out] digital signature
 * @param message       [in] message to sign
 * @param message_bytes [in] number of bytes of the message
 * @param skey          [in] secret key
 * @param entropy       [in] entropy bytes
 * @param tmp_space     [tmp] scratch space
 */
EXPORT void sdith_sign(const signature_parameters* sig_params,       //
                       void* signature,                              // out: signature
                       const void* message, uint64_t message_bytes,  // in: message
                       const void* skey,                             // secret key
                       const void* entropy,                          // in: entropy
                       uint8_t* tmp_space);

/**
 * @brief size of the scratch space to pass to the signature function
 * The signature function does not use any heap allocation: temporaries that are too large to fit
 * on the stack are going to be allocated in this scratch space.
 */
EXPORT uint64_t sdith_verify_tmp_bytes(const signature_parameters* sig_params);

/**
 * Signature verification function
 * @param sig_params SDiTH parameter set
 * @param signature     [in] signature to verify
 * @param message       [in] message that was signed
 * @param message_bytes [in] size of the message
 * @param pkey          [in] public key
 * @param tmp_space     [tmp] scratch space
 * @return
 */
EXPORT uint8_t sdith_verify(const signature_parameters* sig_params,       //
                            const void* signature,                        // out: signature
                            const void* message, uint64_t message_bytes,  // in: message
                            const void* pkey,                             // secret key
                            uint8_t* tmp_space);

#endif  // SDITH_SIGNATURE_H
