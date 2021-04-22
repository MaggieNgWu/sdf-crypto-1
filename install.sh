#!/bin/bash
mkdir build
cd build
cmake ..
make
cd ..
mkdir includes
cp deps/srs/libsdf/NF2180M3/klin_v10/csmsds.h  includes/
cp sdf/SDFCryptoProvider.h  includes/
mkdir lib
cp build/sdf/libsdf-crypto_arm_static.a lib/libsdf-crypto_arm.a
cp build/bin/libsdf-crypto_arm.so lib/libsdf-crypto_arm.so 