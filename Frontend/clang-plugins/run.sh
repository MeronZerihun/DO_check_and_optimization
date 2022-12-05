cd build
cmake ../clang-plugins-checker
make
echo "Checking file..."
# clang -fplugin=lib/libfunction-printer-plugin.so -c ../clang-plugins-checker/test/functions.cpp
bin/print-functions -- clang -c -g ../clang-plugins-checker/test/functions.cpp