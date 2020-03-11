#!/bin/bash

# Detects whether there has been a Git commit in the last day on this
# branch. Returns 1 if there has been a commit, 0 if there has not.
# Always returns 1 if running in circleci local

if [ "x${CIRCLECI}" == "xtrue" ] && [ "x${CIRCLE_BUILD_NUM}" == "x" ]; then
	echo "Running in circleci local"
	exit 1
fi

r=`git log --name-only --since="1 day ago" -n 2`
if [ "x$r" == "x" ]; then 
	echo "No commit in the last day. No build/test required. Exiting."
	exit 0
else
	exit 1
fi
