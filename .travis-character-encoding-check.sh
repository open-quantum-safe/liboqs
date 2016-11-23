#!/bin/bash

retvalue=0

if [[ $(file --mime `find . -name '*.c' -or -name '*.h' -or -name '*.txt' -or -name '*.md' -print0 | xargs -0` | grep -v charset=us-ascii | grep -v charset=utf-8) ]]; 
then 
	tput setaf 1;
	echo "Code contains text files that do not have either us-ascii or utf-8 character encodings:";
	file --mime `find . -name '*.c' -or -name '*.h' -or -name '*.txt' -or -name '*.md' -print0 | xargs -0` | grep -v charset=us-ascii | grep -v charset=utf-8
	retvalue=1;
fi;

# can add more checks here by copying the above code block

if [[ $retvalue == 0 ]];
then
	tput setaf 2;
	echo "Code does not use banned character encodings.";
fi;

exit $retvalue;
