#!/bin/bash

###
# Checks that all generated KATs match their upstream values.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

RET=0

KEMS=`grep 'define OQS_KEM_alg_' src/kem/kem.h | grep -v 'default' | sed -e 's/^[^"]*"//' | sed -e 's/".*$//' | tr -d '[:blank:]'`

for kem in ${KEMS}; do

	kat="kat_kem_rsp/${kem}.kat"
	if [[ ! -e ${kat} ]];
	then
		${PRINT_RED}
		echo "KAT file not generated for ${kem}"
		${PRINT_RESET}
		RET=1
		continue
	fi

	origs=`find src -name ${kem}*.kat |tr '\n' ' '`
	if [[ "x${origs}x" == "xx" ]];
	then
		${PRINT_RED}
		echo "No original KAT file found for ${kem}"
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
			echo "KAT values match for ${kem} and ${orig}"
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
		echo "KAT values do not match for ${kem} with any of ${origs}"
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
