/**
 *  berlekamp_massey.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef _BERLEKAMP_MASSEY_H
#define _BERLEKAMP_MASSEY_H

#include "ff.h"
#include "polynomial.h"

/**
 *  Berlekamp-Massey algorithm
 *
 *  @note
 *  This is a constant-time and inversionless implementation of
 *  Berlekamp-Massey algorithm
 *
 *  @param[in]  ff2m           A pointer to the finite-field object
 *  @param[in]  S              A pointer to a set of 2t syndromes
 *  @param[in]  s_len          The length of the syndromes array
 *  @param[out] extended_error An indicator whether or not there
 *                             is an error at the last coordinate,
 *                             1 if there is an error in the last
 *                             coordinate, 0 otherwise
 *  @return A polynomial e(x) whose reciprocal roots indicate the
 *          location of errors.
 **/
poly *PQCLEAN_NTSKEM1264_CLEAN_berlekamp_massey(const FF2m *ff2m,
                       const ff_unit *S,
                       int s_len,
                       int *extended_error);

#endif
