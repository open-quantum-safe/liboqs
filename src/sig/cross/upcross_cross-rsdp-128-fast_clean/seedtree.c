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

#include "seedtree.h"
#include <stdint.h>
#include <string.h> // memcpy(...), memset(...)

#define LEFT_CHILD(i) (2*(i)+1)
#define RIGHT_CHILD(i) (2*(i)+2)
#define PARENT(i) (((i)-1)/2)

/* Seed tree implementation. The binary seed tree is linearized into an array
 * from root to leaves, and from left to right. The nodes are numbered picking
 * the indexes from the corresponding full tree, having 2**LOG2(T) leaves */
#define DIV_BY_TWO_CEIL(i)  ((i)/2 + (i) % 2)

#define TO_PUBLISH 1
#define NOT_TO_PUBLISH 0

/* PQClean-edit: avoid VLA */
#define CSPRNG_INPUT_LEN (SALT_LENGTH_BYTES + SEED_LENGTH_BYTES + SIZEOF_UINT16)
//const uint32_t csprng_input_len = SALT_LENGTH_BYTES + SEED_LENGTH_BYTES + sizeof(uint16_t);

int PQCLEAN_CROSSRSDP128FAST_CLEAN_compute_round_seeds(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
        const unsigned char root_seed[SEED_LENGTH_BYTES],
        const unsigned char salt[SALT_LENGTH_BYTES]) {

	unsigned char csprng_input[CSPRNG_INPUT_LEN];
	memcpy(csprng_input, root_seed, SEED_LENGTH_BYTES);
	memcpy(csprng_input + SEED_LENGTH_BYTES, salt, SALT_LENGTH_BYTES);
	/* set counter for domain separation to 1 */
	csprng_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES] = 0;
	csprng_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES + 1] = 1;

	unsigned char quad_seed[4 * SEED_LENGTH_BYTES];
	CSPRNG_STATE_T csprng_states[4];
	initialize_csprng(&csprng_states[0], csprng_input, CSPRNG_INPUT_LEN);
	csprng_randombytes(quad_seed, 4 * SEED_LENGTH_BYTES, &csprng_states[0]);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&csprng_states[0]);

	int remainders[4] = {0};
	if (T % 4 > 0) {
		remainders[0] = 1;
	}
	if (T % 4 > 1) {
		remainders[1] = 1;
	}
	if (T % 4 > 2) {
		remainders[2] = 1;
	}

	int offset = 0;
	for (int i = 0; i < 4; i++) {
		memcpy(csprng_input, &quad_seed[i * SEED_LENGTH_BYTES], SEED_LENGTH_BYTES);
		csprng_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES + 1] += 1;
		initialize_csprng(&csprng_states[i], csprng_input, CSPRNG_INPUT_LEN);

		csprng_randombytes(&rounds_seeds[((T / 4)*i + offset)*SEED_LENGTH_BYTES],
		                   (T / 4 + remainders[i])*SEED_LENGTH_BYTES,
		                   &csprng_states[i]);

		/* PQClean-edit: CSPRNG release context */
		csprng_release(&csprng_states[i]);

		offset += remainders[i];
	}
	return T;
}

int PQCLEAN_CROSSRSDP128FAST_CLEAN_publish_round_seeds(unsigned char *seed_storage,
        const unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
        const unsigned char indices_to_publish[T]) {
	int published = 0;
	for (int i = 0; i < T; i++) {
		if (indices_to_publish[i] == TO_PUBLISH) {
			memcpy(&seed_storage[SEED_LENGTH_BYTES * published],
			       &rounds_seeds[i * SEED_LENGTH_BYTES],
			       SEED_LENGTH_BYTES);
			published++;
		}
	}
	return published;
}

/* simply picks seeds out of the storage and places them in the in-memory array */
int PQCLEAN_CROSSRSDP128FAST_CLEAN_regenerate_round_seeds(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
        const unsigned char indices_to_publish[T],
        const unsigned char *seed_storage) {
	int published = 0;
	for (int i = 0; i < T; i++) {
		if (indices_to_publish[i] == TO_PUBLISH) {
			memcpy(&rounds_seeds[i * SEED_LENGTH_BYTES],
			       &seed_storage[SEED_LENGTH_BYTES * published],
			       SEED_LENGTH_BYTES);
			published++;
		}
	}
	return published;
}
