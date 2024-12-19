# To run launch something like
# docker build -t cpp-arm64-build .
# docker run --rm cpp-arm64-build

# Use an Ubuntu base image
FROM ubuntu:22.04

# Set environment variables for non-interactive installs
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    make \
    gcc \
    g++ \
    libi2c-dev \
    python3-venv \
    libasio-dev \
    nlohmann-json3-dev \
    gdb \
    gdb-multiarch \
    qemu-user-static \
    binutils-aarch64-linux-gnu \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    openssh-client \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set up working directory in the container
WORKDIR /usr/src/app

# Copy everything from one level up into the container
COPY ../ .

# Set the working directory to the location of the Dockerfile and Makefile
WORKDIR /usr/src/app/CDFR2025-Programme-Robot

# Set cross-compilation flags
ENV CC=aarch64-linux-gnu-gcc
ENV CXX=aarch64-linux-gnu-g++

# Compile the program
RUN make
RUN make tests

# Need to copy the exec back to the host