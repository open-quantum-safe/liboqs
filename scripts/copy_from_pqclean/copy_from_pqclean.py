#!/usr/bin/env python3

import copy
import glob
import jinja2
import os
import shutil
import subprocess
import yaml

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
    for scheme in f['schemes']:
        scheme['metadata']['implementations'] = [imp for imp in scheme['metadata']['implementations'] if imp['name'] == scheme['implementation']]
        assert(len(scheme['metadata']['implementations']) == 1)
    file_put_contents(destination_filename, jinja2.Template(template).render(f))

def generator_all(filename, kems):
    template = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', filename))
    contents = jinja2.Template(template).render({'kems': kems})
    file_put_contents(filename, contents)

def replacer(filename, kems, delimiter):
    fragments = glob.glob(os.path.join('scripts', 'copy_from_pqclean', filename, '*.fragment'))
    contents = file_get_contents(filename)
    for fragment in fragments:
        template = file_get_contents(fragment)
        identifier = os.path.splitext(os.path.basename(fragment))[0]
        identifier_start = '{} OQS_COPY_FROM_PQCLEAN_FRAGMENT_{}_START'.format(delimiter, identifier.upper())
        identifier_end = '{} OQS_COPY_FROM_PQCLEAN_FRAGMENT_{}_END'.format(delimiter, identifier.upper())
        preamble = contents[:contents.find(identifier_start)]
        postamble = contents[contents.find(identifier_end):]
        contents = preamble + identifier_start + jinja2.Template(template).render({'kems': kems}) + postamble
    file_put_contents(filename, contents)

def load_kems():
    instructions = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', 'copy_from_pqclean.yml'), encoding='utf-8')
    instructions = yaml.safe_load(instructions)
    kems = instructions['kems']
    for family in kems:
        family['family'] = family['name']
        for scheme in family['schemes']:
            scheme['metadata'] = yaml.safe_load(file_get_contents(os.path.join(os.environ['PQCLEAN_DIR'], 'crypto_kem', scheme['pqclean_scheme'], 'META.yml')))
            scheme['metadata']['ind_cca'] = 'true'
    return kems

kems = load_kems()

for family in kems:
    for scheme in family['schemes']:
        try:
            os.mkdir(os.path.join('src', 'kem', family['name']))
        except:
            pass
        shutil.rmtree(os.path.join('src', 'kem', family['name'], 'pqclean_{}_clean'.format(scheme['pqclean_scheme'])), ignore_errors=True)
        subprocess.run([
            'cp',
            '-pr',
            os.path.join(os.environ['PQCLEAN_DIR'], 'crypto_kem', scheme['pqclean_scheme'], scheme['implementation']),
            os.path.join('src', 'kem', family['name'], 'pqclean_{}_clean'.format(scheme['pqclean_scheme']))
        ])
        os.remove(os.path.join('src', 'kem', family['name'], 'pqclean_{}_clean'.format(scheme['pqclean_scheme']), 'Makefile'))
        os.remove(os.path.join('src', 'kem', family['name'], 'pqclean_{}_clean'.format(scheme['pqclean_scheme']), 'Makefile.Microsoft_nmake'))

    generator(
        os.path.join('src', 'kem', family['name'], 'kem_{}.h'.format(family['name'])),
        os.path.join('src', 'kem', 'family', 'kem_family.h'),
        family,
        None,
    )

    generator(
        os.path.join('src', 'kem', family['name'], 'Makefile.am'),
        os.path.join('src', 'kem', 'family', 'Makefile.am'),
        family,
        None,
    )

    for scheme in family['schemes']:
        generator(
            os.path.join('src', 'kem', family['name'], 'kem_{}.c'.format(scheme['pqclean_scheme'])),
            os.path.join('src', 'kem', 'family', 'kem_scheme.c'),
            family,
            scheme,
        )

replacer('config/features.m4', kems, '#####')
replacer('configure.ac', kems, '#####')
replacer('Makefile.am', kems, '#####')
replacer('src/kem/kem.c', kems, '/////')
replacer('src/kem/kem.h', kems, '/////')
