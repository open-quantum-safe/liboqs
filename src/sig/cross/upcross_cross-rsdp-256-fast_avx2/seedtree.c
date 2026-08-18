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

#include <stdint.h>
#include <string.h> // memcpy(...), memset(...)

#include "csprng_hash.h"
#include "seedtree.h"

#define LEFT_CHILD(i) (2*(i)+1)
#define RIGHT_CHILD(i) (2*(i)+2)
#define PARENT(i) ( ((i)%2) ? (((i)-1)/2) : (((i)-2)/2) )
#define SIBLING(i) ( ((i)%2) ? (i)+1 : (i)-1 )

/* Seed tree implementation. The binary seed tree is linearized into an array
 * from root to leaves, and from left to right.
 */

#define TO_PUBLISH 1
#define NOT_TO_PUBLISH 0

int seed_leaves(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
                const unsigned char root_seed[SEED_LENGTH_BYTES],
                const unsigned char salt[SALT_LENGTH_BYTES]) {

	CSPRNG_STATE_T single_csprng_state;
	PAR_CSPRNG_STATE_T par_csprng_state;

	/* CSPRNG input: seed | salt | domain separation counter (0 to 4) */
	unsigned char single_csprng_input[CSPRNG_INPUT_LENGTH];
	unsigned char par_csprng_input[4][CSPRNG_INPUT_LENGTH];
	unsigned char par_csprng_output[4][(T / 4 + 1)*SEED_LENGTH_BYTES];

	/* copy the root seed and the salt */
	memcpy(single_csprng_input, root_seed, SEED_LENGTH_BYTES);
	memcpy(single_csprng_input + SEED_LENGTH_BYTES, salt, SALT_LENGTH_BYTES);

	/* call the CSPRNG once to generate 4 intermediate seeds */
	unsigned char quad_seed[4 * SEED_LENGTH_BYTES];
	csprng_initialize(&single_csprng_state, single_csprng_input, CSPRNG_INPUT_LENGTH, 0);
	csprng_randombytes(quad_seed, 4 * SEED_LENGTH_BYTES, &single_csprng_state);
	/* PQClean-edit: CSPRNG release context */
	csprng_release(&single_csprng_state);

	memset(par_csprng_input, 0, 4 * CSPRNG_INPUT_LENGTH);

	/* copy the salt */
	for (int i = 0; i < 4; i++) {
		memcpy(par_csprng_input[i] + SEED_LENGTH_BYTES, salt, SALT_LENGTH_BYTES);
	}

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

	/* copy the 4 intermediate seeds */
	for (int i = 0; i < 4; i++) {
		memcpy(par_csprng_input[i], &quad_seed[i * SEED_LENGTH_BYTES], SEED_LENGTH_BYTES);
	}

	/* make 4 (parallel) calls to the CSPRNG */
	csprng_initialize_par(
	    4,
	    &par_csprng_state,
	    par_csprng_input[0],
	    par_csprng_input[1],
	    par_csprng_input[2],
	    par_csprng_input[3],
	    CSPRNG_INPUT_LENGTH,
	    CSPRNG_DOMAIN_SEP_CONST + 1,
	    CSPRNG_DOMAIN_SEP_CONST + 2,
	    CSPRNG_DOMAIN_SEP_CONST + 3,
	    CSPRNG_DOMAIN_SEP_CONST + 4);
	csprng_randombytes_par(
	    4,
	    &par_csprng_state,
	    par_csprng_output[0],
	    par_csprng_output[1],
	    par_csprng_output[2],
	    par_csprng_output[3],
	    (T / 4 + 1)*SEED_LENGTH_BYTES);
	/* PQClean-edit: CSPRNG release context */
	csprng_release_par(4, &par_csprng_state);

	/* store the CSPRNG outputs as round seeds */
	int offset = 0;
	for (int i = 0; i < 4; i++) {
		memcpy(&rounds_seeds[((T / 4)*i + offset)*SEED_LENGTH_BYTES],
		       par_csprng_output[i],
		       (T / 4 + remainders[i])*SEED_LENGTH_BYTES);
		offset += remainders[i];
	}
	return T;
}

int seed_path(unsigned char *seed_storage,
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
uint8_t rebuild_leaves(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
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
	return 1;
}
