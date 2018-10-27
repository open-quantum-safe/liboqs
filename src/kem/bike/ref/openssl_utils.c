/******************************************************************************
* BIKE -- Bit Flipping Key Encapsulation
*
* Copyright (c) 2017 Nir Drucker, Shay Gueron, Rafael Misoczki, Tobias Oder, Tim Gueneysu
* (drucker.nir@gmail.com, shay.gueron@gmail.com, rafael.misoczki@intel.com, tobias.oder@rub.de, tim.gueneysu@rub.de)
*
* Permission to use this code BIKE is granted.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
*   this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * The names of the contributors may not be used to endorse or promote
*   products derived from this software without specific prior written
*   permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS ""AS IS"" AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS CORPORATION OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#include "openssl_utils.h"
#include "string.h"
#include "utilities.h"
#include "openssl/bn.h"

#define MAX_OPENSSL_INV_TRIALS 1000

// Perform a cyclic product by using OpenSSL
_INLINE_ OQS_STATUS ossl_cyclic_product(OUT BIGNUM *r,
                                        IN const BIGNUM *a,
                                        IN const BIGNUM *b,
                                        BN_CTX *bn_ctx) {
	OQS_STATUS res = OQS_SUCCESS;
	BIGNUM *m = BN_CTX_get(bn_ctx);
	if (NULL == m) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	// m = x^PARAM_R - 1
	if ((BN_set_bit(m, R_BITS) == 0) || (BN_set_bit(m, 0) == 0)) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	// r = a*b mod m
	if (BN_GF2m_mod_mul(r, a, b, m, bn_ctx) == 0) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

EXIT:
	return res;
}

_INLINE_ OQS_STATUS invert_poly(OUT BIGNUM *r,
                                IN const BIGNUM *a,
                                BN_CTX *bn_ctx) {
	OQS_STATUS res = OQS_SUCCESS;
	BIGNUM *m = BN_CTX_get(bn_ctx);

	if (NULL == m) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	// m = x^PARAM_R - 1
	if ((BN_set_bit(m, R_BITS) == 0) ||
	    (BN_set_bit(m, 0) == 0)) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	// r = a*b mod m
	uint32_t inv_res;
	for (uint32_t i = 0; i < MAX_OPENSSL_INV_TRIALS; i++) {
		inv_res = BN_GF2m_mod_inv(r, a, m, bn_ctx);
		if (inv_res != 0) {
			break;
		}
	}

	if (0 == inv_res) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

EXIT:

	return res;
}

// Loading numbers into OpenSSL should be done in Big Endian representation,
// therefore the byte order of a number should be reversed
_INLINE_ void reverse_endian(OUT uint8_t *res,
                             IN const uint8_t *in,
                             IN const uint32_t n) {
	uint32_t i;
	uint64_t tmp;

	for (i = 0; i < (n / 2); i++) {
		tmp = in[i];
		res[i] = in[n - 1 - i];
		res[n - 1 - i] = tmp;
	}

	// If the number of blocks is odd swap also the middle block
	if (n % 2) {
		res[i] = in[i];
	}
}

_INLINE_ OQS_STATUS ossl_bn2bin(OUT uint8_t *out,
                                IN const BIGNUM *in,
                                IN const uint32_t size) {
	uint8_t be_tmp[size];
	memset(out, 0, size);

	if (BN_bn2bin(in, be_tmp) == 0) {
		return OQS_EXTERNAL_LIB_ERROR_OPENSSL;
	}
	reverse_endian(out, be_tmp, BN_num_bytes(in));

	return OQS_SUCCESS;
}

_INLINE_ OQS_STATUS ossl_bin2bn(IN BIGNUM *out,
                                OUT const uint8_t *in,
                                IN const uint32_t size) {
	uint8_t be_tmp[size];
	memset(be_tmp, 0, size);

	reverse_endian(be_tmp, in, size);

	if (BN_bin2bn(be_tmp, size, out) == 0) {
		return OQS_EXTERNAL_LIB_ERROR_OPENSSL;
	}

	return OQS_SUCCESS;
}

OQS_STATUS ossl_add(OUT uint8_t res_bin[R_SIZE],
                    IN const uint8_t a_bin[R_SIZE],
                    IN const uint8_t b_bin[R_SIZE]) {
	OQS_STATUS res = OQS_SUCCESS;
	BN_CTX *bn_ctx = BN_CTX_new();
	BIGNUM *r = NULL;
	BIGNUM *a = NULL;
	BIGNUM *b = NULL;

	if (NULL == bn_ctx) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	BN_CTX_start(bn_ctx);

	r = BN_CTX_get(bn_ctx);
	a = BN_CTX_get(bn_ctx);
	b = BN_CTX_get(bn_ctx);

	if ((NULL == r) || (NULL == a) || (NULL == b)) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	res = ossl_bin2bn(a, a_bin, R_SIZE);
	CHECK_STATUS(res);
	res = ossl_bin2bn(b, b_bin, R_SIZE);
	CHECK_STATUS(res);

	if (BN_GF2m_add(r, a, b) == 0) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	res = ossl_bn2bin(res_bin, r, R_SIZE);
	CHECK_STATUS(res);

EXIT:
	if (bn_ctx) {
		BN_CTX_end(bn_ctx);
		BN_CTX_free(bn_ctx);
	}

	return res;
}

// Perform a cyclic product by using OpenSSL
OQS_STATUS cyclic_product(OUT uint8_t res_bin[R_SIZE],
                          IN const uint8_t a_bin[R_SIZE],
                          IN const uint8_t b_bin[R_SIZE]) {
	OQS_STATUS res = OQS_SUCCESS;
	BN_CTX *bn_ctx = BN_CTX_new();
	BIGNUM *r = NULL;
	BIGNUM *a = NULL;
	BIGNUM *b = NULL;

	if (NULL == bn_ctx) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	BN_CTX_start(bn_ctx);

	r = BN_CTX_get(bn_ctx);
	a = BN_CTX_get(bn_ctx);
	b = BN_CTX_get(bn_ctx);

	if ((NULL == r) || (NULL == a) || (NULL == b)) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	res = ossl_bin2bn(a, a_bin, R_SIZE);
	CHECK_STATUS(res);
	res = ossl_bin2bn(b, b_bin, R_SIZE);
	CHECK_STATUS(res);
	res = ossl_cyclic_product(r, a, b, bn_ctx);
	CHECK_STATUS(res);
	res = ossl_bn2bin(res_bin, r, R_SIZE);
	CHECK_STATUS(res);

EXIT:
	if (bn_ctx) {
		BN_CTX_end(bn_ctx);
		BN_CTX_free(bn_ctx);
	}

	return res;
}

OQS_STATUS ossl_split_polynomial(OUT uint8_t e0_bin[R_SIZE],
                                 OUT uint8_t e1_bin[R_SIZE],
                                 IN const uint8_t e_bin[N_SIZE]) {
	OQS_STATUS res = OQS_SUCCESS;
	BN_CTX *bn_ctx = BN_CTX_new();
	BIGNUM *e = NULL;
	BIGNUM *e0 = NULL;
	BIGNUM *e1 = NULL;
	BIGNUM *mid = NULL;

	if (NULL == bn_ctx) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	BN_CTX_start(bn_ctx);

	e = BN_CTX_get(bn_ctx);
	e0 = BN_CTX_get(bn_ctx);
	e1 = BN_CTX_get(bn_ctx);
	mid = BN_CTX_get(bn_ctx);

	if ((NULL == e) || (NULL == e0) || (NULL == e1) || (NULL == mid)) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	res = ossl_bin2bn(e, e_bin, N_SIZE);
	CHECK_STATUS(res);

	// Split e to e0 and e1
	if ((BN_set_bit(mid, R_BITS) == 0) ||
	    (BN_mod(e0, e, mid, bn_ctx) == 0) ||
	    (BN_rshift(e1, e, R_BITS) == 0)) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	res = ossl_bn2bin(e0_bin, e0, R_SIZE);
	CHECK_STATUS(res);

	res = ossl_bn2bin(e1_bin, e1, R_SIZE);
	CHECK_STATUS(res);

EXIT:
	if (bn_ctx) {
		BN_CTX_end(bn_ctx);
		BN_CTX_free(bn_ctx);
	}

	return res;
}

// Perform a cyclic product by using OpenSSL
OQS_STATUS ossl_mod_inv(OUT uint8_t res_bin[R_SIZE],
                        IN const uint8_t a_bin[R_SIZE]) {
	OQS_STATUS res = OQS_SUCCESS;
	BN_CTX *bn_ctx = BN_CTX_new();
	BIGNUM *r = NULL;
	BIGNUM *a = NULL;

	if (NULL == bn_ctx) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	BN_CTX_start(bn_ctx);

	r = BN_CTX_get(bn_ctx);
	a = BN_CTX_get(bn_ctx);

	if ((NULL == r) || (NULL == a)) {
		ERR(OQS_EXTERNAL_LIB_ERROR_OPENSSL);
	}

	res = ossl_bin2bn(a, a_bin, R_SIZE);
	CHECK_STATUS(res);

	res = invert_poly(r, a, bn_ctx);
	CHECK_STATUS(res);

	res = ossl_bn2bin(res_bin, r, R_SIZE);
	CHECK_STATUS(res);
EXIT:
	if (bn_ctx) {
		BN_CTX_end(bn_ctx);
		BN_CTX_free(bn_ctx);
	}

	return res;
}
