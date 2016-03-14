#!/usr/bin/env bash

echo "[PATCH] Applying patches to llvm/include/llvm/InitializePasses.h"
patch llvm-3.7.1/include/llvm/InitializePasses.h < patches/InitializePasses.patch
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to apply InitializePasses patch"
    exit 1
fi

echo "[PATCH] Applying patches to llvm/tools/CMakeLists.txt"
patch llvm-3.7.1/tools/CMakeLists.txt < patches/build_less_tools.patch
if [ "$?" -ne "0" ]
then
    echo "[!] Failed to apply CMakeLists patch"
    #exit 1
fi
