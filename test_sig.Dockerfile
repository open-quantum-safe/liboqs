FROM ubuntu:22.04
RUN apt-get update && apt-get install -y \
    cmake gcc g++ ninja-build libssl-dev git \
    python3-pytest python3-pytest-xdist python3-yaml
WORKDIR /liboqs
COPY . .
RUN mkdir build && cd build && cmake -GNinja .. && ninja
CMD ["./build/tests/test_sig", "Dilithium3"]