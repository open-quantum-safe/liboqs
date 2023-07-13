#include "hss_zeroize.h"
#include <string.h>

/*
 * This is a function to zeroize a section of memory
 *
 * We do this because when we release a section of memory (either because it's
 * a local variable going out of scope, or we free it), it's possible that
 * the memory will retain its contents after another allocation (possibly
 * done by someone outside this module).  So, to avoid this potential security
 * issue, we scrub the memory (at least, the parts that have data that would
 * make it possible to forge if it leaked) before releasing it.
 *
 * Now, there's a bunch of things we don't mind being exposed (e.g. internal
 * node values of Merkle trees), so we don't use this everywhere; only where
 * it is needed
 *
 * We use this, rather than having routines simply call memset, to avoid
 * potential problems with overenthusiastic optimizers.  Generally, we zeroize
 * an area immediately before it goes out of scope or we free it, however an
 * optimizer might conclude "they're about to release the memory, there's no
 * need to write to it first"
 *
 * For similar reasons, this function is in its own source file (so that a
 * compiler optimizer who doesn't examine more than one source at a time can't
 * eliminate it).  If we are worried about optimizers who can be even more
 * enthusiastic, there are other things we can try; however we're not going to
 * worry about that right now
 */
void hss_zeroize( void *area, size_t len ) {
#if defined( __STDC_LIB_EXT1__ )
    /*
     * C11 defines a version of memset that does precisely what we want, and is
     * guaranteed not to be molested by the optimizer
     * Note that the first 'len' is supposed to be the length of the buffer
     * we're cleaning and the second 'len' is the area to clear.  Since we
     * expect the caller to ask us to clear the entire area (and hence gives
     * us only one length), we use the same for both
     */
    memset_s( area, len, 0, len );
#else
    /*
     * Fallback code for pre-C11 versions
     */
    volatile unsigned char *p = area;

    while (len--) *p++ = 0;
#endif
}
