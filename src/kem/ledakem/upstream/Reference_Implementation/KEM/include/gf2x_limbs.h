/**
 *
 * <gf2x_limbs.h.c>
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

/*----------------------------------------------------------------------------*/

#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include "qc_ldpc_parameters.h"

/*----------------------------------------------------------------------------*/

#define LITTLE_ENDIAN

/*----------------------------------------------------------------------------*/
/* limb size definitions for the multi-precision GF(2^x) library              */
/*----------------------------------------------------------------------------*/

#ifndef CPU_WORD_BITS
typedef size_t DIGIT;
#define DIGIT_MAX SIZE_MAX
#else
// gcc -DCPU_WORD_BITS=64 ...
#define CAT(a, b, c) PRIMITIVE_CAT(a, b, c)
#define PRIMITIVE_CAT(a, b, c) a ## b ## c

typedef CAT( uint, CPU_WORD_BITS, _t ) DIGIT;
#define DIGIT_MAX (CAT(UINT, CPU_WORD_BITS, _MAX))
#endif

#if (DIGIT_MAX == ULLONG_MAX)
#define DIGIT_IS_ULLONG
#elif (DIGIT_MAX == ULONG_MAX)
#define DIGIT_IS_ULONG
#elif (DIGIT_MAX == UINT_MAX)
#define DIGIT_IS_UINT
#elif (DIGIT_MAX == UCHAR_MAX)
#define DIGIT_IS_UCHAR
#else
#error "unable to find the type of CPU_WORD_BITS"
#endif

#if (DIGIT_MAX == UINT64_MAX)
#define DIGIT_IS_UINT64
#define DIGIT_SIZE_B 8
#elif (DIGIT_MAX == UINT32_MAX)
#define DIGIT_IS_UINT32
#define DIGIT_SIZE_B 4
#elif (DIGIT_MAX == UINT16_MAX)
#define DIGIT_IS_UINT16
#define DIGIT_SIZE_B 2
#elif (DIGIT_MAX == UINT8_MAX)
#define DIGIT_IS_UINT8
#define DIGIT_SIZE_B 1
#else
#error "unable to find the bitsize of size_t"
#endif

#define DIGIT_SIZE_b  (DIGIT_SIZE_B << 3)

#define    POSITION_T uint32_t
/*----------------------------------------------------------------------------*/

