//
// SIKE-Weierstrass
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#ifndef ISOGENY_REF_TEST_SIKE_H
#define ISOGENY_REF_TEST_SIKE_H

#include <sike_params.h>
#include <stdlib.h>

int test_sidh(const char* name, const sike_params_t* params);

int test_pke(const char* name, const sike_params_t* params);

int test_sike(const char* name, const sike_params_t* params, int rnd);

int test_sike_int(const char* name, const sike_params_t* params);

int test_sike_speedy(const char *name, const sike_params_t *params, int runs);

int test_sike_speedy_int(const char *name, const sike_params_t *params, int runs);

#endif //ISOGENY_REF_TEST_SIKE_H
