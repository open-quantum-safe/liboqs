# SPDX-License-Identifier: MIT

import argparse
import sys
import glob
import tabulate
import yaml
import os

def load_yaml(filename, encoding='utf-8'):
    with open(filename, mode='r', encoding=encoding) as fh:
        return yaml.safe_load(fh.read())

def file_get_contents(filename, encoding=None):
    with open(filename, mode='r', encoding=encoding) as fh:
        return fh.read()

kem_yamls = []
sig_yamls = []

########################################
# Update the KEM markdown documentation.
########################################
def do_it(liboqs_root):
    for kem_yaml_path in sorted(glob.glob(os.path.join(liboqs_root, 'docs', 'algorithms', 'kem', '*.yml'))):
        kem_yaml = load_yaml(kem_yaml_path)
        kem_yamls.append(kem_yaml)
        kem_name = os.path.splitext(os.path.basename(kem_yaml_path))[0]
        print('Updating {}/{}.md'.format(os.path.dirname(kem_yaml_path), kem_name))

        with open(os.path.join(liboqs_root, 'docs', 'algorithms', 'kem', '{}.md'.format(kem_name)), mode='w', encoding='utf-8') as out_md:
            out_md.write('# {}\n\n'.format(kem_yaml['name']))
            out_md.write('- **Algorithm type**: Key encapsulation mechanism.\n')
            out_md.write('- **Main cryptographic assumption**: {}.\n'.format(kem_yaml['crypto-assumption']))
            out_md.write('- **Principal submitters**: {}.\n'.format(', '.join(kem_yaml['principal-submitters'])))
            if 'auxiliary-submitters' in kem_yaml and kem_yaml['auxiliary-submitters']:
                out_md.write('- **Auxiliary submitters**: {}.\n'.format(', '.join(kem_yaml['auxiliary-submitters'])))
            out_md.write('- **Authors\' website**: {}\n'.format(kem_yaml['website']))
            out_md.write('- **Specification version**: {}.\n'.format(kem_yaml['spec-version']))

            out_md.write('- **Primary Source**<a name="primary-source"></a>:\n')
            out_md.write('  - **Source**: {}\n'.format(kem_yaml['primary-upstream']['source']))
            out_md.write('  - **Implementation license (SPDX-Identifier)**: {}\n'.format(kem_yaml['primary-upstream']['spdx-license-identifier']))
            if 'optimized-upstreams' in kem_yaml:
                out_md.write('- **Optimized Implementation sources**: {}\n'.format(kem_yaml['primary-upstream']['source']))
                for opt_upstream in kem_yaml['optimized-upstreams']:
                    out_md.write('  - **{}**:<a name="{}"></a>\n'.format(opt_upstream, opt_upstream))
                    out_md.write('      - **Source**: {}\n'.format(kem_yaml['optimized-upstreams'][opt_upstream]['source']))
                    out_md.write('      - **Implementation license (SPDX-Identifier)**: {}\n'.format(kem_yaml['optimized-upstreams'][opt_upstream]['spdx-license-identifier']))
            if 'upstream-ancestors' in kem_yaml:
                out_md.write('- **Ancestors of primary source**:\n')
                for url in kem_yaml['upstream-ancestors'][:-1]:
                    out_md.write('  - {}, which takes it from:\n'.format(url))
                out_md.write('  - {}\n'.format(kem_yaml['upstream-ancestors'][-1]))
            else:
                out_md.write('\n')

            if 'advisories' in kem_yaml:
                out_md.write('\n## Advisories\n\n')
                for advisory in kem_yaml['advisories']:
                    out_md.write('- {}\n'.format(advisory))

            out_md.write('\n## Parameter set summary\n\n')
            table = [['Parameter set',
                      'Parameter set alias',
                      'Security model',
                      'Claimed NIST Level',
                      'Public key size (bytes)',
                      'Secret key size (bytes)',
                      'Ciphertext size (bytes)',
                      'Shared secret size (bytes)']]
            for parameter_set in kem_yaml['parameter-sets']:
                table.append([parameter_set['name'],
                              parameter_set['alias'] if 'alias' in parameter_set else "NA",
                              parameter_set['claimed-security'],
                              parameter_set['claimed-nist-level'],
                              parameter_set['length-public-key'],
                              parameter_set['length-secret-key'],
                              parameter_set['length-ciphertext'],
                              parameter_set['length-shared-secret']])
            out_md.write(tabulate.tabulate(table, tablefmt="pipe", headers="firstrow", colalign=("center",)))
            out_md.write('\n')

            for index, parameter_set in enumerate(kem_yaml['parameter-sets']):
                out_md.write('\n## {} implementation characteristics\n\n'.format(parameter_set['name'].replace("_", "\_")))
                table_header = ['Implementation source',
                                'Identifier in upstream',
                                'Supported architecture(s)',
                                'Supported operating system(s)',
                                'CPU extension(s) used',
                                'No branching-on-secrets claimed?',
                                'No branching-on-secrets checked by valgrind?']
                if index == 0:
                    table_header.append('Large stack usage?‡')
                else:
                    table_header.append('Large stack usage?')

                table = [table_header]
                for impl in parameter_set['implementations']:
                    # todo, automate linking this?
                    # if all platforms are supported, assuming not optimized and is primary upstream
                    if impl['supported-platforms'] == 'all':
                        table.append(['[Primary Source](#primary-source)',
                                      impl['upstream-id'].replace('_', '\_'),
                                      'All',
                                      'All',
                                      'None',
                                      impl['no-secret-dependent-branching-claimed'],
                                      impl['no-secret-dependent-branching-checked-by-valgrind'],
                                      impl['large-stack-usage']])
                    else:
                        for platform in impl['supported-platforms']:
                            if 'operating_systems' not in platform:
                                platform['operating_systems'] = ['All']
                            op_systems = ','.join(platform['operating_systems'])
                            if 'required_flags' in platform and platform['required_flags']:
                                flags = ','.join(flag.upper() for flag in platform['required_flags'])
                            else:
                                flags = 'None'
                            if impl['upstream'] == 'primary-upstream':
                                name = 'Primary Source'
                                anchor = 'primary-source'
                            else:
                                name = impl['upstream']
                                anchor = impl['upstream']
                            upstream_name = '[{}](#{})'.format(name, anchor)
                            table.append([upstream_name,
                                          impl['upstream-id'].replace('_', '\_'),
                                          platform['architecture'].replace('_', '\_'),
                                          op_systems,
                                          flags,
                                          impl['no-secret-dependent-branching-claimed'],
                                          impl['no-secret-dependent-branching-checked-by-valgrind'],
                                          impl['large-stack-usage']])

                out_md.write(tabulate.tabulate(table, tablefmt="pipe", headers="firstrow", colalign=("center",)))
                out_md.write('\n')

                if 'implementations-switch-on-runtime-cpu-features' in parameter_set:
                    out_md.write('\nAre implementations chosen based on runtime CPU feature detection? **{}**.\n'.format('Yes' if parameter_set['implementations-switch-on-runtime-cpu-features'] else 'No'))
                if index == 0:
                    out_md.write('\n ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.\n')

            out_md.write('\n## Explanation of Terms\n\n')
            out_md.write('- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.')

    ##############################################
    # Update the signature markdown documentation.
    ##############################################
    for sig_yaml_path in sorted(glob.glob(os.path.join(liboqs_root, 'docs', 'algorithms', 'sig', '*.yml'))):
        sig_yaml = load_yaml(sig_yaml_path)
        sig_yamls.append(sig_yaml)
        sig_name = os.path.splitext(os.path.basename(sig_yaml_path))[0]
        print('Updating {}/{}.md'.format(os.path.dirname(sig_yaml_path), sig_name))

        with open(os.path.join(liboqs_root, 'docs', 'algorithms', 'sig', '{}.md'.format(sig_name)), mode='w', encoding='utf-8') as out_md:
            out_md.write('# {}\n\n'.format(sig_yaml['name']))
            out_md.write('- **Algorithm type**: Digital signature scheme.\n')
            out_md.write('- **Main cryptographic assumption**: {}.\n'.format(sig_yaml['crypto-assumption']))
            out_md.write('- **Principal submitters**: {}.\n'.format(', '.join(sig_yaml['principal-submitters'])))
            if 'auxiliary-submitters' in sig_yaml and sig_yaml['auxiliary-submitters']:
                out_md.write('- **Auxiliary submitters**: {}.\n'.format(', '.join(sig_yaml['auxiliary-submitters'])))
            out_md.write('- **Authors\' website**: {}\n'.format(sig_yaml['website']))
            out_md.write('- **Specification version**: {}.\n'.format(sig_yaml['spec-version']))

            out_md.write('- **Primary Source**<a name="primary-source"></a>:\n')
            out_md.write('  - **Source**: {}\n'.format(sig_yaml['primary-upstream']['source']))
            out_md.write('  - **Implementation license (SPDX-Identifier)**: {}\n'.format(sig_yaml['primary-upstream']['spdx-license-identifier']))
            if 'optimized-upstreams' in sig_yaml:
                out_md.write('- **Optimized Implementation sources**: {}\n'.format(sig_yaml['primary-upstream']['source']))
                for opt_upstream in sig_yaml['optimized-upstreams']:
                    out_md.write('  - **{}**:<a name="{}"></a>\n'.format(opt_upstream, opt_upstream))
                    out_md.write('      - **Source**: {}\n'.format(sig_yaml['optimized-upstreams'][opt_upstream]['source']))
                    out_md.write('      - **Implementation license (SPDX-Identifier)**: {}\n'.format(sig_yaml['optimized-upstreams'][opt_upstream]['spdx-license-identifier']))

            if 'upstream-ancestors' in sig_yaml:
                out_md.write(', which takes it from:\n')
                for url in sig_yaml['upstream-ancestors'][:-1]:
                    out_md.write('  - {}, which takes it from:\n'.format(url))
                out_md.write('  - {}\n'.format(sig_yaml['upstream-ancestors'][-1]))
            else:
                out_md.write('\n')

            if 'advisories' in sig_yaml:
                out_md.write('\n## Advisories\n\n')
                for advisory in sig_yaml['advisories']:
                    out_md.write('- {}\n'.format(advisory))

            out_md.write('\n## Parameter set summary\n\n')
            table = [['Parameter set',
                      'Parameter set alias',
                      'Security model',
                      'Claimed NIST Level',
                      'Public key size (bytes)',
                      'Secret key size (bytes)',
                      'Signature size (bytes)']]
            for parameter_set in sig_yaml['parameter-sets']:
                table.append([parameter_set['name'].replace('_', '\_'),
                              parameter_set['alias'] if 'alias' in parameter_set else "NA",
                              parameter_set['claimed-security'],
                              parameter_set['claimed-nist-level'],
                              parameter_set['length-public-key'],
                              parameter_set['length-secret-key'],
                              parameter_set['length-signature']])
            out_md.write(tabulate.tabulate(table, tablefmt="pipe", headers="firstrow", colalign=("center",)))
            out_md.write('\n')

            for index, parameter_set in enumerate(sig_yaml['parameter-sets']):
                out_md.write('\n## {} implementation characteristics\n\n'.format(parameter_set['name'].replace("_", "\_")))
                table_header = ['Implementation source',
                                'Identifier in upstream',
                                'Supported architecture(s)',
                                'Supported operating system(s)',
                                'CPU extension(s) used',
                                'No branching-on-secrets claimed?',
                                'No branching-on-secrets checked by valgrind?']
                if index == 0:
                    table_header.append('Large stack usage?‡')
                else:
                    table_header.append('Large stack usage?')

                table = [table_header]
                for impl in parameter_set['implementations']:
                    # todo, automate linking this?
                    # if all platforms are supported, assuming not optimized and is primary upstream
                    if impl['supported-platforms'] == 'all':
                        table.append(['[Primary Source](#primary-source)',
                                      impl['upstream-id'].replace('_', '\_'),
                                      'All',
                                      'All',
                                      'None',
                                      impl['no-secret-dependent-branching-claimed'],
                                      impl['no-secret-dependent-branching-checked-by-valgrind'],
                                      impl['large-stack-usage']])
                    else:
                        for platform in impl['supported-platforms']:
                            if 'operating_systems' not in platform:
                                platform['operating_systems'] = ['All']
                            op_systems = ','.join(platform['operating_systems'])
                            if 'required_flags' in platform and platform['required_flags']:
                                flags = ','.join(flag.upper() for flag in platform['required_flags'])
                            else:
                                flags = 'None'
                            if impl['upstream'] == 'primary-upstream':
                                name = 'Primary Source'
                                anchor = 'primary-source'
                            else:
                                name = impl['upstream']
                                anchor = impl['upstream']
                            upstream_name = '[{}](#{})'.format(name, anchor)
                            table.append([upstream_name,
                                          impl['upstream-id'].replace('_', '\_'),
                                          platform['architecture'].replace('_', '\_'),
                                          op_systems,
                                          flags,
                                          impl['no-secret-dependent-branching-claimed'],
                                          impl['no-secret-dependent-branching-checked-by-valgrind'],
                                          impl['large-stack-usage']])

                out_md.write(tabulate.tabulate(table, tablefmt="pipe", headers="firstrow", colalign=("center",)))
                out_md.write('\n')

                if 'implementations-switch-on-runtime-cpu-features' in parameter_set:
                    out_md.write('\nAre implementations chosen based on runtime CPU feature detection? **{}**.\n'.format('Yes' if parameter_set['implementations-switch-on-runtime-cpu-features'] else 'No'))
                if index == 0:
                    out_md.write('\n ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.\n')

            out_md.write('\n## Explanation of Terms\n\n')
            out_md.write('- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.')



    ####################
    # Update the README.
    ####################
    print("Updating README.md")

    readme_path = os.path.join(liboqs_root, 'README.md')
    start_identifier_tmpl = '<!--- OQS_TEMPLATE_FRAGMENT_LIST_{}_START -->'
    end_identifier_tmpl = '<!--- OQS_TEMPLATE_FRAGMENT_LIST_{}_END -->'

    # KEMS
    readme_contents = file_get_contents(readme_path)

    identifier_start = start_identifier_tmpl.format('KEXS')
    identifier_end = end_identifier_tmpl.format('KEXS')

    preamble = readme_contents[:readme_contents.find(identifier_start)]
    postamble = readme_contents[readme_contents.find(identifier_end):]

    with open(readme_path, mode='w', encoding='utf-8') as readme:
        readme.write(preamble + identifier_start + '\n')

        for kem_yaml in kem_yamls:
            parameter_sets = kem_yaml['parameter-sets']
            if any(impl['large-stack-usage'] for impl in parameter_sets[0]['implementations']):
                readme.write('- **{}**: {}†'.format(kem_yaml['name'], parameter_sets[0]['name']))
                if 'alias' in parameter_sets[0]:
                    readme.write(' (alias: {})'.format(parameter_sets[0]['alias']))
            else:
                readme.write('- **{}**: {}'.format(kem_yaml['name'], parameter_sets[0]['name']))
                if 'alias' in parameter_sets[0]:
                    readme.write(' (alias: {})'.format(parameter_sets[0]['alias']))
            for parameter_set in parameter_sets[1:]:
                if any(impl['large-stack-usage'] for impl in parameter_set['implementations']):
                    readme.write(', {}†'.format(parameter_set['name']))
                    if 'alias' in parameter_set:
                        readme.write(' (alias: {})'.format(parameter_set['alias']))
                else:
                    readme.write(', {}'.format(parameter_set['name']))
                    if 'alias' in parameter_set:
                        readme.write(' (alias: {})'.format(parameter_set['alias']))
            readme.write('\n')

        readme.write(postamble)

    # Signatures
    readme_contents = file_get_contents(readme_path)

    identifier_start = start_identifier_tmpl.format('SIGS')
    identifier_end = end_identifier_tmpl.format('SIGS')

    preamble = readme_contents[:readme_contents.find(identifier_start)]
    postamble = readme_contents[readme_contents.find(identifier_end):]

    with open(readme_path, mode='w', encoding='utf-8') as readme:
        readme.write(preamble + identifier_start + '\n')

        for sig_yaml in sig_yamls[:-1]: # SPHINCS is last in this sorted list and requires special handling.
            parameter_sets = sig_yaml['parameter-sets']
            if any(impl['large-stack-usage'] for impl in parameter_sets[0]['implementations']):
                readme.write('- **{}**: {}†'.format(sig_yaml['name'], parameter_sets[0]['name'].replace('_','\_')))
                if 'alias' in parameter_sets[0]:
                    readme.write(' (alias: {})'.format(parameter_sets[0]['alias']).replace('_','\_'))
            else:
                readme.write('- **{}**: {}'.format(sig_yaml['name'], parameter_sets[0]['name'].replace('_','\_')))
                if 'alias' in parameter_sets[0]:
                    readme.write(' (alias: {})'.format(parameter_sets[0]['alias']).replace('_','\_'))
            for parameter_set in parameter_sets[1:]:
                if any(impl['large-stack-usage'] for impl in parameter_set['implementations']):
                    readme.write(', {}†'.format(parameter_set['name'].replace('_', '\_')))
                    if 'alias' in parameter_set:
                        readme.write(' (alias: {})'.format(parameter_set['alias']).replace('_','\_'))
                else:
                    readme.write(', {}'.format(parameter_set['name'].replace('_', '\_')))
                    if 'alias' in parameter_set:
                        readme.write(' (alias: {})'.format(parameter_set['alias']).replace('_','\_'))
            readme.write('\n')

        sphincs_yml = sig_yamls[-1]
        for hash_func in ['SHA2', 'SHAKE']:
            parameter_sets = [pset for pset in sphincs_yml['parameter-sets'] if hash_func in pset['name']]
            if any(impl['large-stack-usage'] for impl in parameter_sets[0]['implementations']):
                readme.write('- **SPHINCS+-{}**: {}†'.format(hash_func, parameter_sets[0]['name'].replace('_','\_')))
            else:
                readme.write('- **SPHINCS+-{}**: {}'.format(hash_func, parameter_sets[0]['name'].replace('_','\_')))
            for parameter_set in parameter_sets[1:]:
                if any(impl['large-stack-usage'] for impl in parameter_set['implementations']):
                    readme.write(', {}†'.format(parameter_set['name'].replace('_', '\_')))
                else:
                    readme.write(', {}'.format(parameter_set['name'].replace('_', '\_')))
            readme.write('\n')

        readme.write(postamble)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--liboqs-root", default=".")
    args = parser.parse_args()
    do_it(args.liboqs_root)
