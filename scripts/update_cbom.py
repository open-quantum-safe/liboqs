# SPDX-License-Identifier: MIT

# This script generates a Cryptography Bill of Material (CBOM)
# according to https://github.com/IBM/CBOM/blob/main/bom-1.4-cbom-1.0.schema.json

# apt-get install npm python3-wget python3-git

import argparse
import glob
import yaml
import os
import json
import git
import uuid
from datetime import datetime
import copy
import wget

cbom_json_file = "cbom.json"
url_generate_yml = "https://raw.githubusercontent.com/open-quantum-safe/openssl/OQS-OpenSSL_1_1_1-stable/oqs-template/generate.yml"

parser = argparse.ArgumentParser()
parser.add_argument("--liboqs-root", default=".")
parser.add_argument("--liboqs-version", default="git")
args = parser.parse_args()

def load_yaml(filename, encoding='utf-8'):
    with open(filename, mode='r', encoding=encoding) as fh:
        return yaml.safe_load(fh.read())

def file_get_contents(filename, encoding=None):
    with open(filename, mode='r', encoding=encoding) as fh:
        return fh.read()

def out_write(out, str):
    out.write(str)

# Maps oqs_method -> oid from generate.yml
def get_oid_map():
    wget.download(url_generate_yml)
    ossl_generate_config = file_get_contents(os.path.join('.', 'generate.yml'), encoding='utf-8')
    ossl_generate_config = yaml.safe_load(ossl_generate_config)
    oid_map = {}
    for sig in ossl_generate_config['sigs']:
        for variant in sig['variants']:
            if 'oid' in variant:
                oid_map.update({variant['oqs_meth'] : variant['oid']})
    return oid_map

oid_map = get_oid_map()

kem_yamls = []
sig_yamls = []

cbom_components = []
bom_algs_bomrefs = []
bom_algs_use_dependencies = {}

## Common crypto components: aes, sha3
common_crypto_component_aes = {
      "type": "crypto-asset",
      "bom-ref": "alg:aes",
      "name": "aes",
      "cryptoProperties": {
        "assetType": "algorithm",
        "algorithmProperties": {
          "variant": "aes",
          "primitive": "blockcipher",
          "implementationLevel": "softwarePlainRam"
        }
      }
    }
common_crypto_component_sha3 = {
      "type": "crypto-asset",
      "bom-ref": "alg:sha3",
      "name": "sha3",
      "cryptoProperties": {
        "assetType": "algorithm",
        "algorithmProperties": {
          "variant": "sha3",
          "primitive": "hash",
          "implementationLevel": "softwarePlainRam"
        }
      }
    }

def add_cbom_component(out, kem_yaml, parameter_set):
    primitive = kem_yaml['type']

    component = {}
    component['type'] = "crypto-asset"
    if parameter_set['oqs_alg'] in oid_map.keys():
        component['bom-ref'] = "oid:" + oid_map[parameter_set['oqs_alg']]
    else:
        component['bom-ref'] = "alg:" + parameter_set['name']

    component['name'] = kem_yaml['name']

    algorithmProperties = {}
    algorithmProperties['variant'] = parameter_set['name']
    algorithmProperties['primitive'] = primitive
    algorithmProperties['implementationLevel'] = "softwarePlainRam"
    if primitive == 'kem':
        algorithmProperties['cryptoFunctions'] = ["keygen", "encapsulate", "decapsulate"]
    elif primitive == 'signature':
        algorithmProperties['cryptoFunctions'] = ["keygen", "sign", "verify"]

    cryptoProperties = {}
    cryptoProperties['assetType'] = "algorithm"
    cryptoProperties['algorithmProperties'] = algorithmProperties
    cryptoProperties['nistQuantumSecurityLevel'] = parameter_set['claimed-nist-level']
    if parameter_set['oqs_alg'] in oid_map.keys():
        cryptoProperties['oid'] = oid_map[parameter_set['oqs_alg']]

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
                    component_cpy['bom-ref'] += ":" + algorithmProperties['implementationPlatform'] 
                    cbom_components.append(component_cpy)
                    bom_algs_bomrefs.append(component_cpy['bom-ref'])
                    if dep:
                        bom_algs_use_dependencies.update({
                            component_cpy['bom-ref'] : dep
                        })



## Add KEM components
for kem_yaml_path in sorted(glob.glob(os.path.join(args.liboqs_root, 'docs', 'algorithms', 'kem', '*.yml'))):
    kem_yaml = load_yaml(kem_yaml_path)
    kem_yamls.append(kem_yaml)
    kem_name = os.path.splitext(os.path.basename(kem_yaml_path))[0]
    name = kem_yaml['name']
    for parameter_set in kem_yaml['parameter-sets']:
        add_cbom_component(None, kem_yaml, parameter_set)

## Add Sig components
for sig_yaml_path in sorted(glob.glob(os.path.join(args.liboqs_root, 'docs', 'algorithms', 'sig', '*.yml'))):
    sig_yaml = load_yaml(sig_yaml_path)
    sig_yamls.append(sig_yaml)
    sig_name = os.path.splitext(os.path.basename(sig_yaml_path))[0]
    for parameter_set in sig_yaml['parameter-sets']:
        add_cbom_component(None, sig_yaml, parameter_set)

## liboqs component
liboqs_component = {}
version = args.liboqs_version
if version == "git":
    repo = git.Repo(search_parent_directories=True)
    version = repo.head.object.hexsha
liboqs_component['type'] = "library"
liboqs_component['bom-ref'] = "pkg:github/open-quantum-safe/liboqs@" + version
liboqs_component['name'] = "liboqs"
liboqs_component['version'] = version

cbom_components.insert(0, liboqs_component)

metadata = {}
metadata['timestamp'] = datetime.now().isoformat()
metadata['component'] = liboqs_component

## Dependencies

dependencies = []
dependencies.append({
    "ref": liboqs_component['bom-ref'],
    "dependsOn": bom_algs_bomrefs,
    "dependencyType": "implements"
})
for usedep in bom_algs_use_dependencies.keys():
    dependencies.append({
        "ref": usedep,
        "dependsOn": bom_algs_use_dependencies[usedep],
        "dependencyType": "uses"
    })


## CBOM
cbom = {}
cbom['bomFormat'] = "CBOM"
cbom['specVersion'] = "1.4-cbom-1.0"
cbom['serialNumber'] = "urn:uuid:" + str(uuid.uuid4())
cbom['version'] = 1
cbom['metadata'] = metadata
cbom['components'] = cbom_components + [common_crypto_component_aes, common_crypto_component_sha3]
cbom['dependencies'] = dependencies

with open(os.path.join(args.liboqs_root, '.cbom', cbom_json_file), mode='w', encoding='utf-8') as out_md:
    out_md.write(json.dumps(cbom, indent=2))
