#ifndef OQS_KEX_CODE_NRQCMDPC_CONFIG_H
#define OQS_KEX_CODE_NRQCMDPC_CONFIG_H

/**
 * \file config.h
 * \brief System parameters.
 *
 * Defines the parameters for both the crypto system and the particular
 * implementation.
 */

#define POLY_COUNT 2
#define POLY_BITS 4801
#define POLY_WEIGHT 45
#define ERROR_WEIGHT 84
#define THRESHOLDS {29, 27, 25, 24, 23, 23}

/**
 * \def POLY_INDEX_BITS ceil(log_2(POLY_BITS))
 *
 * TODO: hardcoded (should be derived)
 */
#define POLY_INDEX_BITS 13

/**
 * \def ERROR_INDEX_BITS ceil(log_2(ERROR_BITS))
 *      where ERROR_BITS = POLY_COUNT * POLY_BITS
 *
 * TODO: hardcoded (should be derived)
 */
#define ERROR_INDEX_BITS 14


#endif /* OQS_KEX_CODE_NRQCMDPC_CONFIG_H */
