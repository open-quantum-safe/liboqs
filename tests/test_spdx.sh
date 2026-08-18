#!/bin/bash
# SPDX-License-Identifier: MIT

BD=${OQS_BUILD_DIR:-build}
find . \( -name '*.[chsS]' -or -name '*.cmake' -or -name '*.py' -or -name '*.sh' -or -name 'CMakeLists.txt' \) -and -type f | grep -v '/external/' | grep -v 'kem/.*/.*/.*' | grep -v 'sig/.*/.*/.*' | grep -v 'xkcp_low' | grep -v 'patches' | grep -v 'scripts/copy_from_upstream/repos' | grep -v `basename $BD` | xargs grep -L 'SPDX-License-Identifier' | sort
