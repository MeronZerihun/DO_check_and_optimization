#!/bin/bash

cd build
cmake ../clang-plugins-checker
make
echo "Checking file..."
# clang -fplugin=lib/libfunction-printer-plugin.so -c ../clang-plugins-checker/test/functions.cpp
# bin/print-functions -- clang -c ../clang-plugins-checker/test/${1}.cpp
bin/print-functions -- clang -I/usr/local/lib/clang/14.0.6/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include -c ../clang-plugins-checker/test/${1}.cpp

# bin/print-functions