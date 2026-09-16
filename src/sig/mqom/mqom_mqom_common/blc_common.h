#ifndef __BLC_COMMON_H__
#define __BLC_COMMON_H__

#include <string.h>
#include "prg.h"
#include "fields.h"

/* Genuinely shared between CT and OT default paths (both size their
 * SeedExpand ECB cache with it). */
#define PRG_BLC_SIZE (BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA) - MQOM3_PARAM_SEED_SIZE)

/* Third source of v for TweakSalt (sel = 3): SeedExpand tweaks each output block
 * with IndexIdentifier(e, block_index). The block count follows from
 * PRG_BLC_SIZE, which is why this bound lives here and not next to the other two
 * in common.h (reached from here through fields.h). See the MQOM3_TWEAK_MAX_V
 * comment there for what silently breaks past the bound. */
#define BLC_SEEDEXPAND_MAX_BLK ((PRG_BLC_SIZE + MQOM3_PARAM_SEED_SIZE - 1) / MQOM3_PARAM_SEED_SIZE)
#if ((MQOM3_PARAM_TAU - 1) + (64 * BLC_SEEDEXPAND_MAX_BLK)) > MQOM3_TWEAK_MAX_V
#error "TweakSalt: SeedExpand block index exceeds the tweak domain"
#endif

#endif /* __BLC_COMMON_H__ */
