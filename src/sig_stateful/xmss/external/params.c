#include <stdint.h>
#include <string.h>
#include <math.h>
#include "params.h"
#include "xmss_core.h"
#include "xmss_namespace.h"

int xmss_str_to_oid(uint32_t *oid, const char *s)
{
    if (!strcmp(s, "XMSS-SHA2_10_256")) {
        *oid = 0x00000001;
    }
    else if (!strcmp(s, "XMSS-SHA2_16_256")) {
        *oid = 0x00000002;
    }
    else if (!strcmp(s, "XMSS-SHA2_20_256")) {
        *oid = 0x00000003;
    }
    else if (!strcmp(s, "XMSS-SHA2_10_512")) {
        *oid = 0x00000004;
    }
    else if (!strcmp(s, "XMSS-SHA2_16_512")) {
        *oid = 0x00000005;
    }
    else if (!strcmp(s, "XMSS-SHA2_20_512")) {
        *oid = 0x00000006;
    }
    else if (!strcmp(s, "XMSS-SHAKE_10_256")) {
        *oid = 0x00000007;
    }
    else if (!strcmp(s, "XMSS-SHAKE_16_256")) {
        *oid = 0x00000008;
    }
    else if (!strcmp(s, "XMSS-SHAKE_20_256")) {
        *oid = 0x00000009;
    }
    else if (!strcmp(s, "XMSS-SHAKE_10_512")) {
        *oid = 0x0000000a;
    }
    else if (!strcmp(s, "XMSS-SHAKE_16_512")) {
        *oid = 0x0000000b;
    }
    else if (!strcmp(s, "XMSS-SHAKE_20_512")) {
        *oid = 0x0000000c;
    }
    else {
        return -1;
    }
    return 0;
}

int xmssmt_str_to_oid(uint32_t *oid, const char *s)
{
    if (!strcmp(s, "XMSSMT-SHA2_20/2_256")) {
        *oid = 0x00000001;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_20/4_256")) {
        *oid = 0x00000002;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/2_256")) {
        *oid = 0x00000003;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/4_256")) {
        *oid = 0x00000004;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/8_256")) {
        *oid = 0x00000005;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/3_256")) {
        *oid = 0x00000006;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/6_256")) {
        *oid = 0x00000007;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/12_256")) {
        *oid = 0x00000008;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_20/2_512")) {
        *oid = 0x00000009;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_20/4_512")) {
        *oid = 0x0000000a;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/2_512")) {
        *oid = 0x0000000b;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/4_512")) {
        *oid = 0x0000000c;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/8_512")) {
        *oid = 0x0000000d;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/3_512")) {
        *oid = 0x0000000e;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/6_512")) {
        *oid = 0x0000000f;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/12_512")) {
        *oid = 0x00000010;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_20/2_256")) {
        *oid = 0x00000011;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_20/4_256")) {
        *oid = 0x00000012;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_40/2_256")) {
        *oid = 0x00000013;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_40/4_256")) {
        *oid = 0x00000014;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_40/8_256")) {
        *oid = 0x00000015;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_60/3_256")) {
        *oid = 0x00000016;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_60/6_256")) {
        *oid = 0x00000017;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_60/12_256")) {
        *oid = 0x00000018;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_20/2_512")) {
        *oid = 0x00000019;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_20/4_512")) {
        *oid = 0x0000001a;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_40/2_512")) {
        *oid = 0x0000001b;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_40/4_512")) {
        *oid = 0x0000001c;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_40/8_512")) {
        *oid = 0x0000001d;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_60/3_512")) {
        *oid = 0x0000001e;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_60/6_512")) {
        *oid = 0x0000001f;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE_60/12_512")) {
        *oid = 0x00000020;
    }
    // These are custom parameter sets for this prototype and testing purposes;
    // they deviate from the RFC
    else if (!strcmp(s, "XMSSMT-SHA2_12/2_192")) {
        *oid = 0x00000031;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_12/3_192")) {
        *oid = 0x00000032;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_22/2_192")) {
        *oid = 0x00000033;
    }
    else {
        return -1;
    }
    return 0;
}

int xmss_parse_oid(xmss_params *params, const uint32_t oid)
{
    switch (oid) {
        case 0x00000001:
        case 0x00000002:
        case 0x00000003:
        case 0x00000004:
        case 0x00000005:
        case 0x00000006:
            params->func = XMSS_SHA2;
            break;

        case 0x00000007:
        case 0x00000008:
        case 0x00000009:
        case 0x0000000a:
        case 0x0000000b:
        case 0x0000000c:
            params->func = XMSS_SHAKE;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x00000001:
        case 0x00000002:
        case 0x00000003:

        case 0x00000007:
        case 0x00000008:
        case 0x00000009:
            params->n = 32;
            break;

        case 0x00000004:
        case 0x00000005:
        case 0x00000006:

        case 0x0000000a:
        case 0x0000000b:
        case 0x0000000c:
            params->n = 64;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x00000001:
        case 0x00000004:
        case 0x00000007:
        case 0x0000000a:
            params->full_height = 10;
            break;

        case 0x00000002:
        case 0x00000005:
        case 0x00000008:
        case 0x0000000b:
            params->full_height = 16;
            break;

        case 0x00000003:
        case 0x00000006:
        case 0x00000009:
        case 0x0000000c:
            params->full_height = 20;
            break;
        default:
            return -1;
    }

    params->d = 1;
    params->wots_w = 16;

    params->bds_k = 0;

    return xmss_xmssmt_initialize_params(params);
}

int xmssmt_parse_oid(xmss_params *params, const uint32_t oid)
{
    switch (oid) {
        case 0x00000001:
        case 0x00000002:
        case 0x00000003:
        case 0x00000004:
        case 0x00000005:
        case 0x00000006:
        case 0x00000007:
        case 0x00000008:
        case 0x00000009:
        case 0x0000000a:
        case 0x0000000b:
        case 0x0000000c:
        case 0x0000000d:
        case 0x0000000e:
        case 0x0000000f:
        case 0x00000010:

        // custom parameter sets
        case 0x00000031:
        case 0x00000032:
        case 0x00000033:
            params->func = XMSS_SHA2;
            break;

        case 0x00000011:
        case 0x00000012:
        case 0x00000013:
        case 0x00000014:
        case 0x00000015:
        case 0x00000016:
        case 0x00000017:
        case 0x00000018:
        case 0x00000019:
        case 0x0000001a:
        case 0x0000001b:
        case 0x0000001c:
        case 0x0000001e:
        case 0x0000001d:
        case 0x0000001f:
        case 0x00000020:
            params->func = XMSS_SHAKE;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x00000001:
        case 0x00000002:
        case 0x00000003:
        case 0x00000004:
        case 0x00000005:
        case 0x00000006:
        case 0x00000007:
        case 0x00000008:

        case 0x00000011:
        case 0x00000012:
        case 0x00000013:
        case 0x00000014:
        case 0x00000015:
        case 0x00000016:
        case 0x00000017:
        case 0x00000018:
            params->n = 32;
            break;
        // custom parameter sets
        case 0x00000031:
        case 0x00000032:
        case 0x00000033:
            params->n = 24;
            break;

        case 0x00000009:
        case 0x0000000a:
        case 0x0000000b:
        case 0x0000000c:
        case 0x0000000d:
        case 0x0000000e:
        case 0x0000000f:
        case 0x00000010:

        case 0x00000019:
        case 0x0000001a:
        case 0x0000001b:
        case 0x0000001c:
        case 0x0000001d:
        case 0x0000001e:
        case 0x0000001f:
        case 0x00000020:
            params->n = 64;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x00000001:
        case 0x00000002:

        case 0x00000009:
        case 0x0000000a:

        case 0x00000011:
        case 0x00000012:

        case 0x00000019:
        case 0x0000001a:
            params->full_height = 20;
            break;

        case 0x00000003:
        case 0x00000004:
        case 0x00000005:

        case 0x0000000b:
        case 0x0000000c:
        case 0x0000000d:

        case 0x00000013:
        case 0x00000014:
        case 0x00000015:

        case 0x0000001b:
        case 0x0000001c:
        case 0x0000001d:
            params->full_height = 40;
            break;

        case 0x00000006:
        case 0x00000007:
        case 0x00000008:

        case 0x0000000e:
        case 0x0000000f:
        case 0x00000010:

        case 0x00000016:
        case 0x00000017:
        case 0x00000018:

        case 0x0000001e:
        case 0x0000001f:
        case 0x00000020:
            params->full_height = 60;
            break;

        // custom parameter sets
        case 0x00000031:
        case 0x00000032:
            params->full_height = 12;
            break;
        case 0x00000033:
            params->full_height = 22;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x00000001:
        case 0x00000003:
        case 0x00000009:
        case 0x0000000b:
        case 0x00000011:
        case 0x00000013:
        case 0x00000019:
        case 0x0000001b:
            params->d = 2;
            break;

        case 0x00000002:
        case 0x00000004:
        case 0x0000000a:
        case 0x0000000c:
        case 0x00000012:
        case 0x00000014:
        case 0x0000001a:
        case 0x0000001c:
            params->d = 4;
            break;

        case 0x00000005:
        case 0x0000000d:
        case 0x00000015:
        case 0x0000001d:
            params->d = 8;
            break;

        case 0x00000006:
        case 0x0000000e:
        case 0x00000016:
        case 0x0000001e:
            params->d = 3;
            break;

        case 0x00000007:
        case 0x0000000f:
        case 0x00000017:
        case 0x0000001f:
            params->d = 6;
            break;

        case 0x00000008:
        case 0x00000010:
        case 0x00000018:
        case 0x00000020:
            params->d = 12;
            break;

        // custom parameter sets
        case 0x00000031:
            params->d = 2;
            break;
        case 0x00000032:
            params->d = 3;
            break;
        case 0x00000033:
            params->d = 2;
            break;

        default:
            return -1;
    }

    params->wots_w = 16;

    params->bds_k = 0;

    return xmss_xmssmt_initialize_params(params);
}

/**
 * @brief Get the no. of bytes necessary for the max field 
 * in the secret key byte array.
 * @param tree_height - height of the tree
 * @return unsigned int - the number of bytes for the max sigs.
 */
unsigned int get_bytes_for_max(unsigned int tree_height) {
    return (unsigned int)(pow(2, ceil(log2(tree_height))) / 8);
}


/**
 * Given a params struct where the following properties have been initialized;
 *  - full_height; the height of the complete (hyper)tree
 *  - n; the number of bytes of hash function output
 *  - d; the number of layers (d > 1 implies XMSSMT)
 *  - func; one of {XMSS_SHA2, XMSS_SHAKE}
 *  - wots_w; the Winternitz parameter
 *  - optionally, bds_k; the BDS traversal trade-off parameter,
 * this function initializes the remainder of the params structure.
 */
int xmss_xmssmt_initialize_params(xmss_params *params)
{
    params->tree_height = params->full_height  / params->d;
    params->bytes_for_max = get_bytes_for_max(params->full_height);
    if (params->wots_w == 4) {
        params->wots_log_w = 2;
        params->wots_len1 = 8 * params->n / params->wots_log_w;
        /* len_2 = floor(log(len_1 * (w - 1)) / log(w)) + 1 */
        params->wots_len2 = 5;
    }
    else if (params->wots_w == 16) {
        params->wots_log_w = 4;
        params->wots_len1 = 8 * params->n / params->wots_log_w;
        /* len_2 = floor(log(len_1 * (w - 1)) / log(w)) + 1 */
        params->wots_len2 = 3;
    }
    else if (params->wots_w == 256) {
        params->wots_log_w = 8;
        params->wots_len1 = 8 * params->n / params->wots_log_w;
        /* len_2 = floor(log(len_1 * (w - 1)) / log(w)) + 1 */
        params->wots_len2 = 2;
    }
    else {
        return -1;
    }
    params->wots_len = params->wots_len1 + params->wots_len2;
    params->wots_sig_bytes = params->wots_len * params->n;

    if (params->d == 1) {  // Assume this is XMSS, not XMSS^MT
        /* In XMSS, always use fixed 4 bytes for index_bytes */
        params->index_bytes = 4;
    }
    else {
        /* In XMSS^MT, round index_bytes up to nearest byte. */
        params->index_bytes = (params->full_height + 7) / 8;
    }
    params->sig_bytes = (params->index_bytes + params->n
                         + params->d * params->wots_sig_bytes
                         + params->full_height * params->n);

    params->pk_bytes = 2 * params->n;
    params->sk_bytes = xmss_xmssmt_core_sk_bytes(params) + params->bytes_for_max;

    return 0;
}
