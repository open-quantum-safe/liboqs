#include "crypto_uint32.h"

crypto_uint32_signed crypto_uint32_signed_negative_mask(crypto_uint32_signed crypto_uint32_signed_x) {
    return crypto_uint32_signed_x >> 31;
}

crypto_uint32 crypto_uint32_nonzero_mask(crypto_uint32 crypto_uint32_x) {
    return crypto_uint32_signed_negative_mask(crypto_uint32_x) | crypto_uint32_signed_negative_mask(-crypto_uint32_x);
}

crypto_uint32 crypto_uint32_zero_mask(crypto_uint32 crypto_uint32_x) {
    return ~crypto_uint32_nonzero_mask(crypto_uint32_x);
}

crypto_uint32 crypto_uint32_unequal_mask(crypto_uint32 crypto_uint32_x, crypto_uint32 crypto_uint32_y) {
    crypto_uint32 crypto_uint32_xy = crypto_uint32_x ^ crypto_uint32_y;
    return crypto_uint32_nonzero_mask(crypto_uint32_xy);
}

crypto_uint32 crypto_uint32_equal_mask(crypto_uint32 crypto_uint32_x, crypto_uint32 crypto_uint32_y) {
    return ~crypto_uint32_unequal_mask(crypto_uint32_x, crypto_uint32_y);
}

crypto_uint32 crypto_uint32_smaller_mask(crypto_uint32 crypto_uint32_x, crypto_uint32 crypto_uint32_y) {
    crypto_uint32 crypto_uint32_xy = crypto_uint32_x ^ crypto_uint32_y;
    crypto_uint32 crypto_uint32_z = crypto_uint32_x - crypto_uint32_y;
    crypto_uint32_z ^= crypto_uint32_xy & (crypto_uint32_z ^ crypto_uint32_x ^ (((crypto_uint32) 1) << 31));
    return crypto_uint32_signed_negative_mask(crypto_uint32_z);
}

crypto_uint32 crypto_uint32_min(crypto_uint32 crypto_uint32_x, crypto_uint32 crypto_uint32_y) {
    crypto_uint32 crypto_uint32_xy = crypto_uint32_y ^ crypto_uint32_x;
    crypto_uint32 crypto_uint32_z = crypto_uint32_y - crypto_uint32_x;
    crypto_uint32_z ^= crypto_uint32_xy & (crypto_uint32_z ^ crypto_uint32_y ^ (((crypto_uint32) 1) << 31));
    crypto_uint32_z = crypto_uint32_signed_negative_mask(crypto_uint32_z);
    crypto_uint32_z &= crypto_uint32_xy;
    return crypto_uint32_x ^ crypto_uint32_z;
}

crypto_uint32 crypto_uint32_max(crypto_uint32 crypto_uint32_x, crypto_uint32 crypto_uint32_y) {
    crypto_uint32 crypto_uint32_xy = crypto_uint32_y ^ crypto_uint32_x;
    crypto_uint32 crypto_uint32_z = crypto_uint32_y - crypto_uint32_x;
    crypto_uint32_z ^= crypto_uint32_xy & (crypto_uint32_z ^ crypto_uint32_y ^ (((crypto_uint32) 1) << 31));
    crypto_uint32_z = crypto_uint32_signed_negative_mask(crypto_uint32_z);
    crypto_uint32_z &= crypto_uint32_xy;
    return crypto_uint32_y ^ crypto_uint32_z;
}

void crypto_uint32_minmax(crypto_uint32 *crypto_uint32_a, crypto_uint32 *crypto_uint32_b) {
    crypto_uint32 crypto_uint32_x = *crypto_uint32_a;
    crypto_uint32 crypto_uint32_y = *crypto_uint32_b;
    crypto_uint32 crypto_uint32_xy = crypto_uint32_y ^ crypto_uint32_x;
    crypto_uint32 crypto_uint32_z = crypto_uint32_y - crypto_uint32_x;
    crypto_uint32_z ^= crypto_uint32_xy & (crypto_uint32_z ^ crypto_uint32_y ^ (((crypto_uint32) 1) << 31));
    crypto_uint32_z = crypto_uint32_signed_negative_mask(crypto_uint32_z);
    crypto_uint32_z &= crypto_uint32_xy;
    *crypto_uint32_a = crypto_uint32_x ^ crypto_uint32_z;
    *crypto_uint32_b = crypto_uint32_y ^ crypto_uint32_z;
}
