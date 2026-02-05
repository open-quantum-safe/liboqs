/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 2.2 (July 2025)
 *
 * Authors listed in alphabetical order:
 *
 * @author: Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author: Marco Gianvecchio <marco.gianvecchio@mail.polimi.it>
 * @author: Patrick Karl <patrick.karl@tum.de>
 * @author: Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author: Jonas Schupp <jonas.schupp@tum.de>
 *
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#include <stddef.h>  //  size_t
#include <string.h>  // memcpy

#include "CROSS.h"
#include "api.h"
#include "namespace.h"
#include "parameters.h"

/*----------------------------------------------------------------------------*/

int crypto_sign_keypair(unsigned char *pk,
                        unsigned char *sk) {
	/* keygen cannot fail */
	CROSS_keygen((sk_t *) sk,
	             (pk_t *) pk);

	return 0; // NIST convention: 0 == zero errors
} // end crypto_sign_keypair

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*... generating a signed message sm[0],sm[1],...,sm[*smlen-1]                */
/*... from original message m[0],m[1],...,m[mlen-1]                           */
/*... under secret key sk[0],sk[1],...                                        */
int crypto_sign(unsigned char *sm,                              // out parameter
                size_t *smlen,                                  // out parameter
                const unsigned char *m,                         // in parameter
                size_t mlen,                                    // in parameter
                const unsigned char *sk) {                      // in parameter
	/* sign cannot fail */
	memcpy(sm, m, mlen);
	CROSS_sign((const sk_t *) sk,                                // in parameter
	           (const char *const) m,                            // in parameter
	           (const size_t) mlen,                              // in parameter
	           (CROSS_sig_t *) (sm + mlen));                     // out parameter
	*smlen = mlen + (size_t) sizeof(CROSS_sig_t);

	return 0;  // NIST convention: 0 == zero errors
} // end crypto_sign

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*.  ... verifying a signed message sm[0],sm[1],...,sm[smlen-1]               */
/*.  ... under public key pk[0],pk[1],...                                     */
/*.  ... and producing original message m[0],m[1],...,m[*mlen-1]              */
int crypto_sign_open(unsigned char *m,                          // out parameter
                     size_t *mlen,                              // out parameter
                     const unsigned char *sm,                   // in parameter
                     size_t smlen,                              // in parameter
                     const unsigned char *pk) {                 // in parameter

	/* verify returns 1 if signature is ok, 0 otherwise */
	*mlen = smlen - (size_t) sizeof(CROSS_sig_t);

	memcpy((unsigned char *) m, (const unsigned char *) sm, (size_t) *mlen);
	int ok = CROSS_verify((const pk_t * const) pk,               // in parameter
	                      (const char *const) m,                 // in parameter
	                      (const size_t) * mlen,                 // in parameter
	                      (const CROSS_sig_t *const) (sm + *mlen)); // in parameter


	return ok - 1; // NIST convention: 0 == zero errors, -1 == error condition
} // end crypto_sign_open

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*... generating a signature sig[0],sig[1],...,sig[*siglen-1]                 */
/*... from original message m[0],m[1],...,m[mlen-1]                           */
/*... under secret key sk[0],sk[1],...                                        */
int crypto_sign_signature(unsigned char *sig,                   // out parameter
                          size_t *siglen,                       // out parameter
                          const unsigned char *m,               // in parameter
                          size_t mlen,                          // in parameter
                          const unsigned char *sk               // in parameter
                         ) {
	/* sign cannot fail */
	CROSS_sign((const sk_t *) sk,                                // in parameter
	           (const char *const) m, (const size_t) mlen,       // in parameter
	           (CROSS_sig_t *) sig);                             // out parameter
	*siglen = (size_t) sizeof(CROSS_sig_t);

	return 0;  // NIST convention: 0 == zero errors
} // end crypto_sign_signature

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*.  ... verifying a signature sig[0],sig[1],...,sig[siglen-1]                */
/*.  ... under public key pk[0],pk[1],...                                     */
/*.  ... and producing original message m[0],m[1],...,m[*mlen-1]              */
int crypto_sign_verify(const unsigned char *sig,                // in parameter
                       size_t siglen,                           // in parameter
                       const unsigned char *m,                  // in parameter
                       size_t mlen,                             // in parameter
                       const unsigned char *pk                  // in parameter
                      ) {

	/* verify returns 1 if signature is ok, 0 otherwise */
	int ok = CROSS_verify((const pk_t * const) pk,               // in parameter
	                      (const char *const) m,                 // in parameter
	                      (const size_t) mlen,                   // in parameter
	                      (const CROSS_sig_t *const) sig);       // in parameter

	/* liboqs-edit: check signature length */
	ok = ok && (siglen == sizeof(CROSS_sig_t));

	return ok - 1; // NIST convention: 0 == zero errors, -1 == error condition
} // end crypto_sign_verify

/*----------------------------------------------------------------------------*/
