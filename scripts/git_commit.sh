#!/bin/bash

# SPDX-License-Identifier: MIT

# Get the SHA-1 hash of the current git commit, if any
# based on https://stackoverflow.com/questions/8215785/how-can-i-use-ac-revision-with-git

changes=`git status -s 2>&1`
if [ $? -ne 0 ] ; then
  # not a git repository
  printf 'unknown'
elif [ "x$changes" == "x" ] ; then
  # no changes since last commit
  printf '%s' `git rev-parse HEAD`
else
  # changes since last commit
  printf '%s (+ local modifications)' `git rev-parse HEAD`
fi