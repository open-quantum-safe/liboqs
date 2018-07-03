#ifndef LOWMC_H
#define LOWMC_H

#include "lowmc_pars.h"

/**
 * Implements LowMC encryption
 *
 * \param  lowmc the lowmc parameters
 * \param  p     the plaintext
 * \return       the ciphertext
 */
mzd_local_t* lowmc_call(lowmc_t const* lowmc, lowmc_key_t const* lowmc_key, mzd_local_t const* p);

#endif
