#ifndef SIZES_H
#define SIZES_H

#ifndef SECURITY_LEVEL
#define SECURITY_LEVEL 5
#endif

#include "gf.h"

/*
  Note:
  Global parameters LENGTH, ORDER and NB_ERRORS are provided at commpilation
*/


// "#define" related to generic bit/bytes/sizes
// size of a hash (in bytes)
#define HASH_SIZE 32
// number of bytes needed for storing nb_bits bits
#define BITS_TO_BYTES(nb_bits) (((nb_bits) - 1) / 8 + 1)
// number bits in one long = __WORDSIZE
#define BIT_SIZE_OF_LONG (8 * sizeof(long))
// number of long needed for storing nb_bits bits
#define BITS_TO_LONG(nb_bits) (((nb_bits) - 1) / BIT_SIZE_OF_LONG + 1)

// Goppa codes
#define EXT_DEGREE GF_EXT_DEGREE
#define CODIMENSION (NB_ERRORS * EXT_DEGREE)
#define DIMENSION (LENGTH - CODIMENSION)
#define SYNDROME_LENGTH CODIMENSION
#define SYNDROME_BYTES BITS_TO_BYTES(SYNDROME_LENGTH)

// sizes of keys  and ciphertexts
#define SECRETKEY_BYTES ((LENGTH + 1 + 2 * NB_ERRORS) * sizeof (gfelt_t))
#define PUBLICKEY_BYTES (BITS_TO_BYTES(CODIMENSION) * ((DIMENSION) / (ORDER)))
#define CIPHERTEXT_LENGTH (2*(8*HASH_SIZE) + SYNDROME_LENGTH)
#define CIPHERTEXT_BYTES BITS_TO_BYTES(CIPHERTEXT_LENGTH)

// size of the random input for the encapsaluation
#define RANDOM_LENGTH 128
#define RANDOM_BYTES BITS_TO_BYTES(RANDOM_LENGTH)

// access to the i-th bit of a char vector v
#define ind(v, i) ((v)[(i) / 8] >> ((i) % 8) & 1)


#define show_params() {							\
	printf("(n,m,t)=(%d,%d,%d)\n", LENGTH, EXT_DEGREE, NB_ERRORS);	\
	printf("Ciphertext: %d bits\n", CIPHERTEXT_LENGTH);		\
	printf("Public key: %ld bytes\n", PUBLICKEY_BYTES);		\
	printf("Secret key: %ld bytes\n", SECRETKEY_BYTES);		\
    }

#endif
