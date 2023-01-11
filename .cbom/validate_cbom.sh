#!/bin/sh
# SPDX-License-Identifier: MIT

cd "$(dirname "$0")"
wget -nc https://raw.githubusercontent.com/IBM/CBOM/main/bom-1.4-cbom-1.0.schema.json
wget -nc https://raw.githubusercontent.com/IBM/CBOM/main/spdx.schema.json
wget -nc https://raw.githubusercontent.com/IBM/CBOM/main/jsf-0.82.schema.json
ajv validate --spec=draft7 --validate-formats=false -r spdx.schema.json -r jsf-0.82.schema.json --strict=false -s bom-1.4-cbom-1.0.schema.json -d cbom.json
