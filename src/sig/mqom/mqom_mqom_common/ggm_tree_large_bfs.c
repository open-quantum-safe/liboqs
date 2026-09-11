/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

#ifdef LARGE_TREE_BFS

#include "ggm_tree_large_bfs.h"
#include "ggm_tree_large.h"
#include "ggm_tree_common.h"
#include "ggm_tree_large_common.h"

/* Expand one BFS level: derive 2*src_count children of src[0..src_count-1] into dst[].
 * Tweak for src[k]: TweakSalt(salt, SALT_SEL_GGM, base+k), where base is the heap index of src[0].
 * x8/x4 batches independent parents with compound contexts;
 * a scalar tail handles the 0-3 remaining nodes. */
static int bfs_derive_level(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        uint32_t base,
        uint8_t (*src)[MQOM3_PARAM_SEED_SIZE],
        uint32_t src_count,
        uint8_t (*dst)[MQOM3_PARAM_SEED_SIZE])
{
    uint32_t k;
    int ret = -1;

#ifdef LARGE_TREE_BFS_X8
    for (k = 0; k + 8 <= src_count; k += 8) {
        uint8_t ts[8][MQOM3_PARAM_SALT_SIZE];
        enc_ctx_x8 DECL_VAR(ctx_x8);
        uint32_t out = 2 * k;
        uint32_t b;

        for (b = 0; b < 8; b++) {
            TweakSalt(salt, ts[b], SALT_SEL_GGM, base + k + b);
        }

        ret = enc_key_sched_x8(&ctx_x8,
                               ts[0], ts[1], ts[2], ts[3],
                               ts[4], ts[5], ts[6], ts[7]);
        if (ret) {
            enc_clean_ctx_x8(&ctx_x8);
        }
        ERR(ret, err);

        ret = SeedDerive_x8_x8(&ctx_x8,
                               src[k], src[k + 1], src[k + 2], src[k + 3],
                               src[k + 4], src[k + 5], src[k + 6], src[k + 7],
                               dst[out], dst[out + 2], dst[out + 4], dst[out + 6],
                               dst[out + 8], dst[out + 10], dst[out + 12], dst[out + 14]);
        enc_clean_ctx_x8(&ctx_x8);
        ERR(ret, err);

        for (b = 0; b < 8; b++) {
            xor_blocks(dst[out + 2 * b], src[k + b], dst[out + 2 * b + 1]);
        }
    }
#else
    k = 0;
#endif

    for (; k + 4 <= src_count; k += 4) {
        uint8_t ts0[MQOM3_PARAM_SALT_SIZE], ts1[MQOM3_PARAM_SALT_SIZE];
        uint8_t ts2[MQOM3_PARAM_SALT_SIZE], ts3[MQOM3_PARAM_SALT_SIZE];
        enc_ctx_x4 DECL_VAR(ctx_x4);
        uint32_t out = 2 * k;

        TweakSalt(salt, ts0, SALT_SEL_GGM, base + k);
        TweakSalt(salt, ts1, SALT_SEL_GGM, base + k + 1);
        TweakSalt(salt, ts2, SALT_SEL_GGM, base + k + 2);
        TweakSalt(salt, ts3, SALT_SEL_GGM, base + k + 3);

        ret = enc_key_sched_x4(&ctx_x4, ts0, ts1, ts2, ts3);
        if (ret) { enc_clean_ctx_x4(&ctx_x4); goto err; }

        ret = SeedDerive_x4_x4(&ctx_x4,
                                src[k],     src[k + 1], src[k + 2], src[k + 3],
                                dst[out],   dst[out + 2], dst[out + 4], dst[out + 6]);
        enc_clean_ctx_x4(&ctx_x4);
        ERR(ret, err);

        xor_blocks(dst[out],     src[k],     dst[out + 1]);
        xor_blocks(dst[out + 2], src[k + 1], dst[out + 3]);
        xor_blocks(dst[out + 4], src[k + 2], dst[out + 5]);
        xor_blocks(dst[out + 6], src[k + 3], dst[out + 7]);
    }

    for (; k < src_count; k++) {
        uint8_t ts[MQOM3_PARAM_SALT_SIZE];
        enc_ctx DECL_VAR(ctx_s);
        uint32_t out = 2 * k;

        TweakSalt(salt, ts, SALT_SEL_GGM, base + k);
        ret = enc_key_sched(&ctx_s, ts);
        if (ret) { enc_clean_ctx(&ctx_s); goto err; }

        ret = SeedDerive(&ctx_s, src[k], dst[out]);
        enc_clean_ctx(&ctx_s);
        ERR(ret, err);

        xor_blocks(dst[out], src[k], dst[out + 1]);
    }

    return 0;
err:
    return ret;
}

/* Full-tree BFS: fills node[1..LARGE_GGM_TREE_SIZE-1] and lseed[].
 *
 * Works entirely in-place inside node[]: at level j, src = &node[2^j] and
 * dst = &node[2^(j+1)].  The src and dst ranges are always disjoint, so
 * there is no aliasing.  No extra allocation is needed.
 *
 * After the BFS, all node[] entries are valid and lseed[i] is extracted via
 * LeafPosition(i) -- identical semantics to LargeGGMTree_Expand. */
int LargeGGMTree_Expand_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE],
        uint8_t node[LARGE_GGM_TREE_SIZE][MQOM3_PARAM_SEED_SIZE],
        uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE])
{
    int ret;
    uint32_t j, i;
    uint32_t level_size = 1;

    memcpy(node[1], mseed, MQOM3_PARAM_SEED_SIZE);

    /* Levels 0..H-2: all 2^j nodes are internal; expand all. */
    for (j = 0; j < (uint32_t)(LARGE_GGM_H - 1); j++) {
        uint32_t base = 1U << j;
        ret = bfs_derive_level(salt, base, node + base, level_size, node + (2 * base));
        if (ret) { return ret; }
        level_size *= 2;
    }

    /* Level H-1: only BFS_NB_INT_H1 nodes (heap indices BFS_MAX_LEVEL_NODES ..
     * BFS_MAX_LEVEL_NODES + BFS_NB_INT_H1 - 1) are internal; the rest are
     * shallow leaves already written by the previous iteration. */
    ret = bfs_derive_level(salt, (uint32_t)BFS_MAX_LEVEL_NODES,
                           node + (uint32_t)BFS_MAX_LEVEL_NODES, (uint32_t)BFS_NB_INT_H1,
                           node + (2u * BFS_MAX_LEVEL_NODES));
    if (ret) { return ret; }

    /* Extract logical leaf seeds (same as LargeGGMTree_Expand). */
    for (i = 0; i < (uint32_t)LARGE_GGM_TREE_NB_LEAVES; i++) {
        memcpy(lseed[i], node[LeafPosition(i)], MQOM3_PARAM_SEED_SIZE);
    }
    return 0;
}

/* Leaves-only BFS: computes lseed[] without storing internal nodes.
 *
 * Uses two rolling heap-level buffers (cur/nxt, each BFS_MAX_LEVEL_NODES seeds).
 * These can reach 256 KB for cat5, so they are allocated via mqom_malloc rather
 * than placed on the stack.  The deep-leaf scratch buffer is larger still (up to
 * 1.5 MB for cat5-gf2-shorter-ot) and is allocated the same way. */
int LargeGGMTree_ExpandLeavesOnly_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE],
        uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE])
{
    int ret = -1;
    uint32_t j, ki, i;

    /* Allocate the two rolling level buffers; keep original pointers for freeing. */
    const size_t level_buf_size = (size_t)BFS_MAX_LEVEL_NODES * MQOM3_PARAM_SEED_SIZE;
    const size_t deep_buf_size = (size_t)LARGE_GGM_NB_DEEP_LEAVES * MQOM3_PARAM_SEED_SIZE;
    uint8_t (*buf_a)[MQOM3_PARAM_SEED_SIZE] = mqom_malloc(level_buf_size);
    uint8_t (*buf_b)[MQOM3_PARAM_SEED_SIZE] = mqom_malloc(level_buf_size);
    uint8_t (*deep)[MQOM3_PARAM_SEED_SIZE] = mqom_malloc(deep_buf_size);

    if (!buf_a || !buf_b || !deep) { goto err; }

    uint8_t (*cur)[MQOM3_PARAM_SEED_SIZE] = buf_a;
    uint8_t (*nxt)[MQOM3_PARAM_SEED_SIZE] = buf_b;
    uint32_t level_size = 1;

    memcpy(cur[0], mseed, MQOM3_PARAM_SEED_SIZE);

    /* Expand levels 0..H-2 into the rolling buffers. */
    for (j = 0; j < (uint32_t)(LARGE_GGM_H - 1); j++) {
        uint32_t base = 1U << j;
        ret = bfs_derive_level(salt, base, cur, level_size, nxt);
        ERR(ret, err);

        uint8_t (*tmp)[MQOM3_PARAM_SEED_SIZE] = cur;
        cur = nxt;
        nxt = tmp;
        level_size *= 2;
    }
    /* cur now holds BFS_MAX_LEVEL_NODES seeds for level H-1. */

    /* Shallow leaves (depth H-1): cur[BFS_NB_INT_H1 .. BFS_MAX_LEVEL_NODES-1].
     * Logical index: ki + BFS_NB_INT_H1 (first NB_DEEP lseed slots are deep). */
    for (ki = (uint32_t)BFS_NB_INT_H1; ki < (uint32_t)BFS_MAX_LEVEL_NODES; ki++) {
        memcpy(lseed[ki + (uint32_t)BFS_NB_INT_H1], cur[ki], MQOM3_PARAM_SEED_SIZE);
    }

    /* Deep leaves (depth H): expand the BFS_NB_INT_H1 internal nodes at level H-1. */
    ret = bfs_derive_level(salt, (uint32_t)BFS_MAX_LEVEL_NODES,
                           cur, (uint32_t)BFS_NB_INT_H1, deep);
    ERR(ret, err);

    for (i = 0; i < (uint32_t)LARGE_GGM_NB_DEEP_LEAVES; i++) {
        memcpy(lseed[i], deep[i], MQOM3_PARAM_SEED_SIZE);
    }

    ret = 0;
err:
    if (deep) {
        /* deep_buf_size, not sizeof(deep): deep is now a pointer. */
        mqom_cleanse(deep, deep_buf_size);
        mqom_free(deep, deep_buf_size);
    }
    if (buf_a) {
        mqom_cleanse(buf_a, level_buf_size);
        mqom_free(buf_a, level_buf_size);
    }
    if (buf_b) {
        mqom_cleanse(buf_b, level_buf_size);
        mqom_free(buf_b, level_buf_size);
    }
    return ret;
}

/* Derive children of all assigned nodes at one BFS level.
 * base: heap index of the first node in this level (e.g. 2^j).
 * count: number of INTERNAL nodes at this level to scan.
 * assigned[k] != 0 means node[k] holds a valid seed that must be expanded.
 * x4 batches four independent parents; scalar tail handles 0-3 remainders.
 * After return, assigned[2k] and assigned[2k+1] are set for each processed k. */
static int bfs_partially_expand_level(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        uint32_t base,
        uint32_t count,
        uint8_t (*node)[MQOM3_PARAM_SEED_SIZE],
        uint8_t *assigned)
{
    uint32_t ki, b;
#ifdef LARGE_TREE_BFS_X8
    uint32_t batch[8];
    const uint32_t batch_width = 8;
#else
    uint32_t batch[4];
    const uint32_t batch_width = 4;
#endif
    uint32_t batch_n = 0;

    for (ki = 0; ki < count; ki++) {
        uint32_t k = base + ki;
        if (!assigned[k]) { continue; }
        batch[batch_n++] = k;
        if (batch_n < batch_width) { continue; }

#ifdef LARGE_TREE_BFS_X8
        /* Flush x8 batch: eight independent parents -> sixteen child seeds */
        {
            uint8_t ts[8][MQOM3_PARAM_SALT_SIZE];
            enc_ctx_pub_x8 DECL_VAR(ctx_x8);
            int ret;

            for (b = 0; b < 8; b++) {
                TweakSalt(salt, ts[b], SALT_SEL_GGM, batch[b]);
            }

            ret = enc_key_sched_pub_x8(&ctx_x8,
                                   ts[0], ts[1], ts[2], ts[3],
                                   ts[4], ts[5], ts[6], ts[7]);
            if (ret) { enc_clean_ctx_pub_x8(&ctx_x8); return ret; }

            ret = SeedDerive_x8_x8_pub(&ctx_x8,
                    node[batch[0]], node[batch[1]], node[batch[2]], node[batch[3]],
                    node[batch[4]], node[batch[5]], node[batch[6]], node[batch[7]],
                    node[2*batch[0]], node[2*batch[1]], node[2*batch[2]], node[2*batch[3]],
                    node[2*batch[4]], node[2*batch[5]], node[2*batch[6]], node[2*batch[7]]);
            enc_clean_ctx_pub_x8(&ctx_x8);
            if (ret) { return ret; }

            for (b = 0; b < 8; b++) {
                xor_blocks(node[2*batch[b]], node[batch[b]], node[2*batch[b]+1]);
                assigned[2*batch[b]] = assigned[2*batch[b]+1] = 1;
            }
        }
#else
        /* Flush x4 batch: four independent parents -> eight child seeds */
        {
            uint8_t ts0[MQOM3_PARAM_SALT_SIZE], ts1[MQOM3_PARAM_SALT_SIZE];
            uint8_t ts2[MQOM3_PARAM_SALT_SIZE], ts3[MQOM3_PARAM_SALT_SIZE];
            enc_ctx_pub_x4 DECL_VAR(ctx_x4);
            int ret;

            TweakSalt(salt, ts0, SALT_SEL_GGM, batch[0]);
            TweakSalt(salt, ts1, SALT_SEL_GGM, batch[1]);
            TweakSalt(salt, ts2, SALT_SEL_GGM, batch[2]);
            TweakSalt(salt, ts3, SALT_SEL_GGM, batch[3]);

            ret = enc_key_sched_pub_x4(&ctx_x4, ts0, ts1, ts2, ts3);
            if (ret) { enc_clean_ctx_pub_x4(&ctx_x4); return ret; }

            /* Derive left children of all four parents simultaneously */
            ret = SeedDerive_x4_x4_pub(&ctx_x4,
                    node[batch[0]], node[batch[1]], node[batch[2]], node[batch[3]],
                    node[2*batch[0]], node[2*batch[1]], node[2*batch[2]], node[2*batch[3]]);
            enc_clean_ctx_pub_x4(&ctx_x4);
            if (ret) { return ret; }

            /* Right child = left child XOR parent */
            xor_blocks(node[2*batch[0]], node[batch[0]], node[2*batch[0]+1]);
            xor_blocks(node[2*batch[1]], node[batch[1]], node[2*batch[1]+1]);
            xor_blocks(node[2*batch[2]], node[batch[2]], node[2*batch[2]+1]);
            xor_blocks(node[2*batch[3]], node[batch[3]], node[2*batch[3]+1]);

            assigned[2*batch[0]]   = assigned[2*batch[0]+1]   = 1;
            assigned[2*batch[1]]   = assigned[2*batch[1]+1]   = 1;
            assigned[2*batch[2]]   = assigned[2*batch[2]+1]   = 1;
            assigned[2*batch[3]]   = assigned[2*batch[3]+1]   = 1;
        }
#endif
        batch_n = 0;
    }

    /* Preserve the x4 path for a 4-7 node x8 tail. */
#ifdef LARGE_TREE_BFS_X8
    if (batch_n >= 4) {
        uint8_t ts0[MQOM3_PARAM_SALT_SIZE], ts1[MQOM3_PARAM_SALT_SIZE];
        uint8_t ts2[MQOM3_PARAM_SALT_SIZE], ts3[MQOM3_PARAM_SALT_SIZE];
        enc_ctx_pub_x4 DECL_VAR(ctx_x4);
        int ret;

        TweakSalt(salt, ts0, SALT_SEL_GGM, batch[0]);
        TweakSalt(salt, ts1, SALT_SEL_GGM, batch[1]);
        TweakSalt(salt, ts2, SALT_SEL_GGM, batch[2]);
        TweakSalt(salt, ts3, SALT_SEL_GGM, batch[3]);
        ret = enc_key_sched_pub_x4(&ctx_x4, ts0, ts1, ts2, ts3);
        if (ret) { enc_clean_ctx_pub_x4(&ctx_x4); return ret; }
        ret = SeedDerive_x4_x4_pub(&ctx_x4,
                node[batch[0]], node[batch[1]], node[batch[2]], node[batch[3]],
                node[2*batch[0]], node[2*batch[1]], node[2*batch[2]], node[2*batch[3]]);
        enc_clean_ctx_pub_x4(&ctx_x4);
        if (ret) { return ret; }
        for (b = 0; b < 4; b++) {
            xor_blocks(node[2*batch[b]], node[batch[b]], node[2*batch[b]+1]);
            assigned[2*batch[b]] = assigned[2*batch[b]+1] = 1;
        }
        for (b = 4; b < batch_n; b++) {
            batch[b - 4] = batch[b];
        }
        batch_n -= 4;
    }
#endif

    /* Scalar tail: 0-3 remaining assigned nodes. */
    for (b = 0; b < batch_n; b++) {
        uint32_t k = batch[b];
        uint8_t ts[MQOM3_PARAM_SALT_SIZE];
        enc_ctx_pub DECL_VAR(ctx_s);
        int ret;

        TweakSalt(salt, ts, SALT_SEL_GGM, k);
        ret = enc_key_sched_pub(&ctx_s, ts);
        if (ret) { enc_clean_ctx_pub(&ctx_s); return ret; }
        ret = SeedDerive_pub(&ctx_s, node[k], node[2*k]);
        enc_clean_ctx_pub(&ctx_s);
        if (ret) { return ret; }
        xor_blocks(node[2*k], node[k], node[2*k+1]);
        assigned[2*k] = assigned[2*k+1] = 1;
    }
    return 0;
}

/* Open (no-keep path, BFS): re-expand the full tree from mseed using x4 AES batching,
 * then copy revealed node seeds into path and hidden leaf seeds into hidden_lseeds.
 * Allocates node[] on the heap (same size as BLC_KEEP_ALL_TREES_IN_MEMORY, but
 * freed before returning).  Drop-in replacement for LargeGGMTree_ExpandPath. */
int LargeGGMTree_ExpandPath_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t mseed[MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU],
        uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        uint8_t hidden_lseeds[MQOM3_PARAM_TAU][MQOM3_PARAM_SEED_SIZE])
{
    const size_t node_sz = (size_t)LARGE_GGM_TREE_SIZE * MQOM3_PARAM_SEED_SIZE;
    uint8_t (*node)[MQOM3_PARAM_SEED_SIZE] = mqom_malloc(node_sz);
    uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
    uint32_t path_node_idxs[MQOM3_PARAM_LARGE_GGM_T_OPEN];
    uint32_t nb_hidden_nodes, nb_path_nodes, pos;
    uint32_t j, level_size;
    int e, ret = -1;

    if (!node) { goto err; }

    /* BFS full expansion: level-by-level x4 AES batching */
    memcpy(node[1], mseed, MQOM3_PARAM_SEED_SIZE);
    level_size = 1;
    for (j = 0; j < (uint32_t)(LARGE_GGM_H - 1); j++) {
        uint32_t base = 1U << j;
        ret = bfs_derive_level(salt, base, node + base, level_size, node + (2 * base));
        ERR(ret, err);
        level_size *= 2;
    }
    ret = bfs_derive_level(salt, (uint32_t)BFS_MAX_LEVEL_NODES,
                           node + (uint32_t)BFS_MAX_LEVEL_NODES, (uint32_t)BFS_NB_INT_H1,
                           node + (2u * BFS_MAX_LEVEL_NODES));
    ERR(ret, err);

    /* Compute path node indices (same DFS order as verifier PartiallyExpand) */
    GetSensitiveNodeIndexes(hidden_leaf_idxs, hidden_nodes, &nb_hidden_nodes);
    ret = GetNodeIndexesInPath(hidden_nodes, nb_hidden_nodes, path_node_idxs, &nb_path_nodes);
    ERR(ret, err);

    /* Copy revealed path seeds */
    for (pos = 0; pos < nb_path_nodes; pos++) {
        memcpy(path + (pos * MQOM3_PARAM_SEED_SIZE), node[path_node_idxs[pos]], MQOM3_PARAM_SEED_SIZE);
    }

    /* Copy hidden leaf seeds (needed by no-keep path to recompute commitments) */
    for (e = 0; e < MQOM3_PARAM_TAU; e++) {
        memcpy(hidden_lseeds[e], node[LeafPosition(hidden_leaf_idxs[e])], MQOM3_PARAM_SEED_SIZE);
    }

    /* Sanity check */
    if (nb_path_nodes != MQOM3_PARAM_LARGE_GGM_T_OPEN) {
        ret = -1;
        goto err;
    }

    ret = 0;
err:
    if (node) {
        mqom_cleanse(node, node_sz);
        mqom_free(node, node_sz);
    }
    return ret;
}

/* Partial expand (verifier, BFS): recover all non-hidden leaf seeds from the opening path.
 * Allocates node[] + assigned[] on the heap; fills node[] at path positions, then sweeps
 * level-by-level using bfs_partially_expand_level (x4 AES batching).
 * Hidden leaf slots in lseed[] are zeroed; non-hidden slots receive the derived seed.
 * Drop-in replacement for LargeGGMTree_PartiallyExpand. */
int LargeGGMTree_PartiallyExpand_bfs(
        const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
        const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE],
        const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU],
        uint8_t lseed[MQOM3_PARAM_TAU * MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE])
{
    /* Single allocation: node[] followed by assigned[] byte flags (1-indexed, index 0 unused) */
    const size_t node_sz  = (size_t)LARGE_GGM_TREE_SIZE * MQOM3_PARAM_SEED_SIZE;
    const size_t total_sz = node_sz + (size_t)LARGE_GGM_TREE_SIZE;
    uint8_t *buf = mqom_malloc(total_sz);
    uint32_t hidden_nodes[MQOM3_PARAM_TAU * (MQOM3_PARAM_LARGE_GGM_H + 1)];
    uint32_t path_node_idxs[MQOM3_PARAM_LARGE_GGM_T_OPEN];
    uint32_t nb_hidden_nodes, nb_path_nodes, pos;
    uint32_t i, j, level_size;
    int ret = -1;

    if (!buf) { goto err; }

    uint8_t (*node)[MQOM3_PARAM_SEED_SIZE] = (uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) buf;
    uint8_t *assigned = buf + node_sz;
    memset(assigned, 0, (size_t)LARGE_GGM_TREE_SIZE);

    /* Compute path node indices and sanity-check the opening budget */
    GetSensitiveNodeIndexes(hidden_leaf_idxs, hidden_nodes, &nb_hidden_nodes);
    ret = GetNodeIndexesInPath(hidden_nodes, nb_hidden_nodes, path_node_idxs, &nb_path_nodes);
    ERR(ret, err);

    /* Seed node[] at each path position and mark it assigned */
    for (pos = 0; pos < nb_path_nodes; pos++) {
        uint32_t k = path_node_idxs[pos];
        memcpy(node[k], path + (pos * MQOM3_PARAM_SEED_SIZE), MQOM3_PARAM_SEED_SIZE);
        assigned[k] = 1;
    }

    /* BFS forward sweep: propagate assigned seeds level by level with x4 AES batching.
     * At each level, bfs_partially_expand_level derives children of all assigned nodes
     * and marks those children assigned in turn. */
    level_size = 1;
    for (j = 0; j < (uint32_t)(LARGE_GGM_H - 1); j++) {
        uint32_t base = 1U << j;
        ret = bfs_partially_expand_level(salt, base, level_size, node, assigned);
        ERR(ret, err);
        level_size *= 2;
    }
    /* Level H-1: only BFS_NB_INT_H1 internal nodes; shallow leaves are already in node[] */
    ret = bfs_partially_expand_level(salt, (uint32_t)BFS_MAX_LEVEL_NODES,
                                     (uint32_t)BFS_NB_INT_H1, node, assigned);
    ERR(ret, err);

    /* Extract leaf seeds: assigned -> copy derived seed; unassigned (hidden) -> zero */
    for (i = 0; i < (uint32_t)LARGE_GGM_TREE_NB_LEAVES; i++) {
        uint32_t k = LeafPosition(i);
        if (assigned[k]) {
            memcpy(lseed[i], node[k], MQOM3_PARAM_SEED_SIZE);
        } else {
            memset(lseed[i], 0, MQOM3_PARAM_SEED_SIZE);
        }
    }

    ret = 0;
err:
    if (buf) {
        mqom_cleanse(buf, total_sz);
        mqom_free(buf, total_sz);
    }
    return ret;
}

#endif /* LARGE_TREE_BFS */

/* Avoid an empty translation unit */
typedef int mqom_ggm_tree_large_bfs_c_not_empty;

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int ggm_tree_large_bfs_unused_in_ct_build;
#endif
