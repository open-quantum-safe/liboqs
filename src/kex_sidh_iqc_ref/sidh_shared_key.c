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


#include "sidh_shared_key.h"
#include "sidh_isogeny.h"

void oqs_sidh_iqc_ref_shared_key_generate(fp2_element_t shared_key,
        const public_key_t public_key,
        const private_key_t private_key,
        const public_params_t params) {

	point_t kernel_gen;
	oqs_sidh_iqc_ref_point_init(kernel_gen);

	// compute a generator for the kernel of the isogeny
	oqs_sidh_iqc_ref_private_key_compute_kernel_gen(kernel_gen,
	        private_key,
	        public_key->P,
	        public_key->Q,
	        params->le,
	        public_key->E);
	elliptic_curve_t E;
	oqs_sidh_iqc_ref_elliptic_curve_init(E);
	oqs_sidh_iqc_ref_elliptic_curve_set(E, public_key->E);

	oqs_sidh_iqc_ref_isogeny_evaluate_strategy_curve(E, kernel_gen, params->l, params->e, 0.5);
	//    oqs_sidh_iqc_ref_isogeny_evaluate_naive_curve(E, kernel_gen, params->l, params->e, 3);

	oqs_sidh_iqc_ref_elliptic_curve_compute_j_inv(shared_key, E);

	oqs_sidh_iqc_ref_point_clear(kernel_gen);
	oqs_sidh_iqc_ref_elliptic_curve_clear(E);
}