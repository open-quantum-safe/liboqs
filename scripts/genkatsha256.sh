#!/bin/bash

# SPDX-License-Identifier: MIT

# Run in arbitrary folder to create .sha256 files from .kat files
for filename in *.kat; do
    sha256sum $filename | awk '{printf $1}' > $filename.sha256
done
