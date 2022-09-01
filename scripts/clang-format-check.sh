#!/bin/sh

errors=0
paths=$(git ls-files | grep -E '\.c?[c|h]$')

for path in $paths; do
  clang-format -Werror -n $path
  if [ $? -ne 0 ]; then
    errors=1
  fi
done

if [ $errors -ne 0 ]; then
  echo "Format errors detected, please run ./scripts/clang-format.sh to fix!"
  exit 1
fi

echo "No format error detected."