#ifndef SHAKE128_H
#define SHAKE128_H

#define SHAKE128_STATE_SIZE 200
#define SHAKE128_RATE 168

/*
 * Computes SHAKE128 on the array "input" of length "inputByteLen", resulting in "outputByteLen" bytes stored in "output".
 */
static void FIPS202_SHAKE128(const unsigned char *input, unsigned int inputByteLen, unsigned char *output, int outputByteLen);

/*
 * Performs the absorb phase of SHAKE128: ingests the "inputByteLen" bytes stored in "input"; storing the internal 
 * SHAKE128 state of length SHAKE128_STATE_SIZE in "state".
 */
static void FIPS202_SHAKE128_Absorb(const unsigned char *input, unsigned int inputByteLen, unsigned char* state);

/*
 * Performs the squeeze phase of SHAKE128: generates "outputByteLen" bytes stored in "output" from the "state" of length
 * SHAKE128_STATE_SIZE. Must be preceeded by a call to FIPS202_SHAKE128_Absorb.
 */
static void FIPS202_SHAKE128_Squeeze(unsigned char* state, unsigned char *output, int outputByteLen);

#endif
