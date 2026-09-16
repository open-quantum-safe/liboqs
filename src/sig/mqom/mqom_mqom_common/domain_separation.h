#ifndef __DOMAIN_SEPARATION_H__
#define __DOMAIN_SEPARATION_H__

/* Single-byte domain-separation tags prepended to every XOF_i/Hash_i call,
 * per specs/version3/sections/02-04-05-sym.tex. Centralized here so a spec
 * renumbering is a one-line diff instead of a codebase-wide search.
 *
 * Defined as 1-byte string literals (not integers) so every call site stays
 * the same shape it already was: xof_update(ctx, (const uint8_t*) DOMAIN_SEP_X, 1). */

#define DOMAIN_SEP_SEEDEQ      "\x00"  /* XOF_0: seedeq expansion (KeyGen)                */
#define DOMAIN_SEP_COM2        "\x01"  /* Hash_1: com2 = Hash(alpha0, alpha1)             */
#define DOMAIN_SEP_PRESIGID    "\x02"  /* Hash_2: presigid = Hash(pk, com1, com2)         */
#define DOMAIN_SEP_MSGDIGEST   "\x03"  /* Hash_3: msgdigest = Hash(msg)                   */
#define DOMAIN_SEP_SIGID       "\x04"  /* Hash_4: sigid = Hash(presigid, msgdigest)       */
#define DOMAIN_SEP_GRINDING    "\x05"  /* XOF_5: grinding material (g0, g1, k0, k1)       */
#define DOMAIN_SEP_CHALLENGE   "\x06"  /* XOF_6: i* derivation (sigid, nonce, c0, c1)     */
#define DOMAIN_SEP_COM1        "\x07"  /* Hash_7: BLC commitment com1                     */
#define DOMAIN_SEP_GAMMA       "\x08"  /* XOF_8: Gamma batching challenge                 */

/* TweakSalt "sel" domain separation */
#define SALT_SEL_BLC_LEFT    0  /* Line commitment, left (blc_convert.h, seed_commit*.h) */
#define SALT_SEL_BLC_RIGHT   1  /* Line commitment, right (idem)                         */
#define SALT_SEL_GGM         2  /* GGM tree node derivation (ggm_tree/)                  */
#define SALT_SEL_SEED_EXPAND 3  /* Leaf-seed expansion (seed_expand*, blc memopt folding) */

#endif /* __DOMAIN_SEPARATION_H__ */
