#!/bin/bash

set -e

source $(dirname $0)/defs.sh

REGEX=' T [_]?(OQS|picnic|Keccak|.*SIKEp503|.*SIDHp503|.*SIKEp751|.*SIDHp751|.*shake128|.*shake256|rand_bytes|cpu_supports|uint64_from_char_array|uint64_to_char_array|print_hex|ntt_double|rec|aligned_alloc|aligned_free)'

# For catching errors, we have the first (otherwise superfluous) two lines below two make sure that the commands are being executed correctly
NON_NAMESPACED=`nm -g .libs/liboqs.a`
NON_NAMESPACED=`nm -g .libs/liboqs.a | grep ' T '`
# But we need to temporarily remove bash fail-on-error for the last command, because grep returns with error code 1 when there are no lines found
set +e
NON_NAMESPACED=`nm -g .libs/liboqs.a | grep ' T ' | grep -E -v -i "$REGEX"`
ERROR_CODE=$?
set -e

if [ ${ERROR_CODE} -ne 1 ];
then
    ${PRINT_RED}
    echo "Code contains the following non-namespaced global symbols; see https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for function naming conventions.";
    ${PRINT_RESET}
    echo ${NON_NAMESPACED}
    exit 1;
else
    ${PRINT_GREEN}
    echo "Code adheres to the project standards (global namespace).";
    ${PRINT_RESET}
    exit 0;
fi;
