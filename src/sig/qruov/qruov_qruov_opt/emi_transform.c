#include "emi_transform.h"

#if QRUOV_q == 31 && QRUOV_L == 3
void reevaluate_scalar_q31L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void interpolate_scalar_q31L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 127 && QRUOV_L == 3
void reevaluate_scalar_q127L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void interpolate_scalar_q127L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 31 && QRUOV_L == 10
void reevaluate_scalar_q31L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void interpolate_scalar_q31L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 7 && QRUOV_L == 10
void evaluate_scalar_q7L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void reevaluate_scalar_q7L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void interpolate_scalar_q7L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#elif QRUOV_q == 127 && QRUOV_L == 10
void reevaluate_scalar_q127L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
void interpolate_scalar_q127L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size);
#else
#error "Unsupported (q, L) for OPT EMI transform"
#endif

#if QRUOV_L == 3 && (QRUOV_q == 31 || QRUOV_q == 127)
static inline uint8_t subq_if_greater(int val)
{
    if (val >= QRUOV_q) {
        val -= QRUOV_q;
    }
    return (uint8_t)val;
}

static void evaluate_specialized_L3(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
    for (int i = 0; i < size; i++) {
        const int a0 = (int)input[0 * plane_in + i];
        const int a1 = (int)input[1 * plane_in + i];
        const int a2 = (int)input[2 * plane_in + i];
        const int a0a2 = subq_if_greater(a0 + a2);
        const int a2d = subq_if_greater(a2 + a2);
        const int a1a2d = subq_if_greater(a1 + a2d);
        const int a1da2q = subq_if_greater(a1a2d + a1a2d);

        output[0 * plane_out + i] = (uint8_t)a0;
        output[1 * plane_out + i] = subq_if_greater(a1 + a0a2);
        output[2 * plane_out + i] = subq_if_greater((QRUOV_q - a1) + a0a2);
        output[3 * plane_out + i] = subq_if_greater(a0 + a1da2q);
        output[4 * plane_out + i] = (uint8_t)a2;
    }
}
#endif

#if QRUOV_L == 10 && (QRUOV_q == 7 || QRUOV_q == 31 || QRUOV_q == 127)
static void evaluate_specialized_L10(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
#if QRUOV_q == 7
    /* q7/L10 uses a 23x10 packed matrix, not base-field evaluation points. */
    evaluate_scalar_q7L10(output, plane_out, input, plane_in, size);
#else
    for (int i = 0; i < size; i++) {
        const int a0 = (int)input[0 * plane_in + i];
        const int a1 = (int)input[1 * plane_in + i];
        const int a2 = (int)input[2 * plane_in + i];
        const int a3 = (int)input[3 * plane_in + i];
        const int a4 = (int)input[4 * plane_in + i];
        const int a5 = (int)input[5 * plane_in + i];
        const int a6 = (int)input[6 * plane_in + i];
        const int a7 = (int)input[7 * plane_in + i];
        const int a8 = (int)input[8 * plane_in + i];
        const int a9 = (int)input[9 * plane_in + i];

#define EVALUATE_PAIR(x, idx_pos, idx_neg) do { \
            const int even = a0 + QRUOV_POW2(x) * a2 + QRUOV_POW4(x) * a4 + QRUOV_POW6(x) * a6 + QRUOV_POW8(x) * a8; \
            const int odd = QRUOV_POW1(x) * a1 + QRUOV_POW3(x) * a3 + QRUOV_POW5(x) * a5 + QRUOV_POW7(x) * a7 + QRUOV_POW9(x) * a9; \
            int val = even + odd; \
            output[(idx_pos) * plane_out + i] = (uint8_t)(val % QRUOV_q); \
            val = even - odd; \
            val %= QRUOV_q; \
            if (val < 0) val += QRUOV_q; \
            output[(idx_neg) * plane_out + i] = (uint8_t)val; \
        } while (0)

#define EVALUATE_AT(x, idx) do { \
            int val = 0; \
            val += QRUOV_POW0(x) * a0; \
            val += QRUOV_POW1(x) * a1; \
            val += QRUOV_POW2(x) * a2; \
            val += QRUOV_POW3(x) * a3; \
            val += QRUOV_POW4(x) * a4; \
            val += QRUOV_POW5(x) * a5; \
            val += QRUOV_POW6(x) * a6; \
            val += QRUOV_POW7(x) * a7; \
            val += QRUOV_POW8(x) * a8; \
            val += QRUOV_POW9(x) * a9; \
            output[(idx) * plane_out + i] = (uint8_t)(val % QRUOV_q); \
        } while (0)

        output[0 * plane_out + i] = (uint8_t)a0;
        EVALUATE_PAIR(1, 1, 2);
        EVALUATE_PAIR(2, 3, 4);
        EVALUATE_PAIR(3, 5, 6);
        EVALUATE_PAIR(4, 7, 8);
        EVALUATE_PAIR(5, 9, 10);
        EVALUATE_PAIR(6, 11, 12);
        EVALUATE_PAIR(7, 13, 14);
        EVALUATE_PAIR(8, 15, 16);
        EVALUATE_AT(9, 17);
        output[18 * plane_out + i] = (uint8_t)a9;

#undef EVALUATE_AT
#undef EVALUATE_PAIR
    }
#endif
}
#endif

void evaluate(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
#if QRUOV_q == 31 && QRUOV_L == 3
    evaluate_specialized_L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 3
    evaluate_specialized_L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 10
    evaluate_specialized_L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 7 && QRUOV_L == 10
    evaluate_specialized_L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 10
    evaluate_specialized_L10(output, plane_out, input, plane_in, size);
#endif
}

void reevaluate(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
#if QRUOV_q == 31 && QRUOV_L == 3
    reevaluate_scalar_q31L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 3
    reevaluate_scalar_q127L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 10
    reevaluate_scalar_q31L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 7 && QRUOV_L == 10
    reevaluate_scalar_q7L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 10
    reevaluate_scalar_q127L10(output, plane_out, input, plane_in, size);
#endif
}

void interpolate(uint8_t *output, int plane_out, const uint8_t *input, int plane_in, int size)
{
#if QRUOV_q == 31 && QRUOV_L == 3
    interpolate_scalar_q31L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 3
    interpolate_scalar_q127L3(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 31 && QRUOV_L == 10
    interpolate_scalar_q31L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 7 && QRUOV_L == 10
    interpolate_scalar_q7L10(output, plane_out, input, plane_in, size);
#elif QRUOV_q == 127 && QRUOV_L == 10
    interpolate_scalar_q127L10(output, plane_out, input, plane_in, size);
#endif
}
