#!/bin/bash
for l in `seq 128 64 256`
do 
  for blocks in `seq 2 1 4`
    do 
     bin/$l\SL_\N0$blocks/test_$l\SL_\N0$blocks 
  done 
done
