#include "ggm.h"

#include <math.h>
#include <memory.h>

#include "sdith_prng.h"
#include "vole_private.h"

// private layout (move to a ggm-private.h if needed)
struct full_ggm_tree_t {
  uint32_t lambda;
  uint32_t tau;
  uint32_t kappa;
  uint32_t lambda_bytes;
  uint32_t depth;
  uint32_t theta;                  // domain-separation truncation width = depth + 2
  uint32_t num_leaves;
  uint64_t rng_data[4];            // here: global salt (lambda bits used)
  GGM_SEED_RNG_LR_F* seed_rng_lr;  // left and right in one call
  GGM_COMMIT_RNG_F* commit_rng;    // takes the extended leaf seed
  EXTEND_LEAF_SEED_F* extend_leaf_seed;  // raw leaf seed -> extended leaf seed
  uint32_t extended_bytes;               // byte size of an extended leaf seed
  const seed_t* root_seed;  // shared with seeds_cache
  seed_t* seeds_cache;      // 2*lambda_bytes*depth bytes
  uint32_t cached_index;    //
  uint32_t* open_buf;       // 4*tau*depth bytes
  // space for the root seed copy + cache + open buffer
};

/** @brief  if x==0, return 0, else floor(log2(x)) */
uint32_t floor_log2i(uint32_t x) {
  if (x == 0) return 0;
  return 31 - __builtin_clz(x);
}

// alternative function to use if the builtin above does not exist
uint32_t floor_log2i_compat(uint32_t x) {
  uint32_t res = 0;
  uint32_t t = x;
  if (t >> 16) {
    res += 16;
    t >>= 16;
  }
  if (t >> 8) {
    res += 8;
    t >>= 8;
  }
  if (t >> 4) {
    res += 4;
    t >>= 4;
  }
  if (t >> 2) {
    res += 2;
    t >>= 2;
  }
  if (t >> 1) {
    res += 1;
    t >>= 1;
  }
  return res;
}

/** @brief  if x==0, return 0, else ceil(log2(x)) */
uint32_t ceil_log2i(uint32_t x) {
  uint32_t res = floor_log2i(x);
  if (x == (UINT32_C(1) << res))
    return res;
  else
    return res + 1;
}

EXPORT uint32_t estimate_topen_tmp_bytes(uint32_t tau, uint32_t kappa) {
  uint32_t depth = ceil_log2i(tau) + kappa;  // depth = ceil(log2(tau)) + kappa.
  return 4 * tau * depth;
}


/** decode "delta" into the indexes of the hidden leaves */
//  hidden_nodes = [delta[i.kappa, (i+1).kappa) * tau + i  for i in range(tau)]
//  qsort hidden_leaves
// uint64_t hidden_leaves_indexes(uint64_t kappa, uint64_t tau,  // dims
//                                uint32_t* hidden_leaves,       // output: node indexes
//                                const bitvec_t* delta);        // input: bits of delta

// swap two elements - used in partition
void swap(uint32_t* a, uint32_t* b) {
  uint32_t t = *a;
  *a = *b;
  *b = t;
}

// partioning - used in quick sort
int partition(uint32_t arr[], int low, int high) {
  uint32_t pivot = arr[high];
  int hi = high - 1;
  int lo = low;
  while (1) {
    while (lo <= hi && arr[lo] <= pivot) ++lo;
    while (lo <= hi && arr[hi] >= pivot) --hi;
    if (lo > hi) break;
    swap(arr + lo, arr + hi);
    ++lo;
    --hi;
  }
  swap(arr + lo, arr + high);
  return lo;
}

// QuickSort - used in hidden_leaves_indexes
void quickSort(uint32_t arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);

    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

__always_inline uint32_t extract_kappabit_uint_inline(uint64_t kappa, uint64_t bitpos, const uint64_t* data) {
  const uint64_t kappa_mask = (UINT64_C(1) << kappa) - 1;
  uint64_t limb_pos = bitpos >> 6;
  uint64_t limb_rem = bitpos & UINT64_C(63);
  if (limb_rem + kappa <= 64) {
    return (data[limb_pos] >> limb_rem) & kappa_mask;
  } else {
    return ((data[limb_pos] >> limb_rem) | (data[limb_pos + 1] << (64 - limb_rem))) & kappa_mask;
  }
}
__always_inline void xorto_kappabit_uint_inline(uint64_t kappa, uint64_t bitpos, uint64_t* data, uint64_t value) {
  const uint64_t kappa_mask = (UINT64_C(1) << kappa) - 1;
  uint64_t limb_pos = bitpos >> 6;
  uint64_t limb_rem = bitpos & UINT64_C(63);
  value &= kappa_mask;
  if (limb_rem + kappa <= 64) {
    data[limb_pos] ^= value << limb_rem;
  } else {
    data[limb_pos] ^= value << limb_rem;
    data[limb_pos + 1] ^= value >> (64 - limb_rem);
  }
}

EXPORT uint32_t extract_kappabit_uint(uint64_t kappa, uint64_t bitpos, const void* data) {
  return extract_kappabit_uint_inline(kappa, bitpos, (uint64_t*)data);
}
EXPORT void xorto_kappabit_uint(uint64_t kappa, uint64_t bitpos, void* data, uint64_t value) {
  xorto_kappabit_uint_inline(kappa, bitpos, (uint64_t*)data, value);
}

EXPORT void hidden_leaves_indexes(uint64_t kappa, uint64_t tau, uint32_t* hidden_leaves, const bitvec_t* delta) {
  for (uint64_t k = 0; k < tau; ++k) {
    hidden_leaves[k] = extract_kappabit_uint_inline(kappa, k * kappa, delta) * tau + k;
  }
  quickSort(hidden_leaves, 0, tau - 1);
}
EXPORT void hidden_leaves_indexes2(uint64_t kappa, uint64_t tau, uint32_t* hidden_leaves, const bitvec_t* delta) {
  const uint64_t start_index = tau * (UINT64_C(1) << kappa);
  for (uint64_t k = 0; k < tau; ++k) {
    hidden_leaves[k] = extract_kappabit_uint_inline(kappa, k * kappa, delta) * tau + k + start_index;
  }
  quickSort(hidden_leaves, 0, tau - 1);
}

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
uint64_t estimate_topen(const uint32_t tau, const uint32_t kappa, uint64_t max_topen, const uint32_t* hidden_leaves_idx,
                        uint8_t* tmp_space) {
  // CASSERT(max_topen < tree->tau * tree->kappa, "topen bug");
  uint32_t sibling_path_size = 0;
  uint32_t* bstart = (uint32_t*)tmp_space;
  uint32_t* bend = bstart + tau;
#ifndef NDEBUG
  uint32_t num_leaves = tau * UINT64_C(1) << kappa;
  uint32_t last_leave_index = num_leaves - 1;
  for (uint32_t i = 0; i < tau; ++i) {
    CREQUIRE(hidden_leaves_idx[i] > last_leave_index, "incorrect leave node index");
    last_leave_index = hidden_leaves_idx[i];
  }
#endif
  // copy the leaf indexes in reversed order
  for (uint32_t i = 0; i < tau; ++i) {
    bstart[tau - i - 1] = hidden_leaves_idx[i];
  }
  while (bend - bstart >= 2) {
    uint32_t first_idx = *(bstart++);
    uint32_t second_idx = *(bstart);
    uint32_t next_idx = first_idx >> 1;
    CASSERT(first_idx > 1, "bug1");
    CASSERT(*(bend - 1) > next_idx, "bug2");
    if ((first_idx ^ second_idx) == 1) {
      // two hidden nodes with the same parent
      ++bstart;
    } else {
      // single child out of at least 2
      ++sibling_path_size;
    }
    *(bend++) = next_idx;
  }
  while (*bstart != 1) {
    ++sibling_path_size;
    *bstart >>= 1;
  }
  return sibling_path_size;
}


struct ggm_multi_item_t {
  uint16_t type : 2;
  uint16_t addr : 14;
};
