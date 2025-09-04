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

#include "csprng_hash.h"
#include "namespace.h"
#include "parameters.h"


/******************************************************************************/
void seed_leaves(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
                 unsigned char seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES]);

/******************************************************************************/
void gen_seed_tree(unsigned char seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
                   const unsigned char root_seed[SEED_LENGTH_BYTES],
                   const unsigned char salt[SALT_LENGTH_BYTES]);

/******************************************************************************/
/* returns the number of seeds which have been published */
int seed_path(unsigned char *seed_storage,
              const unsigned char
              seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
              // binary array denoting if node has to be released (cell == 0) or not
              const unsigned char indices_to_publish[T]);

/******************************************************************************/
/* returns 1 if padding was correct 0 if there was a mistake */
uint8_t rebuild_tree(unsigned char
                     seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES],
                     const unsigned char indices_to_publish[T],
                     const unsigned char *stored_seeds,
                     const unsigned char salt[SALT_LENGTH_BYTES]);   // input

