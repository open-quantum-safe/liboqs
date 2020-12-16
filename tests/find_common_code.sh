#!/bin/bash

# Purpose of this script is to find common code dependencies of all algorithms
# as per https://github.com/open-quantum-safe/liboqs/issues/849

# Chosen approach:
# step 1: Determine all symbols exposed by common code and store them in COMMON_CODE_FILE:
COMMON_CODE_FILE=common_code.txt

if [ ! -d build/src/common ]; then
   echo "Precondition for script: liboqs must have been built. Exiting."
   exit 1
fi

find build/src/common -name "*.o" -exec nm {} \; | grep " T " | awk '{ print $3 }' > ${COMMON_CODE_FILE} 

# step 2 (conservative, possibly prune more than only OQS_API) also collect all #defines in common source code:
find src/common -name "*.h" -exec grep \#define  {} \; | awk '{print $2}' | sed 's/(.*$//' | grep -v OQS_API  >> ${COMMON_CODE_FILE}

# step 3: Determine which of these symbols are referenced by which QSC algorithm code

find src/kem -maxdepth 1 -mindepth 1 -type d -exec tests/grep_symbols.sh ${COMMON_CODE_FILE} {} \; 
find src/sig -maxdepth 1 -mindepth 1 -type d -exec tests/grep_symbols.sh ${COMMON_CODE_FILE} {} \; 

# clean up
rm ${COMMON_CODE_FILE}
