FROM ubuntu:latest
RUN apt update && apt upgrade -y  && apt-get install -y \
    git \
    g++ \
    cmake \
    make \
    scons \
    wget \
    unzip \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/ZPRProject
COPY . .
