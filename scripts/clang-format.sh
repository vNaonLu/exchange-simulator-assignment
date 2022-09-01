#!/bin/sh

paths=$(git ls-files | grep -E '\.c?[c|h]$')

for path in $paths; do
  clang-format -i --verbose $path
done
