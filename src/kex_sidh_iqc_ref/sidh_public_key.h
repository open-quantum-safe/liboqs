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

#ifndef PUBLIC_KEY_H
#define PUBLIC_KEY_H

#include "sidh_public_param.h"
#include "sidh_private_key.h"
#include "sidh_isogeny.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Representation of the public key in oqs_sidh_iqc_ref
 */
typedef struct {
	elliptic_curve_t E;
	point_t P;
	point_t Q;
} public_key_struct;

typedef public_key_struct public_key_t[1];

/**
 * Initializes the public-key.
 * @param public_key
 */
void oqs_sidh_iqc_ref_public_key_init(public_key_t public_key);

/**
 * Frees the memory allocated to the public-key.
 * @param public_key
 */
void oqs_sidh_iqc_ref_public_key_clear(public_key_t public_key);

/**
 * Generates the public-key
 * @param public_key
 * @param kernel_gen a generator for the kernel of the isogeny
 * @param paramsA own params
 * @param paramsB other's params
 */
void oqs_sidh_iqc_ref_public_key_generate(public_key_t public_key,
        const point_t kernel_gen,
        const public_params_t paramsA,
        const public_params_t paramsB);

/**
 * Prints {@code public_key} to the standard output.
 * @param public_key
 */
void oqs_sidh_iqc_ref_public_key_print(const public_key_t public_key);

/**
 * Converts a public-key to a byte array.
 * @param bytes
 * @param public_key
 * @param prime_size
 */
void oqs_sidh_iqc_ref_public_key_to_bytes(uint8_t *bytes,
        const public_key_t public_key,
        long prime_size);


/**
 * Converts a byte array to a public-key.
 * @param public_key
 * @param bytes
 * @param prime_size
 */
void oqs_sidh_iqc_ref_bytes_to_public_key(public_key_t public_key,
        const uint8_t *bytes,
        long prime_size);

#ifdef __cplusplus
}
#endif

#endif /* PUBLIC_KEY_H */
