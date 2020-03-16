#ifndef _H_RAINBOW_CONFIG_H_
#define _H_RAINBOW_CONFIG_H_

/// @file rainbow_config.h
/// @brief Defining the parameters of the Rainbow and the corresponding constants.
///

#define _GFSIZE 256
#define _V1 92
#define _O1 48
#define _O2 48
#define _MAX_O 48
#define _HASH_LEN 64


#define _V2 ((_V1) + (_O1))

/// size of N, in # of gf elements.
#define _PUB_N (_V1 + _O1 + _O2)

/// size of M, in # gf elements.
#define _PUB_M (_O1 + _O2)

/// size of variables, in # bytes.

// GF256
#define _V1_BYTE (_V1)
#define _V2_BYTE (_V2)
#define _O1_BYTE (_O1)
#define _O2_BYTE (_O2)
#define _MAX_O_BYTE (_MAX_O)
#define _PUB_N_BYTE (_PUB_N)
#define _PUB_M_BYTE (_PUB_M)


/// length of seed for public key, in # bytes
#define LEN_PKSEED 32

/// length of seed for secret key, in # bytes
#define LEN_SKSEED 32

/// length of salt for a signature, in # bytes
#define _SALT_BYTE 16

/// length of a signature
#define _SIGNATURE_BYTE (_PUB_N_BYTE + _SALT_BYTE)

#endif //  _H_RAINBOW_CONFIG_H_
