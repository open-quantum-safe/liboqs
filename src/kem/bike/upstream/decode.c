/******************************************************************************
 * BIKE -- Bit Flipping Key Encapsulation
 *
 * Copyright (c) 2017 Nir Drucker, Shay Gueron, Rafael Misoczki, Tobias Oder, Tim Gueneysu
 * (drucker.nir@gmail.com, shay.gueron@gmail.com, rafael.misoczki@intel.com, tobias.oder@rub.de, tim.gueneysu@rub.de)
 *
 * Permission to use this code for BIKE is granted.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * The names of the contributors may not be used to endorse or promote
 *   products derived from this software without specific prior written
 *   permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ""AS IS"" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS CORPORATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "decode.h"
#include "utilities.h"

#include "sampling.h"
#include "aes_ctr_prf.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

// count number of 1's in tmp:
uint32_t getHammingWeight(const uint8_t tmp[R_BITS], const uint32_t length)
{
    uint32_t count = 0;
    for (uint32_t i = 0; i < length; i++)
    {
        count+=tmp[i];
    }

    return count;
}

// function (not constant time) to check if an array is zero:
uint32_t isZero(uint8_t s[R_BITS])
{
    for (uint32_t i = 0; i < R_BITS; i++)
    {
        if (s[i])
        {
            return 0;
        }
    }
    return 1;
}

uint32_t get_predefined_threshold_var(const uint8_t s[R_BITS])
{
    // compute syndrome weight:
    uint32_t syndromeWeight = getHammingWeight(s, R_BITS);

    // set threshold according to syndrome weight:
    uint32_t threshold = ceil(VAR_TH_FCT(syndromeWeight));

    DMSG("    Thresold: %d\n", threshold);
    return threshold;
}

// compute the max number of unsatisfied parity-check equations:
int get_max_upc(uint8_t unsat_counter[N_BITS])
{
    int maxupc = -1;
    for (uint32_t i = 0; i < N_BITS; i++)
        if (unsat_counter[i] > maxupc)
            maxupc = unsat_counter[i];
    return  maxupc;
}           

void recompute_syndrome(uint8_t s[R_BITS],
        const uint32_t numPositions,
        const uint32_t positions[N_BITS],
        const uint32_t h0_compact[DV],
        const uint32_t h1_compact[DV])
{
    for (uint32_t i = 0; i < numPositions; i++)
    {
        uint32_t pos = positions[i];
        if (pos < R_BITS)
        {
            for (uint32_t j = 0; j < DV; j++)
            {
                if (h0_compact[j] <= pos) 
                {
                    s[pos - h0_compact[j]] ^= 1;
                }
                else 
                {
                    s[R_BITS - h0_compact[j] +  pos] ^= 1;
                }
            }
        }
        else
        {
            pos = pos - R_BITS;
            for (uint32_t j = 0; j < DV; j++)
            {
                if (h1_compact[j] <= pos)
                    s[pos - h1_compact[j]] ^= 1;
                else
                    s[R_BITS - h1_compact[j] + pos] ^= 1;
            }
        }
    }
}

void compute_counter_of_unsat(uint8_t unsat_counter[N_BITS],
        const uint8_t s[R_BITS],
        const uint32_t h0_compact[DV],
        const uint32_t h1_compact[DV])
{
    uint8_t unsat_counter2[N_BITS*2] = {0};
    uint32_t h1_compact2[DV] = {0};

    for (uint32_t i = 0; i < DV; i++)
    {
        h1_compact2[i] = N_BITS + h1_compact[i];
    }

    for (uint32_t i = 0; i < R_BITS; i++)
    {
        if (!s[i])
        {
            continue; 
        }

        for (uint32_t j = 0; j < DV; j++)
        {
            unsat_counter2[h0_compact[j] + i]++;
            unsat_counter2[h1_compact2[j] + i]++;
        }
    }

    for (uint32_t i = 0; i < R_BITS; i++)
    {
        unsat_counter[i] = unsat_counter2[i] + unsat_counter2[R_BITS+i];
        unsat_counter[R_BITS+i] = \
                unsat_counter2[N_BITS+i] + unsat_counter2[N_BITS+R_BITS+i];
    }
}

uint32_t ctr(
        uint32_t h_compact_col[DV],
        int position,
        uint8_t s[R_BITS])
{
    uint32_t count = 0;
    for (uint32_t i = 0; i < DV; i++)
    {
        if (s[(h_compact_col[i] + position) % R_BITS])
            count++;
    }
    return count;
}

void getCol(
        uint32_t h_compact_col[DV],
        uint32_t h_compact_row[DV])
{
    if (h_compact_row[0] == 0)
    {
        h_compact_col[0] = 0;

        for (uint32_t i = 1; i < DV; i++)
        {
            // set indices in increasing order:
            h_compact_col[i] = R_BITS - h_compact_row[DV-i];
        }
    } else
    {
        for (uint32_t i = 0; i < DV; i++)
        {
            // set indices in increasing order:
            h_compact_col[i] = R_BITS - h_compact_row[DV-1-i];
        }
    }
}

// The position in e is adjusted because syndrome is transposed.
void flipAdjustedErrorPosition(uint8_t e[R_BITS*2], uint32_t position)
{
    uint32_t adjustedPosition = position;
    if (position != 0 && position != R_BITS)
    {
        adjustedPosition = (position > R_BITS) ? \
                ((N_BITS - position)+R_BITS) : (R_BITS - position);
    }
    e[adjustedPosition] ^= 1;
}

void check(
        uint8_t e[R_BITS*2],
        uint32_t h0_compact_col[DV],
        uint32_t h1_compact_col[DV],
        uint32_t h0_compact[DV],
        uint32_t h1_compact[DV],
        uint8_t s[R_BITS],
        uint32_t Jl[4*DV],
        uint32_t sizeJl)
{
    for (uint32_t j = 0; j < sizeJl; j++)
    {
        uint32_t pos = Jl[j];
        if (pos < R_BITS)
        {
            uint32_t counter_unsat_pos = ctr(h0_compact_col, pos, s);
            if (counter_unsat_pos > (DV/2))
            {
                flipAdjustedErrorPosition(e, pos);
                recompute_syndrome(s, 1, &pos, h0_compact, h1_compact);
                DMSG("    Weight of syndrome: %d\n",
                        getHammingWeight(s, R_BITS));
            }
        }
        else
        {
            uint32_t counter_unsat_pos = ctr(h1_compact_col, pos-R_BITS, s);
            if (counter_unsat_pos > (DV/2))
            {
                flipAdjustedErrorPosition(e, pos);
                recompute_syndrome(s, 1, &pos, h0_compact, h1_compact);
                DMSG("    Weight of syndrome: %d\n",
                        getHammingWeight(s, R_BITS));
            }
        }
    }
}

#define MIN(a, b) ((a > b) ? b : a);

// Algorithm 3: One-Round Bit Flipping Algorithm
int decode(uint8_t e[R_BITS*2],
        uint8_t s[R_BITS],
        uint32_t h0_compact[DV],
        uint32_t h1_compact[DV],
        uint32_t u)
{

    // PRNG tools:
    double_seed_t seeds = {0};
    get_seeds(&seeds, DECAPS_SEEDS);
    aes_ctr_prf_state_t prf_state = {0};
    init_aes_ctr_prf_state(&prf_state, (MASK(32)), &seeds.u.v.s1);

    // computing the first column of each parity-check block:
    uint32_t h0_compact_col[DV] = {0};
    uint32_t h1_compact_col[DV] = {0};
    getCol(h0_compact_col, h0_compact);
    getCol(h1_compact_col, h1_compact);

    // J: list of positions involved in more than
    // (threshold - delta) unsatisfied p.c. equations:
    uint32_t J[DELTA_BIT_FLIPPING][MAX_J_SIZE] = {0};
    uint32_t sizeJ[DELTA_BIT_FLIPPING] = {0};

    // count the number of unsatisfied parity-checks:
    uint8_t unsat_counter[N_BITS] = {0};
    compute_counter_of_unsat(unsat_counter, s, h0_compact, h1_compact);

    // LINE 1 of One-Round Bit Flipping Algorithm:
    uint32_t threshold = get_predefined_threshold_var(s);
    DMSG("\t\t\tThreshold: %d\n", threshold);

    // LINES 2-4 of One-Round Bit Flipping Algorithm:
    for (uint32_t i = 0; i < N_BITS; i++)
    {
        if (unsat_counter[i] > threshold - DELTA_BIT_FLIPPING)
        {
            uint32_t difference = threshold - MIN(threshold, unsat_counter[i]);
            J[difference][sizeJ[difference]] = i;
            sizeJ[difference]++;
        }
    }

    // LINES 5-6 of One-Round Bit Flipping Algorithm:
    for (uint32_t i = 0; i < sizeJ[0]; i++)
    {
        flipAdjustedErrorPosition(e, J[0][i]);
    }

    recompute_syndrome(s, sizeJ[0], J[0], h0_compact, h1_compact);

    DMSG("\t\tStep 1. Weight(syndrome): %u Weight(error): %u.\n", getHammingWeight(s, R_BITS), getHammingWeight(e, N_BITS));

    // check if decoding finished:
    if (getHammingWeight(s, R_BITS) <= u)
    {
        DMSG("\t\tWeight(syndrome): %d\n", getHammingWeight(s, R_BITS));
        return 0;
    }

    // LINES 7-10 of One-Round Bit Flipping Algorithm:
    while (getHammingWeight(s, R_BITS) > S_BIT_FLIPPING)
    {
        for (int l = 0; l < DELTA_BIT_FLIPPING; l++)
        {
            check(e, h0_compact_col, h1_compact_col, h0_compact, h1_compact, s,
                    J[l], sizeJ[l]);
        }
        DMSG("\t\tStep 2 (loop). Weight(syndrome): %u Weight(error): %u\n", getHammingWeight(s, R_BITS), getHammingWeight(e, N_BITS));
    }

    // check if decoding finished:
    if (getHammingWeight(s, R_BITS) <= u)
    {
        DMSG("\t\tWeight(syndrome): %d\n", getHammingWeight(s, R_BITS));
        return 0;
    }

    // LINES 11-12 of One-Round Bit Flipping Algorithm:
    uint32_t errorPos[R_BITS] = {0};
    int countError = 0;
    for (uint32_t i = 0; i < 2*R_BITS; i++)
    {
        if (e[i])
        {
            uint32_t posError = i;
            if (i != 0 && i != R_BITS)
            {
                // the position in e is adjusted since syndrome is transposed
                posError = (i > R_BITS)? ((N_BITS - i)+R_BITS) : (R_BITS - i);
            }
            errorPos[countError++] = posError;
        }
    }
    for (int j = 0; j < countError; j++)
    {
        uint32_t pos = errorPos[j];
        uint32_t counter_unsat_pos;

        if (pos < R_BITS)
        {
            counter_unsat_pos = ctr(h0_compact_col, pos, s);
        }
        else
        {
            counter_unsat_pos = ctr(h1_compact_col, pos-R_BITS, s);
        }

        if (counter_unsat_pos > (DV/2))
        {
            flipAdjustedErrorPosition(e, pos);
            recompute_syndrome(s, 1, &pos, h0_compact, h1_compact);
        }
    }

    DMSG("\t\tStep 3. Weight(syndrome): %u Weight(error): %u.\n", getHammingWeight(s, R_BITS), getHammingWeight(e, N_BITS));

    // check if decoding finished:
    if (getHammingWeight(s, R_BITS) <= u)
    {
        DMSG("\t\tWeight(syndrome): %d\n", getHammingWeight(s, R_BITS));
        return 0;
    }

    // LINES 13-15 of One-Round Bit Flipping Algorithm:
    while (getHammingWeight(s, R_BITS) > u)
    {
        // find a random non-zero position in the syndrome:
        uint32_t i = 0;
        get_rand_mod_len(&i, R_BITS, &prf_state);
        while (!s[i])
            i = (i + 1) % R_BITS;

        int errorFound = 0;
        for (uint32_t j = 0; j < DV && !errorFound; j++)
        {
            // finding position of 1 in the i-th row:
            uint32_t pos = (h0_compact[j] + i) % R_BITS;
            uint32_t counter_unsat_pos = ctr(h0_compact_col, pos, s);
            if (counter_unsat_pos > (DV/2))
            {
                flipAdjustedErrorPosition(e, pos);
                recompute_syndrome(s, 1, &pos, h0_compact, h1_compact);
                errorFound = 1;
                DMSG("\t\t\tFlipped position %d which has counter_unsat_pos: %d\n", pos, counter_unsat_pos);
            }
        }
        for (uint32_t j = 0; j < DV && !errorFound; j++)
        {
            // finding position of 1 in the i-th row:
            uint32_t pos = (h1_compact[j] + i) % R_BITS;
            pos += R_BITS;
            uint32_t counter_unsat_pos = ctr(h1_compact_col, pos, s);
            if (counter_unsat_pos > (DV/2))
            {
                flipAdjustedErrorPosition(e, pos);
                recompute_syndrome(s, 1, &pos, h0_compact, h1_compact);
                errorFound = 1;
                DMSG("\t\t\tFlipped position %d which has counter_unsat_pos: %d\n", pos, counter_unsat_pos);
            }
        }
        DMSG("\t\t\t\tStep 4 (loop). Weight(syndrome): %d Weight(error): %d\n", getHammingWeight(s, R_BITS), getHammingWeight(e, N_BITS));
    }
    // check if decoding finished:
    if (getHammingWeight(s, R_BITS) <= u)
    {
        DMSG("\t\tWeight(syndrome): %d\n", getHammingWeight(s, R_BITS));
        return 0;
    }

    return -1;
}
