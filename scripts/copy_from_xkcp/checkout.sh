#!/bin/bash
# SPDX-License-Identifier: MIT

BASE=$(dirname "$0")
BASE=$(cd "${BASE}" && pwd)
cd "${BASE}" || exit

VERSION=$(cat "${BASE}/VERSION")
V1=upstream
V2=upstream-patched

ARCHIVE="${VERSION}.zip"
PATCHES="${BASE}/patches"

if [ -e "${V1}" ]
then
  printf "%s directory already exists. Delete it (y/n)? " "${V1}"
  read -r yn
  if [ "${yn:-n}" != "y" ]
  then
    exit -1
  fi
  rm -rf "${V1}"
fi

if [ -e "${V2}" ]
then
  printf "%s directory already exists. Delete it (y/n)? " "${V2}"
  read -r yn
  if [ "${yn:-n}" != "y" ]
  then
    exit -1
  fi
  rm -rf "${V2}"
fi

if [ ! -f "${BASE}/${ARCHIVE}" ]
then
  wget -P "${BASE}" "https://github.com/XKCP/XKCP/archive/${VERSION}.zip"
fi
unzip -qq -d "${BASE}" "${BASE}/${ARCHIVE}"
mv "XKCP-${VERSION}" "${V1}"
mkdir -p "${V2}"

cp -rp "${V1}"/* "${V2}"

( cd "${V2}" || exit
for X in "${PATCHES}"/*
do
  patch -p1 < "${X}"
done
)
