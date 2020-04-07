#ifndef SPX_HASH_STATE_H
#define SPX_HASH_STATE_H

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
 */

#include "sha2.h"
#define hash_state sha256ctx

#endif
