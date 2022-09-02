#!/bin/sh

root=$(dirname $0)/..
builddir=build

pushd $root
  cmake -S . -B $builddir -DCMAKE_BUILD_TYPE=Release || exit 1
  cmake --build $builddir -j || exit 1
popd