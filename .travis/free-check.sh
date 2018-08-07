#!/bin/bash

###
# Checks that "free" is not used unprotected in the main OQS code.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

RET=0

FREE=`find src -name '*.c' | grep -v upstream | grep -v external | xargs grep '[^_]free' | grep -v 'IGNORE free-check'`

if [[ ! -z "${FREE}" ]];
then
	${PRINT_RED}
	echo "'free' is used in the following non-upstream files.  These should be changed to 'OQS_MEM_secure_free' or 'OQS_MEM_insecure_free' as appropriate.";
	${PRINT_RESET}
	echo -n ${FREE} | tr ';' '\n' | sed -e 's/^ //'
	${PRINT_RED}
	echo "If you are sure you want to use 'free' in a particular spot, add the comment"
	echo "    // IGNORE free-check"
	echo "on the line where 'free' occurs."
	${PRINT_RESET}
	RET=1
else
	${PRINT_GREEN}
	echo "No uses of 'free' detected in non-upstream files.";
	${PRINT_RESET}
fi;

exit ${RET}
