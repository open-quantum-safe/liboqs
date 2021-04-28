# SPDX-License-Identifier: MIT

import argparse
import sys
import glob
import tabulate
import yaml
import os

# TODO: Add explanatory notes at the end of each markdown file with
# respect to the following keys:
# - no-secret-dependent-branching-claimed
# - no-secret-dependent-branching-checked-by-valgrind
# - large-stack-usage

parser = argparse.ArgumentParser()
parser.add_argument("--liboqs-root", default=".")
args = parser.parse_args()

def load_yaml(filename, encoding='utf-8'):
    with open(filename, mode='r', encoding=encoding) as fh:
        return yaml.safe_load(fh.read())

for kem_yaml_path in glob.glob(os.path.join(args.liboqs_root, 'docs', 'algorithms', 'kem', '*.yml')):
    kem_yaml = load_yaml(kem_yaml_path)
    kem_name = os.path.splitext(os.path.basename(kem_yaml_path))[0]
    print('Updating {}.md'.format(kem_name))

    with open(os.path.join(args.liboqs_root, 'docs', 'algorithms', 'kem', '{}.md'.format(kem_name)), mode='w', encoding='utf-8') as out_md:
        out_md.write('# {}\n\n'.format(kem_yaml['name']))
        out_md.write('- **Algorithm type**: Key encapsulation mechanism.\n')
        out_md.write('- **Main cryptographic assumption**: {}.\n'.format(kem_yaml['crypto-assumption']))
        out_md.write('- **Principal submitters**: {}.\n'.format(', '.join(kem_yaml['principal-submitters'])))
        if 'auxiliary-submitters' in kem_yaml and kem_yaml['auxiliary-submitters']:
            out_md.write('- **Auxiliary submitters**: {}.\n'.format(', '.join(kem_yaml['auxiliary-submitters'])))
        out_md.write('- **Authors website**: {}\n'.format(kem_yaml['website']))
        out_md.write('- **Specification version**: {}.\n'.format(kem_yaml['spec-version']))

        out_md.write('- **Implementation source**: {}'.format(kem_yaml['upstream']))
        if 'upstream-ancestors' in kem_yaml:
            out_md.write(', which takes it from:\n')
            for url in kem_yaml['upstream-ancestors'][:-1]:
                out_md.write('  - {}, which takes it from:\n'.format(url))
            out_md.write('  - {}\n'.format(kem_yaml['upstream-ancestors'][-1]))
        else:
            out_md.write('\n')

        out_md.write('- **Implementation license (SPDX-Identifier)**: {}.\n'.format(kem_yaml['spdx-license-identifier']))

        out_md.write('\n## Parameter set summary\n\n')
        table = [['Parameter set',
                  'Security model',
                  'Claimed NIST Level',
                  'Public key size (bytes)',
                  'Secret key size (bytes)',
                  'Ciphertext size (bytes)',
                  'Shared secret size (bytes)']]
        for parameter_set in kem_yaml['parameter-sets']:
            table.append([parameter_set['name'],
                          parameter_set['claimed-security'],
                          parameter_set['claimed-nist-level'],
                          parameter_set['length-public-key'],
                          parameter_set['length-secret-key'],
                          parameter_set['length-ciphertext'],
                          parameter_set['length-shared-secret']])
        out_md.write(tabulate.tabulate(table, tablefmt="pipe", headers="firstrow", colalign=("center",)))
        out_md.write('\n')

        for parameter_set in kem_yaml['parameter-sets']:
            out_md.write('\n## {} implementation characteristics\n\n'.format(parameter_set['name']))
            table = [['Identifier in upstream',
                      'Supported architecture(s)',
                      'Supported operating system(s)',
                      'CPU extension(s) used',
                      'No branching-on-secrets claimed?',
                      'No branching-on-secrets checked by valgrind?',
                      'Large stack usage?']]
            for impl in parameter_set['implementations']:
                if impl['supported-platforms'] == 'all':
                    table.append([impl['upstream-id'],
                                  'All',
                                  'All',
                                  'None',
                                  impl['no-secret-dependent-branching-claimed'],
                                  impl['no-secret-dependent-branching-checked-by-valgrind'],
                                  impl['large-stack-usage']])
                else:
                    for platform in impl['supported-platforms']:
                        op_systems = ','.join(platform['operating_systems'])
                        if 'required_flags' in platform:
                            flags = ','.join(flag.upper() for flag in platform['required_flags'])
                        else:
                            flags = 'None'

                        table.append([impl['upstream-id'],
                                      platform['architecture'],
                                      op_systems,
                                      flags,
                                      impl['no-secret-dependent-branching-claimed'],
                                      impl['no-secret-dependent-branching-checked-by-valgrind'],
                                      impl['large-stack-usage']])
            out_md.write(tabulate.tabulate(table, tablefmt="pipe", headers="firstrow", colalign=("center",)))
            out_md.write('\n\nAre implementations chosen based on runtime CPU feature detection? **{}**.\n'.format('Yes' if parameter_set['implementations-switch-on-runtime-cpu-features'] else 'No'))
