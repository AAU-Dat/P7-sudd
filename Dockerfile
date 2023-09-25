FROM ubuntu:jammy 

WORKDIR /base

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    cmake \
    clang \
    libboost-all-dev \
    build-essential \
    git \
    libcln-dev \
    libgmp-dev \
    libginac-dev \
    automake \
    libglpk-dev \
    libhwloc-dev \
    libz3-dev \
    libxerces-c-dev \
    libeigen3-dev

RUN pip install setuptools==68.2.2

RUN git clone --branch 14.25 --single-branch --depth 1 https://github.com/moves-rwth/carl-storm.git /base/carl-storm && \
    git clone --branch 2.2.0 --single-branch --depth 1 https://github.com/moves-rwth/pycarl.git /base/pycarl && \
    git clone --branch 1.8.1 --single-branch --depth 1 https://github.com/moves-rwth/storm.git /base/storm && \
    git clone --branch 1.8.0 --single-branch --depth 1 https://github.com/moves-rwth/stormpy.git /base/stormpy

# Build CArL-storm
WORKDIR /base/carl-storm/build
RUN cmake .. && \
    make lib_carl

# Install Pycarl
WORKDIR /base/pycarl
RUN python3 setup.py develop

# Build Storm
WORKDIR /base/storm/build
RUN cmake .. && \
    make binaries

# Install Stormpy
WORKDIR /base/stormpy
RUN python3 setup.py build_ext --jobs 2 develop

# Copy the application source code (This should always be the last step)
WORKDIR /base/jajapy
COPY . /base/jajapy
RUN pip install -r requirements.txt
RUN pip install pytest==7.4.2 # TODO this should be a part of the requirements.txt, or maybe a dev_requirements.txt
