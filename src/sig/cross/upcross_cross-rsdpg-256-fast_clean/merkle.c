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
#include <string.h>

#include "csprng_hash.h"
#include "merkle_tree.h"
#include "parameters.h"


#define TO_PUBLISH 1
#define NOT_TO_PUBLISH 0

void tree_root(uint8_t root[HASH_DIGEST_LENGTH],
               uint8_t leaves[T][HASH_DIGEST_LENGTH]) {

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

	uint8_t hash_input[4 * HASH_DIGEST_LENGTH];

	int offset = 0;
	for (int i = 0; i < 4; i++) {
		hash(&hash_input[i * HASH_DIGEST_LENGTH], leaves[(T / 4)*i + offset], (T / 4 + remainders[i])*HASH_DIGEST_LENGTH, HASH_DOMAIN_SEP_CONST);
		offset += remainders[i];
	}

	hash(root, hash_input, sizeof(hash_input), HASH_DOMAIN_SEP_CONST);
}

uint16_t tree_proof(uint8_t mtp[W * HASH_DIGEST_LENGTH],
                    uint8_t leaves[T][HASH_DIGEST_LENGTH],
                    const uint8_t leaves_to_reveal[T]) {
	uint16_t published = 0;
	for (int i = 0; i < T; i++) {
		if (leaves_to_reveal[i] == TO_PUBLISH) {
			memcpy(&mtp[HASH_DIGEST_LENGTH * published],
			       &leaves[i],
			       HASH_DIGEST_LENGTH);
			published++;
		}
	}
	return published;
}

uint8_t recompute_root(uint8_t root[HASH_DIGEST_LENGTH],
                       uint8_t recomputed_leaves[T][HASH_DIGEST_LENGTH],
                       const uint8_t mtp[W * HASH_DIGEST_LENGTH],
                       const uint8_t leaves_to_reveal[T]) {
	uint16_t published = 0;
	for (int i = 0; i < T; i++) {
		if (leaves_to_reveal[i] == TO_PUBLISH) {
			memcpy(&recomputed_leaves[i],
			       &mtp[HASH_DIGEST_LENGTH * published],
			       HASH_DIGEST_LENGTH);
			published++;
		}
	}
	tree_root(root, recomputed_leaves);
	return 1;
}

