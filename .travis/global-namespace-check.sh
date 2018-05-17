#!/bin/bash

###
# Checks that all global symbols in liboqs.a are namespaced.
###

PRINT_GREEN="tput setaf 2"
PRINT_RED="tput setaf 1"
PRINT_RESET="tput sgr 0"

REGEX=' [_]?(OQS|randombytes|cSHAKE|LakeKeyak|KMAC|Kangaroo|Keccak|KetJr|KetMj|KetMn|KetSr|Ketje|Keyak|Kra|LunarKeyak|OceanKeyak|ParallelHash|RiverKeyak|SHA3|SHAKE|SeaKeyak|TupleHash|Vatte)'
LIBOQS=liboqs.a

# try to find liboqs.a
if [ ! -f ${LIBOQS} ]
then
    LIBOQS=../liboqs.a
fi
if [ ! -f ${LIBOQS} ]
then
    ${PRINT_RED}
    echo "Could not find liboqs.a"
    ${PRINT_RESET}
    exit 1;
fi

# check for globally namespaced functions using ' T '
if [[ $(nm -g ${LIBOQS} | grep ' T ' | grep -E -v "${REGEX}") ]];
then
    ${PRINT_RED}
    echo "Code contains the following non-namespaced global functions; see https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for naming conventions.";
    ${PRINT_RESET}
    nm -g ${LIBOQS} | grep ' T ' | grep -E -v "${REGEX}"
    exit 1;
else
    ${PRINT_GREEN}
    echo "Code contains no non-namespaced global functions.";
    ${PRINT_RESET}
fi;

# check for globally namespaced variables using ' T '
if [[ $(nm -g ${LIBOQS} | grep ' D ' | grep -E -v "${REGEX}") ]];
then
    ${PRINT_RED}
    echo "Code contains the following non-namespaced global variables; see https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for naming conventions.";
    ${PRINT_RESET}
    nm -g ${LIBOQS} | grep ' D ' | grep -E -v "${REGEX}"
    exit 1;
else
    ${PRINT_GREEN}
    echo "Code contains no non-namespaced global variables.";
    ${PRINT_RESET}
fi;

exit 0
