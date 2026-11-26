#!/bin/bash

# SPDX-License-Identifier: MIT

if [ "$#" -ne 3 ]; then
   echo "Wrong number of arguments: Expecting path to doxygen binary, path to doxygen file, and destination directory. Exiting." 
   exit 1
fi

mkdir -p "$3/docs"

# index page that groups the algorithm datasheets (docs/algorithms); the
# datasheets themselves are converted by the doxyfy_datasheet.py input filter
python3 scripts/doxygen_algorithms_index.py . "$3/docs/algorithms.md" || exit 1

# Doxygen can't handle Github Markdown so we temporarily reformat it
for file in README.md CONFIGURE.md SECURITY.md
do
	cp $file $file-orig
	python3 scripts/doxyfy.py $file-orig $file
done

# run doxygen:
env DOXYGEN_DESTIONATION_DIR="$3/docs" "$1" "$2"
EXITCODE=$?

# undo the Github Markdown reformatting
for file in README.md CONFIGURE.md SECURITY.md
do
	mv $file-orig $file
done

exit ${EXITCODE}
