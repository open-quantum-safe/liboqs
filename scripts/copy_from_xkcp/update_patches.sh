#!/bin/bash
# SPDX-License-Identifier: MIT

BASE=$(dirname "$0")
BASE=$(cd "${BASE}" && pwd)
cd "${BASE}" || exit

PATCHES="${BASE}/patches"

V1=upstream
V2=upstream-patched

if [ ! -e "${V1}" ]
then
  echo "${BASE}/${V1} not found"
  exit
fi

if [ ! -e "${V2}" ]
then
  echo "${BASE}/${V2} not found"
  exit
fi

rm -rf "${PATCHES}"
mkdir -p "${PATCHES}/tmp/"

diff -ruN --no-dereference "${V1}" "${V2}" > "${PATCHES}/tmp/p"

( cd "${PATCHES}/tmp/" || exit
  splitpatch "${PATCHES}/tmp/p"
  rm "${PATCHES}/tmp/p"
  for X in *
  do
    Y=$(echo "${X}" \
        | head -n 1 "${X}" \
        | tr '\t' ' ' \
        | cut -d ' ' -f 2 \
        | cut -d'/' -f 2- \
        | tr '/' '_')
  mv "${X}" "${Y}"
  sed -i '1,2 s/\t.*$//' "${Y}"
  sed -i '$ s/diff.*//' "${Y}"
  done
)
mv "${PATCHES}"/tmp/* "${PATCHES}"
rm -rf "${PATCHES}"/tmp/
