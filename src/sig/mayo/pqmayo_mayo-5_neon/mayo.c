// SPDX-License-Identifier: Apache-2.0

#include <mem.h>
#include <mayo.h>
#include <randombytes.h>
#include <aes_ctr.h>
#include <arithmetic.h>
#include <simple_arithmetic.h>
#include <fips202.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>
#ifdef ENABLE_CT_TESTING
#include <valgrind/memcheck.h>
#endif

#define MAYO_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define PK_PRF AES_128_CTR

static void decode(const unsigned char *m, unsigned char *mdec, int mdeclen) {
    int i;
    for (i = 0; i < mdeclen / 2; ++i) {
        *mdec++ = m[i] & 0xf;
        *mdec++ = m[i] >> 4;
    }

    if (mdeclen % 2 == 1) {
        *mdec++ = m[i] & 0x0f;
    }
}

static void encode(const unsigned char *m, unsigned char *menc, int mlen) {
    int i;
    for (i = 0; i < mlen / 2; ++i, m += 2) {
        menc[i] = (*m) | (*(m + 1) << 4);
    }

    if (mlen % 2 == 1) {
        menc[i] = (*m);
    }
}

static void compute_rhs(const mayo_params_t *p, uint64_t *vPv, const unsigned char *t, unsigned char *y){
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif

    const size_t top_pos = ((PARAM_m(p) - 1) % 16) * 4;
    const size_t m_vec_limbs = PARAM_m_vec_limbs(p);

    // zero out tails of m_vecs if neccesary
    if(PARAM_m(p) % 16 != 0){
        uint64_t mask = 1;
        mask <<= ((PARAM_m(p) % 16)*4);
        mask -= 1;
        for (int i = 0; i < PARAM_k(p)*PARAM_k(p); i++)
        {
            vPv[i*m_vec_limbs + m_vec_limbs - 1] &= mask;
        }
    }

    uint64_t temp[M_VEC_LIMBS_MAX] = {0};
    unsigned char *temp_bytes = (unsigned char *) temp;
    // Horner in z over the whipping schedule: visiting the labels from the
    // highest exponent down leaves the term of label l multiplied by z^l
    const unsigned char *whip_positions = PARAM_whip_pos(p);
    for (int l = PARAM_k(p) * (PARAM_k(p) + 1) / 2 - 1; l >= 0; l--) {
        const int row = whip_positions[2*l], col = whip_positions[2*l + 1];
        // multiply by X (shift up 4 bits)
        unsigned char top = (temp[m_vec_limbs-1] >> top_pos) % 16;
        temp[m_vec_limbs-1] <<= 4;
        for(int k = m_vec_limbs - 2; k>=0; k--){
            temp[k+1] ^= temp[k] >> 60;
            temp[k] <<= 4;
        }
        // reduce mod f(X): tap jj lands in the low or high nibble of byte jj/2
        for (int jj = 0; jj < F_TAIL_LEN; jj++) {
            temp_bytes[MAYO_LIMB_BYTE(jj/2)] ^= mul_f(top, PARAM_f_tail(p)[jj]) << (4 * (jj % 2));
        }

        // extract from vPv and add
        for(size_t k=0; k < m_vec_limbs; k ++){
            temp[k] ^= vPv[( row * PARAM_k(p) + col )* m_vec_limbs + k] ^ ((row!=col)*vPv[( col * PARAM_k(p) + row )* m_vec_limbs + k]);
        }
    }

    // compute y
    for (int i = 0; i < PARAM_m(p); i+=2)
    {
        y[i]   = t[i]   ^ (temp_bytes[MAYO_LIMB_BYTE(i/2)] & 0xF);
        y[i+1] = t[i+1] ^ (temp_bytes[MAYO_LIMB_BYTE(i/2)] >> 4);
    }
}

static void transpose_16x16_nibbles(uint64_t *M){
    static const uint64_t even_nibbles = 0x0f0f0f0f0f0f0f0f;
    static const uint64_t even_bytes   = 0x00ff00ff00ff00ff;
    static const uint64_t even_2bytes  = 0x0000ffff0000ffff;
    static const uint64_t even_half    = 0x00000000ffffffff;

    for (size_t i = 0; i < 16; i+=2)
    {
        uint64_t t = ((M[i] >> 4 ) ^ M[i+1]) & even_nibbles;
        M[i  ] ^= t << 4;
        M[i+1] ^= t;
    }

    for (size_t i = 0; i < 16; i+=4)
    {
        uint64_t t0 = ((M[i  ] >> 8) ^ M[i+2]) & even_bytes;
        uint64_t t1 = ((M[i+1] >> 8) ^ M[i+3]) & even_bytes;
        M[i  ] ^= (t0 << 8);
        M[i+1] ^= (t1 << 8);
        M[i+2] ^= t0;
        M[i+3] ^= t1;
    }

    for (size_t i = 0; i < 4; i++)
    {
        uint64_t t0 = ((M[i  ] >> 16) ^ M[i+ 4]) & even_2bytes;
        uint64_t t1 = ((M[i+8] >> 16) ^ M[i+12]) & even_2bytes;

        M[i   ] ^= t0 << 16;
        M[i+ 8] ^= t1 << 16;
        M[i+ 4] ^= t0;
        M[i+12] ^= t1;
    }

    for (size_t i = 0; i < 8; i++)
    {
        uint64_t t = ((M[i]>>32) ^ M[i+8]) & even_half;
        M[i  ] ^= t << 32;
        M[i+8] ^= t;
    }
}

#define MAYO_M_OVER_8 ((M_MAX + 7) / 8)
static void compute_A(const mayo_params_t *p, uint64_t *VtL, unsigned char *A_out) {
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif

    const int m_vec_limbs = PARAM_m_vec_limbs(p);
    uint64_t A[(((O_MAX*K_MAX+15)/16)*16)*MAYO_M_OVER_8] = {0};
    size_t A_width = ((PARAM_o(p)*PARAM_k(p) + 15)/16)*16;

    // zero out tails of m_vecs if neccesary
    if(PARAM_m(p) % 16 != 0){
        uint64_t mask = 1;
        mask <<= (PARAM_m(p) % 16)*4;
        mask -= 1;
        for (int i = 0; i < PARAM_o(p)*PARAM_k(p); i++)
        {
            VtL[i*m_vec_limbs + m_vec_limbs - 1] &= mask;
        }
    }

    /* the whipping schedule puts the pair of label l at row offset l, i.e. a shift
       of l nibbles: l/16 whole limb-rows plus 4*(l%16) bits within a limb */
    const unsigned char *whip_positions = PARAM_whip_pos(p);
    for (int l = 0; l < PARAM_k(p) * (PARAM_k(p) + 1) / 2; l++) {
        const int row = whip_positions[2*l], col = whip_positions[2*l + 1];
        const int words_to_shift = l / 16;
        const int bits_to_shift = 4 * (l % 16);

        // add the M_row and M_col to A, shifted "down" by l positions
        m_compute_A_acc(A, A_width, words_to_shift, PARAM_o(p) * row,
                        VtL + (size_t) col * m_vec_limbs * PARAM_o(p),
                        PARAM_o(p), m_vec_limbs, bits_to_shift);
        if (row != col) {
            m_compute_A_acc(A, A_width, words_to_shift, PARAM_o(p) * col,
                            VtL + (size_t) row * m_vec_limbs * PARAM_o(p),
                            PARAM_o(p), m_vec_limbs, bits_to_shift);
        }
    }

    for (size_t c = 0; c < A_width*((PARAM_m(p) + (PARAM_k(p)+1)*PARAM_k(p)/2 +15)/16) ; c+= 16)
    {
        transpose_16x16_nibbles(A + c);
    }

    /* the tail rows reduce into the rows below them, one GF(16) multiply per tap. A's
       rows sit sixteen to a block, so copy the sources out and the destinations back and
       let each tap run over contiguous words. */
    #define FTAIL_ROWS ((K_MAX + 1) * K_MAX / 2)
    uint64_t ft_src[FTAIL_ROWS];
    uint64_t ft_dst[FTAIL_ROWS + F_TAIL_LEN];
    const int ftail_rows = (PARAM_k(p) + 1) * PARAM_k(p) / 2;
    /* only the table-driven backends read this; gfni folds its matrix from the
       coefficient, and building it there costs more than the loop it feeds */
    unsigned char ftab[F_TAIL_LEN][16] = {{0}};
#ifdef MAYO_GF16_MULC_NEEDS_TABLE
    for (int t = 0; t < F_TAIL_LEN; t++) {
        for (int j = 0; j < 16; j++) {
            ftab[t][j] = mul_f(PARAM_f_tail(p)[t], (unsigned char) j);
        }
    }
#endif
    for (size_t c = 0; c < A_width; c+= 16)
    {
        for (int i = 0; i < ftail_rows; i++) {
            const int r = PARAM_m(p) + i;
            ft_src[i] = A[(r / 16) * A_width + c + (r % 16)];
        }
        for (int i = 0; i < ftail_rows + F_TAIL_LEN; i++) {
            ft_dst[i] = 0;
        }
        for (int t = 0; t < F_TAIL_LEN; t++) {
            m_gf16_mulc_xor(ft_dst + t, ft_src, ftail_rows, PARAM_f_tail(p)[t], ftab[t]);
        }
        for (int i = 0; i < ftail_rows + F_TAIL_LEN - 1; i++) {
            A[(i / 16) * A_width + c + (i % 16)] ^= ft_dst[i];
        }
    }
    mayo_secure_clear(ft_src, sizeof(ft_src));
    mayo_secure_clear(ft_dst, sizeof(ft_dst));
    #undef FTAIL_ROWS

#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < (((PARAM_o(p)*PARAM_k(p)+15)/16)*16)*MAYO_M_OVER_8; ++i) 
        A[i] = BSWAP64(A[i]);
#endif


    /* A is blocked sixteen rows to a chunk; walk the rows and pick the chunk */
    for (int row = 0; row < PARAM_m(p); row++)
    {
        const size_t base = (size_t) (row & ~15) * A_width / 16 + (size_t) (row & 15);
        for (int c = 0; c < PARAM_A_cols(p)-1 ; c+=16)
        {
            decode( (unsigned char *) &A[base + c], A_out + PARAM_A_cols(p)*row + c, MAYO_MIN(16, PARAM_A_cols(p)-1-c));
        }
    }
}

static void unpack_m_vecs(const unsigned char *in, uint64_t *out, int vecs, int m){
    const int m_vec_limbs = (m + 15) / 16;
    unsigned char *_out = (unsigned char *) out;
    uint64_t tmp[ (M_MAX + 15) / 16] = {0};
    for (int i = vecs-1; i >= 0; i--)
    {
        memcpy(tmp, in + i*m/2, m/2);
        memcpy(_out + i*m_vec_limbs*sizeof(uint64_t), tmp, m_vec_limbs*sizeof(uint64_t));
    }
}
/* ------------------------------------------------------------------ *
 * lambda: the hash-derived linear term                                *
 * ------------------------------------------------------------------ *
 * The verification equation is P*(s) + L(s) = t with
 *
 *     L(s) = sum_i E_i0 * Lambda(s_i),   Lambda(x) = sum_r x_r lambda_r,
 *
 * so the whole term is n elements of R, that is n*m/2 bytes. It is derived from
 * MAYO_LAMBDA_SEED_BYTES taken from the same SHAKE stream that produces t: 16
 * bytes of AES-128 key and 16 bytes of IV, so all 256 bits feed
 * every byte of the keystream.
 *
 * Block j of the keystream is AES-128(key, IV xor j), with the 32-bit block index
 * written big-endian into the last four bytes of the IV. It is built on ECB rather
 * than the native CTR because the CTR entry points take no counter argument, and
 * the portable and AES-NI implementations lay out nonce and counter differently --
 * driving ECB directly keeps lambda identical on every backend. */
#define MAYO_LAMBDA_SEED_BYTES 32

static void derive_lambda(const mayo_params_t *p, const unsigned char *seed,
                          uint64_t *lambda) {
    const int param_n = PARAM_n(p);
    const int param_m = PARAM_m(p);
    const size_t nbytes = (size_t) param_n * (size_t) param_m / 2;
    unsigned char *ks = (unsigned char *) lambda;   /* expanded in place, then unpacked */

    unsigned char in[16 * 64], out[16 * 64];
    const unsigned char *iv = seed + 16;
    uint32_t ctr = 0;
    for (size_t done = 0; done < nbytes; ) {
        size_t nb = (nbytes - done + 15) / 16;
        if (nb > 64) {
            nb = 64;
        }
        for (size_t b = 0; b < nb; b++, ctr++) {
            memcpy(in + 16 * b, iv, 16);
            in[16 * b + 12] ^= (unsigned char) (ctr >> 24);
            in[16 * b + 13] ^= (unsigned char) (ctr >> 16);
            in[16 * b + 14] ^= (unsigned char) (ctr >> 8);
            in[16 * b + 15] ^= (unsigned char) ctr;
        }
        AES_128_ECB(out, in, nb, seed);
        const size_t take = (nbytes - done < nb * 16) ? (nbytes - done) : nb * 16;
        memcpy(ks + done, out, take);
        done += take;
    }
    unpack_m_vecs(ks, lambda, param_n, param_m);
#ifdef TARGET_BIG_ENDIAN
    for (size_t i = 0; i < (size_t) param_n * PARAM_m_vec_limbs(p); ++i) {
        lambda[i] = BSWAP64(lambda[i]);
    }
#endif
    MAYO_ZERO_SLACK(lambda, (size_t) param_n * PARAM_m_vec_limbs(p));
}

static void pack_m_vecs(const uint64_t *in, unsigned char *out, int vecs, int m){
    const int m_vec_limbs = (m + 15) / 16;
    unsigned char *_in = (unsigned char *) in;
    for (int i = 0; i < vecs; i++)
    {
        memmove(out + (i*m/2), _in + i*m_vec_limbs*sizeof(uint64_t), m/2);
    }
}
// Expand P1 and P2 into P. On the vector backends P1 stays in the packed keystream
// layout (its readers use PINV) and P2 is spread out, unless keep_p2_packed.
static void expand_P1_P2(const mayo_params_t *p, uint64_t *P, const unsigned char *seed_pk, int keep_p2_packed) {
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    PK_PRF((unsigned char *)P, PARAM_P1_bytes(p) + PARAM_P2_bytes(p), seed_pk, PARAM_pk_seed_bytes(p));
    // m % 16 == 0: packed stride == padded stride, the keystream already is the layout.
    if (PARAM_m(p) % 16 == 0) {
        return;
    }
#ifdef MAYO_PACKED_P1P2
    if (!keep_p2_packed) { // P1 stays packed; spread only P2 into its padded slot
        unpack_m_vecs((unsigned char *)P + PARAM_P1_bytes(p), P + PARAM_P1_limbs(p),
                      PARAM_P2_limbs(p) / PARAM_m_vec_limbs(p), PARAM_m(p));
    }
#else
    (void) keep_p2_packed;
    unpack_m_vecs((unsigned char *)P, P, (PARAM_P1_limbs(p) + PARAM_P2_limbs(p)) / PARAM_m_vec_limbs(p), PARAM_m(p));
#endif
}

static void eval_public_map(const mayo_params_t *p, const unsigned char *s, const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, const uint64_t *lambda, unsigned char *eval){
    alignas(64) uint64_t SPS[K_MAX * K_MAX * M_VEC_LIMBS_MAX + MAYO_MVEC_SLACK];
    MAYO_ZERO_SLACK(SPS, PARAM_k(p) * PARAM_k(p) * PARAM_m_vec_limbs(p));
    MAYO_ACC_INIT(SPS); // the products below accumulate into it
    /* combine computing PS and SPS in a single step. lambda rides along inside:
       the backend xors it into column 0 of PS, and the S^t * PS half of this product
       then turns it into L(s) = sum_i E_i0 * Lambda(s_i) at SPS[i][0], which
       compute_rhs weights by E_i0 along with everything else. */
    m_calculate_PS_SPS(p, P1, P2, P3, s, lambda, SPS);

    unsigned char zero[M_MAX] = {0};
    compute_rhs(p, SPS, zero, eval);
}

// Public API

int mayo_keypair(const mayo_params_t *p, unsigned char *pk, unsigned char *sk) {
    return mayo_keypair_compact(p, pk, sk);
}

static int expand_sk_internal(const mayo_params_t *p, const unsigned char *csk,
                              sk_t *sk, int materialize_l) {
    int ret = MAYO_OK;
    unsigned char S[PK_SEED_BYTES_MAX + O_BYTES_MAX];
    uint64_t *P = sk->p;
    unsigned char *O = sk->O;

    const int param_o = PARAM_o(p);
    const int param_v = PARAM_v(p);
    const int param_O_bytes = PARAM_O_bytes(p);
    const int param_pk_seed_bytes = PARAM_pk_seed_bytes(p);
    const int param_sk_seed_bytes = PARAM_sk_seed_bytes(p);

    const unsigned char *seed_sk = csk;
    unsigned char *seed_pk = S;

    shake256(S, param_pk_seed_bytes + param_O_bytes, seed_sk,
             param_sk_seed_bytes);
    decode(S + param_pk_seed_bytes, O, param_v * param_o);

    // keep the seed in the expanded key so Sign(esk) can derive the salt
    memcpy(sk->sk_seed, seed_sk, param_sk_seed_bytes);

#ifdef ENABLE_CT_TESTING
    VALGRIND_MAKE_MEM_DEFINED(seed_pk, param_pk_seed_bytes);
#endif

    expand_P1_P2(p, P, seed_pk, 0);
    MAYO_ZERO_SLACK(P, PARAM_P1_limbs(p) + PARAM_P2_limbs(p));
#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < PARAM_P1_limbs(p) + PARAM_P2_limbs(p); ++i) {
        P[i] = BSWAP64(P[i]);
    }
#endif

    if (materialize_l) {
        uint64_t *P2 = P + PARAM_P1_limbs(p);
        uint64_t *P1 = P;

        // compute L_i = (P1 + P1^t) * O + P2
        uint64_t *L = P2;
        P1P1t_times_O(p, P1, O, L);
    }

#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < PARAM_P1_limbs(p) + PARAM_P2_limbs(p); ++i) {
        P[i] = BSWAP64(P[i]);
    }
#endif

    mayo_secure_clear(S, PK_SEED_BYTES_MAX + O_BYTES_MAX);
    return ret;
}

int mayo_expand_sk(const mayo_params_t *p, const unsigned char *csk,
                   sk_t *sk) {
    // L = (P1 + P1^t) O + P2 goes into the expanded key; Sign(esk) reads it back
    return expand_sk_internal(p, csk, sk, 1);
}

static int sign_signature_internal(const mayo_params_t *p, unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const sk_t *sk, int use_l) {
    int ret = MAYO_OK;
    unsigned char tenc[M_BYTES_MAX + MAYO_LAMBDA_SEED_BYTES], t[M_MAX]; // no secret data
    alignas(32) uint64_t lambda[N_MAX * M_VEC_LIMBS_MAX + MAYO_MVEC_SLACK];
    unsigned char y[M_MAX];                    // secret data
    unsigned char salt[SALT_BYTES_MAX];        // not secret data
    unsigned char V[K_MAX * V_BYTES_MAX + R_BYTES_MAX], Vdec[V_MAX * K_MAX];                 // secret data
    unsigned char A[((M_MAX+7)/8*8) * (K_MAX * O_MAX + 1) + 32] = { 0 };   // secret data
    unsigned char x[K_MAX * N_MAX];                       // not secret data
    unsigned char r[K_MAX * O_MAX + 1] = { 0 };           // secret data
    unsigned char s[K_MAX * N_MAX];                       // not secret data
    const unsigned char *seed_sk;
    unsigned char tmp[DIGEST_BYTES_MAX + SALT_BYTES_MAX + SK_SEED_BYTES_MAX + 1];
    unsigned char *ctrbyte;
    int sol_found = 0;

    const int param_m = PARAM_m(p);
    const int param_n = PARAM_n(p);
    const int param_o = PARAM_o(p);
    const int param_k = PARAM_k(p);
    const int param_v = PARAM_v(p);
    const int param_m_bytes = PARAM_m_bytes(p);
    const int param_v_bytes = PARAM_v_bytes(p);
    const int param_r_bytes = PARAM_r_bytes(p);
    const int param_sig_bytes = PARAM_sig_bytes(p);
    const int param_A_cols = PARAM_A_cols(p);
    const int param_digest_bytes = PARAM_digest_bytes(p);
    const int param_sk_seed_bytes = PARAM_sk_seed_bytes(p);
    const int param_salt_bytes = PARAM_salt_bytes(p);

    seed_sk = sk->sk_seed;

    // hash message
    shake256(tmp, param_digest_bytes, m, mlen);

    // sk is read-only on little-endian; on big-endian the swaps below convert it in
    // place and the err: block converts it back, leaving the caller's key unchanged.
    uint64_t *P1 = (uint64_t *) sk->p;
    uint64_t *P2_or_L = P1 + PARAM_P1_limbs(p);
    uint64_t Mtmp[K_MAX * O_MAX * M_VEC_LIMBS_MAX + MAYO_MVEC_SLACK] = {0};

#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < PARAM_P1_limbs(p); ++i) {
        P1[i] = BSWAP64(P1[i]);
    }
    for (int i = 0; i < PARAM_P2_limbs(p); ++i) {
        P2_or_L[i] = BSWAP64(P2_or_L[i]);
    }
#endif

    // choose the randomizer
    #if defined(PQM4) || defined(HAVE_RANDOMBYTES_NORETVAL)
    randombytes(tmp + param_digest_bytes, param_salt_bytes);
    #else
    if (randombytes(tmp + param_digest_bytes, param_salt_bytes) != MAYO_OK) {
        ret = MAYO_ERR;
        goto err;
    }
    #endif

    // hashing to salt
    memcpy(tmp + param_digest_bytes + param_salt_bytes, seed_sk,
           param_sk_seed_bytes);
    shake256(salt, param_salt_bytes, tmp,
             param_digest_bytes + param_salt_bytes + param_sk_seed_bytes);

#ifdef ENABLE_CT_TESTING
    VALGRIND_MAKE_MEM_DEFINED(salt, SALT_BYTES_MAX); // Salt is not secret
#endif

    // hashing to t
    memcpy(tmp + param_digest_bytes, salt, param_salt_bytes);
    ctrbyte = tmp + param_digest_bytes + param_salt_bytes + param_sk_seed_bytes;

    shake256(tenc, param_m_bytes + MAYO_LAMBDA_SEED_BYTES, tmp,
             param_digest_bytes + param_salt_bytes);

    decode(tenc, t, param_m); // may not be necessary
    derive_lambda(p, tenc + param_m_bytes, lambda);

    for (int ctr = 0; ctr <= 255; ++ctr) {
        *ctrbyte = (unsigned char)ctr;

        shake256(V, param_k * param_v_bytes + param_r_bytes, tmp,
                 param_digest_bytes + param_salt_bytes + param_sk_seed_bytes + 1);

        // decode the v_i vectors
        for (int i = 0; i < param_k; ++i) {
            decode(V + i * param_v_bytes, Vdec + i * param_v, param_v);
        }

        // compute M_i matrices and all v_i*P1*v_j, with lambda folded in
        m_calculate_M_and_VPV(p, P1, P2_or_L, sk->O, Vdec, lambda, use_l,
                              Mtmp, (uint64_t *) A);

        compute_rhs(p, (uint64_t*) A, t, y);
        compute_A(p, Mtmp, A);

        for (int i = 0; i < param_m; i++)
        {
            A[(1+i)*(param_k*param_o + 1) - 1] = 0;
        }

        decode(V + param_k * param_v_bytes, r,
               param_k *
               param_o);

        if (sample_solution(p, A, y, r, x, param_k, param_o, param_m, param_A_cols)) {
            sol_found = 1;
            break;
        } else {
            memset(Mtmp, 0, sizeof(Mtmp));
            memset(A, 0, sizeof(A));
        }
    }
    if (!sol_found) {
        ret = MAYO_ERR;
        goto err;
    }

    m_compute_s(p, sk->O, Vdec, x, s);
    encode(s, sig, param_n * param_k);

    memcpy(sig + param_sig_bytes - param_salt_bytes, salt, param_salt_bytes);
    *siglen = param_sig_bytes;

err:
#ifdef TARGET_BIG_ENDIAN
    // convert the caller's key back
    for (int i = 0; i < PARAM_P1_limbs(p); ++i) {
        P1[i] = BSWAP64(P1[i]);
    }
    for (int i = 0; i < PARAM_P2_limbs(p); ++i) {
        P2_or_L[i] = BSWAP64(P2_or_L[i]);
    }
#endif
    mayo_secure_clear(V, sizeof(V));
    mayo_secure_clear(Vdec, sizeof(Vdec));
    mayo_secure_clear(A, sizeof(A));
    mayo_secure_clear(r, sizeof(r));
    mayo_secure_clear(tmp, sizeof(tmp));
    mayo_secure_clear(Mtmp, sizeof(Mtmp));
    return ret;
}

// Sign with a pre-expanded key. L is materialized in esk, matching mayo_expand_sk:
// the pair is the expand-once-sign-many path, so it pays for L once and reads it
// back on every signature. esk is unchanged on return, but big-endian converts it
// in place, so concurrent signers cannot share one.
int mayo_sign_signature_esk(const mayo_params_t *p, unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const sk_t *esk) {
    return sign_signature_internal(p, sig, siglen, m, mlen, esk, 1);
}   

// Fused expandSK + Sign (NIST-style, compact key). All backends keep L implicit
// (expand without L, sign on the fly) for best single-shot latency.
int mayo_sign_signature(const mayo_params_t *p, unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const unsigned char *csk) {
    const int use_l = 0; // all backends expand without L and sign implicitly
    alignas(32) sk_t sk;
    int ret = expand_sk_internal(p, csk, &sk, use_l);
    if (ret == MAYO_OK) {
        ret = sign_signature_internal(p, sig, siglen, m, mlen, &sk, use_l);
    }
    mayo_secure_clear(&sk, sizeof(sk_t));
    return ret;
}

int mayo_sign_esk(const mayo_params_t *p, unsigned char *sm,
                  size_t *smlen, const unsigned char *m,
                  size_t mlen, const sk_t *esk) {
    int ret = MAYO_OK;
    const int param_sig_bytes = PARAM_sig_bytes(p);
    size_t siglen = param_sig_bytes;
    memmove(sm + param_sig_bytes, m, mlen);
    ret = mayo_sign_signature_esk(p, sm, &siglen, sm + param_sig_bytes, mlen, esk);
    if (ret != MAYO_OK || siglen != (size_t)param_sig_bytes) {
        memset(sm, 0, siglen + mlen);
        goto err;
    }
    *smlen = siglen + mlen;
err:
    return ret;
}

int mayo_sign(const mayo_params_t *p, unsigned char *sm,
              size_t *smlen, const unsigned char *m,
              size_t mlen, const unsigned char *csk) {
    int ret = MAYO_OK;
    const int param_sig_bytes = PARAM_sig_bytes(p);
    size_t siglen = param_sig_bytes;
    memmove(sm + param_sig_bytes, m, mlen);
    ret = mayo_sign_signature(p, sm, &siglen, sm + param_sig_bytes, mlen, csk);
    if (ret != MAYO_OK || siglen != (size_t) param_sig_bytes){
        memset(sm, 0, siglen + mlen);
        goto err;
    }

    *smlen = siglen + mlen;
err:
    return ret;
}

int mayo_open(const mayo_params_t *p, unsigned char *m,
              size_t *mlen, const unsigned char *sm,
              size_t smlen, const unsigned char *pk) {
    const int param_sig_bytes = PARAM_sig_bytes(p);
    if (smlen < (size_t)param_sig_bytes) {
        return MAYO_ERR;
    }
    int result = mayo_verify(p, sm + param_sig_bytes, smlen - param_sig_bytes, sm,
                             pk);

    if (result == MAYO_OK) {
        *mlen = smlen - param_sig_bytes;
        memmove(m, sm + param_sig_bytes, *mlen);
    }

    return result;
}

int mayo_keypair_compact(const mayo_params_t *p, unsigned char *cpk,
                         unsigned char *csk) {
    int ret = MAYO_OK;
    unsigned char *seed_sk = csk;
    unsigned char S[PK_SEED_BYTES_MAX + O_BYTES_MAX];
    uint64_t P[P1_LIMBS_MAX + P2_LIMBS_MAX + MAYO_MVEC_SLACK];
    uint64_t P3[O_MAX*O_MAX*M_VEC_LIMBS_MAX + MAYO_MVEC_SLACK] = {0};

    unsigned char *seed_pk;
    unsigned char O[(V_MAX)*O_MAX];

    const int m_vec_limbs = PARAM_m_vec_limbs(p);
    const int param_m = PARAM_m(p);
    const int param_v = PARAM_v(p);
    const int param_o = PARAM_o(p);
    const int param_O_bytes = PARAM_O_bytes(p);
    const int param_P1_limbs = PARAM_P1_limbs(p);
    const int param_P3_limbs = PARAM_P3_limbs(p);
    const int param_pk_seed_bytes = PARAM_pk_seed_bytes(p);
    const int param_sk_seed_bytes = PARAM_sk_seed_bytes(p);

    uint64_t *P1 = P;
    uint64_t *P2 = P + param_P1_limbs;

    // seed_sk $←- B^(sk_seed bytes)
    #if defined(PQM4) || defined(HAVE_RANDOMBYTES_NORETVAL)
    randombytes(seed_sk, param_sk_seed_bytes);
    #else
    if (randombytes(seed_sk, param_sk_seed_bytes) != MAYO_OK) {
        ret = MAYO_ERR;
        goto err;
    }
    #endif

    // S ← shake256(seedsk, pk seed bytes + O bytes)
    shake256(S, param_pk_seed_bytes + param_O_bytes, seed_sk,
             param_sk_seed_bytes);
    // seed_pk ← s[0 : pk_seed_bytes]
    seed_pk = S;

    // o ← Decode_o(s[pk_seed_bytes : pk_seed_bytes + o_bytes])
    decode(S + param_pk_seed_bytes, O, param_v * param_o);

#ifdef ENABLE_CT_TESTING
    VALGRIND_MAKE_MEM_DEFINED(seed_pk, param_pk_seed_bytes);
#endif

    expand_P1_P2(p, P, seed_pk, 0);
    MAYO_ZERO_SLACK(P, param_P1_limbs + PARAM_P2_limbs(p));

    // compute P3 (modifies P2 in the process)
    compute_P3(p, P1, P2, O, P3);

    // store seed_pk in cpk
    memcpy(cpk, seed_pk, param_pk_seed_bytes);

    uint64_t P3_upper[P3_LIMBS_MAX + MAYO_MVEC_SLACK];

    // compute Upper(P3) and store in cpk
    MAYO_ZERO_SLACK(P3_upper, param_P3_limbs);
    m_upper(p, P3, P3_upper, param_o);
    pack_m_vecs(P3_upper, cpk + param_pk_seed_bytes, param_P3_limbs/m_vec_limbs, param_m);

#if !defined(PQM4) && !defined(HAVE_RANDOMBYTES_NORETVAL)
    err:
#endif
    mayo_secure_clear(S, PK_SEED_BYTES_MAX + O_BYTES_MAX);
    mayo_secure_clear(O, sizeof(O));
    mayo_secure_clear(P2, PARAM_P2_limbs(p)*sizeof(uint64_t));
    mayo_secure_clear(P3, sizeof(P3));
    return ret;
}

int mayo_expand_pk(const mayo_params_t *p, const unsigned char *cpk,
                   uint64_t *pk) {
    expand_P1_P2(p, pk, cpk, 1); // verify reads P1 and P2 in place: keep P2 packed too
    // P3 follows P1||P2, whose end depends on whether those stayed packed.
#ifdef MAYO_PACKED_P1P2
    unsigned char *P3 = (unsigned char *)pk + PARAM_P1_bytes(p) + PARAM_P2_bytes(p);
#else
    unsigned char *P3 = (unsigned char *)(pk + PARAM_P1_limbs(p) + PARAM_P2_limbs(p));
#endif
    unpack_m_vecs(cpk + PARAM_pk_seed_bytes(p), (uint64_t *)P3,
                  PARAM_P3_limbs(p) / PARAM_m_vec_limbs(p), PARAM_m(p));
    MAYO_ZERO_SLACK(P3, PARAM_P3_limbs(p));

    return MAYO_OK;
}

int mayo_verify_epk(const mayo_params_t *p, const unsigned char *m,
                    size_t mlen, const unsigned char *sig,
                    const uint64_t *epk) {
    unsigned char tEnc[M_BYTES_MAX + MAYO_LAMBDA_SEED_BYTES];
    alignas(32) uint64_t lambda[N_MAX * M_VEC_LIMBS_MAX + MAYO_MVEC_SLACK];
    unsigned char t[M_MAX];
    unsigned char y[2 * M_MAX] = {0}; // extra space for reduction mod f(X)
    unsigned char s[K_MAX * N_MAX];
    unsigned char tmp[DIGEST_BYTES_MAX + SALT_BYTES_MAX];

    const int param_m = PARAM_m(p);
    const int param_n = PARAM_n(p);
    const int param_k = PARAM_k(p);
    const int param_m_bytes = PARAM_m_bytes(p);
    const int param_sig_bytes = PARAM_sig_bytes(p);
    const int param_digest_bytes = PARAM_digest_bytes(p);
    const int param_salt_bytes = PARAM_salt_bytes(p);

    const uint64_t *P1 = epk;
#ifdef MAYO_PACKED_P1P2
    // P1 and P2 are packed in the expanded key, so P2/P3 start at byte offsets
    const uint64_t *P2 = (const uint64_t *)((const unsigned char *)P1 + PARAM_P1_bytes(p));
    const uint64_t *P3 = (const uint64_t *)((const unsigned char *)P1 + PARAM_P1_bytes(p) + PARAM_P2_bytes(p));
#else
    const uint64_t *P2 = P1 + PARAM_P1_limbs(p);
    const uint64_t *P3 = P2 + PARAM_P2_limbs(p);
#endif

#if defined(MAYO_PACKED_P1P2) && defined(TARGET_BIG_ENDIAN)
#error "packed P1/P2 is little-endian only: the swap below needs limb-aligned m-vecs"
#endif

#ifdef TARGET_BIG_ENDIAN
    uint64_t pk_be[P1_LIMBS_MAX + P2_LIMBS_MAX + P3_LIMBS_MAX];
    memcpy(pk_be, epk, (PARAM_P1_limbs(p) + PARAM_P2_limbs(p) + PARAM_P3_limbs(p)) * sizeof(uint64_t));
    P1 = pk_be;
    P2 = pk_be + PARAM_P1_limbs(p);
    P3 = pk_be + PARAM_P1_limbs(p) + PARAM_P2_limbs(p);
    for (int i = 0; i < PARAM_P1_limbs(p); ++i)
        pk_be[i] = BSWAP64(pk_be[i]);
    for (int i = PARAM_P1_limbs(p); i < PARAM_P1_limbs(p) + PARAM_P2_limbs(p); ++i)
        pk_be[i] = BSWAP64(pk_be[i]);
    for (int i = PARAM_P1_limbs(p) + PARAM_P2_limbs(p); i < PARAM_P1_limbs(p) + PARAM_P2_limbs(p) + PARAM_P3_limbs(p); ++i)
        pk_be[i] = BSWAP64(pk_be[i]);
#endif

    // hash m
    shake256(tmp, param_digest_bytes, m, mlen);

    // compute t
    memcpy(tmp + param_digest_bytes, sig + param_sig_bytes - param_salt_bytes,
           param_salt_bytes);
    /* SHAKE is a stream, so the first param_m_bytes are the same t as before;
       the extra MAYO_LAMBDA_SEED_BYTES seed the lambda expansion. */
    shake256(tEnc, param_m_bytes + MAYO_LAMBDA_SEED_BYTES, tmp,
             param_digest_bytes + param_salt_bytes);
    decode(tEnc, t, param_m);
    derive_lambda(p, tEnc + param_m_bytes, lambda);

    // decode s
    decode(sig, s, param_k * param_n);

    eval_public_map(p, s, P1, P2, P3, lambda, y);

    if (memcmp(y, t, param_m) == 0) {
        return MAYO_OK; // good signature
    }
    return MAYO_ERR; // bad signature
}

int mayo_open_epk(const mayo_params_t *p, unsigned char *m,
                  size_t *mlen, const unsigned char *sm,
                  size_t smlen, const uint64_t *epk) {
    const int param_sig_bytes = PARAM_sig_bytes(p);
    if (smlen < (size_t)param_sig_bytes) {
        return MAYO_ERR;
    }
    int result = mayo_verify_epk(p, sm + param_sig_bytes, smlen - param_sig_bytes,
                                 sm, epk);
    if (result == MAYO_OK) {
        *mlen = smlen - param_sig_bytes;
        memmove(m, sm + param_sig_bytes, *mlen);
    }
    return result;
}

int mayo_verify(const mayo_params_t *p, const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                const unsigned char *cpk) {
    alignas(64) uint64_t pk[P1_LIMBS_MAX + P2_LIMBS_MAX + P3_LIMBS_MAX + MAYO_MVEC_SLACK];

    int ret = mayo_expand_pk(p, cpk, pk);
    if (ret != MAYO_OK) {
        return MAYO_ERR;
    }

    return mayo_verify_epk(p, m, mlen, sig, pk);
}
