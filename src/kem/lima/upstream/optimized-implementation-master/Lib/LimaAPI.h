#ifndef _LimaAPI
#define _LimaAPI

/* This is the main external API file.
 *
 * For the extenral API you do not need to call InitLima first
 * if you do not want to (it is called by each routine just
 * in case, and returns instantly if stuff is already initialized)
 *
 * The file lima.h is  more like an internal API for
 * when you have various Lima objects lying around
 *
 * All functions return 0 if successful, and 1 otherwise
 *
 */

#include <stdint.h>
#include "lima.h"


/* The lengths in this function for pk and sk define the param
 * type which will be used (i.e. for LIMA-2p-1024 we have
 * pklen=6145 and sklen=9217)
 *
 * This function outputs 
 *   - The encoding of the public key (a bytestring of length pklen)
 *   - The encoding of the secret key (a bytestring of length sklen)
 *       - Space already assumed allocated for both
 * Input a seed of length slen in bytes
 */
int Key_Generation(uint8_t* pks, unsigned long long pklen,
                   uint8_t* sks, unsigned long long sklen,
                   const uint8_t *seed, unsigned long long slen);

/* Input
 *   - message m of byte length mlen
 *   - seed seed of byte length slen
 *   - Encoding of public key pks
 * Output
 *   - Encoding of the ciphertext of length clen
 *       - Assumes enough space is already allocated
 * Requires
 *   - slen >= 32
 *   - 8*mlen < params.N
 */
int Enc_CPA(uint8_t* c, unsigned long long *clen,
            const uint8_t* m, unsigned long long mlen,
            const uint8_t *seed, unsigned long long slen,
            const uint8_t* pks);

/* Input
 *    - Encoding of the secret key sks
 *    - Encoding of the ciphertext c
 * Output
 *    - Message m of length mlen
 *         - Assumes enough space is already allocated
 */
int Dec_CPA(uint8_t* m, unsigned long long *mlen,
            const uint8_t* c,
            const uint8_t* sks);

/* Input
 *   - message m of byte length mlen
 *   - seed seed of 32 bytes
 *   - Encoding of public key pks
 * Output
 *   - Encoding of the ciphertext of length clen
 *       - Assumes enough space is already allocated
 * Requires
 *   - mlen < (params.N-256)/8
 */
int Enc_CCA(uint8_t* c, unsigned long long *clen,
            const uint8_t* m, unsigned long long mlen,
            const uint8_t seed[32],
            const uint8_t* pks);

/* Input
 *    - Encoding of the secret key sks
 *    - Encoding of the ciphertext c
 * Output
 *    - Message m of length mlen
 *         - Assumes enough space is already allocated
 */
int Dec_CCA(uint8_t* m, unsigned long long *mlen,
            const uint8_t* c,
            const uint8_t* sks);

/* Input
 *   - seed seed of byte length slen
 *   - Encoding of public key pks
 *   - klen the desired KEM key length
 * Output
 *   - Encoding of the ciphertext of length clen
 *       - Assumes enough space is already allocated
 *   - KEM key k of length klen in bytes
 * Requires
 *   - 256  <= klen*8 < params.N
 *   - slen >= klen+32
 */
int Encap_CPA(uint8_t* c, unsigned long long *clen,
              uint8_t* k, unsigned long long klen,
              const uint8_t *seed, unsigned long long slen,
              const uint8_t* pks);

/* Input
 *    - Encoding of the secret key sks
 *    - Encoding of the ciphertext c
 * Output
 *    - Key length klen
 *    - Key k of length klen
 *         - Assumes enough space is already allocated
 */
int Decap_CPA(uint8_t* k, unsigned long long *klen,
              const uint8_t* c,
              const uint8_t* sks);

/* Input
 *   - seed seed of byte length slen
 *   - Encoding of public key pks
 *   - klen the desired KEM key length
 * Output
 *   - Encoding of the ciphertext of length clen
 *       - Assumes enough space is already allocated
 *   - KEM key k of length klen in bytes
 * Requires
 *   - params.N/8 > slen >= 32
 */
int Encap_CCA(uint8_t* c, unsigned long long *clen,
              uint8_t* k, unsigned long long klen,
              const uint8_t *seed, unsigned long long slen,
              const uint8_t* pks);


/* Input
 *    - Encoding of the secret key sks
 *    - Encoding of the ciphertext c
 *    - klen
 * Output
 *    - Key k of length klen
 *         - Assumes enough space is already allocated
 */
int Decap_CCA(uint8_t* k, unsigned long long klen,
              const uint8_t* c,
              const uint8_t* sks);

#endif
