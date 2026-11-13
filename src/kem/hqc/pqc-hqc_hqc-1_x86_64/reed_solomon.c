/**
 * @file reed_solomon.c
 * @brief Constant time implementation of Reed-Solomon codes
 */

#include "reed_solomon.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "crypto_memset.h"
#include "gf.h"
#include "parameters.h"
#ifdef VERBOSE
#include <stdbool.h>
#include <stdio.h>
#endif

/**
 * @brief Compiler helpers for constant-time routines.
 *
 * On GCC/Clang, CT_OPAQUE(x) makes the value of x opaque to the optimizer: the
 * compiler can no longer make assumptions about it, without actually reading or
 * writing it each time it is referenced. Other compilers fall back to volatile.
 * This only constrains the optimizer; it is not a memory fence.
 */
#if defined(__GNUC__) || defined(__clang__)
#define CT_VOLATILE
#define CT_OPAQUE(var) __asm__("" : "+r"(var) : /* no inputs */)
#else
#define CT_VOLATILE volatile
#define CT_OPAQUE(var)
#endif

/**
 * @def GF_LANES
 * @brief Number of GF(2^8) values per AVX2 vector: 16 lanes of 16 bits.
 */
#define GF_LANES 16

/**
 * @def SYND_VEC_COUNT
 * @brief Number of AVX2 vectors needed to hold the 2·PARAM_T syndromes.
 *
 * Rounds the syndrome count (2·PARAM_T) up to the next multiple of \c GF_LANES.
 */
#define SYND_VEC_COUNT (CEIL_DIVIDE(2 * PARAM_T, GF_LANES))

/**
 * @def POSITIONS_BLOCKS
 * @brief Number of \c GF_LANES-wide blocks needed to cover all message positions.
 *
 * This macro rounds up the number of message bytes \c PARAM_K to the next
 * multiple of \c GF_LANES, matching the lane width used by AVX2 vectors.
 * It is used to iterate over message positions in fixed-size SIMD blocks
 * during Chien search / polynomial evaluation.
 */
#define POSITIONS_BLOCKS ((PARAM_K + GF_LANES - 1) / GF_LANES)

static inline uint16_t mod(uint16_t i, uint16_t modulus);
static inline uint16_t make_nonzero_mask(uint16_t x);
static inline uint8_t is_zero(uint8_t x);
static inline void compute_syndromes(__m256i *syndrome_vecs, const uint8_t *cdw);
static inline void compute_error_locator_polynomial(uint16_t *sigma, const uint16_t *syndromes);
static inline void compute_error_positions(uint8_t *error_positions, const uint16_t *sigma);
static inline void evaluate_poly_at_all_msg_points(uint8_t *out, const uint16_t *poly);
static inline void batch_inverse(uint8_t *inv, const uint8_t *a);
static inline void correct_errors(uint8_t *message, const uint8_t *cdw_bytes, const uint16_t *syndromes,
                                  const uint16_t *sigma, const uint8_t *error_positions);

/**
 * @brief Precomputed AVX2 table of \f$\alpha_i^j\f$ powers for syndrome accumulation (part 1/2).
 *
 * Each entry is one 256-bit block used by compute_syndromes() for a fixed
 * codeword position. The 16-bit lanes contain the GF(2^8) powers needed for
 * one contiguous block of syndrome indices.
 *
 * The table has \c PARAM_N1 - 1 entries, one for each non-constant codeword
 * coefficient processed during syndrome accumulation.
 */
static const __m256i alpha_ij_vecs_1[45] = {
    {0x0011000f00050003, 0x001a00ff00550033, 0x00a100960072002e, 0x005f0035001300f8},
    {0x001a005500110005, 0x005f001300a10072, 0x00f7009500d80038, 0x00e50066001e0006},
    {0x00a1002e0055000f, 0x00f7007300380035, 0x003700340066000a, 0x005300ab00d90026},
    {0x005f00a1001a0011, 0x00e5001e00f700d8, 0x00530090006a0037, 0x004c00d3004f0014},
    {0x00d8003500720033, 0x006a00e400660002, 0x00d300cc000400ab, 0x00830008004d00d4},
    {0x00f7003800a10055, 0x005300d900370066, 0x0062006700d30044, 0x00b500b3006b0078},
    {0x001e0073001300ff, 0x004f00f500d900e4, 0x006b0028004d00cd, 0x00fe001d005700ce},
    {0x00e500f7005f001a, 0x004c004f0053006a, 0x00b5008100830062, 0x00fb002f00fe000b},
    {0x0037000a0038002e, 0x006200cd004400ab, 0x00bb00f900b3009e, 0x00c300c200600092},
    {0x006a006600d80072, 0x0083004d00d30004, 0x002f0061001000b3, 0x009f004000fa003a},
    {0x0090003400950096, 0x00810028006700cc, 0x00d20071006100f9, 0x009b00ef002c0041},
    {0x0053003700f700a1, 0x00b5006b006200d3, 0x00c300d2002f00bb, 0x00fc00e80082009c},
    {0x00140026000600f8, 0x000b00ce007800d4, 0x009c0041003a0092, 0x004500630065009d},
    {0x004f00d9001e0013, 0x00fe0057006b004d, 0x0082002c00fa0060, 0x0012004a00a50065},
    {0x00d300ab00660035, 0x002f001d00b30008, 0x00e800ef004000c2, 0x00390036004a0063},
    {0x004c005300e5005f, 0x00fb00fe00b50083, 0x00fc009b009f00c3, 0x0003003900120045},
    {0x00e0000c005c00e1, 0x005d00ec005000bd, 0x00b000b100bc00ed, 0x00e10001000d0051},
    {0x0062004400370038, 0x00c3006000bb00b3, 0x00a8000700e800d5, 0x0034003500c70085},
    {0x001800d100eb0048, 0x005b006d002b009a, 0x002900cf00c500df, 0x00f50066009600fd},
    {0x008300d3006a00d8, 0x009f00fa002f0010, 0x003900c6001b00e8, 0x00d400ab00020033},
    {0x006b00cd00d90073, 0x0082003d0060001d, 0x00c7008c004a0043, 0x009e00d300260038},
    {0x0081006700900095, 0x009b002c00d20061, 0x002e00dd00c60007, 0x00c40008003c00aa},
    {0x0049003b00e600a4, 0x00ea00ba0032007d, 0x00730052008d00ca, 0x001900b300a90090},
    {0x00b50062005300f7, 0x00fc008200c3002f, 0x0034002e003900a8, 0x0016001d009e00d1},
    {0x0010000800040002, 0x001b008000400020, 0x00ab00d8006c0036, 0x005e002f009a004d},
    {0x000b007800140006, 0x00450065009c003a, 0x00d100aa00330085, 0x00ba00c200d600d0},
    {0x00bb009e0044000a, 0x00a8004300d500c2, 0x003b00d90035007c, 0x00b6004000ae00b5},
    {0x00fe006b004f001e, 0x001200a5008200fa, 0x009e003c000200c7, 0x001f00ef00e700d6},
    {0x0087007f00680022, 0x008f00990089005e, 0x00db004c00e40055, 0x00cf00e800c00093},
    {0x002f00b300d30066, 0x0039004a00e80040, 0x001d000800ab0035, 0x0036006300ef00c2},
    {0x00e900db00b200aa, 0x001c00e3006f0074, 0x009200dc00cc00f7, 0x004b004a00580047},
    {0x00fb00b5004c00e5, 0x0003001200fc009f, 0x001600c400d40034, 0x00050036001f00ba},
    {0x00d200f900670034, 0x002e008c000700ef, 0x001500bb000800d9, 0x0038003900ca0089},
    {0x005d005000e0005c, 0x00e1000d00b000bc, 0x00ed00ec00bd000c, 0x005c0001005100b1},
    {0x00fa001d004d00e4, 0x00020097004a0080, 0x00ef003a009a00d3, 0x000400350094001b},
    {0x00c300bb00620037, 0x003400c700a800e8, 0x00b60015001d003b, 0x006700660024008b},
    {0x004700a300f10059, 0x00be001100f30025, 0x0043005b007d0078, 0x00b900ab001a005a},
    {0x005b002b001800eb, 0x00f50096002900c5, 0x002d00640020007f, 0x005700d300950017},
    {0x009c009200780026, 0x00d1003800850063, 0x008b008900c200b5, 0x002b0008003700c7},
    {0x009f002f0083006a, 0x00d400020039001b, 0x00360025005e001d, 0x003a00b300040072},
    {0x00ac00ae00b900be, 0x00a600e500a200cb, 0x00f200a50074002b, 0x00e2001d004c00a4},
    {0x00820060006b00d9, 0x009e002600c7004a, 0x002400ca00ef00ae, 0x00d5002f00780037},
    {0x007a001600dc0070, 0x009800e6000f0091, 0x0055003e008000d2, 0x00c100c2004900f5},
    {0x009b00d200810090, 0x00c4003c002e00c6, 0x0038007b00250015, 0x00210040002700b2},
    {0x00e800c200b300ab, 0x001d00d300350036, 0x0066003900630040, 0x004a00ef002f0008},
};

/**
 * @brief Precomputed AVX2 table of \f$\alpha_i^j\f$ powers for syndrome accumulation (part 2/2).
 *
 * Each entry is one 256-bit block used by compute_syndromes() for a fixed
 * codeword position. The 16-bit lanes contain the GF(2^8) powers needed for
 * the final syndrome block.
 *
 * The table has \c PARAM_N1 - 1 entries, one for each non-constant codeword
 * coefficient processed during syndrome accumulation. Unused lanes in this
 * final block are zero-padded.
 */
static const __m256i alpha_ij_vecs_2[45] = {
    {0x00d80048003800e1, 0x00f700a400950073, 0x001e000a00060002, 0x0000000000660022},
    {0x006a00eb0037005c, 0x005300e6009000d9, 0x004f004400140004, 0x0000000000d30068},
    {0x00d300d10044000c, 0x0062003b006700cd, 0x006b009e00780008, 0x0000000000b3007f},
    {0x00830018006200e0, 0x00b500490081006b, 0x00fe00bb000b0010, 0x00000000002f0087},
    {0x0010009a00b300bd, 0x002f007d0061001d, 0x00fa00c2003a0020, 0x000000000040005e},
    {0x002f002b00bb0050, 0x00c3003200d20060, 0x008200d5009c0040, 0x0000000000e80089},
    {0x00fa006d006000ec, 0x008200ba002c003d, 0x00a5004300650080, 0x00000000004a0099},
    {0x009f005b00c3005d, 0x00fc00ea009b0082, 0x001200a80045001b, 0x000000000039008f},
    {0x00e800df00d500ed, 0x00a800ca00070043, 0x00c7007c00850036, 0x0000000000350055},
    {0x001b00c500e800bc, 0x0039008d00c6004a, 0x000200350033006c, 0x0000000000ab00e4},
    {0x00c600cf000700b1, 0x002e005200dd008c, 0x003c00d900aa00d8, 0x000000000008004c},
    {0x0039002900a800b0, 0x00340073002e00c7, 0x009e003b00d100ab, 0x00000000001d00db},
    {0x003300fd00850051, 0x00d1009000aa0038, 0x00d600b500d0004d, 0x0000000000c20093},
    {0x0002009600c7000d, 0x009e00a9003c0026, 0x00e700ae00d6009a, 0x0000000000ef00c0},
    {0x00ab006600350001, 0x001d00b3000800d3, 0x00ef004000c2002f, 0x00000000006300e8},
    {0x00d400f5003400e1, 0x0016001900c4009e, 0x001f00b600ba005e, 0x00000000003600cf},
    {0x00bd00e0000c005c, 0x00ed005d00ec0050, 0x005100b000b100bc, 0x000000000001000d},
    {0x001d007f003b000c, 0x00b60075001500ae, 0x0024008c008b0063, 0x00000000006600a1},
    {0x00200027007f00e0, 0x002d0065006400c3, 0x0095000f001700c6, 0x0000000000d300be},
    {0x005e0020001d00bd, 0x003600cb002500ef, 0x0004006600720097, 0x0000000000b3004d},
    {0x00ef00c300ae0050, 0x0024002900ca006f, 0x007800d100370035, 0x00000000002f00f9},
    {0x00250064001500ec, 0x003800b4007b00ca, 0x0027006b00b2006a, 0x00000000004000fb},
    {0x00cb00650075005d, 0x00d9004800b40029, 0x00b700a3008100d4, 0x0000000000e800bf},
    {0x0036002d00b600ed, 0x006700d900380024, 0x00d50032002b00b3, 0x00000000004a006f},
    {0x009700c6006300bc, 0x00b300d4006a0035, 0x00c500ef00fa007d, 0x0000000000390091},
    {0x00720017008b00b1, 0x002b008100b20037, 0x003e00fc002a00fa, 0x0000000000350084},
    {0x0066000f008c00b0, 0x003200a3006b00d1, 0x00a2004200fc00ef, 0x0000000000ab0038},
    {0x0004009500240051, 0x00d500b700270078, 0x004800a2003e00c5, 0x0000000000080031},
    {0x004d00be00a1000d, 0x006f00bf00fb00f9, 0x0031003800840091, 0x00000000001d0018},
    {0x00b300d300660001, 0x004a00e80040002f, 0x000800ab00350039, 0x0000000000c2001d},
    {0x00610088005300e1, 0x00850099007a0015, 0x00f0006200be0072, 0x0000000000ef00b7},
    {0x003a00570067005c, 0x000f005a002100d5, 0x004e00f900a900e4, 0x0000000000630064},
    {0x00400092006b000c, 0x000a002400a800af, 0x0075006000db00d3, 0x00000000003600fc},
    {0x00bc005d005000e0, 0x000c00e1000d00b0, 0x00b100ed00ec00bd, 0x0000000000010051},
    {0x00c50074002f00bd, 0x0008006a00330036, 0x009100e8005e0061, 0x000000000066006c},
    {0x004a008900320050, 0x00f900cd000a00a2, 0x007c00ca00df00c2, 0x0000000000d300f7},
    {0x008d001f009c00ec, 0x00ae00dc00f500a1, 0x005f008500f4009f, 0x0000000000b3003c},
    {0x006c00790089005d, 0x003d00d600d70034, 0x00900055000e0025, 0x00000000002f00b9},
    {0x0035008c00fc00ed, 0x00df00d200db0044, 0x006200340024004a, 0x00000000004000a3},
    {0x00e4006c004a00bc, 0x00630074007d0008, 0x001000d300d80094, 0x0000000000e800fa},
    {0x00cc0013002900b1, 0x004200c100e700b5, 0x00fb007f00e60033, 0x00000000004a009d},
    {0x0008003400a200b0, 0x007c002d00750092, 0x009c002b00620066, 0x0000000000390007},
    {0x009a0014002e0051, 0x00a1001200230032, 0x00ea0015005700cc, 0x00000000003500ee},
    {0x007d00d7000a000d, 0x003700fd00990075, 0x0079008200e90083, 0x0000000000ab0003},
    {0x00c200b300ab0001, 0x00d30035003600e8, 0x003900630040001d, 0x0000000000080066},
};

/**
 * Coefficients of polynomial G
 * stored in 256-bit values
 **/
static const __m256i rs_generator_vecs[2] = {
    {0x007200e00051004a, 0x0058006e007300dd, 0x0097008b0097005b, 0x00f2001500b80054},
    {0x0071005700c70061, 0x006f00c500b80068, 0x0047009c00510059, 0x0000000100d90080},
};

/**
 * @brief Precomputed evaluation powers for message positions (AVX2, 16-lane blocks).
 *
 * This table stores, for each polynomial power index \c j and each SIMD block of
 * message positions, the field elements needed to evaluate a polynomial at all
 * message points in parallel (used by Chien search and by polynomial evaluation
 * in Forney computations).
 *
 * Layout:
 *  - First dimension \c [PARAM_T + 1] :
 *      power index \c j in {0..PARAM_T}. Row \c j=0 is all ones.
 *  - Second dimension \c [POSITIONS_BLOCKS] :
 *      block index over message positions, where each block covers 16 positions.
 *      \c POSITIONS_BLOCKS = ceil(PARAM_K / 16).
 *  - Third dimension \c [16] :
 *      SIMD lane within a block (one lane per message position).
 */
static const uint16_t beta_inv_pow_lanes[PARAM_T + 1][POSITIONS_BLOCKS][GF_LANES] = {
    {// power j = 0
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
    {// power j = 1
     {54, 18, 14, 243, 81, 198, 66, 62, 227, 168, 145, 134, 139, 121, 222, 74}},
    {// power j = 2
     {99, 31, 84, 67, 177, 37, 175, 35, 88, 182, 128, 142, 223, 157, 126, 239}},
    {// power j = 3
     {239, 213, 117, 156, 237, 64, 61, 195, 21, 50, 194, 210, 22, 96, 174, 47}},
    {// power j = 4
     {194, 78, 160, 174, 236, 125, 163, 105, 240, 249, 154, 206, 127, 208, 136, 8}},
    {// power j = 5
     {29, 16, 154, 179, 189, 131, 8, 77, 212, 211, 204, 4, 171, 106, 228, 102}},
    {// power j = 6
     {8, 59, 205, 209, 12, 171, 38, 52, 10, 115, 53, 46, 15, 199, 162, 57}},
    {// power j = 7
     {171, 235, 170, 247, 225, 114, 15, 180, 132, 242, 141, 14, 168, 207, 119, 99}},
    {// power j = 8
     {53, 255, 246, 162, 13, 141, 243, 134, 70, 7, 197, 101, 137, 42, 218, 64}},
    {// power j = 9
     {57, 140, 243, 139, 176, 99, 111, 182, 130, 117, 64, 21, 210, 174, 43, 29}},
    {// power j = 10
     {74, 27, 197, 232, 188, 159, 64, 250, 58, 47, 97, 16, 179, 131, 77, 211}},
    {// power j = 11
     {232, 157, 218, 61, 93, 32, 43, 240, 73, 158, 77, 184, 83, 89, 6, 53}},
    {// power j = 12
     {64, 50, 96, 43, 80, 179, 120, 103, 68, 217, 102, 56, 85, 36, 242, 54}},
    {// power j = 13
     {47, 105, 118, 158, 224, 204, 217, 34, 95, 15, 151, 143, 66, 70, 31, 232}},
    {// power j = 14
     {179, 24, 178, 83, 92, 216, 85, 253, 138, 66, 203, 84, 182, 100, 201, 194}},
    {// power j = 15
     {211, 171, 102, 53, 1, 57, 54, 74, 99, 232, 239, 64, 194, 47, 29, 179}}};

/**
 * Returns i modulo the given modulus.
 * i must be less than 2*modulus.
 * Therefore, the return value is either i or i-modulus.
 * @returns i mod (modulus)
 * @param[in] i The integer whose modulo is taken
 * @param[in] modulus The modulus
 */
static inline uint16_t mod(uint16_t i, uint16_t modulus) {
    uint16_t tmp = i - modulus;

    // mask = 0xffff if(i < PARAM_GF_MUL_ORDER)
    int16_t mask = -(tmp >> 15);

    return tmp + (mask & modulus);
}

/**
 * @brief Constant-time nonzero mask for 16-bit values.
 *
 * Returns a full-bit mask:
 *  - \c 0xFFFF if \p x != 0
 *  - \c 0x0000 if \p x == 0
 *
 * @param x 16-bit input value.
 * @return 0xFFFF if \p x is nonzero, otherwise 0x0000.
 */
static inline uint16_t make_nonzero_mask(uint16_t x) {
    // 0xFFFF if x != 0 else 0x0000
    uint16_t CT_VOLATILE mask = (uint16_t)(-((x ^ (uint16_t)(-x)) >> 15));
    CT_OPAQUE(mask);
    return mask;
}

/**
 * @brief Constant-time zero test for an 8-bit value.
 *
 * Returns 1 if \p x is zero, else 0.
 *
 * @param x 8-bit input value.
 * @return 1 if \p x == 0, otherwise 0.
 */
static inline uint8_t is_zero(uint8_t x) {
    // returns 1 if x==0 else 0
    uint16_t CT_VOLATILE t = x;
    t |= (uint16_t)(-t);
    t = (((t >> 8) ^ 1) & 1);
    CT_OPAQUE(t);
    return (uint8_t)t;
}

/**
 * @brief Computes the generator polynomial of the primitive Reed-Solomon code with given parameters.
 *
 * Code length is 2^m-1. <br>
 * PARAM_T is the targeted correction capacity of the code
 * and receives the real correction capacity (which is at least equal to the target). <br>
 * gf_exp and gf_log are arrays giving antilog and log of GF(2^m) elements.
 *
 * @param[out] poly Array of size (2*PARAM_T + 1) receiving the coefficients of the generator polynomial
 */
void compute_generator_poly(uint16_t *poly) {
    poly[0] = 1;
    int tmp_degree = 0;

    for (uint16_t i = 1; i < (2 * PARAM_T + 1); ++i) {
        for (size_t j = tmp_degree; j; --j) {
            poly[j] = gf_exp[mod(gf_log[poly[j]] + i, PARAM_GF_MUL_ORDER)] ^ poly[j - 1];
        }

        poly[0] = gf_exp[mod(gf_log[poly[0]] + i, PARAM_GF_MUL_ORDER)];
        poly[++tmp_degree] = 1;
    }

    printf("\n");
    for (int i = 0; i < (PARAM_G); ++i) {
        printf("%d, ", poly[i]);
    }
    printf("\n");
}

/**
 * @brief Encode a message into a systematic Reed–Solomon codeword (AVX2-accelerated).
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
        uint16_t terms[16 * CEIL_DIVIDE(PARAM_G, 16)];
        __m256i align;
    } feedback = {0};

    uint8_t msg_bytes[PARAM_K] = {0};
    uint8_t cdw_bytes[PARAM_N1] = {0};
    __m256i *feedback_vecs = (__m256i *)feedback.terms;

    memcpy(msg_bytes, msg, PARAM_K);

    for (int16_t i = PARAM_K - 1; i >= 0; --i) {
        gate_value = msg_bytes[i] ^ cdw_bytes[PARAM_N1 - PARAM_K - 1];
        __m256i gate_vec = _mm256_set1_epi16(gate_value);
        feedback_vecs[0] = gf_mul_vect(gate_vec, rs_generator_vecs[0]);
        feedback_vecs[1] = gf_mul_vect(gate_vec, rs_generator_vecs[1]);

        for (size_t k = PARAM_N1 - PARAM_K - 1; k; --k) {
            cdw_bytes[k] = cdw_bytes[k - 1] ^ feedback.terms[k];
        }

        cdw_bytes[0] = feedback.terms[0];
    }

    memcpy(cdw_bytes + PARAM_N1 - PARAM_K, msg_bytes, PARAM_K);
    memcpy(cdw, cdw_bytes, PARAM_N1);
}

/**
 * @brief Compute the 2·PARAM_T syndromes (AVX2).
 *
 * Evaluates the received codeword at consecutive powers of the field generator
 * using precomputed α^{i·j} tables packed in 256-bit vectors. The result is
 * written into \p syndrome_vecs (viewed as 16-bit lanes; only low bytes matter).
 *
 * @param[out] syndrome_vecs Output syndrome storage (AVX2 backing array).
 * @param[in]  cdw         Received codeword as bytes (\c PARAM_N1 bytes).
 */
static inline void compute_syndromes(__m256i *syndrome_vecs, const uint8_t *cdw) {
    uint16_t *syndromes = (uint16_t *)syndrome_vecs;
    syndrome_vecs[0] = _mm256_set1_epi16(cdw[0]);

    for (size_t i = 0; i < PARAM_N1 - 1; ++i) {
        syndrome_vecs[0] ^= gf_mul_vect(_mm256_set1_epi16(cdw[i + 1]), alpha_ij_vecs_1[i]);
    }

    __m256i last_syndrome_vecs = _mm256_set1_epi16(cdw[0]);

    for (size_t i = 0; i < PARAM_N1 - 1; ++i) {
        last_syndrome_vecs ^= gf_mul_vect(_mm256_set1_epi16(cdw[i + 1]), alpha_ij_vecs_2[i]);
    }

    memcpy(syndromes + 16, &last_syndrome_vecs, 14 * sizeof(uint16_t));

#ifdef VERBOSE
    printf("\n\nThe syndromes: ");
    for (size_t i = 0; i < 2 * PARAM_T; ++i) {
        printf("%u ", (unsigned)(syndromes[i] & 0xFF));
    }
#endif
}

/**
 * @brief Compute the Reed–Solomon error-locator polynomial (constant-time Berlekamp–Massey).
 *
 * @param[out] sigma     Error-locator polynomial, \c PARAM_T+1 GF(2^8) coefficients.
 * @param[in]  syndrome  Syndromes \f$S_0,\ldots,S_{2\,\texttt{PARAM\_T}-1}\f$.
 */
static inline void compute_error_locator_polynomial(uint16_t *sigma, const uint16_t *syndrome) {
    uint16_t deg_sigma = 0;
    uint16_t sigma_mu[PARAM_T + 1] = {0};
    uint16_t deg_sigma_mu = 0;
    uint16_t x_sigma_rho[PARAM_T + 1] = {0, 1};
    uint16_t deg_sigma_rho = 0;
    uint16_t rho = (uint16_t)-1;
    uint16_t d_rho = 1;
    uint16_t d_mu = syndrome[0];
    uint16_t mu, i;

    sigma[0] = 1;
    for (mu = 0; mu < (2 * PARAM_T); ++mu) {
        uint16_t d_nonzero_mask = (uint16_t)(-((uint16_t)-d_mu >> 15));

        memcpy(sigma_mu, sigma, sizeof sigma_mu);
        deg_sigma_mu = deg_sigma;

        uint16_t d_mu_over_d_rho = gf_mul(d_mu, gf_inverse(d_rho));
        for (i = 1; (i <= mu + 1) && (i <= PARAM_T); ++i) {
            sigma[i] ^= gf_mul(d_mu_over_d_rho, x_sigma_rho[i]);
        }

        uint16_t deg_x_sigma_rho = (uint16_t)((uint16_t)(mu - rho) + deg_sigma_rho);

        uint16_t deg_would_grow_mask = (uint16_t)(-((uint16_t)(deg_sigma_mu - deg_x_sigma_rho) >> 15));

        volatile uint16_t promote_mask__ = d_nonzero_mask & deg_would_grow_mask;
        uint16_t promote_mask = promote_mask__;

        deg_sigma ^= promote_mask & (deg_x_sigma_rho ^ deg_sigma);

        for (i = PARAM_T; i; --i) {
            x_sigma_rho[i] = (promote_mask & sigma_mu[i - 1]) ^ (~promote_mask & x_sigma_rho[i - 1]);
        }
        deg_sigma_rho ^= promote_mask & (deg_sigma_mu ^ deg_sigma_rho);
        rho ^= promote_mask & (mu ^ rho);
        d_rho ^= promote_mask & (d_mu ^ d_rho);

        if (mu + 1 < 2 * PARAM_T) {
            d_mu = syndrome[mu + 1];
            for (i = 1; (i <= mu + 1) && (i <= PARAM_T); ++i) {
                d_mu ^= gf_mul(sigma[i], syndrome[mu + 1 - i]);
            }
        }
    }

    memset_zero(sigma_mu, sizeof sigma_mu);
    memset_zero(x_sigma_rho, sizeof x_sigma_rho);

#ifdef VERBOSE
    printf("\n\nThe error locator polynomial: sigma(x) = ");
    bool first_coeff = true;
    for (size_t i = 0; i <= PARAM_T; ++i) {
        uint16_t c = (uint16_t)(sigma[i] & 0xFF);
        if (c == 0)
            continue;

        if (!first_coeff)
            printf(" + ");
        first_coeff = false;

        if (i == 0) {
            printf("%u", (unsigned)c);
        } else {
            if (c != 1)
                printf("%u*", (unsigned)c);
            if (i == 1)
                printf("x");
            else
                printf("x^%zu", i);
        }
    }
    if (first_coeff)
        printf("0");
#endif
}

/**
 * @brief Compute error positions on the message part using Chien search (AVX2).
 *
 * Evaluates the error-locator polynomial \f$\sigma(x)\f$ at all message evaluation
 * points and outputs a 0/1 flag per message position indicating whether that
 * position is a root (hence an error location).
 *
 * The output layout is message-only:
 * - \c error_positions[i] corresponds to the message byte at codeword index
 *   \c (PARAM_N1 - PARAM_K + i).
 *
 * This implementation processes message positions in fixed 16-lane AVX2 blocks
 * using the precomputed table \c beta_inv_pow_lanes.
 *
 * @param[out] error_positions Output error flags (size \c PARAM_K), each entry is 0 or 1.
 * @param[in]  sigma   Error-locator polynomial coefficients (size \c PARAM_T+1).
 */
static inline void compute_error_positions(uint8_t *error_positions, const uint16_t *sigma) {
    const __m256i vzero = _mm256_setzero_si256();
    const __m256i sigma_zero_vec = _mm256_set1_epi16((uint16_t)(sigma[0] & 0xFF));

    uint8_t flags[POSITIONS_BLOCKS * GF_LANES];

    for (size_t pos_blk = 0; pos_blk < POSITIONS_BLOCKS; ++pos_blk) {
        __m256i sigma_eval = sigma_zero_vec;

        for (size_t coef_idx = 1; coef_idx <= PARAM_T; ++coef_idx) {
            const __m256i sigma_coeff_vec = _mm256_set1_epi16((uint16_t)(sigma[coef_idx] & 0xFF));

            const __m256i eval_pow_vec = _mm256_loadu_si256((const __m256i *)beta_inv_pow_lanes[coef_idx][pos_blk]);

            const __m256i prod = gf_mul_vect(sigma_coeff_vec, eval_pow_vec);
            sigma_eval = _mm256_xor_si256(sigma_eval, prod);
        }

        const __m256i is_root_mask = _mm256_cmpeq_epi16(sigma_eval, vzero);

        uint16_t root_mask_lanes[GF_LANES];
        _mm256_storeu_si256((__m256i *)root_mask_lanes, is_root_mask);

        for (size_t lane = 0; lane < GF_LANES; ++lane) {
            flags[pos_blk * GF_LANES + lane] = (uint8_t)((root_mask_lanes[lane] >> 15) & 1u);
        }
    }

    memcpy(error_positions, flags, PARAM_K);
    memset_zero(flags, sizeof flags);
}

/**
 * @brief Evaluate a polynomial at all message evaluation points (AVX2).
 *
 * Computes \f$y_i = \mathrm{poly}(x_i)\f$ for every message position \f$i \in [0, \texttt{PARAM\_K}-1]\f$
 * using AVX2 over 16 positions at a time. The evaluation points \f$x_i\f$ (with the same neg/pos
 * convention used by this RS implementation) are provided by the precomputed table
 * \c beta_inv_pow_lanes.
 *
 * Output layout is message-only:
 * - \c out[i] corresponds to message index \c i (i.e., codeword index \c PARAM_N1-PARAM_K+i).
 *
 * @param[out] out  Output array of size \c PARAM_K holding \f$\mathrm{poly}(x_i)\f$ (GF(2^8) in low byte).
 * @param[in]  poly Polynomial coefficients, size \c PARAM_T+1 (GF(2^8) in low byte).
 */
static inline void evaluate_poly_at_all_msg_points(uint8_t *out, const uint16_t *poly) {
    uint16_t eval_lanes[GF_LANES];
    uint8_t outbuf[POSITIONS_BLOCKS * GF_LANES];
    for (size_t b = 0; b < POSITIONS_BLOCKS; ++b) {
        __m256i acc = _mm256_set1_epi16((uint16_t)(poly[0] & 0xFF));
        for (uint16_t j = 1; j <= PARAM_T; ++j) {
            __m256i cj = _mm256_set1_epi16((uint16_t)(poly[j] & 0xFF));
            __m256i xj = _mm256_loadu_si256((const __m256i *)&beta_inv_pow_lanes[j][b][0]);
            __m256i prd = gf_mul_vect(cj, xj);
            acc ^= prd;
        }

        _mm256_storeu_si256((__m256i *)eval_lanes, acc);
        for (size_t l = 0; l < GF_LANES; ++l) outbuf[b * GF_LANES + l] = (uint8_t)(eval_lanes[l] & 0xFF);
    }
    memcpy(out, outbuf, PARAM_K);
    memset_zero(outbuf, sizeof outbuf);
}

/**
 * @brief Constant-time batch inversion in GF(2^8).
 *
 * Inverts all PARAM_K elements with a single field inversion:
 * \f$\mathrm{inv}[i] = \big(\prod_{k<i} a[k]\big)\big(\prod_{k>i} a[k]\big)\big(\prod_k a[k]\big)^{-1}\f$.
 * Zero inputs are mapped to 1 so 0 is never inverted.
 *
 * @param[out] inv Output inverses (size \c PARAM_K).
 * @param[in]  a   Input elements (size \c PARAM_K).
 */
static inline void batch_inverse(uint8_t *inv, const uint8_t *a) {
    uint8_t prefix[PARAM_K];
    uint8_t acc = 1;

    for (size_t i = 0; i < PARAM_K; i++) {
        uint8_t a_is_zero = is_zero(a[i]);
        uint8_t safe_a = (uint8_t)(a[i] | a_is_zero);

        prefix[i] = acc;
        acc = gf_mul(acc, safe_a);
    }

    uint8_t inv_all = (uint8_t)gf_inverse(acc);
    uint8_t suffix = 1;
    for (size_t i = PARAM_K; i-- > 0;) {
        uint8_t a_is_zero = is_zero(a[i]);
        uint8_t safe_a = (uint8_t)(a[i] | a_is_zero);
        uint8_t prod_except_i = gf_mul(prefix[i], suffix);
        inv[i] = gf_mul(prod_except_i, inv_all);
        suffix = gf_mul(suffix, safe_a);
    }
}

/**
 * @brief Compute error values on message positions and output the corrected message.
 *
 * This function computes the error magnitudes and applies corrections only for
 * the message bytes, which are stored in the last PARAM_K bytes of the
 * received Reed-Solomon codeword.
 *  1) Compute the auxiliary polynomial Z(x) from \p syndromes and \p sigma.
 *  2) Form the odd part \f$\sigma_{\text{odd}}(x)\f$ (formal derivative in GF(2^8)).
 *  3) Evaluate Z and \f$\sigma_{\text{odd}}\f$ at all message points, batch-invert the
 *     denominators, and compute error values for positions marked in
 *     \p error_positions.
 *  4) Correct the message bytes: \c message[i] = cdw_tail[i] XOR error_value[i].
 *
 * @param[out] message          Decoded message (size \c PARAM_K).
 * @param[in]  cdw_bytes        Received codeword bytes (size \c PARAM_N1).
 * @param[in]  syndromes        Syndromes \f$S_0..S_{2t-1}\f$ (low bytes used).
 * @param[in]  sigma            Error-locator polynomial (size \c PARAM_T+1).
 * @param[in]  error_positions  Message-only error flags (size \c PARAM_K, entries 0/1).
 */
static inline void correct_errors(uint8_t *message, const uint8_t *cdw_bytes, const uint16_t *syndromes,
                                  const uint16_t *sigma, const uint8_t *error_positions) {
    uint16_t z[PARAM_T + 1] = {0};
#ifdef VERBOSE
    uint8_t error_values[PARAM_K] = {0};
#endif
    z[0] = 1;

    for (uint16_t i = 0; i < PARAM_T; ++i) {
        z[i + 1] = syndromes[i];

        for (uint16_t j = 0; j < i; ++j) {
            uint16_t sj1 = sigma[j + 1];
            uint16_t sj2 = syndromes[i - 1 - j];
            z[i + 1] ^= gf_mul(sj1, sj2);
        }

        z[i + 1] ^= sigma[i + 1];
    }

    uint16_t sigma_odd[PARAM_T + 1] = {0};
    for (uint16_t i = 1; i <= PARAM_T; i += 2) {
        sigma_odd[i] = sigma[i];
    }

    uint8_t numerator_array[PARAM_K] = {0};
    uint8_t denominator_array[PARAM_K] = {0};
    evaluate_poly_at_all_msg_points(numerator_array, z);
    evaluate_poly_at_all_msg_points(denominator_array, sigma_odd);

    uint8_t inverse_denominator_array[PARAM_K] = {0};
    batch_inverse(inverse_denominator_array, denominator_array);

    for (size_t i = 0; i < PARAM_K; ++i) {
        uint16_t has_error = make_nonzero_mask((uint16_t)error_positions[i]);
        uint16_t den_nonzero = make_nonzero_mask((uint16_t)denominator_array[i]);
        uint16_t valid = has_error & den_nonzero;

        uint8_t mag = gf_mul(numerator_array[i], inverse_denominator_array[i]);
        uint8_t corr = (uint8_t)(valid & mag);
        message[i] = cdw_bytes[i + (PARAM_N1 - PARAM_K)] ^ corr;
#ifdef VERBOSE
        error_values[i] = corr;
#endif
    }

#ifdef VERBOSE
    printf("\n\nThe polynomial: Z(x) = ");
    bool first_z = true;
    for (size_t i = 0; i <= PARAM_T; ++i) {
        uint16_t c = (uint16_t)(z[i] & 0xFF);
        if (c == 0)
            continue;

        if (!first_z)
            printf(" + ");
        first_z = false;

        if (i == 0) {
            printf("%u", (unsigned)c);
        } else {
            if (c != 1)
                printf("%u*", (unsigned)c);
            if (i == 1)
                printf("x");
            else
                printf("x^%zu", i);
        }
    }
    if (first_z)
        printf("0");

    printf("\n\nError locations / magnitudes: ");
    size_t n_err = 0;
    for (size_t i = 0; i < PARAM_K; ++i) {
        n_err += (error_positions[i] != 0);
    }

    if (n_err == 0) {
        printf("none (no errors detected in message positions)\n");
    } else {
        printf("%zu error%s: ", n_err, (n_err == 1) ? "" : "s");
        for (size_t i = 0; i < PARAM_K; ++i) {
            if (error_positions[i]) {
                size_t pos_cw = (size_t)(PARAM_N1 - PARAM_K) + i;
                printf("(%zu, %u) ", pos_cw, (unsigned)error_values[i]);
            }
        }
        printf("\nMessage indices: ");
        for (size_t i = 0; i < PARAM_K; ++i) {
            if (error_positions[i])
                printf("%zu ", i);
        }
        printf("\n");
    }
#endif

    memset_zero(z, sizeof z);
    memset_zero(sigma_odd, sizeof sigma_odd);
    memset_zero(numerator_array, sizeof numerator_array);
    memset_zero(denominator_array, sizeof denominator_array);
    memset_zero(inverse_denominator_array, sizeof inverse_denominator_array);
}

/**
 * @brief Decode a Reed–Solomon codeword and recover the message (message-only correction).
 *
 * Steps:
 *  1) Copy the received codeword (\p cdw) into a local byte buffer.
 *  2) Compute the 2·PARAM_T syndromes.
 *  3) Run Berlekamp–Massey to compute the error-locator polynomial σ(x).
 *  4) Run Chien search over the message positions to get \p error_positions (0/1 flags).
 *  5) Compute z(x), error values (Forney), and correct the message bytes.
 *  6) Copy the corrected message to \p msg and zeroize temporary buffers.
 *
 * @param[out] msg Output message buffer (expects \c PARAM_K bytes).
 * @param[in]  cdw Input codeword buffer (expects \c PARAM_N1 bytes).
 */
void reed_solomon_decode(uint64_t *msg, uint64_t *cdw) {
    uint8_t cdw_bytes[PARAM_N1] = {0};
    __m256i syndrome_vecs[SYND_VEC_COUNT];
    uint16_t *syndromes = (uint16_t *)syndrome_vecs;
    uint16_t sigma[PARAM_T + 1] = {0};
    uint8_t error_positions[PARAM_K] = {0};
    uint8_t message[PARAM_K] = {0};

    memcpy(cdw_bytes, cdw, PARAM_N1);

    compute_syndromes(syndrome_vecs, cdw_bytes);
    compute_error_locator_polynomial(sigma, syndromes);
    compute_error_positions(error_positions, sigma);
    correct_errors(message, cdw_bytes, syndromes, sigma, error_positions);

    memcpy(msg, message, PARAM_K);

    memset_zero(cdw_bytes, sizeof cdw_bytes);
    memset_zero(syndrome_vecs, sizeof syndrome_vecs);
    memset_zero(sigma, sizeof sigma);
    memset_zero(error_positions, sizeof error_positions);
    memset_zero(message, sizeof message);
}
