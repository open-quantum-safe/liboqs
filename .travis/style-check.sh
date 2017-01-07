#!/bin/bash

CLANG_FORMAT=clang-format-3.9

if [ ! -x "$(which clang-format-3.9)" ]; then 
	# If clang-format is not version -3.9, just use clang-format
	CLANG_FORMAT=clang-format
fi;

CLANGFORMAT=$(CLANG_FORMAT) make prettyprint
modified=$(git status -s)

if [ "$modified" ]; then
	tput setaf 1;
	echo "Code does not adhere to the project standards. Run \"make prettyprint\".";
	tput sgr 0;
	exit 1;
else
	tput setaf 2;
	echo "Code adheres to the project standards (prettyprint).";
	tput sgr 0;
	exit 0;
fi;
