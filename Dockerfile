FROM emscripten/emsdk:latest

# Install build dependencies
RUN apt-get update && apt-get install -y \
    git \
    wget \
    zip \
    unzip \
    ruby \
    rake \
    automake \
    libtool \
    pkg-config \
    mm-common \
    xz-utils \
    bzip2 \
    imagemagick \
    file \
    xxd \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /src

RUN mkdir -p /input /output /src/build

# We will mount the source code and deps via volumes.
# Entrypoint will handle building if necessary.
ENTRYPOINT ["/bin/bash", "/src/process_game.sh"]
