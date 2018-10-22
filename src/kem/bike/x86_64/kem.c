/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation".
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#include "stdio.h"
#include "string.h"

#include "sampling.h"
#include "parallel_hash.h"
#include "decode.h"
#include "gf2x_mul.h"
#include "openssl_utils.h"

#if BIKE_VER == 1
_INLINE_ OQS_STATUS encrypt(OUT ct_t *ct,
                            IN const pk_t *pk,
                            IN const seed_t *seed,
                            IN const split_e_t *splitted_e) {
	OQS_STATUS res = OQS_SUCCESS;
	padded_r_t m = {0};
	dbl_pad_ct_t p_ct;

	// Pad the public key
	const pad_pk_t p_pk = {{.u.v.val = PTRV(pk)[0], .u.v.pad = {0}},
	                       {.u.v.val = PTRV(pk)[1], .u.v.pad = {0}}};

	DMSG("    Sampling m.\n");
	res = sample_uniform_r_bits(VAL(m).raw, seed, NO_RESTRICTION);
	CHECK_STATUS(res);

	EDMSG("m:  ");
	print((uint64_t *) VAL(m).raw, R_BITS);

	DMSG("    Calculating the ciphertext.\n");

	gf2x_mod_mul(p_ct[0].u.qw, m.u.qw, p_pk[0].u.qw);
	gf2x_mod_mul(p_ct[1].u.qw, m.u.qw, p_pk[1].u.qw);

	DMSG("    Addding Error to the ciphertext.\n");

	gf2x_add(VAL(p_ct[0]).raw, VAL(p_ct[0]).raw, PTRV(splitted_e)[0].raw, R_SIZE);
	gf2x_add(VAL(p_ct[1]).raw, VAL(p_ct[1]).raw, PTRV(splitted_e)[1].raw, R_SIZE);

	// Copy the data outside
	PTRV(ct)
	[0] = VAL(p_ct[0]);
	PTRV(ct)
	[1] = VAL(p_ct[1]);

EXIT:
	EDMSG("c0: ");
	print((uint64_t *) PTRV(ct)[0].raw, R_BITS);
	EDMSG("c1: ");
	print((uint64_t *) PTRV(ct)[1].raw, R_BITS);

	OQS_MEM_cleanse(VAL(m).raw, sizeof(m));
	OQS_MEM_cleanse(VAL(p_ct[0]).raw, sizeof(p_ct));

	return res;
}

_INLINE_ OQS_STATUS calc_pk(OUT pk_t *pk,
                            IN const seed_t *g_seed,
                            IN const pad_sk_t p_sk) {
	OQS_STATUS res = OQS_SUCCESS;

	// Must intialized padding to zero!!
	padded_r_t g = {.u.v.pad = {0}};
	res = sample_uniform_r_bits(VAL(g).raw, g_seed, MUST_BE_ODD);
	CHECK_STATUS(res);

	EDMSG("g:  ");
	print((uint64_t *) VAL(g).raw, R_BITS);

	// PK is dbl padded because modmul require scratch space for the multiplication result
	dbl_pad_pk_t p_pk = {0};

	// Calculate (g0, g1) = (g*h1, g*h0)
	gf2x_mod_mul(p_pk[0].u.qw, g.u.qw, p_sk[1].u.qw);
	gf2x_mod_mul(p_pk[1].u.qw, g.u.qw, p_sk[0].u.qw);

	// Copy the data outside
	PTRV(pk)
	[0] = VAL(p_pk[0]);
	PTRV(pk)
	[1] = VAL(p_pk[1]);

EXIT:

	EDMSG("g0: ");
	print((uint64_t *) PTRV(pk)[0].raw, R_BITS);
	EDMSG("g1: ");
	print((uint64_t *) PTRV(pk)[1].raw, R_BITS);

	OQS_MEM_cleanse(VAL(g).raw, sizeof(g));

	return res;
}

#endif

#if BIKE_VER == 2
_INLINE_ void encrypt(OUT ct_t *ct,
                      IN const pk_t *pk,
                      IN const split_e_t *splitted_e) {
	// Pad the public key
	const pad_pk_t p_pk = {{.v.val = *pk, .v.pad = {0}}};
	dbl_pad_ct_t p_ct;

	cyclic_product(VAL(p_ct).raw,
	               PTRV(splitted_e)[1].raw,
	               VAL(p_pk).raw);

	gf2x_add(VAL(p_ct).raw,
	         VAL(p_ct).raw,
	         PTRV(splitted_e)[0].raw,
	         R_SIZE);

	*ct = VAL(p_ct);

	EDMSG("c:  ");
	print((uint64_t *) ct->raw, R_BITS);

	OQS_MEM_cleanse(VAL(p_ct).raw, sizeof(p_ct));
}

_INLINE_ void calc_pk(OUT pk_t *pk, IN OUT sk_t *sk) {
	r_t inv;

	ossl_mod_inv(inv.raw, PTR(sk).bin[0].raw);
	cyclic_product(pk->raw, PTR(sk).bin[1].raw, inv.raw);

	PTR(sk).pk = *pk;

	EDMSG("g0: ");
	print((uint64_t *) pk->raw, R_BITS);

	OQS_MEM_cleanse(inv.raw, sizeof(r_t));
}

#if BATCH_SIZE > 1
typedef struct batch_ctx_s {
	uint32_t cnt;
	padded_r_t h0[BATCH_SIZE];
	compressed_idx_dv_t sk_wlist[BATCH_SIZE];
	r_t tmp1[BATCH_SIZE];
	r_t tmp0[BATCH_SIZE];
	r_t inv;
} batch_ctx_t;

_INLINE_ OQS_STATUS init_batch(IN OUT aes_ctr_prf_state_t *h_prf_state,
                               IN OUT batch_ctx_t *ctx) {
	OQS_STATUS res = OQS_SUCCESS;

	if (ctx->cnt != 0) {
		// Already initialized
		return res;
	}

	// First time - Init ad calc
	for (uint32_t i = 0; i < BATCH_SIZE; ++i) {
		res = generate_sparse_fake_rep(ctx->h0[i].val.raw,
		                               ctx->sk_wlist[i].val,
		                               DV, FAKE_DV, R_BITS,
		                               sizeof(ctx->h0[i].val),
		                               h_prf_state);
		CHECK_STATUS(res);
	}

	ctx->tmp0[0] = ctx->h0[0].val;

	for (uint32_t i = 1; i < BATCH_SIZE; ++i) {
		cyclic_product(ctx->tmp0[i].raw,
		               ctx->tmp0[i - 1].raw,
		               ctx->h0[i].val.raw);
	}

	ossl_mod_inv(ctx->tmp1[BATCH_SIZE - 1].raw, ctx->tmp0[BATCH_SIZE - 1].raw);

	for (uint32_t i = (BATCH_SIZE - 2); i >= 1; i--) {
		cyclic_product(ctx->tmp1[i].raw,
		               ctx->tmp1[i + 1].raw,
		               ctx->h0[i + 1].val.raw);
	}

EXIT:

	return res;
}

_INLINE_ void get_batch_keys(OUT pk_t *pk,
                             IN OUT sk_t *sk,
                             IN OUT batch_ctx_t *ctx) {
	// Set the private key
	PTR(sk).bin[0] = ctx->h0[ctx->cnt].val;
	PTR(sk).wlist[0] = ctx->sk_wlist[ctx->cnt];

	if (0 == ctx->cnt) {
		cyclic_product(ctx->inv.raw,
		               ctx->tmp1[1].raw,
		               ctx->h0[1].val.raw);
	} else {
		cyclic_product(ctx->inv.raw,
		               ctx->tmp1[ctx->cnt].raw,
		               ctx->tmp0[ctx->cnt - 1].raw);
	}

	// Set the public key
	cyclic_product(pk->raw, PTR(sk).bin[1].raw, ctx->inv.raw);

	EDMSG("g0: ");
	print((uint64_t *) pk->raw, R_BITS);

	// Increment the counter
	ctx->cnt = (ctx->cnt + 1) % BATCH_SIZE;
}

#endif

#endif

#if BIKE_VER == 3
_INLINE_ OQS_STATUS encrypt(OUT ct_t *ct,
                            IN const pk_t *pk,
                            IN const split_e_t *splitted_e,
                            IN OUT aes_ctr_prf_state_t *e_prf_state) {
	compressed_idx_t_t dummy;
	padded_r_t e_extra;
	OQS_STATUS res = generate_sparse_rep(VAL(e_extra).raw, dummy.val, T1 / 2,
	                                     R_BITS, sizeof(e_extra), e_prf_state);
	CHECK_STATUS(res);

	// ct = (e1*pk0 + e_extra, e1*pk1 + e0)
	cyclic_product(PTRV(ct)[0].raw, PTRV(splitted_e)[1].raw, PTRV(pk)[0].raw);
	cyclic_product(PTRV(ct)[1].raw, PTRV(splitted_e)[1].raw, PTRV(pk)[1].raw);
	gf2x_add(PTRV(ct)[0].raw, PTRV(ct)[0].raw, VAL(e_extra).raw, R_SIZE);
	gf2x_add(PTRV(ct)[1].raw, PTRV(ct)[1].raw, PTRV(splitted_e)[0].raw, R_SIZE);

EXIT:
	EDMSG("c0: ");
	print((uint64_t *) PTRV(ct)[0].raw, R_BITS);
	EDMSG("c1: ");
	print((uint64_t *) PTRV(ct)[1].raw, R_BITS);

	OQS_MEM_cleanse(VAL(e_extra).raw, sizeof(e_extra));

	return res;
}

_INLINE_ OQS_STATUS calc_pk(OUT pk_t *pk,
                            IN const seed_t *g_seed,
                            IN OUT sk_t *sk) {
	OQS_STATUS res = OQS_SUCCESS;
	r_t tmp1;

	// pk = (h1 + g*h0, g)
	padded_r_t g = {.u.v.pad = {0}};
	res = sample_uniform_r_bits(VAL(g).raw, g_seed, NO_RESTRICTION);
	CHECK_STATUS(res);

	cyclic_product(tmp1.raw, VAL(g).raw, PTR(sk).bin[0].raw);
	gf2x_add(PTRV(pk)[0].raw, tmp1.raw, PTR(sk).bin[1].raw, R_SIZE);

	// Copy g to pk[1]
	PTRV(pk)
	[1] = VAL(g);

	// Store the pk for later use
	PTR(sk).pk = *pk;

EXIT:
	EDMSG("g0: ");
	print((uint64_t *) PTRV(pk)[0].raw, R_BITS);
	EDMSG("g1: ");
	print((uint64_t *) PTRV(pk)[1].raw, R_BITS);

	OQS_MEM_cleanse(VAL(g).raw, sizeof(g));
	OQS_MEM_cleanse(tmp1.raw, sizeof(tmp1));

	return res;
}

#endif

// Generate the Shared Secret (K(e))
_INLINE_ void get_ss(OUT ss_t *out, IN const e_t *e) {
	DMSG("    Enter get_ss.\n");

	// Calculate the hash
	sha_hash_t hash = {0};
	parallel_hash(&hash, e->raw, sizeof(*e));

	// Truncate the final hash into K by copying only the LSBs
	memcpy(out->raw, hash.u.raw, sizeof(*out));

	OQS_MEM_cleanse(hash.u.raw, sizeof(hash));
	DMSG("    Exit get_ss.\n");
}

////////////////////////////////////////////////////////////////
// The three APIs below (keygeneration, encapsulate, decapsulate) are defined by NIST:
////////////////////////////////////////////////////////////////
OQS_API OQS_STATUS keypair(OUT unsigned char *pk, OUT unsigned char *sk) {
	// Convert to this implementation types
	sk_t *l_sk = (sk_t *) sk;
	pk_t *l_pk = (pk_t *) pk;

	OQS_STATUS res = OQS_SUCCESS;

	// For DRBG and AES_PRF
	double_seed_t seeds = {0};
	aes_ctr_prf_state_t h_prf_state = {0};

	// Get the entrophy seeds
	get_seeds(&seeds);

	DMSG("  Enter crypto_kem_keypair.\n");
	DMSG("    Calculating the secret key.\n");

	// Both h0 and h1 use the same context
	init_aes_ctr_prf_state(&h_prf_state, MAX_AES_INVOKATION, &seeds.s1);

	// Padded for internal use
	// We don't want to send the padded data outside to save BW.
	pad_sk_t p_sk = {0};

	// Make sure that the wlists are zeroed for the KATs.
	memset(l_sk->u.raw, 0, sizeof(l_sk->u.raw));

#if BATCH_SIZE > 1
	static batch_ctx_t batch_ctx = {.cnt = 0};
	res = init_batch(&h_prf_state, &batch_ctx);
	CHECK_STATUS(res);
#else
	res = generate_sparse_fake_rep(VAL(p_sk[0]).raw, PTR(l_sk).wlist[0].val, DV,
	                               FAKE_DV, R_BITS, sizeof(p_sk[0]), &h_prf_state);
	CHECK_STATUS(res);

	// Copy data
	PTR(l_sk).bin[0] = VAL(p_sk[0]);
#endif
	res = generate_sparse_fake_rep(VAL(p_sk[1]).raw, PTR(l_sk).wlist[1].val, DV,
	                               FAKE_DV, R_BITS, sizeof(p_sk[1]), &h_prf_state);
	CHECK_STATUS(res);

	// Copy data
	PTR(l_sk).bin[1] = VAL(p_sk[1]);

	DMSG("    Calculating the public key.\n");

#if BIKE_VER == 1
	res = calc_pk(l_pk, &seeds.s2, p_sk);
	CHECK_STATUS(res);
#elif BIKE_VER == 2
#if BATCH_SIZE > 1
	get_batch_keys(l_pk, l_sk, &batch_ctx);
#else
	calc_pk(l_pk, l_sk);
#endif
#elif BIKE_VER == 3
	res = calc_pk(l_pk, &seeds.s2, l_sk);
	CHECK_STATUS(res);
#endif

EXIT:
	EDMSG("h0: ");
	print((uint64_t *) &PTR(l_sk).bin[0], R_BITS);
	EDMSG("h1: ");
	print((uint64_t *) &PTR(l_sk).bin[1], R_BITS);
	EDMSG("h0c:");
	print((uint64_t *) &PTR(l_sk).wlist[0], SIZEOF_BITS(compressed_idx_dv_t));
	EDMSG("h1c:");
	print((uint64_t *) &PTR(l_sk).wlist[1], SIZEOF_BITS(compressed_idx_dv_t));

	OQS_MEM_cleanse(seeds.s1.u.raw, sizeof(seeds));
	OQS_MEM_cleanse((uint8_t *) &h_prf_state, sizeof(h_prf_state));
	OQS_MEM_cleanse(VAL(p_sk[0]).raw, sizeof(p_sk));

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

	// Convert to this implementation types
	const pk_t *l_pk = (pk_t *) pk;
	ct_t *l_ct = (ct_t *) ct;
	ss_t *l_ss = (ss_t *) ss;
	padded_e_t e = {0};

	// For NIST DRBG_CTR
	double_seed_t seeds = {0};
	aes_ctr_prf_state_t e_prf_state = {0};

	// Get the entrophy seeds
	get_seeds(&seeds);

	// Random data generator
	// Using first seed
	init_aes_ctr_prf_state(&e_prf_state, MAX_AES_INVOKATION, &seeds.s1);

	DMSG("    Generating error.\n");
	compressed_idx_t_t dummy;
	res = generate_sparse_rep(VAL(e).raw, dummy.val, T1, N_BITS, sizeof(e), &e_prf_state);
	CHECK_STATUS(res);

	EDMSG("e:  ");
	print((uint64_t *) VAL(e).raw, sizeof(VAL(e)) * 8);

	// Split e into e0 and e1. Initialization is done in split_e
	split_e_t splitted_e;
	split_e(&splitted_e, &VAL(e));

	EDMSG("e0: ");
	print((uint64_t *) VAL(splitted_e)[0].raw, R_BITS);
	EDMSG("e1: ");
	print((uint64_t *) VAL(splitted_e)[1].raw, R_BITS);

	// Computing ct = enc(pk, e)
	// Using second seed
	DMSG("    Encrypting.\n");
#if BIKE_VER == 1
	res = encrypt(l_ct, l_pk, &seeds.s2, &splitted_e);
#elif BIKE_VER == 2
	encrypt(l_ct, l_pk, &splitted_e);
#elif BIKE_VER == 3
	res = encrypt(l_ct, l_pk, &splitted_e, &e_prf_state);
#endif

	CHECK_STATUS(res);

	DMSG("    Generating shared secret.\n");
	get_ss(l_ss, &VAL(e));

EXIT:
	EDMSG("ss: ");
	print((uint64_t *) l_ss->raw, SIZEOF_BITS(*l_ss));

	OQS_MEM_cleanse(seeds.s1.u.raw, sizeof(seeds));
	OQS_MEM_cleanse(VAL(e).raw, sizeof(e));
	OQS_MEM_cleanse((uint8_t *) &e_prf_state, sizeof(e_prf_state));
	OQS_MEM_cleanse(VAL(splitted_e)[0].raw, sizeof(splitted_e));

	DMSG("  Exit crypto_kem_enc.\n");
	return res;
}

// Decapsulate - ct is a key encapsulation message (ciphertext),
//               sk is the private key,
//               ss is the shared secret
OQS_API OQS_STATUS decaps(OUT unsigned char *ss,
                          IN const unsigned char *ct,
                          IN const unsigned char *sk) {
	OQS_STATUS res = OQS_SUCCESS;
	DMSG("  Enter crypto_kem_dec.\n");

	// Convert to this implementation types
	const sk_t *l_sk = (sk_t *) sk;
	const ct_t *l_ct = (ct_t *) ct;
	ss_t *l_ss = (ss_t *) ss;

	// Force zero initialization
	syndrome_t syndrome = {0};
	e_t e = {0};

	DMSG("  Computing s.\n");
	compute_syndrome(&syndrome, l_ct, l_sk);

	DMSG("  Decoding.\n");

	if (decode(&e, &syndrome, l_ct, l_sk, U_ERR) != 0) {
		DMSG("    Decoding result: failure!\n");
		ERR(OQS_ERR_KEM_BIKE_DECODING_FAILURE);
	}

	// Check if the error weight equals T1
	if (count_ones(e.raw, sizeof(e)) != T1) {
		MSG("    Error weight is not t\n");
		ERR(OQS_ERR_KEM_BIKE_WEIGHT_IS_NOT_T);
	}

	get_ss(l_ss, &e);

EXIT:
	OQS_MEM_cleanse(syndrome.u.raw, sizeof(syndrome));
	OQS_MEM_cleanse(e.raw, sizeof(e));

	DMSG("  Exit crypto_kem_dec.\n");
	return res;
}
