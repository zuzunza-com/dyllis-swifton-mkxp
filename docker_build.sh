#!/bin/bash
set -e

# Set optimization level
export CFLAGS="-O2 -g0"
export CXXFLAGS="-O2 -g0"
export CPPFLAGS="-O2 -g0"
export LDFLAGS="-O2 -g0"

mkdir -p deps
cd deps

# Get libsigc++
if [ ! -d "libsigc++" ]; then
    wget https://github.com/libsigcplusplus/libsigcplusplus/releases/download/2.12.0/libsigc++-2.12.0.tar.xz -O libsigc++.tar.xz
    tar xf libsigc++.tar.xz && rm libsigc++.tar.xz
    mv libsigc++* libsigc++
fi

# Get pixman
if [ ! -d "pixman" ]; then
    wget https://www.cairographics.org/releases/pixman-0.42.0.tar.gz -O pixman.tar.gz
    tar xf pixman.tar.gz && rm pixman.tar.gz
    mv pixman* pixman
fi

# Get physfs
if [ ! -d "physfs" ]; then
    wget https://icculus.org/physfs/downloads/physfs-3.0.2.tar.bz2 -O physfs.tar.bz2
    tar xf physfs.tar.bz2 && rm physfs.tar.bz2
    mv physfs* physfs
fi

# Get mruby
if [ ! -d "mruby" ]; then
    wget https://github.com/mruby/mruby/archive/2.1.2.tar.gz -O mruby.tar.gz
    tar xf mruby.tar.gz && rm mruby.tar.gz
    mv mruby* mruby
fi

# NOTE: emsdk is assumed to be in the environment (docker base image)

# Build libsigc++
if [ ! -f "libsigc++/sigc++/.libs/libsigc-2.0.a" ]; then
    cd libsigc++
    emconfigure ./autogen.sh
    emconfigure ./configure --enable-static --disable-shared
    emmake make clean
    emmake make -j1 || true
    cd ..
fi

# Build pixman
if [ ! -f "pixman/pixman/.libs/libpixman-1.a" ]; then
    cd pixman
    emconfigure ./configure --enable-static --disable-shared
    emmake make clean
    cd pixman
    emmake make -j1 libpixman-1.la
    cd ../..
fi

# Build physfs
if [ ! -f "physfs/libphysfs.a" ]; then
    cd physfs
    emcmake cmake .
    emmake make clean
    emmake make -j1 physfs-static
    cd ..
fi

# Build mruby
if [ ! -f "mruby/build/wasm32-unknown-gnu/lib/libmruby.a" ]; then
    cd mruby
    cp ../../extra/build_config.rb ../../extra/vm.c.patch ./
    if patch -R -p0 -s -f --dry-run < vm.c.patch; then
        echo "Patch already applied"
    else
        patch -p0 < vm.c.patch
    fi
    make clean
    make -j1
    cd ..
fi

echo "Finished building dependencies"
cd ..

# Build mkxp
rm -f CMakeCache.txt
emcmake cmake .
emmake make -j1

echo "Finished building MKXP"

# Copy to build directory
mkdir -p build
cp -R mkxp.html mkxp.wasm mkxp.js extra/*.webmanifest extra/js build/

# Copy scripts needed for runtime processing
cp extra/rgss.rb build/
cp extra/make_mapping.sh build/
cp extra/dump.sh build/
cp extra/dump_rgss.rb build/
cp extra/dump.rb build/
