FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
       build-essential tar curl sudo wget gnupg \
       tmux cmake ninja-build pkg-config \
 && rm -rf /var/lib/apt/lists/*

# compile capnproto
RUN curl -O https://capnproto.org/capnproto-c++-0.8.0.tar.gz && \
    tar zxf capnproto-c++-0.8.0.tar.gz && \
    cd capnproto-c++-0.8.0 && \
    ./configure && \
    make -j6 && \
    sudo make install

# LLVM 11 repo
RUN echo 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal main' > /etc/apt/sources.list.d/llvm11.list && \
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN apt-get update && apt-get install -y \
       clang-11 \
       clang-tidy-11 \
       libc++-11-dev \
       libc++abi-11-dev \
 && rm -rf /var/lib/apt/lists/*
RUN ln -s /usr/bin/clang++-11 /usr/bin/clang++

RUN apt-get update && apt-get install -y \
       python3-pip && \
 pip3 install --no-cache-dir pandas matplotlib \
 && rm -rf /var/lib/apt/lists/*
