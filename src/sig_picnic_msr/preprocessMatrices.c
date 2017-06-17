/*! @file preprocessMatrices.c
 *  @brief Utility program to generate lookup tables used for fast
 *  matrix multiplication. It outputs precomputed data to the data
 *  directory.
 *
 *  This file is part of the reference implementation of the Picnic and Fish
 *  signature schemes, described in the paper:
 *
 *  Post-Quantum Zero-Knowledge and Signatures from Symmetric-Key Primitives
 *  Melissa Chase and David Derler and Steven Goldfeder and Claudio Orlandi and
 *  Sebastian Ramacher and Christian Rechberger and Daniel Slamanig and Greg
 *  Zaverucha
 *  Cryptology ePrint Archive: Report 2017/279
 *  http://eprint.iacr.org/2017/279
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "matrices_256_10_38.h"
#include "matrices_256_42_14.h"
#include "matrices_256_1_316.h"

#include "LowMCConstants.h"
#include "LowMC.h"
#include "picnic.h"
#include <graycode.h>

#define t 8

/* Function in picnic.c not exposed in picnic.h */
int picnicParamsToLowMCParams(picnic_params_t picnicParams, lowmcparams_t *lowmcParams);

//static code* grayCode;

void xor (uint32_t * in1, uint32_t *in2, uint32_t *out, uint32_t numWords) {
	for (uint32_t i = 0; i < numWords; i++) {
		out[i] = in1[i] ^ in2[i];
	}
}

    void rotate(uint32_t input[STATE_SIZE_BITS][STATE_SIZE_WORDS], uint32_t output[STATE_SIZE_BITS][STATE_SIZE_WORDS]) {
	for (int i = 0; i < STATE_SIZE_BITS; i++) {
		for (int j = 0; j < STATE_SIZE_BITS; j++) {
			setBitInWordArray(output[STATE_SIZE_BITS - 1 - i], STATE_SIZE_BITS - 1 - j,
			                  getBitFromWordArray(input[j], i));
		}
	}
}

void generateTable(size_t numRounds, size_t stateBits, size_t stateWords,
                   uint32_t matrices[numRounds][stateBits][stateWords],
                   const char *matrixName, FILE *fp, FILE *fp2) {
	int *ord = malloc((1 << t) * sizeof(int));
	int *inc = malloc((1 << t) * sizeof(int));

	m4ri_build_code(ord, inc, t);
	uint32_t rotatedMat[stateBits][stateWords];

	uint32_t ****lookupTable = malloc(numRounds * sizeof(uint32_t ***));
	for (size_t i = 0; i < numRounds; i++) {
		lookupTable[i] = malloc(stateBits / t * sizeof(uint32_t **));
		for (size_t j = 0; j < stateBits / t; j++) {
			lookupTable[i][j] = malloc((1 << t) * sizeof(uint32_t *));
			for (int k = 0; k < (1 << t); k++) {
				lookupTable[i][j][k] = calloc(stateWords, sizeof(uint32_t));
			}
		}
	}

	for (size_t i = 0; i < numRounds; i++) {
		rotate(matrices[i], rotatedMat);
		for (size_t j = 0; j < stateBits / t; j++) {
			memset(lookupTable[i][j][0], 0, stateWords * sizeof(uint32_t));
			for (int k = 1; k < 1 << t; k++) {
				xor(lookupTable[i][(stateBits / t) - 1 - j][ord[k - 1]],
				    rotatedMat[(j * t) + (inc[k - 1])],
				    lookupTable[i][(stateBits / t) - 1 - j][ord[k]],
				    stateWords);
			}
		}
	}

	for (size_t i = 0; i < numRounds; i++) {
		for (size_t j = 0; j < stateBits / t; j++) {
			for (size_t k = 0; k < (1 << t); k++) {
				fwrite(lookupTable[i][j][k], sizeof(uint32_t), stateWords, fp);
			}
		}
	}

	size_t i, j, k, l;

	fprintf(fp2, "uint32_t %sLookupTable2 [%zd][%zd][%d][%zd] = {", matrixName,
	        numRounds, stateBits / t, 1 << t, stateWords);
	for (i = 0; i < numRounds; i++) {
		fprintf(fp2, "{");
		for (j = 0; j < stateBits / t; j++) {
			fprintf(fp2, "{");
			for (k = 0; k < (1 << t); k++) {
				fprintf(fp2, "{");
				for (l = 0; l < stateWords; l++) {
					fprintf(fp2, "0x%x", lookupTable[i][j][k][l]);
					if (l != stateWords - 1) {
						fprintf(fp2, ",");
					}
				}

				fprintf(fp2, "}");
				if (k != (1 << t) - 1) {
					fprintf(fp2, ", \n");
				}
			}

			fprintf(fp2, "}");
			if (j != (stateBits / t) - 1) {
				fprintf(fp2, ", \n");
			}
		}
		fprintf(fp2, "}");
		if (i != numRounds - 1) {
			fprintf(fp2, ", \n");
		}
	}
	fprintf(fp2, "};\n");
	for (size_t i = 0; i < numRounds; i++) {
		for (size_t j = 0; j < stateBits / t; j++) {
			for (int k = 0; k < (1 << t); k++) {
				free(lookupTable[i][j][k]);
			}
			free(lookupTable[i][j]);
		}
		free(lookupTable[i]);
	}
	free(lookupTable);
}

void writeRoundConstants(size_t numRounds, size_t stateWords, uint32_t roundConstants[numRounds][stateWords], FILE *fp) {
	for (size_t i = 0; i < numRounds; i++) {
		//for(int j = 0; j < stateWords;j++) {
		fwrite(&roundConstants[i], sizeof(uint32_t), stateWords, fp);
	}
	//}
}

void generatelookupTables(picnic_params_t picnic_params) {
	lowmcparams_t params = {0};

	picnicParamsToLowMCParams(picnic_params, &params);

	FILE *fp;
	char filename[100];
	sprintf(filename, DEFAULT_DATA_PATH "lookupTables_%d_%d_%d.bin", params.stateSizeBits, params.numSboxes, params.numRounds);
	char filename2[100];
	sprintf(filename2, DEFAULT_DATA_PATH "lookupTables_%d_%d_%d.h", params.stateSizeBits, params.numSboxes, params.numRounds);
	fp = fopen(filename, "wb");
	FILE *fp2;
	fp2 = fopen(filename2, "w+");

	if (fp == NULL || fp2 == NULL) {
		printf("Failed to open '%s' or '%s' for writing\n", filename, filename2);
		exit(-1);
	}

	switch (picnic_params) {
	case LowMC_256_256_42_14_FS:
	// fall through
	case LowMC_256_256_42_14_UR:
		generateTable(params.numRounds, params.stateSizeBits, params.stateSizeWords,
		              linearMatrices_256_42_14, "linearMatrices", fp, fp2);
		generateTable(params.numRounds + 1, params.stateSizeBits, params.stateSizeWords,
		              keyMatrices_256_42_14, "keyMatrices", fp, fp2);
		writeRoundConstants(params.numRounds, params.stateSizeWords, roundConstants256_42_14, fp);
		break;
	case LowMC_256_256_1_316_FS:
	// fall through
	case LowMC_256_256_1_316_UR:
		generateTable(params.numRounds, params.stateSizeBits, params.stateSizeWords,
		              linearMatrices_256_1_316, "linearMatrices", fp, fp2);
		generateTable(params.numRounds + 1, params.stateSizeBits, params.stateSizeWords,
		              keyMatrices_256_1_316, "keyMatrices", fp, fp2);
		writeRoundConstants(params.numRounds, params.stateSizeWords, roundConstants256_1_316, fp);

		break;
	case LowMC_256_256_10_38_FS:
	// fall through
	case LowMC_256_256_10_38_UR:
		generateTable(params.numRounds, params.stateSizeBits, params.stateSizeWords,
		              linearMatrices_256_10_38, "linearMatrices", fp, fp2);
		generateTable(params.numRounds + 1, params.stateSizeBits, params.stateSizeWords,
		              keyMatrices_256_10_38, "keyMatrices", fp, fp2);
		writeRoundConstants(params.numRounds, params.stateSizeWords, roundConstants256_10_38, fp);

		break;
	case PARAMETER_SET_INVALID:
	case PARAMETER_SET_MAX_INDEX:
		break;
	}
	fclose(fp);
}

int main() {
	for (picnic_params_t params = 1; params < PARAMETER_SET_MAX_INDEX; params++) {
		generatelookupTables(params);
	}
}
