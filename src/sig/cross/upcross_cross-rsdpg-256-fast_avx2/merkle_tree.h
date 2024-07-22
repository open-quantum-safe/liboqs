/**
 *
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (March 2023)
 *
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author: Patrick Karl <patrick.karl@tum.de>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 *
 * Authors listed in lexicographic order.
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

#include "csprng_hash.h"

#define TO_PUBLISH 1
#define NOT_TO_PUBLISH 0

void PQCLEAN_CROSSRSDPG256FAST_AVX2_merkle_tree_root_compute(uint8_t root[HASH_DIGEST_LENGTH],
        uint8_t leaves[T][HASH_DIGEST_LENGTH]);
uint16_t PQCLEAN_CROSSRSDPG256FAST_AVX2_merkle_tree_proof_compute(uint8_t mtp[W * HASH_DIGEST_LENGTH],
        uint8_t leaves[T][HASH_DIGEST_LENGTH],
        const uint8_t leaves_to_reveal[T]);
void PQCLEAN_CROSSRSDPG256FAST_AVX2_merkle_tree_root_recompute(uint8_t root[HASH_DIGEST_LENGTH],
        uint8_t recomputed_leaves[T][HASH_DIGEST_LENGTH],
        const uint8_t mtp[W * HASH_DIGEST_LENGTH],
        const uint8_t leaves_to_reveal[T]);

void PQCLEAN_CROSSRSDPG256FAST_AVX2_merkle_tree_root_compute(uint8_t root[HASH_DIGEST_LENGTH],
        uint8_t leaves[T][HASH_DIGEST_LENGTH]) {
	uint8_t quad_hash[4][HASH_DIGEST_LENGTH];
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
		hash(quad_hash[i],
		     leaves[(T / 4)*i + offset],
		     (T / 4 + remainders[i])*HASH_DIGEST_LENGTH);
		offset += remainders[i];
	}
	hash(root, (const unsigned char *)quad_hash, 4 * HASH_DIGEST_LENGTH);
}

uint16_t PQCLEAN_CROSSRSDPG256FAST_AVX2_merkle_tree_proof_compute(uint8_t mtp[W * HASH_DIGEST_LENGTH],
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

void PQCLEAN_CROSSRSDPG256FAST_AVX2_merkle_tree_root_recompute(uint8_t root[HASH_DIGEST_LENGTH],
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
	PQCLEAN_CROSSRSDPG256FAST_AVX2_merkle_tree_root_compute(root, recomputed_leaves);
}
