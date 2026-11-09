#ifndef SDITHER_RSD_H
#define SDITHER_RSD_H

#include "vole_parameters.h"

/// keygen stuff

EXPORT uint64_t rsd_generate_random_instance_tmp_bytes(  //
    const vole_parameters* vole_params,                  //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim);

/** generate a random rsd instance out of the master key entropy.
 * Only the parts of the key that have to be stored are produced: the solution
 * (from the sk_seed) and the syndrome y = H.e (from the pk_seed). H itself is
 * the public key -- it is recoverable from pk_seed with
 * rsd_expand_public_key_ref -- so it is never materialised here: y touches at
 * most rsd_w of its rows, and those are drawn one at a time into the scratch.
 * Output is identical to expanding H and xoring the selected rows out of it.
 * Constant-time in the solution. tmp_space still holds one expanded H row on
 * return: the caller must scrub it. */
EXPORT void rsd_generate_random_instance_ref(            //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,  // rsd dimensions
    const vole_parameters* vole_params,                  // prng + matrix-prng family
    bitvec_t* y, uint64_t y_bytes,                       // out: syndrome (ceil(rsd_codim/8) bytes are set)
    uint32_t* solution,                                  // out: rsd_w integers in [0,n/w-1]
    const seed_t* sk_seed,                               // in: secret key seed (lambda bits)
    const seed_t* pk_seed,                               // in: pubkey seed (lambda bits)
    uint8_t* tmp_space                                   // scratch space
);

/** expand the public key from the pubkey seed (rows via vole_params->matrix_prg).
 * Each row occupies rsd_codim_limbs*lambda_bytes bytes; h_slice_bytes must be at
 * least that and keep every row aligned to the cipher block (16/8/32 bytes). */
EXPORT void rsd_expand_public_key_ref(                   //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,  // rsd dimensions
    const vole_parameters* vole_params,                  // matrix-prng family
    bitmat_t* h, uint64_t h_slice_bytes,                 // out: (rsd_n - rsd_codim) x rsd_n matrix
    const seed_t* h_seed                                 // in: pubkey seed (lambda bits)
);

/** unary encoding of the solution (for mux circuits).
 * Constant-time in the solution coefficients (mux_arities must be < 32). */
EXPORT void rsd_encode_solution_ref(                              //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,           // rsd dimensions
    const vole_parameters* vole_params,                           // ct helpers
    uint64_t mux_depth, const uint64_t* mux_arities,              // mux depth and arities
    bitvec_t* encoded_solution, uint64_t encoded_solution_bytes,  // encoded solution
    const uint32_t* solution                                      // in: solution coefficients
);

EXPORT uint64_t rsd_public_key_times_challenge_tmp_bytes(  //
    const vole_parameters* vole_params,                    //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim);

/** multiply the public key with challenge points */
EXPORT void rsd_public_key_times_challenge_ref(          //
    const vole_parameters* vole_params,                  //
    uint64_t rsd_w, uint64_t rsd_n, uint64_t rsd_codim,  // rsd dimensions
    flambda_t* chall_a_H,                                // out: n elements
    flambda_t* chall_a_y,                                // out: 1 element
    const flambda_t* chall_a,                            // in: rsd_codim_limbs elements
    const seed_t* pk_seed,                               // in: pubkey seed (lambda bits)
    const bitvec_t* y,                                   // in: pubkey y (rsd_codim bits padded with zeros)
    uint8_t* tmp_space                                   // scratch space
);

#endif  // SDITHER_RSD_H
