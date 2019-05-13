/* Based on the public domain implementation in
 * crypto_hash/keccakc512/simple/ from http://bench.cr.yp.to/supercop.html
 * by Ronny Van Keer
 * and the public domain "TweetFips202" implementation
 * from https://twitter.com/tweetfips202
 * by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe */

#include <stddef.h>
#include <stdint.h>



#include <oqs/sha3.h>

#define NROUNDS 24
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))

/*************************************************
 * Name:        load64
 *
 * Description: Load 8 bytes into uint64_t in little-endian order
 *
 * Arguments:   - const uint8_t *x: pointer to input byte array
 *
 * Returns the loaded 64-bit unsigned integer
 **************************************************/
static uint64_t load64(const uint8_t *x) {
    uint64_t r = 0;
    for (size_t i = 0; i < 8; ++i) {
        r |= (uint64_t)x[i] << 8 * i;
    }

    return r;
}

/*************************************************
 * Name:        store64
 *
 * Description: Store a 64-bit integer to a byte array in little-endian order
 *
 * Arguments:   - uint8_t *x: pointer to the output byte array
 *              - uint64_t u: input 64-bit unsigned integer
 **************************************************/
static void store64(uint8_t *x, uint64_t u) {
    for (size_t i = 0; i < 8; ++i) {
        x[i] = (uint8_t) (u >> 8 * i);
    }
}

/* Keccak round constants */
static const uint64_t KeccakF_RoundConstants[NROUNDS] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

/*************************************************
 * Name:        OQS_SHA3_keccak_permute
 *
 * Description: The Keccak F1600 Permutation
 *
 * Arguments:   - uint64_t *state: pointer to input/output Keccak state
 **************************************************/
void OQS_SHA3_keccak_permute(uint64_t *state) {
    int round;

    uint64_t Aba, Abe, Abi, Abo, Abu;
    uint64_t Aga, Age, Agi, Ago, Agu;
    uint64_t Aka, Ake, Aki, Ako, Aku;
    uint64_t Ama, Ame, Ami, Amo, Amu;
    uint64_t Asa, Ase, Asi, Aso, Asu;
    uint64_t BCa, BCe, BCi, BCo, BCu;
    uint64_t Da, De, Di, Do, Du;
    uint64_t Eba, Ebe, Ebi, Ebo, Ebu;
    uint64_t Ega, Ege, Egi, Ego, Egu;
    uint64_t Eka, Eke, Eki, Eko, Eku;
    uint64_t Ema, Eme, Emi, Emo, Emu;
    uint64_t Esa, Ese, Esi, Eso, Esu;

    // copyFromState(A, state)
    Aba = state[0];
    Abe = state[1];
    Abi = state[2];
    Abo = state[3];
    Abu = state[4];
    Aga = state[5];
    Age = state[6];
    Agi = state[7];
    Ago = state[8];
    Agu = state[9];
    Aka = state[10];
    Ake = state[11];
    Aki = state[12];
    Ako = state[13];
    Aku = state[14];
    Ama = state[15];
    Ame = state[16];
    Ami = state[17];
    Amo = state[18];
    Amu = state[19];
    Asa = state[20];
    Ase = state[21];
    Asi = state[22];
    Aso = state[23];
    Asu = state[24];

    for (round = 0; round < NROUNDS; round += 2) {
        //    prepareTheta
        BCa = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
        BCe = Abe ^ Age ^ Ake ^ Ame ^ Ase;
        BCi = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
        BCo = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
        BCu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;

        // thetaRhoPiChiIotaPrepareTheta(round  , A, E)
        Da = BCu ^ ROL(BCe, 1);
        De = BCa ^ ROL(BCi, 1);
        Di = BCe ^ ROL(BCo, 1);
        Do = BCi ^ ROL(BCu, 1);
        Du = BCo ^ ROL(BCa, 1);

        Aba ^= Da;
        BCa = Aba;
        Age ^= De;
        BCe = ROL(Age, 44);
        Aki ^= Di;
        BCi = ROL(Aki, 43);
        Amo ^= Do;
        BCo = ROL(Amo, 21);
        Asu ^= Du;
        BCu = ROL(Asu, 14);
        Eba = BCa ^ ((~BCe) & BCi);
        Eba ^= KeccakF_RoundConstants[round];
        Ebe = BCe ^ ((~BCi) & BCo);
        Ebi = BCi ^ ((~BCo) & BCu);
        Ebo = BCo ^ ((~BCu) & BCa);
        Ebu = BCu ^ ((~BCa) & BCe);

        Abo ^= Do;
        BCa = ROL(Abo, 28);
        Agu ^= Du;
        BCe = ROL(Agu, 20);
        Aka ^= Da;
        BCi = ROL(Aka, 3);
        Ame ^= De;
        BCo = ROL(Ame, 45);
        Asi ^= Di;
        BCu = ROL(Asi, 61);
        Ega = BCa ^ ((~BCe) & BCi);
        Ege = BCe ^ ((~BCi) & BCo);
        Egi = BCi ^ ((~BCo) & BCu);
        Ego = BCo ^ ((~BCu) & BCa);
        Egu = BCu ^ ((~BCa) & BCe);

        Abe ^= De;
        BCa = ROL(Abe, 1);
        Agi ^= Di;
        BCe = ROL(Agi, 6);
        Ako ^= Do;
        BCi = ROL(Ako, 25);
        Amu ^= Du;
        BCo = ROL(Amu, 8);
        Asa ^= Da;
        BCu = ROL(Asa, 18);
        Eka = BCa ^ ((~BCe) & BCi);
        Eke = BCe ^ ((~BCi) & BCo);
        Eki = BCi ^ ((~BCo) & BCu);
        Eko = BCo ^ ((~BCu) & BCa);
        Eku = BCu ^ ((~BCa) & BCe);

        Abu ^= Du;
        BCa = ROL(Abu, 27);
        Aga ^= Da;
        BCe = ROL(Aga, 36);
        Ake ^= De;
        BCi = ROL(Ake, 10);
        Ami ^= Di;
        BCo = ROL(Ami, 15);
        Aso ^= Do;
        BCu = ROL(Aso, 56);
        Ema = BCa ^ ((~BCe) & BCi);
        Eme = BCe ^ ((~BCi) & BCo);
        Emi = BCi ^ ((~BCo) & BCu);
        Emo = BCo ^ ((~BCu) & BCa);
        Emu = BCu ^ ((~BCa) & BCe);

        Abi ^= Di;
        BCa = ROL(Abi, 62);
        Ago ^= Do;
        BCe = ROL(Ago, 55);
        Aku ^= Du;
        BCi = ROL(Aku, 39);
        Ama ^= Da;
        BCo = ROL(Ama, 41);
        Ase ^= De;
        BCu = ROL(Ase, 2);
        Esa = BCa ^ ((~BCe) & BCi);
        Ese = BCe ^ ((~BCi) & BCo);
        Esi = BCi ^ ((~BCo) & BCu);
        Eso = BCo ^ ((~BCu) & BCa);
        Esu = BCu ^ ((~BCa) & BCe);

        //    prepareTheta
        BCa = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
        BCe = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
        BCi = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
        BCo = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
        BCu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;

        // thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
        Da = BCu ^ ROL(BCe, 1);
        De = BCa ^ ROL(BCi, 1);
        Di = BCe ^ ROL(BCo, 1);
        Do = BCi ^ ROL(BCu, 1);
        Du = BCo ^ ROL(BCa, 1);

        Eba ^= Da;
        BCa = Eba;
        Ege ^= De;
        BCe = ROL(Ege, 44);
        Eki ^= Di;
        BCi = ROL(Eki, 43);
        Emo ^= Do;
        BCo = ROL(Emo, 21);
        Esu ^= Du;
        BCu = ROL(Esu, 14);
        Aba = BCa ^ ((~BCe) & BCi);
        Aba ^= KeccakF_RoundConstants[round + 1];
        Abe = BCe ^ ((~BCi) & BCo);
        Abi = BCi ^ ((~BCo) & BCu);
        Abo = BCo ^ ((~BCu) & BCa);
        Abu = BCu ^ ((~BCa) & BCe);

        Ebo ^= Do;
        BCa = ROL(Ebo, 28);
        Egu ^= Du;
        BCe = ROL(Egu, 20);
        Eka ^= Da;
        BCi = ROL(Eka, 3);
        Eme ^= De;
        BCo = ROL(Eme, 45);
        Esi ^= Di;
        BCu = ROL(Esi, 61);
        Aga = BCa ^ ((~BCe) & BCi);
        Age = BCe ^ ((~BCi) & BCo);
        Agi = BCi ^ ((~BCo) & BCu);
        Ago = BCo ^ ((~BCu) & BCa);
        Agu = BCu ^ ((~BCa) & BCe);

        Ebe ^= De;
        BCa = ROL(Ebe, 1);
        Egi ^= Di;
        BCe = ROL(Egi, 6);
        Eko ^= Do;
        BCi = ROL(Eko, 25);
        Emu ^= Du;
        BCo = ROL(Emu, 8);
        Esa ^= Da;
        BCu = ROL(Esa, 18);
        Aka = BCa ^ ((~BCe) & BCi);
        Ake = BCe ^ ((~BCi) & BCo);
        Aki = BCi ^ ((~BCo) & BCu);
        Ako = BCo ^ ((~BCu) & BCa);
        Aku = BCu ^ ((~BCa) & BCe);

        Ebu ^= Du;
        BCa = ROL(Ebu, 27);
        Ega ^= Da;
        BCe = ROL(Ega, 36);
        Eke ^= De;
        BCi = ROL(Eke, 10);
        Emi ^= Di;
        BCo = ROL(Emi, 15);
        Eso ^= Do;
        BCu = ROL(Eso, 56);
        Ama = BCa ^ ((~BCe) & BCi);
        Ame = BCe ^ ((~BCi) & BCo);
        Ami = BCi ^ ((~BCo) & BCu);
        Amo = BCo ^ ((~BCu) & BCa);
        Amu = BCu ^ ((~BCa) & BCe);

        Ebi ^= Di;
        BCa = ROL(Ebi, 62);
        Ego ^= Do;
        BCe = ROL(Ego, 55);
        Eku ^= Du;
        BCi = ROL(Eku, 39);
        Ema ^= Da;
        BCo = ROL(Ema, 41);
        Ese ^= De;
        BCu = ROL(Ese, 2);
        Asa = BCa ^ ((~BCe) & BCi);
        Ase = BCe ^ ((~BCi) & BCo);
        Asi = BCi ^ ((~BCo) & BCu);
        Aso = BCo ^ ((~BCu) & BCa);
        Asu = BCu ^ ((~BCa) & BCe);
    }

    // copyToState(state, A)
    state[0] = Aba;
    state[1] = Abe;
    state[2] = Abi;
    state[3] = Abo;
    state[4] = Abu;
    state[5] = Aga;
    state[6] = Age;
    state[7] = Agi;
    state[8] = Ago;
    state[9] = Agu;
    state[10] = Aka;
    state[11] = Ake;
    state[12] = Aki;
    state[13] = Ako;
    state[14] = Aku;
    state[15] = Ama;
    state[16] = Ame;
    state[17] = Ami;
    state[18] = Amo;
    state[19] = Amu;
    state[20] = Asa;
    state[21] = Ase;
    state[22] = Asi;
    state[23] = Aso;
    state[24] = Asu;
}

/*************************************************
 * Name:        OQS_SHA3_keccak_absorb
 *
 * Description: Absorb step of Keccak;
 *              non-incremental, starts by zeroeing the state.
 *
 * Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 *              - const uint8_t *m: pointer to input to be absorbed into s
 *              - size_t mlen: length of input in bytes
 *              - uint8_t p: domain-separation byte for different
 *                                 Keccak-derived functions
 **************************************************/
void OQS_SHA3_keccak_absorb(uint64_t *s, size_t r, const uint8_t *m,
                          size_t mlen, uint8_t p) {
    size_t i;
    uint8_t t[200];

    /* Zero state */
    for (i = 0; i < 25; ++i) {
        s[i] = 0;
    }

    while (mlen >= r) {
        for (i = 0; i < r / 8; ++i) {
            s[i] ^= load64(m + 8 * i);
        }

        OQS_SHA3_keccak_permute(s);
        mlen -= r;
        m += r;
    }

    for (i = 0; i < r; ++i) {
        t[i] = 0;
    }
    for (i = 0; i < mlen; ++i) {
        t[i] = m[i];
    }
    t[i] = p;
    t[r - 1] |= 128;
    for (i = 0; i < r / 8; ++i) {
        s[i] ^= load64(t + 8 * i);
    }
}

/*************************************************
 * Name:        OQS_SHA3_keccak_squeezeblocks
 *
 * Description: Squeeze step of Keccak. Squeezes full blocks of r bytes each.
 *              Modifies the state. Can be called multiple times to keep
 *              squeezing, i.e., is incremental.
 *
 * Arguments:   - uint8_t *h: pointer to output blocks
 *              - size_t nblocks: number of blocks to be
 *                                                squeezed (written to h)
 *              - uint64_t *s: pointer to input/output Keccak state
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 **************************************************/
void OQS_SHA3_keccak_squeezeblocks(uint8_t *h, size_t nblocks,
                                 uint64_t *s, size_t r) {
    while (nblocks > 0) {
        OQS_SHA3_keccak_permute(s);
        for (size_t i = 0; i < (r >> 3); i++) {
            store64(h + 8 * i, s[i]);
        }
        h += r;
        nblocks--;
    }
}

/*************************************************
 * Name:        keccak_inc_init
 *
 * Description: Initializes the incremental Keccak state to zero.
 *
 * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 **************************************************/
static void keccak_inc_init(uint64_t *s_inc) {
    size_t i;

    for (i = 0; i < 25; ++i) {
        s_inc[i] = 0;
    }
    s_inc[25] = 0;
}

/*************************************************
 * Name:        keccak_inc_absorb
 *
 * Description: Incremental keccak absorb
 *              Preceded by keccak_inc_init, succeeded by keccak_inc_finalize
 *
 * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 *              - const uint8_t *m: pointer to input to be absorbed into s
 *              - size_t mlen: length of input in bytes
 **************************************************/
static void keccak_inc_absorb(uint64_t *s_inc, uint32_t r, const uint8_t *m,
                              size_t mlen) {
    size_t i;

    /* Recall that s_inc[25] is the non-absorbed bytes xored into the state */
    while (mlen + s_inc[25] >= r) {
        for (i = 0; i < r - s_inc[25]; i++) {
            /* Take the i'th byte from message
               xor with the s_inc[25] + i'th byte of the state; little-endian */
            s_inc[(s_inc[25] + i) >> 3] ^= (uint64_t)m[i] << (8 * ((s_inc[25] + i) & 0x07));
        }
        mlen -= (size_t)(r - s_inc[25]);
        m += r - s_inc[25];
        s_inc[25] = 0;

        OQS_SHA3_keccak_permute(s_inc);
    }

    for (i = 0; i < mlen; i++) {
        s_inc[(s_inc[25] + i) >> 3] ^= (uint64_t)m[i] << (8 * ((s_inc[25] + i) & 0x07));
    }
    s_inc[25] += mlen;
}

/*************************************************
 * Name:        keccak_inc_finalize
 *
 * Description: Finalizes Keccak absorb phase, prepares for squeezing
 *
 * Arguments:   - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 *              - uint8_t p: domain-separation byte for different
 *                                 Keccak-derived functions
 **************************************************/
static void keccak_inc_finalize(uint64_t *s_inc, uint32_t r, uint8_t p) {
    /* After keccak_inc_absorb, we are guaranteed that s_inc[25] < r,
       so we can always use one more byte for p in the current state. */
    s_inc[s_inc[25] >> 3] ^= (uint64_t)p << (8 * (s_inc[25] & 0x07));
    s_inc[(r - 1) >> 3] ^= (uint64_t)128 << (8 * ((r - 1) & 0x07));
    s_inc[25] = 0;
}

/*************************************************
 * Name:        keccak_inc_squeeze
 *
 * Description: Incremental Keccak squeeze; can be called on byte-level
 *
 * Arguments:   - uint8_t *h: pointer to output bytes
 *              - size_t outlen: number of bytes to be squeezed
 *              - uint64_t *s_inc: pointer to input/output incremental state
 *                First 25 values represent Keccak state.
 *                26th value represents either the number of absorbed bytes
 *                that have not been permuted, or not-yet-squeezed bytes.
 *              - uint32_t r: rate in bytes (e.g., 168 for SHAKE128)
 **************************************************/
static void keccak_inc_squeeze(uint8_t *h, size_t outlen,
                               uint64_t *s_inc, uint32_t r) {
    size_t i;

    /* First consume any bytes we still have sitting around */
    for (i = 0; i < outlen && i < s_inc[25]; i++) {
        /* There are s_inc[25] bytes left, so r - s_inc[25] is the first
           available byte. We consume from there, i.e., up to r. */
        h[i] = (uint8_t)(s_inc[(r - s_inc[25] + i) >> 3] >> (8 * ((r - s_inc[25] + i) & 0x07)));
    }
    h += i;
    outlen -= i;
    s_inc[25] -= i;

    /* Then squeeze the remaining necessary blocks */
    while (outlen > 0) {
        OQS_SHA3_keccak_permute(s_inc);

        for (i = 0; i < outlen && i < r; i++) {
            h[i] = (uint8_t)(s_inc[i >> 3] >> (8 * (i & 0x07)));
        }
        h += i;
        outlen -= i;
        s_inc[25] = r - i;
    }
}

void OQS_SHA3_shake128_inc_init(uint64_t *s_inc) {
    keccak_inc_init(s_inc);
}

void OQS_SHA3_shake128_inc_absorb(uint64_t *s_inc, const uint8_t *input, size_t inlen) {
    keccak_inc_absorb(s_inc, OQS_SHA3_SHAKE128_RATE, input, inlen);
}

void OQS_SHA3_shake128_inc_finalize(uint64_t *s_inc) {
    keccak_inc_finalize(s_inc, OQS_SHA3_SHAKE128_RATE, 0x1F);
}

void OQS_SHA3_shake128_inc_squeeze(uint8_t *output, size_t outlen, uint64_t *s_inc) {
    keccak_inc_squeeze(output, outlen, s_inc, OQS_SHA3_SHAKE128_RATE);
}

void OQS_SHA3_shake256_inc_init(uint64_t *s_inc) {
    keccak_inc_init(s_inc);
}

void OQS_SHA3_shake256_inc_absorb(uint64_t *s_inc, const uint8_t *input, size_t inlen) {
    keccak_inc_absorb(s_inc, OQS_SHA3_SHAKE256_RATE, input, inlen);
}

void OQS_SHA3_shake256_inc_finalize(uint64_t *s_inc) {
    keccak_inc_finalize(s_inc, OQS_SHA3_SHAKE256_RATE, 0x1F);
}

void OQS_SHA3_shake256_inc_squeeze(uint8_t *output, size_t outlen, uint64_t *s_inc) {
    keccak_inc_squeeze(output, outlen, s_inc, OQS_SHA3_SHAKE256_RATE);
}


/*************************************************
 * Name:        shake128_absorb
 *
 * Description: Absorb step of the SHAKE128 XOF.
 *              non-incremental, starts by zeroeing the state.
 *
 * Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
 *              - const uint8_t *input: pointer to input to be absorbed
 *                                            into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void OQS_SHA3_shake128_absorb(uint64_t *s, const uint8_t *input, size_t inlen) {
    OQS_SHA3_keccak_absorb(s, OQS_SHA3_SHAKE128_RATE, input, inlen, 0x1F);
}

/*************************************************
 * Name:        shake128_squeezeblocks
 *
 * Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of
 *              OQS_SHA3_SHAKE128_RATE bytes each. Modifies the state. Can be called
 *              multiple times to keep squeezing, i.e., is incremental.
 *
 * Arguments:   - uint8_t *output: pointer to output blocks
 *              - size_t nblocks: number of blocks to be squeezed
 *                                            (written to output)
 *              - uint64_t *s: pointer to input/output Keccak state
 **************************************************/
void OQS_SHA3_shake128_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *s) {
    OQS_SHA3_keccak_squeezeblocks(output, nblocks, s, OQS_SHA3_SHAKE128_RATE);
}

/*************************************************
 * Name:        shake256_absorb
 *
 * Description: Absorb step of the SHAKE256 XOF.
 *              non-incremental, starts by zeroeing the state.
 *
 * Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
 *              - const uint8_t *input: pointer to input to be absorbed
 *                                            into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void OQS_SHA3_shake256_absorb(uint64_t *s, const uint8_t *input, size_t inlen) {
    OQS_SHA3_keccak_absorb(s, OQS_SHA3_SHAKE256_RATE, input, inlen, 0x1F);
}

/*************************************************
 * Name:        shake256_squeezeblocks
 *
 * Description: Squeeze step of SHAKE256 XOF. Squeezes full blocks of
 *              OQS_SHA3_SHAKE256_RATE bytes each. Modifies the state. Can be called
 *              multiple times to keep squeezing, i.e., is incremental.
 *
 * Arguments:   - uint8_t *output: pointer to output blocks
 *              - size_t nblocks: number of blocks to be squeezed
 *                                (written to output)
 *              - uint64_t *s: pointer to input/output Keccak state
 **************************************************/
void OQS_SHA3_shake256_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *s) {
    OQS_SHA3_keccak_squeezeblocks(output, nblocks, s, OQS_SHA3_SHAKE256_RATE);
}

/*************************************************
 * Name:        shake128
 *
 * Description: SHAKE128 XOF with non-incremental API
 *
 * Arguments:   - uint8_t *output: pointer to output
 *              - size_t outlen: requested output length in bytes
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/
void OQS_SHA3_shake128(uint8_t *output, size_t outlen,
              const uint8_t *input, size_t inlen) {
    size_t nblocks = outlen / OQS_SHA3_SHAKE128_RATE;
    uint8_t t[OQS_SHA3_SHAKE128_RATE];
    uint64_t s[25];

    OQS_SHA3_shake128_absorb(s, input, inlen);
    OQS_SHA3_shake128_squeezeblocks(output, nblocks, s);

    output += nblocks * OQS_SHA3_SHAKE128_RATE;
    outlen -= nblocks * OQS_SHA3_SHAKE128_RATE;

    if (outlen) {
        OQS_SHA3_shake128_squeezeblocks(t, 1, s);
        for (size_t i = 0; i < outlen; ++i) {
            output[i] = t[i];
        }
    }
}

/*************************************************
 * Name:        shake256
 *
 * Description: SHAKE256 XOF with non-incremental API
 *
 * Arguments:   - uint8_t *output: pointer to output
 *              - size_t outlen: requested output length in bytes
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/
void OQS_SHA3_shake256(uint8_t *output, size_t outlen,
              const uint8_t *input, size_t inlen) {
    size_t nblocks = outlen / OQS_SHA3_SHAKE256_RATE;
    uint8_t t[OQS_SHA3_SHAKE256_RATE];
    uint64_t s[25];

    OQS_SHA3_shake256_absorb(s, input, inlen);
    OQS_SHA3_shake256_squeezeblocks(output, nblocks, s);

    output += nblocks * OQS_SHA3_SHAKE256_RATE;
    outlen -= nblocks * OQS_SHA3_SHAKE256_RATE;

    if (outlen) {
        OQS_SHA3_shake256_squeezeblocks(t, 1, s);
        for (size_t i = 0; i < outlen; ++i) {
            output[i] = t[i];
        }
    }
}

void OQS_SHA3_sha3256_inc_init(uint64_t *s_inc) {
    keccak_inc_init(s_inc);
}

void OQS_SHA3_sha3256_inc_absorb(uint64_t *s_inc, const uint8_t *input, size_t inlen) {
    keccak_inc_absorb(s_inc, OQS_SHA3_SHA3_256_RATE, input, inlen);
}

void OQS_SHA3_sha3256_inc_finalize(uint8_t *output, uint64_t *s_inc) {
    uint8_t t[OQS_SHA3_SHA3_256_RATE];
    keccak_inc_finalize(s_inc, OQS_SHA3_SHA3_256_RATE, 0x06);

    OQS_SHA3_keccak_squeezeblocks(t, 1, s_inc, OQS_SHA3_SHA3_256_RATE);

    for (size_t i = 0; i < 32; i++) {
        output[i] = t[i];
    }
}

/*************************************************
 * Name:        sha3_256
 *
 * Description: SHA3-256 with non-incremental API
 *
 * Arguments:   - uint8_t *output:      pointer to output
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen:   length of input in bytes
 **************************************************/
void OQS_SHA3_sha3256(uint8_t *output, const uint8_t *input, size_t inlen) {
    uint64_t s[25];
    uint8_t t[OQS_SHA3_SHA3_256_RATE];

    /* Absorb input */
    OQS_SHA3_keccak_absorb(s, OQS_SHA3_SHA3_256_RATE, input, inlen, 0x06);

    /* Squeeze output */
    OQS_SHA3_keccak_squeezeblocks(t, 1, s, OQS_SHA3_SHA3_256_RATE);

    for (size_t i = 0; i < 32; i++) {
        output[i] = t[i];
    }
}

void OQS_SHA3_sha3512_inc_init(uint64_t *s_inc) {
    keccak_inc_init(s_inc);
}

void OQS_SHA3_sha3512_inc_absorb(uint64_t *s_inc, const uint8_t *input, size_t inlen) {
    keccak_inc_absorb(s_inc, OQS_SHA3_SHA3_512_RATE, input, inlen);
}

void OQS_SHA3_sha3512_inc_finalize(uint8_t *output, uint64_t *s_inc) {
    uint8_t t[OQS_SHA3_SHA3_512_RATE];
    keccak_inc_finalize(s_inc, OQS_SHA3_SHA3_512_RATE, 0x06);

    OQS_SHA3_keccak_squeezeblocks(t, 1, s_inc, OQS_SHA3_SHA3_512_RATE);

    for (size_t i = 0; i < 32; i++) {
        output[i] = t[i];
    }
}

/*************************************************
 * Name:        sha3_512
 *
 * Description: SHA3-512 with non-incremental API
 *
 * Arguments:   - uint8_t *output:      pointer to output
 *              - const uint8_t *input: pointer to input
 *              - size_t inlen:   length of input in bytes
 **************************************************/
void OQS_SHA3_sha3512(uint8_t *output, const uint8_t *input, size_t inlen) {
    uint64_t s[25];
    uint8_t t[OQS_SHA3_SHA3_512_RATE];

    /* Absorb input */
    OQS_SHA3_keccak_absorb(s, OQS_SHA3_SHA3_512_RATE, input, inlen, 0x06);

    /* Squeeze output */
    OQS_SHA3_keccak_squeezeblocks(t, 1, s, OQS_SHA3_SHA3_512_RATE);

    for (size_t i = 0; i < 64; i++) {
        output[i] = t[i];
    }
}


/* cSHAKE */

void OQS_SHA3_cshake128_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen) {
	size_t nblocks = outlen / OQS_SHA3_CSHAKE128_RATE;
	uint64_t state[OQS_SHA3_STATESIZE];
	uint8_t hash[OQS_SHA3_CSHAKE128_RATE];
	size_t i;

	for (i = 0; i < OQS_SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_cshake128_simple_absorb(state, cstm, input, inplen);
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, OQS_SHA3_CSHAKE128_RATE);

	output += (nblocks * OQS_SHA3_CSHAKE128_RATE);
	outlen -= (nblocks * OQS_SHA3_CSHAKE128_RATE);

	if (outlen != 0) {
		OQS_SHA3_keccak_squeezeblocks(hash, 1, state, OQS_SHA3_CSHAKE128_RATE);

		for (i = 0; i < outlen; i++) {
			output[i] = hash[i];
		}
	}
}

void OQS_SHA3_cshake128_simple_absorb(uint64_t *state, uint16_t cstm, const uint8_t *input, size_t inplen) {
	/* Note: This function doesn't align exactly to cSHAKE (SP800-185 3.2), which should produce
	SHAKE output if S and N = zero (sort of a customized custom-SHAKE function).
	Padding is hard-coded as the first 32 bits, plus 16 bits of fixed S,
	and 16 bits of counter, equivalant to S=cstm, N=0.
	The short integer optimizes this function for a digest counter configuration */

	uint8_t sep[8];
	sep[0] = 0x01; /* bytepad */
	sep[1] = 0xA8;
	sep[2] = 0x01;
	sep[3] = 0x00;
	sep[4] = 0x01;
	sep[5] = 0x10; /* bitlen of cstm */
	sep[6] = cstm & 0xFF;
	sep[7] = cstm >> 8;

	state[0] = load64(sep);

	/* transform the domain string */
	OQS_SHA3_keccak_permute(state);

	/* absorb the state */
	OQS_SHA3_keccak_absorb(state, OQS_SHA3_CSHAKE128_RATE, input, inplen, OQS_SHA3_CSHAKE_DOMAIN);
}

void OQS_SHA3_cshake128_simple_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state) {
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, OQS_SHA3_CSHAKE128_RATE);
}

void OQS_SHA3_cshake256_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen) {
	size_t nblocks = outlen / OQS_SHA3_CSHAKE256_RATE;
	uint64_t state[OQS_SHA3_STATESIZE];
	uint8_t hash[OQS_SHA3_CSHAKE256_RATE];
	size_t i;

	for (i = 0; i < OQS_SHA3_STATESIZE; ++i) {
		state[i] = 0;
	}

	OQS_SHA3_cshake256_simple_absorb(state, cstm, input, inplen);
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, OQS_SHA3_CSHAKE256_RATE);

	output += (nblocks * OQS_SHA3_CSHAKE256_RATE);
	outlen -= (nblocks * OQS_SHA3_CSHAKE256_RATE);

	if (outlen != 0) {
		OQS_SHA3_keccak_squeezeblocks(hash, 1, state, OQS_SHA3_CSHAKE256_RATE);

		for (i = 0; i < outlen; i++) {
			output[i] = hash[i];
		}
	}
}

void OQS_SHA3_cshake256_simple_absorb(uint64_t *state, uint16_t cstm, const uint8_t *input, size_t inplen) {
	uint8_t sep[8];
	sep[0] = 0x01; /* bytepad */
	sep[1] = 0x88;
	sep[2] = 0x01;
	sep[3] = 0x00;
	sep[4] = 0x01;
	sep[5] = 0x10; /* bitlen of cstm */
	sep[6] = cstm & 0xFF;
	sep[7] = cstm >> 8;

	state[0] = load64(sep);

	/* transform the domain string */
	OQS_SHA3_keccak_permute(state);

	/* absorb the state */
	OQS_SHA3_keccak_absorb(state, OQS_SHA3_CSHAKE256_RATE, input, inplen, OQS_SHA3_CSHAKE_DOMAIN);
}

void OQS_SHA3_cshake256_simple_squeezeblocks(uint8_t *output, size_t nblocks, uint64_t *state) {
	OQS_SHA3_keccak_squeezeblocks(output, nblocks, state, OQS_SHA3_CSHAKE256_RATE);
}
