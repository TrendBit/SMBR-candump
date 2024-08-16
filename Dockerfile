FROM debian:bookworm-slim

# Install necessary tools and libraries for cross-compiling to ARM64
RUN apt-get update && apt-get install -y \
    build-essential \
    g++-aarch64-linux-gnu \
    libsocketcan-dev

# Set the cross-compiler environment variables
ENV CC=aarch64-linux-gnu-gcc
ENV CXX=aarch64-linux-gnu-g++
ENV PATH="/usr/bin:${PATH}"

WORKDIR /project
