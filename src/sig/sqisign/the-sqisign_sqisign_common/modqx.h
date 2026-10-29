#ifndef MODQX_H
#define MODQX_H

#include <stdio.h>
#include <stdint.h>
#include <mp.h>

#if RADIX == 64
#define sspint int64_t
#define spint uint64_t
#define dpint __uint128_t
#define sdpint __int128_t
#define MODQRADIX 61
#define MODQLIMBMASK ((spint)0x1fffffffffffffff)

#elif RADIX == 32
#define sspint int32_t
#define spint uint32_t
#define dpint uint64_t
#define sdpint int64_t
#define MODQRADIX 28
#define MODQLIMBMASK ((spint)0xfffffff)

#endif

#define MODQBITS (IBZ_NLIMBS * RADIX - 1)
#define MODQLIMBS ((MODQBITS + MODQRADIX - 1) / MODQRADIX)
#define MODQBYTES ((MODQBTIS + 7) / 8)

#define WINDOW_SIZE (3)
#define TABLE_SIZE (1 << WINDOW_SIZE)

// Modular addition - reduce less than 2p
void modqx_modadd(const spint a[MODQLIMBS],
                  const spint b[MODQLIMBS],
                  spint n[MODQLIMBS],
                  const spint two_p[MODQLIMBS],
                  int numwords);

// Modular subtraction - reduce less than 2p
void modqx_modsub(const spint a[MODQLIMBS],
                  const spint b[MODQLIMBS],
                  spint n[MODQLIMBS],
                  const spint two_p[MODQLIMBS],
                  int numwords);

// Modular multiplication, c=a*b mod 2p
void modqx_modmul(const spint a[MODQLIMBS],
                  const spint b[MODQLIMBS],
                  spint c[MODQLIMBS],
                  const spint p[MODQLIMBS],
                  const spint ndash,
                  int numwords);

// Modular squaring, c=a*a  mod 2p
void modqx_modsqr(const spint a[MODQLIMBS],
                  spint c[MODQLIMBS],
                  const spint p[MODQLIMBS],
                  const spint ndash,
                  int numwords);

// copy
void modqx_modcpy(const spint a[MODQLIMBS], spint c[MODQLIMBS], int numwords);

// Convert m to n-residue form, n=nres(m) knowing only 2p
void modqx_nresx(const spint m[MODQLIMBS], spint n[MODQLIMBS], const spint two_p[MODQLIMBS], int numwords);

// Convert n back to normal form, m=modqx_redc(n)
void modqx_redc(const spint n[MODQLIMBS],
                spint m[MODQLIMBS],
                const spint p[MODQLIMBS],
                const spint ndash,
                int numwords);

// is zero?
int modqx_modis0(const spint a[MODQLIMBS], const spint p[MODQLIMBS], const spint ndash, int numwords);

// set to zero
void modqx_modzer(spint a[MODQLIMBS], int numwords);

// set to one
void modqx_modone(spint a[MODQLIMBS], const spint two_p[MODQLIMBS], int numwords);

// conditional move g to f if d=1
// strongly recommend inlining be disabled using compiler specific syntax

// conditional swap g and f if d=1
// strongly recommend inlining be disabled using compiler specific syntax

// Cipolla algorithm for computing sqrt(x); requires any auxiliary t such that t^2-x is non-sqr. Returns -1 if number is
// a square, else 0.
int modqx_modcipolla(const spint x[MODQLIMBS],
                     const spint t[MODQLIMBS],
                     spint r[MODQLIMBS],
                     const spint p[MODQLIMBS],
                     const spint two_p[MODQLIMBS],
                     const spint one[MODQLIMBS],
                     const spint ndash,
                     int numwords);

// shift left by less than a word
void modqx_modshl(unsigned int n, spint a[MODQLIMBS], int numwords);

// shift right by less than a word. Return shifted out part
int modqx_modshr(unsigned int n, spint a[MODQLIMBS], int numwords);

// return true if equal
int modqx_modcmp(const spint a[MODQLIMBS],
                 const spint b[MODQLIMBS],
                 const spint p[MODQLIMBS],
                 const spint ndash,
                 int numwords);

// Compute ndash := (-p)^(-1) mod 2^MODQRADIX
spint modqx_getndash(const spint p0);

// Computes x^e mod p for e up to 1022 bits
void modqx_modxpowe(const spint p[MODQLIMBS],
                    const spint one[MODQLIMBS],
                    const spint ndash,
                    const spint x[MODQLIMBS],
                    const spint e[MODQLIMBS],
                    spint s[MODQLIMBS],
                    int numwords);

// Computes x^((p-1)/4) mod p using a windowed method for x in non-residue form and e up to 1022 bits
void modqx_modxpowe_windowed(const spint p[MODQLIMBS],
                             const spint two_p[MODQLIMBS],
                             const spint ndash,
                             const spint x[MODQLIMBS],
                             const spint e[MODQLIMBS],
                             spint table[TABLE_SIZE][MODQLIMBS],
                             spint s[MODQLIMBS],
                             int numwords);

#endif
