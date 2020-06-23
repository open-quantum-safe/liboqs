/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: supersingular isogeny parameters and generation of functions for P610
*********************************************************************************************/

#include <oqs/rand.h>
#include "../../oqs_namespace_sike.h"
#include "P610_api.h"
#include "P610_internal.h"
#include "../internal.h"

// defines moved from P610_api.h
#define CRYPTO_SECRETKEYBYTES 524 // MSG_BYTES + SECRETKEY_B_BYTES + CRYPTO_PUBLICKEYBYTES bytes
#define CRYPTO_PUBLICKEYBYTES 462
#define CRYPTO_BYTES 24
#define CRYPTO_CIPHERTEXTBYTES 486 // CRYPTO_PUBLICKEYBYTES + MSG_BYTES bytes
#define SIDH_SECRETKEYBYTES_A 39
#define SIDH_SECRETKEYBYTES_B 38
#define SIDH_PUBLICKEYBYTES 462
#define SIDH_BYTES 154

// Encoding of field elements, elements over Z_order, elements over GF(p^2) and elliptic curve points:
// --------------------------------------------------------------------------------------------------
// Elements over GF(p) and Z_order are encoded with the least significant octet (and digit) located at the leftmost position (i.e., little endian format).
// Elements (a+b*i) over GF(p^2), where a and b are defined over GF(p), are encoded as {a, b}, with a in the least significant position.
// Elliptic curve points P = (x,y) are encoded as {x, y}, with x in the least significant position.
// Internally, the number of digits used to represent all these elements is obtained by approximating the number of bits to the immediately greater multiple of 32.
// For example, a 610-bit field element is represented with Ceil(610 / 64) = 10 64-bit digits or Ceil(610 / 32) = 20 32-bit digits.

//
// Curve isogeny system "SIDHp610". Base curve: Montgomery curve By^2 = Cx^3 + Ax^2 + Cx defined over GF(p610^2), where A=6, B=1, C=1 and p610 = 2^305*3^192-1
//

const uint64_t p610[NWORDS64_FIELD] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0x6E01FFFFFFFFFFFF,
                                       0xB1784DE8AA5AB02E, 0x9AE7BF45048FF9AB, 0xB255B2FA10C4252A, 0x819010C251E7D88C, 0x000000027BF6A768
                                      };
const uint64_t p610x2[NWORDS64_FIELD] = {0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xDC03FFFFFFFFFFFF,
                                         0x62F09BD154B5605C, 0x35CF7E8A091FF357, 0x64AB65F421884A55, 0x03202184A3CFB119, 0x00000004F7ED4ED1
                                        };
const uint64_t p610x4[NWORDS64_FIELD]            = { 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xB807FFFFFFFFFFFF, 
                                                     0xC5E137A2A96AC0B9, 0x6B9EFD14123FE6AE, 0xC956CBE8431094AA, 0x06404309479F6232, 0x00000009EFDA9DA2 };
const uint64_t p610p1[NWORDS64_FIELD]            = { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x6E02000000000000,
                                                     0xB1784DE8AA5AB02E, 0x9AE7BF45048FF9AB, 0xB255B2FA10C4252A, 0x819010C251E7D88C, 0x000000027BF6A768 };   
/* OQS note: commented out to avoid unused error
static const uint64_t p610x16p[2*NWORDS64_FIELD]        = { 0x0000000000000010, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x3FC0000000000000, 
                                                     0xD0F642EAB4A9FA32, 0xA308175F6E00CA89, 0xB549A0BDE77B5AAC, 0xCDFDE7B5C304EE69, 0x7FDB7FF0812B12EF, 
                                                     0xE09BA529B9FE1167, 0xD249C196DAB8CD7F, 0xD4E22754A3F20928, 0x97825638B19A7CCE, 0x05E04550FC4CCE0D, 
                                                     0x8FB5DA1152CDE50C, 0xF9649BA3EA408644, 0x4473C93E6441063D, 0xBE190269D1337B7B, 0x0000000000000062 }; 
*/
// Order of Alice's subgroup
// static const uint64_t Alice_order[NWORDS64_ORDER] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0002000000000000};
// Order of Bob's subgroup
// static const uint64_t Bob_order[NWORDS64_ORDER] = {0x26F4552D58173701, 0xDFA28247FCD5D8BC, 0xD97D086212954D73, 0x086128F3EC46592A, 0x00013DFB53B440C8};
// Alice's generator values {XPA0 + XPA1*i, XQA0 + xQA1*i, XRA0 + XRA1*i} in GF(p610^2), expressed in Montgomery representation
static const uint64_t A_gen[6 * NWORDS64_FIELD] = {0x5019EC96A75AC57A, 0x8AEA0E717712C6F1, 0x03C067C819D29E5E, 0x59F454425FE307D9, 0x6D29215D9AD5E6D4,
                                                   0xD8C5A27CDC9DD34A, 0x972DC274DAB435B3, 0x82A597C70A80E10F, 0x48175986EFED547F, 0x00000000671A3592, // XPA0
                                                   0xE4BA9CC3EEEC53F4, 0xBD34E4FEDB0132D3, 0x1B7125C87BEE960C, 0x25D615BF3CFAA355, 0xFC8EC20DC367D66A,
                                                   0xB44F3FD1CC73289C, 0xD84BF51195C2E012, 0x38D7C756EB370F48, 0xBBC236249F94F72A, 0x000000013020CC63, // XPA1
                                                   0x1D7C945D3DBCC38C, 0x9A5F7C12CA8BA5B9, 0x1E8F87985B01CBE3, 0xD2CABF82F5BC5235, 0x3BDE474ECCA9FAA2,
                                                   0xB98CD975DF9FB0A8, 0x444E4464B9C67790, 0xCB2E888565CE6AD9, 0xDB64FFE2A1C350E2, 0x00000001D7532756, // XQA0
                                                   0x1E8B3AA2382C9079, 0x28CB31E08A943C00, 0xE04D02266E8A63E1, 0x84A2D260214EF65F, 0xD5933DA25018E226,
                                                   0xBC8BF038928C4BA9, 0x91E9D0CB7EAF58A9, 0x04A4627B75E008E1, 0x58CEF27583E50C2E, 0x00000002170DDF44, // XQA1
                                                   0x261DD0782CEC958D, 0xC25B3AE64BBC0311, 0x9F21B8A8981B15FE, 0xA3C0B52CD5FFC45B, 0x5D2E65A016702C6A,
                                                   0x8C5586CA98722EDE, 0x61490A967A6B4B1A, 0xFA64E30231F719AF, 0x9CEAB8B6301BB2DF, 0x00000000CF5AEA7D, // XRA0
                                                   0xB980435A77B912C0, 0x2B4A97F70E0FC873, 0x415C7FA4DE96F43C, 0xE5EED95643E443FD, 0xCBE18DB57C51B354,
                                                   0x51C96C3FFABD2D46, 0x5C14637B9A5765D6, 0x45D2369C4D0199A5, 0x25A1F9C5BBF1E683, 0x000000025AD7A11B
                                                  }; // XRA1
// Bob's generator values {XPB0, XQB0, XRB0 + XRB1*i} in GF(p610^2), expressed in Montgomery representation
static const uint64_t B_gen[6 * NWORDS64_FIELD] = {0xC6C8E180E41884BA, 0x2161D2F4FBC32B95, 0xCBF83091BDB34092, 0xD742CC0AD4CC7E38, 0x61A1FA7E1B14FBD7,
                                                   0xF0E5FC70137597C4, 0x1F0C8F2585E20B1F, 0xC68E44A1C032A4C2, 0xE3C65FB8AF155A0D, 0x00000001409EE8D5, // XPB0
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, // XPB1
                                                   0xF586DB4A16BE1880, 0x712F10D95E6C65A9, 0x9D5AAC3B83584B87, 0x4ECDAA98182C8261, 0xAD7D4C15588FD230,
                                                   0x4197C54E96B7D926, 0xED15BB13E8C588ED, 0x3E299AEAD5AAD7C7, 0xF36B25F1BD579F79, 0x000000021CE65B5B, // XQB0
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, // XQB1
                                                   0x7A87897A0C4C3FD7, 0x3C1879ECD4D33D76, 0x595C28A36FFBA1A0, 0xF53FF66A2A7FD0FB, 0xB39F5A91230E56FA,
                                                   0x81F21610DA3EA8B5, 0xEBB3B9A627428A90, 0x8661123B35748010, 0xE196173B9C48781D, 0x00000002198166AC, // XRB0
                                                   0x5E3CC79B37006D6A, 0xE0358A9AB2EA7923, 0x3B725CB595180951, 0x0724637F1DD0C191, 0x7BB031B67DAB9D19,
                                                   0x53CCB8BECEDD3435, 0xEE5DF7FFEBFA7A0A, 0x899EDB7D8B9694C4, 0x0CA38EB4AE5506B6, 0x00000001489DE1CD
                                                  }; // XRB1
// Montgomery constant Montgomery_R2 = (2^640)^2 mod p610
static const uint64_t Montgomery_R2[NWORDS64_FIELD] = {0xE75F5D201A197727, 0xE0B85963B627392E, 0x6BC1707818DE493D, 0xDC7F419940D1A0C5, 0x7358030979EDE54A,
                                                       0x84F4BEBDEED75A5C, 0x7ECCA66E13427B47, 0xC5BB4E65280080B3, 0x7019950F516DA19A, 0x000000008E290FF3
                                                      };
// Value one in Montgomery representation
static const uint64_t Montgomery_one[NWORDS64_FIELD] = {0x00000000670CC8E6, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x9A34000000000000,
                                                        0x4D99C2BD28717A3F, 0x0A4A1839A323D41C, 0xD2B62215D06AD1E2, 0x1369026E862CAF3D, 0x000000010894E964
                                                       };

// Fixed parameters for isogeny tree computation
static const unsigned int strat_Alice[MAX_Alice - 1] = {
	67, 37, 21, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1,
	2, 1, 1, 1, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 16, 9,
	5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 2, 1,
	1, 4, 2, 1, 1, 2, 1, 1, 33, 16, 8, 5, 2, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 1,
	1, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 16, 8, 4, 2, 1, 1, 1, 2, 1, 1,
	4, 2, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1
};

static const unsigned int strat_Bob[MAX_Bob - 1] = {
	86, 48, 27, 15, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1,
	1, 3, 2, 1, 1, 1, 1, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1,
	1, 1, 2, 1, 1, 1, 21, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1,
	1, 1, 2, 1, 1, 1, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 38,
	21, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1,
	9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 17, 9, 5, 3, 2, 1, 1,
	1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2,
	1, 1
};

// Setting up macro defines and including GF(p), GF(p^2), curve, isogeny and kex functions
#define fpcopy fpcopy610
#define fpzero fpzero610
#define fpadd fpadd610
#define fpsub fpsub610
#define fpneg fpneg610
#define fpdiv2 fpdiv2_610
#define fpcorrection fpcorrection610
#define fpmul_mont fpmul610_mont
#define fpsqr_mont fpsqr610_mont
#define fpinv_mont fpinv610_mont
#define fpinv_chain_mont fpinv610_chain_mont
#define fpinv_mont_bingcd fpinv610_mont_bingcd
#define fp2copy fp2copy610
#define fp2zero fp2zero610
#define fp2add fp2add610
#define fp2sub fp2sub610
#define mp_sub_p2 mp_sub610_p2
#define mp_sub_p4 mp_sub610_p4
#define sub_p4 mp_sub_p4
#define fp2neg fp2neg610
#define fp2div2 fp2div2_610
#define fp2correction fp2correction610
#define fp2mul_mont fp2mul610_mont
#define fp2sqr_mont fp2sqr610_mont
#define fp2inv_mont fp2inv610_mont
#define fp2inv_mont_bingcd fp2inv610_mont_bingcd
#define fpequal_non_constant_time fpequal610_non_constant_time
#define mp_add_asm oqs_kem_sike_mp_add610_asm
#define mp_subaddx2_asm oqs_kem_sike_mp_subadd610x2_asm
#define mp_dblsubx2_asm oqs_kem_sike_mp_dblsub610x2_asm
#define crypto_kem_keypair OQS_KEM_sike_p610_keypair
#define crypto_kem_enc OQS_KEM_sike_p610_encaps
#define crypto_kem_dec OQS_KEM_sike_p610_decaps
#define random_mod_order_A oqs_kem_sidh_p610_random_mod_order_A
#define random_mod_order_B oqs_kem_sidh_p610_random_mod_order_B
#define EphemeralKeyGeneration_A oqs_kem_sidh_p610_EphemeralKeyGeneration_A
#define EphemeralKeyGeneration_B oqs_kem_sidh_p610_EphemeralKeyGeneration_B
#define EphemeralSecretAgreement_A oqs_kem_sidh_p610_EphemeralSecretAgreement_A
#define EphemeralSecretAgreement_B oqs_kem_sidh_p610_EphemeralSecretAgreement_B
#ifdef USE_SIKEP610_ASM
#define USE_SIKE_ASM
#endif

#if defined(_AMD64_) && defined(USE_SIKE_ASM)
#include "AMD64/fp_x64.c"
#elif defined(_ARM64_)
#include "ARM64/fp_arm64.c"
#else
#include "generic/fp_generic.c"
#endif

#include "../fpx.c"
#include "../ec_isogeny.c"
#include "../sidh.c"
#include "../sike.c"
