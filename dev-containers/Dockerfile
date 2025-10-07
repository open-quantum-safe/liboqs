# FROM debian:latest
FROM python:3.13.7-bookworm

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        cmake \
        ninja-build \
        libssl-dev \
        valgrind \
        astyle \
        # python3 \
        # python3-pip \
        # python3-git \
        # python3-pip \
        # python3-tabulate \
        # python3-jinja2 \
        # python3-requests \
        # python3-pytest \
        # python3-pytest-xdist \
        git \
        openssh-client \
        gcc clang g++ \
        clang \
        clang-format \
        clang-tools \
        cmake \
        curl \
        doxygen \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

COPY entrypoint.sh /opt/entrypoint.sh

RUN useradd -m -s /bin/bash oqs \
    && chmod +x /opt/entrypoint.sh \
    && chown oqs:oqs /opt/entrypoint.sh

WORKDIR /home/oqs

USER oqs

ENTRYPOINT ["/opt/entrypoint.sh"]
