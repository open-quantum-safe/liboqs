#!/bin/sh

# SPDX-License-Identifier: MIT

# usage: LIBOQS_DIR=<liboqs dir> ./scripts/format_code.sh

arch=$(uname -m)

# tested on Ubuntu 22 / x86_64 and macOS 13 / arm64
if [ "$arch" != "x86_64" ] && [ "$arch" != "arm64" ]
then
	echo "This script does not currently support systems where \`uname -m\` returns $arch."
	exit 1
fi

if [ ! -d "$LIBOQS_DIR" ]
then
	echo "Please set the environment variable LIBOQS_DIR to point to your liboqs directory."
	exit 1
fi

docker run --rm -v"$LIBOQS_DIR":/root/liboqs -w /root/liboqs openquantumsafe/ci-ubuntu-latest:latest ./tests/run_astyle.sh --no-dry-run
