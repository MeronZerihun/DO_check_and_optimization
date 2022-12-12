#!/bin/bash

cd Frontend/clang-plugins

./checker.sh ${1}
if ./checker.sh ${1} | grep -q 'Error'
then
    echo 'Please fix the errors in your program and try again!'
else
    echo 'Your program is data oblivious!'
    if [ -z ${2} ]; then
    echo 'No optimizations applied!'
    elif [ ${2} == fplicm ]; then
    echo 'Applying FPLICM...'
    ../../Backend/DOFplicm/run.sh ${1}
    echo 'FPLICM completed'
    elif [ ${2} == loop-unroll ]; then
    echo 'Applying LoopUnroll...'
    ../../Backend/LoopUnroll/run.sh ${1} 100 10
    echo 'LoopUnroll completed'
    elif [ ${2} == LogicOperators_to_BitwiseCorrectness ]; then
    echo 'Applying Logical to Bitwise Conversion...'
    cd ../../Backend/LogicOprator_to_Bitwise/
    ./run_all.sh LogicOperators_to_BitwiseCorrectness
    echo 'Bitwise Conversion completed'
    elif [ ${2} == loop-fusion ]; then
    echo 'Applying Loop Fusion...'
    echo 'Loop Fusion completed'
    fi
fi