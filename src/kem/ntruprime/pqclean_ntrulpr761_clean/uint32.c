#include <stdint.h>

#include "uint32.h"

/*
CPU division instruction typically takes time depending on x.
This software is designed to take time independent of x.
Time still varies depending on m; user must ensure that m is constant.
Time also varies on CPUs where multiplication is variable-time.
There could be more CPU issues.
There could also be compiler issues.
*/

void PQCLEAN_NTRULPR761_CLEAN_uint32_divmod_uint14(uint32_t *qq, uint16_t *r, uint32_t x, uint16_t m) {
    uint32_t v = 0x80000000;
    uint32_t qpart;
    uint32_t mask;

    v /= m;

    /* caller guarantees m > 0 */
    /* caller guarantees m < 16384 */
    /* vm <= 2^31 <= vm+m-1 */
    /* xvm <= 2^31 x <= xvm+x(m-1) */

    *qq = 0;

    qpart = (x * (uint64_t)v) >> 31;
    /* 2^31 qpart <= xv <= 2^31 qpart + 2^31-1 */
    /* 2^31 qpart m <= xvm <= 2^31 qpart m + (2^31-1)m */
    /* 2^31 qpart m <= 2^31 x <= 2^31 qpart m + (2^31-1)m + x(m-1) */
    /* 0 <= 2^31 newx <= (2^31-1)m + x(m-1) */
    /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
    /* 0 <= newx <= (1-1/2^31)(2^14-1) + (2^32-1)((2^14-1)-1)/2^31 */

    x -= qpart * m;
    *qq += qpart;
    /* x <= 49146 */

    qpart = (x * (uint64_t)v) >> 31;
    /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
    /* 0 <= newx <= m + 49146(2^14-1)/2^31 */
    /* 0 <= newx <= m + 0.4 */
    /* 0 <= newx <= m */

    x -= qpart * m;
    *qq += qpart;
    /* x <= m */

    x -= m;
    *qq += 1;
    mask = ~(x >> 31) + 1; //two two's complement
    x += mask & (uint32_t)m;
    *qq += mask;
    /* x < m */

    *r = (uint16_t) x;
}

uint32_t PQCLEAN_NTRULPR761_CLEAN_uint32_div_uint14(uint32_t x, uint16_t m) {
    uint32_t qq;
    uint16_t r;
    PQCLEAN_NTRULPR761_CLEAN_uint32_divmod_uint14(&qq, &r, x, m);
    return qq;
}

uint16_t PQCLEAN_NTRULPR761_CLEAN_uint32_mod_uint14(uint32_t x, uint16_t m) {
    uint32_t qq;
    uint16_t r;
    PQCLEAN_NTRULPR761_CLEAN_uint32_divmod_uint14(&qq, &r, x, m);
    return r;
}
