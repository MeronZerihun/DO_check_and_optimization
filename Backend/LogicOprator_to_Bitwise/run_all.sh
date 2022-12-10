cd build
make clean
cmake ..
make 
echo -e "Pass build completed \n"
cd ..
cd benchmark
echo -e "Running Optimization ...\n"
bash run.sh LogicOperators_to_BitwiseCorrectness 
bash viz.sh LogicOperators_to_BitwiseCorrectness
bash viz.sh LogicOperators_to_BitwiseCorrectness.optimized
cd dot
# delete these if they don't work, I am not sure if they do
xdg-open LogicOperators_to_BitwiseCorrectness.cfg.pdf 
xdg-open LogicOperators_to_BitwiseCorrectness.optimized.cfg.pdf