#!/bin/bash
# SPDX-License-Identifier: MIT

find . \( -name '*.[chsS]' -or -name '*.cmake' -or -name '*.py' -or -name '*.sh' -or -name 'CMakeLists.txt' \) -and -type f | grep -v '/external/' | grep -v 'kem/.*/.*/.*' | grep -v 'sig/.*/.*/.*' | grep -v 'build' | grep -v 'xkcp_low' | grep -v 'patches' | xargs grep -L 'SPDX-License-Identifier' | sort
