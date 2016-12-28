#!/bin/bash
#
# Install astyle on Linux
# 

get_astyle_path() {
	if [ ! -x "$(which astyle)" ]; then 
		# Download astyle
		wget http://downloads.sourceforge.net/project/astyle/astyle/astyle%202.05.1/astyle_2.05.1_linux.tar.gz
		tar xzf astyle_2.05.1_linux.tar.gz
		cd astyle/build/gcc
		make
		# return astyle bin directory
		return $(pwd)/bin
	fi;
}
