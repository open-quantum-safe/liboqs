#!/bin/bash

###
# Checks that all algorithms have an algorithm datasheet in doc/algorithms.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

# get the list of KEMs and signatures from the list of algorithm identifiers src/kem/kem.h and src/sig/sig.h
ALGS=`grep 'define OQS_..._alg_' src/kem/kem.h src/sig/sig.h | grep -v 'default' | sed -e 's/^[^"]*"//' | sed -e 's/".*$//' | tr -d '[:blank:]'`

RET=0
for alg in ${ALGS}; do
	FOUND=`grep ${alg} docs/algorithms/*.md`
	if [[ -z "${FOUND}" ]];
	then
		${PRINT_RED}
		echo "Could not find algorithm datasheet containing '${alg}'."
		${PRINT_RESET}
		RET=1
	fi
done

if [[ "${RET}" == "0" ]];
then
	${PRINT_GREEN}
	echo "Algorithm datasheet present for all algs #defined in src/kem/kem.h and src/sig/sig.h.";
	${PRINT_RESET}
fi

exit ${RET}
