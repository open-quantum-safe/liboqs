#ifndef _lima
#define _lima

/** This is the main internal API, holding various Lima
 * objects and the functions to manipulate them.
 *
 * For single call/usage the external API in LimaAPI.h
 * may be more useful
 **/

#include "xof.h"
#include "modp.h"
#include <stdint.h>
#include <assert.h>


/** The following structures hold all the states needed for a 
 *  given context.
 *  To initialize the state correctly you need to call
 *    
 *         void precompute_fft_data(lima_params_t* params);
 *
 *  defined in fft.h
 **/

typedef enum { LIMA_2P, LIMA_SP } LimaMode;

#define LIMA_MAX_N 2062
#define LIMA_MAX_P 2063
#define LIMA_MAX_BD 8192         // 2**13

/** The following  holds the modp FFT data in converted 
 *  (e.g. Montgomery) form, for both fft and bfft
 *  data
 **/
typedef struct tag_lima_fft_t
{
    int powers[2][LIMA_MAX_P];
    int powersi[2][LIMA_MAX_P];
    int bd[2][LIMA_MAX_BD];
    int alpha0, alpha1, beta0, beta1;
} lima_fft_t;

/* Data in human readable form 
 *  - The precise values for each context are defined in lima.c
 */
typedef struct tag_lima_params_t
{
    LimaMode mode;            // LIMA_2P or LIMA_SP
    modp_context_t* ctx;
    byte_length N; 
    int e;                    // e not used in LIMA_2P
    int alpha0, alpha1, beta0, beta1;
    lima_fft_t* fft_data;
    int noise[41];
} lima_params_t;




/** Parameter sets for LIMA-2p:
 *  N=1024 and q=133121
 *  N=2018 and q=184321
 **/
extern lima_params_t lima_2p_1024;
extern lima_params_t lima_2p_2048;

/** Parameter sets for LIMA-sp:
 *  N= 1018 and q= 12521473
 *  N= 1306 and q= 48181249
 *  N= 1822 and q= 4480204
 *  N= 2062 and q= 16900097
 **/
extern lima_params_t lima_sp_1018;
extern lima_params_t lima_sp_1306;
extern lima_params_t lima_sp_1822;
extern lima_params_t lima_sp_2062;

extern modp_context_t mod133121;                //q values for LIMA-2p
extern modp_context_t mod184321;
extern modp_context_t mod12521473;              //q values for LIMA-sp
extern modp_context_t mod48181249;
extern modp_context_t mod44802049;
extern modp_context_t mod16900097;


/* Internal representation of public key pk */
typedef struct tag_lima_pk_t
{
  int a[LIMA_MAX_P];
  int b[LIMA_MAX_P];
} lima_pk_t;


/* Internal representation of secret key sk 
 *  sk holds the public key as needed for CCA versions 
 */
typedef struct tag_lima_sk_t
{
  lima_pk_t *pk;
  int s[LIMA_MAX_P];
} lima_sk_t;


/* Internal representation of a ciphertext */
typedef struct tag_lima_ciphertext_t
{
  int c0[LIMA_MAX_P];
  int c1[LIMA_MAX_P];
  int ell;              // Length of c0
} lima_ciphertext_t;


/* Function used to initialise all data structures 
 * When using the internal API this must be called explicitly
 */
void InitLima();


/** Function used for  key generation 
 *   - params      Pointer to LIMA parameters 
 *   - seed        Pointer to the seed (for input to the XOF)
 *   - seedlen     Seed length in bytes 
 *   - pk          Pointer to the public key (pk)
 *   - sk          Pointer to the secret key (sk)
 *  Return 0 if successful, and 1 otherwise 
**/
int lima_key_gen(lima_params_t* params, 
		 const uint8_t* seed, byte_length seedlen, 
		 lima_pk_t* pk, lima_sk_t* sk);

/** Function used to encrypt a message m of length  m_len bytes (at most N/8). 
 *  It uses RandCheck to verify that polynomials e and *  v, used 
 *  to encrypt, are in the correct range. 
 *
 *  This routine is used as a helper routine in lima_enc_cpa, to 
 *  prevent decrypion failures.
 *
 *  It really should not be called directly.
 *    - params                 Pointer to LIMA paramenters
 *    - m                      Pointer to the message (m)
 *    - m_len                  Message length in bytes
 *    - pk                     Pointer to the pk
 *    - xof                    Pointer to an already initialized  xof
 *    - c                      Pointer to the ciphertext (c)     
 *  Return 1 if:
 *       - RandCheck returns 0                or if
 *       - xof produces  invalid randomness   or if
 *       - message  too long (8*m_len > N); 
 *  0 otherwise
 **/
int lima_enc_cpa_sub(lima_params_t* params, const uint8_t* m, byte_length m_len,
                     const lima_pk_t* pk,
                     XOF_t* xof,
                     lima_ciphertext_t* c);


/** Function used for CPA-encryption of a message m of length m_len bytes;   
 *     - seed_len and m_len are in bytes. 
 *  This function never returns an invalid ciphertext, as it calls 
 *  lima_enc_cpa_sub until a valid ciphertext is generated.
 *    - Input paramenters are the same as for lima_enc_cpa_sub, except 
 *      for the pointer seed of length seed_len, used instead of xof
 *  Return 1 if 
 *    - seed_len < 32                
 *  or if
 *    - message  too long (8*m_len > N);
 *  0 otherwise
 **/
int lima_enc_cpa(lima_params_t* params, const uint8_t* m, byte_length m_len, 
                 const lima_pk_t* pk, 
                 const uint8_t* seed, byte_length seed_len, 
                 lima_ciphertext_t* c);

/** Function used for CPA-decryption 
 *  It  assumes m has enough space allocated already (c.ell/8 bytes)
 *  Return 1 if c->ell != 0 mod 8, i.e. if the length of c0 is not 0 mod 8,
 *  0 otherwise 
 **/
int lima_dec_cpa(lima_params_t* params, 
		 const lima_ciphertext_t* c, const lima_sk_t* sk,
                 uint8_t* m,byte_length *m_len);

/** Function used for CCA-encryption  of  a message m of  length m_len bytes; 
 *  Encrypts a message m of m_len<(N-256)/8 bytes in length
 *
 *  This function always returns a valid ciphertext, it uses lima_enc_cpa_sub
 *  as a subroutine, but the number of loops is alway 1 except with
 *  very small probability.  
 *  Seed length is fixed at 32 bytes
 *  This function returns 1 if the message is too long, 0 otherwise 
 **/
int lima_enc_cca(lima_params_t* params, 
		 const uint8_t* m, byte_length m_len,
                 const lima_pk_t* pk,
                 const uint8_t seed[32],
                 lima_ciphertext_t* c);

/** 
 *  Function used to decrypt the above CCA-encryption, 
 *    - Assumes m has enough space allocated already (c.ell-256)/8 bytes)
 *  It calls lima_dec_cpa to decrypt and lima_enc_cpa to check the decryption
 *  This function returns 1 if lima_dec_cpa returns 1 or if the check fails,
 *  it returns 0 otherwise.
 **/
int lima_dec_cca(lima_params_t* params, 
		const lima_ciphertext_t* c, const lima_sk_t* sk, 
                 uint8_t* m, byte_length *m_len);

/** LIMA CPA-Key-Encapsulation mechanism
 *  Output key has length key_len in bytes, 
 *     - Assumes key has enough space already allocated key_len in bytes
 *     - seed_len is in bytes
 *     - seed_len >= key_len +32
 *     - 32 <= key_len < N/8
 **/
int lima_encap_cpa(lima_params_t* params,const lima_pk_t* pk,
                   const uint8_t* seed, byte_length seed_len,
                   uint8_t* key,byte_length key_len, 
                   lima_ciphertext_t* c);

/** LIMA CPA-Key-Decapsulation mechanism
 *  This returns the key length as well, but again enough space is 
 *  assumed to have been allocated already
 **/
int lima_decap_cpa(lima_params_t* params, const lima_ciphertext_t* c, const lima_sk_t* sk, 
                   uint8_t* key, byte_length *key_len);

/**  LIMA CCA-Key-Encapsulation mechanisms
 *   Output key has length key_len in bytes, we assume key has enough space 
 *   already allocated
 *      32 <= seed_len < N/8
 **/
int lima_encap_cca(lima_params_t* params,const lima_pk_t* pk,
                   const uint8_t* seed, byte_length seed_len,
                   uint8_t* key,byte_length key_len,
                   lima_ciphertext_t* c);

/** LIMA CCA-Key-Decapsulation mechanisms
 *   - Enough space for key is assumed to have been allocated
 **/
int lima_decap_cca(lima_params_t* params, const lima_ciphertext_t* c, const lima_sk_t* sk,
                   uint8_t* key, byte_length key_len);


/** Encode/decode routines from byte strings.
 *  The byte strings input are assumed to have enough space already allocated
 *   - Decode routines return 0 if OK, 1 if not
 *   - Encode routines return the length used
 **/

int EncodePK(lima_params_t* params, const lima_pk_t* pk, uint8_t *B);
int EncodeSK(lima_params_t* params, const lima_sk_t* sk, uint8_t *B);
int EncodeCT(lima_params_t* params, const lima_ciphertext_t* c, uint8_t *B);

int DecodePK(lima_params_t** params, lima_pk_t* pk, const uint8_t *B);
int DecodeSK(lima_params_t** params, lima_sk_t* sk, lima_pk_t* pk, const uint8_t *B);
int DecodeCT(lima_params_t** params, lima_ciphertext_t* c, const uint8_t *B);

#endif
