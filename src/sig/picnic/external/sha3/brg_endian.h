#ifndef _BRG_ENDIAN_H
#define _BRG_ENDIAN_H

#include "../endian_compat.h"

#if defined(PICNIC_IS_LITTLE_ENDIAN)
#define IS_LITTLE_ENDIAN 1
#define IS_BIG_ENDIAN 0
#else
#define IS_LITTLE_ENDIAN 0
#define IS_BIG_ENDIAN 1
#endif
#define PLATFORM_BYTE_ORDER 1

#define HTOLE64(x) htole64((x))

#endif
