#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "address.h"
#include "api.h"
#include "fors.h"
#include "hash.h"
#include "hash_state.h"
#include "params.h"
#include "randombytes.h"
#include "thash.h"
#include "utils.h"
#include "wots.h"

/**
 * Computes the leaf at a given address. First generates the WOTS key pair,
 * then computes leaf by hashing horizontally.
 */
static void wots_gen_leaf(unsigned char *leaf, const unsigned char *sk_seed,
                          const unsigned char *pub_seed,
                          uint32_t addr_idx, const uint32_t tree_addr[8],
                          const hash_state *hash_state_seeded) {
    unsigned char pk[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_WOTS_BYTES];
    uint32_t wots_addr[8] = {0};
    uint32_t wots_pk_addr[8] = {0};

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        wots_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_WOTS);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        wots_pk_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_WOTSPK);

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_copy_subtree_addr(
        wots_addr, tree_addr);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_keypair_addr(
        wots_addr, addr_idx);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_wots_gen_pk(
        pk, sk_seed, pub_seed, wots_addr, hash_state_seeded);

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_copy_keypair_addr(
        wots_pk_addr, wots_addr);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_thash_WOTS_LEN(
        leaf, pk, pub_seed, wots_pk_addr, hash_state_seeded);
}

/*
 * Returns the length of a secret key, in bytes
 */
size_t PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_secretkeybytes(void) {
    return PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_CRYPTO_SECRETKEYBYTES;
}

/*
 * Returns the length of a public key, in bytes
 */
size_t PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_publickeybytes(void) {
    return PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_CRYPTO_PUBLICKEYBYTES;
}

/*
 * Returns the length of a signature, in bytes
 */
size_t PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_bytes(void) {
    return PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_CRYPTO_BYTES;
}

/*
 * Returns the length of the seed required to generate a key pair, in bytes
 */
size_t PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_seedbytes(void) {
    return PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_CRYPTO_SEEDBYTES;
}

/*
 * Generates an SPX key pair given a seed of length
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [PUB_SEED || root]
 */
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_seed_keypair(
    uint8_t *pk, uint8_t *sk, const uint8_t *seed) {
    /* We do not need the auth path in key generation, but it simplifies the
       code to have just one treehash routine that computes both root and path
       in one function. */
    unsigned char auth_path[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N];
    uint32_t top_tree_addr[8] = {0};
    hash_state hash_state_seeded;

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_layer_addr(
        top_tree_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_D - 1);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        top_tree_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_HASHTREE);

    /* Initialize SK_SEED, SK_PRF and PUB_SEED from seed. */
    memcpy(sk, seed, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_CRYPTO_SEEDBYTES);

    memcpy(pk, sk + 2 * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_initialize_hash_function(&hash_state_seeded, pk, sk);

    /* Compute root node of the top-most subtree. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_treehash_TREE_HEIGHT(
        sk + 3 * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N, auth_path, sk, sk + 2 * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N, 0, 0,
        wots_gen_leaf, top_tree_addr, &hash_state_seeded);

    memcpy(pk + PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N, sk + 3 * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N);

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_destroy_hash_function(&hash_state_seeded);
    return 0;
}

/*
 * Generates an SPX key pair.
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [PUB_SEED || root]
 */
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_keypair(
    uint8_t *pk, uint8_t *sk) {
    unsigned char seed[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_CRYPTO_SEEDBYTES];
    randombytes(seed, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_CRYPTO_SEEDBYTES);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_seed_keypair(
        pk, sk, seed);

    return 0;
}

/**
 * Returns an array containing a detached signature.
 */
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    const unsigned char *sk_seed = sk;
    const unsigned char *sk_prf = sk + PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N;
    const unsigned char *pk = sk + 2 * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N;
    const unsigned char *pub_seed = pk;

    unsigned char optrand[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N];
    unsigned char mhash[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_FORS_MSG_BYTES];
    unsigned char root[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N];
    uint32_t i;
    uint64_t tree;
    uint32_t idx_leaf;
    uint32_t wots_addr[8] = {0};
    uint32_t tree_addr[8] = {0};

    hash_state hash_state_seeded;

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_initialize_hash_function(
        &hash_state_seeded,
        pub_seed, sk_seed);

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        wots_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_WOTS);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        tree_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_HASHTREE);

    /* Optionally, signing can be made non-deterministic using optrand.
       This can help counter side-channel attacks that would benefit from
       getting a large number of traces when the signer uses the same nodes. */
    randombytes(optrand, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N);
    /* Compute the digest randomization value. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_gen_message_random(
        sig, sk_prf, optrand, m, mlen, &hash_state_seeded);

    /* Derive the message digest and leaf index from R, PK and M. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_hash_message(
        mhash, &tree, &idx_leaf, sig, pk, m, mlen, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N;

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_tree_addr(wots_addr, tree);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_keypair_addr(
        wots_addr, idx_leaf);

    /* Sign the message hash using FORS. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_fors_sign(
        sig, root, mhash, sk_seed, pub_seed, wots_addr, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_FORS_BYTES;

    for (i = 0; i < PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_D; i++) {
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_layer_addr(tree_addr, i);
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_tree_addr(tree_addr, tree);

        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_copy_subtree_addr(
            wots_addr, tree_addr);
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_keypair_addr(
            wots_addr, idx_leaf);

        /* Compute a WOTS signature. */
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_wots_sign(
            sig, root, sk_seed, pub_seed, wots_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_WOTS_BYTES;

        /* Compute the authentication path for the used WOTS leaf. */
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_treehash_TREE_HEIGHT(
            root, sig, sk_seed, pub_seed, idx_leaf, 0,
            wots_gen_leaf, tree_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N;

        /* Update the indices for the next layer. */
        idx_leaf = (tree & ((1 << PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT) - 1));
        tree = tree >> PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT;
    }

    *siglen = PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES;

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_destroy_hash_function(&hash_state_seeded);
    return 0;
}

/**
 * Verifies a detached signature and message under a given public key.
 */
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk) {
    const unsigned char *pub_seed = pk;
    const unsigned char *pub_root = pk + PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N;
    unsigned char mhash[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_FORS_MSG_BYTES];
    unsigned char wots_pk[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_WOTS_BYTES];
    unsigned char root[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N];
    unsigned char leaf[PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N];
    unsigned int i;
    uint64_t tree;
    uint32_t idx_leaf;
    uint32_t wots_addr[8] = {0};
    uint32_t tree_addr[8] = {0};
    uint32_t wots_pk_addr[8] = {0};

    hash_state hash_state_seeded;

    if (siglen != PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES) {
        return -1;
    }

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_initialize_hash_function(
        &hash_state_seeded,
        pub_seed, NULL);

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        wots_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_WOTS);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        tree_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_HASHTREE);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_type(
        wots_pk_addr, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_ADDR_TYPE_WOTSPK);

    /* Derive the message digest and leaf index from R || PK || M. */
    /* The additional PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N is a result of the hash domain separator. */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_hash_message(
        mhash, &tree, &idx_leaf, sig, pk, m, mlen, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N;

    /* Layer correctly defaults to 0, so no need to set_layer_addr */
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_tree_addr(wots_addr, tree);
    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_keypair_addr(
        wots_addr, idx_leaf);

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_fors_pk_from_sig(
        root, sig, mhash, pub_seed, wots_addr, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_FORS_BYTES;

    /* For each subtree.. */
    for (i = 0; i < PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_D; i++) {
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_layer_addr(tree_addr, i);
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_tree_addr(tree_addr, tree);

        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_copy_subtree_addr(
            wots_addr, tree_addr);
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_set_keypair_addr(
            wots_addr, idx_leaf);

        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_copy_keypair_addr(
            wots_pk_addr, wots_addr);

        /* The WOTS public key is only correct if the signature was correct. */
        /* Initially, root is the FORS pk, but on subsequent iterations it is
           the root of the subtree below the currently processed subtree. */
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_wots_pk_from_sig(
            wots_pk, sig, root, pub_seed, wots_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_WOTS_BYTES;

        /* Compute the leaf node using the WOTS public key. */
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_thash_WOTS_LEN(
            leaf, wots_pk, pub_seed, wots_pk_addr, &hash_state_seeded);

        /* Compute the root node of this subtree. */
        PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_compute_root(
            root, leaf, idx_leaf, 0, sig, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT,
            pub_seed, tree_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT * PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N;

        /* Update the indices for the next layer. */
        idx_leaf = (tree & ((1 << PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT) - 1));
        tree = tree >> PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_TREE_HEIGHT;
    }

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_destroy_hash_function(&hash_state_seeded);
    /* Check if the root node equals the root node in the public key. */
    if (memcmp(root, pub_root, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_N) != 0) {
        return -1;
    }

    return 0;
}


/**
 * Returns an array containing the signature followed by the message.
 */
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    size_t siglen;

    PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_signature(
        sm, &siglen, m, mlen, sk);

    memmove(sm + PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES, m, mlen);
    *smlen = siglen + mlen;

    return 0;
}

/**
 * Verifies a given signature-message pair under a given public key.
 */
int PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen, const uint8_t *pk) {
    /* The API caller does not necessarily know what size a signature should be
       but SPHINCS+ signatures are always exactly PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES. */
    if (smlen < PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES) {
        memset(m, 0, smlen);
        *mlen = 0;
        return -1;
    }

    *mlen = smlen - PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES;

    if (PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_crypto_sign_verify(
                sm, PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES, sm + PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES, *mlen, pk)) {
        memset(m, 0, smlen);
        *mlen = 0;
        return -1;
    }

    /* If verification was successful, move the message to the right place. */
    memmove(m, sm + PQCLEAN_SPHINCSHARAKA256SROBUST_CLEAN_BYTES, *mlen);

    return 0;
}
