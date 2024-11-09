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

#include "parameters.h"
#include "sha3.h"

/************************* CSPRNG ********************************/

#define CSPRNG_STATE_T SHAKE_STATE_STRUCT
/* initializes a CSPRNG, given the seed and a state pointer */
static inline
void initialize_csprng(CSPRNG_STATE_T *const csprng_state,
                       const unsigned char *const seed,
                       const uint32_t seed_len_bytes) {
	// the second parameter is the security level of the SHAKE instance
	xof_shake_init(csprng_state, SEED_LENGTH_BYTES * 8);
	xof_shake_update(csprng_state, seed, seed_len_bytes);
	xof_shake_final(csprng_state);
} /* end initialize_csprng */

/* extracts xlen bytes from the CSPRNG, given the state */
static inline
void csprng_randombytes(unsigned char *const x,
                        uint64_t xlen,
                        CSPRNG_STATE_T *const csprng_state) {
	xof_shake_extract(csprng_state, x, xlen);
}

/* PQClean-edit: CSPRNG release context */
static inline
void csprng_release(CSPRNG_STATE_T *const csprng_state) {
	xof_shake_release(csprng_state);
}

/**************** Parallel CSPRNG (x2, x3, x4) ***********************/

#define CSPRNG_X2_STATE_T SHAKE_X2_STATE_STRUCT
/* CRSPRNG_x3 calls SHAKE_x4 and discards the fourth input/output */
#define CSPRNG_X3_STATE_T SHAKE_X4_STATE_STRUCT
#define CSPRNG_X4_STATE_T SHAKE_X4_STATE_STRUCT

/* initialize */
static inline
void initialize_csprng_x2(CSPRNG_X2_STATE_T *const csprng_state, const unsigned char *const seed1, const unsigned char *const seed2, const uint32_t seed_len_bytes) {
	xof_shake_x2_init(csprng_state);
	xof_shake_x2_update(csprng_state, seed1, seed2, seed_len_bytes);
	xof_shake_x2_final(csprng_state);
}
static inline
void initialize_csprng_x3(CSPRNG_X3_STATE_T *const csprng_state, const unsigned char *const seed1, const unsigned char *const seed2, const unsigned char *const seed3, const uint32_t seed_len_bytes) {
	/* to perform xof_x3 it's faster to call xof_x4 and discard the last output */
	xof_shake_x4_init(csprng_state);
	xof_shake_x4_update(csprng_state, seed1, seed2, seed3, seed3, seed_len_bytes);
	xof_shake_x4_final(csprng_state);
}
static inline
void initialize_csprng_x4(CSPRNG_X4_STATE_T *const csprng_state, const unsigned char *const seed1, const unsigned char *const seed2, const unsigned char *const seed3, const unsigned char *const seed4, const uint32_t seed_len_bytes) {
	xof_shake_x4_init(csprng_state);
	xof_shake_x4_update(csprng_state, seed1, seed2, seed3, seed4, seed_len_bytes);
	xof_shake_x4_final(csprng_state);
}
/* randombytes */
static inline
void csprng_randombytes_x2(unsigned char *const x1, unsigned char *const x2, uint64_t xlen, CSPRNG_X2_STATE_T *const csprng_state) {
	xof_shake_x2_extract(csprng_state, x1, x2, xlen);
}
static inline
void csprng_randombytes_x3(unsigned char *const x1, unsigned char *const x2, unsigned char *const x3, uint64_t xlen, CSPRNG_X3_STATE_T *const csprng_state) {
	/* to perform xof_x3 it's faster to call xof_x4 and discard the last output */
	xof_shake_x4_extract(csprng_state, x1, x2, x3, x3, xlen);
}
static inline
void csprng_randombytes_x4(unsigned char *const x1, unsigned char *const x2, unsigned char *const x3, unsigned char *const x4, uint64_t xlen, CSPRNG_X4_STATE_T *const csprng_state) {
	xof_shake_x4_extract(csprng_state, x1, x2, x3, x4, xlen);
}
/* release */
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

/************** Single API for Parallel CSPRNG *******************/

#define PAR_CSPRNG_STATE_T par_shake_ctx

static inline
void par_initialize_csprng(int par_level, PAR_CSPRNG_STATE_T *const states, const unsigned char *const seed1, const unsigned char *const seed2, const unsigned char *const seed3, const unsigned char *const seed4, const uint32_t seed_len_bytes) {
	if (par_level == 1) {
		initialize_csprng(&(states->state1), seed1, seed_len_bytes);
	} else if (par_level == 2) {
		initialize_csprng_x2(&(states->state2), seed1, seed2, seed_len_bytes);
	} else if (par_level == 3) {
		initialize_csprng_x3(&(states->state4), seed1, seed2, seed3, seed_len_bytes);
	} else if (par_level == 4) {
		initialize_csprng_x4(&(states->state4), seed1, seed2, seed3, seed4, seed_len_bytes);
	}
}
static inline
void par_csprng_randombytes(int par_level, PAR_CSPRNG_STATE_T *const states, unsigned char *const x1, unsigned char *const x2, unsigned char *const x3, unsigned char *const x4, uint64_t xlen) {
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
static inline
void par_csprng_release(int par_level, PAR_CSPRNG_STATE_T *const states) {
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

/******************************************************************************/

/* PQClean-edit: remove randombytes definition here to use PQClean randombytes */
//static inline void randombytes(unsigned char * x, uint64_t xlen) ...

/************************* HASH functions ********************************/

/* Opaque algorithm agnostic hash call */
static inline
void hash(uint8_t digest[HASH_DIGEST_LENGTH],
          const unsigned char *const m,
          const uint64_t mlen) {
	/* SHAKE with a 2*lambda bit digest is employed also for hashing */
	CSPRNG_STATE_T csprng_state;
	xof_shake_init(&csprng_state, SEED_LENGTH_BYTES * 8);
	xof_shake_update(&csprng_state, m, mlen);
	xof_shake_final(&csprng_state);
	xof_shake_extract(&csprng_state, digest, HASH_DIGEST_LENGTH);
	/* PQClean-edit: CSPRNG release context */
	xof_shake_release(&csprng_state);
}

#define par_xof_input par_initialize_csprng
#define par_xof_output par_csprng_randombytes
#define par_xof_release par_csprng_release

static inline
void par_hash(
    int par_level,
    uint8_t digest_1[HASH_DIGEST_LENGTH],
    uint8_t digest_2[HASH_DIGEST_LENGTH],
    uint8_t digest_3[HASH_DIGEST_LENGTH],
    uint8_t digest_4[HASH_DIGEST_LENGTH],
    const unsigned char *const m_1,
    const unsigned char *const m_2,
    const unsigned char *const m_3,
    const unsigned char *const m_4,
    const uint64_t mlen) {
	PAR_CSPRNG_STATE_T states;
	par_xof_input(par_level, &states, m_1, m_2, m_3, m_4, mlen);
	par_xof_output(par_level, &states, digest_1, digest_2, digest_3, digest_4, HASH_DIGEST_LENGTH);
	par_xof_release(par_level, &states);
}

/********************** CSPRNG Sampling functions helpers ********************/

static inline
FQ_ELEM fq_star_rnd_state(CSPRNG_STATE_T *const csprng_state) {
	const FQ_ELEM mask = ( (FQ_ELEM) 1 << BITS_TO_REPRESENT(Q - 2) ) - 1;
	FQ_ELEM rnd_value;
	do {
		csprng_randombytes((unsigned char *) &rnd_value,
		                   sizeof(FQ_ELEM),
		                   csprng_state);
		rnd_value = mask & rnd_value;
	} while (rnd_value > Q - 2);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(csprng_state);

	return rnd_value + 1;
} /* end fq_star_rnd_state */

/********************* Endianness detection and handling **********************/

#define BIG_ENDIAN_SYSTEM 0 // assume little-endian by default

/* Check if we are on a big-endian system:
 * the __BYTE_ORDER__ macro is defined by the compiler
 * recent versions of GCC and Clang define it
 * other methods to detect endianness might be needed when using different compilers */
#if defined(__BYTE_ORDER__)&&(__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#undef BIG_ENDIAN_SYSTEM
#define BIG_ENDIAN_SYSTEM 1
#endif

static inline uint64_t to_little_endian64(uint64_t x) {
	/* When compiling on a big-endian system, swap the bytes */
#if BIG_ENDIAN_SYSTEM
	return __builtin_bswap64(x);
#else
	return x;
#endif
}

static inline uint32_t to_little_endian32(uint32_t x) {
	/* When compiling on a big-endian system, swap the bytes */
#if BIG_ENDIAN_SYSTEM
	return __builtin_bswap32(x);
#else
	return x;
#endif
}

static inline uint16_t to_little_endian16(uint16_t x) {
	/* When compiling on a big-endian system, swap the bytes */
#if BIG_ENDIAN_SYSTEM
	return __builtin_bswap16(x);
#else
	return x;
#endif
}

/***************** Specialized CSPRNGs for non binary domains *****************/

/* CSPRNG sampling fixed weight strings */
void PQCLEAN_CROSSRSDP128FAST_CLEAN_expand_digest_to_fixed_weight(uint8_t fixed_weight_string[T],
        const uint8_t digest[HASH_DIGEST_LENGTH]);

#define BITS_FOR_Q BITS_TO_REPRESENT(Q-1)
#define BITS_FOR_Z BITS_TO_REPRESENT(Z-1)

/* PQClean-edit: avoid VLA */
#define CORRECTION_FQ_VEC ((BITS_N_ZQ_CT_RNG - N*BITS_FOR_Q)* (BITS_FOR_Q-1))
#define BUFSIZE_FQ_VEC (ROUND_UP(BITS_N_ZQ_CT_RNG+CORRECTION_FQ_VEC,8)/8)

static inline
void CSPRNG_fq_vec(FQ_ELEM res[N],
                   CSPRNG_STATE_T *const csprng_state) {
	const FQ_ELEM mask = ( (FQ_ELEM) 1 << BITS_FOR_Q) - 1;
	/* NOTE: Current bit cost estimation technique underestimates the failures
	 * whenever they appear in a run; an upper bound on the bit-cost is considering
	 * the failures to be discarding the entire value, instead of a single bit.
	 * The following correction factor takes this into account */

	/* PQClean-edit: avoid VLA */
	//uint32_t correction_bit_len =  (BITS_N_ZQ_CT_RNG - N*BITS_FOR_Q)* (BITS_FOR_Q-1);
	//uint32_t CSPRNG_buffer_size = ROUND_UP(BITS_N_ZQ_CT_RNG+correction_bit_len,8)/8;
	//uint8_t CSPRNG_buffer[ROUND_UP(CSPRNG_buffer_size,4)];
	uint8_t CSPRNG_buffer[BUFSIZE_FQ_VEC];

	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right, shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, BUFSIZE_FQ_VEC, csprng_state);
	int placed = 0;
	uint64_t sub_buffer = to_little_endian64(*(uint64_t *)CSPRNG_buffer);
	int bits_in_sub_buf = 64;
	/* position of the next fresh byte in CSPRNG_buffer*/
	int pos_in_buf = 8;
	while (placed < N) {
		if (bits_in_sub_buf <= 32) {
			/* get 32 fresh bits from main buffer with a single load */
			uint32_t refresh_buf = to_little_endian32(*(uint32_t *) (CSPRNG_buffer + pos_in_buf));
			pos_in_buf += 4;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 32;
		}
		res[placed] = sub_buffer & mask;
		if (res[placed] < Q) {
			placed++;
			sub_buffer = sub_buffer >> BITS_FOR_Q;
			bits_in_sub_buf -= BITS_FOR_Q;

		} else {
			sub_buffer = sub_buffer >> 1;
			bits_in_sub_buf -= 1;
		}
	}
}

#define BITS_FOR_Q_M_ONE BITS_TO_REPRESENT(Q-2)

/* PQClean-edit: avoid VLA */
#define CORRECTION_FQ_VEC_BETA ((BITS_BETA_ZQSTAR_CT_RNG - T*BITS_FOR_Q_M_ONE) * (BITS_FOR_Q_M_ONE-1))
#define BUFSIZE_FQ_VEC_BETA (ROUND_UP(BITS_BETA_ZQSTAR_CT_RNG+CORRECTION_FQ_VEC_BETA,8)/8)

static inline
void CSPRNG_fq_vec_beta(FQ_ELEM res[T],
                        CSPRNG_STATE_T *const csprng_state) {
	const FQ_ELEM mask = ( (FQ_ELEM) 1 << BITS_FOR_Q_M_ONE) - 1;

	/* PQClean-edit: avoid VLA */
	//uint32_t correction_bit_len =  (BITS_BETA_ZQSTAR_CT_RNG - T*BITS_FOR_Q_M_ONE) * (BITS_FOR_Q_M_ONE-1);
	//uint32_t CSPRNG_buffer_size = ROUND_UP(BITS_BETA_ZQSTAR_CT_RNG+correction_bit_len,8)/8;
	//uint8_t CSPRNG_buffer[ROUND_UP(CSPRNG_buffer_size,4)];
	uint8_t CSPRNG_buffer[BUFSIZE_FQ_VEC_BETA];

	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right , shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, BUFSIZE_FQ_VEC_BETA, csprng_state);
	int placed = 0;
	uint64_t sub_buffer = to_little_endian64(*(uint64_t *)CSPRNG_buffer);
	int bits_in_sub_buf = 64;
	/* position of the next fresh byte in CSPRNG_buffer*/
	int pos_in_buf = 8;
	while (placed < T) {
		if (bits_in_sub_buf <= 32) {
			/* get 32 fresh bits from main buffer with a single load */
			uint32_t refresh_buf = to_little_endian32(*(uint32_t *) (CSPRNG_buffer + pos_in_buf));
			pos_in_buf += 4;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 32;
		}
		/* draw from 0 ... Q-2, then add 1*/
		res[placed] = (sub_buffer & mask) + 1;
		if (res[placed] < Q) {
			placed++;
			sub_buffer = sub_buffer >> BITS_FOR_Q_M_ONE;
			bits_in_sub_buf -= BITS_FOR_Q_M_ONE;
		} else {
			sub_buffer = sub_buffer >> 1;
			bits_in_sub_buf -= 1;
		}
	}
}

/* PQClean-edit: avoid VLA */
#define CORRECTION_FQ_MAT ((BITS_V_CT_RNG - K*(N-K)*BITS_FOR_Q) * (BITS_FOR_Q-1))
#define BUFSIZE_FQ_MAT (ROUND_UP(BITS_V_CT_RNG+CORRECTION_FQ_MAT,8)/8)

static inline
void CSPRNG_fq_mat(FQ_ELEM res[K][N - K],
                   CSPRNG_STATE_T *const csprng_state) {
	const FQ_ELEM mask = ( (FQ_ELEM) 1 << BITS_FOR_Q) - 1;

	/* PQClean-edit: avoid VLA */
	//uint32_t correction_bit_len =  (BITS_V_CT_RNG - K*(N-K)*BITS_FOR_Q) * (BITS_FOR_Q-1);
	//uint32_t CSPRNG_buffer_size = ROUND_UP(BITS_V_CT_RNG+correction_bit_len,8)/8;
	//uint8_t CSPRNG_buffer[ROUND_UP(CSPRNG_buffer_size,4)];
	uint8_t CSPRNG_buffer[BUFSIZE_FQ_MAT];

	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right , shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, BUFSIZE_FQ_MAT, csprng_state);
	int placed = 0;
	uint64_t sub_buffer = to_little_endian64(*(uint64_t *)CSPRNG_buffer);
	int bits_in_sub_buf = 64;
	/* position of the next fresh byte in CSPRNG_buffer*/
	int pos_in_buf = 8;

	while (placed < K * (N - K)) {
		if (bits_in_sub_buf <= 32) {
			/* get 32 fresh bits from main buffer with a single load */
			uint32_t refresh_buf = to_little_endian32(*(uint32_t *) (CSPRNG_buffer + pos_in_buf));
			pos_in_buf += 4;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 32;
		}
		*( (FQ_ELEM *)res + placed) = sub_buffer & mask;
		if (*( (FQ_ELEM *)res + placed) < Q) {
			placed++;
			sub_buffer = sub_buffer >> BITS_FOR_Q;
			bits_in_sub_buf -= BITS_FOR_Q;

		} else {
			sub_buffer = sub_buffer >> 1;
			bits_in_sub_buf -= 1;
		}
	}
}

/* PQClean-edit: avoid VLA */
#define CORRECTION_ZZ_VEC ((BITS_N_ZZ_CT_RNG - N*BITS_FOR_Z) * (BITS_FOR_Z-1))
#define BUFSIZE_ZZ_VEC ROUND_UP(BITS_N_ZZ_CT_RNG+CORRECTION_ZZ_VEC,8)/8

static inline
void CSPRNG_zz_vec(FZ_ELEM res[N],
                   CSPRNG_STATE_T *const csprng_state) {
	const FZ_ELEM mask = ( (FZ_ELEM) 1 << BITS_FOR_Z) - 1;

	/* PQClean-edit: avoid VLA */
	//uint32_t correction_bit_len =  (BITS_N_ZZ_CT_RNG - N*BITS_FOR_Z) * (BITS_FOR_Z-1);
	//uint32_t CSPRNG_buffer_size = ROUND_UP(BITS_N_ZZ_CT_RNG+correction_bit_len,8)/8;
	//uint8_t CSPRNG_buffer[ROUND_UP(CSPRNG_buffer_size,4)];
	uint8_t CSPRNG_buffer[BUFSIZE_ZZ_VEC];

	/* To facilitate hardware implementations, the uint64_t
	 * sub-buffer is consumed starting from the least significant byte
	 * i.e., from the first being output by SHAKE. Bits in the byte are
	 * discarded shifting them out to the right , shifting fresh ones
	 * in from the left end */
	csprng_randombytes(CSPRNG_buffer, BUFSIZE_ZZ_VEC, csprng_state);
	int placed = 0;
	uint64_t sub_buffer = to_little_endian64(*(uint64_t *)CSPRNG_buffer);
	int bits_in_sub_buf = 64;
	/* position of the next fresh byte in CSPRNG_buffer*/
	int pos_in_buf = 8;
	while (placed < N) {
		if (bits_in_sub_buf <= 32) {
			/* get 32 fresh bits from main buffer with a single load */
			uint32_t refresh_buf = to_little_endian32(*(uint32_t *) (CSPRNG_buffer + pos_in_buf));
			pos_in_buf += 4;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 32;
		}
		/* get */
		res[placed] = sub_buffer & mask;
		if (res[placed] < Z) {
			placed++;
			sub_buffer = sub_buffer >> BITS_FOR_Z;
			bits_in_sub_buf -= BITS_FOR_Z;

		} else {
			sub_buffer = sub_buffer >> 1;
			bits_in_sub_buf -= 1;
		}
	}
}
