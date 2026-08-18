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
} /* end csprng_initialize */

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

/*************** Parallel CSPRNG (x2, x3, x4) ********************/

#define CSPRNG_X2_STATE_T SHAKE_X2_STATE_STRUCT
/* CRSPRNG_x3 calls SHAKE_x4 and discards the fourth input/output */
#define CSPRNG_X3_STATE_T SHAKE_X4_STATE_STRUCT
#define CSPRNG_X4_STATE_T SHAKE_X4_STATE_STRUCT

/* initialize */
static inline
void csprng_initialize_x2(CSPRNG_X2_STATE_T *const csprng_state,
                          const unsigned char *const seed1,
                          const unsigned char *const seed2,
                          const uint32_t seed_len_bytes,
                          const uint16_t dsc1,
                          const uint16_t dsc2) {
	xof_shake_x2_init(csprng_state, SEED_LENGTH_BYTES * 8);
	xof_shake_x2_update(csprng_state, seed1, seed2, seed_len_bytes);
	uint8_t dsc_ordered1[2], dsc_ordered2[2];
	dsc_ordered1[0] = dsc1 & 0xff;
	dsc_ordered1[1] = (dsc1 >> 8) & 0xff;
	dsc_ordered2[0] = dsc2 & 0xff;
	dsc_ordered2[1] = (dsc2 >> 8) & 0xff;
	xof_shake_x2_update(csprng_state, dsc_ordered1, dsc_ordered2, 2);
	xof_shake_x2_final(csprng_state);
}
static inline
void csprng_initialize_x3(CSPRNG_X3_STATE_T *const csprng_state,
                          const unsigned char *const seed1,
                          const unsigned char *const seed2,
                          const unsigned char *const seed3,
                          const uint32_t seed_len_bytes,
                          const uint16_t dsc1,
                          const uint16_t dsc2,
                          const uint16_t dsc3) {
	/* PQClean-edit: avoid VLA */
	// const unsigned char seed4[seed_len_bytes]; // discarded
	unsigned char *seed4 = (unsigned char *)OQS_MEM_malloc(seed_len_bytes); // discarded
	xof_shake_x4_init(csprng_state);
	xof_shake_x4_update(csprng_state, seed1, seed2, seed3, seed4, seed_len_bytes);
	uint8_t dsc_ordered1[2], dsc_ordered2[2], dsc_ordered3[2], dsc_ordered4[2]; // dsc_ordered4 is discarded
	dsc_ordered1[0] = dsc1 & 0xff;
	dsc_ordered1[1] = (dsc1 >> 8) & 0xff;
	dsc_ordered2[0] = dsc2 & 0xff;
	dsc_ordered2[1] = (dsc2 >> 8) & 0xff;
	dsc_ordered3[0] = dsc3 & 0xff;
	dsc_ordered3[1] = (dsc3 >> 8) & 0xff;
	xof_shake_x4_update(csprng_state, dsc_ordered1, dsc_ordered2, dsc_ordered3, dsc_ordered4, 2);
	xof_shake_x4_final(csprng_state);
	OQS_MEM_secure_free(seed4, seed_len_bytes);
}
static inline
void csprng_initialize_x4(CSPRNG_X4_STATE_T *const csprng_state,
                          const unsigned char *const seed1,
                          const unsigned char *const seed2,
                          const unsigned char *const seed3,
                          const unsigned char *const seed4,
                          const uint32_t seed_len_bytes,
                          const uint16_t dsc1,
                          const uint16_t dsc2,
                          const uint16_t dsc3,
                          const uint16_t dsc4) {
	xof_shake_x4_init(csprng_state);
	xof_shake_x4_update(csprng_state, seed1, seed2, seed3, seed4, seed_len_bytes);
	uint8_t dsc_ordered1[2], dsc_ordered2[2], dsc_ordered3[2], dsc_ordered4[2];
	dsc_ordered1[0] = dsc1 & 0xff;
	dsc_ordered1[1] = (dsc1 >> 8) & 0xff;
	dsc_ordered2[0] = dsc2 & 0xff;
	dsc_ordered2[1] = (dsc2 >> 8) & 0xff;
	dsc_ordered3[0] = dsc3 & 0xff;
	dsc_ordered3[1] = (dsc3 >> 8) & 0xff;
	dsc_ordered4[0] = dsc4 & 0xff;
	dsc_ordered4[1] = (dsc4 >> 8) & 0xff;
	xof_shake_x4_update(csprng_state, dsc_ordered1, dsc_ordered2, dsc_ordered3, dsc_ordered4, 2);
	xof_shake_x4_final(csprng_state);
}
/* randombytes */
static inline
void csprng_randombytes_x2(unsigned char *const x1, unsigned char *const x2, uint64_t xlen, CSPRNG_X2_STATE_T *const csprng_state) {
	xof_shake_x2_extract(csprng_state, x1, x2, xlen);
}
static inline
void csprng_randombytes_x3(unsigned char *const x1, unsigned char *const x2, unsigned char *const x3, uint64_t xlen, CSPRNG_X3_STATE_T *const csprng_state) {
	/* PQClean-edit: avoid VLA */
	// unsigned char x4[xlen]; // discarded
	unsigned char *x4 = (unsigned char *)OQS_MEM_malloc(xlen); // discarded
	xof_shake_x4_extract(csprng_state, x1, x2, x3, x4, xlen);
	OQS_MEM_secure_free(x4, xlen);
}
static inline
void csprng_randombytes_x4(unsigned char *const x1, unsigned char *const x2, unsigned char *const x3, unsigned char *const x4, uint64_t xlen, CSPRNG_X4_STATE_T *const csprng_state) {
	xof_shake_x4_extract(csprng_state, x1, x2, x3, x4, xlen);
}
/* PQClean-edit: CSPRNG release context */
static inline
void csprng_release_x2(CSPRNG_X2_STATE_T *const csprng_state) {
	xof_shake_x2_release(csprng_state);
}
static inline
void csprng_release_x3(CSPRNG_X3_STATE_T *const csprng_state) {
	xof_shake_x4_release(csprng_state);
}
static inline
void csprng_release_x4(CSPRNG_X4_STATE_T *const csprng_state) {
	xof_shake_x4_release(csprng_state);
}


/**************** Common API for Parallel CSPRNG *****************/

#define PAR_CSPRNG_STATE_T par_shake_ctx

static inline
void csprng_initialize_par(int par_level,
                           PAR_CSPRNG_STATE_T *const states,
                           const unsigned char *const seed1,
                           const unsigned char *const seed2,
                           const unsigned char *const seed3,
                           const unsigned char *const seed4,
                           const uint32_t seed_len_bytes,
                           const uint16_t dsc1,
                           const uint16_t dsc2,
                           const uint16_t dsc3,
                           const uint16_t dsc4) {
	if (par_level == 1) {
		csprng_initialize(&(states->state1), seed1, seed_len_bytes, dsc1);
	} else if (par_level == 2) {
		csprng_initialize_x2(&(states->state2), seed1, seed2, seed_len_bytes, dsc1, dsc2);
	} else if (par_level == 3) {
		csprng_initialize_x3(&(states->state4), seed1, seed2, seed3, seed_len_bytes, dsc1, dsc2, dsc3);
	} else if (par_level == 4) {
		csprng_initialize_x4(&(states->state4), seed1, seed2, seed3, seed4, seed_len_bytes, dsc1, dsc2, dsc3, dsc4);
	}
}
static inline
void csprng_randombytes_par(int par_level, PAR_CSPRNG_STATE_T *const states, unsigned char *const x1, unsigned char *const x2, unsigned char *const x3, unsigned char *const x4, uint64_t xlen) {
	if (par_level == 1) {
		csprng_randombytes(x1, xlen, &(states->state1));
	} else if (par_level == 2) {
		csprng_randombytes_x2(x1, x2, xlen, &(states->state2));
	} else if (par_level == 3) {
		csprng_randombytes_x3(x1, x2, x3, xlen, &(states->state4));
	} else if (par_level == 4) {
		csprng_randombytes_x4(x1, x2, x3, x4, xlen, &(states->state4));
	}
}
/* PQClean-edit: CSPRNG release context */
static inline
void csprng_release_par(int par_level, PAR_CSPRNG_STATE_T *const states) {
	if (par_level == 1) {
		csprng_release(&(states->state1));
	} else if (par_level == 2) {
		csprng_release_x2(&(states->state2));
	} else if (par_level == 3) {
		csprng_release_x3(&(states->state4));
	} else if (par_level == 4) {
		csprng_release_x4(&(states->state4));
	}
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

#define par_xof_input csprng_initialize_par
#define par_xof_output csprng_randombytes_par
/* PQClean-edit: SHAKE release context */
#define par_xof_release csprng_release_par

static inline
void hash_par(int par_level,
              uint8_t digest_1[HASH_DIGEST_LENGTH],
              uint8_t digest_2[HASH_DIGEST_LENGTH],
              uint8_t digest_3[HASH_DIGEST_LENGTH],
              uint8_t digest_4[HASH_DIGEST_LENGTH],
              const unsigned char *const m_1,
              const unsigned char *const m_2,
              const unsigned char *const m_3,
              const unsigned char *const m_4,
              const uint64_t mlen,
              const uint16_t dsc1,
              const uint16_t dsc2,
              const uint16_t dsc3,
              const uint16_t dsc4) {
	PAR_CSPRNG_STATE_T states;
	par_xof_input(par_level, &states, m_1, m_2, m_3, m_4, mlen, dsc1, dsc2, dsc3, dsc4);
	par_xof_output(par_level, &states, digest_1, digest_2, digest_3, digest_4, HASH_DIGEST_LENGTH);
	/* PQClean-edit: SHAKE release context */
	par_xof_release(par_level, &states);
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
