#!/bin/bash

source $(dirname $0)/defs.sh

REGEX=' T [_]?(OQS|ntru|picnic|Keccak|.*SIKEp503|.*SIDHp503|.*SIKEp751|.*SIDHp751|.*shake128|.*shake256|rand_bytes|cpu_supports|uint64_from_char_array|uint64_to_char_array|print_hex|ntt_double|rec|aligned_alloc|aligned_free)'

if [[ $(nm -g liboqs.a | grep ' T ' | grep -E -v -i "$REGEX") ]];
then
    ${PRINT_RED}
    echo "Code contains the following non-namespaced global symbols; see https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for function naming conventions.";
    ${PRINT_RESET}
    nm -g liboqs.a | grep ' T ' | grep -E -v -i "$REGEX"
    exit 1;
else
    ${PRINT_GREEN}
    echo "Code adheres to the project standards (global namespace).";
    ${PRINT_RESET}
    exit 0;
fi;

        
