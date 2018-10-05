#!/bin/bash

###
# Checks that all generated KATs match their upstream values.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"
PRINT_YELLOW="tput setaf 3"

RET=0

ALGS=$(grep -E 'define OQS_(KEM|SIG)_alg_' src/kem/kem.h src/sig/sig.h | grep -v 'default' | grep -v 'sidh' | sed -e 's/^[^"]*"//' | sed -e 's/".*$//' | tr -d '[:blank:]')
for alg in ${ALGS}; do

	kat=$(find kat_*_rsp -name ${alg}.kat |tr '\n' ' ')
	if [ -z "${kat}" ];
	then
		${PRINT_YELLOW}
		echo "KAT file not generated for ${alg}"
		${PRINT_RESET}
		RET=0
		continue
	fi

	origs=$(find src -name ${alg}.kat -o -name ${alg}.*.kat |tr '\n' ' ')
	if [[ "x${origs}x" == "xx" ]];
	then
		${PRINT_RED}
		echo "No original KAT file found for ${alg}"
		${PRINT_RESET}
		RET=1
		continue
	fi

	match=0
	for orig in ${origs}; do
		diff ${orig} ${kat} > /dev/null 2>&1
		error=$?
		if [ $error -eq 0 ]
		then
			echo "KAT values match for ${alg} and ${orig}"
			match=1
			break
		elif [ ! $error -eq 1 ]
		then
			${PRINT_RED}
			echo "An error occurred in the diff command"
			${PRINT_RESET}
			RET=1
		fi
        done

        if [ $match -eq 0 ]
	then
		${PRINT_RED}
		echo "KAT values do not match for ${alg} with any of ${origs}"
		${PRINT_RESET}
		RET=1
	fi
done


if [[ "${RET}" == "0" ]];
then
	${PRINT_GREEN}
	echo "All KAT values matched.";
	${PRINT_RESET}
fi

exit ${RET}
