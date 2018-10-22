#!/bin/bash

###
# Checks that all KATs pass.
###

set -e

source $(dirname $0)/defs.sh

RET=0

./tests/kat_kem
scripts/check_kats.sh
error=$?
if [ $error -eq 0 ];
then
	${PRINT_GREEN}
	echo "All known answer tests passed.";
	${PRINT_RESET}
else
	${PRINT_RED}
	echo "Error in known answer tests.";
	${PRINT_RESET}
	RET=1
fi

exit ${RET}
