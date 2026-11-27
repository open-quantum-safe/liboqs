#!/bin/sh

# SPDX-License-Identifier: MIT

# usage: ./scripts/format_code.sh [--check] [file...]
#
# Formats C sources in src/ and tests/ (or only the given files, as paths relative
# to the repository root) according to .clang-format, skipping files listed in
# .clang-format-ignore. With --check, files are left untouched and the script
# exits non-zero if any file needs reformatting.
#
# The clang-format version is pinned in requirements-linter.txt, since different versions
# may format code differently; install it with
# `pip3 install --require-hashes -r requirements-linter.txt`.
# Set CLANG_FORMAT to use a specific clang-format binary.

CLANG_FORMAT=${CLANG_FORMAT:-clang-format}

if [ "$1" = "--check" ]; then
	mode="--dry-run --Werror"
	shift
else
	mode="-i"
fi

cd "$(dirname "$0")/.." || exit 1

# Read the expected clang-format version from requirements-linter.txt
expected=$(sed -n 's/^clang-format==\([0-9.]*\).*/\1/p' requirements-linter.txt)

# Check if clang-format is installed and query its version
if ! command -v "$CLANG_FORMAT" >/dev/null 2>&1; then
	echo "Error: $CLANG_FORMAT not found. Install clang-format $expected with \`pip3 install --require-hashes -r requirements-linter.txt\`." >&2
	exit 1
fi
found=$("$CLANG_FORMAT" --version | sed -n 's/.*clang-format version \([0-9.]*\).*/\1/p')

# Run clang-format
if [ $# -gt 0 ]; then
	printf '%s\n' "$@" | xargs "$CLANG_FORMAT" $mode
else
	find src tests -name '*.[ch]' | xargs "$CLANG_FORMAT" $mode
fi
rv=$?

# Different clang-format versions may format code differently, so flag a mismatch
# with the pinned version (without failing because of it).
if [ "$found" != "$expected" ]; then
	echo "Warning: used clang-format ${found:-unknown} ($(command -v "$CLANG_FORMAT")), but $expected is pinned in requirements-linter.txt." \
		"Results may differ from CI. Install it with \`pip3 install --require-hashes -r requirements-linter.txt\`." >&2
fi

exit $rv
