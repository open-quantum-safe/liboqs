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

#include <string.h>

#include "csprng_hash.h"

#define  POSITION_MASK (( (uint16_t)1 << BITS_TO_REPRESENT(T-1))-1)

/* Fisher-Yates shuffle obtaining the entire required randomness in a single
 * call */
void PQCLEAN_CROSSRSDP256BALANCED_CLEAN_expand_digest_to_fixed_weight(uint8_t fixed_weight_string[T],
        const uint8_t digest[HASH_DIGEST_LENGTH]) {
	CSPRNG_STATE_T csprng_state;
	initialize_csprng(&csprng_state,
	                  (const unsigned char *) digest,
	                  HASH_DIGEST_LENGTH);
	uint8_t CSPRNG_buffer[ROUND_UP(BITS_CWSTR_RNG, 8) / 8];
	csprng_randombytes(CSPRNG_buffer, ROUND_UP(BITS_CWSTR_RNG, 8) / 8, &csprng_state);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&csprng_state);

	/* initialize CW string */
	memset(fixed_weight_string, 1, W);
	memset(fixed_weight_string + W, 0, T - W);

	uint64_t sub_buffer = to_little_endian64(*(uint64_t *)CSPRNG_buffer);
	int bits_in_sub_buf = 64;
	int pos_in_buf = 8;

	int curr = 0;
	while (curr < T) {
		/* refill randomness buffer if needed */
		if (bits_in_sub_buf <= 32) {
			/* get 32 fresh bits from main buffer with a single load */
			uint32_t refresh_buf = to_little_endian32(*(uint32_t *) (CSPRNG_buffer + pos_in_buf));
			pos_in_buf += 4;
			sub_buffer |=  ((uint64_t) refresh_buf) << bits_in_sub_buf;
			bits_in_sub_buf += 32;
		}
		/*we need to draw a number in 0... T-1-curr */
		int bits_for_pos = BITS_TO_REPRESENT(T - 1 - curr);
		uint64_t pos_mask = ( (uint64_t) 1 <<  bits_for_pos) - 1;
		uint16_t candidate_pos = (sub_buffer & pos_mask);
		if (candidate_pos < T - curr) {
			int dest = curr + candidate_pos;
			/* the position is admissible, swap */
			uint8_t tmp = fixed_weight_string[curr];
			fixed_weight_string[curr] = fixed_weight_string[dest];
			fixed_weight_string[dest] = tmp;
			curr++;
			sub_buffer = sub_buffer >> bits_for_pos;
			bits_in_sub_buf -= bits_for_pos;
		} else {
			sub_buffer = sub_buffer >> 1;
			bits_in_sub_buf -= 1;
		}
	}
} /* PQCLEAN_CROSSRSDP256BALANCED_CLEAN_expand_digest_to_fixed_weight */
