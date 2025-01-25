/**
 * @file reed_solomon.c
 * @brief Constant time implementation of Reed-Solomon codes
 */

#include "fft.h"
#include "gf.h"
#include "reed_solomon.h"
#include "parameters.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#ifdef VERBOSE
#include <stdbool.h>
#include <stdio.h>
#endif

static uint16_t mod(uint16_t i, uint16_t modulus);
static void compute_syndromes(__m256i *syndromes, uint8_t *cdw);
static uint16_t compute_elp(uint16_t *sigma, const uint16_t *syndromes);
static void compute_roots(uint8_t *error, uint16_t *sigma);
static void compute_z_poly(uint16_t *z, const uint16_t *sigma, uint16_t degree, const uint16_t *syndromes);
static void compute_error_values(uint16_t *error_values, const uint16_t *z, const uint8_t *error);
static void correct_errors(uint8_t *cdw, const uint16_t *error_values);

static const __m256i alpha_ij256_1[55] = {
    {0x0010000800040002,0x001d008000400020,0x00cd00e80074003a,0x004c002600130087},
    {0x001d004000100004,0x004c001300cd0074,0x008f00ea00b4002d,0x009d006000180006},
    {0x00cd003a00400008,0x008f0075002d0026,0x002500270060000c,0x004600c100b50035},
    {0x004c00cd001d0010,0x009d0018008f00b4,0x004600ee006a0025,0x005f00b9005d0014},
    {0x00b4002600740020,0x006a009c00600003,0x00b900a0000500c1,0x00fd000f005e00be},
    {0x008f002d00cd0040,0x004600b500250060,0x0065006100b90050,0x00d900df006b0078},
    {0x0018007500130080,0x005d008c00b5009c,0x006b003c005e00a1,0x0081001a004300a3},
    {0x009d008f004c001d,0x005f005d0046006a,0x00d900fe00fd0065,0x0085003b0081000d},
    {0x0025000c002d003a,0x006500a1005000c1,0x00d0008600df00e7,0x00a800a9006600ed},
    {0x006a006000b40074,0x00fd005e00b90005,0x003b0067001100df,0x00e600550084002e},
    {0x00ee002700ea00e8,0x00fe003c006100a0,0x00b8007600670086,0x00e3009100390054},
    {0x00460025008f00cd,0x00d9006b006500b9,0x00a800b8003b00d0,0x0082009600fc00e4},
    {0x0014003500060087,0x000d00a3007800be,0x00e40054002e00ed,0x00510064006200e5},
    {0x005d00b500180013,0x00810043006b005e,0x00fc003900840066,0x0012005900c80062},
    {0x00b900c100600026,0x003b001a00df000f,0x00960091005500a9,0x002c002400590064},
    {0x005f0046009d004c,0x0085008100d900fd,0x008200e300e600a8,0x0002002c00120051},
    {0x0099000a004e0098,0x004f0093004400d6,0x00dd00dc00d70092,0x00980001000b0045},
    {0x006500500025002d,0x00a8006600d000df,0x00c30007009600bf,0x0027002600ad00fb},
    {0x001e00ba0094005a,0x0049006d003e00e2,0x003d00a200ae00b3,0x008c006000e80083},
    {0x00fd00b9006a00b4,0x00e60084003b0011,0x002c00ac001c0096,0x00be00c100030020},
    {0x006b00a100b50075,0x00fc00290066001a,0x00ad00f500590057,0x00e700b90035002d},
    {0x00fe006100ee00ea,0x00e3003900b80067,0x003a00b000ac0007,0x00af000f002800c0},
    {0x005b002f009f00c9,0x009500d10021007c,0x0075004700f400a6,0x001f00df00c200ee},
    {0x00d900650046008f,0x008200fc00a8003b,0x0027003a002c00c3,0x0017001a00e700ba},
    {0x0011000f00050003,0x001c00ff00550033,0x00c100b4006c0024,0x004d003b00e2005e},
    {0x000d007800140006,0x0051006200e4002e,0x00ba00c0002000fb,0x00d100a900bd00bb},
    {0x00d000e70050000c,0x00c3005700bf00a9,0x002f00b50026007d,0x00db005500c500d9},
    {0x0081006b005d0018,0x001200c800fc0084,0x00e70028000300ad,0x00190091009e00bd},
    {0x00f8007f00690030,0x00f700e000f1004d,0x00b6005f009c0040,0x00a2009600aa00ec},
    {0x003b00df00b90060,0x002c005900960055,0x001a000f00c10026,0x00240064009100a9},
    {0x009700b600de00c0,0x001b009b006e0072,0x00ed00b100a0008f,0x00580059004b0052},
    {0x008500d9005f009d,0x00020012008200e6,0x001700af00be0027,0x00040024001900d1},
    {0x00b8008600610027,0x003a00f500070091,0x001500d0000f00b5,0x002d002c00a600f1},
    {0x004f00440099004e,0x0098000b00dd00d7,0x0092009300d6000a,0x004e0001004500dc},
    {0x0084001a005e009c,0x000300e9005900ff,0x0091002e00e200b9,0x0005002600eb001c},
    {0x00a800d000650025,0x002700ad00c30096,0x00db0015001a002f,0x00610060003600f2},
    {0x005200ce0089004a,0x00d40010008a0037,0x00570049007c0078,0x00d300c1001d0048},
    {0x0049003e001e0094,0x008c00e8003d00ae,0x003800630033007f,0x004300b900ea0016},
    {0x00e400ed00780035,0x00ba002d00fb0064,0x00f200f100a900d9,0x003e000f002500ad},
    {0x00e6003b00fd006a,0x00be0003002c001c,0x00240037004d001a,0x002e00df00050074},
    {0x00c600c500d300d4,0x00ca009d00cf00a7,0x008b00c80072003e,0x009a001a005f00c9},
    {0x00fc0066006b00b5,0x00e7003500ad0059,0x003600a6009100c5,0x00bf003b00780025},
    {0x007b001700b10077,0x00e1009f000800ef,0x0040002b00ff00b8,0x00ab00a9005b008c},
    {0x00e300b800fe00ee,0x00af0028003a00ac,0x002d007a00370015,0x00320055003400de},
    {0x009600a900df00c1,0x001a00b900260024,0x0060002c00640055,0x00590091003b000f},
    {0x00950021005b009f,0x001f00c2007500f4,0x00b500d800a70073,0x0048009600da00fe},
    {0x00a5001500710023,0x00760089000c00eb,0x0050008000ef00fc,0x00b0006400520022},
    {0x008200a800d90046,0x001700e70027002c,0x0061002d002400db,0x0008005900bf003e},
    {0x00c800290043008c,0x009e00fe003500e9,0x0078003000eb006e,0x005a002400e300cc},
    {0x001c005500110005,0x004d00e200c1006c,0x00df006a00e90064,0x009c002c00ae0084},
    {0x00dd00920044000a,0x00920044000a0001,0x0044000a000100dd,0x000a000100dd0092},
    {0x005100e4000d0014,0x00d100bd00ba0020,0x003e00de007400f2,0x00c20026002b003f},
    {0x0079007300340028,0x00e500f800a10074,0x006600ca00b4008a,0x00bb006000f7004b},
    {0x00c300bf00d00050,0x00db00c5002f0026,0x0021006b006000f5,0x008600c100cf0082},
    {0x00ac0091006700a0,0x0037002e000f00b4,0x005500e2006a002c,0x007c00b9002000a7}
};
static const __m256i alpha_ij256_2[55] = {
    {0x00b4005a002d0098,0x008f00c900ea0075,0x0018000c00060003,0x009d00c000600030},
    {0x006a00940025004e,0x0046009f00ee00b5,0x005d005000140005,0x005f00de00b90069},
    {0x00b900ba0050000a,0x0065002f006100a1,0x006b00e70078000f,0x00d900b600df007f},
    {0x00fd001e00650099,0x00d9005b00fe006b,0x008100d0000d0011,0x00850097003b00f8},
    {0x001100e200df00d6,0x003b007c0067001a,0x008400a9002e0033,0x00e600720055004d},
    {0x003b003e00d00044,0x00a8002100b80066,0x00fc00bf00e40055,0x0082006e009600f1},
    {0x0084006d00660093,0x00fc00d100390029,0x00c80057006200ff,0x0012009b005900e0},
    {0x00e6004900a8004f,0x0082009500e300fc,0x001200c30051001c,0x0002001b002c00f7},
    {0x009600b300bf0092,0x00c300a600070057,0x00ad007d00fb0024,0x0027008f00260040},
    {0x001c00ae009600d7,0x002c00f400ac0059,0x000300260020006c,0x00be00a000c1009c},
    {0x00ac00a2000700dc,0x003a004700b000f5,0x002800b500c000b4,0x00af00b1000f005f},
    {0x002c003d00c300dd,0x00270075003a00ad,0x00e7002f00ba00c1,0x001700ed001a00b6},
    {0x0020008300fb0045,0x00ba00ee00c0002d,0x00bd00d900bb005e,0x00d1005200a900ec},
    {0x000300e800ad000b,0x00e700c200280035,0x009e00c500bd00e2,0x0019004b009100aa},
    {0x00c1006000260001,0x001a00df000f00b9,0x0091005500a9003b,0x0024005900640096},
    {0x00be008c00270098,0x0017001f00af00e7,0x001900db00d1004d,0x00040058002400a2},
    {0x00d60099000a004e,0x0092004f00930044,0x004500dd00dc00d7,0x004e00980001000b},
    {0x001a007f002f000a,0x00db0073001500c5,0x003600f500f20064,0x00610046006000cd},
    {0x00330034007f0099,0x00380062006300a8,0x00ea0008001600ac,0x004300f000b900d4},
    {0x004d0033001a00d6,0x002400a700370091,0x00050060007400e9,0x002e006700df005e},
    {0x009100a800c50044,0x0036003d00a6006e,0x007800ba00250026,0x00bf0015003b0086},
    {0x0037006300150093,0x002d00d8007a00a6,0x0034006b00de006a,0x0032007b00550085},
    {0x00a700620073004f,0x00b5005a00d8003d,0x00da00ce00fe00be,0x004800e0009600d5},
    {0x0024003800db0092,0x006100b5002d0036,0x00bf0021003e00df,0x000800fb0059006e},
    {0x00e900ac006400d7,0x00df00be006a0026,0x00ae00910084007c,0x009c0074002c00ef},
    {0x0074001600f200dc,0x003e00fe00de0025,0x002b0082003f0084,0x00c200d4002600fa},
    {0x0060000800f500dd,0x002100ce006b00ba,0x00cf005600820091,0x0086006500c1002d},
    {0x000500ea00360045,0x00bf00da00340078,0x005a00cf002b00ae,0x005c0088000f0023},
    {0x005e00d400cd000b,0x006e00d500850086,0x0023002d00fa00ef,0x006300da001a001e},
    {0x00df00b900600001,0x005900960055003b,0x000f00c10026002c,0x0064009100a9001a},
    {0x006700f000460098,0x00fb00e0007b0015,0x0088006500d40074,0x009000c8009100da},
    {0x002e00430061004e,0x00080048003200bf,0x005c008600c2009c,0x0010009000640063},
    {0x005500ed006b000a,0x000c003600c300c4,0x0073006600b600b9,0x0025000800240082},
    {0x00d7004f00440099,0x000a0098000b00dd,0x00dc0092009300d6,0x0099004e00010045},
    {0x00ae0072003b00d6,0x000f006a00200024,0x00ef0096004d0067,0x001100be0060006c},
    {0x005900f100210044,0x008600a1000c00cf,0x007d00a600b300a9,0x00b800d900b9008f},
    {0x00f4001900e40093,0x00c500b1008c00cd,0x004c00fb008d00e6,0x00c600cc00df0028},
    {0x006c007900f1004f,0x002900bd00bc0027,0x00ee004000090037,0x00c800b7003b00d3},
    {0x002600f500820092,0x00b300b800b60050,0x0065002700360059,0x003d0057005500ce},
    {0x009c006c005900d7,0x00640072007c000f,0x001100b900b400eb,0x002000ac00960084},
    {0x00a00013003d00dc,0x005600ab009e00d9,0x0085007f009f0020,0x004a00d8005900e5},
    {0x000f002700cf00dd,0x007d0038007300ed,0x00e4003e00650060,0x002f000c002c0007},
    {0x00e20014003a0045,0x00cd001200310021,0x00950015004300a0,0x0022006900260090},
    {0x007c00bc000c000b,0x0025008300e00073,0x007900fc009700fd,0x006d00e100c10002},
    {0x00a900df00c10001,0x00b9002600240096,0x002c00640055001a,0x0091003b000f0060},
    {0x007200bd00a10098,0x006b009400830038,0x0087008a00e3002e,0x008d00aa001a00d2},
    {0x00ff008500e7004e,0x00d0006f0013008a,0x00d4003600700072,0x007a006200a900fe},
    {0x006400290086000a,0x00b8006b0025007d,0x002f0075003d0096,0x004000f2009100ed},
    {0x00ef003f00ed0099,0x00e400680069003a,0x00af0046008e00a7,0x009400fa0064009a},
    {0x00eb003700a900d6,0x0096002e00fd0060,0x0033000f000300f4,0x005e00b4002400ff},
    {0x000100dd00920044,0x00dd00920044000a,0x00920044000a0001,0x0044000a000100dd},
    {0x00b4000900b30093,0x003d00e300970065,0x00310017003c0003,0x00da00d3006000f3},
    {0x006a00b00057004f,0x00ad000e009a00b6,0x00a200e400880005,0x003f001f00b90080},
    {0x00b9004000a60092,0x0075008a00fc003e,0x008b00c40017000f,0x000700a800df0025},
    {0x00fd0003002400d7,0x00c100e900ae00a9,0x0074005900720011,0x00f400ff003b00be}
};



/**
 * Coefficients of polynomial G
 * stored in 256-bit values
 **/
static const __m256i param256[2] = {
    {0x001800EF00D8002D, 0x0028001B006800FD, 0x00D200A30032006B, 0x009E00E0008600E3},
    {0x0001009E000D0077, 0x002B005200A400EE, 0x008E00F600E8000F, 0x00E8001D00BD0032}
};



/**
 * Returns i modulo the given modulus.
 * i must be less than 2*modulus.
 * Therefore, the return value is either i or i-modulus.
 * @returns i mod (modulus)
 * @param[in] i The integer whose modulo is taken
 * @param[in] modulus The modulus
 */
static uint16_t mod(uint16_t i, uint16_t modulus) {
    uint16_t tmp = i - modulus;

    // mask = 0xffff if(i < PARAM_GF_MUL_ORDER)
    int16_t mask = -(tmp >> 15);

    return tmp + (mask & modulus);
}



/**
 * @brief Computes the generator polynomial of the primitive Reed-Solomon code with given parameters.
 *
 * Code length is 2^m-1. <br>
 * PARAM_DELTA is the targeted correction capacity of the code
 * and receives the real correction capacity (which is at least equal to the target). <br>
 * gf_exp and gf_log are arrays giving antilog and log of GF(2^m) elements.
 *
 * @param[out] poly Array of size (2*PARAM_DELTA + 1) receiving the coefficients of the generator polynomial
 */
void compute_generator_poly(uint16_t *poly) {
    poly[0] = 1;
    int tmp_degree = 0;

    for (uint16_t i = 1; i < (2 * PARAM_DELTA + 1); ++i) {

        for (size_t j = tmp_degree; j; --j) {
            poly[j] = gf_exp[mod(gf_log[poly[j]] + i, PARAM_GF_MUL_ORDER)] ^ poly[j - 1];
        }

        poly[0] = gf_exp[mod(gf_log[poly[0]] + i, PARAM_GF_MUL_ORDER)];
        poly[++tmp_degree] = 1;

    }

    printf("\n");
    for (int i = 0 ; i < (PARAM_G); ++i) {
        printf("%d, ", poly[i]);
    }
    printf("\n");
}



/**
 * @brief Encodes a message message of PARAM_K bits to a Reed-Solomon codeword codeword of PARAM_N1 bytes
 *
 * Following @cite lin1983error (Chapter 4 - Cyclic Codes),
 * We perform a systematic encoding using a linear (PARAM_N1 - PARAM_K)-stage shift register
 * with feedback connections based on the generator polynomial PARAM_RS_POLY of the Reed-Solomon code.
 *
 * @param[out] cdw Array of size VEC_N1_SIZE_64 receiving the encoded message
 * @param[in] msg Array of size VEC_K_SIZE_64 storing the message
 */
void reed_solomon_encode(uint64_t *cdw, const uint64_t *msg) {
    uint8_t gate_value = 0;

    union {
        uint16_t arr16[16 * CEIL_DIVIDE(PARAM_G, 16)];
        __m256i dummy;
    } tmp = {0};

    uint8_t msg_bytes[PARAM_K] = {0};
    uint8_t cdw_bytes[PARAM_N1] = {0};
    __m256i *tmp256 = (__m256i *)tmp.arr16;
    uint16_t * PARAM_RS_POLY = (uint16_t *) param256;

    memcpy(msg_bytes, msg, PARAM_K);

    for (int16_t i = PARAM_K - 1; i >= 0; --i) {
        gate_value = msg_bytes[i] ^ cdw_bytes[PARAM_N1 - PARAM_K - 1];

        __m256i gate256 = _mm256_set1_epi16(gate_value);
        tmp256[0] = gf_mul_vect(gate256, param256[0]);
        tmp256[1] = gf_mul_vect(gate256, param256[1]);

        for (size_t j = 32; j < PARAM_G; ++j) {
            tmp.arr16[j] = gf_mul(gate_value, PARAM_RS_POLY[j]);
        }

        for (size_t k = PARAM_N1 - PARAM_K - 1; k; --k) {
            cdw_bytes[k] = cdw_bytes[k - 1] ^ tmp.arr16[k];
        }

        cdw_bytes[0] = tmp.arr16[0];
    }

    memcpy(cdw_bytes + PARAM_N1 - PARAM_K, msg_bytes, PARAM_K);
    memcpy(cdw, cdw_bytes, PARAM_N1);
}



/**
 * @brief Computes 2 * PARAM_DELTA syndromes
 *
 * @param[out] syndromes Array of size 2 * PARAM_DELTA receiving the computed syndromes
 * @param[in] cdw Array of size PARAM_N1 storing the received vector
 */
void compute_syndromes(__m256i *syndromes256, uint8_t *cdw) {
    syndromes256[0] = _mm256_set1_epi16(cdw[0]);

    for (size_t i = 0; i < PARAM_N1 - 1; ++i) {
        syndromes256[0] ^= gf_mul_vect(_mm256_set1_epi16(cdw[i + 1]), alpha_ij256_1[i]);
    }

    syndromes256[1] = _mm256_set1_epi16(cdw[0]);

    for (size_t i = 0; i < PARAM_N1 - 1; ++i) {
        syndromes256[1] ^= gf_mul_vect(_mm256_set1_epi16(cdw[i + 1]), alpha_ij256_2[i]);
    }
}



/**
 * @brief Computes the error locator polynomial (ELP) sigma
 *
 * This is a constant time implementation of Berlekamp's simplified algorithm (see @cite lin1983error (Chapter 6 - BCH Codes). <br>
 * We use the letter p for rho which is initialized at -1. <br>
 * The array X_sigma_p represents the polynomial X^(mu-rho)*sigma_p(X). <br>
 * Instead of maintaining a list of sigmas, we update in place both sigma and X_sigma_p. <br>
 * sigma_copy serves as a temporary save of sigma in case X_sigma_p needs to be updated. <br>
 * We can properly correct only if the degree of sigma does not exceed PARAM_DELTA.
 * This means only the first PARAM_DELTA + 1 coefficients of sigma are of value
 * and we only need to save its first PARAM_DELTA - 1 coefficients.
 *
 * @returns the degree of the ELP sigma
 * @param[out] sigma Array of size (at least) PARAM_DELTA receiving the ELP
 * @param[in] syndromes Array of size (at least) 2*PARAM_DELTA storing the syndromes
 */
static uint16_t compute_elp(uint16_t *sigma, const uint16_t *syndromes) {
    uint16_t deg_sigma = 0;
    uint16_t deg_sigma_p = 0;
    uint16_t deg_sigma_copy = 0;
    uint16_t sigma_copy[PARAM_DELTA + 1] = {0};
    uint16_t X_sigma_p[PARAM_DELTA + 1] = {0, 1};
    uint16_t pp = (uint16_t) -1; // 2*rho
    uint16_t d_p = 1;
    uint16_t d = syndromes[0];

    uint16_t mask1, mask2, mask12;
    uint16_t deg_X, deg_X_sigma_p;
    uint16_t dd;
    uint16_t mu;

    uint16_t i;

    sigma[0] = 1;
    for (mu = 0; (mu < (2 * PARAM_DELTA)); ++mu) {
        // Save sigma in case we need it to update X_sigma_p
        memcpy(sigma_copy, sigma, 2 * (PARAM_DELTA));
        deg_sigma_copy = deg_sigma;

        dd = gf_mul(d, gf_inverse(d_p));

        for (i = 1; (i <= mu + 1) && (i <= PARAM_DELTA); ++i) {
            sigma[i] ^= gf_mul(dd, X_sigma_p[i]);
        }

        deg_X = mu - pp;
        deg_X_sigma_p = deg_X + deg_sigma_p;

        // mask1 = 0xffff if(d != 0) and 0 otherwise
        mask1 = -((uint16_t) - d >> 15);

        // mask2 = 0xffff if(deg_X_sigma_p > deg_sigma) and 0 otherwise
        mask2 = -((uint16_t) (deg_sigma - deg_X_sigma_p) >> 15);

        // mask12 = 0xffff if the deg_sigma increased and 0 otherwise
        mask12 = mask1 & mask2;
        deg_sigma ^= mask12 & (deg_X_sigma_p ^ deg_sigma);

        if (mu == (2 * PARAM_DELTA - 1)) {
            break;
        }

        pp ^= mask12 & (mu ^ pp);
        d_p ^= mask12 & (d ^ d_p);
        for (i = PARAM_DELTA; i; --i) {
            X_sigma_p[i] = (mask12 & sigma_copy[i - 1]) ^ (~mask12 & X_sigma_p[i - 1]);
        }

        deg_sigma_p ^= mask12 & (deg_sigma_copy ^ deg_sigma_p);
        d = syndromes[mu + 1];

        for (i = 1; (i <= mu + 1) && (i <= PARAM_DELTA); ++i) {
            d ^= gf_mul(sigma[i], syndromes[mu + 1 - i]);
        }
    }

    return deg_sigma;
}



/**
 * @brief Computes the error polynomial error from the error locator polynomial sigma
 *
 * See function fft for more details.
 *
 * @param[out] error Array of 2^PARAM_M elements receiving the error polynomial
 * @param[out] error_compact Array of PARAM_DELTA + PARAM_N1 elements receiving a compact representation of the vector error
 * @param[in] sigma Array of 2^PARAM_FFT elements storing the error locator polynomial
 */
static void compute_roots(uint8_t *error, uint16_t *sigma) {
    uint16_t w[1 << PARAM_M] = {0};

    fft(w, sigma, PARAM_DELTA + 1);
    fft_retrieve_error_poly(error, w);
}



/**
 * @brief Computes the polynomial z(x)
 *
 * See @cite lin1983error (Chapter 6 - BCH Codes) for more details.
 *
 * @param[out] z Array of PARAM_DELTA + 1 elements receiving the polynomial z(x)
 * @param[in] sigma Array of 2^PARAM_FFT elements storing the error locator polynomial
 * @param[in] degree Integer that is the degree of polynomial sigma
 * @param[in] syndromes Array of 2 * PARAM_DELTA storing the syndromes
 */
static void compute_z_poly(uint16_t *z, const uint16_t *sigma, uint16_t degree, const uint16_t *syndromes) {
    size_t i, j;
    uint16_t mask;

    z[0] = 1;

    for (i = 1; i < PARAM_DELTA + 1; ++i) {
        mask = -((uint16_t) (i - degree - 1) >> 15);
        z[i] = mask & sigma[i];
    }

    z[1] ^= syndromes[0];

    for (i = 2; i <= PARAM_DELTA; ++i) {
        mask = -((uint16_t) (i - degree - 1) >> 15);
        z[i] ^= mask & syndromes[i - 1];

        for (j = 1; j < i; ++j) {
            z[i] ^= mask & gf_mul(sigma[j], syndromes[i - j - 1]);
        }
    }
}



/**
 * @brief Computes the error values
 *
 * See @cite lin1983error (Chapter 6 - BCH Codes) for more details.
 *
 * @param[out] error_values Array of PARAM_DELTA elements receiving the error values
 * @param[in] z Array of PARAM_DELTA + 1 elements storing the polynomial z(x)
 * @param[in] z_degree Integer that is the degree of polynomial z(x)
 * @param[in] error_compact Array of PARAM_DELTA + PARAM_N1 storing compact representation of the error
 */
static void compute_error_values(uint16_t *error_values, const uint16_t *z, const uint8_t *error) {
    uint16_t beta_j[PARAM_DELTA] = {0};
    uint16_t e_j[PARAM_DELTA] = {0};

    uint16_t delta_counter;
    uint16_t delta_real_value;
    uint16_t found;
    uint16_t mask1;
    uint16_t mask2;
    uint16_t tmp1;
    uint16_t tmp2;
    uint16_t inverse;
    uint16_t inverse_power_j;

    // Compute the beta_{j_i} page 31 of the documentation
    delta_counter = 0;
    for (size_t i = 0; i < PARAM_N1; i++) {
        found = 0;
        mask1 = (uint16_t) (-((int32_t)error[i]) >> 31); // error[i] != 0
        for (size_t j = 0; j < PARAM_DELTA; j++) {
            mask2 = ~((uint16_t) (-((int32_t) j ^ delta_counter) >> 31)); // j == delta_counter
            beta_j[j] += mask1 & mask2 & gf_exp[i];
            found += mask1 & mask2 & 1;
        }
        delta_counter += found;
    }
    delta_real_value = delta_counter;

    // Compute the e_{j_i} page 31 of the documentation
    for (size_t i = 0; i < PARAM_DELTA; ++i) {
        tmp1 = 1;
        tmp2 = 1;
        inverse = gf_inverse(beta_j[i]);
        inverse_power_j = 1;

        for (size_t j = 1; j <= PARAM_DELTA; ++j) {
            inverse_power_j = gf_mul(inverse_power_j, inverse);
            tmp1 ^= gf_mul(inverse_power_j, z[j]);
        }
        for (size_t k = 1; k < PARAM_DELTA; ++k) {
            tmp2 = gf_mul(tmp2, (1 ^ gf_mul(inverse, beta_j[(i + k) % PARAM_DELTA])));
        }
        mask1 = (uint16_t) (((int16_t) i - delta_real_value) >> 15); // i < delta_real_value
        e_j[i] = mask1 & gf_mul(tmp1, gf_inverse(tmp2));
    }

    // Place the delta e_{j_i} values at the right coordinates of the output vector
    delta_counter = 0;
    for (size_t i = 0; i < PARAM_N1; ++i) {
        found = 0;
        mask1 = (uint16_t) (-((int32_t)error[i]) >> 31); // error[i] != 0
        for (size_t j = 0; j < PARAM_DELTA; j++) {
            mask2 = ~((uint16_t) (-((int32_t) j ^ delta_counter) >> 31)); // j == delta_counter
            error_values[i] += mask1 & mask2 & e_j[j];
            found += mask1 & mask2 & 1;
        }
        delta_counter += found;
    }
}



/**
 * @brief Correct the errors
 *
 * @param[out] cdw Array of PARAM_N1 elements receiving the corrected vector
 * @param[in] error Array of the error vector
 * @param[in] error_values Array of PARAM_DELTA elements storing the error values
 */
static void correct_errors(uint8_t *cdw, const uint16_t *error_values) {
    for (size_t i = 0; i < PARAM_N1; ++i) {
        cdw[i] ^= error_values[i];
    }
}



/**
 * @brief Decodes the received word
 *
 * This function relies on six steps:
 *    <ol>
 *    <li> The first step, is the computation of the 2*PARAM_DELTA syndromes.
 *    <li> The second step is the computation of the error-locator polynomial sigma.
 *    <li> The third step, done by additive FFT, is finding the error-locator numbers by calculating the roots of the polynomial sigma and takings their inverses.
 *    <li> The fourth step, is the polynomial z(x).
 *    <li> The fifth step, is the computation of the error values.
 *    <li> The sixth step is the correction of the errors in the received polynomial.
 *    </ol>
 * For a more complete picture on Reed-Solomon decoding, see Shu. Lin and Daniel J. Costello in Error Control Coding: Fundamentals and Applications @cite lin1983error
 *
 * @param[out] msg Array of size VEC_K_SIZE_64 receiving the decoded message
 * @param[in] cdw Array of size VEC_N1_SIZE_64 storing the received word
 */
void reed_solomon_decode(uint64_t* msg, uint64_t* cdw) {
    uint8_t cdw_bytes[PARAM_N1] = {0};
    __m256i syndromes256[SYND_SIZE_256];
    uint16_t *syndromes = (uint16_t *) syndromes256;
    uint16_t sigma[1 << PARAM_FFT] = {0};
    uint8_t error[1 << PARAM_M] = {0};
    uint16_t z[PARAM_N1] = {0};
    uint16_t error_values[PARAM_N1] = {0};
    uint16_t deg;

    // Copy the vector in an array of bytes
    memcpy(cdw_bytes, cdw, PARAM_N1);

    // Calculate the 2*PARAM_DELTA syndromes
    compute_syndromes(syndromes256, cdw_bytes);

    // Compute the error locator polynomial sigma
    // Sigma's degree is at most PARAM_DELTA but the FFT requires the extra room
    deg = compute_elp(sigma, syndromes);

    // Compute the error polynomial error
    compute_roots(error, sigma);

    // Compute the polynomial z(x)
    compute_z_poly(z, sigma, deg, syndromes);

    // Compute the error values
    compute_error_values(error_values, z, error);

    // Correct the errors
    correct_errors(cdw_bytes, error_values);

    // Retrieve the message from the decoded codeword
    memcpy(msg, cdw_bytes + (PARAM_G - 1) , PARAM_K);

    #ifdef VERBOSE
        printf("\n\nThe syndromes: ");
        for (size_t i = 0 ; i < 2*PARAM_DELTA ; ++i) {
            printf("%u ", syndromes[i]);
        }
        printf("\n\nThe error locator polynomial: sigma(x) = ");
        bool first_coeff = true;
        if (sigma[0]) {
            printf("%u", sigma[0]);
            first_coeff = false;
        }
        for (size_t i = 1 ; i < (1 << PARAM_FFT) ; ++i) {
            if (sigma[i] == 0)
                continue;
            if (!first_coeff)
                printf(" + ");
            first_coeff = false;
            if(sigma[i] != 1)
                printf("%u ", sigma[i]);
            if (i == 1)
                printf("x");
            else
                printf("x^%zu", i);
        }   
        if (first_coeff)
            printf("0");

        printf("\n\nThe polynomial: z(x) = ");
        bool first_coeff_1 = true;
        if (z[0]) {
            printf("%u", z[0]);
            first_coeff_1 = false;
        }
        for (size_t i = 1 ; i < (PARAM_DELTA + 1) ; ++i) {
            if (z[i] == 0)
                continue;
            if (!first_coeff_1)
                printf(" + ");
            first_coeff_1 = false;
            if(z[i] != 1)
                printf("%u ", z[i]);
            if (i == 1)
                printf("x");
            else
                printf("x^%zu", i);
        }
        if (first_coeff_1)
            printf("0");
    
        printf("\n\nThe pairs of (error locator numbers, error values): ");
        size_t j = 0;
        for (size_t i = 0 ; i < PARAM_N1 ; ++i) {
            if(error[i]){
                printf("(%zu, %d) ", i, error_values[j]);
                j++;
            }
        }
        printf("\n");
    #endif
}
