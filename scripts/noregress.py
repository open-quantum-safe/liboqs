# SPDX-License-Identifier: MIT

import json
import sys
import os

cutoffpercent = 15

with open(sys.argv[1], 'r') as json_file:
   d1 = json.load(json_file)
with open(sys.argv[2], 'r') as json_file:
   d2 = json.load(json_file)

for k in d1.keys(): # algs
      if k != "config" and k != "cpuinfo":
          if k in d2.keys():
            for op in d1[k]:
              diff = 100.0*(float(d1[k][op]) - float(d2[k][op]))/float(d1[k][op])
              if (abs(diff) > cutoffpercent):
                 print("Alg: %s, Op: %s, Val1: %s; Val2: %s; diff: %2.2f%%" % (k, op, d1[k][op], d2[k][op], diff))
