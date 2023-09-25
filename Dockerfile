FROM ubuntu:jammy 

ARG number_of_threads=2

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
    libeigen3-dev && \
    rm -rf /var/lib/apt/lists/*

RUN pip install setuptools==68.2.2

RUN git clone --branch 14.25 --single-branch --depth 1 https://github.com/moves-rwth/carl-storm.git /base/carl-storm && \
    git clone --branch 2.2.0 --single-branch --depth 1 https://github.com/moves-rwth/pycarl.git /base/pycarl && \
    git clone --branch 1.8.1 --single-branch --depth 1 https://github.com/moves-rwth/storm.git /base/storm && \
    git clone --branch 1.8.0 --single-branch --depth 1 https://github.com/moves-rwth/stormpy.git /base/stormpy

# Build CArL-storm
WORKDIR /base/carl-storm/build
RUN cmake .. && \
    make -j$number_of_threads lib_carl

# Install Pycarl
WORKDIR /base/pycarl
RUN python3 setup.py build_ext --jobs $number_of_threads develop

# Build Storm
WORKDIR /base/storm/build
RUN cmake .. && \
    make -j$number_of_threads binaries

# Install Stormpy
WORKDIR /base/stormpy
RUN python3 setup.py build_ext --jobs $number_of_threads develop

# Setup jajapy
WORKDIR /base/jajapy
COPY ./requirements.txt /base/jajapy/requirements.txt
COPY ./dev_requirements.txt /base/jajapy/dev_requirements.txt
RUN pip install -r requirements.txt -r dev_requirements.txt
COPY . .
