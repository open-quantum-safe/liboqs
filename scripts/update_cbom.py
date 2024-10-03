# SPDX-License-Identifier: MIT

# This script generates a Cryptography Bill of Material (CBOM)
# according to https://github.com/IBM/CBOM/blob/main/bom-1.4-cbom-1.0.schema.json

# apt-get install npm python3-git

import argparse
import glob
import yaml
import os
import json
import git
import uuid
import datetime
import copy

cbom_json_file = "cbom.json"


def load_yaml(filename, encoding='utf-8'):
    with open(filename, mode='r', encoding=encoding) as fh:
        return yaml.safe_load(fh.read())

def file_get_contents(filename, encoding=None):
    with open(filename, mode='r', encoding=encoding) as fh:
        return fh.read()

def out_write(out, str):
    out.write(str)

kem_yamls = []
sig_yamls = []

cbom_components = []
bom_algs_bomrefs = []
bom_algs_use_dependencies = {}

## Common crypto components: aes, sha3
common_crypto_component_aes = {
      "type": "cryptographic-asset",
      "bom-ref": "alg:aes",
      "name": "aes",
      "cryptoProperties": {
        "assetType": "algorithm",
        "algorithmProperties": {
          "primitive": "block-cipher",
          "executionEnvironment": "software-plain-ram"
        }
      }
    }
common_crypto_component_sha3 = {
      "type": "cryptographic-asset",
      "bom-ref": "alg:sha3",
      "name": "sha3",
      "cryptoProperties": {
        "assetType": "algorithm",
        "algorithmProperties": {
          "primitive": "hash",
          "executionEnvironment": "software-plain-ram"
        }
      }
    }

def add_cbom_component(out, kem_yaml, parameter_set):
    primitive = kem_yaml['type']

    component = {}
    component['type'] = "cryptographic-asset"
    component['bom-ref'] = "alg:" + parameter_set['name']

    component['name'] = kem_yaml['name']

    algorithmProperties = {}
    algorithmProperties['parameterSetIdentifier'] = parameter_set['name']
    algorithmProperties['primitive'] = primitive
    algorithmProperties['executionEnvironment'] = "software-plain-ram"
    if primitive == 'kem':
        algorithmProperties['cryptoFunctions'] = ["keygen", "encapsulate", "decapsulate"]
    elif primitive == 'signature':
        algorithmProperties['cryptoFunctions'] = ["keygen", "sign", "verify"]
    algorithmProperties['nistQuantumSecurityLevel'] = parameter_set['claimed-nist-level']

    cryptoProperties = {}
    cryptoProperties['assetType'] = "algorithm"
    cryptoProperties['algorithmProperties'] = algorithmProperties

    component['cryptoProperties'] = cryptoProperties

    for impl in parameter_set['implementations']:
        dic = {
            "all": "generic",
            "x86_64": "x86_64",
            "ARM64_V8": "armv8-a"
        }
        dep = []
        if 'common-crypto' in impl:
            for a in impl['common-crypto']:
                if "SHA3" in a:
                    dep.append(common_crypto_component_sha3['bom-ref'])
                elif "AES" in a:
                    dep.append(common_crypto_component_aes['bom-ref'])

        if impl['supported-platforms'] == "all":
            algorithmProperties['implementationPlatform'] = dic[impl['supported-platforms']]
            component_cpy = copy.deepcopy(component)
            component_cpy['bom-ref'] += ":" + algorithmProperties['implementationPlatform'] 
            cbom_components.append(component_cpy)
            bom_algs_bomrefs.append(component_cpy['bom-ref'])
            if (dep):
                bom_algs_use_dependencies.update({
                    component_cpy['bom-ref'] : dep
                })
        else:
            for plat in impl['supported-platforms']:
                if plat['architecture'] in dic.keys():
                    algorithmProperties['implementationPlatform'] = dic[plat['architecture']]
                    component_cpy = copy.deepcopy(component)
                    if 'upstream' in impl and impl['upstream'] == 'libjade':
                        tag = ":jasmin:"
                        if any('required_flags' in i for i in impl['supported-platforms']):
                            tag += impl['upstream-id'] + ':'
                        component_cpy['bom-ref'] += tag + algorithmProperties['implementationPlatform'] 
                    else:
                        component_cpy['bom-ref'] += ":" + algorithmProperties['implementationPlatform'] 
                    cbom_components.append(component_cpy)
                    bom_algs_bomrefs.append(component_cpy['bom-ref'])
                    if dep:
                        bom_algs_use_dependencies.update({
                            component_cpy['bom-ref'] : dep
                        })

def build_cbom(liboqs_root, liboqs_version):
    ## Add KEM components
    for kem_yaml_path in sorted(glob.glob(os.path.join(liboqs_root, 'docs', 'algorithms', 'kem', '*.yml'))):
        kem_yaml = load_yaml(kem_yaml_path)
        kem_yamls.append(kem_yaml)
        kem_name = os.path.splitext(os.path.basename(kem_yaml_path))[0]
        name = kem_yaml['name']
        for parameter_set in kem_yaml['parameter-sets']:
            add_cbom_component(None, kem_yaml, parameter_set)

    ## Add Sig components
    for sig_yaml_path in sorted(glob.glob(os.path.join(liboqs_root, 'docs', 'algorithms', 'sig', '*.yml'))):
        sig_yaml = load_yaml(sig_yaml_path)
        sig_yamls.append(sig_yaml)
        sig_name = os.path.splitext(os.path.basename(sig_yaml_path))[0]
        for parameter_set in sig_yaml['parameter-sets']:
            add_cbom_component(None, sig_yaml, parameter_set)

    ## liboqs component
    liboqs_component = {}
    version = liboqs_version
    if version == "git":
        repo = git.Repo(search_parent_directories=True, odbt=git.GitDB)
        version = repo.head.object.hexsha
    liboqs_component['type'] = "library"
    liboqs_component['bom-ref'] = "pkg:github/open-quantum-safe/liboqs@" + version
    liboqs_component['name'] = "liboqs"
    liboqs_component['version'] = version

    cbom_components.insert(0, liboqs_component)

    metadata = {}
    metadata['timestamp'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    metadata['component'] = liboqs_component

    ## Dependencies

    dependencies = []
    dependencies.append({
        "ref": liboqs_component['bom-ref'],
        "provides": bom_algs_bomrefs
    })
    for usedep in bom_algs_use_dependencies.keys():
        dependencies.append({
            "ref": usedep,
            "dependsOn": bom_algs_use_dependencies[usedep]
        })


    ## CBOM
    cbom = {}
    cbom['$schema'] = "https://raw.githubusercontent.com/CycloneDX/specification/1.6/schema/bom-1.6.schema.json"
    cbom['bomFormat'] = "CycloneDX"
    cbom['specVersion'] = "1.6"
    cbom['serialNumber'] = "urn:uuid:" + str(uuid.uuid4())
    cbom['version'] = 1
    cbom['metadata'] = metadata
    cbom['components'] = cbom_components + [common_crypto_component_aes, common_crypto_component_sha3]
    cbom['dependencies'] = dependencies
    return cbom
    

def algorithms_changed(cbom, cbom_path):
    if os.path.isfile(cbom_path):
        with open(cbom_path, mode='r', encoding='utf-8') as c:
            existing_cbom = json.load(c)
            existing_cbom['serialNumber'] = cbom['serialNumber']
            existing_cbom['metadata']['timestamp'] = cbom['metadata']['timestamp']
            existing_cbom['metadata']['component']['bom-ref'] = cbom['metadata']['component']['bom-ref']
            existing_cbom['metadata']['component']['version'] = cbom['metadata']['component']['version']
            existing_cbom['components'][0]['bom-ref'] = cbom['components'][0]['bom-ref']
            existing_cbom['components'][0]['version'] = cbom['components'][0]['version']
            existing_cbom['dependencies'][0]['ref'] = cbom['dependencies'][0]['ref']
            update_cbom = existing_cbom != cbom
            c.close()
            return update_cbom
    else:
        return True

def update_cbom_if_algs_not_changed(liboqs_root, liboqs_version):
    cbom_path = os.path.join(liboqs_root, 'docs', cbom_json_file)
    cbom = build_cbom(liboqs_root, liboqs_version)
    if algorithms_changed(cbom, cbom_path):
        with open(cbom_path, mode='w', encoding='utf-8') as out_md:
            out_md.write(json.dumps(cbom, indent=2))
            out_md.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--liboqs-root", default=".")
    parser.add_argument("--liboqs-version", default="git")
    args = parser.parse_args()
    update_cbom_if_algs_not_changed(args.liboqs_root, args.liboqs_version)