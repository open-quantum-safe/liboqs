#!/bin/bash

set -o xtrace

r=`git log --name-only --since="1 day ago" -n 2`
if [ "x$r" == "x" ]; then 
	echo "No checkin in the last day. No build/test required. Exiting."
       	exit 0
fi

CONFIGURE_ARGS=--without-openssl
# New stuff found: Testing _is_ needed:
brew update; brew unlink python@2; brew install autoconf automake libtool wget doxygen graphviz clang-format; pip3 install pytest
autoreconf -i && ./configure ${CONFIGURE_ARGS}
if [ $? -ne 0 ]; then
	echo "Configure failed. Exiting."
	exit -1
fi
make && make check
if [ $? -ne 0 ]; then
	echo "Make failed. Exiting."
	exit -1
fi
sysctl -a | grep machdep.cpu; cat src/kem/sike/Makefile; ls -l tests; ./tests/example_kem
# Test result becomes exit code for whole build
export SKIP_TESTS=style
mkdir -p test-results/pytest && python3 -m pytest --verbose --junitxml=test-results/pytest/results.xml
