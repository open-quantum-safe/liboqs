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

#include "csprng_hash.h"
#include "parameters.h"

void PQCLEAN_CROSSRSDP192FAST_CLEAN_pseed(unsigned char seed[SEED_LENGTH_BYTES]);
void PQCLEAN_CROSSRSDP192FAST_CLEAN_ptree(unsigned char seed_tree[NUM_NODES_SEED_TREE * SEED_LENGTH_BYTES]);

int PQCLEAN_CROSSRSDP192FAST_CLEAN_compute_round_seeds(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
        const unsigned char root_seed[SEED_LENGTH_BYTES],
        const unsigned char salt[SALT_LENGTH_BYTES]);

int PQCLEAN_CROSSRSDP192FAST_CLEAN_publish_round_seeds(unsigned char *seed_storage,
        const unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
        const unsigned char indices_to_publish[T]);

int PQCLEAN_CROSSRSDP192FAST_CLEAN_regenerate_round_seeds(unsigned char rounds_seeds[T * SEED_LENGTH_BYTES],
        const unsigned char indices_to_publish[T],
        const unsigned char *seed_storage);
