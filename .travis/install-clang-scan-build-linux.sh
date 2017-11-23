#!/bin/bash

if [ ! -x "$(which scan-build)" ]; then 
	pip install scan-build
fi;
