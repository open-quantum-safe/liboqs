/*
 * Copyright (C) 2016 Javad Doliskani, javad.doliskani@uwaterloo.ca
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PUBLIC_PARAM_H
#define PUBLIC_PARAM_H

#include "sidh_elliptic_curve.h"
#include "sidh_quadratic_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Representation of the public parameters in oqs_sidh_iqc_ref
 */
typedef struct {
	// the characteristic
	mpz_t characteristic;

	elliptic_curve_t E;
	unsigned long l;
	unsigned long e;

	// a generator for the l^e torsion subgroup of E
	point_t P;
	point_t Q;

	// l^e, precomputed
	mpz_t le;

} public_params_struct;

typedef public_params_struct public_params_t[1];

/**
 * Initializes the public parameters.
 * @param params
 */
void oqs_sidh_iqc_ref_public_params_init(public_params_t params);

/**
 * Reads the public parameters from file named {@code file_name}.
 * @param paramsA
 * @param paramsB
 * @param file_name
 * @return 1 if the parameters are read successfully, and 0 otherwise.
 */
int oqs_sidh_iqc_ref_public_params_read(public_params_t paramsA,
                                        public_params_t paramsB,
                                        const char *file_name);

/**
 * Prints the public parameters to the standard output.
 * @param params
 * @param torsion if it is 1 only the torsion parameters are printed
 */
void oqs_sidh_iqc_ref_public_params_print(const public_params_t params,
                                          int print_torsion);

/**
 * Frees the memory allocated to {@code params}.
 * @param params
 */
void oqs_sidh_iqc_ref_public_params_clear(public_params_t params);

#ifdef __cplusplus
}
#endif

#endif /* PUBLIC_PARAM_H */
