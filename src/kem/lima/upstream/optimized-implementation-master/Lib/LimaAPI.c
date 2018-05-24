
#include "LimaAPI.h"
#include "lima.h"

#include <stdlib.h>

/* The lengths in this function for pk and sk define the param
 * type which will be used
 */
int Key_Generation(uint8_t* pks,unsigned long long pklen,
                   uint8_t* sks,unsigned long long sklen,
                   const uint8_t *seed,unsigned long long slen)
{
  InitLima();

  lima_pk_t pk;
  lima_sk_t sk;
  
  lima_params_t *params;
  if (sklen==9217 && pklen==6145)
    { params=&lima_2p_1024; }
  else if (sklen==18433 && pklen==12289)
    { params=&lima_2p_2048; }
  else if (sklen==9163 && pklen==6109)
    { params=&lima_sp_1018; }
  else if (sklen==15673 && pklen==10449)
    { params=&lima_sp_1306; }
  else if (sklen==21865 && pklen==14577)
    { params=&lima_sp_1822; }
  else if (sklen==24745 && pklen==16497)
    { params=&lima_sp_2062; }
  else
    { return 1; }

  if (lima_key_gen(params,seed,slen,&pk,&sk)) { return 1; }
  unsigned long long lp=EncodePK(params,&pk,pks);
  unsigned long long ls=EncodeSK(params,&sk,sks);
  if (lp!=pklen || ls!=sklen)
     { return 1; }
  return 0;
}

int Enc_CPA(uint8_t* c,unsigned long long *clen,
            const uint8_t* m,unsigned long long mlen,
            const uint8_t *seed,unsigned long long slen,
            const uint8_t* pks)
{
  InitLima();

  lima_params_t *params;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodePK(&params,&pk,pks)) { return 1; }
  if (lima_enc_cpa(params,m,mlen,&pk,seed,slen,&ct)) { return 1; }
  *clen=EncodeCT(params,&ct,c);
  return 0;
}

int Dec_CPA(uint8_t* m, unsigned long long *mlen,
            const uint8_t* c,
            const uint8_t* sks)
{
  InitLima();

  lima_params_t *params,*params2;
  lima_sk_t sk;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodeSK(&params,&sk,&pk,sks)) { return 1; }
  if (DecodeCT(&params2,&ct,c))  { return 1; }
  if (params!=params2)          { return 1; }
  if (lima_dec_cpa(params,&ct,&sk,m,mlen)) { return 1; }
  return 0;
}

int Enc_CCA(uint8_t* c, unsigned long long *clen,
            const uint8_t* m, unsigned long long mlen,
            const uint8_t seed[32],
            const uint8_t* pks)
{
  InitLima();

  lima_params_t *params;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodePK(&params,&pk,pks)) { return 1; }
  if (lima_enc_cca(params,m,mlen,&pk,seed,&ct)) { return 1; }
  *clen=EncodeCT(params,&ct,c);
  return 0;
}

int Dec_CCA(uint8_t* m, unsigned long long *mlen,
            const uint8_t* c,
            const uint8_t* sks)
{
  InitLima();

  lima_params_t *params,*params2;
  lima_sk_t sk;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodeSK(&params,&sk,&pk,sks)) { return 1; }
  if (DecodeCT(&params2,&ct,c))  { return 1; }
  if (params!=params2)          { return 1; }
  if (lima_dec_cca(params,&ct,&sk,m,mlen)) { return 1; }
  return 0;
}

int Encap_CPA(uint8_t* c, unsigned long long *clen,
              uint8_t* k, unsigned long long klen,
              const uint8_t *seed, unsigned long long slen,
              const uint8_t* pks)
{
  InitLima();

  lima_params_t *params;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodePK(&params,&pk,pks)) { return 1; }
  if (lima_encap_cpa(params,&pk,seed,slen,k,klen,&ct)) { return 1; }
  *clen=EncodeCT(params,&ct,c);
  return 0;
}

int Decap_CPA(uint8_t* k, unsigned long long *klen,
              const uint8_t* c,
              const uint8_t* sks)
{
  InitLima();

  lima_params_t *params,*params2;
  lima_sk_t sk;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodeSK(&params,&sk,&pk,sks)) { return 1; }
  if (DecodeCT(&params2,&ct,c))  { return 1; }
  if (params!=params2)          { return 1; }
  if (lima_decap_cpa(params,&ct,&sk,k,klen)) { return 1; }
  return 0;
}

int Encap_CCA(uint8_t* c, unsigned long long *clen,
              uint8_t* k, unsigned long long klen,
              const uint8_t *seed,unsigned long long slen,
              const uint8_t* pks)
{
  InitLima();

  lima_params_t *params;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodePK(&params,&pk,pks)) { return 1; }
  if (lima_encap_cca(params,&pk,seed,slen,k,klen,&ct)) { return 1; }
  *clen=EncodeCT(params,&ct,c);
  return 0;
}

int Decap_CCA(uint8_t* k, unsigned long long klen,
              const uint8_t* c,
              const uint8_t* sks)
{
  InitLima();

  lima_params_t *params,*params2;
  lima_sk_t sk;
  lima_pk_t pk;
  lima_ciphertext_t ct;
  if (DecodeSK(&params,&sk,&pk,sks)) { return 1; }
  if (DecodeCT(&params2,&ct,c))  { return 1; }
  if (params!=params2)          { return 1; }
  if (lima_decap_cca(params,&ct,&sk,k,klen)) { return 1; }
  return 0;
}

