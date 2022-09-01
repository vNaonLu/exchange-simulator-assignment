#!/bin/sh

errors=0
paths=$(git ls-files | grep -E '\.c?[c|h]$')

for path in $path; do
  cpplint --quiet $path
done

if [ $errors -ne 0 ]; then
  echo "Cpplint errors detected, please fix them!"
  exit 1
fi

echo "No format error detected."