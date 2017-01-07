#!/bin/bash

# Verify that clang-format is installed
if [ ! -x "$(which clang-format-3.9)" ]; then 
	tput setaf 1;
	echo "clang-format is not installed.";
	tput sgr 0;
	exit 1;
fi;

CLANGFORMAT=clang-format-3.9 make prettyprint
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
