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

#include "namespace.h"
#include "parameters.h"
#include "sha3.h"

/************************* CSPRNG ********************************/

#define CSPRNG_STATE_T SHAKE_STATE_STRUCT
/* initializes a CSPRNG, given the seed and a state pointer */
static inline
void csprng_initialize(CSPRNG_STATE_T *const csprng_state,
                       const unsigned char *const seed,
                       const uint32_t seed_len_bytes,
                       const uint16_t dsc) {
	// the second parameter is the security level of the SHAKE instance
	xof_shake_init(csprng_state, SEED_LENGTH_BYTES * 8);
	xof_shake_update(csprng_state, seed, seed_len_bytes);
	uint8_t dsc_ordered[2];
	dsc_ordered[0] = dsc & 0xff;
	dsc_ordered[1] = (dsc >> 8) & 0xff;
	xof_shake_update(csprng_state, dsc_ordered, 2);
	xof_shake_final(csprng_state);
} /* end initialize_csprng */

/* extracts xlen bytes from the CSPRNG, given the state */
static inline
void csprng_randombytes(unsigned char *const x,
                        unsigned long long xlen,
                        CSPRNG_STATE_T *const csprng_state) {
	xof_shake_extract(csprng_state, x, xlen);
}

/* PQClean-edit: CSPRNG release context */
static inline
void csprng_release(CSPRNG_STATE_T *const csprng_state) {
	xof_shake_release(csprng_state);
}

/************************* HASH functions ********************************/

/* Opaque algorithm agnostic hash call */
static inline
void hash(uint8_t digest[HASH_DIGEST_LENGTH],
          const unsigned char *const m,
          const uint64_t mlen,
          const uint16_t dsc) {
	/* SHAKE with a 2*lambda bit digest is employed also for hashing */
	CSPRNG_STATE_T csprng_state;
	xof_shake_init(&csprng_state, SEED_LENGTH_BYTES * 8);
	xof_shake_update(&csprng_state, m, mlen);
	uint8_t dsc_ordered[2];
	dsc_ordered[0] = dsc & 0xff;
	dsc_ordered[1] = (dsc >> 8) & 0xff;
	xof_shake_update(&csprng_state, dsc_ordered, 2);
	xof_shake_final(&csprng_state);
	xof_shake_extract(&csprng_state, digest, HASH_DIGEST_LENGTH);
	/* PQClean-edit: SHAKE release context */
	xof_shake_release(&csprng_state);
}

/***************** Specialized CSPRNGs for non binary domains *****************/

/* CSPRNG sampling fixed weight strings */
void expand_digest_to_fixed_weight(uint8_t fixed_weight_string[T],
                                   const uint8_t digest[HASH_DIGEST_LENGTH]);

#define BITS_FOR_P BITS_TO_REPRESENT(P-1)
#define BITS_FOR_Z BITS_TO_REPRESENT(Z-1)

static inline
void csprng_fp_vec(FP_ELEM res[N],
                   CSPRNG_STATE_T *const csprng_state) {
	const FP_ELEM mask = ( (FP_ELEM) 1 << BITS_FOR_P) - 1;
	uint8_t CSPRNG_buffer[ROUND_UP(BITS_N_FP_CT_RNG, 8) / 8];
	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right, shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, sizeof(CSPRNG_buffer), csprng_state);
	int placed = 0;
	uint64_t sub_buffer = 0;
	for (int i = 0; i < 8; i++) {
		sub_buffer |= ((uint64_t) CSPRNG_buffer[i]) << 8 * i;
	}
	/* position of the next fresh byte in CSPRNG_buffer*/
	int bits_in_sub_buf = 64;
	int pos_in_buf = 8;
	int pos_remaining = sizeof(CSPRNG_buffer) - pos_in_buf;
	while (placed < N) {
		if (bits_in_sub_buf <= 32 && pos_remaining > 0) {
			/* get at most 4 bytes from buffer */
			int refresh_amount = (pos_remaining >= 4) ? 4 : pos_remaining;
			uint32_t refresh_buf = 0;
			for (int i = 0; i < refresh_amount; i++) {
				refresh_buf |= ((uint32_t)CSPRNG_buffer[pos_in_buf + i]) << 8 * i;
			}
			pos_in_buf += refresh_amount;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 8 * refresh_amount;
			pos_remaining -= refresh_amount;
		}
		res[placed] = sub_buffer & mask;
		if (res[placed] < P) {
			placed++;
		}
		sub_buffer = sub_buffer >> BITS_FOR_P;
		bits_in_sub_buf -= BITS_FOR_P;
	}
}

#define BITS_FOR_P_M_ONE BITS_TO_REPRESENT(P-2)

static inline
void csprng_fp_vec_chall_1(FP_ELEM res[T],
                           CSPRNG_STATE_T *const csprng_state) {
	const FP_ELEM mask = ( (FP_ELEM) 1 << BITS_FOR_P_M_ONE) - 1;
	uint8_t CSPRNG_buffer[ROUND_UP(BITS_CHALL_1_FPSTAR_CT_RNG, 8) / 8];
	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right , shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, sizeof(CSPRNG_buffer), csprng_state);
	int placed = 0;
	uint64_t sub_buffer = 0;
	for (int i = 0; i < 8; i++) {
		sub_buffer |= ((uint64_t) CSPRNG_buffer[i]) << 8 * i;
	}
	/* position of the next fresh byte in CSPRNG_buffer*/
	int bits_in_sub_buf = 64;
	int pos_in_buf = 8;
	int pos_remaining = sizeof(CSPRNG_buffer) - pos_in_buf;
	while (placed < T) {
		if (bits_in_sub_buf <= 32 && pos_remaining > 0) {
			/* get at most 4 bytes from buffer */
			int refresh_amount = (pos_remaining >= 4) ? 4 : pos_remaining;
			uint32_t refresh_buf = 0;
			for (int i = 0; i < refresh_amount; i++) {
				refresh_buf |= ((uint32_t)CSPRNG_buffer[pos_in_buf + i]) << 8 * i;
			}
			pos_in_buf += refresh_amount;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 8 * refresh_amount;
			pos_remaining -= refresh_amount;
		}
		/* draw from 0 ... P-2, then add 1*/
		res[placed] = (sub_buffer & mask) + 1;
		if (res[placed] < P) {
			placed++;
		}
		sub_buffer = sub_buffer >> BITS_FOR_P_M_ONE;
		bits_in_sub_buf -= BITS_FOR_P_M_ONE;
	}
}

static inline
void csprng_fp_mat(FP_ELEM res[K][N - K],
                   CSPRNG_STATE_T *const csprng_state) {
	const FP_ELEM mask = ( (FP_ELEM) 1 << BITS_TO_REPRESENT(P - 1)) - 1;
	uint8_t CSPRNG_buffer[ROUND_UP(BITS_V_CT_RNG, 8) / 8];
	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right , shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, sizeof(CSPRNG_buffer), csprng_state);
	int placed = 0;
	uint64_t sub_buffer = 0;
	for (int i = 0; i < 8; i++) {
		sub_buffer |= ((uint64_t) CSPRNG_buffer[i]) << 8 * i;
	}
	/* position of the next fresh byte in CSPRNG_buffer*/
	int bits_in_sub_buf = 64;
	int pos_in_buf = 8;
	int pos_remaining = sizeof(CSPRNG_buffer) - pos_in_buf;
	while (placed < K * (N - K)) {
		if (bits_in_sub_buf <= 32 && pos_remaining > 0) {
			/* get at most 4 bytes from buffer */
			int refresh_amount = (pos_remaining >= 4) ? 4 : pos_remaining;
			uint32_t refresh_buf = 0;
			for (int i = 0; i < refresh_amount; i++) {
				refresh_buf |= ((uint32_t)CSPRNG_buffer[pos_in_buf + i]) << 8 * i;
			}
			pos_in_buf += refresh_amount;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 8 * refresh_amount;
			pos_remaining -= refresh_amount;
		}
		*((FP_ELEM *)res + placed) = sub_buffer & mask;
		if (*((FP_ELEM *)res + placed) < P) {
			placed++;
		}
		sub_buffer = sub_buffer >> BITS_FOR_P;
		bits_in_sub_buf -= BITS_FOR_P;
	}
}

static inline
void csprng_fz_inf_w(FZ_ELEM res[M],
                     CSPRNG_STATE_T *const csprng_state) {
	const FZ_ELEM mask = ( (FZ_ELEM) 1 << BITS_TO_REPRESENT(Z - 1)) - 1;
	uint8_t CSPRNG_buffer[ROUND_UP(BITS_M_FZ_CT_RNG, 8) / 8];
	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right , shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, sizeof(CSPRNG_buffer), csprng_state);
	int placed = 0;
	uint64_t sub_buffer = 0;
	for (int i = 0; i < 8; i++) {
		sub_buffer |= ((uint64_t) CSPRNG_buffer[i]) << 8 * i;
	}
	/* position of the next fresh byte in CSPRNG_buffer*/
	int bits_in_sub_buf = 64;
	int pos_in_buf = 8;
	int pos_remaining = sizeof(CSPRNG_buffer) - pos_in_buf;
	while (placed < M) {
		if (bits_in_sub_buf <= 32 && pos_remaining > 0) {
			/* get at most 4 bytes from buffer */
			int refresh_amount = (pos_remaining >= 4) ? 4 : pos_remaining;
			uint32_t refresh_buf = 0;
			for (int i = 0; i < refresh_amount; i++) {
				refresh_buf |= ((uint32_t)CSPRNG_buffer[pos_in_buf + i]) << 8 * i;
			}
			pos_in_buf += refresh_amount;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 8 * refresh_amount;
			pos_remaining -= refresh_amount;
		}
		res[placed] = sub_buffer & mask;
		if (res[placed] < Z) {
			placed++;
		}
		sub_buffer = sub_buffer >> BITS_FOR_Z;
		bits_in_sub_buf -= BITS_FOR_Z;
	}
}

static inline
void csprng_fz_mat(FZ_ELEM res[M][N - M],
                   CSPRNG_STATE_T *const csprng_state) {
	const FZ_ELEM mask = ( (FZ_ELEM) 1 << BITS_TO_REPRESENT(Z - 1)) - 1;
	uint8_t CSPRNG_buffer[ROUND_UP(BITS_W_CT_RNG, 8) / 8];
	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right , shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, sizeof(CSPRNG_buffer), csprng_state);

	int placed = 0;
	uint64_t sub_buffer = 0;
	for (int i = 0; i < 8; i++) {
		sub_buffer |= ((uint64_t) CSPRNG_buffer[i]) << 8 * i;
	}
	/* position of the next fresh byte in CSPRNG_buffer*/
	int bits_in_sub_buf = 64;
	int pos_in_buf = 8;
	int pos_remaining = sizeof(CSPRNG_buffer) - pos_in_buf;
	while (placed < M * (N - M)) {
		if (bits_in_sub_buf <= 32 && pos_remaining > 0) {
			/* get at most 4 bytes from buffer */
			int refresh_amount = (pos_remaining >= 4) ? 4 : pos_remaining;
			uint32_t refresh_buf = 0;
			for (int i = 0; i < refresh_amount; i++) {
				refresh_buf |= ((uint32_t)CSPRNG_buffer[pos_in_buf + i]) << 8 * i;
			}
			pos_in_buf += refresh_amount;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 8 * refresh_amount;
			pos_remaining -= refresh_amount;
		}
		*((FZ_ELEM *)res + placed) = sub_buffer & mask;
		if (*((FZ_ELEM *)res + placed) < Z) {
			placed++;
		}
		sub_buffer = sub_buffer >> BITS_FOR_Z;
		bits_in_sub_buf -= BITS_FOR_Z;
	}
}
