/********************************************************************************************
* Abstract: run tests against known answer test vectors for FrodoKEM-976 using AES128
*
* Modified from a file created by Bassham, Lawrence E (Fed) on 8/29/17.
* Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
*********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "../src/api_frodo976.h"

char    AlgName[] = "FrodoKEM-976-AES";


#define	MAX_MARKER_LEN		50
#define KAT_SUCCESS          0
#define KAT_FILE_OPEN_ERROR -1
#define KAT_VERIFICATION_ERROR -2
#define KAT_DATA_ERROR      -3
#define KAT_CRYPTO_FAILURE  -4

int		FindMarker(FILE *infile, const char *marker);
int		ReadHex(FILE *infile, unsigned char *A, int Length, char *str);
void	fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L);

int
main()
{
    char                fn_rsp[32];
    FILE                *fp_rsp;
    unsigned char       seed[48];
    unsigned char       ct[CRYPTO_CIPHERTEXTBYTES], ss[CRYPTO_BYTES], ss1[CRYPTO_BYTES], ct_rsp[CRYPTO_CIPHERTEXTBYTES], ss_rsp[CRYPTO_BYTES];
    int                 count;
    int                 done;
    unsigned char       pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES], pk_rsp[CRYPTO_PUBLICKEYBYTES], sk_rsp[CRYPTO_SECRETKEYBYTES];
    int                 ret_val;
    
    sprintf(fn_rsp, "KAT/PQCkemKAT_%d.rsp", CRYPTO_SECRETKEYBYTES);
    if ( (fp_rsp = fopen(fn_rsp, "r")) == NULL ) {
        printf("Couldn't open <%s> for read\n", fn_rsp);
        return KAT_FILE_OPEN_ERROR;
    }
    
    printf("# %s\n\n", AlgName);
    done = 0;
    do {
        if ( FindMarker(fp_rsp, "count = ") ) {
            if (fscanf(fp_rsp, "%d", &count) != 1) {
                done = 1;
                break;
            }
        } else {
            done = 1;
            break;
        }
        
        if ( !ReadHex(fp_rsp, seed, 48, "seed = ") ) {
            printf("ERROR: unable to read 'seed' from <%s>\n", fn_rsp);
            return KAT_DATA_ERROR;
        }
        
        randombytes_init(seed, NULL, 256);
        
        // Generate the public/private keypair
        if ( (ret_val = crypto_kem_keypair_Frodo976(pk, sk)) != 0) {
            printf("crypto_kem_keypair returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }
        if ( !ReadHex(fp_rsp, pk_rsp, CRYPTO_PUBLICKEYBYTES, "pk = ") ) {
            printf("ERROR: unable to read 'pk' from <%s>\n", fn_rsp);
            return KAT_DATA_ERROR;
        }
        if ( !ReadHex(fp_rsp, sk_rsp, CRYPTO_SECRETKEYBYTES, "sk = ") ) {
            printf("ERROR: unable to read 'sk' from <%s>\n", fn_rsp);
            return KAT_DATA_ERROR;
        }

        if(memcmp(pk,pk_rsp,CRYPTO_PUBLICKEYBYTES)!=0){
        printf("ERROR: pk is different from <%s>\n", fn_rsp);
        return KAT_VERIFICATION_ERROR;
        }
        if(memcmp(sk,sk_rsp,CRYPTO_SECRETKEYBYTES)!=0){
        printf("ERROR: sk is different from <%s>\n", fn_rsp);
        return KAT_VERIFICATION_ERROR;
        }

        if ( (ret_val = crypto_kem_enc_Frodo976(ct, ss, pk)) != 0) {
            printf("crypto_kem_enc returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }
    
        if ( !ReadHex(fp_rsp, ct_rsp, CRYPTO_CIPHERTEXTBYTES, "ct = ") ) {
            printf("ERROR: unable to read 'pk' from <%s>\n", fn_rsp);
            return KAT_DATA_ERROR;
        }
        if ( !ReadHex(fp_rsp, ss_rsp, CRYPTO_BYTES, "ss = ") ) {
            printf("ERROR: unable to read 'sk' from <%s>\n", fn_rsp);
            return KAT_DATA_ERROR;
        }

        if(memcmp(ct,ct_rsp,CRYPTO_CIPHERTEXTBYTES)!=0){
            printf("ERROR: ct is different from <%s>\n", fn_rsp);
            return KAT_VERIFICATION_ERROR;
        }
        if(memcmp(ss,ss_rsp,CRYPTO_BYTES)!=0){
            printf("ERROR: ss is different from <%s>\n", fn_rsp);
            return KAT_VERIFICATION_ERROR;
        }
        
        if ( (ret_val = crypto_kem_dec_Frodo976(ss1, ct, sk)) != 0) {
            printf("crypto_kem_dec returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }
        
        if ( memcmp(ss, ss1, CRYPTO_BYTES) ) {
            printf("crypto_kem_dec returned bad 'ss' value\n");
            return KAT_CRYPTO_FAILURE;
        }

    } while ( !done );
    
    fclose(fp_rsp);
    printf("Known Answer Tests PASSED. \n");
    printf("\n\n");

    return KAT_SUCCESS;
}


//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//

int
FindMarker(FILE *infile, const char *marker)
{
    char    line[MAX_MARKER_LEN];
    int     i, len;
    int     curr_line;

    len = (int)strlen(marker);
    if ( len > MAX_MARKER_LEN-1 )
        len = MAX_MARKER_LEN-1;

    for ( i=0; i<len; i++ )
      {
        curr_line = fgetc(infile);
        line[i] = curr_line;
        if (curr_line == EOF )
          return 0;
      }
    line[len] = '\0';

    while ( 1 ) {
        if ( !strncmp(line, marker, len) )
            return 1;

        for ( i=0; i<len-1; i++ )
            line[i] = line[i+1];
        curr_line = fgetc(infile);
        line[len-1] = curr_line;
        if (curr_line == EOF )
            return 0;
        line[len] = '\0';
    }

    // shouldn't get here
    return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
ReadHex(FILE *infile, unsigned char *A, int Length, char *str)
{
    int              i, ch, started;
    unsigned char    ich;

    if ( Length == 0 ) {
        A[0] = 0x00;
        return 1;
    }
    memset(A, 0x00, Length);
    started = 0;
    if ( FindMarker(infile, str) )
        while ( (ch = fgetc(infile)) != EOF ) {
            if ( !isxdigit(ch) ) {
                if ( !started ) {
                    if ( ch == '\n' )
                        break;
                    else
                        continue;
                }
                else
                    break;
            }
            started = 1;
            if ( (ch >= '0') && (ch <= '9') )
                ich = ch - '0';
            else if ( (ch >= 'A') && (ch <= 'F') )
                ich = ch - 'A' + 10;
            else if ( (ch >= 'a') && (ch <= 'f') )
                ich = ch - 'a' + 10;
            else // shouldn't ever get here
                ich = 0;
            
            for ( i=0; i<Length-1; i++ )
                A[i] = (A[i] << 4) | (A[i+1] >> 4);
            A[Length-1] = (A[Length-1] << 4) | ich;
        }
    else
        return 0;

    return 1;
}

void
fprintBstr(FILE *fp, char *S, unsigned char *A, unsigned long long L)
{
    unsigned long long  i;

    fprintf(fp, "%s", S);

    for ( i=0; i<L; i++ )
        fprintf(fp, "%02X", A[i]);

    if ( L == 0 )
        fprintf(fp, "00");

    fprintf(fp, "\n");
}
