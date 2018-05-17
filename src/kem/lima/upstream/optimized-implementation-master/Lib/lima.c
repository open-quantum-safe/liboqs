#include "lima.h"
#include "xof.h"
#include "fft.h"
#include <math.h>
#include <stdlib.h>

lima_fft_t lima_fft_2p_1024;
lima_fft_t lima_fft_2p_2048;
lima_fft_t lima_fft_sp_1018;
lima_fft_t lima_fft_sp_1306;
lima_fft_t lima_fft_sp_1822;
lima_fft_t lima_fft_sp_2062;

/** LIMA_2p:
 *  Input parameters = {Type, Mod, N, unused, alpha_0, alpha_1, beta_0, unused, fft_data}
 **/
lima_params_t lima_2p_1024 = { LIMA_2P, &mod133121,   1024,  0,   32141,    100666,  132991,        0, &lima_fft_2p_1024, {0} };
lima_params_t lima_2p_2048 = { LIMA_2P, &mod184321,   2048,  0,   88992,    152704,  184231,        0, &lima_fft_2p_2048, {0} };

/** LIMA_sp:
 *  Input parameters =  {Type, Mod, N, e, alpha_0, alpha_1, beta_0, beta_1, fft_data}
 **/
lima_params_t lima_sp_1018 = { LIMA_SP, &mod12521473, 1018, 11,  1561269,   8501297, 9597006, 10910567, &lima_fft_sp_1018, {0} };
lima_params_t lima_sp_1306 = { LIMA_SP, &mod48181249, 1306, 12, 30019814,  39013233, 5599915, 28280508, &lima_fft_sp_1306, {0} };
lima_params_t lima_sp_1822 = { LIMA_SP, &mod44802049, 1822, 12, 43213195,  19941338, 8284672,  1121361, &lima_fft_sp_1822, {0} };
lima_params_t lima_sp_2062 = { LIMA_SP, &mod16900097, 2062, 13, 12381941,  15641966,  213248,  7202243, &lima_fft_sp_2062, {0} };


/* These functions implement a fast method of accessing
 * the noise function, using precomputed values and
 * table lookup
 */
static int8_t small_gaussian_values[256];

void precompute_noise_data(lima_params_t* params)
{
  for (int i=-20; i<=+20; i++)
  { 
    params->noise[i+20] = to_mod_q(params->ctx, i);
  }
}

void precompute_noise()
{
  for (int i=0; i<256; i++)
  {
    int x = i;
    int s = 0;

    s += x & 1; x>>=1;  s -= x & 1; x>>=1;
    s += x & 1; x>>=1;  s -= x & 1; x>>=1;
    s += x & 1; x>>=1;  s -= x & 1; x>>=1;
    s += x & 1; x>>=1;  s -= x & 1; x>>=1;
    small_gaussian_values[i] = s;
  }

  precompute_noise_data(&lima_2p_1024);
  precompute_noise_data(&lima_2p_2048);
  precompute_noise_data(&lima_sp_1018);
  precompute_noise_data(&lima_sp_1306);
  precompute_noise_data(&lima_sp_1822);
  precompute_noise_data(&lima_sp_2062);
}


/* Given 5 byte output of the XOF ss, this output the
 * modp value of the noise
 */
int xof5_to_modp(lima_params_t* params, uint8_t* ss)
{
  int x = 
    small_gaussian_values[ss[0]] + 
    small_gaussian_values[ss[1]] + 
    small_gaussian_values[ss[2]] + 
    small_gaussian_values[ss[3]] + 
    small_gaussian_values[ss[4]];
  return params->noise[20+x];
}



int init_done=0;

void InitLima()
{
  if (init_done) { return; }

  precompute_noise();
  precompute_fft_data(&lima_2p_1024);
  precompute_fft_data(&lima_2p_2048);
  precompute_fft_data(&lima_sp_1018);
  precompute_fft_data(&lima_sp_1306);
  precompute_fft_data(&lima_sp_1822);
  precompute_fft_data(&lima_sp_2062);

  init_done=1;

}

//LIMA Key-Generation
int lima_key_gen(lima_params_t* params, const uint8_t* seed, byte_length seedlen, 
                 lima_pk_t* pk, lima_sk_t* sk)
{
    int N = (int)params->N;
    int e_tag[LIMA_MAX_N];

    int i;
    XOF_t xof;

    InitXOF(&xof, seed, seedlen, 1);    //data = 0x01

    get_random_field_elements(&xof, params->ctx, N, pk->a);
    for (i = 0; i<N; i++) sk->s[i] = to_mod_q(params->ctx, generate_gaussian_noise(&xof));
    for (i = 0; i<N; i++) e_tag[i] = to_mod_q(params->ctx, generate_gaussian_noise(&xof));
    fft(params, pk->a, pk->a);
    fft(params, sk->s, sk->s);
    fft(params, e_tag, e_tag);
    for (i = 0; i<N; i++) pk->b[i] = mul_add_mod_q(params->ctx,pk->a[i], sk->s[i], e_tag[i]);
    sk->pk=pk;

    return 0;
}

/** Section 2.4 Specification 
 *  Function to check if  polynomials v and e used for encryption
 *  encryption are in the correct range. This is done to keep 
 *  parameters small. 
 *  Return 1 if the input polynomials are in the correct range, else 0
 **/
int RandCheck(lima_params_t* params, int v[LIMA_MAX_N], int e[LIMA_MAX_N])
{
   int a=1;
   int N=(int)params->N;
   int t[LIMA_MAX_N];
   double lhs,rhs;
   for (int i=0; i<N; i++)
     { int x=add_mod_q(params->ctx,v[i],e[i]); 
       t[i]=to_int(params->ctx,x);               // t[i] = v[i] + e[i]
       if (t[i]>params->ctx->q/2)
          { t[i]-=params->ctx->q; }
     }
   if (params->mode==LIMA_2P)                    
     { int e=0;
       for (int i=0; i<N; i++)
         { e+=t[i]; }
       lhs=fabs((double) e);
       rhs=11*sqrt(fabs(2*N))*3.16;              // Accepting condition for LIMA_2p 
       if (lhs>rhs) { a=0; }
     }
   else                     
     { 
       int e = 0;
       for (int i=2; i<N; i++) e += 2*t[i];
       e += t[1];
       rhs=11*sqrt(fabs(4*N))*3.16;              // Accepting condition for LIMA_sp
       for (int k=0; k<N; k++)
	 { e += t[k];
           lhs=fabs((double) e);
           if (lhs>rhs) { return a=0; }
           if (k+2<N) e -= t[k+2];
         }
     }
   return a;
}

/* LIMA CPA-Encryption (subroutine). 
 * It encrypts a message m of m_len bytes in length
 * This is only called inside lima_enc_cpa etc
 */
int lima_enc_cpa_sub(lima_params_t* params, const uint8_t* m, byte_length m_len,
                     const lima_pk_t* pk,
                     XOF_t* xof,
                     lima_ciphertext_t* c)
{
    int i,N = (int)params->N;
    int delta_q = to_mod_q(params->ctx,params->ctx->q / 2);
    int v[LIMA_MAX_N];
    int vp[LIMA_MAX_N];
    int e[LIMA_MAX_N];
    int ep[LIMA_MAX_N];
    int dp[LIMA_MAX_N];
    int x[LIMA_MAX_N];
    int s[LIMA_MAX_N];
    int t[LIMA_MAX_N];
    c->ell=(int)(8*m_len);

    if (c->ell>N)   { return 1; }          // Message too long

    // Get noise terms, using one call to the XOF
    uint8_t noize[LIMA_MAX_N*3*5];
    if (XOF(xof, noize, N*3*5)) { return 1; }
    uint8_t* ss = noize;
    for (i = 0; i<N; i++, ss+=5) vp[i] = xof5_to_modp(params, ss);
    for (i = 0; i<N; i++, ss+=5) ep[i] = xof5_to_modp(params, ss);
    for (i = 0; i<N; i++, ss+=5) dp[i] = xof5_to_modp(params, ss);

    // Doing check here is not really a timing issue
    if (RandCheck(params,vp,ep)==0) { return 1; }

    fft(params, vp, v);
    fft(params, ep, e);

    for (i = 0; i<c->ell; i++)
      {
        int bit = (m[i >> 3] >> (i & 7)) & 1;
        x[i] = add_mod_q(params->ctx,bit*delta_q,dp[i]);
      }

    for (i = 0; i<N; i++) s[i] = mul_mod_q(params->ctx,pk->b[i], v[i]);
    fft_inv(params, s, s);
    for (i = 0; i<N; i++) t[i] = add_mod_q(params->ctx,s[i], x[i]);

    for (i = 0; i<c->ell; i++) c->c0[i] = t[i];
    for (i = 0; i<N; i++) c->c1[i] = mul_add_mod_q(params->ctx,pk->a[i], v[i], e[i]);

    return 0;
}

//Used in LIMA_enc_cca
void increment_seed(uint8_t* seed, int seed_len)
{
  int i=0,fl=0;
  do
    { fl=0;
      seed[i]=seed[i]+1;
      if (seed[i]==0)
	    { i++;
        if (i<seed_len) { fl=1; }
      }
    }
  while (fl);
}

// LIMA CPA-Encryption of  message m of m_len bytes
int lima_enc_cpa(lima_params_t* params, const uint8_t* m, byte_length m_len,
                 const lima_pk_t* pk,
                 const uint8_t* seed, byte_length seed_len,
                 lima_ciphertext_t* c)
{
  if (seed_len<32 || (8*m_len>params->N)) { return 1; }
  XOF_t xof;
  InitXOF(&xof, seed, seed_len, 2);             //data = 0x02
  int fl;
  do
    { fl=lima_enc_cpa_sub(params, m, m_len, pk, &xof, c); }
  while (fl==1);
  return 0;
}

/* LIMA CPA-Decryption. 
 *It assumes m has enough space allocated already (c.ell/16 bytes)
 */
int lima_dec_cpa(lima_params_t* params, const lima_ciphertext_t* c, 
                 const lima_sk_t* sk, uint8_t* m, byte_length* m_len)
{
    int i;
    int N = (int)params->N;
    int delta_q = to_mod_q(params->ctx, params->ctx->q / 2);
    int v[LIMA_MAX_N];
    int f[LIMA_MAX_N];
  
   if (c->ell>N || (c->ell&0x7)!=0) { return 1; }

    for (i = 0; i<N; i++) v[i] = mul_mod_q(params->ctx,sk->s[i], c->c1[i]);
    fft_inv(params, v, v);
    for (i = 0; i<c->ell; i++) f[i] = sub_mod_q(params->ctx,c->c0[i], v[i]);

    for (i = 0; i<c->ell; i++)
    {
        // Convert from internal to human form
        f[i]=to_int(params->ctx,f[i]);
        if (f[i] > delta_q) f[i] -= params->ctx->q;
    }

    for (i = 0; i<c->ell / 8; i++) m[i] = 0;

    for (i = 0; i<c->ell; i++)
    {
        int bit = round(f[i] * 2.0 / params->ctx->q) != 0;
        m[i >> 3] |= bit << (i & 7);
    }

    *m_len = c->ell / 8;

    return 0;
}

/* LIMA CCA-Encryption of a message m of m_len<(N/2-256)/8 bytes */
int lima_enc_cca(lima_params_t* params, const uint8_t* m, byte_length m_len,
                 const lima_pk_t* pk,
                 const uint8_t seed[32],
                 lima_ciphertext_t* c)
{
  uint8_t mu[LIMA_MAX_N],s[32];

  if (m_len+32>=params->N) { return 1; }

  for (int i=0; i<32; i++) { s[i]=seed[i]; }
  for (byte_length i=0; i<m_len; i++) { mu[i]=m[i]; }
  XOF_t xof;
  int fl;

  do
    { for (int i=0; i<32;    i++) { mu[m_len+i]=s[i]; }
 
      InitXOF(&xof, mu, m_len+32, 3);               //data=0x03 
      fl=lima_enc_cpa_sub(params,mu,m_len+32,pk,&xof,c); 
      if (fl==1) { increment_seed(s,32); }
    }
  while (fl==1);

  return 0;
}

/* LIMA CCA-decryption. 
 * It assumes m has enough space allocated already (c.ell-256)/8 bytes
 */
int lima_dec_cca(lima_params_t* params, const lima_ciphertext_t* c, 
                 const lima_sk_t* sk, uint8_t* m, byte_length *m_len)
{
    uint8_t mu[LIMA_MAX_N/8];
    lima_ciphertext_t cd;
    int i;
    byte_length mlen;

    if (lima_dec_cpa(params,c,sk,mu,&mlen)) { return 1; }
    if (mlen<32) { return 1; }

    XOF_t xof;
    InitXOF(&xof,mu,mlen,3);

    int ans=0;
    if (lima_enc_cpa_sub(params, mu, mlen, sk->pk, &xof, &cd)) { ans=1; }
    if (cd.ell!=c->ell) { ans=1; }
    for (i=0; i<c->ell; i++)
      { if (c->c0[i]!=cd.c0[i]) { ans=1; } }
    for (byte_length i=0;i<params->N; i++)
      { if (c->c1[i] != cd.c1[i]) { ans=1; } }
    if (ans==1) { return 1; }
   
    for (byte_length i=0; i<mlen-32; i++) { m[i]=mu[i]; } 
    *m_len=mlen-32;

    return 0;
}

/* Key encapsulation mechanisms
 * Output key has length key_len*8 in bits, 
 * we assume key has enough space already allocated
 */
int lima_encap_cpa(lima_params_t* params,const lima_pk_t* pk,
                   const uint8_t* seed, byte_length seed_len,
                   uint8_t* key,byte_length key_len,
                   lima_ciphertext_t* c)
{
  if ((seed_len<key_len+32) || (key_len*8)>params->N || key_len<32) 
      { return 1; }

  for (byte_length i=0; i<key_len; i++) { key[i]=seed[i+seed_len-key_len]; }
  XOF_t xof;
  InitXOF(&xof,seed,seed_len-key_len,4);     // data = 0x04
  int fl;
  do
    { fl=lima_enc_cpa_sub(params, key, key_len, pk, &xof, c); }
  while (fl==1);

  return 0;
}

/* This returns the key length as well,  but enough space is already
 * assumed to be allocated
 */
int lima_decap_cpa(lima_params_t* params, const lima_ciphertext_t* c, const lima_sk_t* sk, 
                   uint8_t* key, byte_length *key_len)
{
  int fl=lima_dec_cpa(params,c,sk,key,key_len);
  return fl;
}

/* Key encapsulation mechanisms
 * Output key has length key_len in bytes
 * we assume key has enough space already allocated
 */
int lima_encap_cca(lima_params_t* params,const lima_pk_t* pk,
                   const uint8_t* seed, byte_length seed_len,
                   uint8_t* key,byte_length key_len,
                   lima_ciphertext_t* c)
{
  if (seed_len<32 || 8*seed_len>params->N) { return 1; }
  
  uint8_t s[LIMA_MAX_N/8];
  for (byte_length i=0; i<seed_len; i++) { s[i]=seed[i]; }

  XOF_t xof;
  int fl;

  do
    { InitXOF(&xof, s, seed_len,5);          // data = 0x05
      fl=lima_enc_cpa_sub(params,s,seed_len,pk,&xof,c);
      if (fl==1) { increment_seed(s,(int)seed_len); }
    }
  while (fl==1);

  fl=KDF(s,seed_len,key,key_len);
  return fl;
}


/* Enough space for key is assumed to have been allocated
 */
int lima_decap_cca(lima_params_t* params, const lima_ciphertext_t* c, const lima_sk_t* sk,
                   uint8_t* key, byte_length key_len)
{
    uint8_t s[LIMA_MAX_N/8];
    lima_ciphertext_t cd;
    int i;
    byte_length mlen;

    if (lima_dec_cpa(params,c,sk,s,&mlen)) { return 1; }
    if (mlen<32) { return 1; }

    XOF_t xof;
    InitXOF(&xof,s,mlen,5);

    int ans=0;
    if (lima_enc_cpa_sub(params, s, mlen, sk->pk, &xof, &cd)) { ans=1; }

    if (cd.ell!=c->ell) { ans=1; }
    for (i=0; i<c->ell; i++)
      { if (c->c0[i]!=cd.c0[i]) { ans=1; } }
    for (byte_length i=0;i<params->N; i++)
      { if (c->c1[i] != cd.c1[i]) { ans=1; } }
    if (ans==1) { return ans; }

    ans=KDF(s,mlen,key,key_len);

    return ans;
}

/* This encodes integers as sequence of bytes
 * Used in EncodePK, EncodeSK, Encode CT
 */
void Encode_Val(unsigned int a, int t, uint8_t* B)
{
  unsigned int b=a;
  for (int i=0; i<t; i++)
    { B[t-1-i]=b&0xFF;
      b-=B[t-1-i];
      b>>=8;
    }
}

/* This is the inverse of Encode_Val
 * Used in DecodePK, DecodeSK, DecodeCT
 */
unsigned int Decode_Val(int t, const uint8_t* B)
{
  unsigned int a=0;
  for (int i=0; i<t; i++)
    { a<<=8;
      a+=B[i];
    }
  return a;
}

/* Encode PK as a byte string */
int EncodePK(lima_params_t* params, const lima_pk_t* pk, uint8_t *B)
{
  switch (params->N)
    { case 1024: B[0]=0; break;
      case 2048: B[0]=1; break;
      case 1018: B[0]=2; break;
      case 1306: B[0]=3; break;
      case 1822: B[0]=4; break;
      case 2062: B[0]=5; break;
      B[0]=6;
    }
  int len=1,t=params->ctx->log256_q;
  unsigned int a;
  for (byte_length i=0; i<params->N; i++)
    { a=to_int(params->ctx,pk->a[i]);
      Encode_Val(a,t,B+len+i*t);
    }
  len+=(int)(t*params->N);
  for (byte_length i=0; i<params->N; i++)
    { a=to_int(params->ctx,pk->b[i]);
      Encode_Val(a,t,B+len+i*t);
    }
  len+=(int)(t*params->N);
  return len;
}

/*  Encode SK as a byte string  */
int EncodeSK(lima_params_t* params, const lima_sk_t* sk, uint8_t *B)
{
 int len=EncodePK(params,sk->pk,B),t=params->ctx->log256_q;
 unsigned int a;
 for (byte_length i=0; i<params->N; i++)
    { a=to_int(params->ctx,sk->s[i]);
      Encode_Val(a,t,B+len+i*t);
    }
  len+=(int)(t*params->N);
  return len;

}

/* Encode CT as a byte string  */
int EncodeCT(lima_params_t* params, const lima_ciphertext_t* c, uint8_t *B)
{
 switch (params->N)
    { case 1024: B[0]=0; break;
      case 2048: B[0]=1; break;
      case 1018: B[0]=2; break;
      case 1306: B[0]=3; break;
      case 1822: B[0]=4; break;
      case 2062: B[0]=5; break;
      B[0]=6;
    }
  Encode_Val(c->ell,2,B+1);
  int len=3,t=params->ctx->log256_q;
  unsigned int a;
  for (int i=0; i<c->ell; i++)
    { a=to_int(params->ctx,c->c0[i]);
      Encode_Val(a,t,B+len+i*t);
    }
  len+=t*c->ell;
  for (byte_length i=0; i<params->N; i++)
    { a=to_int(params->ctx,c->c1[i]);
      Encode_Val(a,t,B+len+i*t);
    }
  len+=(int)(t*params->N);
  return len;
}

/* Inverse of EncodePK   */
int DecodePK(lima_params_t** params,lima_pk_t* pk,const uint8_t *B)
{
  switch (B[0])
    { case 0: *params=&lima_2p_1024; break;
      case 1: *params=&lima_2p_2048; break;
      case 2: *params=&lima_sp_1018; break;
      case 3: *params=&lima_sp_1306; break;
      case 4: *params=&lima_sp_1822; break;
      case 5: *params=&lima_sp_2062; break;
      return 1;
    }
  int len=1,t=(*params)->ctx->log256_q;
  unsigned int a;
  for (byte_length i=0; i<(*params)->N; i++)
    { a=Decode_Val(t,B+len+i*t);
      pk->a[i]=to_mod_q((*params)->ctx,a);
    }
  len+=(int)(t*(*params)->N);
  for (byte_length i=0; i<(*params)->N; i++)
    { a=Decode_Val(t,B+len+i*t);
      pk->b[i]=to_mod_q((*params)->ctx,a);
    }
  return 0;
}

/* Inverse of EncodeSK  */
int DecodeSK(lima_params_t** params, lima_sk_t* sk, lima_pk_t* pk, const uint8_t *B)
{
  switch (B[0])
    { case 0: *params=&lima_2p_1024; break;
      case 1: *params=&lima_2p_2048; break;
      case 2: *params=&lima_sp_1018; break;
      case 3: *params=&lima_sp_1306; break;
      case 4: *params=&lima_sp_1822; break;
      case 5: *params=&lima_sp_2062; break;
      return 1;
    }
  sk->pk=pk;
  int len=1,t=(*params)->ctx->log256_q;
  unsigned int a;
  for (byte_length i=0; i<(*params)->N; i++)
    { a=Decode_Val(t,B+len+i*t);
      pk->a[i]=to_mod_q((*params)->ctx,a);
    }
  len+=(int)(t*(*params)->N);
  for (byte_length i=0; i<(*params)->N; i++)
    { a=Decode_Val(t,B+len+i*t);
      pk->b[i]=to_mod_q((*params)->ctx,a);
    }
  len+=(int)(t*(*params)->N);
  for (byte_length i=0; i<(*params)->N; i++)
    { a=Decode_Val(t,B+len+i*t);
      sk->s[i]=to_mod_q((*params)->ctx,a);
    }
  return 0;
}

/* Inverse of EncodeCT
 */
int DecodeCT(lima_params_t** params,lima_ciphertext_t* c,const uint8_t *B)
{
  switch (B[0])
    { case 0: *params=&lima_2p_1024; break;
      case 1: *params=&lima_2p_2048; break;
      case 2: *params=&lima_sp_1018; break;
      case 3: *params=&lima_sp_1306; break;
      case 4: *params=&lima_sp_1822; break;
      case 5: *params=&lima_sp_2062; break;
      return 1;
    }
  int len=3,t=(*params)->ctx->log256_q;
  c->ell=Decode_Val(2,B+1);
  unsigned int a;
  for (int i=0; i<c->ell; i++)
    { a=Decode_Val(t,B+len+i*t);
      c->c0[i]=to_mod_q((*params)->ctx,a);
    }
  len+=t*c->ell;
  for (byte_length i=0; i<(*params)->N; i++)
    { a=Decode_Val(t,B+len+i*t);
      c->c1[i]=to_mod_q((*params)->ctx,a);
    }
  return 0;
}


