# COCO: A Compiler-based Obliviousness Checker and Optimizer
To check a C++ program's obliviousness using **COCO**, add the program to the **Frontend/clang-plugins/clang-plugins-checker/test/** directory.

To run the checker, execute the following command on the terminal:
```console
$ ./run.sh <program-name> <optimization>
```

The available optimization options are: 
- *fplicm*: Frequent-Path Loop Invariant Code Motion (FPLICM)
- *loop-unroll*: Loop Unrolling
- *bitwise-conversion*: Logical Operator to Bitwise Operator Conversion

To run the program test1.cpp and apply the FPLICM optimization:
```console
$ ./run.sh test1 fplicm
```
