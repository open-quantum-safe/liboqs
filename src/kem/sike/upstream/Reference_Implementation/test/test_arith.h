//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#ifndef ISOGENY_REF_TEST_ARITH_H
#define ISOGENY_REF_TEST_ARITH_H

#include <sike_params.h>

/**
 * Tests Fp and Fp2 arithmetic with the given SIDH parameters
 * @param params SIDH parameters
 * @return 0 if tests succeeded, 1 otherwise
 */
int test_arith(const sike_params_t* params);

#endif //ISOGENY_REF_TEST_ARITH_H
