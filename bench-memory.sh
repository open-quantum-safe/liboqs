#!/bin/bash

DEFAULT_TMP_DIR=/tmp
TMP_DIR=$DEFAULT_TMP_DIR
ALGORITHMS=""
ROOT_DIR=`dirname $0`

#parse arguments
for arg in "$@"
do
  case $arg in
    -tmp-dir=*|-t=*)
      TMP_DIR="${arg#*=}"
      shift
      ;;
    *)
      ALGORITHMS="$ALGORITHMS $arg"
      ;;
  esac
done


function print_help {
cat << EOF
Usage: $0 [OPTION]... ALGORITHM

  --tmp-dir=DIR       temporary directory [default: $DEFAULT_TMP_DIR]
    ALGORITHM         algorithm to test

Example usage: $0 ntru

EOF
  exit 0
}

if [[ ! -d $TMP_DIR ]]; then
  print_help
fi

TMP_FILE_NAME="oqs_mem_bench"
TMP_FILE_PATH=$TMP_DIR/$TMP_FILE_NAME

rm -f $TMP_FILE_PATH
valgrind --tool=massif --massif-out-file=$TMP_FILE_PATH $ROOT_DIR/test_kex -m $ALGORITHMS
ms_print $TMP_FILE_PATH
rm -f $TMP_FILE_PATH



