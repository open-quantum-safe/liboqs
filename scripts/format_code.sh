#!/bin/sh

# SPDX-License-Identifier: MIT

if [ ! -d "$LIBOQS_DIR" ]
then
	echo Must set environment variable LIBOQS_DIR
	exit 1
fi

docker run --rm -v"$LIBOQS_DIR":/root/liboqs -w /root/liboqs openquantumsafe/ci-ubuntu-focal-x86_64:latest ./tests/run_astyle.sh --no-dry-run
