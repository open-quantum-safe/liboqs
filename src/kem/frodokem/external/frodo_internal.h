/********************************************************************************************
* FrodoKEM: Learning with Errors Key Encapsulation
*
* Abstract: header for internal functions
*********************************************************************************************/

#ifndef _FRODO_INTERNAL_H_
#define _FRODO_INTERNAL_H_

#include <stddef.h>
#include <stdint.h>

void frodo_pack(unsigned char *out, const size_t outlen, const uint16_t *in, const size_t inlen, const unsigned char lsb);
void frodo_unpack(uint16_t *out, const size_t outlen, const unsigned char *in, const size_t inlen, const unsigned char lsb);
void frodo_sample_n(uint16_t *s, const size_t n);
int8_t ct_verify(const uint16_t *a, const uint16_t *b, size_t len);
void ct_select(uint8_t *r, const uint8_t *a, const uint8_t *b, size_t len, int8_t selector);

int frodo_mul_add_as_plus_e(uint16_t *b, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
int frodo_mul_add_sa_plus_e(uint16_t *b, const uint16_t *s, const uint16_t *e, const uint8_t *seed_A);
void frodo_mul_add_sb_plus_e(uint16_t *out, const uint16_t *b, const uint16_t *s, const uint16_t *e);
void frodo_mul_bs(uint16_t *out, const uint16_t *b, const uint16_t *s);

void frodo_add(uint16_t *out, const uint16_t *a, const uint16_t *b);
void frodo_sub(uint16_t *out, const uint16_t *a, const uint16_t *b);
void frodo_key_encode(uint16_t *out, const uint16_t *in);
void frodo_key_decode(uint16_t *out, const uint16_t *in);

// Configuration for endianness
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define LE_TO_UINT16(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
    #define UINT16_TO_LE(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
#elif (TARGET == TARGET_x86 || TARGET == TARGET_AMD64) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__))
    #define LE_TO_UINT16(n) (n)
    #define UINT16_TO_LE(n) (n)
#else
    #define LE_TO_UINT16(n) (((uint8_t *) &(n))[0] | (((uint8_t *) &(n))[1] << 8))
    static inline uint16_t UINT16_TO_LE(const uint16_t x) {
        uint16_t y;
        uint8_t *z = (uint8_t *) &y;
        z[0] = x & 0xFF;
        z[1] = x >> 8;
        return y;
    }
#endif

#if defined(_MSC_VER) // if using Visual Studio compiler
    #define ALIGN_HEADER(N) __declspec(align(N))
    #define ALIGN_FOOTER(N) 
#else
    #define ALIGN_HEADER(N)
    #define ALIGN_FOOTER(N) __attribute__((aligned(N)))
#endif

#endif
