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

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <oqs/rand.h>

#include "sidh_util.h"

char *oqs_sidh_iqc_ref_concat(char *str1, const char *str2) {
	char *temp = (char *) malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(temp, str1);
	strcat(temp, str2);
	return temp;
}

char *oqs_sidh_iqc_ref_get_random_str(int num_bytes) {
	char *rand_value = (char *) malloc(num_bytes);
	OQS_RAND *rand = OQS_RAND_new(OQS_RAND_alg_urandom_chacha20);
	OQS_RAND_n(rand, (uint8_t *) rand_value, num_bytes);

	return rand_value;
}

void oqs_sidh_iqc_ref_get_random_mpz(mpz_t x) {
	int num_bytes = 20;
	char *a = oqs_sidh_iqc_ref_get_random_str(num_bytes);
	mpz_import(x, num_bytes, 1, sizeof(char), 0, 0, a);
}

char *oqs_sidh_iqc_ref_array_xor(const char *array1, const char *array2,
                                 long lenght) {
	char *result = (char *) malloc(lenght);
	for (long i = 0; i < lenght; i++)
		result[i] = array1[i] ^ array2[i];

	return result;
}