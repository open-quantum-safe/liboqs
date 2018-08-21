#!/bin/bash

###
# Checks that all algorithms have an algorithm datasheet in doc/algorithms.
###

source $(dirname $0)/defs.sh

# get the list of KEMs from the list of algorithm identifiers in enum OQS_KEM_alg_name in src/kem/kem.h
KEMS=`grep 'define OQS_KEM_alg_' src/kem/kem.h | grep -v 'default' | sed -e 's/^[^"]*"//' | sed -e 's/".*$//' | tr -d '[:blank:]'`

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
	echo "Algorithm datasheet present for all KEMs #defined in src/kem/kem.h.";
	${PRINT_RESET}
fi

exit ${RET}
