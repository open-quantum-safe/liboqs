#!/bin/bash

# SPDX-License-Identifier: MIT

if [ "$#" -ne 3 ]; then
   echo "Wrong number of arguments: Expecting path to doxygen binary, path to doxygen file, and destination directory. Exiting." 
   exit 1
fi

# Doxygen can't handle Github Markdown so we temporarily reformat it
cp README.md README.md-orig
python3 scripts/doxyfy.py README.md-orig README.md
cp CONFIGURE.md CONFIGURE.md-orig
python3 scripts/doxyfy.py CONFIGURE.md-orig CONFIGURE.md

# run doxygen:
mkdir -p "$3/docs"
env DOXYGEN_DESTIONATION_DIR="$3/docs" "$1" "$2"
EXITCODE=$?

# undo the Github Markdown reformatting
mv README.md-orig README.md
mv CONFIGURE.md-orig CONFIGURE.md

exit ${EXITCODE}