#include "crypto_int16.h"

crypto_int16 crypto_int16_negative_mask(crypto_int16 crypto_int16_x) {
    return crypto_int16_x >> 15;
}

crypto_int16 crypto_int16_nonzero_mask(crypto_int16 crypto_int16_x) {
    return crypto_int16_negative_mask(crypto_int16_x) | crypto_int16_negative_mask(-crypto_int16_x);
}

crypto_int16 crypto_int16_zero_mask(crypto_int16 crypto_int16_x) {
    return ~crypto_int16_nonzero_mask(crypto_int16_x);
}

crypto_int16 crypto_int16_positive_mask(crypto_int16 crypto_int16_x) {
    crypto_int16 crypto_int16_z = -crypto_int16_x;
    crypto_int16_z ^= crypto_int16_x & crypto_int16_z;
    return crypto_int16_negative_mask(crypto_int16_z);
}

crypto_int16 crypto_int16_unequal_mask(crypto_int16 crypto_int16_x, crypto_int16 crypto_int16_y) {
    crypto_int16 crypto_int16_xy = crypto_int16_x ^ crypto_int16_y;
    return crypto_int16_nonzero_mask(crypto_int16_xy);
}

crypto_int16 crypto_int16_equal_mask(crypto_int16 crypto_int16_x, crypto_int16 crypto_int16_y) {
    return ~crypto_int16_unequal_mask(crypto_int16_x, crypto_int16_y);
}

crypto_int16 crypto_int16_smaller_mask(crypto_int16 crypto_int16_x, crypto_int16 crypto_int16_y) {
    crypto_int16 crypto_int16_xy = crypto_int16_x ^ crypto_int16_y;
    crypto_int16 crypto_int16_z = crypto_int16_x - crypto_int16_y;
    crypto_int16_z ^= crypto_int16_xy & (crypto_int16_z ^ crypto_int16_x);
    return crypto_int16_negative_mask(crypto_int16_z);
}

crypto_int16 crypto_int16_min(crypto_int16 crypto_int16_x, crypto_int16 crypto_int16_y) {
    crypto_int16 crypto_int16_xy = crypto_int16_y ^ crypto_int16_x;
    crypto_int16 crypto_int16_z = crypto_int16_y - crypto_int16_x;
    crypto_int16_z ^= crypto_int16_xy & (crypto_int16_z ^ crypto_int16_y);
    crypto_int16_z = crypto_int16_negative_mask(crypto_int16_z);
    crypto_int16_z &= crypto_int16_xy;
    return crypto_int16_x ^ crypto_int16_z;
}

crypto_int16 crypto_int16_max(crypto_int16 crypto_int16_x, crypto_int16 crypto_int16_y) {
    crypto_int16 crypto_int16_xy = crypto_int16_y ^ crypto_int16_x;
    crypto_int16 crypto_int16_z = crypto_int16_y - crypto_int16_x;
    crypto_int16_z ^= crypto_int16_xy & (crypto_int16_z ^ crypto_int16_y);
    crypto_int16_z = crypto_int16_negative_mask(crypto_int16_z);
    crypto_int16_z &= crypto_int16_xy;
    return crypto_int16_y ^ crypto_int16_z;
}

void crypto_int16_minmax(crypto_int16 *crypto_int16_a, crypto_int16 *crypto_int16_b) {
    crypto_int16 crypto_int16_x = *crypto_int16_a;
    crypto_int16 crypto_int16_y = *crypto_int16_b;
    crypto_int16 crypto_int16_xy = crypto_int16_y ^ crypto_int16_x;
    crypto_int16 crypto_int16_z = crypto_int16_y - crypto_int16_x;
    crypto_int16_z ^= crypto_int16_xy & (crypto_int16_z ^ crypto_int16_y);
    crypto_int16_z = crypto_int16_negative_mask(crypto_int16_z);
    crypto_int16_z &= crypto_int16_xy;
    *crypto_int16_a = crypto_int16_x ^ crypto_int16_z;
    *crypto_int16_b = crypto_int16_y ^ crypto_int16_z;
}
