# SPDX-License-Identifier: MIT

import argparse
import sys
import glob
import yaml
import os

# TODO: This script is a temporary solution to uniformly formatting
# the YAML files in the docs/ folder. It does not sit well with yamllint.

parser = argparse.ArgumentParser()
parser.add_argument("--liboqs-root", default=".")
args = parser.parse_args()

def load_yaml(filename, encoding='utf-8'):
    with open(filename, mode='r', encoding=encoding) as fh:
        return yaml.safe_load(fh.read())

def store_yaml(filename, contents, encoding='utf-8'):
    with open(filename, mode='w', encoding=encoding) as fh:
        yaml.dump(contents, fh, sort_keys=False, allow_unicode=True)

for kem_yaml_path in glob.glob(os.path.join(args.liboqs_root, 'docs', 'algorithms', 'kem', '*.yml')):
    print('Formatting {}.'.format(os.path.basename(kem_yaml_path)))
    kem_yaml = load_yaml(kem_yaml_path)
    store_yaml(kem_yaml_path, kem_yaml)

for sig_yaml_path in glob.glob(os.path.join(args.liboqs_root, 'docs', 'algorithms', 'sig', '*.yml')):
    print('Formatting {}.'.format(os.path.basename(sig_yaml_path)))
    sig_yaml = load_yaml(sig_yaml_path)
    store_yaml(sig_yaml_path, sig_yaml)
