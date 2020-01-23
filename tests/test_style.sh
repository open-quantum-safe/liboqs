#!/bin/bash
find src tests -name '*.c' -o -name '*.h' | grep -v picnic/external* | grep -v frodokem/external* | grep -v pqclean | xargs ${CLANGFORMAT} -style=file -i
