### Instructions to build and run the Docker container:
    
# docker build -t minilang .
# docker run --rm -it -v $(pwd):/usr/src/app minilang
# make parser
# ./parser <path_to_test_file>


# Use latest Ubuntu as base
FROM ubuntu:24.04

# Prevent prompts during install
ENV DEBIAN_FRONTEND=noninteractive

# Install essential tools
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        wget \
        cmake \
        flex \
        bison \
        git \
        g++ \
        clang \
        ninja-build \
        pkg-config \
        libstdc++-12-dev \
        libfl-dev \
        && rm -rf /var/lib/apt/lists/*

# Install latest Bison from source
RUN wget https://ftp.gnu.org/gnu/bison/bison-3.8.2.tar.gz && \
    tar -xzf bison-3.8.2.tar.gz && \
    cd bison-3.8.2 && \
    ./configure --prefix=/usr/local && \
    make -j$(nproc) && make install && \
    cd .. && rm -rf bison-3.8.2 bison-3.8.2.tar.gz

# Create app directory
WORKDIR /usr/src/app

# Copy source code
COPY . .

# Default build command
CMD ["bash"]