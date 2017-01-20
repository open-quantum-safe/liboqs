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

#ifndef SHARED_KEY_H
#define SHARED_KEY_H

#include "sidh_private_key.h"
#include "sidh_public_key.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Generates the shared-key.
 * @param shared_key the generated shared-key
 * @param public_key other's public-key
 * @param private_key own private-key
 * @param params own parameters
 */
void oqs_sidh_iqc_ref_shared_key_generate(fp2_element_t shared_key,
        const public_key_t public_key,
        const private_key_t private_key,
        const public_params_t params);

#ifdef __cplusplus
}
#endif

#endif /* SHARED_KEY_H */

