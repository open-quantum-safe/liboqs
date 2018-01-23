#!/bin/bash

if [[ $(nm -g liboqs.a | grep ' T ' | grep -E -v -i ' T [_]?(OQS|ntru|picnic|Keccak|.*SIKEp503|.*SIDHp503|.*SIKEp751|.*SIDHp751|.*shake128|.*shake256|rand_bytes|cpu_supports|uint64_from_char_array|uint64_to_char_array|_aligned_alloc|_aligned_free)') ]];
then
	tput setaf 1;
	echo "Code contains the following non-namespaced global symbols; see https://github.com/open-quantum-safe/liboqs/wiki/Coding-conventions for function naming conventions.";
	tput sgr 0
  nm -g liboqs.a | grep ' T ' | grep -E -v -i ' T [_]?(OQS|ntru|picnic|Keccak|.*SIKEp503|.*SIDHp503|.*SIKEp751|.*SIDHp751|.*shake128|.*shake256|rand_bytes|cpu_supports|uint64_from_char_array|uint64_to_char_array|_aligned_alloc|_aligned_free)'
	exit 1;
else
	tput setaf 2;
	echo "Code adheres to the project standards (global namespace).";
	tput sgr 0
	exit 0;
fi;
