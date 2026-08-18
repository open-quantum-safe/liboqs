#!/bin/bash
# SPDX-License-Identifier: MIT

rv=0

if [ "$1" = "--no-dry-run" ]; then
	dryrun=""
else
	dryrun="--dry-run"
fi

# check style of non-external code:
find src tests -name '*.[ch]' | grep -v '/external/' | grep -v 'kem/.*/.*/.*' | grep -v 'sig/.*/.*/.*' | xargs astyle $dryrun --options=.astylerc | grep Format
if [ $? -ne 1 ]; then
   echo "Error: Some files need reformatting. Check output above."
   rv=-1
fi

# check _all_ source files for CRLF line endings (except in repos directory):
find . \( -type d -name repos -prune \) -o -name '*.[chS]' -exec file "{}" ";" | grep CRLF
if [ $? -ne 1 ]; then
   echo "Error: Files found with non-UNIX line endings."
   echo "To fix, consider running \"find src tests -name '*.[chS]' | xargs sed -i 's/\r//' \"."
   rv=-1
fi

exit $rv
