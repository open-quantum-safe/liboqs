#!/bin/sh

# SPDX-License-Identifier: MIT

if [ "$#" -ne 2 ]; then
   echo "Wrong number of arguments: Expecting path to doxygen binary and doxygen file. Exiting." 
   exit 1
fi

cp README.md README.md-orig
python3 scripts/doxyfy.py README.md-orig README.md

# run doxygen:
$1 $2

# remove helper README
mv README.md-orig README.md
