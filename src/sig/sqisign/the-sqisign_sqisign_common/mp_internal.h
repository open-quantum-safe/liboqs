#ifndef MP_INTERNAL_H
#define MP_INTERNAL_H

#include <tutil.h>
#include <fp_constants.h>
#include <encoded_sizes.h>

struct ibz_t
{
    int bitlen;
    digit_t limbs[IBZ_NLIMBS];
};

#endif
