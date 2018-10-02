#!/bin/bash

###
# Checks that "free" is not used unprotected in the main OQS code.
###

set -e

source $(dirname $0)/defs.sh

RET=0

# We need to temporarily remove bash fail-on-error for the last command, because grep returns with error code 1 when there are no lines found
set +e
FREE=$(find src -name '*.c' | grep -v upstream | xargs grep '[^_]free' | grep "free(" | grep -v 'IGNORE free-check')
ERROR_CODE=$?
set -e

if [ ${ERROR_CODE} -ne 1 ];
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
