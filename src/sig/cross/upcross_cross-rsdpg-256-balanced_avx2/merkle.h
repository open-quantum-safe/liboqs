/*
 * Reference ISO-C11 Implementation of CROSS.
 *
 * @version 1.1 (May 2023)
 *
 * @author: Patrick Karl <patrick.karl@tum.de>
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
 */

#pragma once

#include "csprng_hash.h"
#include "parameters.h"

/***********************************************************************************************/
void PQCLEAN_CROSSRSDPG256BALANCED_AVX2_generate_merkle_tree(unsigned char merkle_tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH],
        unsigned char commitments[T][HASH_DIGEST_LENGTH]);

/***********************************************************************************************/
void PQCLEAN_CROSSRSDPG256BALANCED_AVX2_generate_merkle_proof(uint16_t merkle_proof_indices[TREE_NODES_TO_STORE],
        uint16_t *merkle_proof_len,
        const unsigned char challenge[T]);

/***********************************************************************************************/
void PQCLEAN_CROSSRSDPG256BALANCED_AVX2_rebuild_merkle_tree(unsigned char merkle_tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH],
        const unsigned char merkle_proof[TREE_NODES_TO_STORE * HASH_DIGEST_LENGTH],
        unsigned char commitments[T][HASH_DIGEST_LENGTH],
        const unsigned char challenge[T]);
