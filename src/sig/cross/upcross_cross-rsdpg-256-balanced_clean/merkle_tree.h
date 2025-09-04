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


/* Stub of the interface to Merkle tree root computer from all leaves */
void tree_root(uint8_t root[HASH_DIGEST_LENGTH],
               uint8_t tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH],
               /* input, although mutable in caller, having as const is non
                * tolerated in strict ISO C */
               uint8_t leaves[T][HASH_DIGEST_LENGTH]);

/* Stub interface to the function computing the Merkle tree proof, storing it
 * in the signature. Returns the number of digests in the merkle tree proof */
uint16_t tree_proof(uint8_t mtp[HASH_DIGEST_LENGTH * TREE_NODES_TO_STORE],
                    const uint8_t tree[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH],
                    const uint8_t leaves_to_reveal[T]);

/* stub of the interface to Merkle tree recomputation given the proof and
 * the computed leaves */
uint8_t recompute_root(uint8_t root[HASH_DIGEST_LENGTH],
                       uint8_t recomputed_leaves[T][HASH_DIGEST_LENGTH],
                       const uint8_t mtp[HASH_DIGEST_LENGTH * TREE_NODES_TO_STORE],
                       const uint8_t leaves_to_reveal[T]);
