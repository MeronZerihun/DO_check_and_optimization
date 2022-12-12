cd ../../Backend/LoopUnroll/benchmark/

echo -e "**********************************************"
echo Testing loop unrolling ...
echo -e "**********************************************"
bash run.sh ${1} ${2} ${3}
echo Done
echo -e "**********************************************"
cd ../Pass/
g++ find_opt_UF.cpp -o f
echo -e "Optimal Unroll Factor: "
./f
echo -e "**********************************************"
X=`./f`
cd ../benchmark/
bash run_final.sh ${1} ${X}
echo -e "DONE"

