#!/bin/bash
# SPDX-License-Identifier: MIT

BASE=$(dirname "$0")
BASE=$(cd "${BASE}" && pwd)
echo "${BASE}"

VERSION=$(cat VERSION)
UPSTREAM_ARCHIVE="${VERSION}.zip"

BUILD="${BASE}/build"
BUILD_XKCP="${BUILD}/src/common/sha3/xkcp_low"
BUILD_UPSTREAM="${BUILD}/upstream"

function cleanup {
  rm -rf "${BUILD}"
}
trap cleanup EXIT

if [ -e "${BUILD}" ]
then
  printf "%s directory already exists. Delete it (y/n)? " "${BUILD}"
  read -r yn
  if [ "${yn:-n}" != "y" ]
  then
    exit -1
  fi
  rm -rf "${BUILD}"
fi
mkdir -p "${BUILD}"

if [ ! -f "${BASE}/${UPSTREAM_ARCHIVE}" ]
then
  wget -P "${BASE}" "https://github.com/XKCP/XKCP/archive/${VERSION}.zip"
fi

unzip -qq -d "${BUILD}" "${BASE}/${UPSTREAM_ARCHIVE}"
mv "${BUILD}/XKCP-${VERSION}" "${BUILD_UPSTREAM}"

( cd "${BUILD_UPSTREAM}" || exit

  for X in "${BASE}/patches/"*
  do
    patch -s -p1 < "${X}"
  done
)

OUT="${BUILD_XKCP}/KeccakP-1600/plain-64bits"
mkdir -p "${OUT}"
cp -Lp "${BUILD_UPSTREAM}/lib/common/brg_endian.h" \
       "${BUILD_UPSTREAM}/lib/low/common/SnP-Relaned.h" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/common/KeccakP-1600-64.macros" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/common/KeccakP-1600-unrolling.macros" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/plain-64bits/KeccakP-1600-opt64.c" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/plain-64bits/KeccakP-1600-SnP.h" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/plain-64bits/ua/KeccakP-1600-opt64-config.h" \
       "${OUT}"

OUT="${BUILD_XKCP}/KeccakP-1600/avx2"
mkdir -p "${OUT}"
cp -Lp "${BUILD_UPSTREAM}/lib/common/align.h" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/AVX2/KeccakP-1600-AVX2.s" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/AVX2/KeccakP-1600-SnP.h" \
       "${OUT}"

OUT="${BUILD_XKCP}/KeccakP-1600times4/serial"
mkdir -p "${OUT}"
cp -Lp "${BUILD_UPSTREAM}/lib/low/common/PlSnP-Fallback.inc" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600-times4/fallback-on1/KeccakP-1600-times4-on1.c" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600-times4/fallback-on1/KeccakP-1600-times4-SnP.h" \
       "${OUT}"

OUT="${BUILD_XKCP}/KeccakP-1600times4/avx2"
mkdir -p "${OUT}"
cp -Lp "${BUILD_UPSTREAM}/lib/common/brg_endian.h" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600/common/KeccakP-1600-unrolling.macros" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600-times4/AVX2/KeccakP-1600-times4-SIMD256.c" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600-times4/AVX2/KeccakP-1600-times4-SnP.h" \
       "${BUILD_UPSTREAM}/lib/low/KeccakP-1600-times4/AVX2/ua/SIMD256-config.h" \
       "${OUT}"

find "${BUILD_XKCP}" \( -name \*.c -o -name \*.h \) -execdir \
unifdef -k -m \
  -DNO_MISALIGNED_ACCESSES \
  -DKeccakP1600_fullUnrolling \
  -DKeccakP1600times4_fullUnrolling \
  -DKeccakP1600_useLaneComplementing \
  {} \;

find "${BUILD_XKCP}/KeccakP-1600times4/avx2" \( -name \*.c -o -name \*.h \) -execdir \
unifdef -k -m \
  -DKeccakP1600times4_useAVX2 \
  {} \;

cat << EOF > "${BUILD_XKCP}/CMakeLists.txt"
# SPDX-License-Identifier: MIT

set(_XKCP_LOW_OBJS "")
set(_XKCP_LOW_INCLUDE_DIRS "")

if(OQS_USE_AVX2_INSTRUCTIONS AND NOT OQS_DIST_BUILD)
  add_library(xkcp_low_keccakp_1600_avx2 OBJECT KeccakP-1600/avx2/KeccakP-1600-AVX2.s)
  target_compile_options(xkcp_low_keccakp_1600_avx2 PRIVATE -mavx2)
  target_include_directories(xkcp_low_keccakp_1600_avx2 PRIVATE \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/avx2)
  set(_XKCP_LOW_OBJS \${_XKCP_LOW_OBJS} \$<TARGET_OBJECTS:xkcp_low_keccakp_1600_avx2>)
  set(_XKCP_LOW_INCLUDE_DIRS \${_XKCP_LOW_INCLUDE_DIRS} \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/avx2)

  add_library(xkcp_low_keccakp_1600times4_avx2 OBJECT KeccakP-1600times4/avx2/KeccakP-1600-times4-SIMD256.c)
  target_compile_options(xkcp_low_keccakp_1600times4_avx2 PRIVATE -mavx2)
  target_include_directories(xkcp_low_keccakp_1600times4_avx2 PRIVATE \${_XKCP_LOW_INCLUDE_DIRS} \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/avx2)
  set(_XKCP_LOW_OBJS \${_XKCP_LOW_OBJS} \$<TARGET_OBJECTS:xkcp_low_keccakp_1600times4_avx2>)
  set(_XKCP_LOW_INCLUDE_DIRS \${_XKCP_LOW_INCLUDE_DIRS} \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/avx2)
else()
  add_library(xkcp_low_keccakp_1600_plain64 OBJECT KeccakP-1600/plain-64bits/KeccakP-1600-opt64.c)
  target_include_directories(xkcp_low_keccakp_1600_plain64 PRIVATE \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/plain-64bits)
  set(_XKCP_LOW_OBJS \${_XKCP_LOW_OBJS} \$<TARGET_OBJECTS:xkcp_low_keccakp_1600_plain64>)
  set(_XKCP_LOW_INCLUDE_DIRS \${_XKCP_LOW_INCLUDE_DIRS} \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600/plain-64bits)

  add_library(xkcp_low_keccakp_1600times4_plain64 OBJECT KeccakP-1600times4/serial/KeccakP-1600-times4-on1.c)
  target_include_directories(xkcp_low_keccakp_1600times4_plain64 PRIVATE \${_XKCP_LOW_INCLUDE_DIRS} \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/serial)
  set(_XKCP_LOW_OBJS \${_XKCP_LOW_OBJS} \$<TARGET_OBJECTS:xkcp_low_keccakp_1600times4_plain64>)
  set(_XKCP_LOW_INCLUDE_DIRS \${_XKCP_LOW_INCLUDE_DIRS} \${CMAKE_CURRENT_SOURCE_DIR}/KeccakP-1600times4/serial)
endif()

set(XKCP_LOW_OBJS \${_XKCP_LOW_OBJS} PARENT_SCOPE)
set(XKCP_LOW_INCLUDE_DIRS \${_XKCP_LOW_INCLUDE_DIRS} PARENT_SCOPE)
EOF

# Apply liboqs formatting 
astyle \
  --style=google \
  --indent=tab \
  --pad-oper \
  --pad-comma \
  --pad-header \
  --align-pointer=name \
  --add-braces \
  --convert-tabs \
  --mode=c \
  --suffix=none \
  --lineend=linux \
  "${BUILD_XKCP}"/*/*/*.{c,h} >/dev/null

# Package
( cd "${BUILD}" || exit
  tar czf "${BASE}"/liboqs-xkcp-"$(date +'%Y%m%d')".tar.gz src
)
