#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Detect which slow, algorithm-specific CI work a change requires.
#
# Compares HEAD against origin/BASE_REF (default: main) and prints one
# KEY=value line per setting that applies, ready to append to $GITHUB_ENV:
#
#   RUN_MQOM_LEAKS=true    MQOM, or shared code it depends on, changed
#   RUN_QRUOV_LEAKS=true   QR-UOV, or shared code it depends on, changed
#
# tests/test_leaks.py reads these on pull requests and feature-branch pushes.
# If the changed files cannot be determined, every setting is printed so that
# nothing is skipped.
#
# Usage: scripts/detect_changed_algorithms.sh [BASE_REF]

set -eu

BASE_REF="${1:-main}"

# Changes to these paths can affect every algorithm. One extended regular
# expression per line, matched against paths relative to the repository root.
SHARED=$(paste -s -d '|' - <<'EOF'
^CMakeLists\.txt$
^\.CMake/
^\.github/workflows/platform-matrix\.yml$
^\.github/workflows/run-suites\.yml$
^scripts/detect_changed_algorithms\.sh$
^src/[^/]*$
^src/common/
^src/sig/sig\.[ch]$
^tests/CMakeLists\.txt$
^tests/conftest\.py$
^tests/helpers\.py$
^tests/system_info\.c$
^tests/test_helpers\.[ch]$
^tests/test_leaks\.py$
^tests/test_sig\.c$
EOF
)

if ! git fetch --quiet --depth=1 origin "$BASE_REF" >&2 ||
   ! changed=$(git diff --name-only "origin/$BASE_REF" HEAD); then
    echo "::warning::Could not determine changed files; running all leak tests" >&2
    echo "RUN_MQOM_LEAKS=true"
    echo "RUN_QRUOV_LEAKS=true"
    exit 0
fi

if printf '%s\n' "$changed" | grep -qE "$SHARED|^src/sig/mqom/"; then
    echo "RUN_MQOM_LEAKS=true"
fi
if printf '%s\n' "$changed" | grep -qE "$SHARED|^src/sig/qruov/"; then
    echo "RUN_QRUOV_LEAKS=true"
fi
