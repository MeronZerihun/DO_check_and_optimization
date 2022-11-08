cd build
cmake ../clang-plugins-demo
make
clang -fplugin=lib/libfunction-printer-plugin.so -c ../clang-plugins-demo/test/functions.cpp
# bin/print-functions -- clang -c ../clang-plugins-demo/test/functions.c