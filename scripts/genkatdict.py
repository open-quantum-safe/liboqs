# SPDX-License-Identifier: MIT

import os
import json

KATSHA = ".kat.sha256"
d = {}
for filename in os.listdir("."):
    if filename.endswith(KATSHA): 
        alg = filename[:-len(KATSHA)]
        with open(filename, "r") as f:
           d[alg] = f.read()
        print("added %s with KATSHA %s" % (alg, d[alg]))

with open("kats.json", "w") as f:
   json.dumps(d, f, indent=2, sort_keys=True)
