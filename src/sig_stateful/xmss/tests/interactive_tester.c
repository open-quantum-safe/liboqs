#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../xmss.h"
#include "../params.h"
#include "../randombytes.h"

#define XMSS_MLEN 32
#define XMSS_IMPLEMENTATION "XMSS-SHA2_20_256"

#define COMMAND_LEN 4
#define FILESTEM_LEN 32

static void hexdump(unsigned char *d, unsigned int l)
{
    for(unsigned int i=0; i<l  && i < 16;i++)
    {
        printf("%02x", d[i]);
    }
    printf("\n");
}

int test_case(const char *name, int xmssmt, unsigned int num_tests){
    xmss_params params;
    uint32_t oid;
    int ret = 0;
    unsigned int i;
    if(xmssmt){
        ret  = xmssmt_str_to_oid(&oid, name);
        ret |= xmssmt_parse_oid(&params, oid);
        if(ret) {
          printf("Invalid XMSSMT parameter string, exiting.\n");
          return -1;
        }
    }
    else {
        ret  = xmss_str_to_oid(&oid, name);
        ret |= xmss_parse_oid(&params, oid);
        if(ret) {
          printf("Invalid XMSS parameter string, exiting.\n");
          return -1;
        }
    }

    // STRINGS FOR XMSS PROTOCOL
    unsigned char pk[XMSS_OID_LEN + params.pk_bytes];
    unsigned char sk[XMSS_OID_LEN + params.sk_bytes];
    unsigned char *m = malloc(XMSS_MLEN);
    unsigned char *sm = malloc(params.sig_bytes + XMSS_MLEN);
    unsigned char *mout = malloc(params.sig_bytes + XMSS_MLEN);
    unsigned long long smlen;
    unsigned long long mlen;

    // STRINGS FOR USER INTERACTION
    unsigned char command[COMMAND_LEN];
    unsigned char filename[FILESTEM_LEN + 5];
    
    printf("\nCommand >");
    scanf("%3s", command);


    if (!strcmpi(command, "gen")) { // KEY GENERATION

        /** Generating the keys with the xmss(mt)_keypair(...) method */
        printf("sk_bytes=%llu + oid\n", params.sk_bytes);
        if(xmssmt){
            xmssmt_keypair(pk, sk, oid);
        }
        else {
            xmss_keypair(pk, sk, oid);
        }

        /* Saving the generated keys to a file so repeated key generation
           is not necessary */
        unsigned char keystem[FILESTEM_LEN + 10];
        printf("\nFilestem for key storage >");
        scanf("%32s", keystem);
        
        FILE *pub_key = fopen(strcat(keystem, ".pub"), "w+");
        for (int i = 0; i < XMSS_OID_LEN + params.pk_bytes; i++) {
            fputc(pk[i], pub_key);
        }
        fclose(pub_key);

        FILE *prv_key = fopen(strcat(keystem, ".prv"), "w+");
        for (int i = 0; i < XMSS_OID_LEN + params.sk_bytes; i++) {
            fputc(sk[i], prv_key);
        }
        fclose(prv_key);

    } else if (!strcmpi(command, "sgn")) { // SIGNING PROCESS

        printf("Message (less than 32 chars) >");
        scanf("%32s", m);



    } else if (!strcmpi(command, "vrf")) { // VERIFICATION PROCESS

    }

    printf("pk="); hexdump(pk, sizeof pk);
    printf("sk="); hexdump(sk, sizeof sk);
    printf("Testing %d %s signatures.. \n", num_tests, name);

    // for (i = 0; i < num_tests; i++) {
        // printf("  - iteration #%d:\n", i);

        if(xmssmt){
            ret = xmssmt_sign(sk, sm, &smlen, m, XMSS_MLEN);
            if(i >= ((1ULL << params.full_height)-1)) {
                printf("here\n");
                if(ret != -2) {
                    printf("Error detecting running out of OTS keys\n");
                }
                else {
                    printf("Successfully detected running out of OTS keys\n");
                    return 0;
                }
            }
        }
        else {
            ret = xmss_sign(sk, sm, &smlen, m, XMSS_MLEN);
            if(i >= ((1ULL << params.tree_height)-1)) {
                if(ret != -2) {
                    printf("Error detecting running out of OTS keys\n");
                }
                else {
                    printf("Successfully detected running out of OTS keys\n");
                    return 0;
                }
            }
        }

        printf("sm="); hexdump(sm, smlen);

        if (smlen != params.sig_bytes + XMSS_MLEN) {
            printf("  X smlen incorrect [%llu != %u]!\n",
                   smlen, params.sig_bytes);
            ret = -1;
        }
        else {
            printf("    smlen as expected [%llu].\n", smlen);
        }

        /* Test if signature is valid. */

        if(xmssmt){
            ret = xmssmt_sign_open(mout, &mlen, sm, smlen, pk);
        }
        else {
            ret = xmss_sign_open(mout, &mlen, sm, smlen, pk);
        }
        if (ret) {
            printf("  X verification failed!\n");
        }
        else {
            printf("    verification succeeded.\n");
        }

        /* Test if the correct message was recovered. */
        if (mlen != XMSS_MLEN) {
            printf("  X mlen incorrect [%llu != %u]!\n", mlen, XMSS_MLEN);
            ret = -1;
        }
        else {
            printf("    mlen as expected [%llu].\n", mlen);
        }
        if (memcmp(m, mout, XMSS_MLEN)) {
            printf("  X output message incorrect!\n");
            ret = -1;
        }
        else {
            printf("    output message as expected.\n");
        }

        if(ret) return ret;
    // }
    free(m);
    free(sm);
    free(mout);
    return 0;
}

int main()
{
    int rc;
    rc = test_case(XMSS_IMPLEMENTATION, 0, 1<<16);
    if(rc) return rc;
    return 0;
}