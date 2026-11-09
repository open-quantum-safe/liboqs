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

static inline uint16_t mod(uint16_t i, uint16_t modulus);
static inline uint16_t make_nonzero_mask(uint16_t x);
static inline uint8_t is_zero(uint8_t x);
static inline void compute_syndromes(uint16_t *syndromes, const uint8_t *cdw);
static inline void compute_error_locator_polynomial(uint16_t *sigma, const uint16_t *syndromes);
static inline void compute_error_positions(uint8_t *error_positions, const uint16_t *sigma);
static inline uint16_t evaluate_poly_at_powers(const uint16_t *poly, const uint8_t *beta_inv_pows);
static inline void batch_inverse(uint8_t *inv, const uint8_t *a);
static inline void correct_errors(uint8_t *message, const uint8_t *cdw_bytes, const uint16_t *syndromes,
                                  const uint16_t *sigma, const uint8_t *error_positions);

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
 * @brief Encodes a message message of PARAM_K bits to a Reed-Solomon codeword codeword of PARAM_N1 bytes
 *
 * Following @cite lin1983error (Chapter 4 - Cyclic Codes),
 * We perform a systematic encoding using a linear (PARAM_N1 - PARAM_K)-stage shift register
 * with feedback connections based on the generator polynomial rs_generator_polynomial of the Reed-Solomon code.
 *
 * @param[out] cdw Array of size VEC_N1_SIZE_64 receiving the encoded message
 * @param[in] msg Array of size VEC_K_SIZE_64 storing the message
 */
void reed_solomon_encode(uint64_t *cdw, const uint64_t *msg) {
    size_t i, k;
    uint8_t gate_value = 0;

    uint16_t rs_generator_polynomial[] = {RS_POLY_COEFS};

    uint8_t msg_bytes[PARAM_K] = {0};
    uint8_t cdw_bytes[PARAM_N1] = {0};

    memcpy(msg_bytes, msg, PARAM_K);

    for (i = 0; i < PARAM_K; ++i) {
        gate_value = msg_bytes[PARAM_K - 1 - i] ^ cdw_bytes[PARAM_N1 - PARAM_K - 1];

        for (k = PARAM_N1 - PARAM_K - 1; k; --k) {
            cdw_bytes[k] = cdw_bytes[k - 1] ^ gf_mul(gate_value, rs_generator_polynomial[k]);
        }

        cdw_bytes[0] = gf_mul(gate_value, rs_generator_polynomial[0]);
    }

    memcpy(cdw_bytes + PARAM_N1 - PARAM_K, msg_bytes, PARAM_K);
    memcpy(cdw, cdw_bytes, PARAM_N1);
}

/**
 * @brief Compute the 2·PARAM_T Reed–Solomon syndromes.
 *
 * For each syndrome index \c i, this evaluates the received codeword polynomial at the
 * corresponding RS evaluation point (as defined by the precomputed table
 * \c alpha_ij_pow) and stores the result in \c syndromes[i] (GF(2^8) in the low byte).
 *
 * @param[out] syndromes Output array of size \c 2*PARAM_T.
 * @param[in]  cdw       Received codeword bytes (size \c PARAM_N1).
 */
static inline void compute_syndromes(uint16_t *syndromes, const uint8_t *cdw) {
    for (size_t i = 0; i < 2 * PARAM_T; ++i) {
        for (size_t j = 1; j < PARAM_N1; ++j) {
            syndromes[i] ^= gf_mul(cdw[j], alpha_ij_pow[i][j - 1]);
        }
        syndromes[i] ^= cdw[0];
    }

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
 * @brief Compute error positions on the message part using Chien search.
 *
 * Evaluates the error-locator polynomial \f$\sigma(x)\f$ at each message evaluation point
 * and sets a 0/1 flag indicating whether that point is a root.
 *
 * @param[out] error Output error flags (size \c PARAM_K), each entry is 0 or 1.
 * @param[in]  sigma Error-locator polynomial coefficients (size \c PARAM_T+1).
 */
static inline void compute_error_positions(uint8_t *error_positions, const uint16_t *sigma) {
    for (size_t i = 0; i < PARAM_K; ++i) {
        const uint8_t *beta_inv_pows = &eval_alpha_pow[i][0];
        uint16_t val = evaluate_poly_at_powers(sigma, beta_inv_pows);
        error_positions[i] = is_zero((uint8_t)(val & 0xFFu));
    }
}

/**
 * @brief Evaluate a PARAM_T-capped polynomial at a point using precomputed powers.
 *
 * Computes:
 *   out = poly[0] ⊕ Σ_{j=1..PARAM_T} poly[j] · beta_inv^j
 * where the powers (beta_inv^1..beta_inv^PARAM_T) are provided in @p beta_inv_pows.
 *
 * @param[in] poly          Polynomial coefficients (size PARAM_T+1).
 * @param[in] beta_inv_pows Precomputed powers {beta_inv^1, beta_inv^2, ..., beta_inv^PARAM_T}
 *                          (size PARAM_T).
 * @return The polynomial value in GF(2^8) (stored in a uint16_t).
 */
static inline uint16_t evaluate_poly_at_powers(const uint16_t *poly, const uint8_t *beta_inv_pows) {
    uint16_t out = poly[0];

    for (uint16_t j = 1; j <= PARAM_T; ++j) {
        uint16_t beta_j = beta_inv_pows[j - 1];
        out ^= gf_mul(poly[j], beta_j);
    }
    return out;
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

    for (size_t i = 0; i < PARAM_K; ++i) {
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
 *  3) Evaluate Z and \f$\sigma_{\text{odd}}\f$ at all message points and compute error values for
 *     positions marked in \p error_positions.
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
    for (uint16_t i = 0; i < PARAM_K; ++i) {
        const uint8_t *beta_inv_pows = &eval_alpha_pow[i][0];

        numerator_array[i] = (uint8_t)evaluate_poly_at_powers(z, beta_inv_pows);
        denominator_array[i] = (uint8_t)evaluate_poly_at_powers(sigma_odd, beta_inv_pows);
    }

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
                size_t pos_cw = (size_t)(PARAM_N1 - PARAM_K) + i;  // map to codeword index
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
    uint16_t syndromes[2 * PARAM_T] = {0};
    uint16_t sigma[PARAM_T + 1] = {0};
    uint8_t error_positions[PARAM_K] = {0};
    uint8_t message[PARAM_K] = {0};

    memcpy(cdw_bytes, cdw, PARAM_N1);

    compute_syndromes(syndromes, cdw_bytes);
    compute_error_locator_polynomial(sigma, syndromes);
    compute_error_positions(error_positions, sigma);
    correct_errors(message, cdw_bytes, syndromes, sigma, error_positions);

    memcpy(msg, message, PARAM_K);

    memset_zero(cdw_bytes, sizeof cdw_bytes);
    memset_zero(syndromes, sizeof syndromes);
    memset_zero(sigma, sizeof sigma);
    memset_zero(error_positions, sizeof error_positions);
    memset_zero(message, sizeof message);
}
