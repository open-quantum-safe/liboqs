// SPDX-License-Identifier: MIT
#if !defined( HSS_ZEROIZE_H_ )
#define HSS_ZEROIZE_H_

#include <stdlib.h>
#include "lms_namespace.h"

/* Zeroize an area, that is, scrub it from holding any potentially secret */
/* information */
void hss_zeroize( void *area, size_t len );

#endif /* HSS_ZEROIZE_H_ */
