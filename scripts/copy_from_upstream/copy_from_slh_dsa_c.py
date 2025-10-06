# SPDX-License-Identifier: MIT

import os
import yaml
import tarfile
import requests
import shutil
import jinja2
import glob
import itertools
import copy
import subprocess

#get contents of a file
def file_get_contents(filename, encoding=None):
    with open(filename, mode='r', encoding=encoding) as fh:
        return fh.read()

#copy tarball of the specified commit
def copy_from_commit():
    tar_name = 'slh_dsa_c.tar.gz'
    tar_path = os.path.join(slh_dir, tar_name)

    #clean up code
    shutil.rmtree(os.path.join(slh_dsa_c_dir))

    url = os.path.join('https://github.com/pq-code-package/slhdsa-c/archive/', commit_hash, ".tar.gz")
    
    response = requests.get(url) 

    if response.status_code == 200:
        os.makedirs(slh_dir, exist_ok=True)

        with open(tar_path, 'wb') as file:
            file.write(response.content)

        with tarfile.open(tar_path) as tar_file:
            tar_file.extractall(slh_dir)
        
        os.remove(tar_path)

        for entry in os.listdir(slh_dir):
            if entry.startswith('slhdsa'):
                full_path = os.path.join(slh_dir, entry)
                if os.path.isdir(full_path):
                    os.rename(full_path, slh_dsa_temp_dir)

        #load meta file
        meta = file_get_contents(meta_file, encoding='utf-8')
        meta = yaml.safe_load(meta)

        #copy sources from temp
        os.makedirs(slh_dsa_c_dir, exist_ok=True)
        sources = meta['sources']

        for root, dirs, files in os.walk(slh_dsa_temp_dir):
            for file in files:
                full_path = os.path.join(root, file)
                rel_path = os.path.relpath(full_path, slh_dsa_temp_dir)
                if rel_path in sources:
                    os.makedirs(os.path.dirname(os.path.join(slh_dsa_c_dir,rel_path)), exist_ok=True)
                    shutil.copy(os.path.join(slh_dsa_temp_dir,rel_path), os.path.join(slh_dsa_c_dir,rel_path))
        
        shutil.rmtree(slh_dsa_temp_dir)

        print('Copied from slh dsa commit succesfully')
    else:
        print('Failed to copy from slh dsa commit with HTTP status code: ' + str(response.status_code))

# Will retrieve start or end indices for sections
def section_bound(identifier, delimiter, text, side):
    searchString = delimiter + ' OQS_COPY_FROM_SLH_DSA_FRAGMENT_' + identifier + '_' + side
    res = text.find(searchString)
    if side == 'START':
        res += len(searchString)
    return res

#replace a single fragment
def fragment_replacer(template_file, destination_file, identifier, variants, destination_delimiter):
    #get section at identifier in template
    template = file_get_contents(template_file)
    section = template[section_bound(identifier,'#####',template,'START'):section_bound(identifier,'#####',template,'END')]
    
    #get preamble/postamble in destination file
    destination = file_get_contents(destination_file)
    preamble = destination[:section_bound(identifier,destination_delimiter,destination,'START')]
    postamble = destination[section_bound(identifier,destination_delimiter,destination,'END'):]

    #replace destination section with rendered template
    contents = preamble + jinja2.Template(section).render(variants) + postamble
    with open(destination_file, "w") as f:
                f.write(contents)

#replace all fragment in destination file
def file_replacer(template_file, destination_file, variants, destination_delimiter):
    #get fragment list in template file
    template = file_get_contents(template_file)
    list_string = template[section_bound('IDENTIFIER_LIST','#####',template,'START'):section_bound('IDENTIFIER_LIST','#####',template,'END')]
    id_list = list_string.split()
    for id in id_list:
        fragment_replacer(template_file,destination_file,id,variants, destination_delimiter)

def doc_replacer(template_file, destination_file):
    implementations = []

    #add pure variants
    for i,j in itertools.product(range(len(meta['hashAlgs'])),range(len(meta['paramSets']))):
        hashAlg = meta['hashAlgs'][i]
        paramSet = meta['paramSets'][j]
        variant = "SLH_DSA_PURE_" + hashAlg['name'].upper() + "_" + paramSet['name'].upper()
        implementations.append({"variant": variant, "paramSet": paramSet})
    
    #add prehash variants
    for i,j,k in itertools.product(range(len(meta['hashAlgs'])),range(len(meta['paramSets'])),range(len(meta['prehashHashAlgs']))):
        hashAlg = meta['hashAlgs'][i]
        paramSet = meta['paramSets'][j]
        prehashHashAlg = meta['prehashHashAlgs'][k]
        variant = "SLH_DSA_" + prehashHashAlg['name'].upper() + "_PREHASH_" + hashAlg['name'].upper() + "_" + paramSet['name'].upper()
        implementations.append({"variant": variant, "paramSet": paramSet})
    
    #render template
    template = file_get_contents(template_file)

    #write to destination
    contents = jinja2.Template(template).render({'implementations': implementations, 'commitHash': commit_hash})
    with open(destination_file, "w") as f:
                f.write(contents)

#generate slh_dsa specific files
def internal_code_gen():
    #clean up code
    shutil.rmtree(slh_wrappers_dir)

    #Start Header File and Setup
    header_template = file_get_contents(jinja_header_file)
    
    header_section = header_template[section_bound('0','#####', header_template,'START'):section_bound('0','#####',header_template,'END')]
    header_contents = jinja2.Template(header_section).render()
    
    src_template = file_get_contents(jinja_src_file)
    header_section = header_template[section_bound('BODY','#####',header_template,'START'):section_bound('BODY','#####',header_template,'END')]
    
    algDetails = meta['algDetails']
    impl['algVersion'] = algDetails['algVersion']
    impl['eufCMA'] = algDetails['eufCMA']
    impl['sufCMA'] = algDetails['sufCMA']

    # Create Src Files for Pure variants
    for paramSet in meta['paramSets']:
        impl['paramSet'] = paramSet['name']
        impl['pkSize'] = paramSet['pkSize']
        impl['skSize'] = paramSet['skSize']
        impl['sigSize'] = paramSet['sigSize']
        impl['claimedNISTLevel'] = paramSet['claimedNISTLevel']
    
        for hashAlg in meta['hashAlgs']:
            impl['hashAlg'] = hashAlg['name']
    
            src_contents = jinja2.Template(src_template).render(impl)
    
            src_file = 'slh_dsa_pure_' + impl['hashAlg'] + '_' + impl['paramSet'] + '.c'
            src_path = os.path.join(slh_wrappers_dir, 'pure', src_file)
            os.makedirs(os.path.dirname(src_path),exist_ok=True)
            
            with open(src_path, "w") as f:
                f.write(src_contents)
    
            header_contents += jinja2.Template(header_section).render(impl)
    
    # Create Src Files for Prehash variants
    impl['pure'] = False
    
    for paramSet in meta['paramSets']:
        impl['paramSet'] = paramSet['name']
        impl['pkSize'] = paramSet['pkSize']
        impl['skSize'] = paramSet['skSize']
        impl['sigSize'] = paramSet['sigSize']
        impl['claimedNISTLevel'] = paramSet['claimedNISTLevel']
    
        for hashAlg in meta['hashAlgs']:
            impl['hashAlg'] = hashAlg['name']

            for i in range(len(meta['prehashHashAlgs'])):
                prehashHashAlg = meta['prehashHashAlgs'][i]
                prehashString = prehashStrings[i]

                impl['prehashHashAlg'] = prehashHashAlg['name']
                impl['prehashString'] = prehashString['name']
    
                src_contents = jinja2.Template(src_template).render(impl)
    
                src_file = 'slh_dsa_' + impl['prehashHashAlg'] + '_prehash_' + impl['hashAlg'] + '_' + impl['paramSet'] + '.c'
                src_path = os.path.join(slh_wrappers_dir, 'prehash_' + prehashHashAlg['name'], src_file)
                os.makedirs(os.path.dirname(src_path),exist_ok=True)
    
                with open(src_path, "w") as f:
                    f.write(src_contents)
    
                header_contents += jinja2.Template(header_section).render(impl)
    
    #finish header file
    header_section = header_template[section_bound('2','#####',header_template,'START'):section_bound('2','#####',header_template,'END')]
    header_contents += jinja2.Template(header_section).render()
    header_file = "sig_slh_dsa.h"
    header_path = os.path.join(slh_dir, header_file)
    with open(header_path, "w") as f:
                f.write(header_contents)

def internal_cmake_gen():
    sources = [
        os.path.relpath(file, start=slh_dir)
        for file in glob.glob(os.path.join(slh_dir, '**', '*.c'), recursive=True)
        if 'slh_dsa_c/test/' not in os.path.relpath(file, start=slh_dir)
    ]
    sources.sort()
    prehashHashAlgs = [prehashHashAlg['name'] for prehashHashAlg in meta['prehashHashAlgs']]

    cmake_template = file_get_contents(jinja_cmake_file)
    cmake_contents = jinja2.Template(cmake_template).render({"sources": sources,
                                                            "prehashHashAlgs": prehashHashAlgs})
    cmake_file = "CMakeLists.txt"
    cmake_path = os.path.join(slh_dir, cmake_file)
    with open(cmake_path, "w") as f:
                f.write(cmake_contents)

#create list of all slh_dsa variants
def list_variants():
    variants = []
    #add pure variants
    for hashAlg, paramSet in itertools.product(meta['hashAlgs'], meta['paramSets']):
        variants.append('pure_' + hashAlg['name'] + '_' + paramSet['name'])
    #add prehash variants
    for prehashHashAlg, hashAlg, paramSet in itertools.product(meta['prehashHashAlgs'], meta['hashAlgs'], meta['paramSets']):
        variants.append(prehashHashAlg['name'] + '_prehash_' + hashAlg['name'] + '_' + paramSet['name'])
    return variants

def apply_patches(slh_patch_dir):
    for root, dirs, files in os.walk(slh_patch_dir):
        for file in files:
            full_path = os.path.join(root, file)
            subprocess.run(["git","apply",full_path], check=True)
            
def main():
    os.chdir(os.path.join(os.environ['LIBOQS_DIR']))

    #initialize globals
    global commit_hash, slh_dir, slh_dsa_c_dir, slh_dsa_temp_dir, slh_wrappers_dir, template_dir, slh_patch_dir, meta_file, \
        jinja_header_file, jinja_src_file, jinja_cmake_file, meta, prehashStrings, impl, variants, jinja_sig_c_file, \
        jinja_sig_h_file, jinja_alg_support_file, jinja_oqsconfig_file, sig_c_path, sig_h_path, \
        alg_support_path, oqsconfig_path

    # This commit hash will need to be updated
    commit_hash = "a0fc1ff253930060d0246aebca06c2538eb92b88"
    
    # internal paths
    slh_dir = os.path.join(os.environ['LIBOQS_DIR'], 'src/sig/slh_dsa')
    slh_dsa_c_dir = os.path.join(slh_dir, 'slh_dsa_c')
    slh_dsa_temp_dir = os.path.join(slh_dir, 'slh_dsa_temp')
    slh_wrappers_dir = os.path.join(slh_dir, 'wrappers')
    slh_patch_dir = os.path.join(slh_dir, 'patches')
    template_dir = os.path.join(slh_dir, 'templates')
    
    #ensure these paths exist
    os.makedirs(slh_dir,exist_ok=True)
    os.makedirs(slh_dsa_c_dir,exist_ok=True)
    os.makedirs(slh_wrappers_dir,exist_ok=True)
    os.makedirs(template_dir,exist_ok=True)
    
    # internal files
    meta_file = os.path.join(slh_dsa_temp_dir, 'integration/liboqs/META.yml')
    jinja_header_file = os.path.join(template_dir, 'slh_dsa_header_template.jinja')
    jinja_src_file = os.path.join(template_dir, 'slh_dsa_src_template.jinja')
    jinja_cmake_file = os.path.join(template_dir, 'slh_dsa_cmake_template.jinja')
    
    #copy source code from upstream
    copy_from_commit()

    #load meta file globally
    meta_file = os.path.join(slh_dsa_c_dir, 'integration/liboqs/META.yml')
    meta = file_get_contents(meta_file, encoding='utf-8')
    meta = yaml.safe_load(meta)
    prehashStrings = copy.deepcopy(meta['prehashHashAlgs'])
    for i in range(len(meta['prehashHashAlgs'])):
        meta['prehashHashAlgs'][i]['name'] = (meta['prehashHashAlgs'][i]['name']).replace("/","_")

    #Create implementation dictionary
    impl = {
      "pure": True,
      "paramSet": "",
      "hashAlg": "",
      "prehashHashAlg": "",
      "prehashString": "",
      "pkSize": "",
      "skSize": "",
      "sigSize": "",
      "algVersion": "",
      "claimedNISTLevel": "",
      "eufCMA": "",
      "sufCMA": ""
    }
    
    #Replace contents of other files using fragments
    #generate variant list
    variants = list_variants()
    
    #enumerate template file paths
    jinja_sig_c_file = os.path.join(template_dir,'slh_dsa_sig_c_template.jinja')
    jinja_sig_h_file = os.path.join(template_dir,'slh_dsa_sig_h_template.jinja')
    jinja_alg_support_file = os.path.join(template_dir,'slh_dsa_alg_support_template.jinja')
    jinja_oqsconfig_file = os.path.join(template_dir,'slh_dsa_oqsconfig_template.jinja')
    jinja_docs_yml_file = os.path.join(template_dir,'slh_dsa_docs_yml_template.jinja')
    jinja_docs_md_file = os.path.join(template_dir,'slh_dsa_docs_md_template.jinja')
    
    #enumerate destination file paths
    sig_c_path = os.path.join(os.environ['LIBOQS_DIR'],'src','sig','sig.c')
    sig_h_path = os.path.join(os.environ['LIBOQS_DIR'],'src','sig','sig.h')
    alg_support_path = os.path.join(os.environ['LIBOQS_DIR'],'.CMake','alg_support.cmake')
    oqsconfig_path = os.path.join(os.environ['LIBOQS_DIR'],'src','oqsconfig.h.cmake')
    docs_yml_path = os.path.join(os.environ['LIBOQS_DIR'],'docs','algorithms','sig','slh_dsa.yml')
    docs_md_path = os.path.join(os.environ['LIBOQS_DIR'],'docs','algorithms','sig','slh_dsa.md')
    
    #generate internal c and h files
    internal_code_gen()
    
    #generate internal cmake file
    internal_cmake_gen()
    
    #replace file contents
    file_replacer(jinja_sig_c_file, sig_c_path, {'variants': variants},'/////')
    file_replacer(jinja_sig_h_file, sig_h_path, {'variants': variants},'/////')
    file_replacer(jinja_alg_support_file, alg_support_path, {'variants': variants},'#####')
    file_replacer(jinja_oqsconfig_file, oqsconfig_path, {'variants': variants},'/////')
    
    #replace document contents
    doc_replacer(jinja_docs_yml_file, docs_yml_path)

    # apply patches
    apply_patches(slh_patch_dir)

    # NOTE: from [issue 2203](https://github.com/open-quantum-safe/liboqs/issues/2203)
    # SLH-DSA is not described in copy_from_upstream.yml. It is instead described
    # here in this separate module. This makes replacing SPHINCS+ with SLH-DSA
    # in list_standardized_algs.fragment non-trivial because this Jinja template
    # is rendered from copy_from_upstream.yml.
    # As a necessary hack, the list of variants (e.g. "pure_sha2_128s") is returned
    # so that copy_from_upstream.py can use this list to construct a dictionary
    # that resembles the structure of copy_from_upstream.yml.
    # In the near future I want to consider refactoring build configuration
    # management and upstream integration scripts. The status quo is a mess and
    # will make future integrations all the more difficult.
    return variants

if __name__ == "__main__":
    main()
