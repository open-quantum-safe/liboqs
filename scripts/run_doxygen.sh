#!/bin/bash

# SPDX-License-Identifier: MIT

if [ "$#" -ne 2 ]; then
   echo "Wrong number of arguments: Expecting path to doxygen binary and doxygen file. Exiting." 
   exit 1
fi

# Doxygen can't handle Github Markdown so we temporarily reformat it
cp README.md README.md-orig
python3 scripts/doxyfy.py README.md-orig README.md
cp CONFIGURE.md CONFIGURE.md-orig
python3 scripts/doxyfy.py CONFIGURE.md-orig CONFIGURE.md

# run doxygen:
$1 $2
EXITCODE=$?

# undo the Github Markdown reformatting
mv README.md-orig README.md
mv CONFIGURE.md-orig CONFIGURE.md

exit ${EXITCODE}