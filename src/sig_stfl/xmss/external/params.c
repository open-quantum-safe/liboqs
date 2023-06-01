#include <stdint.h>
#include <string.h>

#include "params.h"
#include "xmss_core.h"

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
    else if (!strcmp(s, "XMSS-SHA2_10_192")) {
        *oid = 0x0000000d;
    }
    else if (!strcmp(s, "XMSS-SHA2_16_192")) {
        *oid = 0x0000000e;
    }
    else if (!strcmp(s, "XMSS-SHA2_20_192")) {
        *oid = 0x0000000f;
    }
    else if (!strcmp(s, "XMSS-SHAKE256_10_256")) {
        *oid = 0x00000010;
    }
    else if (!strcmp(s, "XMSS-SHAKE256_16_256")) {
        *oid = 0x00000011;
    }
    else if (!strcmp(s, "XMSS-SHAKE256_20_256")) {
        *oid = 0x00000012;
    }
    else if (!strcmp(s, "XMSS-SHAKE256_10_192")) {
        *oid = 0x00000013;
    }
    else if (!strcmp(s, "XMSS-SHAKE256_16_192")) {
        *oid = 0x00000014;
    }
    else if (!strcmp(s, "XMSS-SHAKE256_20_192")) {
        *oid = 0x00000015;
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
    else if (!strcmp(s, "XMSSMT-SHA2_20/2_192")) {
        *oid = 0x00000021;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_20/4_192")) {
        *oid = 0x00000022;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/2_192")) {
        *oid = 0x00000023;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/4_192")) {
        *oid = 0x00000024;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_40/8_192")) {
        *oid = 0x00000025;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/3_192")) {
        *oid = 0x00000026;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/6_192")) {
        *oid = 0x00000027;
    }
    else if (!strcmp(s, "XMSSMT-SHA2_60/12_192")) {
        *oid = 0x00000028;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_20/2_256")) {
        *oid = 0x00000029;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_20/4_256")) {
        *oid = 0x0000002a;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_40/2_256")) {
        *oid = 0x0000002b;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_40/4_256")) {
        *oid = 0x0000002c;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_40/8_256")) {
        *oid = 0x0000002d;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_60/3_256")) {
        *oid = 0x0000002e;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_60/6_256")) {
        *oid = 0x0000002f;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_60/12_256")) {
        *oid = 0x00000030;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_20/2_192")) {
        *oid = 0x00000031;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_20/4_192")) {
        *oid = 0x00000032;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_40/2_192")) {
        *oid = 0x00000033;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_40/4_192")) {
        *oid = 0x00000034;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_40/8_192")) {
        *oid = 0x00000035;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_60/3_192")) {
        *oid = 0x00000036;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_60/6_192")) {
        *oid = 0x00000037;
    }
    else if (!strcmp(s, "XMSSMT-SHAKE256_60/12_192")) {
        *oid = 0x00000038;
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

        case 0x0000000d:
        case 0x0000000e:
        case 0x0000000f:
            params->func = XMSS_SHA2;
            break;

        case 0x00000007:
        case 0x00000008:
        case 0x00000009:
            params->func = XMSS_SHAKE128;
            break;

        case 0x0000000a:
        case 0x0000000b:
        case 0x0000000c:

        case 0x00000010:
        case 0x00000011:
        case 0x00000012:
        case 0x00000013:
        case 0x00000014:
        case 0x00000015:
            params->func = XMSS_SHAKE256;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x0000000d:
        case 0x0000000e:
        case 0x0000000f:

        case 0x00000013:
        case 0x00000014:
        case 0x00000015:
            params->n = 24;
            params->padding_len = 4;
            break;

        case 0x00000001:
        case 0x00000002:
        case 0x00000003:

        case 0x00000007:
        case 0x00000008:
        case 0x00000009:

        case 0x00000010:
        case 0x00000011:
        case 0x00000012:
            params->n = 32;
            params->padding_len = 32;
            break;

        case 0x00000004:
        case 0x00000005:
        case 0x00000006:

        case 0x0000000a:
        case 0x0000000b:
        case 0x0000000c:
            params->n = 64;
            params->padding_len = 64;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x00000001:
        case 0x00000004:
        case 0x00000007:
        case 0x0000000a:
        case 0x0000000d:
        case 0x00000010:
        case 0x00000013:
            params->full_height = 10;
            break;

        case 0x00000002:
        case 0x00000005:
        case 0x00000008:
        case 0x0000000b:
        case 0x0000000e:
        case 0x00000011:
        case 0x00000014:
            params->full_height = 16;
            break;

        case 0x00000003:
        case 0x00000006:
        case 0x00000009:
        case 0x0000000c:
        case 0x0000000f:
        case 0x00000012:
        case 0x00000015:
            params->full_height = 20;

            break;
        default:
            return -1;
    }

    params->d = 1;
    params->wots_w = 16;

    // TODO figure out sensible and legal values for this based on the above
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

        case 0x00000021:
        case 0x00000022:
        case 0x00000023:
        case 0x00000024:
        case 0x00000025:
        case 0x00000026:
        case 0x00000027:
        case 0x00000028:
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
            params->func = XMSS_SHAKE128;
            break;

        case 0x00000019:
        case 0x0000001a:
        case 0x0000001b:
        case 0x0000001c:
        case 0x0000001e:
        case 0x0000001d:
        case 0x0000001f:
        case 0x00000020:

        case 0x00000029:
        case 0x0000002a:
        case 0x0000002b:
        case 0x0000002c:
        case 0x0000002d:
        case 0x0000002e:
        case 0x0000002f:
        case 0x00000030:
        case 0x00000031:
        case 0x00000032:
        case 0x00000033:
        case 0x00000034:
        case 0x00000035:
        case 0x00000036:
        case 0x00000037:
        case 0x00000038:
            params->func = XMSS_SHAKE256;
            break;

        default:
            return -1;
    }
    switch (oid) {
        case 0x00000021:
        case 0x00000022:
        case 0x00000023:
        case 0x00000024:
        case 0x00000025:
        case 0x00000026:
        case 0x00000027:
        case 0x00000028:

        case 0x00000031:
        case 0x00000032:
        case 0x00000033:
        case 0x00000034:
        case 0x00000035:
        case 0x00000036:
        case 0x00000037:
        case 0x00000038:
            params->n = 24;
            params->padding_len = 4;
            break;

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

        case 0x00000029:
        case 0x0000002a:
        case 0x0000002b:
        case 0x0000002c:
        case 0x0000002d:
        case 0x0000002e:
        case 0x0000002f:
        case 0x00000030:
            params->n = 32;
            params->padding_len = 32;
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
            params->padding_len = 64;
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

        case 0x00000021:
        case 0x00000022:

        case 0x00000029:
        case 0x0000002a:

        case 0x00000031:
        case 0x00000032:
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

        case 0x00000023:
        case 0x00000024:
        case 0x00000025:

        case 0x0000002b:
        case 0x0000002c:
        case 0x0000002d:

        case 0x00000033:
        case 0x00000034:
        case 0x00000035:
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

        case 0x00000026:
        case 0x00000027:
        case 0x00000028:

        case 0x0000002e:
        case 0x0000002f:
        case 0x00000030:

        case 0x00000036:
        case 0x00000037:
        case 0x00000038:
            params->full_height = 60;
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
        case 0x00000021:
        case 0x00000023:
        case 0x00000029:
        case 0x0000002b:
        case 0x00000031:
        case 0x00000033:
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
        case 0x00000022:
        case 0x00000024:
        case 0x0000002a:
        case 0x0000002c:
        case 0x00000032:
        case 0x00000034:
            params->d = 4;
            break;

        case 0x00000005:
        case 0x0000000d:
        case 0x00000015:
        case 0x0000001d:
        case 0x00000025:
        case 0x0000002d:
        case 0x00000035:
            params->d = 8;
            break;

        case 0x00000006:
        case 0x0000000e:
        case 0x00000016:
        case 0x0000001e:
        case 0x00000026:
        case 0x0000002e:
        case 0x00000036:
            params->d = 3;
            break;

        case 0x00000007:
        case 0x0000000f:
        case 0x00000017:
        case 0x0000001f:
        case 0x00000027:
        case 0x0000002f:
        case 0x00000037:
            params->d = 6;
            break;

        case 0x00000008:
        case 0x00000010:
        case 0x00000018:
        case 0x00000020:
        case 0x00000028:
        case 0x00000030:
        case 0x00000038:
            params->d = 12;
            break;

        default:
            return -1;
    }

    params->wots_w = 16;

    // TODO figure out sensible and legal values for this based on the above
    params->bds_k = 0;

    return xmss_xmssmt_initialize_params(params);
}

/**
 * Given a params struct where the following properties have been initialized;
 *  - full_height; the height of the complete (hyper)tree
 *  - n; the number of bytes of hash function output
 *  - d; the number of layers (d > 1 implies XMSSMT)
 *  - func; one of {XMSS_SHA2, XMSS_SHAKE128, XMSS_SHAKE256}
 *  - wots_w; the Winternitz parameter
 *  - optionally, bds_k; the BDS traversal trade-off parameter,
 * this function initializes the remainder of the params structure.
 */
int xmss_xmssmt_initialize_params(xmss_params *params)
{
    params->tree_height = params->full_height  / params->d;
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
    params->sk_bytes = xmss_xmssmt_core_sk_bytes(params);

    return 0;
}
