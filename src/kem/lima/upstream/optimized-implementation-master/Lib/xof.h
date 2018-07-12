#ifndef _prng

/** 
 * These routines implement the XOF methods from 
 * Section 2.1 of the specification document.
 * XOF takes a string as input and outputs an
 * unbounded "random string".
 * KDF produces a string of fixed length. 
 **/

#include <libkeccak.a.headers/SP800-185.h>
#include "modp.h"

#include <stdint.h>

typedef KMAC_Instance XOF_t;
typedef unsigned long long bit_length;
typedef unsigned long long byte_length;


/** 
 * Section 2.1.1 of Specification
 * Function  used to initialize the XOF instance
 *   - xof             Pointer to the xof to be inialized
 *   - key             Pointer to the key  (seed)
 *   - key_len         Length in BYTES of the seed
 *   - data            One byte data string. 
 *   We use the following values for data
 *     data = 0x00  for KDF
 *     data = 0x01  in KeyGeneration  (lima.c)
 *     data = 0x02  in LIMA Enc-CPA   (lima.c)
 *     data = 0x03  in LIMA Enc-CCA   (lima.c)
 *     data = 0x04  in LIMA Encap-CPA (lima.c)
 *     data = 0x05  in LIMA Encap-CCA (lima.c)
 * Return 0 if successful, 1 otherwise
**/
int InitXOF(XOF_t* xof, const uint8_t* key, byte_length key_len, uint8_t data);

/** 
 * Function used to get an array of bytes of arbitrary length
 * InitXOF already called
 *   - xof             Pointer to the instance initialized by InitXOF
 *   - output          Pointer to the output data, 
 *                     output is assumed to have length >= output_len already
 *   - output_length   The desired number of output bytes
 *  Return 0 if successful, 1 otherwise
 **/
int XOF(XOF_t* xof, uint8_t* output, byte_length output_len);


/** 
 * KDF: Function used to get an array of bytes of fixed length
 *    - k              Pointer to the input key
 *    - key            Pointer to the output key
 *    - Lengths are in bytes, and key is assumed already assigned enough length
 *  Returns 0 if successful, 1 otherwise
 **/
int KDF(const uint8_t* k, byte_length k_len, uint8_t* key, byte_length key_len);


/** Section 2.1.2 of Specification
 *  Function used to get an element from a finite field F_q
 *  It assumes that the number of bytes in q, log[256]_q <= 4, 
 *  i.e. maximum q fits in 32 bits
 *     - xof           Pointer to xof as before
 *     - ctx           Pointer to ctx used for arithmetic mod q
 **/
int  get_random_field_element(XOF_t* xof, modp_context_t* ctx);

/** 
 *   Function used to produce a "random" element from F_q^n
 *   It assumes an InitXOF has already been called
 *    - a is assumed to have at least n elements of space
**/
void get_random_field_elements(XOF_t* xof, modp_context_t* ctx, int n, int a[]);



/** Section 2.1.3 of the Specification
 *  Routine for approximate Gaussian noise of standard deviation close to 3.16
 *  and mean =0
**/
int generate_gaussian_noise(XOF_t* xof);

#endif
