/********************************************************************************************
* SIDH: an efficient supersingular isogeny-based cryptography library for Diffie-Hellman key 
*       exchange providing 128 bits of quantum security and 192 bits of classical security.
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
*
* Abstract: isogeny-based key exchange
*
*********************************************************************************************/ 

#include "SIDH_internal.h"

extern const unsigned int splits_Alice[MAX_Alice];
extern const unsigned int splits_Bob[MAX_Bob];

#ifdef SIDH_ASM
#include "AMD64/fp_x64.c"
#else
#include "generic/fp_generic.c"
#endif

SIDH_CRYPTO_STATUS SIDH_KeyGeneration_A(unsigned char* pPrivateKeyA, unsigned char* pPublicKeyA, PCurveIsogenyStruct CurveIsogeny, OQS_RAND* rand)
{ // Alice's key-pair generation
  // It produces a private key pPrivateKeyA and computes the public key pPublicKeyA.
  // The private key is an even integer in the range [2, oA-2], where oA = 2^372 (i.e., 372 bits in total). 
  // The public key consists of 3 elements in GF(p751^2), i.e., 564 bytes.
  // CurveIsogeny must be set up in advance using SIDH_curve_initialize().
    unsigned int owords = NBITS_TO_NWORDS(CurveIsogeny->owordbits), pwords = NBITS_TO_NWORDS(CurveIsogeny->pwordbits);
    point_basefield_t P;
    point_proj_t R, phiP = {0}, phiQ = {0}, phiD = {0}, pts[MAX_INT_POINTS_ALICE];
    publickey_t* PublicKeyA = (publickey_t*)pPublicKeyA;
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_ALICE], npts = 0; 
    f2elm_t coeff[5], A = {0}, C = {0}, Aout, Cout;
	SIDH_CRYPTO_STATUS Status;

    if (pPrivateKeyA == NULL || pPublicKeyA == NULL || is_CurveIsogenyStruct_null(CurveIsogeny)) {
        return SIDH_CRYPTO_ERROR_INVALID_PARAMETER;
    }  

    // Choose a random even number in the range [2, oA-2] as secret key for Alice
    Status = SIDH_random_mod_order((digit_t*)pPrivateKeyA, ALICE, CurveIsogeny, rand);    
    if (Status != SIDH_CRYPTO_SUCCESS) {
        SIDH_clear_words((void*)pPrivateKeyA, owords);
        return Status;
    }

    to_mont((digit_t*)CurveIsogeny->PA, (digit_t*)P);                               // Conversion of Alice's generators to Montgomery representation
    to_mont(((digit_t*)CurveIsogeny->PA)+NWORDS_FIELD, ((digit_t*)P)+NWORDS_FIELD); 

    Status = secret_pt(P, (digit_t*)pPrivateKeyA, ALICE, R, CurveIsogeny);
    if (Status != SIDH_CRYPTO_SUCCESS) {
        SIDH_clear_words((void*)pPrivateKeyA, owords);
        return Status;
    }

    copy_words((digit_t*)CurveIsogeny->PB, (digit_t*)phiP, pwords);                 // Copy X-coordinates from Bob's public parameters, set Z <- 1
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)phiP->Z);  
    to_mont((digit_t*)phiP, (digit_t*)phiP);                                        
    copy_words((digit_t*)phiP, (digit_t*)phiQ, pwords);                             // QB = (-XPB:1)
    fpneg751(phiQ->X[0]);   
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)phiQ->Z); 
    distort_and_diff(phiP->X[0], phiD, CurveIsogeny);                               // DB = (x(QB-PB),z(QB-PB))

    fpcopy751(CurveIsogeny->A, A[0]);                                               // Extracting curve parameters A and C
    fpcopy751(CurveIsogeny->C, C[0]);
    to_mont(A[0], A[0]);
    to_mont(C[0], C[0]);

    first_4_isog(phiP, A, Aout, Cout, CurveIsogeny);     
    first_4_isog(phiQ, A, Aout, Cout, CurveIsogeny);
    first_4_isog(phiD, A, Aout, Cout, CurveIsogeny);
    first_4_isog(R, A, A, C, CurveIsogeny);
    
    index = 0;        
    for (row = 1; row < MAX_Alice; row++) {
        while (index < MAX_Alice-row) {
            fp2copy751(R->X, pts[npts]->X);
            fp2copy751(R->Z, pts[npts]->Z);
            pts_index[npts] = index;
            npts += 1;
            m = splits_Alice[MAX_Alice-index-row];
            xDBLe(R, R, A, C, (int)(2*m));
            index += m;
        }
        get_4_isog(R, A, C, coeff);        

        for (i = 0; i < npts; i++) {
            eval_4_isog(pts[i], coeff);
        }
        eval_4_isog(phiP, coeff);
        eval_4_isog(phiQ, coeff);
        eval_4_isog(phiD, coeff);

        fp2copy751(pts[npts-1]->X, R->X); 
        fp2copy751(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }

    get_4_isog(R, A, C, coeff); 
    eval_4_isog(phiP, coeff);
    eval_4_isog(phiQ, coeff);
    eval_4_isog(phiD, coeff);

    inv_3_way(phiP->Z, phiQ->Z, phiD->Z);
    fp2mul751_mont(phiP->X, phiP->Z, phiP->X);
    fp2mul751_mont(phiQ->X, phiQ->Z, phiQ->X);
    fp2mul751_mont(phiD->X, phiD->Z, phiD->X);
                                   
    from_fp2mont(phiP->X, ((f2elm_t*)PublicKeyA)[0]);                               // Converting back to standard representation
    from_fp2mont(phiQ->X, ((f2elm_t*)PublicKeyA)[1]);
    from_fp2mont(phiD->X, ((f2elm_t*)PublicKeyA)[2]);

// Cleanup:
    SIDH_clear_words((void*)R, 2*2*pwords);
    SIDH_clear_words((void*)phiP, 2*2*pwords);
    SIDH_clear_words((void*)phiQ, 2*2*pwords);
    SIDH_clear_words((void*)phiD, 2*2*pwords);
    SIDH_clear_words((void*)pts, MAX_INT_POINTS_ALICE*2*2*pwords);
    SIDH_clear_words((void*)A, 2*pwords);
    SIDH_clear_words((void*)C, 2*pwords);
    SIDH_clear_words((void*)coeff, 5*2*pwords);
      
    return Status;
}


SIDH_CRYPTO_STATUS SIDH_KeyGeneration_B(unsigned char* pPrivateKeyB, unsigned char* pPublicKeyB, PCurveIsogenyStruct CurveIsogeny, OQS_RAND* rand)
{ // Bob's key-pair generation
  // It produces a private key pPrivateKeyB and computes the public key pPublicKeyB.
  // The private key is an integer in the range [1, oB-1], where oA = 3^239 (i.e., 379 bits in total). 
  // The public key consists of 3 elements in GF(p751^2), i.e., 564 bytes.
  // CurveIsogeny must be set up in advance using SIDH_curve_initialize().
    unsigned int owords = NBITS_TO_NWORDS(CurveIsogeny->owordbits), pwords = NBITS_TO_NWORDS(CurveIsogeny->pwordbits);
    point_basefield_t P;
    point_proj_t R, phiP = {0}, phiQ = {0}, phiD = {0}, pts[MAX_INT_POINTS_BOB];
    publickey_t* PublicKeyB = (publickey_t*)pPublicKeyB;
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_BOB], npts = 0; 
    f2elm_t A = {0}, C = {0};
	SIDH_CRYPTO_STATUS Status;

    if (pPrivateKeyB == NULL || pPublicKeyB == NULL || is_CurveIsogenyStruct_null(CurveIsogeny)) {
        return SIDH_CRYPTO_ERROR_INVALID_PARAMETER;
    }  

    // Choose a random number equivalent to 0 (mod 3) in the range [3, oB-3] as secret key for Bob
    Status = SIDH_random_mod_order((digit_t*)pPrivateKeyB, BOB, CurveIsogeny, rand);
    if (Status != SIDH_CRYPTO_SUCCESS) {
        SIDH_clear_words((void*)pPrivateKeyB, owords);
        return Status;
    }

    to_mont((digit_t*)CurveIsogeny->PB, (digit_t*)P);                               // Conversion of Bob's generators to Montgomery representation
    to_mont(((digit_t*)CurveIsogeny->PB)+NWORDS_FIELD, ((digit_t*)P)+NWORDS_FIELD); 

    Status = secret_pt(P, (digit_t*)pPrivateKeyB, BOB, R, CurveIsogeny);
    if (Status != SIDH_CRYPTO_SUCCESS) {
        SIDH_clear_words((void*)pPrivateKeyB, owords);
        return Status;
    }

    copy_words((digit_t*)CurveIsogeny->PA, (digit_t*)phiP, pwords);                 // Copy X-coordinates from Alice's public parameters, set Z <- 1
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)phiP->Z);   
    to_mont((digit_t*)phiP, (digit_t*)phiP);                                        // Conversion to Montgomery representation
    copy_words((digit_t*)phiP, (digit_t*)phiQ, pwords);                             // QA = (-XPA:1)
    fpneg751(phiQ->X[0]); 
    fpcopy751((digit_t*)CurveIsogeny->Montgomery_one, (digit_t*)phiQ->Z);  
    distort_and_diff(phiP->X[0], phiD, CurveIsogeny);                               // DA = (x(QA-PA),z(QA-PA))

    fpcopy751(CurveIsogeny->A, A[0]);                                               // Extracting curve parameters A and C
    fpcopy751(CurveIsogeny->C, C[0]);
    to_mont(A[0], A[0]);
    to_mont(C[0], C[0]);
    
    index = 0;  
    for (row = 1; row < MAX_Bob; row++) {
        while (index < MAX_Bob-row) {
            fp2copy751(R->X, pts[npts]->X);
            fp2copy751(R->Z, pts[npts]->Z);
            pts_index[npts] = index;
            npts += 1;
            m = splits_Bob[MAX_Bob-index-row];
            xTPLe(R, R, A, C, (int)m);
            index += m;
        }
        get_3_isog(R, A, C);        

        for (i = 0; i < npts; i++) {
            eval_3_isog(R, pts[i]);
        }     
        eval_3_isog(R, phiP);
        eval_3_isog(R, phiQ);
        eval_3_isog(R, phiD);

        fp2copy751(pts[npts-1]->X, R->X); 
        fp2copy751(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }
    
    get_3_isog(R, A, C);    
    eval_3_isog(R, phiP);
    eval_3_isog(R, phiQ);
    eval_3_isog(R, phiD);

    inv_3_way(phiP->Z, phiQ->Z, phiD->Z);
    fp2mul751_mont(phiP->X, phiP->Z, phiP->X);
    fp2mul751_mont(phiQ->X, phiQ->Z, phiQ->X);
    fp2mul751_mont(phiD->X, phiD->Z, phiD->X);
                                   
    from_fp2mont(phiP->X, ((f2elm_t*)PublicKeyB)[0]);                               // Converting back to standard representation
    from_fp2mont(phiQ->X, ((f2elm_t*)PublicKeyB)[1]);
    from_fp2mont(phiD->X, ((f2elm_t*)PublicKeyB)[2]);

// Cleanup:
    SIDH_clear_words((void*)R, 2*2*pwords);
    SIDH_clear_words((void*)phiP, 2*2*pwords);
    SIDH_clear_words((void*)phiQ, 2*2*pwords);
    SIDH_clear_words((void*)phiD, 2*2*pwords);
    SIDH_clear_words((void*)pts, MAX_INT_POINTS_BOB*2*2*pwords);
    SIDH_clear_words((void*)A, 2*pwords);
    SIDH_clear_words((void*)C, 2*pwords);
      
    return Status;
}


SIDH_CRYPTO_STATUS SIDH_SecretAgreement_A(unsigned char* pPrivateKeyA, unsigned char* pPublicKeyB, unsigned char* pSharedSecretA, bool validate, PCurveIsogenyStruct CurveIsogeny, OQS_RAND* rand)
{ // Alice's shared secret generation
  // It produces a shared secret key pSharedSecretA using her secret key pPrivateKeyA and Bob's public key pPublicKeyB
  // Inputs: Alice's pPrivateKeyA is an even integer in the range [2, oA-2], where oA = 2^372 (i.e., 372 bits in total). 
  //         Bob's pPublicKeyB consists of 3 elements in GF(p751^2), i.e., 564 bytes.
  //         "validate" flag that indicates if Alice must validate Bob's public key. 
  // Output: a shared secret pSharedSecretA that consists of one element in GF(p751^2), i.e., 1502 bits in total. 
  // CurveIsogeny must be set up in advance using SIDH_curve_initialize().
    unsigned int pwords = NBITS_TO_NWORDS(CurveIsogeny->pwordbits);
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_ALICE], npts = 0; 
    point_proj_t R, pts[MAX_INT_POINTS_ALICE];
    publickey_t* PublicKeyB = (publickey_t*)pPublicKeyB;
    f2elm_t jinv, coeff[5], PKB[3], A, C = {0};
    bool valid_PublicKey = false; 
	SIDH_CRYPTO_STATUS Status;

    if (pPrivateKeyA == NULL || pPublicKeyB == NULL || pSharedSecretA == NULL || is_CurveIsogenyStruct_null(CurveIsogeny)) {
        return SIDH_CRYPTO_ERROR_INVALID_PARAMETER;
    }
      
    to_fp2mont(((f2elm_t*)PublicKeyB)[0], PKB[0]);    // Extracting and converting Bob's public curve parameters to Montgomery representation
    to_fp2mont(((f2elm_t*)PublicKeyB)[1], PKB[1]);        
    to_fp2mont(((f2elm_t*)PublicKeyB)[2], PKB[2]);

    get_A(PKB[0], PKB[1], PKB[2], A, CurveIsogeny);
    fpcopy751(CurveIsogeny->C, C[0]);
    to_mont(C[0], C[0]);

    if (validate == true) {                           // Alice validating Bob's public key
        Status = Validate_PKB(A, &PKB[0], &valid_PublicKey, CurveIsogeny, rand);                   
        if (Status != SIDH_CRYPTO_SUCCESS) {
            return Status;
        }  
        if (valid_PublicKey != true) {
            Status = SIDH_CRYPTO_ERROR_PUBLIC_KEY_VALIDATION;
            return Status;
        }
    }

    Status = ladder_3_pt(PKB[0], PKB[1], PKB[2], (digit_t*)pPrivateKeyA, ALICE, R, A, CurveIsogeny);
    if (Status != SIDH_CRYPTO_SUCCESS) {
        return Status;
    }
    first_4_isog(R, A, A, C, CurveIsogeny); 
        
    index = 0;  
    for (row = 1; row < MAX_Alice; row++) {
        while (index < MAX_Alice-row) {
            fp2copy751(R->X, pts[npts]->X);
            fp2copy751(R->Z, pts[npts]->Z);
            pts_index[npts] = index;
            npts += 1;
            m = splits_Alice[MAX_Alice-index-row];
            xDBLe(R, R, A, C, (int)(2*m));
            index += m;
        }
        get_4_isog(R, A, C, coeff);        

        for (i = 0; i < npts; i++) {
            eval_4_isog(pts[i], coeff);
        }

        fp2copy751(pts[npts-1]->X, R->X); 
        fp2copy751(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }
    
    get_4_isog(R, A, C, coeff); 
    j_inv(A, C, jinv);
    from_fp2mont(jinv, (felm_t*)pSharedSecretA);      // Converting back to standard representation

// Cleanup:
    SIDH_clear_words((void*)R, 2*2*pwords);
    SIDH_clear_words((void*)pts, MAX_INT_POINTS_ALICE*2*2*pwords);
    SIDH_clear_words((void*)A, 2*pwords);
    SIDH_clear_words((void*)C, 2*pwords);
    SIDH_clear_words((void*)jinv, 2*pwords);
    SIDH_clear_words((void*)coeff, 5*2*pwords);
      
    return Status;
}


SIDH_CRYPTO_STATUS SIDH_SecretAgreement_B(unsigned char* pPrivateKeyB, unsigned char* pPublicKeyA, unsigned char* pSharedSecretB, bool validate, PCurveIsogenyStruct CurveIsogeny, OQS_RAND* rand)
{ // Bob's shared secret generation
  // It produces a shared secret key pSharedSecretB using his secret key pPrivateKeyB and Alice's public key pPublicKeyA
  // Inputs: Bob's pPrivateKeyB is an integer in the range [1, oB-1], where oA = 3^239 (i.e., 379 bits in total). 
  //         Alice's pPublicKeyA consists of 3 elements in GF(p751^2), i.e., 564 bytes.
  //         "validate" flag that indicates if Bob must validate Alice's public key. 
  // Output: a shared secret pSharedSecretB that consists of one element in GF(p751^2), i.e., 1502 bits in total. 
  // CurveIsogeny must be set up in advance using SIDH_curve_initialize().
    unsigned int pwords = NBITS_TO_NWORDS(CurveIsogeny->pwordbits);
    unsigned int i, row, m, index = 0, pts_index[MAX_INT_POINTS_BOB], npts = 0; 
    point_proj_t R, pts[MAX_INT_POINTS_BOB];
    publickey_t* PublicKeyA = (publickey_t*)pPublicKeyA;
    f2elm_t jinv, A, PKA[3], C = {0};
    bool valid_PublicKey = false; 
	SIDH_CRYPTO_STATUS Status;

    if (pPrivateKeyB == NULL || pPublicKeyA == NULL || pSharedSecretB == NULL || is_CurveIsogenyStruct_null(CurveIsogeny)) {
        return SIDH_CRYPTO_ERROR_INVALID_PARAMETER;
    }
       
    to_fp2mont(((f2elm_t*)PublicKeyA)[0], PKA[0]);    // Extracting and converting Alice's public curve parameters to Montgomery representation
    to_fp2mont(((f2elm_t*)PublicKeyA)[1], PKA[1]);       
    to_fp2mont(((f2elm_t*)PublicKeyA)[2], PKA[2]);
    
    get_A(PKA[0], PKA[1], PKA[2], A, CurveIsogeny);
    fpcopy751(CurveIsogeny->C, C[0]);
    to_mont(C[0], C[0]);

    if (validate == true) {                           // Bob validating Alice's public key
        Status = Validate_PKA(A, &PKA[0], &valid_PublicKey, CurveIsogeny, rand);                   
        if (Status != SIDH_CRYPTO_SUCCESS) {
            return Status;
        }  
        if (valid_PublicKey != true) {
            Status = SIDH_CRYPTO_ERROR_PUBLIC_KEY_VALIDATION;
            return Status;
        }
    }

    Status = ladder_3_pt(PKA[0], PKA[1], PKA[2], (digit_t*)pPrivateKeyB, BOB, R, A, CurveIsogeny);
    if (Status != SIDH_CRYPTO_SUCCESS) {
        return Status;
    }
    
    index = 0;  
    for (row = 1; row < MAX_Bob; row++) {
        while (index < MAX_Bob-row) {
            fp2copy751(R->X, pts[npts]->X);
            fp2copy751(R->Z, pts[npts]->Z);
            pts_index[npts] = index;
            npts += 1;
            m = splits_Bob[MAX_Bob-index-row];
            xTPLe(R, R, A, C, (int)m);
            index += m;
        }
        get_3_isog(R, A, C);        

        for (i = 0; i < npts; i++) {
            eval_3_isog(R, pts[i]);
        } 

        fp2copy751(pts[npts-1]->X, R->X); 
        fp2copy751(pts[npts-1]->Z, R->Z);
        index = pts_index[npts-1];
        npts -= 1;
    }
    
    get_3_isog(R, A, C);    
    j_inv(A, C, jinv);
    from_fp2mont(jinv, (felm_t*)pSharedSecretB);      // Converting back to standard representation

// Cleanup:
    SIDH_clear_words((void*)R, 2*2*pwords);
    SIDH_clear_words((void*)pts, MAX_INT_POINTS_BOB*2*2*pwords);
    SIDH_clear_words((void*)A, 2*pwords);
    SIDH_clear_words((void*)C, 2*pwords);
    SIDH_clear_words((void*)jinv, 2*pwords);
      
    return Status;
}
