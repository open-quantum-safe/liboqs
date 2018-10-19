/* FILE TO BE INCLUDED IN qTESLA_*.c, original code from sign.c */

/*********************************************************
* Name:        crypto_sign_keypair
* Description: generates a public and private key pair
* Parameters:  inputs:  none
*              outputs:
*              - unsigned char *pk: public key
*              - unsigned char *sk: secret key
* Returns:     0 for successful execution
**********************************************************/
static OQS_STATUS crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
	unsigned char randomness[CRYPTO_RANDOMBYTES], randomness_extended[4 * CRYPTO_SEEDBYTES];
	poly s, e, a, t;
	int nonce = 0; // Initialize domain separator for error and secret polynomials
#ifdef DEBUG
	ctr_keygen = 0;
#endif

	// Get randomness_extended <- seed_e, seed_s, seed_a, seed_y
	OQS_randombytes(randomness, CRYPTO_RANDOMBYTES);
	if (strcmp(CRYPTO_ALGNAME, "qTesla-I") == 0) {
		OQS_SHA3_shake128(randomness_extended, 4 * CRYPTO_SEEDBYTES, randomness, CRYPTO_RANDOMBYTES);
	} else /* III-size, III-speed */ {
		OQS_SHA3_shake256(randomness_extended, 4 * CRYPTO_SEEDBYTES, randomness, CRYPTO_RANDOMBYTES);
	}

	do { // Sample the error polynomial
#ifdef DEBUG
		ctr_keygen++;
#endif
		sample_gauss_poly(e, randomness_extended, ++nonce);
	} while (check_ES(e, (int) PARAM_KEYGEN_BOUND_E) != 0);

	do { // Sample the secret polynomial
#ifdef DEBUG
		ctr_keygen++;
#endif
		sample_gauss_poly(s, &randomness_extended[CRYPTO_SEEDBYTES], ++nonce);
	} while (check_ES(s, (int) PARAM_KEYGEN_BOUND_S) != 0);

	// Generate uniform polynomial "a"
	poly_uniform(a, &randomness_extended[2 * CRYPTO_SEEDBYTES]);

	// Compute the public key t = as+e
	poly_mul(t, a, s);
	poly_add(t, t, e);

	// Pack public and private keys
	pack_sk(sk, s, e, &randomness_extended[2 * CRYPTO_SEEDBYTES]);
	encode_pk(pk, t, &randomness_extended[2 * CRYPTO_SEEDBYTES]);

	return OQS_SUCCESS;
}

/***************************************************************
* OQS note: modified to satisfy the sign/verify API
* Name:        crypto_sign
* Description: outputs a signature for a given message m
* Parameters:  inputs:
*              - const unsigned char *m: message to be signed
*              - unsigned long long mlen: message length
*              - const unsigned char* sk: secret key
*              outputs:
*              - unsigned char *sm: signature
*              - unsigned long long *smlen: signature length*
* Returns:     0 for successful execution
***************************************************************/
static OQS_STATUS crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk) {
	unsigned char c[CRYPTO_C_BYTES], randomness[CRYPTO_SEEDBYTES], randomness_input[CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES + mlen];
	uint32_t pos_list[PARAM_W];
	int16_t sign_list[PARAM_W];
	poly y, v, Sc, Ec, z, a;
	int nonce = 0; // Initialize domain separator for sampling y
#ifdef DEBUG
	ctr_sign = 0;
	rejwctr = 0;
	rejyzctr = 0;
#endif

	// Get H(seed_y, r, m) to sample y
	OQS_randombytes(randomness_input + CRYPTO_RANDOMBYTES, CRYPTO_RANDOMBYTES);
	memcpy(randomness_input, &sk[CRYPTO_SECRETKEYBYTES - CRYPTO_SEEDBYTES], CRYPTO_SEEDBYTES);
	memcpy(randomness_input + CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES, m, mlen);
	if (strcmp(CRYPTO_ALGNAME, "qTesla-I") == 0) {
		OQS_SHA3_shake128(randomness, CRYPTO_SEEDBYTES, randomness_input, CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES + mlen);
	} else /* III-size, III-speed */ {
		OQS_SHA3_shake256(randomness, CRYPTO_SEEDBYTES, randomness_input, CRYPTO_RANDOMBYTES + CRYPTO_SEEDBYTES + mlen);
	}

	poly_uniform(a, &sk[CRYPTO_SECRETKEYBYTES - 2 * CRYPTO_SEEDBYTES]);

	while (1) {
#ifdef DEBUG
		ctr_sign++;
#endif
		sample_y(y, randomness, ++nonce); // Sample y uniformly at random from [-B,B]
		poly_mul(v, a, y);
		hash_vm(c, v, m, mlen);
		encode_c(pos_list, sign_list, c); // Generate c = Enc(c'), where c' is the hashing of v together with m
		sparse_mul16(Sc, sk, pos_list, sign_list);
		poly_add(z, y, Sc); // Compute z = y + sc

		if (test_rejection(z) != 0) { // Rejection sampling
#ifdef DEBUG
			rejyzctr++;
#endif
			continue;
		}

		sparse_mul16(Ec, sk + (sizeof(int16_t) * PARAM_N), pos_list, sign_list);
		poly_sub(v, v, Ec);

		if (test_v(v) != 0) {
#ifdef DEBUG
			rejwctr++;
#endif
			continue;
		}

		// Pack signature
		*smlen = CRYPTO_BYTES;
		encode_sig(sm, c, z);

		return OQS_SUCCESS;
	}
}

/************************************************************
* OQS note: modified to satisfy the sign/verify API
* Name:        crypto_sign_open
* Description: verification of a signature sm
* Parameters:  inputs:
*              - unsigned char *m: original (signed) message
*              - unsigned long long mlen: message length*
*              - const unsigned char *sm: signature
*              - unsigned long long smlen: signature length
*              - const unsigned char* pk: public Key
* Returns:     0 for valid signature
*              <0 for invalid signature
************************************************************/
static OQS_STATUS crypto_verify(unsigned char *m, unsigned long long mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk) {
	unsigned char c[CRYPTO_C_BYTES], c_sig[CRYPTO_C_BYTES], seed[CRYPTO_SEEDBYTES];
	uint32_t pos_list[PARAM_W];
	int16_t sign_list[PARAM_W];
	int32_t pk_t[PARAM_N];
	poly w, z, a, Tc;

	if (smlen < CRYPTO_BYTES)
		return OQS_ERROR;

	decode_sig(c, z, sm);
	if (test_z(z) != 0)
		return OQS_ERROR; // Check norm of z
	decode_pk((int32_t *) pk_t, seed, pk);
	poly_uniform(a, seed);
	encode_c(pos_list, sign_list, c);
	poly_mul(w, a, z);
	sparse_mul32(Tc, pk_t, pos_list, sign_list);
	poly_sub(w, w, Tc); // Compute w = az - tc
	hash_vm(c_sig, w, m, mlen);

	// Check if the calculated c matches c from the signature
	if (memcmp(c, c_sig, CRYPTO_C_BYTES))
		return OQS_ERROR;

	return OQS_SUCCESS;
}
