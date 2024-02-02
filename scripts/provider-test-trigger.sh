#!/bin/bash

# SPDX-License-Identifier: MIT

# Trigger the oqs-provider release tests in CI.

# Variables:
# ACCESS_TOKEN: a GitHub Personal Access Token with repo and workflow permissions. Required.
# LIBOQS_REF: the liboqs branch or tag on which to run. Defaults to "main" if not specified.
# PROVIDER_REF: the oqs-provider branch or tag on which to run. Defaults to "main" if not specified.
# CURL_FLAGS: additional flags (e.g., "--silent") to pass to the curl command

if [ -z $ACCESS_TOKEN ]; then
    echo "This script requires a GitHub Personal Access Token with repo and workflow permissions."
    exit 1
fi

# default to running on liboqs main / provider main
if [ -z $LIBOQS_REF ]; then
    export LIBOQS_REF="main"
fi
if [ -z $PROVIDER_REF ]; then
    export PROVIDER_REF="main"
fi

curl $CURL_FLAGS \
    --request POST \
    --header "Accept: application/vnd.github+json" \
    --header "Authorization: Bearer $ACCESS_TOKEN" \
    --header "X-GitHub-Api-Version: 2022-11-28" \
    --data "{
              \"event_type\": \"liboqs-release\",
              \"client_payload\": {
                                  \"liboqs_ref\": \"$LIBOQS_REF\",
                                  \"provider_ref\": \"$PROVIDER_REF\"
              }
            }" \
    https://api.github.com/repos/open-quantum-safe/oqs-provider/dispatches
