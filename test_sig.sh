#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building Docker image for OQS sigs test...${NC}"
docker build --progress=plain \
    -f test_sig.Dockerfile -t liboqs-sig-test .
    # --platform=linux/amd64 \

if [ $? -ne 0 ]; then
    echo -e "${RED}Docker build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Running OQS sigs test in container...${NC}"
docker run --rm liboqs-sig-test

# Optional: Run with interactive shell for debugging
if [ "$1" == "--debug" ]; then
    echo -e "${GREEN}Starting interactive debugging session...${NC}"
    docker run --rm -it liboqs-sigs-test /bin/bash
fi

# Optional: Clean up the image after testing
if [ "$1" == "--clean" ]; then
    echo -e "${GREEN}Cleaning up Docker image...${NC}"
    docker rmi liboqs-sig-test
fi