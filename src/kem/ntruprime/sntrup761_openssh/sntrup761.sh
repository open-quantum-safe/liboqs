#!/usr/bin/env bash

# the upstream source sntrup761.c is
# https://github.com/openssh/openssh-portable/blob/master/sntrup761.c
# this script is to automate changes to that,
# so there are no "manual changes" directly to sntrup761.c
#
# when you pull a fresh copy of sntrup761.c from upstream,
# run this script. (Which you might have to modify.)

# replace variable length arrays on the stack with alloca calls
sed -i \
  -e 's|uint16_t R2\[(len + 1) / 2\], M2\[(len + 1) / 2\];|uint16_t *R2 = (uint16_t *)alloca(sizeof(uint16_t) * ((len + 1) / 2)), *M2 = (uint16_t *)alloca(sizeof(uint16_t) * ((len + 1) / 2));|' \
  -e 's|uint16_t R2\[(len + 1) / 2\], M2\[(len + 1) / 2\], bottomr\[len / 2\];|uint16_t *R2 = (uint16_t *)alloca(sizeof(uint16_t) * ((len + 1) / 2)), *M2 = (uint16_t *)alloca(sizeof(uint16_t) * ((len + 1) / 2)), *bottomr = (uint16_t *)alloca(sizeof(uint16_t) * (len / 2));|' \
  -e 's|uint32_t bottomt\[len / 2\];|uint32_t *bottomt = (uint32_t *)alloca(sizeof(uint32_t) * (len / 2));|' \
  -e 's|unsigned char x\[inlen + 1\], h\[64\];|unsigned char *x = (unsigned char *)alloca(inlen + 1), h[64];|' \
  sntrup761.c
