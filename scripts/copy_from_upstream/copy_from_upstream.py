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

# kats of all algs
kats = {}

non_upstream_kems = 0

parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbosity", type=int)
parser.add_argument("-k", "--keep_data", action='store_true')
parser.add_argument("operation", choices=["copy", "verify"])
args = parser.parse_args()

if args.verbosity:
    DEBUG = args.verbosity
else:
    DEBUG = 0

keepdata = True if args.keep_data else False

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

def generator(destination_file_path, template_filename, family, scheme_desired):
    template = file_get_contents(
        os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', template_filename))
    f = copy.deepcopy(family)
    if scheme_desired != None:
        f['schemes'] = [x for x in f['schemes'] if x == scheme_desired]
        assert (len(f['schemes']) == 1)
    # if scheme['implementation'] is not set, run over all implementations!
    file_put_contents(destination_file_path, jinja2.Template(template).render(f))


def generator_all(filename, instructions):
    template = file_get_contents(os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', filename))
    contents = jinja2.Template(template).render({'instructions': instructions})
    file_put_contents(filename, contents)


def replacer(filename, instructions, delimiter):
    fragments = glob.glob(
        os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', filename, '*.fragment'))
    contents = file_get_contents(os.path.join(os.environ['LIBOQS_DIR'], filename))
    for fragment in fragments:
        template = file_get_contents(fragment)
        identifier = os.path.splitext(os.path.basename(fragment))[0]
        identifier_start = '{} OQS_COPY_FROM_UPSTREAM_FRAGMENT_{}_START'.format(delimiter, identifier.upper())
        identifier_end = '{} OQS_COPY_FROM_UPSTREAM_FRAGMENT_{}_END'.format(delimiter, identifier.upper())
        preamble = contents[:contents.find(identifier_start)]
        postamble = contents[contents.find(identifier_end):]
        contents = preamble + identifier_start + jinja2.Template(template).render(
            {'instructions': instructions, 'non_upstream_kems': non_upstream_kems}) + postamble
    file_put_contents(os.path.join(os.environ['LIBOQS_DIR'], filename), contents)

def load_instructions():
    instructions = file_get_contents(
        os.path.join(os.environ['LIBOQS_DIR'], 'scripts', 'copy_from_upstream', 'copy_from_upstream.yml'),
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
          shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'fetch', '--depth=1', 'origin', upstream_git_commit])
          shell(['git', '--git-dir', work_dotgit, '--work-tree', work_dir, 'reset', '--hard', upstream_git_commit])
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
            if not 'upstream_location' in scheme:
                scheme['upstream_location'] = family['upstream_location']
            if (not 'arch_specific_upstream_locations' in scheme) and 'arch_specific_upstream_locations' in family:
                scheme['arch_specific_upstream_locations'] = family['arch_specific_upstream_locations']
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
            scheme['metadata']['euf_cma'] = 'true'
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
        shutil.rmtree(os.path.join(dst_basedir, 'src', family['type'], family['name'],
                               '{}_{}_{}'.format(upstream_location, scheme['pqclean_scheme'], impl)),
                  ignore_errors=True)
        srcfolder = os.path.join(dst_basedir, 'src', family['type'], family['name'],
                             '{}_{}_{}'.format(upstream_location, scheme['pqclean_scheme'], impl))
        # Don't copy from PQClean straight but check for origfile list
        try:
            os.mkdir(srcfolder)
        except FileExistsError as fee:
            print(fee)
            pass
        # determine list of files to copy:
        if 'sources' in i:
            srcs = i['sources'].split(" ")
            for s in srcs:
                # Copy recursively only in case of directories not with plain files to avoid copying over symbolic links
                if os.path.isfile(os.path.join(origfolder, s)):
                    subprocess.run(['cp', os.path.join(origfolder, s), os.path.join(srcfolder, os.path.basename(s))])
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


def process_families(instructions, basedir, with_kat, with_generator):
    for family in instructions['kems'] + instructions['sigs']:
        try:
            os.makedirs(os.path.join(basedir, 'src', family['type'], family['name']))
        except:
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
                        if kats['kem'][scheme['pretty_name_full']] != scheme['metadata']['nistkat-sha256']:
                            print("Info: Updating KAT for %s" % (scheme['pretty_name_full']))
                    except KeyError:  # new key
                        print("Adding new KAT for %s" % (scheme['pretty_name_full']))
                        pass
                    kats['kem'][scheme['pretty_name_full']] = scheme['metadata']['nistkat-sha256']
                else:
                    try:
                        if kats['sig'][scheme['pretty_name_full']] != scheme['metadata']['nistkat-sha256']:
                            print("Info: Updating KAT for %s" % (scheme['pretty_name_full']))
                    except KeyError:  # new key
                        print("Adding new KAT for %s" % (scheme['pretty_name_full']))
                        pass
                    kats['sig'][scheme['pretty_name_full']] = scheme['metadata']['nistkat-sha256']

        if with_generator:
            generator(
                os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'],
                             family['type'] + '_{}.h'.format(family['name'])),
                os.path.join('src', family['type'], 'family', family['type'] + '_family.h'),
                family,
                None,
            )
            generator(
                os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'], 'CMakeLists.txt'),
                os.path.join('src', family['type'], 'family', 'CMakeLists.txt'),
                family,
                None,
            )

            for scheme in family['schemes']:
                generator(
                    os.path.join(os.environ['LIBOQS_DIR'], 'src', family['type'], family['name'],
                                 family['type'] + '_{}_{}.c'.format(family['name'], scheme['scheme_c'])),
                    os.path.join('src', family['type'], 'family', family['type'] + '_scheme.c'),
                    family,
                    scheme,
                )

def copy_from_upstream():
    for t in ["kem", "sig"]:
        with open(os.path.join(os.environ['LIBOQS_DIR'], 'tests', 'KATs', t, 'kats.json'), 'r') as fp:
            kats[t] = json.load(fp)

    instructions = load_instructions()
    process_families(instructions, os.environ['LIBOQS_DIR'], True, True)
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
                if not os.path.isdir(oqsdir) and os.path.isdir(erifydir):
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

non_upstream_kems = count_non_upstream_kems(['bike', 'frodokem', 'ntruprime'])

if args.operation == "copy":
    copy_from_upstream()
elif args.operation == "verify":
    verify_from_upstream()
