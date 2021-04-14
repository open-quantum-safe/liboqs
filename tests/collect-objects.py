import os
import sys
import subprocess
import difflib
from pprint import pprint

# Set this to also get double-defined symbol warnings; typically just multi-config (e.g., ref+avx2) duplicates
WARN=False

# set of expected unknowns:
KNOWN_UNKNOWNS=["_GLOBAL_OFFSET_TABLE_", "__popcountdi2", "__stack_chk_fail", "__printf_chk", "__fprintf_chk", "fput", "fopen", "fclose", "fput", "fread", "fwrite", "read", "putchar", "puts", "stderr", "stdout", "malloc", "calloc", "realloc", "aligned_alloc", "free", "memset", "memcmp", "memmove", "memcpy", "sqrt", "time", "strstr", "strcmp", "strcasecmp", "strtol", "__strcat_chk", "__strcpy_chk", "__memcpy_chk", "__assert_fail", "__explicit_bzero_chk", "exit", "abort", "strftime", "localtime", "gettimeofday", "pthread", "EVP_", "BN_"]

# return the substring of line starting with idx to the next space or the end of line:
def get_symbol(line, idx):
   si = line.find(" ",idx)
   if si>idx:
     return line[idx:si]
   else:
     return line[idx:]

# return list of all known ("t" or "T") and unknown ("U") symbols:
def get_symbols(filepath):
   known = []
   unknown = []
   # method: Use nm:
   #print(filepath)
   p = subprocess.Popen(["nm", filepath], stdout=subprocess.PIPE)

   while True:
     line = p.stdout.readline()
     if not line:
       break
     l = line.decode().strip()
     ui = l.find("U ")
     ki = l.find("T ")
     if ui >= 0:
        unknown.append(get_symbol(l, ui+2))
     if (ki >= 0):
        known.append(get_symbol(l, ki+2))
     else: # check for "R ":
        ki = l.find("R ")
        if (ki >= 0):
           known.append(get_symbol(l, ki+2))
        
   return (known, unknown)

# iterate through directory given on command line or ./build:
if len(sys.argv)>1:
   rootdir = sys.argv[1]
else:
   print("Usage: %s <objectcode folder to search>." % (sys.argv[0]))
   print("     Will examine all object files for known (T,R) and unknown (U) symbols")
   print("     Outputs unresolved symbols taking common C lib and OpenSSL symbols into account.")
   print("     Also suggests library candidates with more than 5 locations of use.")
   exit(-1)


known = {} # dictionary of known symbols with file containing them
unknown = {} # dictionary of unknown symbols with list of files containing them
for subdir, dirs, files in os.walk(rootdir):
    for file in files:
      # Only look for object files:
      if file.endswith(".o"):
        fpath = os.path.join(subdir, file)
        # determine lists of all new known and unknown symbols for this file
        (nk, nu) = get_symbols(fpath)
        # now add new known symbols to global known symbol table
        for s in nk:
         if s != "main":
           if WARN and (s in known):
              #print("%s in %s already known. Why already in %s?" % (s, fpath, known[s]))
              print("%s already known:" % (s))
              sm=difflib.SequenceMatcher()
              sm.set_seqs(fpath, known[s])
              r=sm.find_longest_match(0, len(fpath), 0, len(known[s]))
              if (r.size>0):
                print("   Diff: " )
                print("         %s " % (fpath[r.size:]))
                print("         %s " % (known[s][r.size:]))
           else:
              known[s]=fpath
        # now add all unknown symbols to global unknown symbol list, possibly adding counts
        for s in nu:
           if not s in unknown:
              unknown[s] = []
           unknown[s].append(fpath)

# finally, output all unknown symbols (i.e., not in known symbol list)
print("known symbols: %d; unknown symbols: %d" % (len(known.keys()), len(unknown.keys())))

for us in unknown.keys():
   known_unknown = False
   for ku in KNOWN_UNKNOWNS:
      if us.startswith(ku):
         known_unknown=True
         break
   # possible candidates for library functions are those appearing often (and not already in the library)
   if not known_unknown and len(unknown[us]) > 5 and not us.startswith("OQS_"):
      print("Library candidate (unknown in %d locations): %s" % (len(unknown[us]), us))
   if not us in known.keys() and not known_unknown:
         print("Symbol not found: %s " % (us))
