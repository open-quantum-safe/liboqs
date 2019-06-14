#include "inttypes.h"
#include "ntt.h"
#include "params.h"
#include "reduce.h"

/************************************************************
* Name:        bitrev_table
*
* Description: Contains bit-reversed 9-bit indices to be used to re-order
*              polynomials before number theoratic transform
************************************************************/
static uint16_t bitrev_table [512] = {
    0, 256, 128, 384, 64, 320, 192, 448, 32, 288, 160, 416, 96, 352, 224, 480, 16, 272, 144, 400, 80, 336, 208, 464, 48, 304, 176, 432, 112, 368, 240, 496, 8,
    264, 136, 392, 72, 328, 200, 456, 40, 296, 168, 424, 104, 360, 232, 488, 24, 280, 152, 408, 88, 344, 216, 472, 56, 312, 184, 440, 120, 376, 248, 504, 4,
    260, 132, 388, 68, 324, 196, 452, 36, 292, 164, 420, 100, 356, 228, 484, 20, 276, 148, 404, 84, 340, 212, 468, 52, 308, 180, 436, 116, 372, 244, 500, 12,
    268, 140, 396, 76, 332, 204, 460, 44, 300, 172, 428, 108, 364, 236, 492, 28, 284, 156, 412, 92, 348, 220, 476, 60, 316, 188, 444, 124, 380, 252, 508, 2,
    258, 130, 386, 66, 322, 194, 450, 34, 290, 162, 418, 98, 354, 226, 482, 18, 274, 146, 402, 82, 338, 210, 466, 50, 306, 178, 434, 114, 370, 242, 498, 10,
    266, 138, 394, 74, 330, 202, 458, 42, 298, 170, 426, 106, 362, 234, 490, 26, 282, 154, 410, 90, 346, 218, 474, 58, 314, 186, 442, 122, 378, 250, 506, 6,
    262, 134, 390, 70, 326, 198, 454, 38, 294, 166, 422, 102, 358, 230, 486, 22, 278, 150, 406, 86, 342, 214, 470, 54, 310, 182, 438, 118, 374, 246, 502, 14,
    270, 142, 398, 78, 334, 206, 462, 46, 302, 174, 430, 110, 366, 238, 494, 30, 286, 158, 414, 94, 350, 222, 478, 62, 318, 190, 446, 126, 382, 254, 510, 1,
    257, 129, 385, 65, 321, 193, 449, 33, 289, 161, 417, 97, 353, 225, 481, 17, 273, 145, 401, 81, 337, 209, 465, 49, 305, 177, 433, 113, 369, 241, 497, 9,
    265, 137, 393, 73, 329, 201, 457, 41, 297, 169, 425, 105, 361, 233, 489, 25, 281, 153, 409, 89, 345, 217, 473, 57, 313, 185, 441, 121, 377, 249, 505, 5,
    261, 133, 389, 69, 325, 197, 453, 37, 293, 165, 421, 101, 357, 229, 485, 21, 277, 149, 405, 85, 341, 213, 469, 53, 309, 181, 437, 117, 373, 245, 501, 13,
    269, 141, 397, 77, 333, 205, 461, 45, 301, 173, 429, 109, 365, 237, 493, 29, 285, 157, 413, 93, 349, 221, 477, 61, 317, 189, 445, 125, 381, 253, 509, 3,
    259, 131, 387, 67, 323, 195, 451, 35, 291, 163, 419, 99, 355, 227, 483, 19, 275, 147, 403, 83, 339, 211, 467, 51, 307, 179, 435, 115, 371, 243, 499, 11,
    267, 139, 395, 75, 331, 203, 459, 43, 299, 171, 427, 107, 363, 235, 491, 27, 283, 155, 411, 91, 347, 219, 475, 59, 315, 187, 443, 123, 379, 251, 507, 7,
    263, 135, 391, 71, 327, 199, 455, 39, 295, 167, 423, 103, 359, 231, 487, 23, 279, 151, 407, 87, 343, 215, 471, 55, 311, 183, 439, 119, 375, 247, 503, 15,
    271, 143, 399, 79, 335, 207, 463, 47, 303, 175, 431, 111, 367, 239, 495, 31, 287, 159, 415, 95, 351, 223, 479, 63, 319, 191, 447, 127, 383, 255, 511
};

/*************************************************
* Name:        bitrev_vector
*
* Description: Permutes coefficients of a polynomial into bitreversed order
*
* Arguments:   - uint16_t* poly: pointer to in/output polynomial
**************************************************/
void PQCLEAN_NEWHOPE512CCA_CLEAN_bitrev_vector(uint16_t *poly) {
    unsigned int i, r;
    uint16_t tmp;

    for (i = 0; i < NEWHOPE_N; i++) {
        r = bitrev_table[i];
        if (i < r) {
            tmp = poly[i];
            poly[i] = poly[r];
            poly[r] = tmp;
        }
    }
}

/*************************************************
* Name:        mul_coefficients
*
* Description: Performs pointwise (coefficient-wise) multiplication
*              of two polynomials
* Arguments:   - uint16_t* poly:          pointer to in/output polynomial
*              - const uint16_t* factors: pointer to input polynomial, coefficients
*                                         are assumed to be in Montgomery representation
**************************************************/
void PQCLEAN_NEWHOPE512CCA_CLEAN_mul_coefficients(uint16_t *poly, const uint16_t *factors) {
    unsigned int i;

    for (i = 0; i < NEWHOPE_N; i++) {
        poly[i] = PQCLEAN_NEWHOPE512CCA_CLEAN_montgomery_reduce((poly[i] * factors[i]));
    }
}


/*************************************************
* Name:        ntt
*
* Description: Computes number-theoretic transform (NTT) of
*              a polynomial in place; inputs assumed to be in
*              bitreversed order, output in normal order
*
* Arguments:   - uint16_t * a:          pointer to in/output polynomial
*              - const uint16_t* omega: pointer to input powers of root of unity omega;
*                                       assumed to be in Montgomery domain
**************************************************/
void PQCLEAN_NEWHOPE512CCA_CLEAN_ntt(uint16_t *a, const uint16_t *omega) {
    int i, start, j, jTwiddle, distance;
    uint16_t temp, W;


    for (i = 0; i < 9; i += 2) {
        // Even level
        distance = (1 << i);
        for (start = 0; start < distance; start++) {
            jTwiddle = 0;
            for (j = start; j < NEWHOPE_N - 1; j += 2 * distance) {
                W = omega[jTwiddle++];
                temp = a[j];
                a[j] = (temp + a[j + distance]); // Omit reduction (be lazy)
                a[j + distance] = PQCLEAN_NEWHOPE512CCA_CLEAN_montgomery_reduce((W * ((uint32_t)temp + 3 * NEWHOPE_Q - a[j + distance])));
            }
        }
        if (i + 1 < 9) {
            // Odd level
            distance <<= 1;
            for (start = 0; start < distance; start++) {
                jTwiddle = 0;
                for (j = start; j < NEWHOPE_N - 1; j += 2 * distance) {
                    W = omega[jTwiddle++];
                    temp = a[j];
                    a[j] = (temp + a[j + distance]) % NEWHOPE_Q;
                    a[j + distance] = PQCLEAN_NEWHOPE512CCA_CLEAN_montgomery_reduce((W * ((uint32_t)temp + 3 * NEWHOPE_Q - a[j + distance])));
                }
            }
        }
    }
}
