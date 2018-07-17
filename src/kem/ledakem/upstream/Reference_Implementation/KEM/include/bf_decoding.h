/**
 *
 * <bf_deconding.h>
 *
 * @version 1.0 (September 2017)
 *
 * Reference ISO-C99 Implementation of LEDAkem cipher" using GCC built-ins.
 *
 * In alphabetical order:
 *
 * @author Marco Baldi <m.baldi@univpm.it>
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 * @author Franco Chiaraluce <f.chiaraluce@univpm.it>
 * @author Gerardo Pelosi <gerardo.pelosi@polimi.it>
 * @author Paolo Santini <p.santini@pm.univpm.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#pragma once
#include "qc_ldpc_parameters.h"
#include "gf2x_limbs.h"
#define              ITERATIONS_MAX   (15)


int bf_decoding(DIGIT err[],
                const POSITION_T HtrPosOnes[N0][DV],
                const POSITION_T QtrPosOnes[N0][M], // N0 vectors containing exp.s of Qtr ones
                DIGIT privateSyndrome[]       //  1 polynomial  -- param. in/out
               );

/******************** START of definitions for N0 = 2 *************************/
#if (CATEGORY == 1) && (N0 == 2)
#define SYNDROME_TRESH_LOOKUP_TABLE {    0,62},\
{ 4588,63},\
{ 7609,64},\
{ 9152,65},\
{10129,66},\
{10800,67},\
{11261,68},\
{11614,69},\
{11885,70}
#endif

#if ((CATEGORY == 2) || (CATEGORY == 3)) && (N0 == 2)
#define SYNDROME_TRESH_LOOKUP_TABLE {0    , 96,}, \
                                    {7395 , 97,}, \
                                    {14569, 98,}, \
                                    {18177, 99,}, \
                                    {20338,100,}, \
                                    {21789,101,}, \
                                    {22838,102,}, \
                                    {23628,103,}, \
                                    {24223,104,}, \
                                    {24723,105,}, \
                                    {25119,106,}, \
                                    {25455,107,}, \
                                    {25718,108,}, \
                                    {25961,109,}
#endif

#if ((CATEGORY == 4) || (CATEGORY == 5)) && (N0 == 2)
#define SYNDROME_TRESH_LOOKUP_TABLE {0    ,126}, \
                                    {11060,127}, \
                                    {24098,128}, \
                                    {30408,129}, \
                                    {34269,130}, \
                                    {36917,131}, \
                                    {38786,132}, \
                                    {40192,133}, \
                                    {41289,134}, \
                                    {42184,135}, \
                                    {42880,136}, \
                                    {43479,137}, \
                                    {43997,138}, \
                                    {44417,139}, \
                                    {44781,140}, \
                                    {45120,141}
#endif

/******************** START of definitions for N0 = 3 *************************/

#if (CATEGORY == 1) && (N0 == 3)
#define SYNDROME_TRESH_LOOKUP_TABLE {0   ,70}, \
                                    {4546,71}, \
                                    {5803,72}, \
                                    {6538,73}, \
                                    {7024,74}, \
                                    {7369,75}, \
                                    {7614,76}, \
                                    {7829,77}, \
                                    {7978,78}, \
                                    {8112,79}

#endif

#if ((CATEGORY == 2) || (CATEGORY == 3)) && (N0 == 3)
#define SYNDROME_TRESH_LOOKUP_TABLE  {    0,108}, \
                                     { 9160,109}, \
                                     {12218,110}, \
                                     {13942,111}, \
                                     {15152,112}, \
                                     {16005,113}, \
                                     {16603,114}, \
                                     {17089,115}, \
                                     {17485,116}, \
                                     {17808,117}, \
                                     {18072,118}, \
                                     {18287,119}, \
                                     {18461,120}

#endif

#if ((CATEGORY == 4) || (CATEGORY == 5)) && (N0 == 3)
#define SYNDROME_TRESH_LOOKUP_TABLE {0    ,146}, \
                                    {14621,147}, \
                                    {20360,148}, \
                                    {23618,149}, \
                                    {25803,150}, \
                                    {27330,151}, \
                                    {28450,152}, \
                                    {29320,153}, \
                                    {30042,154}, \
                                    {30644,155}, \
                                    {31106,156}, \
                                    {31493,157}, \
                                    {31849,158}, \
                                    {32146,159}, \
                                    {32392,160}, \
                                    {32623,161}, \
                                    {32840,162}
#endif

/******************** START of definitions for N0 = 4 *************************/


#if (CATEGORY == 1) && (N0 == 4)
#define SYNDROME_TRESH_LOOKUP_TABLE {   0,77}, \
                                    {3669,78}, \
                                    {4974,79}, \
                                    {5743,80}, \
                                    {6229,81}, \
                                    {6563,82}, \
                                    {6820,83}, \
                                    {7016,84}, \
                                    {7167,85}, \
                                    {7303,86}

#endif

#if ((CATEGORY == 2) || (CATEGORY == 3)) && (N0 == 4)
#define SYNDROME_TRESH_LOOKUP_TABLE {    0,114}, \
                                    { 6623,115}, \
                                    { 9694,116}, \
                                    {11382,117}, \
                                    {12456,118}, \
                                    {13235,119}, \
                                    {13801,120}, \
                                    {14209,121}, \
                                    {14572,122}, \
                                    {14862,123}, \
                                    {15092,124}, \
                                    {15274,125}, \
                                    {15442,126}, \
                                    {15598,127}
#endif

#if ((CATEGORY == 4) || (CATEGORY == 5)) && (N0 == 4)
#define SYNDROME_TRESH_LOOKUP_TABLE {0    ,153}, \
                                    {10000,154}, \
                                    {16098,155}, \
                                    {19224,156}, \
                                    {21134,157}, \
                                    {22486,158}, \
                                    {23505,159}, \
                                    {24272,160}, \
                                    {24899,161}, \
                                    {25413,162}, \
                                    {25833,163}, \
                                    {26217,164}, \
                                    {26493,165}, \
                                    {26786,166}, \
                                    {26990,167}, \
                                    {27214,168}, \
                                    {27393,169}
#endif
