/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (March 2023)
 *
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
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

#include "pack_unpack.h"
#include "parameters.h"

/* Public key: the parity check matrix is shrunk to a seed, syndrome
 * represented in full */
typedef struct {
	uint8_t seed_pub[KEYPAIR_SEED_LENGTH_BYTES];
	uint8_t s[DENSELY_PACKED_FQ_SYN_SIZE];
} pubkey_t;

/* Private key: just a single seed*/
typedef struct {
	uint8_t seed[KEYPAIR_SEED_LENGTH_BYTES];
} prikey_t;

typedef struct {
	uint8_t y[DENSELY_PACKED_FQ_VEC_SIZE];
	uint8_t sigma[DENSELY_PACKED_FZ_VEC_SIZE];
} rsp_0_t;

/* Signature: */
typedef struct {
	uint8_t salt[SALT_LENGTH_BYTES];
	uint8_t digest_01[HASH_DIGEST_LENGTH];
	uint8_t digest_b[HASH_DIGEST_LENGTH];
	uint8_t stp[W * SEED_LENGTH_BYTES];
	uint8_t mtp[W * HASH_DIGEST_LENGTH];
	rsp_0_t rsp_0[T - W];
	uint8_t rsp_1[T - W][HASH_DIGEST_LENGTH];
} CROSS_sig_t;

/* keygen cannot fail */
void PQCLEAN_CROSSRSDP128FAST_CLEAN_CROSS_keygen(prikey_t *SK,
        pubkey_t *PK);

/* sign cannot fail */
void PQCLEAN_CROSSRSDP128FAST_CLEAN_CROSS_sign(const prikey_t *SK,
        const char *m,
        size_t mlen,
        CROSS_sig_t *sig);

/* verify returns 1 if signature is ok, 0 otherwise */
int PQCLEAN_CROSSRSDP128FAST_CLEAN_CROSS_verify(const pubkey_t *PK,
        const char *m,
        size_t mlen,
        const CROSS_sig_t *sig);
