#!/bin/bash
# SPDX-License-Identifier: MIT

# This script fetches the NIST vectors for the ML-KEM-ipd and ML-DSA-ipd and extracts the values we use for testing

wget -O PQC_Intermediate_Values.zip https://csrc.nist.gov/csrc/media/Projects/post-quantum-cryptography/documents/example-files/PQC%20Intermediate%20Values.zip
unzip PQC_Intermediate_Values.zip && mv "PQC Intermediate Values" PQC_Intermediate_Values

file_keygen="PQC_Intermediate_Values/Key Generation -- "
file_signature="PQC_Intermediate_Values/Signature Generation -- "
file_verification="PQC_Intermediate_Values/Signature Verification -- "

for VARIANT in "ML-DSA-44" "ML-DSA-65" "ML-DSA-87"
do
  echo "Origin of the values: https://csrc.nist.gov/csrc/media/Projects/post-quantum-cryptography/documents/example-files/PQC%20Intermediate%20Values.zip" >> "$VARIANT.txt"
  grep "seed: " "$file_keygen$VARIANT.txt" >> "$VARIANT.txt" 
  grep "message: " "$file_signature$VARIANT.txt" | sed 's/message: /sig_message: /g' >> "$VARIANT.txt"
  grep "sk: " "$file_signature$VARIANT.txt" | sed 's/sk: /sig_sk: /g' >> "$VARIANT.txt"
  grep "rnd: " "$file_signature$VARIANT.txt" >> "$VARIANT.txt"

  grep "signature: " "$file_verification$VARIANT.txt" | sed "s/signature: /verif_signature: /g" >> "$VARIANT.txt"
  grep "pk: " "$file_verification$VARIANT.txt" | sed "s/pk: /verif_pk: /g" >> "$VARIANT.txt"
  grep "message: " "$file_verification$VARIANT.txt" | sed "s/message: /verif_message: /g" >> "$VARIANT.txt"

  grep "pk: " "$file_keygen$VARIANT.txt" | sed "s/pk: /keygen_pk: /g" >> "$VARIANT.txt"
  grep "sk: " "$file_keygen$VARIANT.txt" | sed 's/sk: /keygen_sk: /g' >> "$VARIANT.txt"
  grep "signature: " "$file_signature$VARIANT.txt" | sed "s/signature: /sig_signature: /g" >> "$VARIANT.txt"
done

file_keygen="PQC_Intermediate_Values/Key Generation -- "
file_encaps="PQC_Intermediate_Values/Encapsulation -- "
file_decaps="PQC_Intermediate_Values/Decapsulation -- "

for VARIANT in "ML-KEM-512" "ML-KEM-768" "ML-KEM-1024"
do
  echo "Origin of the values: https://csrc.nist.gov/csrc/media/Projects/post-quantum-cryptography/documents/example-files/PQC%20Intermediate%20Values.zip" >> "$VARIANT.txt"
  grep "z: " "$file_keygen$VARIANT.txt" | sed 's/z: /keygen_z: /g' >> "$VARIANT.txt"
  grep "d: " "$file_keygen$VARIANT.txt" | sed 's/d: /keygen_d: /g' >> "$VARIANT.txt"
  
  grep "m: " "$file_encaps$VARIANT.txt" | sed 's/m: /encaps_m: /g' >> "$VARIANT.txt"
  grep "ek: " "$file_encaps$VARIANT.txt" | sed 's/ek: /encaps_ek: /g' >> "$VARIANT.txt"
  grep "K: " "$file_encaps$VARIANT.txt" | sed 's/K: /encaps_K: /g' >> "$VARIANT.txt"
  
  grep "dk: " "$file_decaps$VARIANT.txt" | sed 's/dk: /decaps_dk: /g' >> "$VARIANT.txt"
  grep "c: " "$file_decaps$VARIANT.txt" | sed 's/c: /decaps_c: /g' >> "$VARIANT.txt"
  grep "KPrime: " "$file_decaps$VARIANT.txt" | sed 's/KPrime: /decaps_KPrime: /g' >> "$VARIANT.txt"

  grep "ek: " "$file_keygen$VARIANT.txt" | sed 's/ek: /keygen_ek: /g' >> "$VARIANT.txt"
  grep "dk: " "$file_keygen$VARIANT.txt" | sed 's/dk: /keygen_dk: /g' >> "$VARIANT.txt"
  
  grep "c: " "$file_encaps$VARIANT.txt" | sed 's/c: /encaps_c: /g' >> "$VARIANT.txt"

done