/******************************************************************************
 * BIKE -- Bit Flipping Key Encapsulation
 *
 * Copyright (c) 2017 Nir Drucker, Shay Gueron, Rafael Misoczki, Tobias Oder, Tim Gueneysu
 * (drucker.nir@gmail.com, shay.gueron@gmail.com, rafael.misoczki@intel.com, tobias.oder@rub.de, tim.gueneysu@rub.de)
 *
 * Permission to use this code for BIKE is granted.
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

#include "stdio.h"
#include "string.h"

#include "parallel_hash.h"
#include "openssl_utils.h"
#include "decode.h"
#include "sampling.h"
#include "aes_ctr_prf.h"
#include "conversions.h"

_INLINE_ OQS_STATUS encrypt(OUT ct_t *ct,
                            IN const uint8_t *e,
                            IN const uint8_t *ep __attribute__((__unused__)),
                            IN const pk_t *pk,
                            IN const seed_t *seed) {
	OQS_STATUS res = OQS_SUCCESS;

#if BIKE_VER != 2
	uint8_t c0[R_SIZE] = {0};
#endif
	uint8_t c1[R_SIZE] = {0};

	uint8_t e0[R_SIZE] = {0};
	uint8_t e1[R_SIZE] = {0};

	res = ossl_split_polynomial(e0, e1, e);
	CHECK_STATUS(res);

#if BIKE_VER == 1
	// ct = (m*pk0 + e0, m*pk1 + e1)
	uint8_t m[R_SIZE] = {0};
	sample_uniform_r_bits(m, seed, NO_RESTRICTION);
	res = cyclic_product(c0, m, pk->u.v.val0);
	CHECK_STATUS(res);
	res = cyclic_product(c1, m, pk->u.v.val1);
	CHECK_STATUS(res);
	res = ossl_add(ct->u.v.val0, c0, e0);
	CHECK_STATUS(res);
	res = ossl_add(ct->u.v.val1, c1, e1);
	CHECK_STATUS(res);
#elif BIKE_VER == 2
	BIKE_UNUSED(seed);
	// ct = (e1*pk1 + e0)
	res = cyclic_product(c1, e1, pk->u.v.val1);
	CHECK_STATUS(res);
	res = ossl_add(ct->u.v.val0, c1, e0);
	CHECK_STATUS(res);
	for (uint32_t i = 0; i < R_SIZE; i++)
		ct->u.v.val1[i] = 0;
#elif BIKE_VER == 3
	BIKE_UNUSED(seed);
	// ct = (e1*pk0 + e_extra, e1*pk1 + e0)
	res = cyclic_product(c0, e1, pk->u.v.val0);
	CHECK_STATUS(res);
	res = cyclic_product(c1, e1, pk->u.v.val1);
	CHECK_STATUS(res);
	res = ossl_add(ct->u.v.val0, c0, ep);
	CHECK_STATUS(res);
	res = ossl_add(ct->u.v.val1, c1, e0);
	CHECK_STATUS(res);
#endif

	EDMSG("c0: ");
	print((uint64_t *) ct->u.v.val0, R_BITS);
	EDMSG("c1: ");
	print((uint64_t *) ct->u.v.val1, R_BITS);
EXIT:
	return res;
}

// Generate the Shared Secres (K(e))
_INLINE_ OQS_STATUS get_ss(OUT ss_t *out, IN uint8_t *e) {
	OQS_STATUS res = OQS_SUCCESS;

	DMSG("    Enter get_ss.\n");

	sha384_hash_t hash = {0};

	// Calculate the hash
	parallel_hash(&hash, e, N_SIZE);

	// Truncate the final hash into K
	// By copying only the LSBs
	for (uint32_t i = 0; i < sizeof(ss_t); i++) {
		out->raw[i] = hash.u.raw[i];
	}

	DMSG("    Exit get_ss.\n");
	return res;
}

// Transpose a row into a column:
_INLINE_ void transpose(uint8_t col[R_BITS], uint8_t row[R_BITS]) {
	col[0] = row[0];
	for (uint64_t i = 1; i < R_BITS; ++i) {
		col[i] = row[(R_BITS) -i];
	}
}

_INLINE_ OQS_STATUS compute_syndrome(OUT syndrome_t *syndrome,
                                     IN const ct_t *ct,
                                     IN const sk_t *sk) {
	OQS_STATUS res = OQS_SUCCESS;
	uint8_t s_tmp_bytes[R_BITS] = {0};
	uint8_t s0[R_SIZE] = {0};

#if BIKE_VER == 1
	uint8_t s1[R_SIZE] = {0};
	// BIKE-1 syndrome: s = h0*c0 + h1*c1:
	res = cyclic_product(s0, sk->u.v.val0, ct->u.v.val0);
	CHECK_STATUS(res);
	res = cyclic_product(s1, sk->u.v.val1, ct->u.v.val1);
	CHECK_STATUS(res);
	res = ossl_add(s0, s0, s1);
	CHECK_STATUS(res);
#elif BIKE_VER == 2
	// BIKE-2 syndrome: s = c0*h0
	res = cyclic_product(s0, sk->u.v.val0, ct->u.v.val0);
	CHECK_STATUS(res);
#elif BIKE_VER == 3
	// BIKE3 syndrome: s = c0 + c1*h0
	res = cyclic_product(s0, ct->u.v.val1, sk->u.v.val0);
	CHECK_STATUS(res);
	res = ossl_add(s0, s0, ct->u.v.val0);
	CHECK_STATUS(res);
#endif

	// Store the syndrome in a bit array
	convertByteToBinary(s_tmp_bytes, s0, R_BITS);
	transpose(syndrome->raw, s_tmp_bytes);
EXIT:
	return res;
}

////////////////////////////////////////////////////////////////
// The three APIs below (keypair, enc, dec) are defined by NIST:
////////////////////////////////////////////////////////////////
OQS_API OQS_STATUS keypair(OUT unsigned char *pk, OUT unsigned char *sk) {
	// Convert to this implementation types
	sk_t *l_sk = (sk_t *) sk;
	pk_t *l_pk = (pk_t *) pk;
	OQS_STATUS res = OQS_SUCCESS;

	// For NIST DRBG_CTR
	double_seed_t seeds = {0};
	aes_ctr_prf_state_t h_prf_state = {0};

	// Get the entropy seeds
	get_seeds(&seeds);

	// sk = (h0, h1)
	uint8_t *h0 = l_sk->u.v.val0;
	uint8_t *h1 = l_sk->u.v.val1;

	DMSG("  Enter crypto_kem_keypair.\n");
	DMSG("    Calculating the secres key.\n");

#if BIKE_VER == 1
	uint8_t g[R_SIZE] = {0};
#elif BIKE_VER == 2
	uint8_t inv_h0[R_SIZE];
#elif BIKE_VER == 3
	uint8_t tmp1[R_SIZE] = {0};
	uint8_t *g = l_pk->u.v.val1;
#endif

	// Both h0 and h1 use the same context
	res = init_aes_ctr_prf_state(&h_prf_state, MAX_AES_INVOKATION, &seeds.s1);
	CHECK_STATUS(res);

	res = generate_sparse_rep(h0, DV, R_BITS, &h_prf_state);
	CHECK_STATUS(res);
	res = generate_sparse_rep(h1, DV, R_BITS, &h_prf_state);
	CHECK_STATUS(res);

	DMSG("    Calculating the public key.\n");

#if BIKE_VER == 1
	//  pk = (g*h1, g*h0)
	res = sample_uniform_r_bits(g, &seeds.s2, MUST_BE_ODD);
	CHECK_STATUS(res);

	cyclic_product(l_pk->u.v.val0, g, h1);
	CHECK_STATUS(res);
	cyclic_product(l_pk->u.v.val1, g, h0);
	CHECK_STATUS(res);
#elif BIKE_VER == 2
	// pk = (1, h1*h0^(-1))
	memset(l_pk->u.v.val0, 0, R_SIZE);

	// assume all elements initialized with 0
	l_pk->u.v.val0[0] = 1;
	res = ossl_mod_inv(inv_h0, h0);
	CHECK_STATUS(res);
	res = cyclic_product(l_pk->u.v.val1, h1, inv_h0);
	CHECK_STATUS(res);
#elif BIKE_VER == 3

	// pk = (h1 + g*h0, g)
	res = sample_uniform_r_bits(g, &seeds.s2, MUST_BE_ODD);
	CHECK_STATUS(res);
	res = cyclic_product(tmp1, g, h0);
	CHECK_STATUS(res);
	res = ossl_add(l_pk->u.v.val0, tmp1, h1);
	CHECK_STATUS(res);
#endif

	EDMSG("h0: ");
	print((uint64_t *) l_sk->u.v.val0, R_BITS);
	EDMSG("h1: ");
	print((uint64_t *) l_sk->u.v.val1, R_BITS);
	EDMSG("g0: ");
	print((uint64_t *) l_pk->u.v.val0, R_BITS);
	EDMSG("g1: ");
	print((uint64_t *) l_pk->u.v.val1, R_BITS);

EXIT:
	DMSG("  Exit crypto_kem_keypair.\n");
	return res;
}

// Encapsulate - pk is the public key,
//               ct is a key encapsulation message (ciphertext),
//               ss is the shared secret.
OQS_API OQS_STATUS encaps(OUT unsigned char *ct,
                          OUT unsigned char *ss,
                          IN const unsigned char *pk) {
	DMSG("  Enter crypto_kem_enc.\n");

	OQS_STATUS res = OQS_SUCCESS;

	// Convert to these implementation types
	const pk_t *l_pk = (pk_t *) pk;
	ct_t *l_ct = (ct_t *) ct;
	ss_t *l_ss = (ss_t *) ss;

	// For NIST DRBG_CTR
	double_seed_t seeds = {0};
	aes_ctr_prf_state_t e_prf_state = {0};

	// Get the entropy seeds
	get_seeds(&seeds);

	// Error vector:
	uint8_t e[N_SIZE] = {0};
#if BIKE_VER == 3
	uint8_t e_extra[R_SIZE] = {0};
#endif

	// Random data generator
	// Using first seed
	res = init_aes_ctr_prf_state(&e_prf_state, MAX_AES_INVOKATION, &seeds.s1);
	CHECK_STATUS(res);

	DMSG("    Generating error.\n");
	res = generate_sparse_rep(e, T1, N_BITS, &e_prf_state);
	CHECK_STATUS(res);

#if BIKE_VER == 3
	res = generate_sparse_rep(e_extra, T1 / 2, R_BITS, &e_prf_state);
	CHECK_STATUS(res);
#endif

	// Compute ct = enc(pk, e)
	// Using second seed
	DMSG("    Encrypting.\n");
#if BIKE_VER == 3
	res = encrypt(l_ct, e, e_extra, l_pk, &seeds.s2);
	CHECK_STATUS(res);
#else
	res = encrypt(l_ct, e, 0, l_pk, &seeds.s2);
	CHECK_STATUS(res);
#endif

	DMSG("    Generating shared secret.\n");
	res = get_ss(l_ss, e);
	CHECK_STATUS(res);

	EDMSG("ss: ");
	print((uint64_t *) l_ss->raw, sizeof(*l_ss) * 8);

EXIT:

	DMSG("  Exit crypto_kem_enc.\n");
	return res;
}

// Decapsulate - ct is a key encapsulation message (ciphertext),
//               sk is the private key,
//               ss is the shared secret
OQS_API OQS_STATUS decaps(OUT unsigned char *ss,
                          IN const unsigned char *ct,
                          IN const unsigned char *sk) {
	DMSG("  Enter crypto_kem_dec.\n");
	OQS_STATUS res = OQS_SUCCESS;

	// Convert to this implementation types
	const sk_t *l_sk = (sk_t *) sk;
	const ct_t *l_ct = (ct_t *) ct;
	ss_t *l_ss = (ss_t *) ss;

	DMSG("  Converting to compact rep.\n");
	uint32_t h0_compact[DV] = {0};
	uint32_t h1_compact[DV] = {0};
	convert2compact(h0_compact, l_sk->u.v.val0);
	convert2compact(h1_compact, l_sk->u.v.val1);

	DMSG("  Computing s.\n");
	syndrome_t syndrome;
	uint8_t e[R_BITS * 2] = {0};
	uint8_t eBytes[N_SIZE] = {0};
	int rc;
	// For BIKE-1 and BIKE-2, u = 0 (i.e. syndrome must become a zero-vector)
	uint32_t u = 0;
	res = compute_syndrome(&syndrome, l_ct, l_sk);
	CHECK_STATUS(res);

	DMSG("  Decoding.\n");
#if BIKE_VER == 3
	u = T1 / 2; // For BIKE-3, u = t/2
#endif
	rc = decode(e, syndrome.raw, h0_compact, h1_compact, u);

	if (rc == 0) {
		DMSG("    Decoding result: success\n");
	} else {
		DMSG("    Decoding result: failure!\n");
		ERR(OQS_ERR_KEM_BIKE_DECODING_FAILURE);
	}

	// Check if error weight equals T1:
	if (getHammingWeight(e, 2 * R_BITS) != T1) {
		MSG("Error weight is not t\n");
		ERR(OQS_ERR_KEM_BIKE_WEIGHT_IS_NOT_T);
	}

	convertBinaryToByte(eBytes, e, 2 * R_BITS);
	res = get_ss(l_ss, eBytes);
	CHECK_STATUS(res);

EXIT:

	DMSG("  Exit crypto_kem_dec.\n");
	return res;
}
