/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (March 2023)
 *
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
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
#include <stdint.h>

#include "set.h"

/******************************************************************************/
/*************************** Base Fields Parameters ***************************/
/******************************************************************************/

/* The same base field and restriction are employed for all categories of RSDP */
#define   Q (127)
#define   Z (  7)
/* single-register table representation of E, the value of g^7=1 is also
 * represented to avoid exponent renormalization*/
#define RESTR_G_TABLE ((uint64_t) (0x0140201008040201))
#define RESTR_G_GEN 2
#define FQ_ELEM uint8_t
#define FZ_ELEM uint8_t
#define FQ_DOUBLEPREC uint16_t
#define FQ_TRIPLEPREC uint32_t

/******************************************************************************/
/****************************** RSDP Parameters *******************************/
/******************************************************************************/
/********************************* Category 1 *********************************/
#define SEC_MARGIN_LAMBDA (192)
#define   N (187)
#define   K (111)

#define   T (245)
#define   W (127)
#define POSITION_IN_FW_STRING_T uint16_t

/********************************* Category 5 *********************************/

/******************************************************************************/
/****************************** RSDP(G) Parameters ****************************/
/******************************************************************************/

#define HASH_CSPRNG_DOMAIN_SEP_CONST ((uint16_t)32768)

/************* Helper macros for derived parameter computation ***************/

#define ROUND_UP(amount, round_amt) ( (((amount)+(round_amt)-1)/(round_amt))*(round_amt) )

#define IS_REPRESENTABLE_IN_D_BITS(D, N)                \
(((uint32_t) (N)>=(1UL << ((D)-1)) && (uint32_t) (N)<(1UL << (D))) ? (D) : -1)

#define BITS_TO_REPRESENT(N)                            \
  ((N) == 0 ? 1 : (15                                     \
                 + IS_REPRESENTABLE_IN_D_BITS( 1, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 2, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 3, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 4, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 5, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 6, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 7, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 8, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS( 9, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(10, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(11, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(12, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(13, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(14, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(15, N)    \
                 + IS_REPRESENTABLE_IN_D_BITS(16, N)    \
                 )                                      \
   )

#define LOG2(L) ( (BITS_TO_REPRESENT(L) > BITS_TO_REPRESENT((L)-1)) ? (BITS_TO_REPRESENT((L)-1)) : (BITS_TO_REPRESENT(L)) )

/***************** Derived parameters *****************************************/
#define SEED_LENGTH_BYTES (SEC_MARGIN_LAMBDA/8)
#define KEYPAIR_SEED_LENGTH_BYTES (2*(SEC_MARGIN_LAMBDA/8))
#define HASH_DIGEST_LENGTH (2*(SEC_MARGIN_LAMBDA/8))
#define SALT_LENGTH_BYTES (2*(SEC_MARGIN_LAMBDA/8))

#define NUM_LEAVES_MERKLE_TREE (T)
#define NUM_NODES_MERKLE_TREE (2*NUM_LEAVES_MERKLE_TREE-1)

/*to be derived via script for each T/W*/
#define NUM_LEAVES_SEED_TREE ( T )
// #define NUM_NODES_SEED_TREE ( 2*NUM_LEAVES_SEED_TREE-1 )
#define NUM_INNER_NODES_SEED_TREE ( NUM_NODES_SEED_TREE-NUM_LEAVES_SEED_TREE )

/* Sizes of bitpacked field element vectors
 * Bitpacking an n-elements vector of num_bits_for_q-1 bits long values
 * will pack 8 values in num_bits_for_q-1 bytes exactly, leaving the remaining
 * N % 8 as a tail */
#define DENSELY_PACKED_FQ_VEC_SIZE ((N/8)*BITS_TO_REPRESENT(Q-1) + \
                                   ROUND_UP( ((N%8)*BITS_TO_REPRESENT(Q-1)),8)/8)
#define DENSELY_PACKED_FQ_SYN_SIZE (((N-K)/8)*BITS_TO_REPRESENT(Q-1) + \
                                   ROUND_UP( (((N-K)%8)*BITS_TO_REPRESENT(Q-1)),8)/8)
#define DENSELY_PACKED_FZ_VEC_SIZE ((N/8)*BITS_TO_REPRESENT(Z-1) + \
                                   ROUND_UP( ((N%8)*BITS_TO_REPRESENT(Z-1)),8)/8)

/* Derived parameters computed via compute_derived_parameters.py */
#define TREE_NODES_TO_STORE 125
#define NUM_NODES_SEED_TREE 492
#define NODES_PER_LEVEL_ARRAY {1, 2, 4, 8, 16, 31, 62, 123, 245}
#define MISSING_NODES_BEFORE_LEVEL_ARRAY {0, 0, 0, 0, 0, 0, 1, 3, 8}
#define BITS_N_ZQ_CT_RNG 1361
#define BITS_BETA_ZQSTAR_CT_RNG 1785
#define BITS_V_CT_RNG 59289
#define BITS_N_ZZ_CT_RNG 729
#define BITS_CWSTR_RNG 2125
