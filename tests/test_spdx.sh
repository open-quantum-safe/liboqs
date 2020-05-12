#!/bin/bash
# SPDX-License-Identifier: MIT

find . \( -name '*.[chsS]' -or -name '*.cmake' -or -name '*.py' -or -name '*.sh' -or -name 'CMakeLists.txt' \) -and -type f | grep -v '/external/' | grep -v 'kem.*/pqclean_' | grep -v 'sig.*/pqclean_' | grep -v 'build' | xargs grep -L 'SPDX-License-Identifier' | sort
