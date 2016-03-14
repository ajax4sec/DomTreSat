#!/usr/bin/env bash

ROOT=$PWD
DTS_BUILD_DIR=$PWD/DTS_BUILD
mkdir -p $DTS_BUILD_DIR

LLVM_VERSION=3.7.1
LLVM_DOWNLOAD_PATH="http://llvm.org/releases/$LLVM_VERSION/"
LLVM_SRC_DIR=$DTS_BUILD_DIR/llvm-$LLVM_VERSION


echo ""
echo "Downloading llvm $LLVM_version"
wget $LLVM_DOWNLOAD_PATH"llvm-"$LLVM_VERSION".src.tar.xz";
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to download LLVM"
    exit 1
fi
echo ""
echo "Downloading clang $LLVM_version"
wget $LLVM_DOWNLOAD_PATH"cfe-"$LLVM_VERSION".src.tar.xz";
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to download LLVM"
    exit 1
fi
echo ""
echo "Downloading compiler-rt $LLVM_version"
wget $LLVM_DOWNLOAD_PATH"compiler-rt-"$LLVM_VERSION".src.tar.xz";
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to download LLVM"
    exit 1
fi

echo ""
echo "Unpacking LLVM..." 1>&2
tar xf llvm-$LLVM_VERSION.src.tar.xz
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to unpack LLVM"
    exit 1
fi

echo "Moving LLVM..." 1>&2
mv llvm-$LLVM_VERSION.src $LLVM_SRC_DIR
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to move src directory"
    exit 1
fi

echo ""
echo "Unpacking Clang into tools..." 1>&2
tar -xf cfe-$LLVM_VERSION.src.tar.xz
mv cfe-$LLVM_VERSION.src $LLVM_SRC_DIR/tools/clang


echo ""
echo "Unpacking Compiler-RT into projects..." 1>&2
tar -xf compiler-rt-$LLVM_VERSION.src.tar.xz
mv compiler-rt-$LLVM_VERSION.src $LLVM_SRC_DIR/projects/compiler-rt




echo ""
echo "Copying DTS pass sources" 1>&2
echo "into LLVM source tree..." 1>&2
cp -r LLVMPassBuild $LLVM_SRC_DIR/projects/
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to copy in DTS project"
    exit 1
fi

mkdir $LLVM_SRC_DIR/build
cd $LLVM_SRC_DIR/build
LLVM_BUILD_DIR=$PWD
export CC=gcc
export CXX=g++
../configure --enable-optimized 

REQUIRES_RTTI=1 make -j5
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to build project"
fi

exit

echo ""
echo "Installing LLVM..." 1>&2
mkdir $DTS_BUILD_DIR/build
cd $DTS_BUILD_DIR/build



LLVM_BUILD_DIR=$PWD
export CC=gcc
export CXX=g++
cmake -DLLVM_INCLUDE_TOOLS=ON \
    -DLLVM_BUILD_TOOLS=ON \
    -DLLVM_INCLUDE_UTILS=OFF \
    -DLLVM_BUILD_EXAMPLES=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_BUILD_DOCS=OFF \
    -DLLVM_INCLUDE_DOCS=OFF \
    -DLLVM_TARGETS_TO_BUILD="X86;ARM;AArch64" \
    -DCMAKE_BUILD_TYPE=Release $LLVM_SRC_DIR
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to run CMake"
fi


