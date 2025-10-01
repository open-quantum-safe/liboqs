#!/usr/bin/env python3

# SPDX-License-Identifier: MIT

import argparse
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
import platform
import update_upstream_alg_docs
import copy_from_slh_dsa_c
from copy import deepcopy

# kats of all algs
kats = {}

non_upstream_kems = 0

parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbosity", type=int)
parser.add_argument("-k", "--keep_data", action='store_true', help='Keep upstream code in the "repos" folder')
parser.add_argument("-d", "--delete", action='store_true', help='Delete untracked files from implementation directories')
parser.add_argument("operation", choices=["copy", "verify", "libjade"])
args = parser.parse_args()

if args.verbosity:
    DEBUG = args.verbosity
else:
    DEBUG = 0

keepdata = True if args.keep_data else False

delete = True if args.delete else False

if 'LIBOQS_DIR' not in os.environ:
    print("Must set environment variable LIBOQS_DIR")
    exit(1)

# scours the documentation for non-upstream KEMs
# returns the number of documented ones
def count_non_upstream_kems(alglist):
    counted=0
    docs_dir = os.path.join(os.environ['LIBOQS_DIR'], 'docs', 'algorithms', 'kem')
    for alg in alglist:
       with open(os.path.join(docs_dir, alg+".yml"), mode='r', encoding='utf-8') as f:
           algyml = yaml.safe_load(f.read())
           counted = counted + len(algyml['parameter-sets'])
    return counted


def file_get_contents(filename, encoding=None):
    with open(filename, mode='r', encoding=encoding) as fh:
        return fh.read()


def file_put_contents(filename, s, encoding=None):
    with open(filename, mode='w', encoding=encoding) as fh:
        fh.write(s)

def shell(command, expect=0):
    subprocess_stdout = None if DEBUG > 0 else subprocess.DEVNULL
    ret = subprocess.run(command, stdout=subprocess_stdout, stderr=subprocess_stdout)
    if ret.returncode != expect:
        raise Exception("'{}' failed with error {}. Expected {}.".format(" ".join(command), ret, expect))

# Generate template from specified scheme to replace old file in 'copy' mode
# but preserves additions made to file in prior runs of 'libjade' mode 
def generator(destination_file_path, template_filename, delimiter, family, scheme_desired):
    template = file_get_contents(
        os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', template_filename))
    f = copy.deepcopy(family)
    contents = file_get_contents(os.path.join(os.environ['LIBOQS_DIR'], destination_file_path))
    if scheme_desired != None:
        f['schemes'] = [x for x in f['schemes'] if x == scheme_desired]
    identifier = '{} OQS_COPY_FROM_{}_FRAGMENT_{}'.format(delimiter, 'LIBJADE', os.path.splitext(os.path.basename(template_filename))[0].upper())
    if identifier in contents:
        identifier_start, identifier_end = identifier + '_START', identifier + '_END'
        contents = contents.split('\n')
        libjade_contents = '\n'.join(contents[contents.index(identifier_start) + 1: contents.index(identifier_end)])
        contents = jinja2.Template(template).render(f)
        preamble = contents[:contents.find(identifier_start)]
        postamble = contents[contents.find(identifier_end):]
        contents = preamble + identifier_start + '\n' + libjade_contents + '\n' + postamble
    else:
        contents = jinja2.Template(template).render(f)
    file_put_contents(destination_file_path, contents)


def generator_all(filename, instructions):
    template = file_get_contents(os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', filename))
    contents = jinja2.Template(template).render({'instructions': instructions})
    file_put_contents(filename, contents)

# TODO: consider refactoring replacer by calling replace_one_fragment
def replace_one_fragment(
    dst_path: str,
    template_path: str,
    instructions: dict,
    delimiter: str,
    libjade: bool = False,
):
    """Replace a single fragment with a rendered Jinja template

    :param dst_path: path to the rendered file, relative to LIBOQS_DIR
    :param template_path: path to the Jinja template file, relative to LIBOQS_DIR
    :param instructions: copy_from_upstream.yml or some patched version
    :param delimiter: how the identifer for the fragment in the destination file
    is prefixed
    """
    liboqs_dir = os.environ.get("LIBOQS_DIR", None)
    if not liboqs_dir:
        raise KeyError("Environment variable LIBOQS_DIR is missing")
    dst_path = os.path.join(liboqs_dir, dst_path)
    template_path = os.path.join(liboqs_dir, template_path)
    with open(template_path, "r") as template_f, open(dst_path, "r") as dst_f:
        template = template_f.read()
        dst_content = dst_f.read()
    identifier, _ = os.path.splitext(os.path.basename(template_path))
    jade_or_upstream = "LIBJADE" if libjade else "UPSTREAM"
    identifier_start = f"{delimiter} OQS_COPY_FROM_{jade_or_upstream}_FRAGMENT_{identifier.upper()}_START"
    identifier_end = f"{delimiter} OQS_COPY_FROM_{jade_or_upstream}_FRAGMENT_{identifier.upper()}_END"
    preamble = dst_content[: dst_content.find(identifier_start)]
    postamble = dst_content[dst_content.find(identifier_end) :]
    dst_content = (
        preamble
        + identifier_start
        + jinja2.Template(template).render(
            {"instructions": instructions, "non_upstream_kems": non_upstream_kems}
        )
        + postamble
    )
    with open(dst_path, "w") as f:
        f.write(dst_content)

def replacer(filename, instructions, delimiter, libjade=False):
    fragments = glob.glob(
        os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', filename, '*.{}'.format('libjade' if libjade else 'fragment')))
    contents = file_get_contents(os.path.join(os.environ['LIBOQS_DIR'], filename))
    for fragment in fragments:
        template = file_get_contents(fragment)
        identifier = os.path.splitext(os.path.basename(fragment))[0]
        identifier_start = '{} OQS_COPY_FROM_{}_FRAGMENT_{}_START'.format(delimiter, 'LIBJADE' if libjade else 'UPSTREAM', identifier.upper())
        identifier_end = '{} OQS_COPY_FROM_{}_FRAGMENT_{}_END'.format(delimiter, 'LIBJADE' if libjade else 'UPSTREAM', identifier.upper())
        preamble = contents[:contents.find(identifier_start)]
        postamble = contents[contents.find(identifier_end):]
        contents = preamble + identifier_start + jinja2.Template(template).render(
            {'instructions': instructions, 'non_upstream_kems': non_upstream_kems}) + postamble
    file_put_contents(os.path.join(os.environ['LIBOQS_DIR'], filename), contents)

def replacer_contextual(destination_file_path, template_file_path, delimiter, family, scheme_desired, libjade=False):
    contents = file_get_contents(destination_file_path)
    template = file_get_contents(template_file_path)
    identifier = os.path.basename(template_file_path).split(os.extsep)[0]
    identifier_start = '{} OQS_COPY_FROM_{}_FRAGMENT_{}_START'.format(delimiter, 'LIBJADE' if libjade else 'UPSTREAM', identifier.upper())
    identifier_end = '{} OQS_COPY_FROM_{}_FRAGMENT_{}_END'.format(delimiter, 'LIBJADE' if libjade else 'UPSTREAM', identifier.upper())
    f = copy.deepcopy(family)
    if scheme_desired != None:
        f['schemes'] = [x for x in f['schemes'] if x == scheme_desired]
    preamble = contents[:contents.find(identifier_start)]
    postamble = contents[contents.find(identifier_end):]
    contents = preamble + identifier_start + jinja2.Template(template).render(f) + postamble
    file_put_contents(destination_file_path, contents)

def load_instructions(file='copy_from_upstream.yml'):
    instructions = file_get_contents(
        os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', file),
        encoding='utf-8')
    instructions = yaml.safe_load(instructions)
    upstreams = {}
    for upstream in instructions['upstreams']:
        upstream_name = upstream['name']
        upstream_git_url = upstream['git_url']
        upstream_git_commit = upstream['git_commit']
        upstream_git_branch = upstream['git_branch']
        upstreams[upstream_name] = upstream

        work_dir = os.path.join('repos', upstream_name)
        work_dotgit = os.path.join(work_dir, '.git')

        if not os.path.exists(work_dir):
          os.makedirs(work_dir)
          if not os.path.exists(work_dotgit):
            shell(['git', 'init', work_dir])
            shell(['git', '--git-dir', work_dotgit, 'remote', 'add', 'origin', upstream_git_url])
        shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'remote', 'set-url', 'origin', upstream_git_url])
        if file == 'copy_from_libjade.yml':
            shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'fetch', '--depth=1', 'origin', upstream_git_branch])
        else:
            shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'fetch', '--depth=1', 'origin', upstream_git_commit])
        shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'reset', '--hard', upstream_git_commit])
        if file == 'copy_from_libjade.yml':
            try:
                version = subprocess.run(['jasminc', '-version'], capture_output=True).stdout.decode('utf-8').strip().split(' ')[-1]
                if version != instructions['jasmin_version']:
                    print('Expected Jasmin compiler version {}; got version {}.'.format(instructions['jasmin_version'], version))
                    print('Must use Jasmin complier version {} or update copy_from_libjade.yml.'.format(instructions['jasmin_version']))
                    exit(1)
            except FileNotFoundError:
                print('Jasmin compiler not found; must add `jasminc` to PATH.')
                exit(1)
            shell(['make', '-C', os.path.join(work_dir, 'src')])
        if 'patches' in upstream:
            for patch in upstream['patches']:
                patch_file = os.path.join('patches', patch)
                shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'apply', '--whitespace=fix', '--directory', work_dir, patch_file])
                # Make a commit in the temporary repo for each of our patches.
                # Helpful when upstream changes and one of our patches cannot be applied.
                shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'add', '.'])
                shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'commit', '-m', 'Applied {}'.format(patch_file)])

        if 'common_meta_path' in upstream:
            common_meta_path_full = os.path.join(work_dir, upstream['common_meta_path'])
            common_deps = yaml.safe_load(
                file_get_contents(common_meta_path_full))
            for common_dep in common_deps['commons']:
                if not 'folder_name' in common_dep or not 'sources' in common_dep:
                    raise Exception("folder_name and sources required in common dependencies.")
                common_dep['sources'] = common_dep['sources'].split(" ")
                if 'supported_platforms' in common_dep:
                    for i in range(len(common_dep['supported_platforms'])):
                        req = common_dep['supported_platforms'][i]
                        common_dep['required_flags'] = req['required_flags']
            upstream['commons'] = dict(map(lambda x: (x['name'], x), common_deps['commons'] ))

    for family in instructions['kems']:
        family['type'] = 'kem'
        family['pqclean_type'] = 'kem'
        family['family'] = family['name']
        family['common_deps'] = []
        family['common_deps_usedby'] = {}
        family['all_required_flags'] = set()
        for scheme in family['schemes']:
            scheme['family'] = family['name']
            if not 'upstream_location' in scheme:
                scheme['upstream_location'] = family['upstream_location']
            if (not 'arch_specific_upstream_locations' in scheme) and 'arch_specific_upstream_locations' in family:
                scheme['arch_specific_upstream_locations'] = family['arch_specific_upstream_locations']
            if (not 'derandomized_keypair' in scheme) and 'derandomized_keypair' in family:
                scheme['derandomized_keypair'] = family['derandomized_keypair']
            if (not 'derandomized_encaps' in scheme) and 'derandomized_encaps' in family:
                scheme['derandomized_encaps'] = family['derandomized_encaps']
            if not 'git_commit' in scheme:
                scheme['git_commit'] = upstreams[scheme['upstream_location']]['git_commit']
            if not 'git_branch' in scheme:
                scheme['git_branch'] = upstreams[scheme['upstream_location']]['git_branch']
            if not 'git_url' in scheme:
                scheme['git_url'] = upstreams[scheme['upstream_location']]['git_url']
            # upstream_check(scheme)
            if not 'kem_meta_paths' in scheme:
                scheme['kem_meta_paths'] = {}
                scheme['kem_meta_paths']['default'] = os.path.join('repos', scheme['upstream_location'],
                                                       upstreams[scheme['upstream_location']][
                                                           'kem_meta_path'].format_map(scheme))
                if 'arch_specific_upstream_locations' in family:
                    if 'extras' not in scheme['kem_meta_paths']:
                        scheme['kem_meta_paths']['extras'] = {}

                    for arch in family['arch_specific_upstream_locations']:
                        location = family['arch_specific_upstream_locations'][arch]
                        scheme['kem_meta_paths']['extras'][arch] = os.path.join('repos', location,
                                                               upstreams[location]['kem_meta_path'].format_map(scheme))
            metadata = {}
            if not 'metadata' in scheme:
                metadata = yaml.safe_load(file_get_contents(scheme['kem_meta_paths']['default']))
                imps_to_remove = []
                upstream = upstreams[scheme['upstream_location']]
                for imp in metadata['implementations']:
                    if 'ignore' in upstream and "{}_{}_{}".format(upstream['name'], scheme['pqclean_scheme'], imp['name']) in upstream['ignore']:
                        imps_to_remove.append(imp['name'])
                    else:
                        imp['upstream'] = upstream
                for imp_name in imps_to_remove:
                    for i in range(len(metadata['implementations'])):
                        if metadata['implementations'][i]['name'] == imp_name:
                            del metadata['implementations'][i]
                            break

                if 'extras' in scheme['kem_meta_paths']:
                    for arch in scheme['kem_meta_paths']['extras']:
                        implementations = yaml.safe_load(file_get_contents(scheme['kem_meta_paths']['extras'][arch]))['implementations']
                        for imp in implementations:
                            upstream = upstreams[family['arch_specific_upstream_locations'][arch]]
                            if (arch in family['arch_specific_implementations'] and imp['name'] in family['arch_specific_implementations']) \
                                    and ('ignore' not in upstream or ('ignore' in upstream and "{}_{}_{}".format(upstream['name'], scheme['pqclean_scheme'], impl['name']) \
                                            not in upstream['ignore'])):
                                imp['upstream'] = upstream
                                metadata['implementations'].append(imp)
                                break
            scheme['metadata'] = metadata
            if not 'scheme_paths' in scheme:
                scheme['scheme_paths'] = {}
                for imp in scheme['metadata']['implementations']:
                    imp_name = imp['name']
                    location = imp['upstream']['kem_scheme_path']
                    scheme['scheme_paths'][imp_name] = os.path.join('repos', scheme['upstream_location'],
                                                         location.format_map(scheme))
                if 'arch_specific_upstream_locations' in family:
                    # This is to override any implememtations provided by the default upstream that 
                    # are also specifically specified
                    for arch in family['arch_specific_upstream_locations']:
                        if arch in scheme['scheme_paths']:
                            del scheme['scheme_paths'][arch]

                    for arch in family['arch_specific_upstream_locations']:
                        location = family['arch_specific_upstream_locations'][arch]
                        if arch in scheme['scheme_paths']:
                            raise RuntimeError("Found duplicate arch {} in scheme {}".format(arch, scheme))
                        scheme['scheme_paths'][arch] = (os.path.join('repos', location,
                                                                    upstreams[location]['kem_scheme_path'].format_map(scheme)))
            scheme['metadata']['ind_cca'] = 'true' if (
                    scheme['metadata']['claimed-security'] == "IND-CCA2") else 'false'
            scheme['pqclean_scheme_c'] = scheme['pqclean_scheme'].replace('-', '')
            scheme['scheme_c'] = scheme['scheme'].replace('-', '')
            scheme['default_implementation'] = family['default_implementation']
            for impl in scheme['metadata']['implementations']:
                if 'common_dep' in impl:
                    cdeps_names = impl['common_dep'].split(" ")
                    sname = scheme['pretty_name_full']
                    uloc = scheme['upstream_location']
                    for cdep_name in cdeps_names:
                        cdep = upstreams[uloc]['commons'][cdep_name]
                        if 'required_flags' in cdep:
                            family['all_required_flags'].update(cdep['required_flags'])
                        if not 'cdep_path' in cdep:
                            cdep['cdep_path'] = scheme['scheme_paths'][impl['name']]
                        if not cdep['name'] in family['common_deps_usedby']:
                            family['common_deps'].append(cdep)
                            family['common_deps_usedby'][cdep_name] = [{'scheme_c': scheme['scheme_c'], 'impl_name': impl['name']}]
                        else:
                            family['common_deps_usedby'][cdep_name].append({'scheme_c': scheme['scheme_c'], 'impl_name': impl['name']})

    for family in instructions['sigs']:
        family['type'] = 'sig'
        family['pqclean_type'] = 'sign'
        family['family'] = family['name']
        family['common_deps'] = []
        family['common_deps_usedby'] = {}
        family['all_required_flags'] = set()
        for scheme in family['schemes']:
            if not 'upstream_location' in scheme:
                scheme['upstream_location'] = family['upstream_location']
            if not 'git_commit' in scheme:
                scheme['git_commit'] = upstreams[scheme['upstream_location']]['git_commit']
            if not 'git_branch' in scheme:
                scheme['git_branch'] = upstreams[scheme['upstream_location']]['git_branch']
            if not 'git_url' in scheme:
                scheme['git_url'] = upstreams[scheme['upstream_location']]['git_url']
            # upstream_check(scheme)
            if not 'sig_meta_paths' in scheme:
                scheme['sig_meta_paths'] = {}
                scheme['sig_meta_paths']['default'] = os.path.join('repos', scheme['upstream_location'],
                                                       upstreams[scheme['upstream_location']][
                                                           'sig_meta_path'].format_map(scheme))
                if 'arch_specific_upstream_locations' in family:
                    if 'extras' not in scheme['sig_meta_paths']:
                        scheme['sig_meta_paths']['extras'] = {}

                    for arch in family['arch_specific_upstream_locations']:
                        location = family['arch_specific_upstream_locations'][arch]
                        scheme['sig_meta_paths']['extras'][arch] = os.path.join('repos', location,
                                                               upstreams[location]['sig_meta_path'].format_map(scheme))
            metadata = {}
            if not 'metadata' in scheme:
                metadata = yaml.safe_load(file_get_contents(scheme['sig_meta_paths']['default']))
                imps_to_remove = []
                upstream = upstreams[scheme['upstream_location']]
                for imp in metadata['implementations']:
                    if 'ignore' in upstream and "{}_{}_{}".format(upstream['name'], scheme['pqclean_scheme'], imp['name']) in upstream['ignore']:
                        imps_to_remove.append(imp['name'])
                    else:
                        imp['upstream'] = upstream
                for imp_name in imps_to_remove:
                    for i in range(len(metadata['implementations'])):
                        if metadata['implementations'][i]['name'] == imp_name:
                            del metadata['implementations'][i]
                            break

                if 'extras' in scheme['sig_meta_paths']:
                    for arch in scheme['sig_meta_paths']['extras']:
                        implementations = yaml.safe_load(file_get_contents(scheme['sig_meta_paths']['extras'][arch]))['implementations']
                        for imp in implementations:
                            upstream = upstreams[family['arch_specific_upstream_locations'][arch]]
                            if (arch in family['arch_specific_implementations'] and imp['name'] in family['arch_specific_implementations']) \
                                    and ('ignore' not in upstream or ('ignore' in upstream and "{}_{}_{}".format(upstream['name'], scheme['pqclean_scheme'], impl['name']) \
                                            not in upstream['ignore'])):
                                imp['upstream'] = upstream
                                metadata['implementations'].append(imp)
                                break
            scheme['metadata'] = metadata
            if not 'scheme_paths' in scheme:
                scheme['scheme_paths'] = {}
                for imp in scheme['metadata']['implementations']:
                    imp_name = imp['name']
                    location = imp['upstream']['sig_scheme_path']
                    scheme['scheme_paths'][imp_name] = os.path.join('repos', scheme['upstream_location'],
                                                         location.format_map(scheme))
                if 'arch_specific_upstream_locations' in family:
                    # This is to override any implememtations provided by the default upstream that 
                    # are also specifically specified
                    for arch in family['arch_specific_upstream_locations']:
                        if arch in scheme['scheme_paths']:
                            del scheme['scheme_paths'][arch]

                    for arch in family['arch_specific_upstream_locations']:
                        location = family['arch_specific_upstream_locations'][arch]
                        if arch in scheme['scheme_paths']:
                            raise RuntimeError("Found duplicate arch {} in scheme {}".format(arch, scheme))
                        scheme['scheme_paths'][arch] = (os.path.join('repos', location,
                                                                    upstreams[location]['sig_scheme_path'].format_map(scheme)))
            # assume EUF-CMA for schemes that don't specify a security classification
            scheme['metadata']['euf_cma'] = 'true'
            scheme['metadata']['suf_cma'] = 'false'
            if 'claimed-security' in metadata:
                if metadata['claimed-security'] == "SUF-CMA":
                    scheme['metadata']['suf_cma'] = 'true'
            scheme['pqclean_scheme_c'] = scheme['pqclean_scheme'].replace('-', '')
            scheme['scheme_c'] = scheme['scheme'].replace('-', '')
            scheme['default_implementation'] = family['default_implementation']
            for impl in scheme['metadata']['implementations']:
                if 'common_dep' in impl:
                    cdeps_names = impl['common_dep'].split(" ")
                    sname = scheme['pretty_name_full']
                    uloc = scheme['upstream_location']
                    for cdep_name in cdeps_names:
                        cdep = upstreams[uloc]['commons'][cdep_name]
                        if 'required_flags' in cdep:
                            family['all_required_flags'].update(cdep['required_flags'])
                        if not 'cdep_path' in cdep:
                            cdep['cdep_path'] = scheme['scheme_paths'][impl['name']]
                        if not cdep['name'] in family['common_deps_usedby']:
                            family['common_deps'].append(cdep)
                            family['common_deps_usedby'][cdep_name] = [{'scheme_c': scheme['scheme_c'], 'impl_name': impl['name']}]
                        else:
                            family['common_deps_usedby'][cdep_name].append({'scheme_c': scheme['scheme_c'], 'impl_name': impl['name']})

    return instructions


# Copy over all files for a given impl in a family using scheme
# Returns list of all relative source files
def handle_common_deps(common_dep, family, dst_basedir):
    # Obtain current implementation array in i
    if DEBUG > 2:
        print("CDEP = %s" % (common_dep))
    # if 'upstream_location' in scheme and os.environ.get(scheme['upstream_location']):
    if DEBUG > 3:
        print("Obtain files for common dependency %s" % (common_dep))
        print("Obtain files for %s" % (scheme))

    cdep_folder_name = '{}_{}'.format(family['upstream_location'], common_dep['name'])
    shutil.rmtree(os.path.join(dst_basedir, 'src', family['type'], family['name'],
                               cdep_folder_name),
                  ignore_errors=True)
    srcfolder = os.path.join(dst_basedir, 'src', family['type'], family['name'],
                             cdep_folder_name)

    # Don't copy from PQClean straight but check for origfile list
    try:
        os.mkdir(srcfolder)
    except FileExistsError as fee:
        print(fee)
        pass
    # determine origin folder of (we checked before that 'folder_name' is available):
    of = common_dep['folder_name']

    origfolder = os.path.join(common_dep['cdep_path'], of)

    # We checked before that 'sources' are available in the common dependency
    srcs = common_dep['sources']
    for s in srcs:
        # Copy recursively only in case of directories not with plain files to avoid copying over symbolic links
        if os.path.isfile(os.path.join(origfolder, s)):
            os.makedirs(os.path.join(srcfolder, os.path.dirname(s)), exist_ok=True)
            subprocess.run(['cp', os.path.join(origfolder, s), os.path.join(srcfolder, s)])
        else:
            subprocess.run(
                ['cp', '-r', os.path.join(origfolder, s), os.path.join(srcfolder, os.path.basename(s))])


    extensions = ['.c', '.s']
    ffs = []
    for subdir, dirs, files in os.walk(srcfolder):
        for x in files:
            for i in extensions:
                if x.lower().endswith(i):
                    fname = subdir + os.sep + x
                    if DEBUG > 2:
                        print("srcfolder: %s - File: %s" % (srcfolder, fname))
                    ffs.append(fname)
    if DEBUG > 2:
        print(ffs)
    return [x[len(srcfolder) + 1:] for x in ffs]

# Copy over all files for a given impl in a family using scheme
# Returns list of all relative source files
def handle_implementation(impl, family, scheme, dst_basedir):
    # Obtain current implementation array in i
    for imp in scheme['metadata']['implementations']:
        if imp['name'] == impl:
            i = imp
    if DEBUG > 2:
        print("IMP = %s" % (i))
    # if 'upstream_location' in scheme and os.environ.get(scheme['upstream_location']):
    if DEBUG > 3:
        print("Obtain files for implementation %s" % (impl))
        print("Obtain files for %s" % (scheme))

    if 'upstream_location' in scheme:
        # determine origin folder of (may be renamed via 'folder_name'):
        if 'folder_name' in i:
            of = i['folder_name']
        else:
            of = impl
        origfolder = os.path.join(scheme['scheme_paths'][impl], of)
        upstream_location = i['upstream']['name']
        srcfolder = os.path.join(dst_basedir, 'src', family['type'], family['name'],
                             '{}_{}_{}'.format(upstream_location, scheme['pqclean_scheme'], impl))
        shutil.rmtree(srcfolder, ignore_errors=True)
        # Don't copy from PQClean straight but check for origfile list
        try:
            os.mkdir(srcfolder)
        except FileExistsError as fee:
            print(fee)
            pass
        if upstream_location == 'libjade':
            # Flatten directory structure while copying relevant files from libjade repo
            for root, _, files in os.walk(origfolder):
                for file in files:
                    if os.path.splitext(file)[1] in ['.c', '.h']:
                        source_path = os.path.join(root, file)
                        dest_path = os.path.join(srcfolder, file)
                        subprocess.run(['cp', source_path, dest_path])
                    if os.path.splitext(file)[1] in ['.s']:
                        file_name, file_ext = os.path.splitext(file)
                        new_file = ''.join([file_name, file_ext.upper()])
                        source_path = os.path.join(root, file)
                        dest_path = os.path.join(srcfolder, new_file)
                        subprocess.run(['cp', source_path, dest_path])
        else:
            # determine list of files to copy:
            if 'sources' in i:
                if i['sources']:
                    preserve_folder_structure = ('preserve_folder_structure' in i['upstream']) and i['upstream']['preserve_folder_structure'] == True
                    srcs = i['sources'].split(" ")
                    for s in srcs:
                        # Copy recursively only in case of directories not with plain files to avoid copying over symbolic links
                        if os.path.isfile(os.path.join(origfolder, s)):
                            if preserve_folder_structure:
                                subprocess.run(['mkdir', '-p', os.path.join(srcfolder, os.path.dirname(s))])
                                subprocess.run(['cp', os.path.join(origfolder, s), os.path.join(srcfolder, s)])
                            else:
                                subprocess.run(['cp', os.path.join(origfolder, s), os.path.join(srcfolder, os.path.basename(s))])

                        else:
                            if preserve_folder_structure:
                                subprocess.run(
                                    ['cp', '-r', os.path.join(origfolder, s), os.path.join(srcfolder, os.path.dirname(s))])                    
                            else:
                                subprocess.run(
                                    ['cp', '-r', os.path.join(origfolder, s), os.path.join(srcfolder, os.path.basename(s))])
            else:
                subprocess.run(['cp', '-pr', os.path.join(origfolder, '.'), srcfolder])
                # raise Exception("Malformed YML file: No sources listed to copy. Check upstream YML file." )

    else:
        raise Exception("Mandatory argument upstream_location is missing")


    try:
        ul = scheme['upstream_location']
        if 'arch_specific_upstream_locations' in family and impl in family['arch_specific_upstream_locations']:
            ul = family['arch_specific_upstream_locations'][impl]
        elif 'arch_specific_upstream_locations' in scheme and impl in scheme['arch_specific_upstream_locations']:
            ul = scheme['arch_specific_upstream_locations'][impl]
        
        os.remove(os.path.join(dst_basedir, 'src', family['type'], family['name'],
                               '{}_{}_{}'.format(ul, scheme['pqclean_scheme'], impl),
                               'Makefile'))
        os.remove(os.path.join(dst_basedir, 'src', family['type'], family['name'],
                               '{}_{}_{}'.format(ul, scheme['pqclean_scheme'], impl),
                               'Makefile.Microsoft_nmake'))
    except FileNotFoundError:
        pass
    extensions = ['.c', '.s']
    ffs = []
    for subdir, dirs, files in os.walk(srcfolder):
        for x in files:
            for i in extensions:
                if x.lower().endswith(i):
                    fname = subdir + os.sep + x
                    if DEBUG > 2:
                        print("srcfolder: %s - File: %s" % (srcfolder, fname))
                    ffs.append(fname)
    if DEBUG > 2:
        print(ffs)
    return [x[len(srcfolder) + 1:] for x in ffs]


def process_families(instructions, basedir, with_kat, with_generator, with_libjade=False):
    for family in instructions['kems'] + instructions['sigs']:
        try:
            os.makedirs(os.path.join(basedir, 'src', family['type'], family['name']))
        except:
            if delete:
                # clear out all subdirectories
                with os.scandir(os.path.join(basedir, 'src', family['type'], family['name'])) as ls:
                    for entry in ls:
                        if entry.is_dir(follow_symlinks=False):
                            if with_libjade:
                                if not entry.name.startswith('libjade'):
                                    continue
                            elif entry.name.startswith('libjade'):
                                continue
                            to_rm = os.path.join(basedir, 'src', family['type'], family['name'], entry.name)
                            if DEBUG > 3:
                                print("removing %s" % to_rm)
                            shutil.rmtree(to_rm)
            pass
        if 'common_deps' in family:
            for common_dep in family['common_deps']:
                srcs = handle_common_deps(common_dep, family, basedir)
                if DEBUG > 3:
                    print("SRCs found: %s" % (srcs))
                if (common_dep['sources']):
                    expected_sources = list(filter(lambda x: x.lower().endswith(".c") or x.lower().endswith(".s"), common_dep['sources']))
                    assert (len(expected_sources) == len(srcs))
                    common_dep['sources_addl'] = srcs
        for scheme in family['schemes']:
            if 'implementation' in scheme:
                impl = scheme['implementation']
                srcs = handle_implementation(impl, family, scheme, basedir)
                if DEBUG > 3:
                    print("SRCs found: %s" % (srcs))
                if ('sources' in scheme):
                    assert (len(scheme['sources']) == len(srcs))
                # in any case: add 'sources' to implementation(s)
                # Only retain this 1 implementation:
                scheme['metadata']['implementations'] = [imp for imp in scheme['metadata']['implementations'] if
                                                         imp['name'] == impl]
                scheme['metadata']['implementations'][0]['sources'] = srcs
            else:
                for impl in scheme['metadata']['implementations']:
                    srcs = handle_implementation(impl['name'], family, scheme, basedir)
                    if DEBUG > 2:
                        print("SRCs found: %s" % (srcs))
                    # in any case: add 'sources' to implementation(s)
                    impl['sources'] = srcs
                    # also add suitable defines:
                    try:
                        for i in range(len(impl['supported_platforms'])):
                            req = impl['supported_platforms'][i]
                            # if compiling for ARM64_V8, asimd/neon is implied and will cause errors
                            # when provided to the compiler; OQS uses the term ARM_NEON
                            if req['architecture'] == 'arm_8':
                                req['architecture'] = 'ARM64_V8'
                            if 'required_flags' in req:
                                if req['architecture'] == 'ARM64_V8' and 'asimd' in req['required_flags']:
                                    req['required_flags'].remove('asimd')
                                    req['required_flags'].append('arm_neon')
                                if req['architecture'] == 'ARM64_V8' and 'sha3' in req['required_flags']:
                                    req['required_flags'].remove('sha3')
                                    req['required_flags'].append('arm_sha3')
                                impl['required_flags'] = req['required_flags']
                                family['all_required_flags'].update(req['required_flags'])
                    except KeyError as ke:
                        if (impl['name'] != family['default_implementation']):
                            print("No required flags found for %s (KeyError %s on impl %s)" % (
                                scheme['scheme'], str(ke), impl['name']))
                        pass


            if with_kat:
                if family in instructions['kems']:
                    try:
                        if kats['kem'][scheme['pretty_name_full']]['single'] != scheme['metadata']['nistkat-sha256']:
                            print("Info: Updating KAT for %s" % (scheme['pretty_name_full']))
                    except KeyError:  # new key
                        print("Adding new KAT for %s" % (scheme['pretty_name_full']))
                        # either a new scheme or a new KAT
                        if scheme['pretty_name_full'] not in kats['kem']:
                            kats['kem'][scheme['pretty_name_full']] = {}
                        pass
                    kats['kem'][scheme['pretty_name_full']]['single'] = scheme['metadata']['nistkat-sha256']
                    if 'alias_pretty_name_full' in scheme:
                        kats['kem'][scheme['alias_pretty_name_full']]['single'] = scheme['metadata']['nistkat-sha256']
                else:
                    try:
                        if kats['sig'][scheme['pretty_name_full']]['single'] != scheme['metadata']['nistkat-sha256']:
                            print("Info: Updating KAT for %s" % (scheme['pretty_name_full']))
                    except KeyError:  # new key
                        print("Adding new KAT for %s" % (scheme['pretty_name_full']))
                        # either a new scheme or a new KAT
                        if scheme['pretty_name_full'] not in kats['sig']:
                            kats['sig'][scheme['pretty_name_full']] = {}
                        pass
                    kats['sig'][scheme['pretty_name_full']]['single'] = scheme['metadata']['nistkat-sha256']
                    if 'alias_pretty_name_full' in scheme:
                        kats['sig'][scheme['alias_pretty_name_full']]['single'] = scheme['metadata']['nistkat-sha256']

        if with_generator:
            generator(
                os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'],
                             family['type'] + '_{}.h'.format(family['name'])),
                os.path.join('src', family['type'], 'family', family['type'] + '_family.h'),
                '/////',
                family,
                None,
            )
            generator(
                os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'], 'CMakeLists.txt'),
                os.path.join('src', family['type'], 'family', 'CMakeLists.txt'),
                '#####',
                family,
                None,
            )

            for scheme in family['schemes']:
                generator(
                    os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'],
                                 family['type'] + '_{}_{}.c'.format(family['name'], scheme['scheme_c'])),
                    os.path.join('src', family['type'], 'family', family['type'] + '_scheme.c'),
                    '/////',
                    family,
                    scheme,
                )
        
        if with_libjade:
            replacer_contextual(
                os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'], 'CMakeLists.txt'),
                os.path.join('src', family['type'], 'family', 'CMakeLists.txt.libjade'),
                '#####',
                family,
                None,
                libjade=True
            )


def copy_from_upstream(slh_dsa_inst: dict):
    """Integrate upstreams implementations and algorithms described in 
    copy_from_upstream.yml.

    :param slh_dsa_inst: instruction for integrating SLH-DSA, only used for 
    rendering alg_support.cmake
    """
    for t in ["kem", "sig"]:
        with open(os.path.join(os.environ['LIBOQS_DIR'], 'tests', 'KATs', t, 'kats.json'), 'r') as fp:
            kats[t] = json.load(fp)

    instructions = load_instructions('copy_from_upstream.yml')
    patched_inst: dict = deepcopy(instructions)
    patched_inst["sigs"].append(slh_dsa_inst["sigs"][0])
    process_families(instructions, os.environ['LIBOQS_DIR'], True, True)
    replacer('.CMake/alg_support.cmake', instructions, '#####')
    # NOTE: issue 2203, only for replacing list of standardized algs
    replace_one_fragment(
        ".CMake/alg_support.cmake",
        "scripts/copy_from_upstream/.CMake/alg_support.cmake/list_standardized_algs.fragment",
        patched_inst,
        "#####"
    )
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
        with open(os.path.join(os.environ['LIBOQS_DIR'], 'tests', 'KATs', t, 'kats.json'), "w") as f:
            json.dump(kats[t], f, indent=2, sort_keys=True)

    update_upstream_alg_docs.do_it(os.environ['LIBOQS_DIR'])

    sys.path.insert(1, os.path.join(os.environ['LIBOQS_DIR'], 'scripts'))
    import update_docs_from_yaml
    import update_cbom
    update_docs_from_yaml.do_it(os.environ['LIBOQS_DIR'])
    update_cbom.update_cbom_if_algs_not_changed(os.environ['LIBOQS_DIR'], "git")
    if not keepdata:
        shutil.rmtree('repos')

# Copy algorithms from libjade specified in copy_from_libjade.yml, apply 
# patches and generate select templates
# Can be run independant of 'copy' mode.
# When adding an algorithm to copy_from_libjade.yml, the boolean 
# 'libjade_implementation' and list of implementation 'libjade_implementations' 
# must updated for the relevant algorithm in copy_from_upstream.yml
def copy_from_libjade():
    for t in ["kem", "sig"]:
        with open(os.path.join(os.environ['LIBOQS_DIR'], 'tests', 'KATs', t, 'kats.json'), 'r') as fp:
            kats[t] = json.load(fp)

    instructions = load_instructions('copy_from_libjade.yml')
    process_families(instructions, os.environ['LIBOQS_DIR'], True, False, True)
    replacer('.CMake/alg_support.cmake', instructions, '#####', libjade=True)
    replacer('src/oqsconfig.h.cmake', instructions, '/////', libjade=True)
    for t in ["kem", "sig"]:
        with open(os.path.join(os.environ['LIBOQS_DIR'], 'tests', 'KATs', t, 'kats.json'), "w") as f:
            json.dump(kats[t], f, indent=2, sort_keys=True)

    update_upstream_alg_docs.do_it(os.environ['LIBOQS_DIR'], upstream_location='libjade')

    sys.path.insert(1, os.path.join(os.environ['LIBOQS_DIR'], 'scripts'))
    import update_docs_from_yaml
    import update_cbom
    update_docs_from_yaml.do_it(os.environ['LIBOQS_DIR'])
    update_cbom.update_cbom_if_algs_not_changed(os.environ['LIBOQS_DIR'], "git")
    if not keepdata:
        shutil.rmtree('repos')


def verify_from_upstream():
    instructions = load_instructions()
    basedir = "verify_from_upstream"

    process_families(instructions, basedir, False, False)

    validated = 0
    differ = 0
    dinfo = []

    for family in instructions['kems'] + instructions['sigs']:
        for scheme in family['schemes']:
            if 'implementation' in scheme:
                impl = scheme['implementation']
                oqsdir = os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'],
                                      '{}_{}_{}'.format(impl['upstream']['name'], scheme['pqclean_scheme'], impl))
                verifydir = os.path.join(basedir, 'src', family['type'], family['name'],
                                         '{}_{}_{}'.format(impl['upstream']['name'], scheme['pqclean_scheme'], impl))
                if not os.path.isdir(oqsdir) and os.path.isdir(verifydir):
                    print('Available implementation in upstream that isn\'t integrated into LIBOQS: {}_{}_{}'.format(impl['upstream']['name'],
                                                                                                                scheme['pqclean_scheme'], impl))
                else:
                    scheme['verifydir'] = '{}_{}_{}'.format(impl['upstream']['name'], scheme['pqclean_scheme'], impl)
                    ret = subprocess.run(['diff', '-rq', oqsdir, verifydir], stdout=subprocess.DEVNULL)
                    # If we haven't integrated something from upstream it shouldn't be reported as an error, it should just be reported.
                    if ret.returncode == 0:
                        validated += 1
                    else:
                        differ += 1
                        dinfo.append(scheme)
            else:
                # If no scheme['implementation'] given, get the list from META.yml and add all implementations
                for impl in scheme['metadata']['implementations']:
                    oqsdir = os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'],
                                          '{}_{}_{}'.format(impl['upstream']['name'], scheme['pqclean_scheme'],
                                                            impl['name']))
                    verifydir = os.path.join(basedir, 'src', family['type'], family['name'],
                                             '{}_{}_{}'.format(impl['upstream']['name'], scheme['pqclean_scheme'],
                                                               impl['name']))
                    if not os.path.isdir(oqsdir) and os.path.isdir(verifydir):
                        print('Available implementation in upstream that isn\'t integrated into LIBOQS: {}_{}_{}'.format(impl['upstream']['name'],
                                                                                                                    scheme['pqclean_scheme'], impl['name']))
                    else:
                        scheme['verifydir'] = '{}_{}_{}'.format(impl['upstream']['name'], scheme['pqclean_scheme'],
                                                            impl['name'])
                        ret = subprocess.run(['diff', '-rq', oqsdir, verifydir], stdout=subprocess.DEVNULL)
                        if ret.returncode == 0:
                            validated += 1
                        else:
                            differ += 1
                            dinfo.append(scheme)

    patch_list = []
    for upstream in instructions['upstreams']:
        if 'patches' in upstream:
            patch_list.extend(upstream['patches'])

    print("-----\nTotal schemes: {} - {} match upstream up to local patches, {} differ".format(validated + differ, validated, differ))
    if len(patch_list):
        print("-----\nPatches applied:\n\t{}".format("\n\t".join(patch_list)))
    if differ > 0:
        print("-----\nSchemes that differ from upstream:")
    for s in dinfo:
        print("Name: {}, expected upstream: {} - commit: {}".format(s['verifydir'], s['git_url'], s['git_commit']))
    print("-----")

    if not keepdata:
        shutil.rmtree(basedir)
        shutil.rmtree('repos')

    if (differ > 0):
        exit(1)

non_upstream_kems = count_non_upstream_kems(['bike', 'frodokem', 'ntruprime', 'ntru'])

if args.operation == "copy":
    # copy_from_slh_dsa_c will modify slh_dsa.yml before copy_from_upstream modifies md files
    slh_dsa_schemes: list[str] = copy_from_slh_dsa_c.main()
    slh_dsa_instruction = {
        "sigs": [
            {
                "name": "slh_dsa",
                "schemes": [
                    {"scheme": scheme} for scheme in slh_dsa_schemes
                    if "pure" in scheme
                ]
            }
        ]
    }
    os.chdir(os.path.join(os.environ['LIBOQS_DIR'],"scripts","copy_from_upstream"))
    copy_from_upstream(slh_dsa_instruction)
elif args.operation == "libjade":
    copy_from_libjade()
elif args.operation == "verify":
    verify_from_upstream()
