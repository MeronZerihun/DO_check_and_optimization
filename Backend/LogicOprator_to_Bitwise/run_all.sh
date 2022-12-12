cd build
make clean
cmake ..
make 
echo -e "Pass build completed \n"
cd ..
cd benchmark
echo -e "Running Optimization ...\n"
bash run.sh ${1} 
bash viz.sh ${1}
bash viz.sh ${1}.optimized
cd dot
# delete these if they don't work, I am not sure if they do
# xdg-open LogicOperators_to_BitwiseCorrectness.cfg.pdf 
# xdg-open LogicOperators_to_BitwiseCorrectness.optimized.cfg.pdf