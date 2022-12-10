PATH2LIB=~/583Project/Backend/LogicOprator_to_Bitwise/build/Pass/ProjectBackend1.so
PASS=-to_bitwisePass



# clean up previous runs
# rm -f default.profraw ${1}_prof ${1} ${1}_baseline *.bc ${1}.profdata *_output *.ll

clang -emit-llvm -c ${1}.c -o ${1}.bc 
# Instrument profiler
opt -enable-new-pm=0 -pgo-instr-gen -instrprof ${1}.bc -o ${1}.prof.bc
clang -fprofile-instr-generate ${1}.prof.bc -o ${1}_prof
./${1}_prof >correct_output
# Translate raw profiling data into LLVM data format
llvm-profdata merge -o ${1}.profdata default.profraw
#todo
# 1 add apply whatever thing I did here
opt -enable-new-pm=0 -o ${1}.optimized.bc -pgo-instr-use -pgo-test-profile-file=${1}.profdata -load ${PATH2LIB} ${PASS}  < ${1}.bc >  /dev/null
