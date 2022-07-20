#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "params.h"
#include "symmetric.h"
#include "fips202.h"
extern param params[];
/*************************************************
* Name:        kyber_shake128_absorb
*
* Description: Absorb step of the SHAKE128 specialized for the Kyber context.
*
* Arguments:   - keccak_state *state: pointer to (uninitialized) output Keccak state
*              - const uint8_t *seed: pointer to KYBER_SYMBYTES input to be absorbed into state
*              - uint8_t i: additional byte of input
*              - uint8_t j: additional byte of input
**************************************************/
void kyber_shake128_absorb(shake128incctx *state,
                           const uint8_t *seed,
                           uint8_t x,
                           uint8_t y,
                           int8_t security_level)
{
  uint8_t extseed[params[security_level].KYBER_SYMBYTES+2];

  memcpy(extseed, seed, params[security_level].KYBER_SYMBYTES);
  extseed[params[security_level].KYBER_SYMBYTES+0] = x;
  extseed[params[security_level].KYBER_SYMBYTES+1] = y;

  shake128_absorb_once(state, extseed, sizeof(extseed));
}

/*************************************************
* Name:        kyber_shake256_prf
*
* Description: Usage of SHAKE256 as a PRF, concatenates secret and public input
*              and then generates outlen bytes of SHAKE256 output
*
* Arguments:   - uint8_t *out: pointer to output
*              - size_t outlen: number of requested output bytes
*              - const uint8_t *key: pointer to the key (of length KYBER_SYMBYTES)
*              - uint8_t nonce: single-byte nonce (public PRF input)
**************************************************/
void kyber_shake256_prf(uint8_t *out, 
                        size_t outlen,
                        const uint8_t *key,
                        uint8_t nonce,
                        int8_t security_level)
{
  uint8_t extkey[params[security_level].KYBER_SYMBYTES+1];

  memcpy(extkey, key, params[security_level].KYBER_SYMBYTES);
  extkey[params[security_level].KYBER_SYMBYTES] = nonce;

  shake256(out, outlen, extkey, sizeof(extkey));
}
