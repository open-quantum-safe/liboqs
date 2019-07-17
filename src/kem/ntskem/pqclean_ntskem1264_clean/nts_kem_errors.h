/**
 *  nts_kem_errors.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef __NTS_KEM_ERRORS_H
#define __NTS_KEM_ERRORS_H

#define NTS_KEM_SUCCESS                            0

#define NTS_KEM_RNG_BAD_MAX_LENGTH                -1
#define NTS_KEM_RNG_INVALID_OUTPUT_BUFFER         -2
#define NTS_KEM_RNG_INVALID_REQUEST_LENGTH        -3
#define NTS_KEM_RNG_INVALID_PROVIDER              -4

#define NTS_KEM_BAD_MEMORY_ALLOCATION           -100
#define NTS_KEM_BAD_KEY_LENGTH                  -101
#define NTS_KEM_BAD_PARAMETERS                  -102
#define NTS_KEM_INVALID_CIPHERTEXT              -103

#define NTS_KEM_UNEXPECTED_ERROR                -200


#endif /* __NTS_KEM_ERRORS_H */
