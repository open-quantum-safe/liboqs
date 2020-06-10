#!/bin/bash

###########
# Clone liboqs source code
#
# Environment variables:
#  - LIBOQS_REPO: which repo to check out from, default https://github.com/open-quantum-safe/liboqs.git
#  - LIBOQS_BRANCH: which branch to check out, default master
###########

set -exo pipefail

LIBOQS_REPO=${LIBOQS_REPO:-"https://github.com/open-quantum-safe/liboqs.git"}
LIBOQS_BRANCH=${LIBOQS_BRANCH:-"master"}

rm -rf tmp/liboqs
git clone --branch ${LIBOQS_BRANCH} --single-branch ${LIBOQS_REPO} tmp/liboqs
