/**
 *  additive_fft.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef __ADDITIVE_FFT_H
#define __ADDITIVE_FFT_H

#include "ff.h"
#include "polynomial.h"

/**
 *  Performs additive FFT on polynomial f
 *
 *  @note
 *  The FFT is evaluated on a basis given by
 *  B = {b^{m1}, ..., b, 1}, where b is an element
 *  of ff2m. The FFT basically produces 
 *  (f[B[0]], f[B[1]], ..., f[B[n-1]]).
 *
 *  @param[in] ff2m     The pointer to a finite field object
 *  @param[in] f        The input polynomial
 *  @return the FFT of f, or the multipoint evaluation of f
 *          on all elements of ff2m on success, NULL otherwise.
 **/
ff_unit* PQCLEAN_NTSKEM1264_CLEAN_additive_fft(const FF2m* ff2m, const poly *f);

#endif /* __ADDITIVE_FFT_H */
