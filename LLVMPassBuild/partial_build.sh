#!/usr/bin/env bash

MAST_BUILD_DIR=$PWD/DTS_BUILD
LLVM_VERSION=3.7.1
LLVM_SRC_DIR=$MAST_BUILD_DIR/llvm-$LLVM_VERSION

echo ""
echo "Copying pass sources" 1>&2
echo "into the LLVM source tree..." 1>&2
cp LLVMPassBuild/* $LLVM_SRC_DIR/projects/LLVMPassBuild/
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to copy in MAST project"
    exit 1
fi

echo ""
echo "Building only the DomTreSat Pass..." 1>&2
cd $LLVM_SRC_DIR/build
REQUIRES_RTTI=1 make
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to build project"
    exit 1
fi
