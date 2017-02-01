#!/bin/bash

if [ ! -x "$(which clang-format-3.9)" ]; then 
	# If clang-format is not version -3.9, just use clang-format
	CLANGFORMAT=clang-format make prettyprint
else
	CLANGFORMAT=clang-format-3.9 make prettyprint
fi;

modified=$(git status -s| awk '{print $1}')

for  modi in $modified; do
  if [ $modi = M ];
  then
    tput setaf 1;
	  echo "Code does not adhere to the project standards. Run \"make prettyprint\".";
	  tput sgr 0;
	  exit 1;
  fi
done

tput setaf 2;
echo "Code adheres to the project standards (prettyprint).";
tput sgr 0;
exit 0;

