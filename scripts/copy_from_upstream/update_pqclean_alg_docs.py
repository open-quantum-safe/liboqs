#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
import os
import subprocess
import yaml
import inspect

parser = argparse.ArgumentParser()
parser.add_argument("--liboqs-root", default=os.path.join("..", ".."))
parser.add_argument("-w", "--write-changes", dest="write_changes", action='store_true')
parser.add_argument("-v", "--verbosity", type=int)
args = parser.parse_args()

if args.verbosity:
    DEBUG = args.verbosity
else:
    DEBUG = 0

if not args.write_changes:
    print("--write-changes not set; changes will not be written out.")

def shell(command, expect=0):
    subprocess_stdout = None if DEBUG > 0 else subprocess.DEVNULL
    ret = subprocess.run(command, stdout=subprocess_stdout, stderr=subprocess_stdout)
    if ret.returncode != expect:
        raise Exception("'{}' failed with error {}. Expected {}.".format(" ".join(command), ret, expect))

def load_yaml(filename, encoding='utf-8'):
    with open(filename, mode='r', encoding=encoding) as fh:
        return yaml.safe_load(fh.read())

def store_yaml(filename, contents, encoding='utf-8'):
    with open(filename, mode='w', encoding=encoding) as fh:
        yaml.dump(contents, fh, sort_keys=False, allow_unicode=True)

def fetch_upstream(upstream_info):
    os.makedirs('repos', exist_ok=True)

    work_dir = os.path.join('repos', upstream_info['name'])
    work_dotgit = os.path.join(work_dir, '.git')
    if not os.path.exists(work_dotgit):
        shell(['git', 'init', work_dir])
        shell(['git', '--git-dir', work_dotgit, 'remote', 'add', 'origin', upstream_info['git_url']])
        shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'remote', 'set-url', 'origin', upstream_info['git_url']])
        shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'fetch', '--depth=1', 'origin', upstream_info['git_commit']])
        shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'reset', '--hard', upstream_info['git_commit']])

    return work_dir

def rhs_if_not_equal(lhs, rhs, not_equal_msg):
    if lhs != rhs:
        if DEBUG > 0:
            caller = inspect.getframeinfo(inspect.stack()[1][0])
            print("Line {}: Discrepancy in {}: lhs: {}, rhs: {}".format(caller.lineno, not_equal_msg, lhs, rhs))
        return rhs
    return lhs

def update_pqclean_kem_alg_docs(kems, upstream_info, pqclean_root, meta_yaml_path_template):
    for kem in kems:
        if kem['upstream_location'] == 'pqclean':
            oqs_yaml_path = os.path.join(args.liboqs_root, 'docs', 'algorithms', 'kem', '{}.yml'.format(kem['name']))
            if os.path.isfile(oqs_yaml_path):
               oqs_yaml = load_yaml(oqs_yaml_path)
            else:
                continue

            # We assume that the ordering of "parameter-sets"
            # in the OQS YAML files matches that of copy_from_upstream.yml
            for index, scheme in enumerate(kem['schemes']):
                if DEBUG > 0:
                    print("Examining {}'s META.yml.".format(scheme['pretty_name_full']))
                pqclean_meta_path = os.path.join(pqclean_root, meta_yaml_path_template.format_map(scheme))
                pqclean_yaml = load_yaml(pqclean_meta_path)

                oqs_yaml['type'] = rhs_if_not_equal(oqs_yaml['type'], pqclean_yaml['type'], "type")
                oqs_yaml['principal-submitters'] = rhs_if_not_equal(oqs_yaml['principal-submitters'], pqclean_yaml['principal-submitters'], "principal-submitters")

                upstream_base_url = upstream_info['git_url'][:-len(".git")]
                oqs_yaml['upstream'] = rhs_if_not_equal(oqs_yaml['upstream'], "{}/commit/{}".format(upstream_base_url, upstream_info['git_commit']), "upstream")

                if 'auxiliary-submitters' in pqclean_yaml:
                        oqs_yaml['auxiliary-submitters'] = rhs_if_not_equal(oqs_yaml['auxiliary-submitters'] if 'auxiliary-submitters' in oqs_yaml else '', pqclean_yaml['auxiliary-submitters'], "auxiliary-submitters")

                oqs_scheme_yaml = oqs_yaml['parameter-sets'][index]

                # TODO: PQClean and liboqs pretty-naming conventions for the
                # following algorithms are out of sync.
                if kem['name'] == 'classic_mceliece' or kem['name'] == 'hqc' or kem['name'] == 'ntru' or kem['name'] == 'saber':
                    oqs_scheme_yaml['name'] = rhs_if_not_equal(oqs_scheme_yaml['name'], scheme['pretty_name_full'], "scheme pretty name")
                else:
                    oqs_scheme_yaml['name'] = rhs_if_not_equal(oqs_scheme_yaml['name'], pqclean_yaml['name'], "scheme pretty name")

                oqs_scheme_yaml['claimed-nist-level'] = rhs_if_not_equal(oqs_scheme_yaml['claimed-nist-level'], pqclean_yaml['claimed-nist-level'], "claimed-nist-level")
                oqs_scheme_yaml['claimed-security'] = rhs_if_not_equal(oqs_scheme_yaml['claimed-security'], pqclean_yaml['claimed-security'], "claimed-security")
                oqs_scheme_yaml['length-public-key'] = rhs_if_not_equal(oqs_scheme_yaml['length-public-key'], pqclean_yaml['length-public-key'], "length-public-key")
                oqs_scheme_yaml['length-ciphertext'] = rhs_if_not_equal(oqs_scheme_yaml['length-ciphertext'], pqclean_yaml['length-ciphertext'], "length-ciphertext")
                oqs_scheme_yaml['length-secret-key'] = rhs_if_not_equal(oqs_scheme_yaml['length-secret-key'], pqclean_yaml['length-secret-key'], "legnth-secret-key")
                oqs_scheme_yaml['length-shared-secret'] = rhs_if_not_equal(oqs_scheme_yaml['length-shared-secret'], pqclean_yaml['length-shared-secret'], "length-shared-secret")

                for impl_index, impl in enumerate(oqs_scheme_yaml['implementations']):
                    for pqclean_impl in pqclean_yaml['implementations']:
                        if impl['upstream-id'] == pqclean_impl['name']:
                            break

                    if 'supported_platforms' in pqclean_impl:
                        impl['supported-platforms'] = rhs_if_not_equal(impl['supported-platforms'], pqclean_impl['supported_platforms'], "supported-platforms")
                    else:
                        impl['supported-platforms'] = rhs_if_not_equal(impl['supported-platforms'], "all", "supported-platforms")

                    oqs_scheme_yaml['implementations'][impl_index] = impl

                oqs_yaml['parameter-sets'][index] = oqs_scheme_yaml

            if args.write_changes:
                store_yaml(oqs_yaml_path, oqs_yaml)

instructions = load_yaml(
    os.path.join(args.liboqs_root, 'scripts', 'copy_from_upstream', 'copy_from_upstream.yml'),
    encoding='utf-8')

for upstream in instructions['upstreams']:
    if upstream['name'] == 'pqclean':
        pqclean_info = upstream
        break
pqclean_root = fetch_upstream(pqclean_info)

update_pqclean_kem_alg_docs(instructions['kems'], pqclean_info, pqclean_root, pqclean_info['kem_meta_path'])
