#!/bin/bash

cd Frontend/clang-plugins
./checker.sh ${1}
if ./checker.sh ${1} | grep -q 'Error'
then
    echo -e '\n\033[1;36m[COCO]\033[0m \033[1;31mPlease fix the errors in your program and try again!\n\033[0m'
else
    echo -e '\033[1;36m[COCO]\033[0m \033[1;33mYour program is data oblivious!\033[0m'
    if [ -z ${2} ]; then
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mNo optimizations applied!\033[0m'
    elif [ ${2} == fplicm ]; then
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mApplying FPLICM...\033[0m'
    ../../Backend/DOFplicm/run.sh ${1}
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mFPLICM completed \033[0m'
    elif [ ${2} == loop-unroll ]; then
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mApplying LoopUnroll...\033[0m'
    ../../Backend/LoopUnroll/run.sh ${1} 100 10
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mLoopUnroll completed\033[0m'
    elif [ ${2} == bitwise-conversion ]; then
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35m Applying Logical to Bitwise Conversion...\033[0m'
    cd ../../Backend/LogicOprator_to_Bitwise/
    ./run_all.sh ${1}
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mBitwise Conversion completed \033[0m'
    elif [ ${2} == loop-fusion ]; then
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mApplying Loop Fusion... \033[0m'
    echo -e '\033[1;36m[COCO]\033[0m \033[1;35mLoop Fusion completed[COCO] \033[0m'
    fi
fi