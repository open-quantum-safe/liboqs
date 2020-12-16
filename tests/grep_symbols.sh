#!/bin/bash
# SPDX-License-Identifier: MIT

# Purpose of script: Output if a given symbol in symbols file 
# (parameter 1) is found in files in a given directory (parameter 2)

if [ $# -ne 2 ]; then
    echo "Usage: $0 <symbols file> <directory to check>"
    exit 1
fi

if [ ! -d $2 ]; then
    echo "Usage: $0 <symbols file> <directory to check>"
    exit 1
fi

while IFS= read -r line
do
  grep -r "$line" $2 > /dev/null
  if [ $? == 0 ]; then
    echo "found $line in $2"
  fi
done < "$1"
