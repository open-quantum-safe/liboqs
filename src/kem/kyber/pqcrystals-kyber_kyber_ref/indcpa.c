#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "indcpa.h"
#include "polyvec.h"
#include "poly.h"
#include "ntt.h"
#include "symmetric.h"
#include "randombytes.h"
extern param params[];
/*************************************************
* Name:        pack_pk
*
* Description: Serialize the public key as concatenation of the
*              serialized vector of polynomials pk
*              and the public seed used to generate the matrix A.
*
* Arguments:   uint8_t *r: pointer to the output serialized public key
*              polyvec *pk: pointer to the input public-key polyvec
*              const uint8_t *seed: pointer to the input public seed
**************************************************/
static void pack_pk(uint8_t *r,
                    polyvec *pk,
                    const uint8_t *seed,
                    int8_t security_level)
{
  size_t i;
  polyvec_tobytes(r, pk);
  for(i=0;i<params[security_level].KYBER_SYMBYTES;i++)
    r[i+params[security_level].KYBER_POLYVECBYTES] = seed[i];
}

/*************************************************
* Name:        unpack_pk
*
* Description: De-serialize public key from a byte array;
*              approximate inverse of pack_pk
*
* Arguments:   - polyvec *pk: pointer to output public-key polynomial vector
*              - uint8_t *seed: pointer to output seed to generate matrix A
*              - const uint8_t *packedpk: pointer to input serialized public key
**************************************************/
static void unpack_pk(polyvec *pk,
                      uint8_t *seed,
                      const uint8_t *packedpk,
                      int8_t security_level)
{
  size_t i;
  polyvec_frombytes(pk, packedpk);
  for(i=0;i<params[security_level].KYBER_SYMBYTES;i++)
    seed[i] = packedpk[i+params[security_level].KYBER_POLYVECBYTES];
}

/*************************************************
* Name:        pack_sk
*
* Description: Serialize the secret key
*
* Arguments:   - uint8_t *r: pointer to output serialized secret key
*              - polyvec *sk: pointer to input vector of polynomials (secret key)
**************************************************/
static void pack_sk(uint8_t *r, polyvec *sk)
{
  polyvec_tobytes(r, sk);
}

/*************************************************
* Name:        unpack_sk
*
* Description: De-serialize the secret key; inverse of pack_sk
*
* Arguments:   - polyvec *sk: pointer to output vector of polynomials (secret key)
*              - const uint8_t *packedsk: pointer to input serialized secret key
**************************************************/
static void unpack_sk(polyvec *sk, const uint8_t *packedsk)
{
  polyvec_frombytes(sk, packedsk);
}

/*************************************************
* Name:        pack_ciphertext
*
* Description: Serialize the ciphertext as concatenation of the
*              compressed and serialized vector of polynomials b
*              and the compressed and serialized polynomial v
*
* Arguments:   uint8_t *r: pointer to the output serialized ciphertext
*              poly *pk: pointer to the input vector of polynomials b
*              poly *v: pointer to the input polynomial v
**************************************************/
static void pack_ciphertext(uint8_t *r, polyvec *b, poly *v,int8_t security_level)
{
  polyvec_compress(r, b);
  poly_compress(r+params[security_level].KYBER_POLYVECCOMPRESSEDBYTES, v, security_level);
}

/*************************************************
* Name:        unpack_ciphertext
*
* Description: De-serialize and decompress ciphertext from a byte array;
*              approximate inverse of pack_ciphertext
*
* Arguments:   - polyvec *b: pointer to the output vector of polynomials b
*              - poly *v: pointer to the output polynomial v
*              - const uint8_t *c: pointer to the input serialized ciphertext
**************************************************/
static void unpack_ciphertext(polyvec *b, poly *v, const uint8_t *c,int8_t security_level)
{
  polyvec_decompress(b, c);
  poly_decompress(v, c+params[security_level].KYBER_POLYVECCOMPRESSEDBYTES, security_level);
}

/*************************************************
* Name:        rej_uniform
*
* Description: Run rejection sampling on uniform random bytes to generate
*              uniform random integers mod q
*
* Arguments:   - int16_t *r: pointer to output buffer
*              - unsigned int len: requested number of 16-bit integers (uniform mod q)
*              - const uint8_t *buf: pointer to input buffer (assumed to be uniformly random bytes)
*              - unsigned int buflen: length of input buffer in bytes
*
* Returns number of sampled 16-bit integers (at most len)
**************************************************/
static unsigned int rej_uniform(int16_t *r,
                                unsigned int len,
                                const uint8_t *buf,
                                unsigned int buflen,
                                int8_t security_level)
{
  unsigned int ctr, pos;
  uint16_t val0, val1;

  ctr = pos = 0;
  while(ctr < len && pos + 3 <= buflen) {
    val0 = ((buf[pos+0] >> 0) | ((uint16_t)buf[pos+1] << 8)) & 0xFFF;
    val1 = ((buf[pos+1] >> 4) | ((uint16_t)buf[pos+2] << 4)) & 0xFFF;
    pos += 3;

    if(val0 < params[security_level].KYBER_Q)
      r[ctr++] = val0;
    if(ctr < len && val1 < params[security_level].KYBER_Q)
      r[ctr++] = val1;
  }

  return ctr;
}

#define gen_a(A,B,C)  gen_matrix(A,B,C,0)
#define gen_at(A,B,C) gen_matrix(A,B,C,1)

/*************************************************
* Name:        gen_matrix
*
* Description: Deterministically generate matrix A (or the transpose of A)
*              from a seed. Entries of the matrix are polynomials that look
*              uniformly random. Performs rejection sampling on output of
*              a XOF
*
* Arguments:   - polyvec *a: pointer to ouptput matrix A
*              - const uint8_t *seed: pointer to input seed
*              - int transposed: boolean deciding whether A or A^T is generated
**************************************************/
// #define GEN_MATRIX_NBLOCKS ((12*KYBER_N/8*(1 << 12)/KYBER_Q + XOF_BLOCKBYTES)/XOF_BLOCKBYTES)
// Not static for benchmarking
void gen_matrix(polyvec *a, const uint8_t *seed, int8_t security_level, int transposed)
{
  unsigned int GEN_MATRIX_NBLOCKS = (12*params[security_level].KYBER_N/8*(1 << 12)/params[security_level].KYBER_Q + XOF_BLOCKBYTES)/XOF_BLOCKBYTES;
  unsigned int ctr, i, j, k;
  unsigned int buflen, off;
  uint8_t buf[GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES+2];
  xof_state state;
  xof_init(&state, seed);

  for(i=0;i<params[security_level].KYBER_K;i++) {
    for(j=0;j<params[security_level].KYBER_K;j++) {
      if(transposed)
        xof_absorb(&state, seed, i, j);
      else
        xof_absorb(&state, seed, j, i);

      xof_squeezeblocks(buf, GEN_MATRIX_NBLOCKS, &state);
      buflen = GEN_MATRIX_NBLOCKS*XOF_BLOCKBYTES;
      ctr = rej_uniform(a[i].vec[j].coeffs, params[security_level].KYBER_N, buf, buflen, security_level);

      while(ctr < params[security_level].KYBER_N) {
        off = buflen % 3;
        for(k = 0; k < off; k++)
          buf[k] = buf[buflen - off + k];
        xof_squeezeblocks(buf + off, 1, &state);
        buflen = off + XOF_BLOCKBYTES;
        ctr += rej_uniform(a[i].vec[j].coeffs + ctr, params[security_level].KYBER_N - ctr, buf, buflen, security_level);
      }
    }
  }
  xof_release(&state);
}

/*************************************************
* Name:        indcpa_keypair
*
* Description: Generates public and private key for the CPA-secure
*              public-key encryption scheme underlying Kyber
*
* Arguments:   - uint8_t *pk: pointer to output public key
*                             (of length KYBER_INDCPA_PUBLICKEYBYTES bytes)
*              - uint8_t *sk: pointer to output private key
                              (of length KYBER_INDCPA_SECRETKEYBYTES bytes)
**************************************************/
void indcpa_keypair(uint8_t *pk,
                    uint8_t *sk, int8_t security_level)
{
  unsigned int i;
  uint8_t buf[2*params[security_level].KYBER_SYMBYTES];
  const uint8_t *publicseed = buf;
  const uint8_t *noiseseed = buf+params[security_level].KYBER_SYMBYTES;
  uint8_t nonce = 0;
  polyvec a[params[security_level].KYBER_K], e, pkpv, skpv;

  randombytes(buf, params[security_level].KYBER_SYMBYTES);
  hash_g(buf, buf, params[security_level].KYBER_SYMBYTES);

  gen_a(a, publicseed,security_level);

  for(i=0;i<params[security_level].KYBER_K;i++)
    poly_getnoise_eta1(&skpv.vec[i], noiseseed, nonce++, security_level);
  for(i=0;i<params[security_level].KYBER_K;i++)
    poly_getnoise_eta1(&e.vec[i], noiseseed, nonce++, security_level);

  polyvec_ntt(&skpv);
  polyvec_ntt(&e);

  // matrix-vector multiplication
  for(i=0;i<params[security_level].KYBER_K;i++) {
    polyvec_basemul_acc_montgomery(&pkpv.vec[i], &a[i], &skpv);
    poly_tomont(&pkpv.vec[i],security_level);
  }

  polyvec_add(&pkpv, &pkpv, &e);
  polyvec_reduce(&pkpv);

  pack_sk(sk, &skpv);
  pack_pk(pk, &pkpv, publicseed,security_level);
}

/*************************************************
* Name:        indcpa_enc
*
* Description: Encryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *c: pointer to output ciphertext
*                            (of length KYBER_INDCPA_BYTES bytes)
*              - const uint8_t *m: pointer to input message
*                                  (of length KYBER_INDCPA_MSGBYTES bytes)
*              - const uint8_t *pk: pointer to input public key
*                                   (of length KYBER_INDCPA_PUBLICKEYBYTES)
*              - const uint8_t *coins: pointer to input random coins used as seed
*                                      (of length KYBER_SYMBYTES) to deterministically
*                                      generate all randomness
**************************************************/
void indcpa_enc(uint8_t *c,
                const uint8_t *m,
                const uint8_t *pk,
                const uint8_t *coins,
                int8_t security_level)
{
  unsigned int i;
  uint8_t seed[params[security_level].KYBER_SYMBYTES];
  uint8_t nonce = 0;
  polyvec sp, pkpv, ep, at[params[security_level].KYBER_K], b;
  poly v, k, epp;

  unpack_pk(&pkpv, seed, pk,security_level);
  poly_frommsg(&k, m, security_level);
  gen_at(at, seed,security_level);

  for(i=0;i<params[security_level].KYBER_K;i++)
    poly_getnoise_eta1(sp.vec+i, coins, nonce++, security_level);
  for(i=0;i<params[security_level].KYBER_K;i++)
    poly_getnoise_eta2(ep.vec+i, coins, nonce++, security_level);
  poly_getnoise_eta2(&epp, coins, nonce++, security_level);

  polyvec_ntt(&sp);

  // matrix-vector multiplication
  for(i=0;i<params[security_level].KYBER_K;i++)
    polyvec_basemul_acc_montgomery(&b.vec[i], &at[i], &sp);

  polyvec_basemul_acc_montgomery(&v, &pkpv, &sp);

  polyvec_invntt_tomont(&b);
  poly_invntt_tomont(&v);

  polyvec_add(&b, &b, &ep);
  poly_add(&v, &v, &epp, security_level);
  poly_add(&v, &v, &k, security_level);
  polyvec_reduce(&b);
  poly_reduce(&v, security_level);

  pack_ciphertext(c, &b, &v,security_level);
}

/*************************************************
* Name:        indcpa_dec
*
* Description: Decryption function of the CPA-secure
*              public-key encryption scheme underlying Kyber.
*
* Arguments:   - uint8_t *m: pointer to output decrypted message
*                            (of length KYBER_INDCPA_MSGBYTES)
*              - const uint8_t *c: pointer to input ciphertext
*                                  (of length KYBER_INDCPA_BYTES)
*              - const uint8_t *sk: pointer to input secret key
*                                   (of length KYBER_INDCPA_SECRETKEYBYTES)
**************************************************/
void indcpa_dec(uint8_t *m,
                const uint8_t *c,
                const uint8_t *sk,
                int8_t security_level)
{
  polyvec b, skpv;
  poly v, mp;

  unpack_ciphertext(&b, &v, c,security_level);
  unpack_sk(&skpv, sk);

  polyvec_ntt(&b);
  polyvec_basemul_acc_montgomery(&mp, &skpv, &b);
  poly_invntt_tomont(&mp);

  poly_sub(&mp, &v, &mp, security_level);
  poly_reduce(&mp,security_level);

  poly_tomsg(m, &mp,security_level);
}
