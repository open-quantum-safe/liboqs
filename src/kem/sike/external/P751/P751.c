/********************************************************************************************
* SIDH: an efficient supersingular isogeny cryptography library
*
* Abstract: supersingular isogeny parameters and generation of functions for P751
*********************************************************************************************/

#include <oqs/rand.h>
#include "../../oqs_namespace_sike.h"
#include "P751_api.h"
#include "P751_internal.h"
#include "../internal.h"

// defines moved from P751_api.h
#define CRYPTO_SECRETKEYBYTES 644 // MSG_BYTES + SECRETKEY_B_BYTES + CRYPTO_PUBLICKEYBYTES bytes
#define CRYPTO_PUBLICKEYBYTES 564
#define CRYPTO_BYTES 32
#define CRYPTO_CIPHERTEXTBYTES 596 // CRYPTO_PUBLICKEYBYTES + MSG_BYTES bytes
#define SIDH_SECRETKEYBYTES_A 47
#define SIDH_SECRETKEYBYTES_B 48
#define SIDH_PUBLICKEYBYTES 564
#define SIDH_BYTES 188

// Encoding of field elements, elements over Z_order, elements over GF(p^2) and elliptic curve points:
// --------------------------------------------------------------------------------------------------
// Elements over GF(p) and Z_order are encoded with the least significant octet (and digit) located at the leftmost position (i.e., little endian format).
// Elements (a+b*i) over GF(p^2), where a and b are defined over GF(p), are encoded as {a, b}, with a in the least significant position.
// Elliptic curve points P = (x,y) are encoded as {x, y}, with x in the least significant position.
// Internally, the number of digits used to represent all these elements is obtained by approximating the number of bits to the immediately greater multiple of 32.
// For example, a 751-bit field element is represented with Ceil(751 / 64) = 12 64-bit digits or Ceil(751 / 32) = 24 32-bit digits.

//
// Curve isogeny system "SIDHp751". Base curve: Montgomery curve By^2 = Cx^3 + Ax^2 + Cx defined over GF(p751^2), where A=6, B=1, C=1 and p751 = 2^372*3^239-1
//

const uint64_t p751[NWORDS64_FIELD] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xEEAFFFFFFFFFFFFF,
                                       0xE3EC968549F878A8, 0xDA959B1A13F7CC76, 0x084E9867D6EBE876, 0x8562B5045CB25748, 0x0E12909F97BADC66, 0x00006FE5D541F71C
                                      };
const uint64_t p751x2[NWORDS64_FIELD] = {0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xDD5FFFFFFFFFFFFF,
                                         0xC7D92D0A93F0F151, 0xB52B363427EF98ED, 0x109D30CFADD7D0ED, 0x0AC56A08B964AE90, 0x1C25213F2F75B8CD, 0x0000DFCBAA83EE38
                                        };
const uint64_t p751x4[NWORDS64_FIELD]            = { 0xFFFFFFFFFFFFFFFC, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xBABFFFFFFFFFFFFF, 
                                                     0x8FB25A1527E1E2A3, 0x6A566C684FDF31DB, 0x213A619F5BAFA1DB, 0x158AD41172C95D20, 0x384A427E5EEB719A, 0x0001BF975507DC70 }; 
const uint64_t p751p1[NWORDS64_FIELD]            = { 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0xEEB0000000000000,
                                                     0xE3EC968549F878A8, 0xDA959B1A13F7CC76, 0x084E9867D6EBE876, 0x8562B5045CB25748, 0x0E12909F97BADC66, 0x00006FE5D541F71C };   
/* OQS note: commented out to avoid unused error
static const uint64_t p751x16p[2*NWORDS64_FIELD]        = { 0x0000000000000010, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x2A00000000000000, 
                                                     0x826D2F56C0F0EAE2, 0xAD4C9CBD81067123, 0xF62CF3052282F124, 0x53A95F7469B516FE, 0x3DADEC0D08A4732F, 0x58AD934557C11C7E, 
                                                     0x7F731B89B2DA43F2, 0x51AE9F5F5F6AFF3B, 0xD74319A6C9BCA375, 0x5BAB790796CF84D4, 0xA421554FE2E49CA8, 0x20AD617C8DF437CF, 
                                                     0x3AB06E7A12F5FF7B, 0x70A25E037E40347E, 0x51F1D323FB4C1151, 0xAE0D99AA4835FED9, 0xDF5429960D2536B6, 0x000000030E91D466 };
*/
// Order of Alice's subgroup
// static const uint64_t Alice_order[NWORDS64_ORDER] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0010000000000000};
// Order of Bob's subgroup
// static const uint64_t Bob_order[NWORDS64_ORDER] = {0xC968549F878A8EEB, 0x59B1A13F7CC76E3E, 0xE9867D6EBE876DA9, 0x2B5045CB25748084, 0x2909F97BADC66856, 0x06FE5D541F71C0E1};
// Alice's generator values {XPA0 + XPA1*i, XQA0 + xQA1*i, XRA0 + XRA1*i} in GF(p751^2), expressed in Montgomery representation
static const uint64_t A_gen[6 * NWORDS64_FIELD] = {0x884F46B74000BAA8, 0xBA52630F939DEC20, 0xC16FB97BA714A04D, 0x082536745B1AB3DB, 0x1117157F446F9E82, 0xD2F27D621A018490,
                                                   0x6B24AB523D544BCD, 0x9307D6AA2EA85C94, 0xE1A096729528F20F, 0x896446F868F3255C, 0x2401D996B1BFF8A5, 0x00000EF8786A5C0A, // XPA0
                                                   0xAEB78B3B96F59394, 0xAB26681E29C90B74, 0xE520AC30FDC4ACF1, 0x870AAAE3A4B8111B, 0xF875BDB738D64EFF, 0x50109A7ECD7ED6BC,
                                                   0x4CC64848FF0C56FB, 0xE617CB6C519102C9, 0x9C74B3835921E609, 0xC91DDAE4A35A7146, 0x7FC82A155C1B9129, 0x0000214FA6B980B3, // XPA1
                                                   0x0F93CC38680A8CA9, 0x762E733822E7FED7, 0xE549F005AC0ADB67, 0x94A71FDD2C43A4ED, 0xD48645C2B04721C5, 0x432DA1FE4D4CA4DC,
                                                   0xBC99655FAA7A80E8, 0xB2C6D502BCFD4823, 0xEE92F40CA2EC8BDB, 0x7B074132EFB6D16C, 0x3340B46FA38A7633, 0x0000215749657F6C, // XQA0
                                                   0xECFF375BF3079F4C, 0xFBFE74B043E80EF3, 0x17376CBE3C5C7AD1, 0xC06327A7E29CDBF2, 0x2111649C438BF3D4, 0xC1F9298261BA2E97,
                                                   0x1F9FECE869CFD1C2, 0x01A39B4FC9346D62, 0x147CD1D3E82A3C9F, 0xDE84E9D249E533EE, 0x1C48A5ADFB7C578D, 0x000061ACA0B82E1D, // XQA1
                                                   0x1600C525D41059F1, 0xA596899A0A1D83F7, 0x6BFDEED6D2B23F35, 0x5C7E707270C23910, 0x276CA1A4E8369411, 0xB193651A602925A0,
                                                   0x243D239F1CA1F04A, 0x543DC6DA457860AD, 0xCDA590F325181DE9, 0xD3AB7ACFDA80B395, 0x6C97468580FDDF7B, 0x0000352A3E5C4C77, // XRA0
                                                   0x9B794F9FD1CC3EE8, 0xDB32E40A9B2FD23E, 0x26192A2542E42B67, 0xA18E94FCA045BCE7, 0x96DC1BC38E7CDA2D, 0x9A1D91B752487DE2,
                                                   0xCC63763987436DA3, 0x1316717AACCC551D, 0xC4C368A4632AFE72, 0x4B6EA85C9CCD5710, 0x7A12CAD582C7BC9A, 0x00001C7E240149BF
                                                  }; // XRA1
// Bob's generator values {XPB0, XQB0, XRB0 + XRB1*i} in GF(p751^2), expressed in Montgomery representation
static const uint64_t B_gen[6 * NWORDS64_FIELD] = {0x85691AAF4015F88C, 0x7478C5B8C36E9631, 0x7EF2A185DE4DD6E2, 0x943BBEE46BEB9DC7, 0x1A3EC62798792D22, 0x791BC4B084B31D69,
                                                   0x03DBE6522CEA17C4, 0x04749AA65D665D83, 0x3D52B5C45EF450F3, 0x0B4219848E36947D, 0xA4CF7070466BDE27, 0x0000334B1FA6D193, // XPB0
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, // XPB1
                                                   0x8E7CB3FA53211340, 0xD67CE54F7A05EEE0, 0xFDDC2C8BCE46FC38, 0x08587FAE3110DF1E, 0xD6B8246FA22B058B, 0x4DAC3ACC905A5DBD,
                                                   0x51D0BF2FADCED3E8, 0xE5A2406DF6484425, 0x907F177584F671B8, 0x4738A2FFCCED051C, 0x2B0067B4177E4853, 0x00002806AC948D3D, // XQB0
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
                                                   0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, // XQB1
                                                   0xB56457016D1D6D1C, 0x03DECCB38F39C491, 0xDFB910AC8A559452, 0xA9D0F17D1FF24883, 0x8562BBAF515C248C, 0x249B2A6DDB1CB67D,
                                                   0x3131AF96FB46835C, 0xE10258398480C3E1, 0xEAB5E2B872D4FAB1, 0xB71E63875FAEB1DF, 0xF8384D4F13757CF6, 0x0000361EC9B09912, // XRB0
                                                   0x58C967899ED16EF4, 0x81998376DC622A4B, 0x3D1C1DCFE0B12681, 0x9347DEBB953E1730, 0x9ABB344D3A82C2D7, 0xE4881BD2820552B2,
                                                   0x0037247923D90266, 0x2E3156EDB157E5A5, 0xF86A46A7506823F7, 0x8FE5523A7B7F1CFC, 0xFA3CFFA38372F67B, 0x0000692DCE85FFBD
                                                  }; // XRB1
// Montgomery constant Montgomery_R2 = (2^768)^2 mod p751
static const uint64_t Montgomery_R2[NWORDS64_FIELD] = {0x233046449DAD4058, 0xDB010161A696452A, 0x5E36941472E3FD8E, 0xF40BFE2082A2E706, 0x4932CCA8904F8751, 0x1F735F1F1EE7FC81,
                                                       0xA24F4D80C1048E18, 0xB56C383CCDB607C5, 0x441DD47B735F9C90, 0x5673ED2C6A6AC82A, 0x06C905261132294B, 0x000041AD830F1F35
                                                      };
// Value one in Montgomery representation
static const uint64_t Montgomery_one[NWORDS64_FIELD] = {0x00000000000249ad, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x8310000000000000,
                                                        0x5527b1e4375c6c66, 0x697797bf3f4f24d0, 0xc89db7b2ac5c4e2e, 0x4ca4b439d2076956, 0x10f7926c7512c7e9, 0x00002d5b24bce5e2
                                                       };

// Fixed parameters for isogeny tree computation
static const unsigned int strat_Alice[MAX_Alice - 1] = {
	80, 48, 27, 15, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1,
	1, 3, 2, 1, 1, 1, 1, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1,
	1, 1, 2, 1, 1, 1, 21, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1,
	1, 1, 1, 2, 1, 1, 1, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1,
	33, 20, 12, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 5, 3, 2, 1, 1, 1, 1, 2, 1,
	1, 1, 8, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 2, 1, 1, 16, 8, 4, 2, 1, 1,
	1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1
};

static const unsigned int strat_Bob[MAX_Bob - 1] = {
	112, 63, 32, 16, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 2, 1,
	1, 4, 2, 1, 1, 2, 1, 1, 16, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 8, 4, 2,
	1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 31, 16, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2,
	1, 1, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 15, 8, 4, 2, 1, 1, 2, 1, 1, 4,
	2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1, 1, 1, 1, 49, 31, 16, 8, 4, 2,
	1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1,
	15, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 7, 4, 2, 1, 1, 2, 1, 1, 3, 2, 1,
	1, 1, 1, 21, 12, 8, 4, 2, 1, 1, 2, 1, 1, 4, 2, 1, 1, 2, 1, 1, 5, 3, 2, 1, 1, 1, 1,
	2, 1, 1, 1, 9, 5, 3, 2, 1, 1, 1, 1, 2, 1, 1, 1, 4, 2, 1, 1, 1, 2, 1, 1
};

// Setting up macro defines and including GF(p), GF(p^2), curve, isogeny and kex functions
#define fpcopy fpcopy751
#define fpzero fpzero751
#define fpadd fpadd751
#define fpsub fpsub751
#define fpneg fpneg751
#define fpdiv2 fpdiv2_751
#define fpcorrection fpcorrection751
#define fpmul_mont fpmul751_mont
#define fpsqr_mont fpsqr751_mont
#define fpinv_mont fpinv751_mont
#define fpinv_chain_mont fpinv751_chain_mont
#define fpinv_mont_bingcd fpinv751_mont_bingcd
#define fp2copy fp2copy751
#define fp2zero fp2zero751
#define fp2add fp2add751
#define fp2sub fp2sub751
#define mp_sub_p2 mp_sub751_p2
#define mp_sub_p4 mp_sub751_p4
#define sub_p4 mp_sub_p4
#define fp2neg fp2neg751
#define fp2div2 fp2div2_751
#define fp2correction fp2correction751
#define fp2mul_mont fp2mul751_mont
#define fp2sqr_mont fp2sqr751_mont
#define fp2inv_mont fp2inv751_mont
#define fp2inv_mont_bingcd fp2inv751_mont_bingcd
#define fpequal_non_constant_time fpequal751_non_constant_time
#define mp_add_asm oqs_kem_sike_mp_add751_asm
#define mp_subaddx2_asm oqs_kem_sike_mp_subadd751x2_asm
#define mp_dblsubx2_asm oqs_kem_sike_mp_dblsub751x2_asm
#define crypto_kem_keypair OQS_KEM_sike_p751_keypair
#define crypto_kem_enc OQS_KEM_sike_p751_encaps
#define crypto_kem_dec OQS_KEM_sike_p751_decaps
#define random_mod_order_A oqs_kem_sidh_p751_random_mod_order_A
#define random_mod_order_B oqs_kem_sidh_p751_random_mod_order_B
#define EphemeralKeyGeneration_A oqs_kem_sidh_p751_EphemeralKeyGeneration_A
#define EphemeralKeyGeneration_B oqs_kem_sidh_p751_EphemeralKeyGeneration_B
#define EphemeralSecretAgreement_A oqs_kem_sidh_p751_EphemeralSecretAgreement_A
#define EphemeralSecretAgreement_B oqs_kem_sidh_p751_EphemeralSecretAgreement_B
#ifdef USE_SIKEP751_ASM
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
