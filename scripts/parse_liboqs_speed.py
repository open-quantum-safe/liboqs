# SPDX-License-Identifier: MIT

import json
import re
import argparse
from enum import Enum

class State(Enum):
   starting=0
   config=1
   parsing=2

data=[]

# Parse command-line arguments
parser = argparse.ArgumentParser(description="Parse speed_kem output and extract cycles.")
parser.add_argument("logfile", help="Log file to parse")
parser.add_argument("--algorithm", help="Algorithm name (e.g., BIKE-L1)", required=True)
args = parser.parse_args()

fn = args.logfile
alg = args.algorithm
state = State.starting

config = ''

with open(fn) as fp: 
   while True:
      line = fp.readline() 
      if not line: 
         break 
      # Remove newlines
      line = line.rstrip()
      if state==State.starting:
         if line.startswith("Configuration info"):
            state=State.config
            fp.readline()
      elif state==State.config:
         if line=="\n": # Skip forward
            fp.readline()
            fp.readline()
         if line.startswith("-------"):
            state=State.parsing
         elif line.startswith("Started at"):
            fp.readline()
         elif ":" in line:
            config = config + line[:line.index(":")] + ": " + line[line.index(":")+1:].lstrip() + " | " # Retrieve build configuration
            
      elif state==State.parsing:
         if line.startswith("Ended"): # Finish
            break
         else:
            alg = line[:line.index(" ")]
            p = re.compile('\S+\s*\|')
            for i in 0,1,2: # Iterate through the different operations under each algorithm
               x=p.findall(fp.readline().rstrip())
               tag = x[0][:x[0].index(" ")] # keygen, encaps, decaps
               iterations = float(x[1][:x[1].index(" ")]) # Iterations
               total_t = float(x[2][:x[2].index(" ")]) # Total time
               mean_t = float(x[3][:x[3].index(" ")]) # Mean time in microseconds
               cycles = int(x[5][:x[5].index(" ")]) # Cycles
               val = iterations/total_t # Number of iterations per second

               data.append({"name": alg + " " + tag, "value": cycles, "unit": "cycles", "extra": config})
      else:
         print("Unknown state: %s" % (line))

# Dump data
output_file = f"{alg}_formatted.json"
with open(output_file, 'w') as outfile:
    json.dump(data, outfile)
