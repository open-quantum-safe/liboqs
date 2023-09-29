#!/bin/bash

# SPDX-License-Identifier: MIT

if [ $# -lt 1 ]; then
   echo "Usage: $0 <liboqs-release to test regression against> [<cmake opts> [<make cmd>]]. Exiting."
   exit -1
fi

if [ $# -lt 3 ]; then
   MAKECMD="make -j 2"
else
   MAKECMD=$3
fi

# Approach: Check out $1 into tmp folder, build, run speed_kem|sig and compare results

mkdir tmp && cd tmp && git clone --depth 1 --branch $1 https://github.com/open-quantum-safe/liboqs && cd liboqs && mkdir build && cd build && cmake $2 .. && $MAKECMD && ./tests/speed_kem > ../../speed_kem.log && ./tests/speed_sig > ../../speed_sig.log && cd ../../..

if [ $? -ne 0 ]; then
   echo "Build and test of baseline $1 failed. Exiting."
   exit -1
fi

# transform results into JSON files for simple comparison

cd tmp && git clone --depth 1 https://github.com/open-quantum-safe/profiling.git && cd profiling/perf/scripts && python3 parse_liboqs_speed.py ../../../speed_kem.log && python3 parse_liboqs_speed.py ../../../speed_sig.log && cd ../../../..

if [ $? -ne 0 ]; then
   echo "Failure converting results. Exiting."
   exit -1
fi

# obtain current base speed results
rm -rf build && mkdir build && cd build && cmake $2 .. && $MAKECMD && ./tests/speed_kem > speed_kem.log && ./tests/speed_sig > speed_sig.log && cd ../tmp/profiling/perf/scripts && python3 parse_liboqs_speed.py ../../../../build/speed_kem.log && python3 parse_liboqs_speed.py ../../../../build/speed_sig.log && cd ../../../..

if [ $? -ne 0 ]; then
   echo "Failure creating current results. Exiting."
   exit -1
fi

# now compare results using old/tmp runs as baseline (for list of algorithms)
python3 scripts/noregress.py tmp/speed_kem.json build/speed_kem.json && python3 scripts/noregress.py tmp/speed_sig.json build/speed_sig.json
