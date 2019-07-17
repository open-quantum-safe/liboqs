/**
 *  nts_kem.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef __NTS_KEM_H
#define __NTS_KEM_H

#include <stddef.h>
#include <stdint.h>

/**
 *  NTS data structure
 **/
typedef struct {
    int length;                 /* The length */
    int t;                      /* The number of correctible errors */
    uint8_t *public_key;        /* The public key */
    uint8_t *private_key;       /* The private key */
    size_t public_key_size;     /* The size of the public key in bytes */
    size_t private_key_size;    /* The size of the private key in bytes */
    void *priv;                 /* Private component */
} NTSKEM;

/**
 *  Initialise an NTS-KEM object with a given parameter
 *
 *  @param[out] nts_kem A pointer of NTSKEM object created
 *  @return NTS_KEM_SUCCESS on success, otherwise a negative error code 
 *          {@see nts_kem_errors.h}
 **/
int PQCLEAN_NTSKEM1264_CLEAN_nts_kem_create(NTSKEM** nts_kem);

/**
 *  Initialise an NTS-KEM object from a buffer containing the private key
 *
 *  @param[out] nts_kem     A pointer of NTSKEM object created
 *  @param[in]  buffer      The buffer containing the private key
 *  @param[in]  buffer_size The size of the private key buffer in bytes
 *  @return NTS_KEM_SUCCESS on success, otherwise a negative error code
 *          {@see nts_kem_errors.h}
 **/
int PQCLEAN_NTSKEM1264_CLEAN_nts_kem_init_from_private_key(NTSKEM** nts_kem,
                                  const uint8_t *buffer,
                                  size_t buffer_size);

/**
 *  Release the NTS-KEM object
 *
 *  @param[in] nts_kem  A pointer to an NTS-KEM object
 **/
void PQCLEAN_NTSKEM1264_CLEAN_nts_kem_release(NTSKEM *nts_kem);

/**
 *  Return the key size in bytes of NTS-KEM
 
 *  @return NTS-KEM key size in bytes
 **/
int PQCLEAN_NTSKEM1264_CLEAN_nts_kem_key_size(void);

/**
 *  Return the ciphertext size in bytes of NTS-KEM
 
 *  @param[in] nts_kem  The pointer to an NTS-KEM object
 *  @return NTS-KEM ciphertext size in bytes
 **/
int PQCLEAN_NTSKEM1264_CLEAN_nts_kem_ciphertext_size(const NTSKEM *nts_kem);

/**
 *  NTS-KEM encapsulation
 *
 *  @param[in]  pk      The pointer to NTS-KEM public key
 *  @param[in]  pk_size The size of the public key in bytes
 *  @param[out] c_ast   The pointer to the NTS-KEM ciphertext
 *  @param[out] k_r     The pointer to the encapsulated key
 *  @return NTS_KEM_SUCCESS on success, otherwise a negative error code
 *          {@see nts_kem_errors.h}
 **/
int PQCLEAN_NTSKEM1264_CLEAN_nts_kem_encapsulate(const uint8_t *pk,
                        size_t pk_size,
                        uint8_t *c_ast,
                        uint8_t *k_r);

/**
 *  NTS-KEM decapsulation
 *
 *  @param[in]  sk      The pointer to NTS-KEM private key
 *  @param[in]  sk_size The size of the private key in bytes
 *  @param[in]  c_ast   The pointer to the NTS-KEM ciphertext
 *  @param[out] k_r     The pointer to the encapsulated key
 *  @return NTS_KEM_SUCCESS on success, otherwise a negative error code
 *          {@see nts_kem_errors.h}
 **/
int PQCLEAN_NTSKEM1264_CLEAN_nts_kem_decapsulate(const uint8_t *sk,
                        size_t sk_size,
                        const uint8_t *c_ast,
                        uint8_t *k_r);

#endif /* __NTS_KEM_H */
