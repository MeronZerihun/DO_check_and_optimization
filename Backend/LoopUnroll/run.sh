echo -e "**********************************************"
echo -e "correctness check..."
cd benchmark/
bash run_correctness.sh LoopUnrollCorrectness 10


echo -e "**********************************************"
echo running testing loop unrolling ...
echo -e "**********************************************"
bash run.sh ${1} ${2} ${3}
echo Done
echo -e "**********************************************"
cd ..
cd Pass/
g++ find_opt_UF.cpp -o f
echo -e "optimal UF: "
./f
echo -e "**********************************************"
X=`./f`
cd ..
cd benchmark/
bash run_final.sh ${1} ${X}
echo -e "DONE"

