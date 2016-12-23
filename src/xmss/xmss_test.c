/*
 * Copyright (c) 2016, Denis Butin and Stefan-Lukas Gazdag
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * This is to provide a test and demonstration of this reference implementation
 * of draft-irtf-cfrg-xmss-hash-based-signatures-06 for WOTS+ and XMSS(^MT).
 */

#include "xmssmt_draft.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

static int
IntegrationTests(int wotsVSxmss)
{
	unsigned int msglen = 480;
	unsigned int n = 32;
	unsigned int w = 16;

	if ((wotsVSxmss < 0) || (wotsVSxmss > 2))
		return 1;

	/*
	 * Manipulation of data before verification:
	 * If set to '0' no manipulation occurs
	 * If set to '1' every second message is manipulated
	 * If set to '2' every second signature is manipulated (index)
	 * If set to '3' every second signature is manipulated (actual sig part)
	 */
	unsigned int manipulation = 0;

	if (wotsVSxmss == 0) {
		puts("\n=========XMSS integration testing=========");
	} else if (wotsVSxmss == 1) {
		puts("\n=========XMSS^MT integration testing=========");
	} else {
		puts("\n=========Plain WOTS+ integration testing=========");
	}

	fputs("\nStarting to declare and populate wots_param para...\t", stdout);
	wots_param para;
	if (wots_param_init(&para, n, w) == 1) {
		fputs("Error: wots_param_init: wrong parameters\n", stderr);
		return 1;
	}
	puts("Done with wots_param_init.");
	unsigned int h = 10;
	unsigned int d = 5;	/* Only used for XMSS^MT */
	printf("Parameters: n = %u, w = %u, h = %u", para.n, para.w, h);
	if (wotsVSxmss == 1) {
		printf(", d = %u\n", d);
	} else {
		putchar('\n');
	}

	/* Number of desired message/signature pairs. */
	int pairs = 1 << h;

	/* Recycle variables, since we write as we go */
	unsigned char message[msglen];
	memset(message, 0, sizeof(message));
	unsigned char adrs[32] = { 0 };
	unsigned int successcount = 0;
	unsigned int successes[pairs];
	memset(successes, 0, sizeof(successes));

	if (wotsVSxmss == 0) {
		int check = 2;

		puts("Generating XMSS key pair...");

		xmss_sk SK;
		unsigned char pk[2 * para.n];
		memset(pk, 0, sizeof(pk));
		xmss_keyGen(&SK, pk, &para, h);
		puts("Done generating XMSS key pair.\n");

		unsigned char signature[4 + para.n + para.len * para.n +
					h * para.n];
		memset(signature, 0, sizeof(signature));
		puts("XMSS signature initialisation done.\n");

		for (int i = 1; i <= pairs; i++) {
			printf("Randomly generating message %d...\t", i);

			/* Generate a message randomly. Length is msglen bytes*/
			int rcmessage = RAND_bytes(message, msglen);
			if (rcmessage != 1) {
				fputs("Error: call to RAND_bytes during random "
				    "plaintext generation failed", stderr);
				return 1;
			}
			printf("Message %d generation successful.\n\n", i);

			printf("Signing message %i...\t", i);
			xmss_signing(&SK, message, msglen, signature, adrs);
			puts("Done signing.\n");

			if (manipulation == 1 && (i % 2) == 0) {
				message[1] ^= 1;
			} else if (manipulation == 2 && (i % 2) == 0) {
				signature[0] ^= 1;
			} else if (manipulation == 3 && (i % 2) == 0) {
				signature[n] ^= 1;
			}

			printf("Verifying signature on message %d...\t", i);
			check = xmss_verification(pk, signature, message,
					msglen, &para, h);
			printf("Done with verification of message %d.\n\n",i);

			if (check == 2) {
				puts("Did not get a return value!");
			} else if (check == 0) {
				successcount++;
				successes[successcount-1] = i;
				printf(GREEN "Verification successful for "
					"i = %u!\n\n" RESET, i);
				check = 2;
			} else if (check == 1) {
				printf(RED "Verification FAILED for "
						"i = %u!\n\n" RESET, i);
				check = 2;
			} else {
				fputs("Something's wrong with the \
				    verification!", stdout);
			}
		}
		printf("%u verifications successful, out of %u in total.\n",
				successcount, pairs);
		if (successcount > 0) {
			fputs("Indexes of successful verifications: [", stdout);
			for (unsigned int j = 0; j < successcount - 1; j++)
				printf("%u, ", successes[j]);

			printf("%u]\n", successes[successcount-1]);
		}
		xmss_sk_free(&SK);
	} else if (wotsVSxmss == 1) {
		int check = 2;

		puts("\nGenerating XMSS^MT key pair...");

		xmss_mt_sk SKmt;
		unsigned char pk[2 * para.n];
		memset(pk, 0, sizeof(pk));
		xmss_mt_keyGen(&SKmt, pk, &para, h, d);
		puts("Done generating XMSS^MT key pair.\n");

		unsigned int siglength = (int)ceil(h / 8.0) + para.n +
		    (h + para.len + (d - 1) * para.len) * n;
		unsigned char signature[siglength];
		memset(signature, 0, sizeof(signature));
		printf("XMSS^MT signature initialisation done "
				"(siglength = %u)\n\n", siglength);

		for (int i = 1; i <= pairs; i++) {
			printf("Randomly generating message %d...\t", i);
			/* Generate a message randomly. Length is msglen bytes*/
			int rcmessage = RAND_bytes(message, msglen);
			if (rcmessage != 1) {
				fputs("Error: call to RAND_bytes during random "
				    "plaintext generation failed\n", stderr);
				return 1;
			}
			printf("Message %d generation successful.\n\n", i);

			printf("Signing message %i...\t", i);
			xmss_mt_sign(&SKmt, message, msglen, signature);
			puts("Done signing.\n");

			if (manipulation == 1 && (i % 2) == 0) {
				message[1] ^= 1;
			} else if (manipulation == 2 && (i % 2) == 0) {
				signature[0] ^= 1;
			} else if (manipulation == 3 && (i % 2) == 0) {
				signature[n] ^= 1;
			}

			printf("Verifying signature on message %d...\t", i);
			check = xmss_mt_verify(pk, signature, message, msglen,
					&para, h, d);
			printf("Done with verification of message %d.\n\n",i);

			if (check == 2) {
				printf("Did not get a return value!\n");
			} else if (check == 0) {
				successcount++;
				successes[successcount-1] = i;
				printf(GREEN "Verification successful for "
						"i = %u!\n\n" RESET, i);
				check = 2;
			} else if (check == 1) {
				printf(RED "Verification FAILED for "
						"i = %u!\n\n" RESET, i);
				check = 2;
			} else {
				fputs("Something's wrong with the \
				    verification!", stdout);
			}
		}
		printf("%u verifications successful, out of %u in total.\n",
		    successcount, pairs);
		if (successcount > 0) {
			fputs("Indexes of successful verifications: [", stdout);
			for (unsigned int j = 0 ; j < successcount - 1 ; j++)
				printf("%u, ", successes[j]);

			printf("%u]\n", successes[successcount-1]);
		}
		xmss_mt_sk_free(&SKmt);
	} else if (wotsVSxmss == 2) {
		fputs("Randomly generate seed...\t", stdout);
		unsigned char *seed = calloc(para.n, sizeof(unsigned char));

		/* Generate n-byte seed randomly */
		int rcseed = RAND_bytes(seed, para.n);
		if (rcseed != 1) {
			fputs("Error: call to RAND_bytes during seed generation"
			    " failed\n", stderr);
			return 1;
		}
		fputs("Seed generated.\nRandomly generating message...\t",
		    stdout);

		/* Generate a message randomly. Length is n bytes */
		int rcmessage = RAND_bytes(message, para.n);
		if (rcmessage != 1) {
			fprintf(stderr, "Error: call to RAND_bytes during "
					"random plaintext generation failed\n");
			return 1;
		}
		puts("Message generation successful.\n");

		unsigned int len = para.len, n = para.n;
		unsigned char sk[len * n], pk[len * n], sig[len * n],
				tmp_pk[len * n];
		memset(sk, 0, sizeof(sk));
		memset(pk, 0, sizeof(pk));
		memset(sig, 0, sizeof(sig));
		memset(tmp_pk, 0, sizeof(tmp_pk));
		puts("Declaration of sk, pk, sig and tmp_pk done.\n");

		int test = 0;
		setType(adrs, 0);

		genWotsPrivKey(&para, sk);
		puts("genWotsPrivKey complete.\n");

		genWotsPubKey(pk, &para, sk, seed, adrs);
		puts("genWotsPubKey complete.\n");

		genWotsSig(sk, &para, message, sig, seed, adrs);
		puts("genWotsSig complete.\n");

		wotsPubKeyFromSig(&para, message, sig, tmp_pk, seed, adrs);
		puts("wotsPubKeyFromSig complete.\n");

		for (unsigned int i = 0; i < n * len; i++) {
			if (pk[i] != tmp_pk[i]) {
				printf(RED "WOTS verification FAILED for index "
				    "%u!\n" RESET, i);
				test = 1;
			}

			if (pk[i] == tmp_pk[i]) {
				printf(GREEN "WOTS verification OK for index "
				    "%u!\n" RESET, i);
			}
		}

		if (test == 0)
			puts(GREEN "WOTS verification succeeded!" RESET);

		free(seed);
	}

	return 0;
}

int
main(void)
{
	/*
	 * Returns '-1' if no test was run
	 * Returns '0' if test have run successfully
	 * Returns '1' or higher, if one ore more test runs have failed
	 */
	int ret = 0;

	/*
	 * If 'tests' is set to '0', we sign and verify messages using XMSS only
	 * If it is set to '1', we sign and verify messages using XMSS^MT only
	 * If it is set to '2', we sign and verify messages using WOTS+ only
	 * If it is set to '3', tests are run for XMSS as well as XMSS^MT
	 */
	int tests = 3;

	if (tests >= 0 && tests < 3) {
		if (IntegrationTests(tests) == 1)
			ret = 1;
	} else if (tests == 3) {
		if (IntegrationTests(0) == 1)
			ret++;
		if (IntegrationTests(1) == 1)
			ret++;
	} else {
		return -1;
	}

	return ret;
}

