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
    for (int i = PARAM_k(p) - 1; i >= 0 ; i--) {
        for (int j = i; j < PARAM_k(p); j++) {
            // multiply by X (shift up 4 bits)
            unsigned char top = (temp[m_vec_limbs-1] >> top_pos) % 16;
            temp[m_vec_limbs-1] <<= 4;
            for(int k = m_vec_limbs - 2; k>=0; k--){
                temp[k+1] ^= temp[k] >> 60;
                temp[k] <<= 4;
            }
            // reduce mod f(X)
            for (int jj = 0; jj < F_TAIL_LEN; jj++) {
                if(jj%2 == 0){
#ifdef TARGET_BIG_ENDIAN
                    temp_bytes[(((jj/2 + 8) / 8) * 8) - 1 - (jj/2)%8] ^= mul_f(top, PARAM_f_tail(p)[jj]);
#else
                    temp_bytes[jj/2] ^= mul_f(top, PARAM_f_tail(p)[jj]);
#endif
                }
                else {
#ifdef TARGET_BIG_ENDIAN
                    temp_bytes[(((jj/2 + 8) / 8) * 8) - 1 - (jj/2)%8] ^= mul_f(top, PARAM_f_tail(p)[jj]) << 4;
#else
                    temp_bytes[jj/2] ^= mul_f(top, PARAM_f_tail(p)[jj]) << 4;
#endif
                }
            }

            // extract from vPv and add
            for(size_t k=0; k < m_vec_limbs; k ++){
                temp[k] ^= vPv[( i * PARAM_k(p) + j )* m_vec_limbs + k] ^ ((i!=j)*vPv[( j * PARAM_k(p) + i )* m_vec_limbs + k]);
            }
        }
    }

    // compute y
    for (int i = 0; i < PARAM_m(p); i+=2)
    {
#ifdef TARGET_BIG_ENDIAN
        y[i]   = t[i]   ^ (temp_bytes[(((i/2 + 8) / 8) * 8) - 1 - (i/2)%8] & 0xF);
        y[i+1] = t[i+1] ^ (temp_bytes[(((i/2 + 8) / 8) * 8) - 1 - (i/2)%8] >> 4);
#else
        y[i]   = t[i]   ^ (temp_bytes[i/2] & 0xF);
        y[i+1] = t[i+1] ^ (temp_bytes[i/2] >> 4);
#endif
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

    int bits_to_shift = 0;
    int words_to_shift = 0;
    const int m_vec_limbs = PARAM_m_vec_limbs(p);
    uint64_t A[(((O_MAX*K_MAX+15)/16)*16)*MAYO_M_OVER_8] = {0};
    size_t A_width = ((PARAM_o(p)*PARAM_k(p) + 15)/16)*16;
    const uint64_t *Mi, *Mj;

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

    for (int i = 0; i <= PARAM_k(p) - 1; ++i) {
        for (int j = PARAM_k(p) - 1; j >= i; --j) {
            // add the M_i and M_j to A, shifted "down" by l positions
            Mj = VtL + j * m_vec_limbs * PARAM_o(p);
            for (int c = 0; c < PARAM_o(p); c++) {
                for (int k = 0; k < m_vec_limbs; k++)
                {
                    A[ PARAM_o(p) * i + c + (k + words_to_shift)*A_width] ^= Mj[k + c*m_vec_limbs] << bits_to_shift;
                    if(bits_to_shift > 0){
                        A[ PARAM_o(p) * i + c + (k + words_to_shift + 1)*A_width] ^= Mj[k + c*m_vec_limbs] >> (64-bits_to_shift);
                    }
                }
            }

            if (i != j) {
                Mi = VtL + i * m_vec_limbs * PARAM_o(p);
                for (int c = 0; c < PARAM_o(p); c++) {
                    for (int k = 0; k < m_vec_limbs; k++)
                    {
                        A[PARAM_o(p) * j + c + (k + words_to_shift)*A_width] ^= Mi[k + c*m_vec_limbs] << bits_to_shift;
                        if(bits_to_shift > 0){
                            A[PARAM_o(p) * j + c + (k + words_to_shift + 1)*A_width] ^= Mi[k + c*m_vec_limbs] >> (64-bits_to_shift);
                        }
                    }
                }
            }

            bits_to_shift += 4;
            if(bits_to_shift == 64){
                words_to_shift ++;
                bits_to_shift = 0;
            }
        }
    }

    for (size_t c = 0; c < A_width*((PARAM_m(p) + (PARAM_k(p)+1)*PARAM_k(p)/2 +15)/16) ; c+= 16)
    {
        transpose_16x16_nibbles(A + c);
    }

    unsigned char tab[F_TAIL_LEN*4] = {0};
    for (size_t i = 0; i < F_TAIL_LEN; i++)
    {
        tab[4*i]   = mul_f(PARAM_f_tail(p)[i],1);
        tab[4*i+1] = mul_f(PARAM_f_tail(p)[i],2);
        tab[4*i+2] = mul_f(PARAM_f_tail(p)[i],4);
        tab[4*i+3] = mul_f(PARAM_f_tail(p)[i],8);
    }

    uint64_t low_bit_in_nibble = 0x1111111111111111;

    for (size_t c = 0; c < A_width; c+= 16)
    {
        for (int r = PARAM_m(p); r < PARAM_m(p) + (PARAM_k(p)+1)*PARAM_k(p)/2 ; r++)
        {
            size_t pos = (r/16)*A_width + c + (r%16);
            uint64_t t0 =  A[pos]       & low_bit_in_nibble;
            uint64_t t1 = (A[pos] >> 1) & low_bit_in_nibble;
            uint64_t t2 = (A[pos] >> 2) & low_bit_in_nibble;
            uint64_t t3 = (A[pos] >> 3) & low_bit_in_nibble;
            for (size_t t = 0; t < F_TAIL_LEN; t++)
            {
                A[((r+t-PARAM_m(p))/16)*A_width + c + ((r+t-PARAM_m(p))%16)] ^= t0*tab[4*t+0] ^ t1*tab[4*t+1] ^ t2*tab[4*t+2] ^ t3*tab[4*t+3];
            }
        }
    }

#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < (((PARAM_o(p)*PARAM_k(p)+15)/16)*16)*MAYO_M_OVER_8; ++i) 
        A[i] = BSWAP64(A[i]);
#endif


    for (int r = 0; r < PARAM_m(p); r+=16)
    {
        for (int c = 0; c < PARAM_A_cols(p)-1 ; c+=16)
        {
            for (int i = 0; i + r < PARAM_m(p); i++)
            {
                decode( (unsigned char *) &A[r*A_width/16 + c + i], A_out + PARAM_A_cols(p)*(r+i) + c, MAYO_MIN(16, PARAM_A_cols(p)-1-c));
            }
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
static void pack_m_vecs(const uint64_t *in, unsigned char *out, int vecs, int m){
    const int m_vec_limbs = (m + 15) / 16;
    unsigned char *_in = (unsigned char *) in;
    for (int i = 0; i < vecs; i++)
    {
        memmove(out + (i*m/2), _in + i*m_vec_limbs*sizeof(uint64_t), m/2);
    }
}
static void expand_P1_P2(const mayo_params_t *p, uint64_t *P, const unsigned char *seed_pk) {
    #ifndef ENABLE_PARAMS_DYNAMIC
    (void) p;
    #endif
    PK_PRF((unsigned char *)P, PARAM_P1_bytes(p) + PARAM_P2_bytes(p), seed_pk, PARAM_pk_seed_bytes(p));
    unpack_m_vecs((unsigned char *)P, P, (PARAM_P1_limbs(p) + PARAM_P2_limbs(p))/PARAM_m_vec_limbs(p), PARAM_m(p));
}

static void eval_public_map(const mayo_params_t *p, const unsigned char *s, const uint64_t *P1, const uint64_t *P2, const uint64_t *P3, unsigned char *eval){
    uint64_t SPS[K_MAX * K_MAX * M_VEC_LIMBS_MAX] = {0};
    // combine computing PS and SPS in a single step
    m_calculate_PS_SPS(p, P1, P2, P3, s, SPS);
    unsigned char zero[M_MAX] = {0};
    compute_rhs(p, SPS, zero, eval);
}

// Public API

int mayo_keypair(const mayo_params_t *p, unsigned char *pk, unsigned char *sk) {
    int ret = 0;

    ret = mayo_keypair_compact(p, pk, sk);
    if (ret != MAYO_OK) {
        goto err;
    }

err:
    return ret;
}

int mayo_expand_sk(const mayo_params_t *p, const unsigned char *csk,
                   sk_t *sk) {
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

#ifdef ENABLE_CT_TESTING
    VALGRIND_MAKE_MEM_DEFINED(seed_pk, param_pk_seed_bytes);
#endif

    expand_P1_P2(p, P, seed_pk);
#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < PARAM_P1_limbs(p) + PARAM_P2_limbs(p); ++i) {
        P[i] = BSWAP64(P[i]);
    }
#endif

    uint64_t *P2 = P + PARAM_P1_limbs(p);

    uint64_t *P1 = P;

    // compute L_i = (P1 + P1^t)*O + P2
    uint64_t *L = P2;
    P1P1t_times_O(p, P1, O, L);

#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < PARAM_P1_limbs(p) + PARAM_P2_limbs(p); ++i) {
        P[i] = BSWAP64(P[i]);
    }
#endif

    mayo_secure_clear(S, PK_SEED_BYTES_MAX + O_BYTES_MAX);
    return ret;
}

int mayo_sign_signature(const mayo_params_t *p, unsigned char *sig,
              size_t *siglen, const unsigned char *m,
              size_t mlen, const unsigned char *csk) {
    int ret = MAYO_OK;
    unsigned char tenc[M_BYTES_MAX], t[M_MAX]; // no secret data
    unsigned char y[M_MAX];                    // secret data
    unsigned char salt[SALT_BYTES_MAX];        // not secret data
    unsigned char V[K_MAX * V_BYTES_MAX + R_BYTES_MAX], Vdec[V_MAX * K_MAX];                 // secret data
    unsigned char A[((M_MAX+7)/8*8) * (K_MAX * O_MAX + 1)] = { 0 };   // secret data
    unsigned char x[K_MAX * N_MAX];                       // not secret data
    unsigned char r[K_MAX * O_MAX + 1] = { 0 };           // secret data
    unsigned char s[K_MAX * N_MAX];                       // not secret data
    const unsigned char *seed_sk;
    alignas(32) sk_t sk;                    // secret data
    unsigned char Ox[V_MAX];        // secret data
    unsigned char tmp[DIGEST_BYTES_MAX + SALT_BYTES_MAX + SK_SEED_BYTES_MAX + 1];
    unsigned char *ctrbyte;
    unsigned char *vi;

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

    ret = mayo_expand_sk(p, csk, &sk);
    if (ret != MAYO_OK) {
        goto err;
    }

    seed_sk = csk;


    // hash message
    shake256(tmp, param_digest_bytes, m, mlen);

    uint64_t *P1 = sk.p;
    uint64_t *L  = P1 + PARAM_P1_limbs(p);
    uint64_t Mtmp[K_MAX * O_MAX * M_VEC_LIMBS_MAX] = {0};

#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < PARAM_P1_limbs(p); ++i) {
        P1[i] = BSWAP64(P1[i]);
    }
    for (int i = 0; i < PARAM_P2_limbs(p); ++i) {
        L[i] = BSWAP64(L[i]);
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

    shake256(tenc, param_m_bytes, tmp, param_digest_bytes + param_salt_bytes);

    decode(tenc, t, param_m); // may not be necessary

    for (int ctr = 0; ctr <= 255; ++ctr) {
        *ctrbyte = (unsigned char)ctr;

        shake256(V, param_k * param_v_bytes + param_r_bytes, tmp,
                 param_digest_bytes + param_salt_bytes + param_sk_seed_bytes + 1);

        // decode the v_i vectors
        for (int i = 0; i <= param_k - 1; ++i) {
            decode(V + i * param_v_bytes, Vdec + i * param_v, param_v);
        }

        // compute M_i matrices and all v_i*P1*v_j
        compute_M_and_VPV(p, Vdec, L, P1, Mtmp, (uint64_t*) A);

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
            break;
        } else {
            memset(Mtmp, 0, sizeof(Mtmp));
            memset(A, 0, sizeof(A));
        }
    }

    for (int i = 0; i <= param_k - 1; ++i) {
        vi = Vdec + i * (param_n - param_o);
        mat_mul(sk.O, x + i * param_o, Ox, param_o, param_n - param_o, 1);
        mat_add(vi, Ox, s + i * param_n, param_n - param_o, 1);
        memcpy(s + i * param_n + (param_n - param_o), x + i * param_o, param_o);
    }
    encode(s, sig, param_n * param_k);

    memcpy(sig + param_sig_bytes - param_salt_bytes, salt, param_salt_bytes);
    *siglen = param_sig_bytes;

err:
    mayo_secure_clear(V, sizeof(V));
    mayo_secure_clear(Vdec, sizeof(Vdec));
    mayo_secure_clear(A, sizeof(A));
    mayo_secure_clear(r, sizeof(r));
    mayo_secure_clear(sk.O, sizeof(sk.O));
    mayo_secure_clear(&sk, sizeof(sk_t));
    mayo_secure_clear(Ox, sizeof(Ox));
    mayo_secure_clear(tmp, sizeof(tmp));
    mayo_secure_clear(Mtmp, sizeof(Mtmp));
    return ret;
}

int mayo_sign(const mayo_params_t *p, unsigned char *sm,
              size_t *smlen, const unsigned char *m,
              size_t mlen, const unsigned char *csk) {
    int ret = MAYO_OK;
    const int param_sig_bytes = PARAM_sig_bytes(p);
    size_t siglen;
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
    uint64_t P[P1_LIMBS_MAX + P2_LIMBS_MAX];
    uint64_t P3[O_MAX*O_MAX*M_VEC_LIMBS_MAX] = {0};

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

    expand_P1_P2(p, P, seed_pk);

    // compute P3 (modifies P2 in the process)
    compute_P3(p, P1, P2, O, P3);

    // store seed_pk in cpk
    memcpy(cpk, seed_pk, param_pk_seed_bytes);

    uint64_t P3_upper[P3_LIMBS_MAX];

    // compute Upper(P3) and store in cpk
    m_upper(p, P3, P3_upper, param_o);
    pack_m_vecs(P3_upper, cpk + param_pk_seed_bytes, param_P3_limbs/m_vec_limbs, param_m);

#if !defined(PQM4) && !defined(HAVE_RANDOMBYTES_NORETVAL)
    err:
#endif
    mayo_secure_clear(O, sizeof(O));
    mayo_secure_clear(P2, PARAM_P2_limbs(p)*sizeof(uint64_t));
    mayo_secure_clear(P3, sizeof(P3));
    return ret;
}

int mayo_expand_pk(const mayo_params_t *p, const unsigned char *cpk,
                   uint64_t *pk) {
    expand_P1_P2(p, pk, cpk);

    unpack_m_vecs(cpk + PARAM_pk_seed_bytes(p), pk + PARAM_P1_limbs(p) + PARAM_P2_limbs(p), PARAM_P3_limbs(p)/PARAM_m_vec_limbs(p), PARAM_m(p));

    return MAYO_OK;
}

int mayo_verify(const mayo_params_t *p, const unsigned char *m,
                size_t mlen, const unsigned char *sig,
                const unsigned char *cpk) {
    unsigned char tEnc[M_BYTES_MAX];
    unsigned char t[M_MAX];
    unsigned char y[2 * M_MAX] = {0}; // extra space for reduction mod f(X)
    unsigned char s[K_MAX * N_MAX];
    uint64_t pk[P1_LIMBS_MAX + P2_LIMBS_MAX + P3_LIMBS_MAX] = {0};
    unsigned char tmp[DIGEST_BYTES_MAX + SALT_BYTES_MAX];

    const int param_m = PARAM_m(p);
    const int param_n = PARAM_n(p);
    const int param_k = PARAM_k(p);
    const int param_m_bytes = PARAM_m_bytes(p);
    const int param_sig_bytes = PARAM_sig_bytes(p);
    const int param_digest_bytes = PARAM_digest_bytes(p);
    const int param_salt_bytes = PARAM_salt_bytes(p);

    int ret = mayo_expand_pk(p, cpk, pk);
    if (ret != MAYO_OK) {
        return MAYO_ERR;
    }

    uint64_t *P1 = pk;
    uint64_t *P2 = P1 + PARAM_P1_limbs(p);
    uint64_t *P3 = P2 + PARAM_P2_limbs(p);

#ifdef TARGET_BIG_ENDIAN
    for (int i = 0; i < PARAM_P1_limbs(p); ++i) {
        P1[i] = BSWAP64(P1[i]);
    }
    for (int i = 0; i < PARAM_P2_limbs(p); ++i) {
        P2[i] = BSWAP64(P2[i]);
    }
    for (int i = 0; i < PARAM_P3_limbs(p); ++i) {
        P3[i] = BSWAP64(P3[i]);
    }
#endif

    // hash m
    shake256(tmp, param_digest_bytes, m, mlen);

    // compute t
    memcpy(tmp + param_digest_bytes, sig + param_sig_bytes - param_salt_bytes,
           param_salt_bytes);
    shake256(tEnc, param_m_bytes, tmp, param_digest_bytes + param_salt_bytes);
    decode(tEnc, t, param_m);

    // decode s
    decode(sig, s, param_k * param_n);

    eval_public_map(p, s, P1, P2, P3, y);

    if (memcmp(y, t, param_m) == 0) {
        return MAYO_OK; // good signature
    }
    return MAYO_ERR; // bad signature
}

