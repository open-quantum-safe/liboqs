#!/bin/bash

###
# Checks that all algorithms have an algorithm datasheet in doc/algorithms.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

# get the list of KEMs from the list of algorithm identifiers in enum OQS_KEM_alg_name in src/kem/kem.h
KEMS=`grep '\tOQS_KEM_alg_' src/kem/kem.h | grep -v 'default' | grep -v 'last' | sed -e 's/OQS_KEM_alg_//' | sed -e 's/ =.*//' | tr -d '[:blank:]'`

RET=0
for kem in ${KEMS}; do
	FOUND=`grep ${kem} docs/algorithms/*.md`
	if [[ -z "${FOUND}" ]];
	then
		${PRINT_RED}
		echo "Could not find algorithm datasheet containing '${kem}'."
		${PRINT_RESET}
		RET=1
	fi
done

if [[ "${RET}" == "0" ]];
then
	${PRINT_GREEN}
	echo "Algorithm datasheet present for all KEMs in enum OQS_KEM_alg_name in src/kem/kem.h.";
	${PRINT_RESET}
fi

exit ${RET}
