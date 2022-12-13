#!/bin/bash

cd build
cmake ../clang-plugins-checker
make
display=$(cat ../coco.txt)
echo -e "\033[1;36m ${display} \033[0m" 

echo -e "\n\n\033[1;36m[COCO]\033[0m \033[1;32mChecking file: ${1} \033[0m\n"
# clang -fplugin=lib/libfunction-printer-plugin.so -c ../clang-plugins-checker/test/functions.cpp
# bin/print-functions -- clang -c ../clang-plugins-checker/test/${1}.cpp
bin/print-functions -- clang -I/usr/local/lib/clang/14.0.6/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include -c ../clang-plugins-checker/test/${1}.cpp

# bin/print-functions