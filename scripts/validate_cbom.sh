#!/bin/sh
# SPDX-License-Identifier: MIT

cd "$(dirname "$0")"
wget -nc https://raw.githubusercontent.com/CycloneDX/specification/1.6/schema/bom-1.6.schema.json
wget -nc https://raw.githubusercontent.com/CycloneDX/specification/1.6/schema/spdx.schema.json
wget -nc https://raw.githubusercontent.com/CycloneDX/specification/1.6/schema/jsf-0.82.schema.json
ajv validate --spec=draft7 --validate-formats=false -r spdx.schema.json -r jsf-0.82.schema.json --strict=false -s bom-1.6.schema.json -d ../docs/cbom.json