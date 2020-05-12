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

if 'PQCLEAN_DIR' not in os.environ:
    print("Must set environment variable PQCLEAN_DIR")
    exit(1)

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

def load_instructions():
    instructions = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', 'copy_from_pqclean.yml'), encoding='utf-8')
    instructions = yaml.safe_load(instructions)
    for family in instructions['kems']:
        family['type'] = 'kem'
        family['pqclean_type'] = 'kem'
        family['family'] = family['name']
        for scheme in family['schemes']:
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
            scheme['metadata'] = yaml.safe_load(file_get_contents(os.path.join(os.environ['PQCLEAN_DIR'], 'crypto_sign', scheme['pqclean_scheme'], 'META.yml')))
            # This is a temporary hack to work around the fact that
            # the PQClean's META.ymls for the Dilithium AVX2 variants
            # are not properly specified.
            if scheme['pretty_name_full'].startswith('DILITHIUM_'):
                scheme['metadata']['implementations'][1]['supported_platforms'][0]['operating_systems'] = ['Linux']
                scheme['metadata']['implementations'][1]['supported_platforms'][0]['required_flags'] = ['avx2', 'bmi', 'popcnt']

            scheme['metadata']['euf_cma'] = 'true'
            scheme['pqclean_scheme_c'] = scheme['pqclean_scheme'].replace('-', '')
            scheme['scheme_c'] = scheme['scheme'].replace('-', '')
            scheme['default_implementation'] = family['default_implementation']
    return instructions

# Copy over all files for a given impl in a family using scheme
# Returns list of all relative source files
def handle_implementation(impl, family, scheme):
        shutil.rmtree(os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl)), ignore_errors=True)
        srcfolder = os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl))
        subprocess.run([
            'cp',
            '-pr',
            os.path.join(os.environ['PQCLEAN_DIR'], 'crypto_' + family['pqclean_type'], scheme['pqclean_scheme'], impl),
            srcfolder
        ])
        try:
            os.remove(os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl), 'Makefile'))
            os.remove(os.path.join('src', family['type'], family['name'], 'pqclean_{}_{}'.format(scheme['pqclean_scheme'], impl), 'Makefile.Microsoft_nmake'))
        except FileNotFoundError:
           pass
        extensions = [ '.c', '.s' ]
        return [str(x.relative_to(srcfolder)) for x in Path(srcfolder).iterdir() if x.suffix.lower() in extensions]


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
