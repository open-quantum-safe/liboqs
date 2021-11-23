#!/bin/bash
# SPDX-License-Identifier: MIT

# check style of non-external code:
find src tests -name '*.[ch]' | grep -v '/external/' | grep -v 'kem/.*/.*/.*' | grep -v 'sig/.*/.*/.*' | xargs astyle --dry-run --options=.astylerc | grep Format
if [ $? -ne 1 ]; then
   echo "Error: Some files need reformatting. Check output above. Exiting."
   exit -1
fi

# check _all_ source files for CRLF line endings:
find . -name '*.[chS]' -exec file "{}" ";" | grep CRLF
if [ $? -ne 1 ]; then
   echo "Error: Files found with non-UNIX line endings. Exiting."
   echo "To fix, consider running \"find src tests -name '*.[chS]' | xargs sed -i 's/\r//' \"."
   exit -1
fi
