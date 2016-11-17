#!/bin/bash

retvalue=0

if [[ $(find -E . -name '*.[ch]' -exec grep -H bzero {} \;) ]]; 
then 
	tput setaf 1;
	echo "Code uses banned functions (bzero).";
	retvalue=1;
fi;

# can add more checks here by copying the above code block

if [[ $retvalue == 0 ]];
then
	tput setaf 2;
	echo "Code does not use banned functions.";
fi;

exit $retvalue;
