#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../xmss.h"
#include "../params.h"
#include "../randombytes.h"

#define XMSS_MLEN 32
#define XMSS_IMPLEMENTATION "XMSS-SHA2_20_256"

/** The various implementations available to you are :
 *      * "XMSS-SHA2_10_256"
 *      * "XMSS-SHA2_16_256"
 *      * "XMSS-SHA2_20_256"
 *      * "XMSS-SHA2_10_512"
 *      * "XMSS-SHA2_16_512"
 *      * "XMSS-SHA2_20_512"
 */

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
    unsigned char keystem[FILESTEM_LEN + 10];
    

    while (1) {
        printf("\nCommand >");
        scanf("%3s", command);



        if (!strcmp(command, "gen")) { // KEY GENERATION

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
            printf("\nFilestem for key storage >");
            scanf("%32s", keystem);
            
            FILE *pub_key = fopen(strcat(keystem, ".pub"), "w+");
            for (unsigned int i = 0; i < XMSS_OID_LEN + params.pk_bytes; i++) {
                fputc(pk[i], pub_key);
            }
            fclose(pub_key);

            FILE *prv_key = fopen(strcat(keystem, ".prv"), "w+");
            for (unsigned int i = 0; i < XMSS_OID_LEN + params.sk_bytes; i++) {
                fputc(sk[i], prv_key);
            }
            fclose(prv_key);

        } else if (!strcmp(command, "sgn")) { // SIGNING PROCESS

            printf("Message >");
            scanf("%32s", m);

            /* Get the bytes of the key from  the .pub and .prv files
            respectively */
            printf("\nFilestem where keys are stored>");
            scanf("%32s", keystem);
            FILE *pub_key = fopen(strcat(keystem, ".pub"), "r");
            for (unsigned int i = 0; i < XMSS_OID_LEN + params.pk_bytes; i++) {
                pk[i] = fgetc(pub_key);
            }
            fclose(pub_key);

            FILE *prv_key = fopen(strcat(keystem, ".prv"), "r");
            for (unsigned int i = 0; i < XMSS_OID_LEN + params.sk_bytes; i++) {
                sk[i] = fgetc(prv_key);
            }
            fclose(prv_key);

            printf("pk="); hexdump(pk, sizeof pk);
            printf("sk="); hexdump(sk, sizeof sk);
            printf("Testing %d %s signatures.. \n", num_tests, name);

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
            } else {
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
            
            /* Write the signature generated to the .sig file that the user names */ 
            printf("Filestem where the signature is stored (useful to add the keystem as part of the name)\n>");
            scanf("%32s", filename);
            FILE *sig_file = fopen(strcat(filename, ".sig"), "w+");
            for (unsigned long long i = 0; i < smlen; i++) {
                fputc(sm[i], sig_file);
            }     
            printf("Signature written to file %s successfully", filename);
            fclose(sig_file);

        } else if (!strcmp(command, "vrf")) { // VERIFICATION PROCESS

            printf("Filestem where the signature is specified >");
            scanf("%32s", filename);
            FILE *sig_file;
            sig_file = fopen(strcat(filename, ".sig"), "rb");
            fseek(sig_file, 0, SEEK_END);
            smlen = ftell(sig_file);
            fclose(sig_file);

            sig_file = fopen(strcat(filename, ".sig"), "rb");
            printf("Bytes in the file: %i\n", smlen);
            for (unsigned long long  i = 0; i < smlen; i++) {
                sm[i] = fgetc(sig_file);
            }
            printf("sm="); hexdump(sm, sizeof sm);
            printf("%s", filename);
            fclose(sig_file);

            /* Get the bytes of the key from  the .pub and .prv files
            respectively */
            printf("\nFilestem where keys are stored>");
            scanf("%32s", keystem);
            FILE *pub_key = fopen(strcat(keystem, ".pub"), "rb");
            for (unsigned int i = 0; i < XMSS_OID_LEN + params.pk_bytes; i++) {
                pk[i] = fgetc(pub_key);
            }
            printf("pk="); hexdump(pk, sizeof pk);
            fclose(pub_key);

            FILE *prv_key = fopen(strcat(keystem, ".prv"), "rb");
            for (unsigned int i = 0; i < XMSS_OID_LEN + params.sk_bytes; i++) {
                sk[i] = fgetc(prv_key);
            }
            printf("sk="); hexdump(sk, sizeof sk);
            fclose(prv_key);

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
        }
    }
    
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