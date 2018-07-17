/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: high-level functions of the signature scheme
**************************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "api.h"
#include "params.h"
#include "poly.h"
#include "sample.h"
#include "sha3/fips202.h"
#include "random/random.h"

#ifdef DEBUG
unsigned long long rejwctr;
unsigned long long rejyzctr;
unsigned long long ctr_keygen;
unsigned long long ctr_sign;
#endif


static void pack_sk(unsigned char *sk, poly s, poly e, unsigned char *seeds)
{ // Pack secret key sk
  int i;
  int16_t *isk = (int16_t *)sk;

  for (i=0; i<PARAM_N; i++)
    isk[i] = s[i];

  isk += PARAM_N;
  for (i=0; i<(PARAM_N); i++)
    isk[i] = e[i];
  
  memcpy(&isk[PARAM_N], seeds, 2*CRYPTO_SEEDBYTES);
} 


static void encode_pk(unsigned char *pk, const poly t, const unsigned char *seedA)
{ // Encode public key pk
  unsigned int i, j=0;
  uint32_t *pt = (uint32_t*)pk;
  
  for (i=0; i<(PARAM_N*PARAM_Q_LOG/32); i+=(PARAM_Q_LOG/8)) {
    pt[i  ] = t[j] | (t[j+1] << 24);
    pt[i+1] = (t[j+1] >>  8) | (t[j+2] << 16); 
    pt[i+2] = (t[j+2] >> 16) | (t[j+3] <<  8);
    j += 4;
  }
  memcpy(&pk[PARAM_N*PARAM_Q_LOG/8], seedA, CRYPTO_SEEDBYTES);
}


static void decode_pk(int32_t *pk, unsigned char *seedA, const unsigned char *pk_in)
{ // Decode public key pk
  unsigned int i, j=0;
  uint32_t *pt = (uint32_t*)pk_in, *pp = (uint32_t*)pk, mask24 = (1<<PARAM_Q_LOG)-1;

  for (i=0; i<PARAM_N; i+=4) {
    pp[i  ] = pt[j] & mask24; 
    pp[i+1] = ((pt[j  ] >> 24) | (pt[j+1] <<  8)) & mask24; 
    pp[i+2] = ((pt[j+1] >> 16) | (pt[j+2] << 16)) & mask24; 
    pp[i+3] = pt[j+2] >> 8;
    j += 3;
  }   
  memcpy(seedA, &pk_in[PARAM_N*PARAM_Q_LOG/8], CRYPTO_SEEDBYTES);
}


static void encode_sig(unsigned char *sm, unsigned char *c, poly z)
{ // Encode signature sm
  unsigned int i, j=0;
  uint64_t *t = (uint64_t*)z;
  uint32_t *pt = (uint32_t*)sm;
  
  for (i=0; i<(PARAM_N*PARAM_D/32); i+=(PARAM_D/2)) {
    pt[i   ] = (t[j] & ((1<<22)-1)) | (t[j+1] << 22);
    pt[i+ 1] = ((t[j+ 1] >> 10) & ((1<<12)-1)) | (t[j+2] << 12);
    pt[i+ 2] = ((t[j+ 2] >> 20) & ((1<< 2)-1)) | ((t[j+ 3] & ((1<<22)-1)) << 2) | (t[j+ 4] << 24);
    pt[i+ 3] = ((t[j+ 4] >>  8) & ((1<<14)-1)) | (t[j+5] << 14);
    pt[i+ 4] = ((t[j+ 5] >> 18) & ((1<<4)-1 )) | ((t[j+ 6] & ((1<<22)-1)) << 4) | (t[j+ 7] << 26);
    pt[i+ 5] = ((t[j+ 7] >>  6) & ((1<<16)-1)) | (t[j+8] << 16);
    pt[i+ 6] = ((t[j+ 8] >> 16) & ((1<<6)-1 )) | ((t[j+ 9] & ((1<<22)-1)) << 6) | (t[j+10] << 28);
    pt[i+ 7] = ((t[j+10] >>  4) & ((1<<18)-1)) | (t[j+11] << 18);
    pt[i+ 8] = ((t[j+11] >> 14) & ((1<<8)-1 )) | ((t[j+12] & ((1<<22)-1)) << 8) | (t[j+13] << 30);
    pt[i+ 9] = ((t[j+13] >>  2) & ((1<<20)-1)) | (t[j+14] << 20);
    pt[i+10] = ((t[j+14] >> 12) & ((1<<10)-1)) | (t[j+15] << 10);
    j += 16;
  }
  memcpy(&sm[PARAM_N*PARAM_D/8], c, CRYPTO_C_BYTES);
}


static void decode_sig(unsigned char *c, poly z, const unsigned char *sm)
{ // Decode signature sm
  unsigned int i, j=0;
  uint32_t *pt = (uint32_t*)sm;

  for (i=0; i<PARAM_N; i+=16) {
    z[i   ] = ((int32_t)pt[j+ 0] << 10) >> 10; 
    z[i+ 1] = (int32_t)(pt[j+ 0] >> 22) | ((int32_t)(pt[j+ 1] << 20) >> 10); 
    z[i+ 2] = (int32_t)(pt[j+ 1] >> 12) | ((int32_t)(pt[j+ 2] << 30) >> 10);
    z[i+ 3] = (int32_t)(pt[j+ 2] <<  8) >> 10; 
    z[i+ 4] = (int32_t)(pt[j+ 2] >> 24) | ((int32_t)(pt[j+ 3] << 18) >> 10);
    z[i+ 5] = (int32_t)(pt[j+ 3] >> 14) | ((int32_t)(pt[j+ 4] << 28) >> 10);
    z[i+ 6] = (int32_t)(pt[j+ 4] <<  6) >> 10; 
    z[i+ 7] = (int32_t)(pt[j+ 4] >> 26) | ((int32_t)(pt[j+ 5] << 16) >> 10);
    z[i+ 8] = (int32_t)(pt[j+ 5] >> 16) | ((int32_t)(pt[j+ 6] << 26) >> 10);
    z[i+ 9] = (int32_t)(pt[j+ 6] <<  4) >> 10; 
    z[i+10] = (int32_t)(pt[j+ 6] >> 28) | ((int32_t)(pt[j+ 7] << 14) >> 10);
    z[i+11] = (int32_t)(pt[j+ 7] >> 18) | ((int32_t)(pt[j+ 8] << 24) >> 10);
    z[i+12] = (int32_t)(pt[j+ 8] <<  2) >> 10; 
    z[i+13] = (int32_t)(pt[j+ 8] >> 30) | ((int32_t)(pt[j+ 9] << 12) >> 10);
    z[i+14] = (int32_t)(pt[j+ 9] >> 20) | ((int32_t)(pt[j+10] << 22) >> 10);
    z[i+15] = (int32_t)pt[j+10] >> 10;
    j += 11;
  }   
  memcpy(c, &sm[PARAM_N*PARAM_D/8], CRYPTO_C_BYTES);
}


static void hash_vm(unsigned char *c_bin, poly v, const unsigned char *m, unsigned long long mlen)
{ // Hash to generate c' 
  unsigned char t[PARAM_N+mlen];
  int64_t mask, cL;
  unsigned int i;  

  for (i=0; i<PARAM_N; i++) { 
    // If v[i] > PARAM_Q/2 then v[i] -= PARAM_Q
    mask = (int64_t)(PARAM_Q/2 - v[i]) >> 63;                    
    v[i] = ((v[i]-PARAM_Q) & mask) | (v[i] & ~mask);
    
    cL = v[i] & ((1<<PARAM_D)-1);
    // If cL > 2^(d-1) then cL -= 2^d
    mask = (int64_t)((1<<(PARAM_D-1)) - cL) >> 63;                    
    cL = ((cL-(1<<PARAM_D)) & mask) | (cL & ~mask);  
    t[i] = (unsigned char)((v[i] - cL) >> PARAM_D);      
  }  
  memcpy(&t[PARAM_N], m, mlen);
  shake256(c_bin, CRYPTO_C_BYTES, t, mlen+PARAM_N);
} 


static __inline uint64_t Abs(int64_t value)
{ // Compute absolute value

    uint64_t mask = (uint64_t)(value >> 63);
    return ((mask ^ value) - mask);
}


static int test_rejection(poly z)
{ // Check bounds for signature vector z during signing. Returns 0 if valid, otherwise outputs 1 if invalid (rejected).
  // This function leaks the position of the coefficient that fails the test (but this is independent of the secret data). 
  // It does not leak the sign of the coefficients.
  unsigned int i;

  for (i=0; i<PARAM_N; i++) {
    if (Abs(z[i]) > (PARAM_B-PARAM_U))
      return 1;
  }
  return 0;
}


static int test_v(poly v)
{ // Check bounds for w = v - ec during signature verification. Returns 0 if valid, otherwise outputs 1 if invalid (rejected).
  // This function leaks the position of the coefficient that fails the test (but this is independent of the secret data). 
  // It does not leak the sign of the coefficients.
  unsigned int i;
  int64_t mask, left, val;
  uint64_t t0, t1;
  
  for (i=0; i<PARAM_N; i++) {      
    // If v[i] > PARAM_Q/2 then v[i] -= PARAM_Q
    mask = (int64_t)(PARAM_Q/2 - v[i]) >> 63;
    val = ((v[i]-PARAM_Q) & mask) | (v[i] & ~mask);
    // If (Abs(val) < PARAM_Q/2 - PARAM_REJECTION) then t0 = 0, else t0 = 1
    t0 = (uint64_t)(~((int64_t)Abs(val) - (int64_t)(PARAM_Q/2 - PARAM_REJECTION))) >> 63;
                     
    left = val;
    val = (int32_t)((val + (1<<(PARAM_D-1))-1) >> PARAM_D); 
    val = left - (val << PARAM_D);
    // If (Abs(val) < (1<<(PARAM_D-1))-PARAM_REJECTION) then t1 = 0, else t1 = 1 
    t1 = (uint64_t)(~((int64_t)Abs(val) - (int64_t)((1<<(PARAM_D-1))-PARAM_REJECTION))) >> 63; 

    if ((t0 | t1) == 1)  // Returns 1 if any of the two tests failed
      return 1;
  }
  return 0;
}


static int test_z(poly z)
{ // Check bounds for signature vector z during signature verification
  // Returns 0 if valid, otherwise outputs 1 if invalid (rejected)
  unsigned int i;
  
  for (i=0; i<PARAM_N; i++) {                                  
    if (z[i] < -(PARAM_B-PARAM_U) || z[i] > (PARAM_B-PARAM_U))
      return 1;
  }
  return 0;
}


static int check_ES(poly p, int bound)
{ // Checks the generated polynomial e or s
  // Returns 0 if ok, otherwise returns 1
  unsigned int i, j, sum = 0, limit = PARAM_N;
  int16_t temp, mask, list[PARAM_N];

  for (j=0; j<PARAM_N; j++)    
    list[j] = (int16_t)(Abs(p[j]));

  for (j=0; j<PARAM_W; j++) {
    for (i=0; i<limit-1; i++) {
      // If list[i+1] > list[i] then exchange contents
      mask = (list[i+1] - list[i]) >> 15;  
      temp = (list[i+1] & mask) | (list[i] & ~mask);
      list[i+1] = (list[i] & mask) | (list[i+1] & ~mask);
      list[i] = temp;
    }
    sum += list[limit-1];
    limit -= 1;
  }

  if (sum > bound)
    return 1;
  return 0;
}


/********************************************************************************************
* Name:        sparse_mul16
* Description: performs sparse polynomial multiplication
* Parameters:  inputs:
*              - const unsigned char* sk: part of the secret key
*              - const uint32_t pos_list[PARAM_W]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_W]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*
* Note: pos_list[] and sign_list[] contain public information since c is public
*********************************************************************************************/
static void sparse_mul16(poly prod, const unsigned char *sk, const uint32_t pos_list[PARAM_W], const int16_t sign_list[PARAM_W])
{
  int i, j, pos;
  int16_t *t = (int16_t*)sk;

  for (i=0; i<PARAM_N; i++)
    prod[i] = 0;

  for (i=0; i<PARAM_W; i++) {
    pos = pos_list[i];
    for (j=0; j<pos; j++) {
        prod[j] = prod[j] - sign_list[i]*t[j+PARAM_N-pos];
    }
    for (j=pos; j<PARAM_N; j++) {
        prod[j] = prod[j] + sign_list[i]*t[j-pos];
    }
  }
}


/********************************************************************************************
* Name:        sparse_mul32
* Description: performs sparse polynomial multiplication 
* Parameters:  inputs:
*              - const int32_t* pk: part of the public key
*              - const uint32_t pos_list[PARAM_W]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_W]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*********************************************************************************************/
static void sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_W], const int16_t sign_list[PARAM_W])
{
  int i, j, pos;

  for (i=0; i<PARAM_N; i++)
    prod[i] = 0;
  
  for (i=0; i<PARAM_W; i++) {
    pos = pos_list[i];
    for (j=0; j<pos; j++) {
        prod[j] = prod[j] - sign_list[i]*pk[j+PARAM_N-pos];
    }
    for (j=pos; j<PARAM_N; j++) {
        prod[j] = prod[j] + sign_list[i]*pk[j-pos];
    }
  }
  for (i=0; i<PARAM_N; i++)
    prod[i] = barr_reduce(prod[i]);
}


/*********************************************************
* Name:        crypto_sign_keypair
* Description: generates a public and private key pair
* Parameters:  inputs:  none
*              outputs:
*              - unsigned char *pk: public key
*              - unsigned char *sk: secret key
* Returns:     0 for successful execution
**********************************************************/
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{
  unsigned char randomness[CRYPTO_RANDOMBYTES], randomness_extended[4*CRYPTO_SEEDBYTES];
  poly s, e, a, t;  
  int nonce = 0;  // Initialize domain separator for error and secret polynomials
#ifdef DEBUG
  ctr_keygen=0;
#endif

  // Get randomness_extended <- seed_e, seed_s, seed_a, seed_y
  randombytes(randomness, CRYPTO_RANDOMBYTES);
  shake256(randomness_extended, 4*CRYPTO_SEEDBYTES, randomness, CRYPTO_RANDOMBYTES);

  do {        // Sample the error polynomial
#ifdef DEBUG
  ctr_keygen++;
#endif
    sample_gauss_poly(e, randomness_extended, ++nonce);       
  } while(check_ES(e, (int)PARAM_KEYGEN_BOUND_E) != 0);  
 
  do {        // Sample the secret polynomial
#ifdef DEBUG
  ctr_keygen++;
#endif
    sample_gauss_poly(s, &randomness_extended[CRYPTO_SEEDBYTES], ++nonce);
  } while(check_ES(s, (int)PARAM_KEYGEN_BOUND_S) != 0);

  // Generate uniform polynomial "a"
  poly_uniform(a, &randomness_extended[2*CRYPTO_SEEDBYTES]);
  
  // Compute the public key t = as+e
  poly_mul(t, a, s);
  poly_add(t, t, e);
    
  // Pack public and private keys
  pack_sk(sk, s, e, &randomness_extended[2*CRYPTO_SEEDBYTES]);
  encode_pk(pk, t, &randomness_extended[2*CRYPTO_SEEDBYTES]);

  return 0;
}


/***************************************************************
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
int crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char* sk)
{
  unsigned char c[CRYPTO_C_BYTES], randomness[CRYPTO_SEEDBYTES], randomness_input[CRYPTO_RANDOMBYTES+CRYPTO_SEEDBYTES+mlen];
  uint32_t pos_list[PARAM_W];
  int16_t sign_list[PARAM_W];
  poly y, v, Sc, Ec, z, a;
  int nonce = 0;  // Initialize domain separator for sampling y   
#ifdef DEBUG
  ctr_sign=0;
  rejwctr=0;
  rejyzctr=0;
#endif

  // Get H(seed_y, r, m) to sample y
  randombytes(randomness_input+CRYPTO_RANDOMBYTES, CRYPTO_RANDOMBYTES);
  memcpy(randomness_input, &sk[CRYPTO_SECRETKEYBYTES-CRYPTO_SEEDBYTES], CRYPTO_SEEDBYTES);
  memcpy(randomness_input+CRYPTO_RANDOMBYTES+CRYPTO_SEEDBYTES, m, mlen);
  shake256(randomness, CRYPTO_SEEDBYTES, randomness_input, CRYPTO_RANDOMBYTES+CRYPTO_SEEDBYTES+mlen);
  
  poly_uniform(a, &sk[CRYPTO_SECRETKEYBYTES-2*CRYPTO_SEEDBYTES]);

  while (1) {
#ifdef DEBUG
  ctr_sign++;
#endif    
    sample_y(y, randomness, ++nonce);           // Sample y uniformly at random from [-B,B]
    poly_mul(v, a, y); 
    hash_vm(c, v, m, mlen);
    encode_c(pos_list, sign_list, c);           // Generate c = Enc(c'), where c' is the hashing of v together with m
    sparse_mul16(Sc, sk, pos_list, sign_list);
    poly_add(z, y, Sc);                         // Compute z = y + sc
    
    if (test_rejection(z) != 0) {               // Rejection sampling
#ifdef DEBUG
  rejyzctr++;
#endif
      continue;
    }
 
    sparse_mul16(Ec, sk+(sizeof(int16_t)*PARAM_N), pos_list, sign_list);
    poly_sub(v, v, Ec);                         
    
    if (test_v(v) != 0) {
#ifdef DEBUG
  rejwctr++;
#endif
      continue;
    }    

    // Copy message to signature package, and pack signature
    memcpy(sm+CRYPTO_BYTES, m, mlen);
    *smlen = CRYPTO_BYTES + mlen; 
    encode_sig(sm, c, z);

    return 0;
  }
}


/************************************************************
* Name:        crypto_sign_open
* Description: verification of a signature sm
* Parameters:  inputs:
*              - const unsigned char *sm: signature
*              - unsigned long long smlen: signature length
*              - const unsigned char* pk: public Key
*              outputs:
*              - unsigned char *m: original (signed) message
*              - unsigned long long *mlen: message length*
* Returns:     0 for valid signature
*              <0 for invalid signature
************************************************************/
int crypto_sign_open(unsigned char *m, unsigned long long *mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk)
{
  unsigned char c[CRYPTO_C_BYTES], c_sig[CRYPTO_C_BYTES], seed[CRYPTO_SEEDBYTES];
  uint32_t pos_list[PARAM_W];
  int16_t sign_list[PARAM_W]; 
  int32_t pk_t[PARAM_N];
  poly w, z, a, Tc;

  if (smlen < CRYPTO_BYTES) return -1;

  decode_sig(c, z, sm);  
  if (test_z(z) != 0) return -2;                 // Check norm of z
  decode_pk((int32_t*)pk_t, seed, pk);
  poly_uniform(a, seed);
  encode_c(pos_list, sign_list, c);    
  poly_mul(w, a, z);                             
  sparse_mul32(Tc, pk_t, pos_list, sign_list);     
  poly_sub(w, w, Tc);                            // Compute w = az - tc
  hash_vm(c_sig, w, sm+CRYPTO_BYTES, ((int)smlen - (int)CRYPTO_BYTES));

  // Check if the calculated c matches c from the signature
  if (memcmp(c, c_sig, CRYPTO_C_BYTES)) return -3;
  
  *mlen = smlen-CRYPTO_BYTES;
  memcpy(m, sm+CRYPTO_BYTES, *mlen);

  return 0;
}
