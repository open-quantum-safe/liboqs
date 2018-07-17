#!/bin/bash

###
# Checks that all KATs pass.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

RET=0

./kat_kem
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
