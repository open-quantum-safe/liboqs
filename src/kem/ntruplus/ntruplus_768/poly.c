#include <stdint.h>
#include "params.h"
#include "poly.h"
#include "ntt.h"
#include "symmetric.h"

/*************************************************
* Name:        load16_littleendian
*
* Description: load 2 bytes into a 16-bit integer
*              in little-endian order
*
* Arguments:   - const uint8_t *x: pointer to input byte array
*
* Returns 16-bit unsigned integer loaded from x
**************************************************/
static uint16_t load16_littleendian(const uint8_t x[2])
{
	uint16_t r;
	r  = (uint32_t)x[0];
	r |= (uint32_t)x[1] << 8;;
	return r;
}

/*************************************************
* Name:        crepmod3
*
* Description: Compute modulus 3 operation
*
* Arguments: - poly *a: pointer to intput integer to be reduced
*
* Returns:     integer in {-1,0,1} congruent to a modulo 3.
**************************************************/
static int16_t crepmod3(int16_t a)
{
	a += (a >> 15) & NTRUPLUS_Q;
	a -= (NTRUPLUS_Q+1)/2;
	a += (a >> 15) & NTRUPLUS_Q;
	a -= (NTRUPLUS_Q-1)/2;

	a  = (a >> 8) + (a & 255);
	a  = (a >> 4) + (a & 15);
	a  = (a >> 2) + (a & 3);
	a  = (a >> 2) + (a & 3);
	a -= 3;
	a += ((a + 1) >> 15) & 3;
	return a;
}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for NTRUPLUS_POLYBYTES bytes)
*              - poly *a:    pointer to input polynomial
**************************************************/
void poly_tobytes(uint8_t r[NTRUPLUS_POLYBYTES], const poly *a)
{
	int16_t t[4];

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			t[0]  = a->coeffs[64*j + i];
			t[0] += (t[0] >> 15) & NTRUPLUS_Q;
			t[1]  = a->coeffs[64*j + i + 16];
			t[1] += (t[1] >> 15) & NTRUPLUS_Q;
			t[2]  = a->coeffs[64*j + i + 32];
			t[2] += (t[2] >> 15) & NTRUPLUS_Q;			
			t[3]  = a->coeffs[64*j + i + 48];
			t[3] += (t[3] >> 15) & NTRUPLUS_Q;

			r[96*j + 2*i +  0] = t[0];
			r[96*j + 2*i +  1] = (t[0] >> 8) | (t[1] << 4);			
			r[96*j + 2*i + 32] = (t[1] >> 4);
			r[96*j + 2*i + 33] = t[2];
			r[96*j + 2*i + 64] = (t[2] >> 8) | (t[3] << 4); 
			r[96*j + 2*i + 65] = (t[3] >> 4); 
		}
	}	
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r:          pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of NTRUPLUS_POLYBYTES bytes)
**************************************************/
void poly_frombytes(poly *r, const uint8_t a[NTRUPLUS_POLYBYTES])
{
	unsigned char t[6];

	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 12; j++)
		{
			t[0] = a[96*j + 2*i];
			t[1] = a[96*j + 2*i + 1];
			t[2] = a[96*j + 2*i + 32];
			t[3] = a[96*j + 2*i + 33];
			t[4] = a[96*j + 2*i + 64];
			t[5] = a[96*j + 2*i + 65];								

			r->coeffs[64*j + i +  0] = t[0]      | ((int16_t)t[1] & 0xf) << 8;
			r->coeffs[64*j + i + 16] = t[1] >> 4 | ((int16_t)t[2]      ) << 4;
			r->coeffs[64*j + i + 32] = t[3]      | ((int16_t)t[4] & 0xf) << 8;
			r->coeffs[64*j + i + 48] = t[4] >> 4 | ((int16_t)t[5]      ) << 4;
		}
	}
}

/*************************************************
* Name:        poly_cbd1
*
* Description: Sample a polynomial deterministically from a random,
*              with output polynomial close to centered binomial distribution
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *buf: pointer to input random
*                                     (of length NTRUPLUS_N/4 bytes)
**************************************************/
void poly_cbd1(poly *r, const unsigned char buf[NTRUPLUS_N/4])
{
	uint16_t t1, t2;

	for(int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			t1 = load16_littleendian(buf + 32*i + 2*j);
			t2 = load16_littleendian(buf + 32*i + 2*j + 96);

			for(int k = 0; k < 16; k++)
			{
				r->coeffs[256*i + 16*k + j] = (t1 & 0x1) - (t2 & 0x1);

				t1 >>= 1;   
				t2 >>= 1;
			}
		}
	}
}

/*************************************************
* Name:        poly_sotp
*
* Description: Encode a message deterministically using SOTP and a random,
			   with output polynomial close to centered binomial distribution
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
*              - const uint8_t *buf: pointer to input random
**************************************************/
void poly_sotp(poly *r, const uint8_t *msg, const uint8_t *buf)
{
    uint8_t tmp[NTRUPLUS_N/4];

    for(int i = 0; i < NTRUPLUS_N/8; i++)
    {
         tmp[i] = buf[i]^msg[i];
    }

    for(int i = NTRUPLUS_N/8; i < NTRUPLUS_N/4; i++)
    {
         tmp[i] = buf[i];
    }

	poly_cbd1(r, tmp);
}

/*************************************************
* Name:        poly_sotp_inv
*
* Description: Decode a message deterministically using SOTP_INV and a random
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - const poly *a: pointer to iput polynomial
*              - const uint8_t *buf: pointer to input random
*
* Returns 0 (success) or 1 (failure)
**************************************************/
int poly_sotp_inv(unsigned char *msg, const poly *a, const unsigned char *buf)
{
	uint32_t t1, t2, t3, t4;
	uint32_t r = 0;

	for(int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			t1 = load16_littleendian(buf + 32*i + 2*j);
			t2 = load16_littleendian(buf + 32*i + 2*j + 96);
			t3 = 0;

			for(int k = 0; k < 16; k++)
			{
				t4 = t2 & 0x1;
				t4 += a->coeffs[256*i + 16*k + j];
				r |= t4;
				t4 = (t4^t1) & 0x1;
				t3 ^= t4 << k;

				t1 >>= 1;
				t2 >>= 1;
			}

			msg[32*i + 2*j    ] = t3;
			msg[32*i + 2*j + 1] = t3 >> 8;
		}
	}

	r = r >> 1;
	r = (-(uint64_t)r) >> 63;

	return r;
}

/*************************************************
* Name:        poly_ntt
*
* Description: Computes number-theoretic transform (NTT)
*
* Arguments:   - poly *r: pointer to output polynomial
*              - poly *a: pointer to input polynomial
**************************************************/
void poly_ntt(poly *r, const poly *a)
{
	ntt(r->coeffs, a->coeffs);
}

/*************************************************
* Name:        poly_invntt
*
* Description: Computes inverse of number-theoretic transform (NTT)
*
* Arguments:   - poly *r: pointer to output polynomial
*              - poly *a: pointer to input polynomial
**************************************************/
void poly_invntt(poly *r, const poly *a)
{
	invntt(r->coeffs, a->coeffs);
}

/*************************************************
* Name:        poly_baseinv
*
* Description: Inversion of polynomial in NTT domain
*
* Arguments:   - poly *r:       pointer to output polynomial
*              - const poly *a: pointer to input polynomial
* 
* Returns:     integer
**************************************************/
int poly_baseinv(poly *r, const poly *a)
{
	for(int i = 0; i < NTRUPLUS_N/8; ++i)
	{
		if(baseinv(r->coeffs + 8*i, a->coeffs + 8*i, zetas_plant[96 + i])) return 1;
	 }

	return 0;
}

/*************************************************
* Name:        poly_basemul
*
* Description: Multiplication of two polynomials in NTT domain
*
* Arguments:   - poly *r:       pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
**************************************************/
void poly_basemul(poly *r, const poly *a, const poly *b)
{
	for(int i = 0; i < NTRUPLUS_N/8; ++i)
	{
		basemul(r->coeffs + 8*i, a->coeffs + 8*i, b->coeffs + 8*i, zetas[96 + i]);
		basemul(r->coeffs + 8*i + 4, a->coeffs + 8*i + 4, b->coeffs + 8*i + 4, -zetas[96 + i]);
	}
}

/*************************************************
* Name:        poly_basemul_add
*
* Description: Multiplication then addition of three polynomials in NTT domain
*
* Arguments:   - poly *r:       pointer to output polynomial
*              - const poly *a: pointer to first input polynomial
*              - const poly *b: pointer to second input polynomial
*              - const poly *c: pointer to third input polynomial
**************************************************/
void poly_basemul_add(poly *r, const poly *a, const poly *b, const poly *c)
{
	for(int i = 0; i < NTRUPLUS_N/8; ++i)
	{
		basemul_add(r->coeffs + 8*i, a->coeffs + 8*i, b->coeffs + 8*i, c->coeffs + 8*i, zetas[96 + i]);
		basemul_add(r->coeffs + 8*i + 4, a->coeffs + 8*i + 4, b->coeffs + 8*i + 4, c->coeffs + 8*i + 4, -zetas[96 + i]);
	}
}

/*************************************************
* Name:        poly_sub
*
* Description: Subtract two polynomials; no modular reduction is performed
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to first input polynomial
*            - const poly *b: pointer to second input polynomial
**************************************************/
void poly_sub(poly *r, const poly *a, const poly *b)
{
	for(int i = 0; i < NTRUPLUS_N; ++i)
		r->coeffs[i] = a->coeffs[i] - b->coeffs[i];
}

/*************************************************
* Name:        poly_triple
*
* Description: Multiply polynomial by 3; no modular reduction is performed
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to input polynomial
**************************************************/
void poly_triple(poly *r, const poly *a) 
{
	for(int i = 0; i < NTRUPLUS_N; ++i)
		r->coeffs[i] = 3*a->coeffs[i];
}

/*************************************************
* Name:        poly_crepmod3
*
* Description: Compute modulus 3 operation to polynomial
*
* Arguments: - poly *r: pointer to output polynomial
*            - const poly *a: pointer to input polynomial
**************************************************/
void poly_crepmod3(poly *r, const poly *a)
{
	for(int i = 0; i < NTRUPLUS_N; i++)
    	r->coeffs[i] = crepmod3(a->coeffs[i]);
}
