
project(clang-plugins-demo-tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(testlib
  functions.c
  hw2perf1.cpp
)

