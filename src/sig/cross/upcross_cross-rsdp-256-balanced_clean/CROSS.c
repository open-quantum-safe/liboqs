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
#include <assert.h>
#include <stdalign.h>

#include "CROSS.h"
#include "csprng_hash.h"
#include "fq_arith.h"
#include "merkle_tree.h"
#include "pack_unpack.h"
#include "randombytes.h"
#include "seedtree.h"

static
void expand_public_seed(FQ_ELEM V_tr[K][N - K],
                        const uint8_t seed_pub[KEYPAIR_SEED_LENGTH_BYTES]) {
	CSPRNG_STATE_T CSPRNG_state_mat;
	initialize_csprng(&CSPRNG_state_mat, seed_pub, KEYPAIR_SEED_LENGTH_BYTES);
	CSPRNG_fq_mat(V_tr, &CSPRNG_state_mat);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&CSPRNG_state_mat);
}

static
void expand_private_seed(FZ_ELEM eta[N],
                         FQ_ELEM V_tr[K][N - K],
                         const uint8_t seed[KEYPAIR_SEED_LENGTH_BYTES]) {
	uint8_t seede_seed_pub[2][KEYPAIR_SEED_LENGTH_BYTES];
	CSPRNG_STATE_T CSPRNG_state;
	initialize_csprng(&CSPRNG_state, seed, KEYPAIR_SEED_LENGTH_BYTES);
	csprng_randombytes((uint8_t *)seede_seed_pub,
	                   2 * KEYPAIR_SEED_LENGTH_BYTES,
	                   &CSPRNG_state);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&CSPRNG_state);

	expand_public_seed(V_tr, seede_seed_pub[1]);

	CSPRNG_STATE_T CSPRNG_state_eta;
	initialize_csprng(&CSPRNG_state_eta, seede_seed_pub[0], KEYPAIR_SEED_LENGTH_BYTES);
	CSPRNG_zz_vec(eta, &CSPRNG_state_eta);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&CSPRNG_state_eta);
}

void PQCLEAN_CROSSRSDP256BALANCED_CLEAN_CROSS_keygen(prikey_t *SK,
        pubkey_t *PK) {
	/* generation of random material for public and private key */
	randombytes(SK->seed, KEYPAIR_SEED_LENGTH_BYTES);
	uint8_t seede_seed_pub[2][KEYPAIR_SEED_LENGTH_BYTES];

	CSPRNG_STATE_T CSPRNG_state;
	initialize_csprng(&CSPRNG_state, SK->seed, KEYPAIR_SEED_LENGTH_BYTES);
	csprng_randombytes((uint8_t *)seede_seed_pub,
	                   2 * KEYPAIR_SEED_LENGTH_BYTES,
	                   &CSPRNG_state);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&CSPRNG_state);

	memcpy(PK->seed_pub, seede_seed_pub[1], KEYPAIR_SEED_LENGTH_BYTES);

	/* expansion of matrix/matrices */
	FQ_ELEM V_tr[K][N - K];
	expand_public_seed(V_tr, PK->seed_pub);

	/* expansion of secret key material */
	FZ_ELEM eta[N];
	CSPRNG_STATE_T CSPRNG_state_eta;
	initialize_csprng(&CSPRNG_state_eta, seede_seed_pub[0], KEYPAIR_SEED_LENGTH_BYTES);

	CSPRNG_zz_vec(eta, &CSPRNG_state_eta);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&CSPRNG_state_eta);

	/* compute public syndrome */
	FQ_ELEM pub_syn[N - K];
	restr_vec_by_fq_matrix(pub_syn, eta, V_tr);
	fq_dz_norm_synd(pub_syn);
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fq_syn(PK->s, pub_syn);
}

/* sign cannot fail */
void PQCLEAN_CROSSRSDP256BALANCED_CLEAN_CROSS_sign(const prikey_t *const SK,
        const char *const m,
        const size_t mlen,
        CROSS_sig_t *const sig) {
	/* Wipe any residual information in the sig structure allocated by the
	 * caller */
	memset(sig, 0, sizeof(CROSS_sig_t));
	/* Key material expansion */
	FQ_ELEM V_tr[K][N - K];
	FZ_ELEM eta[N];
	expand_private_seed(eta, V_tr, SK->seed);

	uint8_t root_seed[SEED_LENGTH_BYTES];
	randombytes(root_seed, SEED_LENGTH_BYTES);
	randombytes(sig->salt, SALT_LENGTH_BYTES);

	uint8_t seed_tree[SEED_LENGTH_BYTES * NUM_NODES_SEED_TREE] = {0};
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_generate_seed_tree_from_root(seed_tree, root_seed, sig->salt);
	uint8_t *rounds_seeds = seed_tree +
	                        SEED_LENGTH_BYTES * NUM_INNER_NODES_SEED_TREE;

	FZ_ELEM eta_tilde[T][N];
	FZ_ELEM sigma[T][N];
	FQ_ELEM u_tilde[T][N];
	FQ_ELEM s_tilde[N - K];

	uint8_t cmt_0_i_input[DENSELY_PACKED_FQ_SYN_SIZE +
	                      DENSELY_PACKED_FZ_VEC_SIZE +
	                      SALT_LENGTH_BYTES + sizeof(uint16_t)];
	const int offset_salt = DENSELY_PACKED_FQ_SYN_SIZE + DENSELY_PACKED_FZ_VEC_SIZE;
	const int offset_round_idx = offset_salt + SALT_LENGTH_BYTES;
	/* cmt_0_i_input is syndrome||sigma ||salt ; place salt at the end */
	memcpy(cmt_0_i_input + offset_salt, sig->salt, SALT_LENGTH_BYTES);

	uint8_t cmt_1_i_input[SEED_LENGTH_BYTES +
	                      SALT_LENGTH_BYTES + sizeof(uint16_t)];
	/* cmt_1_i_input is concat(seed,salt,round index) */
	memcpy(cmt_1_i_input + SEED_LENGTH_BYTES, sig->salt, SALT_LENGTH_BYTES);

	uint8_t cmt_0[T][HASH_DIGEST_LENGTH] = {0};
	uint8_t cmt_1[T][HASH_DIGEST_LENGTH] = {0};

	CSPRNG_STATE_T CSPRNG_state;
	for (uint16_t i = 0; i < T; i++) {
		/* CSPRNG is fed with concat(seed,salt,round index) represented
		 * as a 2 bytes little endian unsigned integer */
		uint8_t csprng_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES + sizeof(uint16_t)];
		memcpy(csprng_input, rounds_seeds + SEED_LENGTH_BYTES * i, SEED_LENGTH_BYTES);
		memcpy(csprng_input + SEED_LENGTH_BYTES, sig->salt, SALT_LENGTH_BYTES);
		/* i+c */
		uint16_t domain_sep_i = i + NUM_NODES_SEED_TREE;
		csprng_input[SALT_LENGTH_BYTES + SEED_LENGTH_BYTES] = (domain_sep_i >> 8) & 0xFF;
		csprng_input[SALT_LENGTH_BYTES + SEED_LENGTH_BYTES + 1] = domain_sep_i & 0xFF;

		/* expand seed[i] into seed_e and seed_u */
		initialize_csprng(&CSPRNG_state,
		                  csprng_input,
		                  SEED_LENGTH_BYTES + SALT_LENGTH_BYTES + sizeof(uint16_t));
		/* expand eta_tilde */
		CSPRNG_zz_vec(eta_tilde[i], &CSPRNG_state);
		restr_vec_sub(sigma[i], eta, eta_tilde[i]);

		FQ_ELEM v[N];
		convert_restr_vec_to_fq(v, sigma[i]);
		fz_dz_norm_sigma(sigma[i]);
		/* expand u_tilde */
		CSPRNG_fq_vec(u_tilde[i], &CSPRNG_state);

		/* PQClean-edit: CSPRNG release context */
		csprng_release(&CSPRNG_state);

		FQ_ELEM u[N];
		fq_vec_by_fq_vec_pointwise(u, v, u_tilde[i]);
		fq_vec_by_fq_matrix(s_tilde, u, V_tr);
		fq_dz_norm_synd(s_tilde);

		/* cmt_0_i_input contains s-tilde || sigma_i || salt */
		PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fq_syn(cmt_0_i_input, s_tilde);

		PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fz_vec(cmt_0_i_input + DENSELY_PACKED_FQ_SYN_SIZE, sigma[i]);
		/* Fixed endianness marshalling of round counter
		 * i+c+dsc */
		uint16_t domain_sep_idx_hash = domain_sep_i + HASH_CSPRNG_DOMAIN_SEP_CONST;
		cmt_0_i_input[offset_round_idx] = (domain_sep_idx_hash >> 8) & 0xFF;
		cmt_0_i_input[offset_round_idx + 1] = domain_sep_idx_hash & 0xFF;

		hash(cmt_0[i], cmt_0_i_input, sizeof(cmt_0_i_input));
		memcpy(cmt_1_i_input,
		       rounds_seeds + SEED_LENGTH_BYTES * i,
		       SEED_LENGTH_BYTES);

		cmt_1_i_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES] = (domain_sep_idx_hash >> 8) & 0xFF;
		cmt_1_i_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES + 1] = domain_sep_idx_hash & 0xFF;
		hash(cmt_1[i], cmt_1_i_input, sizeof(cmt_1_i_input));

	}

	/* vector containing d_0 and d_1 from spec */
	uint8_t commit_digests[2][HASH_DIGEST_LENGTH];
	uint8_t merkle_tree_0[NUM_NODES_MERKLE_TREE * HASH_DIGEST_LENGTH];
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_merkle_tree_root_compute(commit_digests[0], merkle_tree_0, cmt_0);
	hash(commit_digests[1], (unsigned char *)cmt_1, sizeof(cmt_1));
	hash(sig->digest_01,
	     (unsigned char *) commit_digests,
	     sizeof(commit_digests));

	/* first challenge extraction */
	uint8_t beta_buf[2 * HASH_DIGEST_LENGTH + SALT_LENGTH_BYTES];
	/* place d_m at the beginning of the input of the hash generating d_beta*/
	hash(beta_buf, (uint8_t *) m, mlen);
	memcpy(beta_buf + HASH_DIGEST_LENGTH, sig->digest_01, HASH_DIGEST_LENGTH);
	memcpy(beta_buf + 2 * HASH_DIGEST_LENGTH, sig->salt, SALT_LENGTH_BYTES);

	uint8_t d_beta[HASH_DIGEST_LENGTH];
	hash(d_beta, beta_buf, 2 * HASH_DIGEST_LENGTH + SALT_LENGTH_BYTES);

	FQ_ELEM beta[T];
	initialize_csprng(&CSPRNG_state, d_beta, HASH_DIGEST_LENGTH);
	CSPRNG_fq_vec_beta(beta, &CSPRNG_state);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&CSPRNG_state);

	/* Computation of the first round of responses */
	FQ_ELEM y[T][N];
	for (int i = 0; i < T; i++) {
		fq_vec_by_restr_vec_scaled(y[i],
		                           eta_tilde[i],
		                           beta[i],
		                           u_tilde[i]);
		fq_dz_norm(y[i]);
	}
	/* y vectors are packed before being hashed */
	uint8_t digest_b_buf[T * DENSELY_PACKED_FQ_VEC_SIZE + HASH_DIGEST_LENGTH];
	for (int x = 0; x < T; x++) {
		PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fq_vec(digest_b_buf + (x * DENSELY_PACKED_FQ_VEC_SIZE), y[x]);
	}
	/* Second challenge extraction */
	memcpy(digest_b_buf + T * DENSELY_PACKED_FQ_VEC_SIZE, d_beta, HASH_DIGEST_LENGTH);

	hash(sig->digest_b, digest_b_buf, sizeof(digest_b_buf));

	uint8_t fixed_weight_b[T] = {0};
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_expand_digest_to_fixed_weight(fixed_weight_b, sig->digest_b);

	/* Computation of the second round of responses */

	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_merkle_tree_proof_compute(sig->mtp, merkle_tree_0, fixed_weight_b);
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_publish_seeds(sig->stp, seed_tree, fixed_weight_b);

	int published_rsps = 0;
	for (int i = 0; i < T; i++) {
		if (fixed_weight_b[i] == 0) {
			/* PQClean-edit: remove assertion */
			//assert(published_rsps < T-W);
			PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fq_vec(sig->rsp_0[published_rsps].y, y[i]);
			PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fz_vec(sig->rsp_0[published_rsps].sigma, sigma[i]);
			memcpy(sig->rsp_1[published_rsps], cmt_1[i], HASH_DIGEST_LENGTH);
			published_rsps++;
		}
	}
}

/* PQClean-edit: avoid VLA */
#define CSPRNG_INPUT_LENGTH (SALT_LENGTH_BYTES+SEED_LENGTH_BYTES+SIZEOF_UINT16)
//const int csprng_input_length = SALT_LENGTH_BYTES+SEED_LENGTH_BYTES+sizeof(uint16_t);

/* verify returns 1 if signature is ok, 0 otherwise */
int PQCLEAN_CROSSRSDP256BALANCED_CLEAN_CROSS_verify(const pubkey_t *const PK,
        const char *const m,
        const size_t mlen,
        const CROSS_sig_t *const sig) {
	CSPRNG_STATE_T CSPRNG_state;

	FQ_ELEM V_tr[K][N - K];
	expand_public_seed(V_tr, PK->seed_pub);

	FQ_ELEM pub_syn[N - K];
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_unpack_fq_syn(pub_syn, PK->s);

	uint8_t beta_buf[2 * HASH_DIGEST_LENGTH + SALT_LENGTH_BYTES];
	hash(beta_buf, (uint8_t *) m, mlen);
	memcpy(beta_buf + HASH_DIGEST_LENGTH, sig->digest_01, HASH_DIGEST_LENGTH);
	memcpy(beta_buf + 2 * HASH_DIGEST_LENGTH, sig->salt, SALT_LENGTH_BYTES);

	uint8_t d_beta[HASH_DIGEST_LENGTH];
	hash(d_beta, beta_buf, sizeof(beta_buf));

	FQ_ELEM beta[T];
	initialize_csprng(&CSPRNG_state, d_beta, HASH_DIGEST_LENGTH);
	CSPRNG_fq_vec_beta(beta, &CSPRNG_state);

	/* PQClean-edit: CSPRNG release context */
	csprng_release(&CSPRNG_state);

	uint8_t fixed_weight_b[T] = {0};
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_expand_digest_to_fixed_weight(fixed_weight_b, sig->digest_b);

	uint8_t seed_tree[SEED_LENGTH_BYTES * NUM_NODES_SEED_TREE] = {0};
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_regenerate_round_seeds(seed_tree, fixed_weight_b, sig->stp, sig->salt);
	uint8_t *rounds_seeds = seed_tree +
	                        SEED_LENGTH_BYTES * NUM_INNER_NODES_SEED_TREE;

	uint8_t cmt_0_i_input[DENSELY_PACKED_FQ_SYN_SIZE +
	                      DENSELY_PACKED_FZ_VEC_SIZE +
	                      SALT_LENGTH_BYTES + sizeof(uint16_t)];
	const int offset_salt = DENSELY_PACKED_FQ_SYN_SIZE + DENSELY_PACKED_FZ_VEC_SIZE;
	const int offset_round_idx = offset_salt + SALT_LENGTH_BYTES;
	/* cmt_0_i_input is syndrome||sigma ||salt */
	memcpy(cmt_0_i_input + offset_salt, sig->salt, SALT_LENGTH_BYTES);

	/* cmt_1_i_input is concat(seed,salt,round index) */
	uint8_t cmt_1_i_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES + sizeof(uint16_t)];
	memcpy(cmt_1_i_input + SEED_LENGTH_BYTES, sig->salt, SALT_LENGTH_BYTES);

	uint8_t cmt_0[T][HASH_DIGEST_LENGTH] = {0};
	uint8_t cmt_1[T][HASH_DIGEST_LENGTH] = {0};

	FZ_ELEM eta_tilde[N];
	FQ_ELEM u_tilde[N];

	FQ_ELEM y_tilde[N] = {0};
	FQ_ELEM s_tilde[N - K] = {0};

	FQ_ELEM y[T][N];

	int used_rsps = 0;
	int is_signature_ok = 1;
	for (uint16_t i = 0; i < T; i++) {

		/* i+c */
		uint16_t domain_sep_i = i + NUM_NODES_SEED_TREE;
		/* i+c+dsc */
		uint16_t domain_sep_idx_hash = domain_sep_i + HASH_CSPRNG_DOMAIN_SEP_CONST;

		if (fixed_weight_b[i] == 1) {
			memcpy(cmt_1_i_input,
			       rounds_seeds + SEED_LENGTH_BYTES * i,
			       SEED_LENGTH_BYTES);

			cmt_1_i_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES] = (domain_sep_idx_hash >> 8) & 0xFF;
			cmt_1_i_input[SEED_LENGTH_BYTES + SALT_LENGTH_BYTES + 1] = domain_sep_idx_hash & 0xFF;
			hash(cmt_1[i], cmt_1_i_input, sizeof(cmt_1_i_input));

			/* CSPRNG is fed with concat(seed,salt,round index) represented
			* as a 2 bytes little endian unsigned integer */
			uint8_t csprng_input[CSPRNG_INPUT_LENGTH];
			memcpy(csprng_input + SEED_LENGTH_BYTES, sig->salt, SALT_LENGTH_BYTES);
			memcpy(csprng_input, rounds_seeds + SEED_LENGTH_BYTES * i, SEED_LENGTH_BYTES);
			csprng_input[SALT_LENGTH_BYTES + SEED_LENGTH_BYTES] = (domain_sep_i >> 8) & 0xFF;
			csprng_input[SALT_LENGTH_BYTES + SEED_LENGTH_BYTES + 1] = domain_sep_i & 0xFF;

			/* expand seed[i] into seed_e and seed_u */
			initialize_csprng(&CSPRNG_state, csprng_input, CSPRNG_INPUT_LENGTH);
			/* expand eta_tilde */
			CSPRNG_zz_vec(eta_tilde, &CSPRNG_state);
			/* expand u_tilde */
			CSPRNG_fq_vec(u_tilde, &CSPRNG_state);

			/* PQClean-edit: CSPRNG release context */
			csprng_release(&CSPRNG_state);

			fq_vec_by_restr_vec_scaled(y[i],
			                           eta_tilde,
			                           beta[i],
			                           u_tilde);
			fq_dz_norm(y[i]);
		} else {
			/* place y[i] in the buffer for later on hashing */
			PQCLEAN_CROSSRSDP256BALANCED_CLEAN_unpack_fq_vec(y[i], sig->rsp_0[used_rsps].y);

			FZ_ELEM sigma_local[N];
			/*sigma is memcpy'ed directly into cmt_0 input buffer */
			FZ_ELEM *sigma_ptr = cmt_0_i_input + DENSELY_PACKED_FQ_SYN_SIZE;
			PQCLEAN_CROSSRSDP256BALANCED_CLEAN_unpack_fz_vec(sigma_local, sig->rsp_0[used_rsps].sigma);
			memcpy(sigma_ptr,
			       &sig->rsp_0[used_rsps].sigma,
			       DENSELY_PACKED_FZ_VEC_SIZE);
			is_signature_ok = is_signature_ok &&
			                  is_zz_vec_in_restr_group(sigma_local);
			memcpy(cmt_1[i], sig->rsp_1[used_rsps], HASH_DIGEST_LENGTH);
			used_rsps++;

			FQ_ELEM v[N];
			convert_restr_vec_to_fq(v, sigma_local);
			fq_vec_by_fq_vec_pointwise(y_tilde, v, y[i]);
			fq_vec_by_fq_matrix(s_tilde, y_tilde, V_tr);
			fq_dz_norm_synd(s_tilde);
			FQ_ELEM to_compress[N - K];
			fq_synd_minus_fq_vec_scaled(to_compress,
			                            s_tilde,
			                            beta[i],
			                            pub_syn);
			fq_dz_norm_synd(to_compress);
			PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fq_syn(cmt_0_i_input, to_compress);
			cmt_0_i_input[offset_round_idx] = (domain_sep_idx_hash >> 8) & 0xFF;
			cmt_0_i_input[offset_round_idx + 1] = domain_sep_idx_hash & 0xFF;

			hash(cmt_0[i], cmt_0_i_input, sizeof(cmt_0_i_input));
		}
	} /* end for iterating on ZKID iterations */

	/* PQClean-edit: remove assertion */
	//assert(is_signature_ok);

	uint8_t commit_digests[2][HASH_DIGEST_LENGTH];
	PQCLEAN_CROSSRSDP256BALANCED_CLEAN_merkle_tree_root_recompute(commit_digests[0],
	        cmt_0,
	        sig->mtp,
	        fixed_weight_b);
	hash(commit_digests[1], (unsigned char *)cmt_1, sizeof(cmt_1));

	uint8_t digest_01_recomputed[HASH_DIGEST_LENGTH];
	hash(digest_01_recomputed,
	     (unsigned char *) commit_digests,
	     sizeof(commit_digests));

	uint8_t digest_b_buf[T * DENSELY_PACKED_FQ_VEC_SIZE + HASH_DIGEST_LENGTH];
	for (int x = 0; x < T; x++) {
		PQCLEAN_CROSSRSDP256BALANCED_CLEAN_pack_fq_vec(digest_b_buf + (x * DENSELY_PACKED_FQ_VEC_SIZE), y[x]);
	}
	memcpy(digest_b_buf + T * DENSELY_PACKED_FQ_VEC_SIZE, d_beta, HASH_DIGEST_LENGTH);

	uint8_t digest_b_recomputed[HASH_DIGEST_LENGTH];
	hash(digest_b_recomputed, digest_b_buf, sizeof(digest_b_buf));

	int does_digest_01_match = ( memcmp(digest_01_recomputed,
	                                    sig->digest_01,
	                                    HASH_DIGEST_LENGTH) == 0);

	/* PQClean-edit: remove assertion */
	//assert(does_digest_01_match);

	int does_digest_b_match = ( memcmp(digest_b_recomputed,
	                                   sig->digest_b,
	                                   HASH_DIGEST_LENGTH) == 0);

	/* PQClean-edit: remove assertion */
	//assert(does_digest_b_match);

	is_signature_ok = is_signature_ok &&
	                  does_digest_01_match &&
	                  does_digest_b_match;
	return is_signature_ok;
}
