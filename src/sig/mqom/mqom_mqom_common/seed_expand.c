#include "seed_expand.h"
#include "seed_expand_impl.h"     /* secret variant: SeedExpand_memopt, SeedExpand */
#define SEED_EXPAND_PUBLIC
#include "seed_expand_impl.h"     /* public  variant: SeedExpand_memopt_pub, SeedExpand_pub */

#include "seed_expand_xn_impl.h"  /* secret: SeedExpand_x2, _x4, _x8, _x8_ecb */
#define SEED_EXPAND_PUBLIC
#include "seed_expand_xn_impl.h"  /* public: SeedExpand_x2_pub, _x4_pub, _x8_pub, _x8_pub_ecb */
