#!/bin/bash
# SPDX-License-Identifier: MIT

SOURCE_DIR="${BASH_SOURCE%/*}"
IGNORE_FILE="$SOURCE_DIR/.spdxignore"

BD=${OQS_BUILD_DIR:-build}
find . \( -name '*.[chsS]' -or -name '*.cmake' -or -name '*.py' -or -name '*.sh' -or -name 'CMakeLists.txt' \) -and -type f \
    | grep -v -f $IGNORE_FILE \
    | grep -v `basename $BD` \
    | xargs grep -L 'SPDX-License-Identifier' \
    | sort
