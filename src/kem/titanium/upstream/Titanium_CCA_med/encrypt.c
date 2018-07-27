/* ****************************** *
 * Titanium_CCA_med               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * CPA Encryption                 *
 * ****************************** */

#include "encrypt.h"
#include "param.h"
#include "fastrandombytes.h"
#include "sampler.h"
#include "ntt.h"
#include "fastmodulo.h"
#include "littleendian.h"
#include "pack.h"
#include <string.h>
#include <stdint.h>

int crypto_encrypt_keypair(unsigned char *pk, unsigned char *sk, const unsigned char *randomness)
{
	uint32_t s[N + D + K + 1];
	uint32_t a[T][N + D + K + 1];
	uint32_t b[T][D + K + 1];
	uint32_t e[T][D + K];
	unsigned char seed_sk[CRYPTO_RANDOMBYTES], seed_pk[CRYPTO_RANDOMBYTES];
	uint32_t i, j;
	
	fastrandombytes_setseed(randomness);
	
	/* generate seed_pk and seed_sk */
	fastrandombytes(seed_sk, CRYPTO_RANDOMBYTES);
	fastrandombytes(seed_pk, CRYPTO_RANDOMBYTES);
		
	fastrandombytes_setseed(seed_pk); /* use seed_pk to sample a_i */
	for (i = 0; i < T; i++)
	{
		sampler_zq(a[i], N, ZQ_BYTPCA); /* a_i <-- U(Z_q^{<n}[x]) */
		NTT_N_NDK(a[i]); /* transform a_i here */
	}
	
	fastrandombytes_setseed(seed_sk); /* change to seed_sk */
	
	/* sample s <-- NTT(lambda_s) over Z_q^{<n+d+k+1)[x] 
	 * generate two more samples, which wouldn't affect the result of middle-product a_i mp_{d+k} s
	 * (can save the adjustment of s to make the most significant bits of lambda_s be 0) */
	sampler_zq(s, N + D + K + 1, ZQ_BYTPCS);
	
	sampler_binomial(e); /* sample e_i <-- lambda_e over Z_q^{<d+k}[x] */
	
	for (i = 0; i < T; i++)
	{
		/* a_i <-- a_i mp_{d+k} s */
		for (j = 0; j < N + D + K + 1; j++)
		{
			a[i][j] = barrett_4q2((uint64_t)a[i][j] * (uint64_t)s[j]);
		}
		
		INTT_NDK_DK(a[i]);
		
		/* b_i <-- as_i + e_i */
		for (j = 0; j < D + K; j++)
		{
			b[i][j] = a[i][j] + e[i][j];
		}

		/* pre-transform b_i here to make Enc and Dec faster */ 
		b[i][D + K] = 0;
		NTT_DK_DK_INV(b[i]);
	}
	
	/* encode pk and sk to binary string */
	/* sk <-- seed_sk */
	memcpy(sk, seed_sk, CRYPTO_RANDOMBYTES);
	
	/* pk <-- (seed_pk, b_i) */
	memcpy(pk, seed_pk, CRYPTO_RANDOMBYTES); 
	for (i = 0; i < T; i++)
	{
		poly_encode(pk + CRYPTO_RANDOMBYTES + i * (D + K + 1) / 8 * Q_BITS, b[i], D + K + 1);
	}
	
	return 0;
}

int crypto_encrypt(unsigned char *c, unsigned long long *clen, const unsigned char *m, unsigned long long mlen, const unsigned char *pk, const unsigned char *randomness)
{
	uint32_t mu[D];
	uint32_t r[T][N + K + 1];
	uint32_t r2[T][N + K + 1];
	uint32_t a[T][N + K + 1];
	uint32_t b[T][D + K + 1];
	uint32_t c1[N + K + 1];
	uint32_t c2[D + K + 1];
	uint32_t i, j;
	
	/* reconstruct a_i by the seed saved in pk */
	fastrandombytes_setseed(pk);
	
	for(i = 0; i < T; i++)
	{
		sampler_zq(a[i], N, ZQ_BYTPCA); /* reconstruct a_i */
		NTT_N_NK(a[i]); /* transform a_i here */
	}
	
	/* take hash(m) as the seed */
	fastrandombytes_setseed(randomness);
	
	/* decode pk to polynomials */
	for (i = 0; i < T; i++) 
	{
		poly_decode(b[i], pk + CRYPTO_RANDOMBYTES + i * (D + K + 1) / 8 * Q_BITS, D + K + 1);
	}
	
	/* convert the message to a bit string and map {0,1}^D-->{0,Q/2}^D */
	memset(mu, 0, sizeof(mu));
	for (i = 0; i < mlen; i++)
	{
		for (j = 0; j < 8; j++)
		{
			mu[i * 8 + j] = ((m[i] >> j) & 0x1) * ((Q + 1) / 2);
		}
	}
	
	/* sample r_i <-- lambda_r over Z_q^{<k+1}[x] */
	sampler_zb(r);
	memcpy(r2, r, sizeof(r));
	for (i = 0; i < T; i++)
	{
		/* transform r_i here */
		NTT_K_NK(r[i]); 
		NTT_K_DK(r2[i]);
	}
	
	/* c1 <-- NTT(sum(r_i * a_i)) */
	for (j = 0; j < N + K + 1; j++)
	{
		c1[j] = barrett_8q2((uint64_t)r[0][j] * (uint64_t)a[0][j] + (uint64_t)r[1][j] * (uint64_t)a[1][j]);
		c1[j] = barrett_8q2((uint64_t)c1[j] + (uint64_t)r[2][j] * (uint64_t)a[2][j] + (uint64_t)r[3][j] * (uint64_t)a[3][j]);
		c1[j] = barrett_8q2((uint64_t)c1[j] + (uint64_t)r[4][j] * (uint64_t)a[4][j] + (uint64_t)r[5][j] * (uint64_t)a[5][j]);
		c1[j] = barrett_8q2((uint64_t)c1[j] + (uint64_t)r[6][j] * (uint64_t)a[6][j] + (uint64_t)r[7][j] * (uint64_t)a[7][j]);
		c1[j] = barrett_8q2((uint64_t)c1[j] + (uint64_t)r[8][j] * (uint64_t)a[8][j] + (uint64_t)r[9][j] * (uint64_t)a[9][j]);
	}
	
	/* c2 <-- sum(r_i mp_d b_i) + mu */
	for (j = 0; j < D + K + 1; j++)
	{
		c2[j] = barrett_8q2((uint64_t)r2[0][j] * (uint64_t)b[0][j] + (uint64_t)r2[1][j] * (uint64_t)b[1][j]);
		c2[j] = barrett_8q2((uint64_t)c2[j] + (uint64_t)r2[2][j] * (uint64_t)b[2][j] + (uint64_t)r2[3][j] * (uint64_t)b[3][j]);
		c2[j] = barrett_8q2((uint64_t)c2[j] + (uint64_t)r2[4][j] * (uint64_t)b[4][j] + (uint64_t)r2[5][j] * (uint64_t)b[5][j]);
		c2[j] = barrett_8q2((uint64_t)c2[j] + (uint64_t)r2[6][j] * (uint64_t)b[6][j] + (uint64_t)r2[7][j] * (uint64_t)b[7][j]);
		c2[j] = barrett_8q2((uint64_t)c2[j] + (uint64_t)r2[8][j] * (uint64_t)b[8][j] + (uint64_t)r2[9][j] * (uint64_t)b[9][j]);
	}
	
	INTT_DK_D(c2);
	
	for (i = 0; i < D; i++)
	{
		c2[i] = barrett_short(c2[i] + mu[i]);
	}
	
	/* encode c <-- (c1,c2) */
	poly_encode(c, c1, N + K + 1);
	poly_encode_c2(c + (N + K + 1) / 8 * Q_BITS, c2, D);
	
	*clen = (N + K + 1) / 8 * Q_BITS + D * C2_COMPRESSION_BYTE;
	
	return 0;
}

int crypto_encrypt_open(unsigned char *m, unsigned long long *mlen, const unsigned char *c, unsigned long long clen, const unsigned char *sk)
{
	uint32_t s[N + D + K + 1];
	uint32_t c1[N + D + K + 1];
	uint32_t c2[D];
	uint32_t c_prime[D];
	uint32_t i, j;

	/* recover sk */
	fastrandombytes_setseed(sk);
	sampler_zq(s, N + D + K + 1, ZQ_BYTPCS);
	
	/* decode c to polynomials c1,c2*/
	poly_decode(c1, c, N + K + 1);
	poly_decode_c2(c2, c + (N + K + 1) / 8 * Q_BITS, D);
	
	/* transform c1 back here (will save 1 NTT in the Encap) */
	INTT_NK_NK_INV(c1);
	
	/* c1 <-- c1 mp_d s*/
	NTT_NK_NDK(c1);
	
	for (i = 0; i < N + D + K + 1; i++)
	{
		c1[i] = barrett_4q2((uint64_t)c1[i] * (uint64_t)s[i]);
	}
	
	INTT_NDK_D(c1);

	/* c^prime <-- c2 - c1 mp_d s */	
	for (i = 0; i < D; i++)
	{
		c_prime[i] = Q2 + c2[i] - c1[i];
	}

	/* decode the message (still work for x+kQ) */
	memset(m, 0, D_BYTES);
	for (i = 0; i < D_BYTES; i++)
	{
		for (j = 0; j < 8; j++)
		{
			m[i] |= ((((c_prime[i * 8 + j] << 1) + Q / 2) / Q) & 0x1) << j;
		}
	}
	
	*mlen = D_BYTES;
	return 0;
}
