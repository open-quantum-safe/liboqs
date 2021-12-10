#!/bin/bash

if [ $# -ne 1 ]; then
   echo "Usage: $0 <liboqs-release to test regression against>. Exiting."
   exit -1
fi

# Approach: Check out $1 into tmp folder, build, run speed_kem|sig and compare results

mkdir tmp && cd tmp && git clone --depth 1 --branch $1 https://github.com/open-quantum-safe/liboqs && cd liboqs && mkdir build && cd build && cmake -GNinja .. && ninja && ./tests/speed_kem > ../../speed_kem.log && ./tests/speed_sig > ../../speed_sig.log && cd ../../..

# transform results into JSON files for simple comparison

cd tmp && git clone --depth 1 https://github.com/open-quantum-safe/profiling.git && python3 profiling/perf/scripts/parse_liboqs_speed.py speed_kem.log && python3 profiling/perf/scripts/parse_liboqs_speed.py speed_sig.log && cd ..

# obtain current base speed results
rm -rf build && mkdir build && cd build && cmake -GNinja .. && ninja && ./tests/speed_kem > speed_kem.log && ./tests/speed_sig > speed_sig.log && python3 ../tmp/profiling/perf/scripts/parse_liboqs_speed.py speed_kem.log && python3 ../tmp/profiling/perf/scripts/parse_liboqs_speed.py speed_sig.log && cd ..

# now compare results using old/tmp runs as baseline (for list of algorithms)
python3 scripts/noregress.py tmp/speed_kem.json build/speed_kem.json && python3 scripts/noregress.py tmp/speed_sig.json build/speed_sig.json
