#!/bin/bash

###
# Checks that all algorithms have an algorithm datasheet in doc/algorithms.
###

set -e

source $(dirname $0)/defs.sh

# get the list of KEMs and signatures from the list of algorithm identifiers src/kem/kem.h and src/sig/sig.h
ALGS=$(grep -E 'define OQS_(KEM|SIG)_alg_' src/kem/kem.h src/sig/sig.h | grep -v 'default' | sed -e 's/^[^"]*"//' | sed -e 's/".*$//' | tr -d '[:blank:]')

RET=0
for alg in ${ALGS}; do
	set +e
	FOUND=$(grep ${alg} docs/algorithms/*.md)
	set -e
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
