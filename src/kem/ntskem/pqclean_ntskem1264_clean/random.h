/**
 *  random.h
 *  NTS-KEM
 *
 *  Parameter: NTS-KEM(12, 64)
 *
 *  This file is part of the reference implemention of NTS-KEM
 *  submitted as part of NIST Post-Quantum Cryptography
 *  Standardization Process.
 **/

#ifndef __NTSKEM_RANDOM_H
#define __NTSKEM_RANDOM_H

#include <stddef.h>
#include <stdint.h>

/**
 *  Generate a 16-bit random number between 0 and `bound-1`
 *  
 *  @param[in]  bound  The limit of the number to be generated
 *  @return a 16-bit random number
 **/
uint16_t PQCLEAN_NTSKEM1264_CLEAN_random_uint16_bounded(uint16_t bound);

#endif /* __NTSKEM_RANDOM_H */
