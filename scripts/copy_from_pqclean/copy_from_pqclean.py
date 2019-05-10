#!/usr/bin/env python3

import copy
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

def generator(destination_filename, template_filename, pqclean_dir, family, scheme_desired):
    template = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', template_filename))
    f = copy.deepcopy(family)
    f['family'] = f['name']
    if scheme_desired != None:
        f['schemes'] = [x for x in f['schemes'] if x == scheme_desired]
        assert(len(f['schemes']) == 1)
    for scheme in f['schemes']:
        scheme['metadata'] = yaml.safe_load(file_get_contents(os.path.join(pqclean_dir, 'crypto_kem', scheme['pqclean_scheme'], 'META.yml')))
        scheme['metadata']['implementations'] = [imp for imp in scheme['metadata']['implementations'] if imp['name'] == scheme['implementation']]
        assert(len(scheme['metadata']['implementations']) == 1)
        scheme['metadata']['ind_cca'] = 'true'
    file_put_contents(destination_filename, jinja2.Template(template).render(f))

instructions = file_get_contents(os.path.join('scripts', 'copy_from_pqclean', 'copy_from_pqclean.yml'), encoding='utf-8')
instructions = yaml.safe_load(instructions)

for family in instructions:
    for scheme in family['schemes']:
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
        os.environ['PQCLEAN_DIR'],
        family,
        None,
    )

    generator(
        os.path.join('src', 'kem', family['name'], 'Makefile.am'),
        os.path.join('src', 'kem', 'family', 'Makefile.am'),
        os.environ['PQCLEAN_DIR'],
        family,
        None,
    )

    for scheme in family['schemes']:
        generator(
            os.path.join('src', 'kem', family['name'], 'kem_{}.c'.format(scheme['pqclean_scheme'])),
            os.path.join('src', 'kem', 'family', 'kem_scheme.c'),
            os.environ['PQCLEAN_DIR'],
            family,
            scheme,
        )