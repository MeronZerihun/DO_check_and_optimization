cd build
cmake ../clang-plugins-checker
make
clang -fplugin=lib/libfunction-printer-plugin.so -c ../clang-plugins-checker/test/functions.cpp
# bin/print-functions -- clang -c ../clang-plugins-demo/test/functions.c