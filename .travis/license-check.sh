#!/bin/bash

###
# Checks that all upstream implementations include a LICENSE or LICENSE.txt file.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

RET=0

for d in $(find src/kem -name upstream); do
    if [[ ! -f ${d}/LICENSE ]]; then
        if [[ ! -f ${d}/LICENSE.txt ]]; then
            ${PRINT_RED}
            echo "No LICENSE or LICENSE.txt file in ${d}."
            RET=1
            ${PRINT_RESET}
        fi
    fi
done

if [[ ${RET} == 0 ]]; then
    ${PRINT_GREEN}
    echo "All LICENSE or LICENSE.txt files present.";
    ${PRINT_RESET}
fi

exit ${RET}
