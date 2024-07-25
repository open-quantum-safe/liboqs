#!/bin/sh

# SPDX-License-Identifier: MIT

arch=$(uname -m)

if [ "$arch" != "x86_64" ] && [ "$arch" != "arm64" ]
then
	echo "This script does not currently support systems where \`uname -m\` returns $arch."
	exit 1
fi

if [ ! -d "$LIBOQS_DIR" ]
then
	echo Please set the environment variable LIBOQS_DIR to point to your liboqs directory.
	exit 1
fi

docker run --rm -v"$LIBOQS_DIR":/root/liboqs -w /root/liboqs openquantumsafe/ci-ubuntu-focal-$arch:latest ./tests/run_astyle.sh --no-dry-run
