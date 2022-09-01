#!/bin/sh

errors=0
paths=$(git ls-files | grep -E '\.c?[c|h]$')

for path in $paths; do
  cpplint --quiet $path
  if [ $? -ne 0 ]; then
    errors=1
  fi
done

if [ $errors -ne 0 ]; then
  echo "Cpplint errors detected, please fix them!"
  exit 1
fi

echo "No format error detected."