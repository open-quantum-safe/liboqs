/*! @file LowMCConstants.h
 *  @brief Constant parameters for the LowMC instantiations used in the
 *  signature scheme.
 *
 *  This file is part of the reference implementation of the Picnic and Fish
 *  signature schemes, described in the paper:
 *
 *  Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives
 *  Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and
 *  Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg
 *  Zaverucha
 *  Cryptology ePrint Archive: Report 2017/279
 *  http://eprint.iacr.org/2017/279
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *
 */
#include <stdint.h>
#ifndef LOWMCCONSTANTS_H
#define LOWMCCONSTANTS_H

#define WORD_SIZE_BITS 32 // the word size for the implementation. Not a LowMC parameter

#define STATE_SIZE_BITS 256
#define STATE_SIZE_WORDS STATE_SIZE_BITS / WORD_SIZE_BITS

#define LOWMC_MAX_STATE_SIZE 32

#if (STATE_SIZE_WORDS & 1) == 1
#error The implementation of LowMC  assumes STATE_SIZE_WORDS is even
#endif

#ifdef __WINDOWS__
#define DEFAULT_DATA_PATH "precomputed_data\\"
#else
#define DEFAULT_DATA_PATH "precomputed_data/"
#endif

typedef struct lowmcparams_t {
	uint32_t numRounds;
	uint32_t numSboxes;
	uint32_t stateSizeBits;
	uint32_t stateSizeBytes;
	uint32_t stateSizeWords;
	uint32_t numAndBytes;
	uint32_t UnruhGWithoutInputBytes;
	uint32_t UnruhGWithInputBytes;
} lowmcparams_t;

#endif /* LOWMCCONSTANTS_H */
