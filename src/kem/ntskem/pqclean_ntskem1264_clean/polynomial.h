/**
 *  polynomial.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

#include <stdio.h>
#include <stdint.h>
#include "ff.h"

typedef struct {
    int size, degree;
    ff_unit *coeff;
} poly;

/**
 *  Initialise a new poly object
 *
 *  @param[in] size     The length of the polynomial
 *  @return The pointer to the polynomial object
 **/
poly* PQCLEAN_NTSKEM1264_CLEAN_init_poly(int size);

/**
 *  Release a polynomial object
 *
 *  @param[in] px   The pointer to a polynomial object
 **/
void PQCLEAN_NTSKEM1264_CLEAN_free_poly(poly* px);

/**
 *  Zero a polynomial
 *
 *  @param[in] px   The pointer to a polynomial object
 **/
void PQCLEAN_NTSKEM1264_CLEAN_zero_poly(poly* px);

/**
 *  Clone a polynomial
 *
 *  @param[in] px   The pointer to a polynomial object
 *  @return The pointer to the cloned polynomial object
 **/
poly* PQCLEAN_NTSKEM1264_CLEAN_clone_poly(const poly *px);

/**
 *  Compute the degree of a polynomial
 *
 *  @param[in] px   The pointer to a polynomial object
 **/
void PQCLEAN_NTSKEM1264_CLEAN_update_poly_degree(poly *px);

/**
 *  Compute the formal derivative of a polynomial
 *
 *  @param[in]  fx  The pointer to a polynomial object
 *  @param[out] dx  The derivative of f(x)
 *  @return 1 on success, 0 otherwise
 **/
int PQCLEAN_NTSKEM1264_CLEAN_formal_derivative_poly(const poly* fx, poly *dx);

/**
 *  Obtain the GCD of two finite-field polynomials
 *
 *  The method performs the following operation:
 *      g(x) = GCD(a(x), b(x))
 *
 *  @param ff2m The pointer to FF2m instance
 *  @param ax   The left input polynomial
 *  @param bx   The right input polynomial
 *  @param gx   The output polynomial
 *  @return 1 on successful operation, 0 otherwise
 **/
int PQCLEAN_NTSKEM1264_CLEAN_gcd_poly(const FF2m* ff2m, const poly* ax, const poly *bx, poly *gx);

#endif /* _POLYNOMIAL_H */
