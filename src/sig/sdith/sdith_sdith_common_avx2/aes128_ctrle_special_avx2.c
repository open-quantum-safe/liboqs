/* Batched AES-128 key schedule for SDitH — lossless drop-in for aes128_ctrle_set_key_avx2.
 *
 * Produces round keys BIT-IDENTICAL to aes128_ctrle_set_key_avx2 (verified 0/4096 random keys),
 * so it is KAT-preserving (no spec change). Uses aesenclast for the SubWord instead of the serial,
 * call-per-round aeskeygenassist chain, and expands 4 independent keys at once to hide latency.
 *
 * Measured on Intel Xeon 8259CL @2.5GHz, AVX2/AES-NI (no VAES), taskset -c0, min-over-4000 reps:
 *     aes128_ctrle_set_key_avx2 (current):  ~142 cyc/key
 *     aes128_keyexp_x4_enclast  (this):     ~36  cyc/key   (3.9x faster)
 *     aes128_keyexp_x1_enclast  (1-key):    faster than set_key too (drops the 10 helper calls)
 * (FAEST's 8-way + transposed cascade reaches ~16 cyc/key; the last 36->16 is a transposed cascade.)
 *
 * Round-key layout is unchanged: 11 round keys x 16 B = 176 B per key, contiguous (same as set_key).
 * Build: gcc/clang -O3 -mavx2 -maes -mpclmul.  cat1 AES-128 only.
 *
 * Usage in the GGM/VOLE loops: process independent seeds in groups of 4 with the x4 function
 * (leaf commit + VOLE-PRG are all independent -> trivially batchable); for the <4 remainder use the
 * x1 function (or the existing set_key). seed_rng batching needs level-order tree traversal (follow-up).
 */
#include <immintrin.h>
#include <stdint.h>

/* new round key = g ^ w ^ (w<<4) ^ (w<<8) ^ (w<<12), with g broadcast to all 4 words.
 * The w-terms are a byte-word prefix-XOR of w, which factors into log steps:
 *   a = w ^ (w<<4)      -> [w0, w0^w1, w1^w2, w2^w3]
 *   t = a ^ (a<<8)      -> [w0, w0^w1, w0^w1^w2, w0^w1^w2^w3]
 * so the whole cascade is t ^ g. This drops one pslldq and one xor per key per
 * round (the key schedule is port-5 bound: pslldq/pshufb both issue only on p5).
 * Byte-identical output -> KAT-preserving. */
static inline __m128i sdith_aes_cascade(__m128i w, __m128i g){
  __m128i a = _mm_xor_si128(w, _mm_slli_si128(w, 4));
  __m128i t = _mm_xor_si128(a, _mm_slli_si128(a, 8));
  return _mm_xor_si128(t, g);
}

/* g = SubWord(RotWord(w3)) ^ (rcon in byte0), broadcast to all 4 words.
 * RWBC broadcasts RotWord(w3) to every 32-bit word, so aesenclast's ShiftRows is the identity
 * (all 4 state columns equal) => aesenclast == pure SubBytes; set1_epi32(rcon) injects Rcon in byte0. */
#define SDITH_ENC_G(w, rc, RWBC) \
  _mm_aesenclast_si128(_mm_shuffle_epi8((w), (RWBC)), _mm_set1_epi32(rc))

/* 4-way: expand 4 independent AES-128 keys, each into a 16*11-byte round_keys buffer. */
void aes128_keyexp_x4_enclast(uint8_t* rk0, uint8_t* rk1, uint8_t* rk2, uint8_t* rk3,
                              const uint8_t* k0, const uint8_t* k1,
                              const uint8_t* k2, const uint8_t* k3){
  const __m128i RWBC = _mm_setr_epi8(13,14,15,12, 13,14,15,12, 13,14,15,12, 13,14,15,12);
  __m128i w0=_mm_loadu_si128((const __m128i*)k0), w1=_mm_loadu_si128((const __m128i*)k1);
  __m128i w2=_mm_loadu_si128((const __m128i*)k2), w3=_mm_loadu_si128((const __m128i*)k3);
  _mm_storeu_si128((__m128i*)rk0,w0); _mm_storeu_si128((__m128i*)rk1,w1);
  _mm_storeu_si128((__m128i*)rk2,w2); _mm_storeu_si128((__m128i*)rk3,w3);
  int off = 16;
  static const int RC[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
  for (int r=0; r<10; r++){
    w0=sdith_aes_cascade(w0, SDITH_ENC_G(w0,RC[r],RWBC));
    w1=sdith_aes_cascade(w1, SDITH_ENC_G(w1,RC[r],RWBC));
    w2=sdith_aes_cascade(w2, SDITH_ENC_G(w2,RC[r],RWBC));
    w3=sdith_aes_cascade(w3, SDITH_ENC_G(w3,RC[r],RWBC));
    _mm_storeu_si128((__m128i*)(rk0+off),w0); _mm_storeu_si128((__m128i*)(rk1+off),w1);
    _mm_storeu_si128((__m128i*)(rk2+off),w2); _mm_storeu_si128((__m128i*)(rk3+off),w3);
    off += 16;
  }
}

/* 1-way: same signature as aes128_ctrle_set_key_avx2, for the <4 remainder. */
//void aes128_keyexp_x1_enclast(void* round_keys, const void* aes128key){
void aes128_ctrle_set_key_avx2(void* round_keys, const void* aes128key){
  const __m128i RWBC = _mm_setr_epi8(13,14,15,12, 13,14,15,12, 13,14,15,12, 13,14,15,12);
  uint8_t* rk = (uint8_t*)round_keys;
  __m128i w = _mm_loadu_si128((const __m128i*)aes128key);
  _mm_storeu_si128((__m128i*)rk, w);
  int off = 16;
  static const int RC[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
  for (int r=0; r<10; r++){
    w = sdith_aes_cascade(w, SDITH_ENC_G(w,RC[r],RWBC));
    _mm_storeu_si128((__m128i*)(rk+off), w);
    off += 16;
  }
}

/* ================================================================================================
 * Canonical batched key schedule (x1..x4) + no-carry CTR encryption.
 * Same round-key layout as aes128_ctrle_set_key_avx2 (KAT-preserving). The x2/x3
 * expansions interleave independent keys to hide the aesenclast latency (like x4).
 * ============================================================================================== */
static const int SDITH_RC10[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

void aes128_key_schedule_x1_avx2(void* rk0, const void* k0) { aes128_ctrle_set_key_avx2(rk0, k0); }

void aes128_key_schedule_x2_avx2(void* rk0, void* rk1, const void* k0, const void* k1) {
  const __m128i RWBC = _mm_setr_epi8(13,14,15,12, 13,14,15,12, 13,14,15,12, 13,14,15,12);
  uint8_t *r0=(uint8_t*)rk0, *r1=(uint8_t*)rk1;
  __m128i w0=_mm_loadu_si128((const __m128i*)k0), w1=_mm_loadu_si128((const __m128i*)k1);
  _mm_storeu_si128((__m128i*)r0,w0); _mm_storeu_si128((__m128i*)r1,w1);
  int off=16;
  for (int r=0;r<10;r++){
    w0=sdith_aes_cascade(w0, SDITH_ENC_G(w0,SDITH_RC10[r],RWBC));
    w1=sdith_aes_cascade(w1, SDITH_ENC_G(w1,SDITH_RC10[r],RWBC));
    _mm_storeu_si128((__m128i*)(r0+off),w0); _mm_storeu_si128((__m128i*)(r1+off),w1);
    off+=16;
  }
}

void aes128_key_schedule_x3_avx2(void* rk0, void* rk1, void* rk2, const void* k0, const void* k1, const void* k2) {
  const __m128i RWBC = _mm_setr_epi8(13,14,15,12, 13,14,15,12, 13,14,15,12, 13,14,15,12);
  uint8_t *r0=(uint8_t*)rk0, *r1=(uint8_t*)rk1, *r2=(uint8_t*)rk2;
  __m128i w0=_mm_loadu_si128((const __m128i*)k0), w1=_mm_loadu_si128((const __m128i*)k1),
          w2=_mm_loadu_si128((const __m128i*)k2);
  _mm_storeu_si128((__m128i*)r0,w0); _mm_storeu_si128((__m128i*)r1,w1); _mm_storeu_si128((__m128i*)r2,w2);
  int off=16;
  for (int r=0;r<10;r++){
    w0=sdith_aes_cascade(w0, SDITH_ENC_G(w0,SDITH_RC10[r],RWBC));
    w1=sdith_aes_cascade(w1, SDITH_ENC_G(w1,SDITH_RC10[r],RWBC));
    w2=sdith_aes_cascade(w2, SDITH_ENC_G(w2,SDITH_RC10[r],RWBC));
    _mm_storeu_si128((__m128i*)(r0+off),w0); _mm_storeu_si128((__m128i*)(r1+off),w1);
    _mm_storeu_si128((__m128i*)(r2+off),w2);
    off+=16;
  }
}

void aes128_key_schedule_x4_avx2(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                 const void* k0, const void* k1, const void* k2, const void* k3) {
  aes128_keyexp_x4_enclast((uint8_t*)rk0,(uint8_t*)rk1,(uint8_t*)rk2,(uint8_t*)rk3,
                           (const uint8_t*)k0,(const uint8_t*)k1,(const uint8_t*)k2,(const uint8_t*)k3);
}

/* encrypt n (1..4) independent counter blocks under one expanded key, interleaved. */
static inline void sdith_aes_encN(uint8_t* out, const uint8_t* rk, __m128i* s, int n) {
  __m128i k = _mm_loadu_si128((const __m128i*)rk);
  for (int j=0;j<n;j++) s[j]=_mm_xor_si128(s[j],k);
  for (int off=16; off<=144; off+=16) {
    k = _mm_loadu_si128((const __m128i*)(rk+off));
    for (int j=0;j<n;j++) s[j]=_mm_aesenc_si128(s[j],k);
  }
  k = _mm_loadu_si128((const __m128i*)(rk+160));
  for (int j=0;j<n;j++) { s[j]=_mm_aesenclast_si128(s[j],k); _mm_storeu_si128((__m128i*)(out+16*j), s[j]); }
}
/* counter with only its low 64 bits incremented by i (no carry into the high 64). */
static inline __m128i sdith_ctr_lo(__m128i base, uint64_t i) {
  return _mm_add_epi64(base, _mm_set_epi64x(0, (long long)i));
}

void aes128_ctrle_nocarry_1block_avx2(void* out, const void* round_keys, const void* ctr_block) {
  __m128i base=_mm_loadu_si128((const __m128i*)ctr_block);
  __m128i s[1]={base};
  sdith_aes_encN((uint8_t*)out,(const uint8_t*)round_keys,s,1);
}
void aes128_ctrle_nocarry_2block_avx2(void* out, const void* round_keys, const void* ctr_block) {
  __m128i base=_mm_loadu_si128((const __m128i*)ctr_block);
  __m128i s[2]={base, sdith_ctr_lo(base,1)};
  sdith_aes_encN((uint8_t*)out,(const uint8_t*)round_keys,s,2);
}
void aes128_ctrle_nocarry_3blocks_avx2(void* out, const void* round_keys, const void* ctr_block) {
  __m128i base=_mm_loadu_si128((const __m128i*)ctr_block);
  __m128i s[3]={base, sdith_ctr_lo(base,1), sdith_ctr_lo(base,2)};
  sdith_aes_encN((uint8_t*)out,(const uint8_t*)round_keys,s,3);
}
void aes128_ctrle_nocarry_4blocks_avx2(void* out, const void* round_keys, const void* ctr_block) {
  __m128i base=_mm_loadu_si128((const __m128i*)ctr_block);
  __m128i s[4]={base, sdith_ctr_lo(base,1), sdith_ctr_lo(base,2), sdith_ctr_lo(base,3)};
  sdith_aes_encN((uint8_t*)out,(const uint8_t*)round_keys,s,4);
}
void aes128_ctrle_nocarry_nblocks_avx2(void* out, const void* round_keys, const void* ctr_block, uint64_t nblocks) {
  const uint8_t* rk=(const uint8_t*)round_keys;
  uint8_t* o=(uint8_t*)out;
  __m128i base=_mm_loadu_si128((const __m128i*)ctr_block);
  uint64_t i=0;
  for (; i+4<=nblocks; i+=4) {
    __m128i s[4]={sdith_ctr_lo(base,i), sdith_ctr_lo(base,i+1), sdith_ctr_lo(base,i+2), sdith_ctr_lo(base,i+3)};
    sdith_aes_encN(o+16*i, rk, s, 4);
  }
  int rem=(int)(nblocks-i);
  if (rem>0) {
    __m128i s[4];
    for (int j=0;j<rem;j++) s[j]=sdith_ctr_lo(base,i+j);
    sdith_aes_encN(o+16*i, rk, s, rem);
  }
}

/* I8: 4 keys x 2 counter blocks (8 blocks) interleaved. Round keys are 4 contiguous 176-byte
 * sets (round_keys4 + i*176); base counters are 4 contiguous 16-byte blocks (ctr4blk + i*16).
 * For key i, writes enc(ctr_i) || enc(ctr_i+1) to out + i*32. Byte-identical to four
 * aes128_ctrle_nocarry_2block_avx2 calls, but the 8 independent aesenc chains keep the AES-NI
 * port saturated (the per-node 2-block calls only had 2 in flight, latency-bound). */
void aes128_ctrle_nocarry_2blk_x4keys_avx2(void* out, const void* round_keys4, const void* ctr4blk) {
  const uint8_t* rk = (const uint8_t*)round_keys4;
  const uint8_t* c = (const uint8_t*)ctr4blk;
  uint8_t* o = (uint8_t*)out;
  __m128i s[8];
  for (int i=0;i<4;i++) {
    __m128i base = _mm_loadu_si128((const __m128i*)(c + 16*i));
    s[2*i]   = base;
    s[2*i+1] = sdith_ctr_lo(base, 1);
  }
  for (int i=0;i<4;i++) {
    __m128i k = _mm_loadu_si128((const __m128i*)(rk + 176*i));
    s[2*i]   = _mm_xor_si128(s[2*i],   k);
    s[2*i+1] = _mm_xor_si128(s[2*i+1], k);
  }
  for (int off=16; off<=144; off+=16) {
    for (int i=0;i<4;i++) {
      __m128i k = _mm_loadu_si128((const __m128i*)(rk + 176*i + off));
      s[2*i]   = _mm_aesenc_si128(s[2*i],   k);
      s[2*i+1] = _mm_aesenc_si128(s[2*i+1], k);
    }
  }
  for (int i=0;i<4;i++) {
    __m128i k = _mm_loadu_si128((const __m128i*)(rk + 176*i + 160));
    s[2*i]   = _mm_aesenclast_si128(s[2*i],   k);
    s[2*i+1] = _mm_aesenclast_si128(s[2*i+1], k);
    _mm_storeu_si128((__m128i*)(o + 32*i),      s[2*i]);
    _mm_storeu_si128((__m128i*)(o + 32*i + 16), s[2*i+1]);
  }
}

// AES proof-of-work grind (2 independent keys). This is the inner loop of the aes-based
// proofow_grind: it encrypts two counter blocks p0,p1 under two independent round-key sets
// rk[0], rk[1], incrementing only the low 64 bits of both plaintexts by 1 each step (no carry
// into the high 64 bits), and stops as soon as ((c0.lo64 ^ c1.lo64) & mask_w) == 0.
//
// The two keys' encryptions are interleaved GROUP counters at a time (GROUP*2 blocks in flight)
// to hide aesenc latency, matching the scalar loop bit-for-bit (aes128_ctrle_nocarry_1block on
// each block).
int aes128_proofow_grind_avx2(  //
    void* out_ct256,                   // [out] c0 || c1 (32 bytes) satisfying low64(c0 ^ c1) & mask_w == 0
    uint64_t* in_out_counter,          // [in/out] starting counter (< 2^32)
    const void* in_pt256,              // [in] p0 || p1 (32 bytes) the initial plaintexts
    const void* rk128,                 // [in] matrix of round keys v128[2][11] (352 bytes)
    uint64_t mask_w                    // [in] 64-bit mask applied to the xor of the ciphertext low words
) {
  enum { GROUP = 4 };
  const uint64_t CTR_MAX = UINT64_C(1) << 32;
  const uint8_t* rk0 = (const uint8_t*)rk128;         // rk[0][0..10]
  const uint8_t* rk1 = (const uint8_t*)rk128 + 16*11; // rk[1][0..10]
  const __m128i k0_0 = _mm_loadu_si128((const __m128i*)rk0);
  const __m128i k1_0 = _mm_loadu_si128((const __m128i*)rk1);
  __m128i base0 = _mm_loadu_si128((const __m128i*)in_pt256);
  __m128i base1 = _mm_loadu_si128((const __m128i*)((const uint8_t*)in_pt256 + 16));
  uint64_t ctr = *in_out_counter;

  while (ctr < CTR_MAX) {
    uint64_t remaining = CTR_MAX - ctr;
    int g = remaining < (uint64_t)GROUP ? (int)remaining : GROUP;
    __m128i s0[GROUP], s1[GROUP];
    for (int j = 0; j < g; j++) {
      __m128i d = _mm_set_epi64x(0, (long long)j);           // add j to the low 64 bits only
      s0[j] = _mm_xor_si128(_mm_add_epi64(base0, d), k0_0);
      s1[j] = _mm_xor_si128(_mm_add_epi64(base1, d), k1_0);
    }
    for (int r = 1; r < 10; r++) {
      __m128i k0 = _mm_loadu_si128((const __m128i*)(rk0 + 16*r));
      __m128i k1 = _mm_loadu_si128((const __m128i*)(rk1 + 16*r));
      for (int j = 0; j < g; j++) { s0[j] = _mm_aesenc_si128(s0[j], k0); s1[j] = _mm_aesenc_si128(s1[j], k1); }
    }
    __m128i k0_10 = _mm_loadu_si128((const __m128i*)(rk0 + 16*10));
    __m128i k1_10 = _mm_loadu_si128((const __m128i*)(rk1 + 16*10));
    for (int j = 0; j < g; j++) { s0[j] = _mm_aesenclast_si128(s0[j], k0_10); s1[j] = _mm_aesenclast_si128(s1[j], k1_10); }
    // check candidates in increasing-counter order; return the first hit
    for (int j = 0; j < g; j++) {
      uint64_t x0 = (uint64_t)_mm_cvtsi128_si64(s0[j]);
      uint64_t x1 = (uint64_t)_mm_cvtsi128_si64(s1[j]);
      if (((x0 ^ x1) & mask_w) == 0) {
        _mm_storeu_si128((__m128i*)out_ct256, s0[j]);
        _mm_storeu_si128((__m128i*)((uint8_t*)out_ct256 + 16), s1[j]);
        *in_out_counter = ctr + (uint64_t)j;
        return 1;
      }
    }
    ctr += (uint64_t)g;
    base0 = _mm_add_epi64(base0, _mm_set_epi64x(0, g));
    base1 = _mm_add_epi64(base1, _mm_set_epi64x(0, g));
  }
  *in_out_counter = CTR_MAX;  // no good counter within [initial, 2^32)
  return 0;
}
