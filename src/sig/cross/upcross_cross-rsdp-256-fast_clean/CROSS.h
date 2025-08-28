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

#pragma once

#include <stdint.h>

#include "namespace.h"
#include "pack_unpack.h"
#include "parameters.h"

/* Public key: the parity check matrix is shrunk to a seed, syndrome
 * represented in full */
typedef struct {
	uint8_t seed_pk[KEYPAIR_SEED_LENGTH_BYTES];
	uint8_t s[DENSELY_PACKED_FP_SYN_SIZE];
} pk_t;

/* Private key: just a single seed*/
typedef struct {
	uint8_t seed_sk[KEYPAIR_SEED_LENGTH_BYTES];
} sk_t;

typedef struct {
	uint8_t y[DENSELY_PACKED_FP_VEC_SIZE];
	uint8_t v_bar[DENSELY_PACKED_FZ_VEC_SIZE];
} resp_0_t;

/* Signature: */
typedef struct {
	uint8_t salt[SALT_LENGTH_BYTES];
	uint8_t digest_cmt[HASH_DIGEST_LENGTH];
	uint8_t digest_chall_2[HASH_DIGEST_LENGTH];
	uint8_t path[W * SEED_LENGTH_BYTES];
	uint8_t proof[W * HASH_DIGEST_LENGTH];
	uint8_t resp_1[T - W][HASH_DIGEST_LENGTH];
	resp_0_t resp_0[T - W];
} CROSS_sig_t;


/* keygen cannot fail */
void CROSS_keygen(sk_t *SK,
                  pk_t *PK);

/* sign cannot fail */
void CROSS_sign(const sk_t *SK,
                const char *m,
                uint64_t mlen,
                CROSS_sig_t *sig);

/* verify returns 1 if signature is ok, 0 otherwise */
int CROSS_verify(const pk_t *PK,
                 const char *m,
                 uint64_t mlen,
                 const CROSS_sig_t *sig);
