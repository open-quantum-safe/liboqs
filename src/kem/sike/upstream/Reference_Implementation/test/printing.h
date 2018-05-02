//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

/** @file
Helper functions for printing interanal structures
*/


#ifndef ISOGENY_REF_PRINTING_H
#define ISOGENY_REF_PRINTING_H

/*
 * Prints as hex
 */
void print_hex(const char* title, const unsigned char* seq, size_t seqLen);

/*
 * Prints a point on a curve
 */
void printPt(const char* title, const mont_pt_t* pt);

/*
 * Prints a fp2 element
 */
void printFp2(const char* title, const fp2* f);

/*
 * Prints curve points and coefficients
 */
void printPk(const char* title, const mont_curve_int_t* curve);

void printPkExt(const char* title, const sike_public_key_t* curve);

#endif //ISOGENY_REF_PRINTING_H
