#!/bin/bash
# SPDX-License-Identifier: MIT

IGNORE_FILE='spdx-ignore.txt'
if [ ! -f "$IGNORE_FILE" ]; then
    echo "Error: $IGNORE_FILE not found. Please create it with a list of files to ignore."
    exit 1
fi

# Strip blank lines
IGNORE_PATTERNS=$(grep -v '^[[:space:]]*$' "$IGNORE_FILE")
IGNORE_PATTERNS=${IGNORE_PATTERNS:-a^}

BD=${OQS_BUILD_DIR:-build}
find . \( -name '*.[chsS]' -or -name '*.cmake' -or -name '*.py' -or -name '*.sh' -or -name 'CMakeLists.txt' \) -and -type f | grep -v -f <(printf '%s\n' "$IGNORE_PATTERNS") | grep -v `basename $BD` | xargs grep -L 'SPDX-License-Identifier' | sort
