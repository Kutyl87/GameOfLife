FROM ubuntu:latest
RUN apt-get update && apt-get install -y \
    git \
    g++ \
    cmake \
    make \
    scons \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /usr/ZPRProject
COPY . .
