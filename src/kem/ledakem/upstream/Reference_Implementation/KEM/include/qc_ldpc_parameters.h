/**
 *
 * <qc_ldpc_parameters.h>
 *
 * @version 1.0 (September 2017)
 *
 * Reference ISO-C99 Implementation of LEDAkem cipher" using GCC built-ins.
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/
#pragma once
// CATEGORY defined in the makefile

/*----------------------------------------------------------------------------*/
#if CATEGORY == 1
#define TRNG_BYTE_LENGTH (24)
#define    HASH_FUNCTION sha3_256
#define HASH_BYTE_LENGTH (32)
// N0 defined in the makefile
#if N0 == 2
#define    P  (27779)  // modulus(x) = x^P-1
#define    DV (17)  // odd number
#define    M  (7)
#define    M0 (4)
#define    M1 (3)
#define    NUM_ERRORS_T   (224)

#elif N0 == 3
#define    P  (18701)  // modulus(x) = x^P-1
#define    DV (19)  // odd number
#define    M  (7)
#define    M0 (3)
#define    M1 (2)
#define    M2 (2)
#define    NUM_ERRORS_T   (141)

#elif N0 == 4
#define    P  (17027)  // modulus(x) = x^P-1
#define    DV (21)  // odd number
#define    M  (7)
#define    M0 (4)
#define    M1 (1)
#define    M2 (1)
#define    M3 (1)
#define    NUM_ERRORS_T   (112)

#else
#error "Unsupported number of circulant blocks"
#endif
#endif // end CATEGORY == 1

/*----------------------------------------------------------------------------*/

// We employ the parameters for Category 3 also in the case where the required
// security level is Category 2, where Category 2 has the following parameters.
//   #define TRNG_BYTE_LENGTH (32)
//   #define    HASH_FUNCTION sha3_256
//   #define HASH_BYTE_LENGTH (32)

/*----------------------------------------------------------------------------*/

#if (CATEGORY == 2) || (CATEGORY == 3)
#define TRNG_BYTE_LENGTH (32)
#define    HASH_FUNCTION sha3_384
#define HASH_BYTE_LENGTH (48)
// N0 defined in the makefile
#if N0 == 2
#define    P  (57557)  // 57503 modulus(x) = x^P-1
#define    DV (17)  // odd number
#define    M  (11)
#define    M0 (6)
#define    M1 (5)
#define    NUM_ERRORS_T   (349)

#elif N0 == 3
#define    P  (41507)  // modulus(x) = x^P-1
#define    DV (19)  // odd number
#define    M  (11)
#define    M0 (3)
#define    M1 (4)
#define    M2 (4)
#define    NUM_ERRORS_T   (220)

#elif N0 == 4
#define    P  (35027)  // modulus(x) = x^P-1
#define    DV (17)  // odd number
#define    M  (13)
#define    M0 (4)
#define    M1 (3)
#define    M2 (3)
#define    M3 (3)
#define    NUM_ERRORS_T   (175)

#else
#error "Unsupported number of circulant blocks"
#endif
#endif

/*----------------------------------------------------------------------------*/

// We employ the parameters for Category 4 also in the case where the required
// security level is Category 5, where Category 4 has the following parameters.
// #if CATEGORY == 4
//   #define TRNG_BYTE_LENGTH (40)
//   #define    HASH_FUNCTION sha3_384
//   #define HASH_BYTE_LENGTH (48)
// #endif

/*----------------------------------------------------------------------------*/

#if (CATEGORY == 4) || (CATEGORY == 5)
#define TRNG_BYTE_LENGTH (40)
#define    HASH_FUNCTION  sha3_512
#define HASH_BYTE_LENGTH (64)
// N0 defined in the makefile
#if N0 == 2
#define    P  (99053)  // modulus(x) = x^P-1
#define    DV (19)  // odd number
#define    M  (13)
#define    M0 (7)
#define    M1 (6)
#define    NUM_ERRORS_T   (474)

#elif N0 == 3
#define    P  (72019)  // modulus(x) = x^P-1
#define    DV (19)  // odd number
#define    M  (15)
#define    M0 (7)
#define    M1 (4)
#define    M2 (4)
#define    NUM_ERRORS_T   (301)

#elif N0 == 4
#define    P  (60509)  // modulus(x) = x^P-1
#define    DV (23)  // odd number
#define    M  (13)
#define    M0 (4)
#define    M1 (3)
#define    M2 (3)
#define    M3 (3)
#define    NUM_ERRORS_T   (239)

#else
#error "Unsupported number of circulant blocks"
#endif
#endif
/*----------------------------------------------------------------------------*/

// Derived parameters, they are useful for QC-LDPC algorithms
#define HASH_BIT_LENGTH (HASH_BYTE_LENGTH << 3)
#define               K ((N0-1)*P)
#define               N (N0*P)
#define              DC (N0*DV)

// Circulant weight structure of the Q matrix, specialized per value of N0

#if N0 == 2
#define    Q_BLOCK_WEIGHTS  {{M0,M1},{M1,M0}}
#elif N0 == 3
#define    Q_BLOCK_WEIGHTS  {{M0,M1,M2},{M2,M0,M1},{M1,M2,M0}}
#elif N0 == 4
#define    Q_BLOCK_WEIGHTS  {{M0,M1,M2,M3},{M3,M0,M1,M2},{M2,M3,M0,M1},{M1,M2,M3,M0}}
#else
#error "Unsupported number of circulant blocks"
#endif

static const unsigned char qBlockWeights[N0][N0] = Q_BLOCK_WEIGHTS;

/*----------------------------------------------------------------------------*/
