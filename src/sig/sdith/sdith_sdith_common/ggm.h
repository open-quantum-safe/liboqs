#ifndef AQ_VOLE_IMPL_GGM_H
#define AQ_VOLE_IMPL_GGM_H

// This file contains the declaration of all algebra functions over vectors and matrices over F2

#include "commons.h"

typedef void GGM_SEED_RNG_LR_F(void* lr_out, const void* salt, const void* key, uint64_t node_idx, uint32_t theta);
typedef void GGM_COMMIT_RNG_F(void* out, const void* salt, const void* key, uint64_t node_idx);

/** decode "delta" into the indexes of the hidden leaves */
//  hidden_nodes = [delta[i.kappa, (i+1).kappa) * tau + i  for i in range(tau)]
//  qsort hidden_leaves
EXPORT void hidden_leaves_indexes(uint64_t kappa, uint64_t tau,  // dims
                                  uint32_t* hidden_leaves,       // output: node indexes
                                  const bitvec_t* delta);        // input: bits of delta
/** decode "delta" into the indexes of the hidden leaves:
 * the v2 version returns node indexes instead of leave indexes */
EXPORT void hidden_leaves_indexes2(uint64_t kappa, uint64_t tau,  // dims
                                   uint32_t* hidden_leaves,       // output: node indexes
                                   const bitvec_t* delta);        // input: bits of delta

/** estimate the number of nodes to open for this choice of delta: */
// algorithm:
//  sibling_path_indexes = []
//  new nodes_list = []
//  for (depth = tau*kappa-1 down to 0)
//    while hidden_leave is not empty {
//       if (hidden_leaves have >= 2 elements and hidden_leave[0] ^ hidden_leave[1] == 1) {
//          append hidden_leave[0] >> 1 to new nodes_list
//          pop the two first nodes of hidden_nodes
//       } else {
//          append hidden_leave[0] ^ 1 to sibling_path_indexes
//          append hidden_leave[0] >> 1 to new nodes_list
//          pop the first nodes of hidden_nodes
//       }
//    }
// }
EXPORT uint32_t estimate_topen_tmp_bytes(
    uint32_t tau, 
    uint32_t kappa
    );

EXPORT uint64_t estimate_topen(  //
    const uint32_t tau, 
    const uint32_t kappa, 
    uint64_t max_topen,                        // TODO ignored for now
    const uint32_t* hidden_leaves_idx,
    uint8_t* tmp_space
    );        //


#endif  // AQ_VOLE_IMPL_GGM_H
