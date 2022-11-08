mkdir build && cd build
cmake ../clang-plugins-demo
make
bin/print-functions -- clang -c ../clang-plugins-demo/test/functions.c