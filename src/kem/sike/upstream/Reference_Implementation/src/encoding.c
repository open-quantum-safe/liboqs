//
// Supersingular Isogeny Key Encapsulation Ref. Library
//
// InfoSec Global Inc., 2017
// Basil Hess <basil.hess@infosecglobal.com>
//

#include <stdlib.h>
#include <sike_params.h>
#include <encoding.h>
#include <string.h>

#define PUBLIC_KEY_NPS 6

///////////////////////////////////////////////
// Encoding functions for field-p elements
///////////////////////////////////////////////

void ostoi(const unsigned char* to_dec, size_t to_decLen, mp dec) {
  mp_import(dec, to_decLen, -1, 1, 1, 0, to_dec);
}

int ostofp(const unsigned char *to_dec, size_t np, const mp p, mp dec) {
  ostoi(to_dec, np, dec);
  int rc = mpz_cmp(dec, p) >= 0;
  return rc;
}

int ostofp2(const unsigned char *to_dec, size_t np, const mp p, fp2* dec) {
  int rc = 0;
  rc  = ostofp(to_dec,      np, p, dec->x0);
  rc |= ostofp(to_dec + np, np, p, dec->x1);
  return rc;
}

static void itoos(const mp to_enc, unsigned char* enc) {
  mp_export(enc, NULL, -1, 1, 1, 0, to_enc);
}

static void fptoos(const mp to_enc, unsigned char* enc) {
  itoos(to_enc, enc);
}

static size_t get_np_len(const mp p) {
  return BITS_TO_BYTES_CEIL(mp_sizeinbase(p, 2));
}

///////////////////////////////////////////////
// Encoding functions for field-p2 elements (shared secrets)
///////////////////////////////////////////////

void fp2toos(const sike_params_t* params,
            const fp2* shared_sec,
            unsigned char* enc) {

  size_t np = get_np_len(params->EA.ffData->mod);

  fptoos(shared_sec->x0, enc     );
  fptoos(shared_sec->x1, enc + np);

}

void fp2toos_alloc(const sike_params_t *params,
                  const fp2* shared_sec,
                  unsigned char** enc,
                  size_t* encLen) {

  size_t np = get_np_len(params->EA.ffData->mod);

  *encLen = 2 * np;
  *enc = calloc(*encLen, 1);

  fp2toos(params, shared_sec, *enc);

}

size_t fp2toos_len(const sike_params_t* params,
                             const party_t party) {
  if (party == ALICE) {
    return 2*get_np_len(params->EA.ffData->mod);
  } else {
    return 2*get_np_len(params->EB.ffData->mod);
  }
}



///////////////////////////////////////////////
// Encoding/decoding functions for private keys
///////////////////////////////////////////////

int ostosk(const sike_params_t *params,
           const party_t party,
           const unsigned char *sk,
           unsigned char* s,
           sike_private_key dec,
           sike_public_key_t* pkDec) {

  size_t skLen = 0;
  int rc = 0;

  if (s != NULL)
    memcpy(s, sk, params->msg_bytes);

  if (party == ALICE)
    skLen = BITS_TO_BYTES_CEIL(params->msbA);
  else
    skLen = BITS_TO_BYTES_CEIL(params->msbB);

  ostoi(sk + params->msg_bytes, skLen, dec);

  if (pkDec != NULL)
    rc = ostopk(params, party, sk + params->msg_bytes + skLen, pkDec);

  return rc;
}

void sktoos(const sike_params_t *params,
            const party_t party,
            const unsigned char* s,
            const sike_private_key sk,
            const sike_public_key_t* pk,
            unsigned char *enc) {

  size_t skPartLen = 0, np = 0;

  if (party == ALICE) {
    skPartLen = BITS_TO_BYTES_CEIL(params->msbA);
    np = get_np_len(params->EA.ffData->mod);
  } else {
    skPartLen = BITS_TO_BYTES_CEIL(params->msbB);
    np = get_np_len(params->EB.ffData->mod);
  }

  memcpy(enc, s, params->msg_bytes);
  memset(enc + params->msg_bytes, 0, skPartLen + 6*np);
  itoos(sk, enc + params->msg_bytes);
  pktoos(params, pk, enc + params->msg_bytes + skPartLen, party);
}

size_t sktoos_len(const sike_params_t* params,
                           const party_t party) {
  size_t skLen = 0, np = 0;

  if (party == ALICE) {
    skLen = BITS_TO_BYTES_CEIL(params->msbA);
    np = get_np_len(params->EA.ffData->mod);
  } else {
    skLen = BITS_TO_BYTES_CEIL(params->msbB);
    np = get_np_len(params->EB.ffData->mod);
  }
  return params->msg_bytes + skLen + PUBLIC_KEY_NPS*np;
}

///////////////////////////////////////////////
// Encoding functions for public keys
///////////////////////////////////////////////


int ostopk(const sike_params_t *params,
           party_t party,
           const unsigned char *pk,
           sike_public_key_t *dec) {

  int rc = 0;

  size_t np = (party == ALICE ?
               get_np_len(params->EA.ffData->mod) :
               get_np_len(params->EB.ffData->mod));

  rc  = ostofp2(pk       , np, params->p, &dec->xP);
  rc |= ostofp2(pk + 2*np, np, params->p, &dec->xQ);
  rc |= ostofp2(pk + 4*np, np, params->p, &dec->xR);

  dec->ffData = (party == ALICE ? params->EA.ffData : params->EB.ffData);

  return rc;
}


void pktoos(const sike_params_t* params,
           const sike_public_key_t* pk,
           unsigned char* enc,
           party_t party) {

  size_t np = (party == ALICE ?
               get_np_len(params->EA.ffData->mod) :
               get_np_len(params->EB.ffData->mod));

  memset(enc, 0, PUBLIC_KEY_NPS*np);

  fp2toos(params, &pk->xP, enc       );
  fp2toos(params, &pk->xQ, enc + 2*np);
  fp2toos(params, &pk->xR, enc + 4*np);

}

void pktoos_alloc(const sike_params_t *params,
                 const sike_public_key_t *pk,
                 unsigned char **enc,
                 size_t *encLen,
                 party_t party) {

  *encLen = pktoos_len(params, party);
  *enc = calloc(*encLen, 1);
  pktoos(params, pk, *enc, party);

}

size_t pktoos_len(const sike_params_t* params,
                          const party_t party) {

  if (party == ALICE)
    return PUBLIC_KEY_NPS * get_np_len(params->EA.ffData->mod);
  else
    return PUBLIC_KEY_NPS * get_np_len(params->EB.ffData->mod);
}


///////////////////////////////////////////////
// Encoding functions for encapsulations (ct)
///////////////////////////////////////////////

int ostoencaps(const sike_params_t* params,
               const unsigned char* ct,
               sike_public_key_t* c0,
               unsigned char* c1) {
  int rc = 0;
  size_t np = get_np_len(params->EA.ffData->mod);

  rc = ostopk(params, BOB, ct, c0);
  memcpy(c1, ct + PUBLIC_KEY_NPS*np, params->msg_bytes);

  return rc;
}


void encapstoos(const sike_params_t* params,
               const sike_public_key_t* c0,
               const unsigned char* c1,
               unsigned char* ct) {

  size_t np = get_np_len(params->EB.ffData->mod);

  memset(ct, 0, PUBLIC_KEY_NPS*np);

  pktoos(params, c0, ct, BOB);

  memcpy(ct + PUBLIC_KEY_NPS*np, c1, params->msg_bytes);
}

void encapstoos_alloc(const sike_params_t *params,
                          const sike_public_key_t *c0,
                          const unsigned char *c1,
                          unsigned char **ct,
                          size_t *ctLen) {

  *ctLen = encapstoos_len(params);
  *ct = calloc(*ctLen, 1);

  encapstoos(params, c0, c1, *ct);
}

size_t encapstoos_len(const sike_params_t* params) {
  int components = PUBLIC_KEY_NPS;

  size_t np = get_np_len(params->EB.ffData->mod);
  return components * np + params->msg_bytes;
}

// Memory functions
void clear_free( void* ptr, size_t size, int free_mem )
{
  if (ptr) {
    memset(ptr, 0, size);
    *(volatile unsigned char*)ptr = *(volatile unsigned char*)ptr;
    if (free_mem == MEM_FREE)
      free(ptr);
  }
}