/**
 *  m4r.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  Methods of Four Russians
 *
 *  References:
 *  [1]  Martin Albrecht, Gregory Bard and William Hart, (2010),
 *       "Algorithm 898 : efficient multiplication of dense matrices over GF(2)", 
 *       ACM Transactions on Mathematical Software, Volume 37 (Number 1), 
 *       Article: 9, ISSN 0098-3500
 *  [2]  Martin Albrecht, Gregory Bard and Clement Pernet, (2011),
 *       "Efficient Dense Gaussian Elimination over the Finite Field with Two Elements",
 *       Research Report, https://hal.inria.fr/hal-00796873
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef __M4R_H
#define __M4R_H

#include <stdint.h>
#include "matrix_ff2.h"

/**
 *  Transform matrix into row-reduced echelon form
 *
 *  @note
 *  The transformation is performed in-place.
 *  This implementation transforms the matrix from
 *  last column downwards, i.e. it tries to create
 *  an identity matrix on the right-hand side of
 *  matrix A.
 *
 *  @param[in,out] A    Matrix A
 *  @return The rank matrix A
 **/
uint32_t PQCLEAN_NTSKEM1264_CLEAN_m4r_rref(matrix_ff2* A);

#endif /* __M4R_H */
