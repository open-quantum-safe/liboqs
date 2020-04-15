#include <immintrin.h>
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
    unsigned char pk[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_WOTS_BYTES];
    uint32_t wots_addr[8] = {0};
    uint32_t wots_pk_addr[8] = {0};

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        wots_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_WOTS);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        wots_pk_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_WOTSPK);

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_copy_subtree_addr(
        wots_addr, tree_addr);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_keypair_addr(
        wots_addr, addr_idx);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_wots_gen_pk(
        pk, sk_seed, pub_seed, wots_addr, hash_state_seeded);

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_copy_keypair_addr(
        wots_pk_addr, wots_addr);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_thash_WOTS_LEN(
        leaf, pk, pub_seed, wots_pk_addr, hash_state_seeded);
}

/*
 * Returns the length of a secret key, in bytes
 */
size_t PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_secretkeybytes(void) {
    return PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SECRETKEYBYTES;
}

/*
 * Returns the length of a public key, in bytes
 */
size_t PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_publickeybytes(void) {
    return PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES;
}

/*
 * Returns the length of a signature, in bytes
 */
size_t PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_bytes(void) {
    return PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_BYTES;
}

/*
 * Returns the length of the seed required to generate a key pair, in bytes
 */
size_t PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_seedbytes(void) {
    return PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SEEDBYTES;
}

/*
 * Generates an SPX key pair given a seed of length
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [PUB_SEED || root]
 */
int PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_seed_keypair(
    uint8_t *pk, uint8_t *sk, const uint8_t *seed) {
    /* We do not need the auth path in key generation, but it simplifies the
       code to have just one treehash routine that computes both root and path
       in one function. */
    unsigned char auth_path[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N];
    uint32_t top_tree_addr[8] = {0};
    hash_state hash_state_seeded;

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_layer_addr(
        top_tree_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_D - 1);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        top_tree_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_HASHTREE);

    /* Initialize SK_SEED, SK_PRF and PUB_SEED from seed. */
    memcpy(sk, seed, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SEEDBYTES);

    memcpy(pk, sk + 2 * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N);

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_initialize_hash_function(&hash_state_seeded, pk, sk);

    /* Compute root node of the top-most subtree. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_treehash_TREE_HEIGHT(
        sk + 3 * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N, auth_path, sk, sk + 2 * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N, 0, 0,
        wots_gen_leaf, top_tree_addr, &hash_state_seeded);

    memcpy(pk + PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N, sk + 3 * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N);

    return 0;
}

/*
 * Generates an SPX key pair.
 * Format sk: [SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [PUB_SEED || root]
 */
int PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_keypair(
    uint8_t *pk, uint8_t *sk) {

    // guarantee alignment of pk
    union {
        __m128 _x[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES / 16];
        uint8_t pk[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES];
    } aligned_pk;

    // guarantee alignment of sk
    union {
        __m128 _x[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SECRETKEYBYTES / 16];
        uint8_t sk[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SECRETKEYBYTES];
    } aligned_sk;

    union {
        __m128 _x[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SEEDBYTES / 16];
        uint8_t seed[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SEEDBYTES];
    } aligned_seed;
    randombytes(aligned_seed.seed, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SEEDBYTES);

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_seed_keypair(
        aligned_pk.pk, aligned_sk.sk, aligned_seed.seed);
    memcpy(pk, aligned_pk.pk, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES);
    memcpy(sk, aligned_sk.sk, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SECRETKEYBYTES);

    return 0;
}

/**
 * Returns an array containing a detached signature.
 */
int PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_signature(
    uint8_t *sig, size_t *siglen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    // guarantee alignment of sk
    union {
        __m128 *_x;
        uint8_t sk[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SECRETKEYBYTES];
    } aligned_sk;
    memcpy(aligned_sk.sk, sk, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_SECRETKEYBYTES);
    sk = aligned_sk.sk;

    // guarantee alignment of sig
    union {
        __m128 *_x;
        uint8_t sig[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES];
    } aligned_sig;
    uint8_t *orig_sig = sig;
    sig = (uint8_t *)aligned_sig.sig;

    const unsigned char *sk_seed = sk;
    const unsigned char *sk_prf = sk + PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N;
    const unsigned char *pk = sk + 2 * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N;
    const unsigned char *pub_seed = pk;

    unsigned char optrand[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N];
    unsigned char mhash[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_FORS_MSG_BYTES];
    unsigned char root[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N];
    uint32_t i;
    uint64_t tree;
    uint32_t idx_leaf;
    uint32_t wots_addr[8] = {0};
    uint32_t tree_addr[8] = {0};

    hash_state hash_state_seeded;

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_initialize_hash_function(
        &hash_state_seeded,
        pub_seed, sk_seed);

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        wots_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_WOTS);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        tree_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_HASHTREE);

    /* Optionally, signing can be made non-deterministic using optrand.
       This can help counter side-channel attacks that would benefit from
       getting a large number of traces when the signer uses the same nodes. */
    randombytes(optrand, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N);
    /* Compute the digest randomization value. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_gen_message_random(
        sig, sk_prf, optrand, m, mlen, &hash_state_seeded);

    /* Derive the message digest and leaf index from R, PK and M. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_hash_message(
        mhash, &tree, &idx_leaf, sig, pk, m, mlen, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N;

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_tree_addr(wots_addr, tree);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_keypair_addr(
        wots_addr, idx_leaf);

    /* Sign the message hash using FORS. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_fors_sign(
        sig, root, mhash, sk_seed, pub_seed, wots_addr, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_FORS_BYTES;

    for (i = 0; i < PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_D; i++) {
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_layer_addr(tree_addr, i);
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_tree_addr(tree_addr, tree);

        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_copy_subtree_addr(
            wots_addr, tree_addr);
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_keypair_addr(
            wots_addr, idx_leaf);

        /* Compute a WOTS signature. */
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_wots_sign(
            sig, root, sk_seed, pub_seed, wots_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_WOTS_BYTES;

        /* Compute the authentication path for the used WOTS leaf. */
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_treehash_TREE_HEIGHT(
            root, sig, sk_seed, pub_seed, idx_leaf, 0,
            wots_gen_leaf, tree_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N;

        /* Update the indices for the next layer. */
        idx_leaf = (tree & ((1 << PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT) - 1));
        tree = tree >> PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT;
    }

    memcpy(orig_sig, aligned_sig.sig, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES);
    *siglen = PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES;

    return 0;
}

/**
 * Verifies a detached signature and message under a given public key.
 */
int PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_verify(
    const uint8_t *sig, size_t siglen,
    const uint8_t *m, size_t mlen, const uint8_t *pk) {
    // guarantee alignment of pk
    union {
        __m128 *_x;
        uint8_t pk[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES];
    } aligned_pk;
    memcpy(aligned_pk.pk, pk, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES);
    pk = aligned_pk.pk;

    const unsigned char *pub_seed = pk;
    const unsigned char *pub_root = pk + PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N;
    unsigned char mhash[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_FORS_MSG_BYTES];
    unsigned char wots_pk[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_WOTS_BYTES];
    unsigned char root[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N];
    unsigned char leaf[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N];
    unsigned int i;
    uint64_t tree;
    uint32_t idx_leaf;
    uint32_t wots_addr[8] = {0};
    uint32_t tree_addr[8] = {0};
    uint32_t wots_pk_addr[8] = {0};

    hash_state hash_state_seeded;

    if (siglen != PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES) {
        return -1;
    }

    /* This hook allows the hash function instantiation to do whatever
       preparation or computation it needs, based on the public seed. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_initialize_hash_function(
        &hash_state_seeded,
        pub_seed, NULL);

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        wots_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_WOTS);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        tree_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_HASHTREE);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_type(
        wots_pk_addr, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_ADDR_TYPE_WOTSPK);

    /* Derive the message digest and leaf index from R || PK || M. */
    /* The additional PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N is a result of the hash domain separator. */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_hash_message(
        mhash, &tree, &idx_leaf, sig, pk, m, mlen, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N;

    /* Layer correctly defaults to 0, so no need to set_layer_addr */
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_tree_addr(wots_addr, tree);
    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_keypair_addr(
        wots_addr, idx_leaf);

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_fors_pk_from_sig(
        root, sig, mhash, pub_seed, wots_addr, &hash_state_seeded);
    sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_FORS_BYTES;

    /* For each subtree.. */
    for (i = 0; i < PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_D; i++) {
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_layer_addr(tree_addr, i);
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_tree_addr(tree_addr, tree);

        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_copy_subtree_addr(
            wots_addr, tree_addr);
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_set_keypair_addr(
            wots_addr, idx_leaf);

        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_copy_keypair_addr(
            wots_pk_addr, wots_addr);

        /* The WOTS public key is only correct if the signature was correct. */
        /* Initially, root is the FORS pk, but on subsequent iterations it is
           the root of the subtree below the currently processed subtree. */
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_wots_pk_from_sig(
            wots_pk, sig, root, pub_seed, wots_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_WOTS_BYTES;

        /* Compute the leaf node using the WOTS public key. */
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_thash_WOTS_LEN(
            leaf, wots_pk, pub_seed, wots_pk_addr, &hash_state_seeded);

        /* Compute the root node of this subtree. */
        PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_compute_root(
            root, leaf, idx_leaf, 0, sig, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT,
            pub_seed, tree_addr, &hash_state_seeded);
        sig += PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT * PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N;

        /* Update the indices for the next layer. */
        idx_leaf = (tree & ((1 << PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT) - 1));
        tree = tree >> PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_TREE_HEIGHT;
    }

    /* Check if the root node equals the root node in the public key. */
    if (memcmp(root, pub_root, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_N) != 0) {
        return -1;
    }

    return 0;
}


/**
 * Returns an array containing the signature followed by the message.
 */
int PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign(
    uint8_t *sm, size_t *smlen,
    const uint8_t *m, size_t mlen, const uint8_t *sk) {
    size_t siglen;

    PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_signature(
        sm, &siglen, m, mlen, sk);

    memmove(sm + PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES, m, mlen);
    *smlen = siglen + mlen;

    return 0;
}

/**
 * Verifies a given signature-message pair under a given public key.
 */
int PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_open(
    uint8_t *m, size_t *mlen,
    const uint8_t *sm, size_t smlen, const uint8_t *pk) {

    // guarantee alignment of pk
    union {
        __m128 *_x;
        uint8_t pk[PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES];
    } aligned_pk;
    memcpy(aligned_pk.pk, pk, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_CRYPTO_PUBLICKEYBYTES);
    pk = aligned_pk.pk;


    /* The API caller does not necessarily know what size a signature should be
       but SPHINCS+ signatures are always exactly PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES. */
    if (smlen < PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES) {
        memset(m, 0, smlen);
        *mlen = 0;
        return -1;
    }

    *mlen = smlen - PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES;

    if (PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_crypto_sign_verify(
                sm, PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES, sm + PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES, *mlen, pk)) {
        memset(m, 0, smlen);
        *mlen = 0;
        return -1;
    }

    /* If verification was successful, move the message to the right place. */
    memmove(m, sm + PQCLEAN_SPHINCSSHAKE256128FROBUST_AVX2_BYTES, *mlen);

    return 0;
}
