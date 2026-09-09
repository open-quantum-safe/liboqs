#ifndef __SEED_COMMIT_DEFAULT_H__
#define __SEED_COMMIT_DEFAULT_H__

#include "enc.h"
#include "common.h"
#include "prg.h"

/* Pass 1: non-pub variants (SeedCommit, SeedCommit_x2, SeedCommit_x4_x4, SeedCommit_x4_ecb) */
#include "seed_commit_impl.h"

/* Pass 2: pub variants (SeedCommit_pub, SeedCommit_pub_x2, SeedCommit_x4_x4_pub, SeedCommit_x4_pub_ecb) */
#define SEED_COMMIT_PUBLIC
#include "seed_commit_impl.h"
#undef SEED_COMMIT_PUBLIC

#endif /* __SEED_COMMIT_DEFAULT_H__ */
