#!/bin/bash
# SPDX-License-Identifier: MIT

find src tests -name '*.[ch]' | grep -v '/external/' | grep -v 'kem/.*/.*/.*' | grep -v 'sig/.*/.*/.*' | xargs astyle --dry-run --options=.astylerc
