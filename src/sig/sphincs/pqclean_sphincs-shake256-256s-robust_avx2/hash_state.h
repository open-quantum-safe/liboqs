#ifndef PQCLEAN_SPHINCSSHAKE256256SROBUST_AVX2_HASH_STATE_H
#define PQCLEAN_SPHINCSSHAKE256256SROBUST_AVX2_HASH_STATE_H

/**
 * Defines the type of the hash function state.
 *
 * Don't be fooled into thinking this instance of SPHINCS+ isn't stateless!
 *
 * From Section 7.2.2 from the SPHINCS+ round-2 specification:
 *
 * Each of the instances of the tweakable hash function take PK.seed as its
 * first input, which is constant for a given key pair – and, thus, across
 * a single signature. This leads to a lot of redundant computation. To remedy
 * this, we pad PK.seed to the length of a full 64-byte SHA-256 input block.
 * Because of the Merkle-Damgård construction that underlies SHA-256, this
 * allows for reuse of the intermediate SHA-256 state after the initial call to
 * the compression function which improves performance.
 *
 * We pass this hash state around in functions, because otherwise we need to
 * have a global variable.
 *
 * SHAKE256 does not need this state. Because this implementation is generated
 * from a shared code base, we still need to specify some hash_state as it is
 * still passed around. We chose to use an `int` as a placeholder for this
 * purpose.
 */

typedef int hash_state;

#endif
