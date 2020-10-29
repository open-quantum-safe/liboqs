#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import copy
import glob
import jinja2
import os
import shutil
import subprocess
import yaml
from pathlib import Path
import sys
import json

# kats of all algs
kats = {}

# The higher the value the more is output
if len(sys.argv)>1:
   DEBUG=int(sys.argv[1])
else:
   DEBUG=0

if 'PQCLEAN_DIR' not in os.environ:
    print("Must set environment variable PQCLEAN_DIR")
    exit(1)

for t in ["kem", "sig"]:
   with open(os.path.join('tests', 'KATs', t, 'kats.json'), 'r') as fp:
      kats[t] = json.load(fp)

def file_get_contents(filename, encoding=None):
    with open(filename, mode='r', encoding=encoding) as fh:
        return fh.read()

def file_put_contents(filename, s, encoding=None):
    with open(filename, mode='w', encoding=encoding) as fh:
        fh.write(s)

def generator(destination_filename, template_filename, family, scheme_desired):
    template = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', template_filename))
    f = copy.deepcopy(family)
    if scheme_desired != None:
        f['schemes'] = [x for x in f['schemes'] if x == scheme_desired]
        assert(len(f['schemes']) == 1)
    # if scheme['implementation'] is not set, run over all implementations!
    file_put_contents(destination_filename, jinja2.Template(template).render(f))

def generator_all(filename, instructions):
    template = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', filename))
    contents = jinja2.Template(template).render({'instructions': instructions})
    file_put_contents(filename, contents)

def replacer(filename, instructions, delimiter):
    fragments = glob.glob(os.path.join('scripts', 'copy_from_pqclean', filename, '*.fragment'))
    contents = file_get_contents(filename)
    for fragment in fragments:
        template = file_get_contents(fragment)
        identifier = os.path.splitext(os.path.basename(fragment))[0]
        identifier_start = '{} OQS_COPY_FROM_PQCLEAN_FRAGMENT_{}_START'.format(delimiter, identifier.upper())
        identifier_end = '{} OQS_COPY_FROM_PQCLEAN_FRAGMENT_{}_END'.format(delimiter, identifier.upper())
        preamble = contents[:contents.find(identifier_start)]
        postamble = contents[contents.find(identifier_end):]
        contents = preamble + identifier_start + jinja2.Template(template).render({'instructions': instructions}) + postamble
    file_put_contents(filename, contents)

def upstream_check(scheme):
    # check for presence of an upstream location and insert into scheme if found:
    try:
      if 'upstream_location' in scheme:
        if os.environ.get(scheme['upstream_location']):
          if os.path.isdir(os.environ.get(scheme['upstream_location'])):
            if DEBUG>0:
               print("Loading META-yml from %s" % (os.path.join(os.environ.get(scheme['upstream_location']), '{}_META.yml'.format(scheme['pretty_name_full']))))
            scheme['metadata'] = yaml.safe_load(file_get_contents(os.path.join(os.environ.get(scheme['upstream_location']), '{}_META.yml'.format(scheme['pretty_name_full']))))
          else:
            if DEBUG>0:
              print("upstream location '%s' set but not a directory in the local file system. Falling back to PQClean." % (scheme['upstream_location']))
            scheme.pop('upstream_location', None)
        else:
          if DEBUG>0:
             print("upstream location '%s' set but not found in environment. Falling back to PQClean." % (scheme['upstream_location']))
    except FileNotFoundError as fnf:
      if DEBUG>0:
         print("%s. Falling back to PQClean." %(fnf))
      scheme.pop('upstream_location', None)
      pass

def load_instructions():
    instructions = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', 'copy_from_pqclean.yml'), encoding='utf-8')
    instructions = yaml.safe_load(instructions)
    # drop instructions selectively if not ready
    if ("NOT_READY" in os.environ):
        not_ready=os.environ['NOT_READY'].split(" ")
        for family in instructions['kems']:
            if family['name'] in not_ready:
                instructions["kems"].remove(family)
        for family in instructions['sigs']:
            if family['name'] in not_ready:
                instructions["sigs"].remove(family)
    for family in instructions['kems']:
        family['type'] = 'kem'
        family['pqclean_type'] = 'kem'
        family['family'] = family['name']
        for scheme in family['schemes']:
            upstream_check(scheme)
            if not 'metadata' in scheme:
                scheme['metadata'] = yaml.safe_load(file_get_contents(os.path.join(os.environ['PQCLEAN_DIR'], 'crypto_kem', scheme['pqclean_scheme'], 'META.yml')))
            scheme['metadata']['ind_cca'] = 'true' if (scheme['metadata']['claimed-security'] == "IND-CCA2") else 'false'
            scheme['pqclean_scheme_c'] = scheme['pqclean_scheme'].replace('-', '')
            scheme['scheme_c'] = scheme['scheme'].replace('-', '')
            scheme['default_implementation'] = family['default_implementation']
    for family in instructions['sigs']:
        family['type'] = 'sig'
        family['pqclean_type'] = 'sign'
        family['family'] = family['name']
        for scheme in family['schemes']:
            upstream_check(scheme)
            if not 'metadata' in scheme:
               scheme['metadata'] = yaml.safe_load(file_get_contents(os.path.join(os.environ['PQCLEAN_DIR'], 'crypto_sign', scheme['pqclean_scheme'], 'META.yml')))
               # This is a temporary hack to work around the fact that
               # the PQClean's META.ymls for the Dilithium AVX2 variants
               # are not properly specified.
               if scheme['pretty_name_full'].startswith('DILITHIUM_'):
                   scheme['metadata']['implementations'][1]['supported_platforms'][0]['operating_systems'] = ['Linux']
                   scheme['metadata']['implementations'][1]['supported_platforms'][0]['required_flags'] = ['avx2', 'bmi1', 'popcnt']

            scheme['metadata']['euf_cma'] = 'true'
            scheme['pqclean_scheme_c'] = scheme['pqclean_scheme'].replace('-', '')
            scheme['scheme_c'] = scheme['scheme'].replace('-', '')
            scheme['default_implementation'] = family['default_implementation']
    return instructions

# Copy over all files for a given impl in a family using scheme
# Returns list of all relative source files
def handle_implementation(impl, family, scheme):
        # Obtain current implementation array in i
        for imp in scheme['metadata']['implementations']:
           if imp['name'] == impl:
             i = imp
        if DEBUG>2:
           print("IMP = %s" %(i))
        #if 'upstream_location' in scheme and os.environ.get(scheme['upstream_location']):
        if DEBUG>3:
           print("Obtain files for implementation %s" % (impl))
           print("Obtain files for %s" % (scheme))
        shutil.rmtree(os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl)), ignore_errors=True)
        srcfolder = os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl))
        if 'upstream_location' in scheme and os.environ.get(scheme['upstream_location']):
           # Don't copy from PQClean straight but check for origfile list
           try:
              os.mkdir(srcfolder)
           except FileExistsError as fee:
              print(fee)
              pass
           # determine origin folder of (may be renamed via 'folder_name'):
           if 'folder_name' in i:
              of = i['folder_name']
           else:
              of = impl
           origfolder = os.path.join(os.environ[scheme['upstream_location']], of)
           # determine list of files to copy:
           if 'sources' in i:
              srcs = i['sources'].split(" ")
              for s in srcs:
                 # Copy recursively only in case of directories not with plain files to avoid copying over symbolic links
                 if os.path.isfile(os.path.join(origfolder, s)):
                    subprocess.run([ 'cp', os.path.join(origfolder, s), os.path.join(srcfolder, s) ])
                 else:
                    subprocess.run([ 'cp', '-r', os.path.join(origfolder, s), os.path.join(srcfolder, s) ])
           else:
              raise Exception("Malformed YML file: No sources listed to copy. Check upstream YML file." )
           
        else: # simple case: PQClean sourcefolder copy
           origfolder = os.path.join(os.environ['PQCLEAN_DIR'], 'crypto_' + family['pqclean_type'], scheme['pqclean_scheme'], impl)
           subprocess.run([ 'cp', '-pr', origfolder, srcfolder ])

        try:
            os.remove(os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl), 'Makefile'))
            os.remove(os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl), 'Makefile.Microsoft_nmake'))
        except FileNotFoundError:
           pass
        extensions = [ '.c', '.s' ]
        ffs = []
        for subdir, dirs, files in os.walk(srcfolder):
          for x in files:
           for i in extensions:
              if x.lower().endswith(i):
                 fname = subdir + os.sep + x
                 if DEBUG>2:
                    print("srcfolder: %s - File: %s" % (srcfolder, fname))
                 ffs.append(fname)
        if DEBUG>2:
          print(ffs)
        return [x[len(srcfolder)+1:] for x in ffs]


instructions = load_instructions()

for family in instructions['kems'] + instructions['sigs']:
    for scheme in family['schemes']:
        try:
            os.mkdir(os.path.join('src', family['type'], family['name']))
        except:
            pass
        if 'implementation' in scheme:
            impl = scheme['implementation']
            srcs = handle_implementation(impl, family, scheme)
            if DEBUG>3: 
               print("SRCs found: %s" % (srcs))
            if (scheme['sources']):
               assert(len(scheme['sources']) == len(srcs))
            # in any case: add 'sources' to implementation(s)
            # Only retain this 1 implementation:
            scheme['metadata']['implementations'] = [imp for imp in scheme['metadata']['implementations'] if imp['name'] == impl]
            scheme['metadata']['implementations'][0]['sources'] = srcs
        else:
            # If no scheme['implementation'] given, get the list from META.yml and add all implementations
           for impl in scheme['metadata']['implementations']:
               srcs = handle_implementation(impl['name'], family, scheme)
               if DEBUG>2:
                 print("SRCs found: %s" % (srcs))
               # in any case: add 'sources' to implementation(s)
               impl['sources'] = srcs
               # also add suitable defines:
               try:
                      for i in range(len(impl['supported_platforms'])):
                         req = impl['supported_platforms'][i]
                         impl['required_flags'] = req['required_flags']
               except KeyError as ke:
                      if (impl['name'] != family['default_implementation']):
                          print("No required flags found for %s (KeyError %s on impl %s)\n" % (scheme['scheme'], str(ke), impl['name']))
                      pass

        if family in instructions['kems']:
          try:
            if kats['kem'][scheme['pretty_name_full']] != scheme['metadata']['nistkat-sha256']:
                print("Info: Updating KAT for %s" % (scheme['pretty_name_full']))
          except KeyError: # new key
            print("Adding new KAT for %s" % (scheme['pretty_name_full']))
            pass
          kats['kem'][scheme['pretty_name_full']] = scheme['metadata']['nistkat-sha256'] 
        else:
          try:
            if kats['sig'][scheme['pretty_name_full']] != scheme['metadata']['nistkat-sha256']:
                print("Info: Updating KAT for %s" % (scheme['pretty_name_full']))
          except KeyError: # new key
            print("Adding new KAT for %s" % (scheme['pretty_name_full']))
            pass
          kats['sig'][scheme['pretty_name_full']] = scheme['metadata']['nistkat-sha256'] 

    generator(
        os.path.join('src', family['type'], family['name'], family['type'] + '_{}.h'.format(family['name'])),
        os.path.join('src', family['type'], 'family', family['type'] + '_family.h'),
        family,
        None,
    )

    generator(
        os.path.join('src', family['type'], family['name'], 'CMakeLists.txt'),
        os.path.join('src', family['type'], 'family', 'CMakeLists.txt'),
        family,
        None,
    )

    for scheme in family['schemes']:
        generator(
            os.path.join('src', family['type'], family['name'], family['type'] + '_{}_{}.c'.format(family['name'], scheme['scheme_c'])),
            os.path.join('src', family['type'], 'family', family['type'] + '_scheme.c'),
            family,
            scheme,
        )

replacer('.CMake/alg_support.cmake', instructions, '#####')
replacer('CMakeLists.txt', instructions, '#####')
replacer('src/oqsconfig.h.cmake', instructions, '/////')
replacer('src/CMakeLists.txt', instructions, '#####')
replacer('src/kem/kem.c', instructions, '/////')
replacer('src/kem/kem.h', instructions, '/////')
replacer('src/sig/sig.c', instructions, '/////')
replacer('src/sig/sig.h', instructions, '/////')
replacer('tests/kat_sig.c', instructions, '/////')

# Finally store KATS away again
for t in ["kem", "sig"]:
   with open(os.path.join('tests', 'KATs', t, 'kats.json'), "w") as f:
      json.dump(kats[t], f, indent=2, sort_keys=True)


