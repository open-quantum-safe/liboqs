#if !defined( HSS_DERIVE_H_ )
#define HSS_DERIVE_H_

#include "common_defs.h"

#include "config.h"

#if SECRET_MAX > 31
#error The code is not designed for a SECRET_MAX that high
#endif
#define SECRET_MAX_MASK (((merkle_index_t)1 << SECRET_MAX) - 1)

struct seed_derive {
    const unsigned char *I;
    const unsigned char *master_seed;
    merkle_index_t q;
    unsigned j;
#if SECRET_METHOD == 2
    unsigned hash;  /* Hash function to use */
    unsigned m;     /* Length of hash function */
#endif

#if SECRET_METHOD == 1
    unsigned q_levels, j_levels;
    merkle_index_t r_mask;
    unsigned j_mask;
#define MAX_Q_HEIGHT ((MAX_MERKLE_HEIGHT + SECRET_MAX - 1) / SECRET_MAX)
#define MAX_J_HEIGHT ((                9 + SECRET_MAX - 1) / SECRET_MAX)
        /* '9' is the number of bits a maximum 'p' can take up */

    unsigned j_value[MAX_J_HEIGHT];  /* these are the values we insert */
        /* into the hash.  The lower SECRET_MAX bits are which child of */
        /* the parent it is; the higher bits indicate the parents' */
        /* identities */

    unsigned char q_seed[MAX_Q_HEIGHT][SEED_LEN];
    unsigned char j_seed[MAX_Q_HEIGHT][SEED_LEN];
#endif
};

bool hss_seed_derive_init( struct seed_derive *derive,
                 param_set_t lm, param_set_t ots,
                 const unsigned char *I, const unsigned char *seed );

/* This sets the internal 'q' value */
/* If we've already have a 'q' value set, it'll try to minimize the number */
/* of hashes done */
/* Once you've done that, you'll need to reset the 'h' */
void hss_seed_derive_set_q( struct seed_derive *derive, merkle_index_t q );

/* This sets the internal 'j' value */
void hss_seed_derive_set_j( struct seed_derive *derive, unsigned j );

#define NUM_ARTIFICIAL_SEEDS    3  /* 3 seeds are listed below */
    /* This is the j value used when we're deriving the seed value */
    /* for child Merkle trees */
#define SEED_CHILD_SEED         (~1)
    /* This is the j value used when we're deriving the I value */
    /* used; either in the context of the parent tree, or of this tree */
#define SEED_CHILD_I            (SEED_CHILD_SEED + 1)
    /* This is the j value used when we're asking for the randomizer C */
    /* for signing a message */
#define SEED_RANDOMIZER_INDEX   (~2)

/* This generates the current seed.  If increment_j is set, this will set */
/* up for the next j value */
void hss_seed_derive( unsigned char *seed, struct seed_derive *derive,
                      bool increment_j );

/* This needs to be called when we done with a seed_derive */
/* That structure contains keying data, this makes sure those are cleaned */
void hss_seed_derive_done( struct seed_derive *derive );

#endif /* HSS_DERIVE_H_ */
