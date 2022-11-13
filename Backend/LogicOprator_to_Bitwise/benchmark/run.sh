PATH2LIB=~/583Project/Backend/LogicOprator_to_Bitwise/build/Pass/ProjectBackend1.so
PASS=-to_bitwisePass



# clean up previous runs
# rm -f default.profraw ${1}_prof ${1} ${1}_baseline *.bc ${1}.profdata *_output *.ll

clang -emit-llvm -c ${1}.c -o ${1}.bc 
# Instrument profiler
opt -enable-new-pm=0 -pgo-instr-gen -instrprof ${1}.bc -o ${1}.prof.bc
# Generate binary executable with profiler embedded
clang -fprofile-instr-generate ${1}.prof.bc -o ${1}.prof
# Collect profiling data
./${1}.prof >correct_output
# Translate raw profiling data into LLVM data format
llvm-profdata merge -o ${1}.profdata default.profraw
#todo
# 1 add apply whatever thing I did here
#eg opt -enable-new-pm=0 -o ${1}.fplicm.bc -pgo-instr-use -pgo-test-profile-file=${1}.profdata -load ${PATH2LIB} ${PASS} < ${1}.ls.bc > /dev/null
opt -enable-new-pm=0 -o ${1}.optimized.bc -pgo-instr-use -pgo-test-profile-file=${1}.profdata -load ${PATH2LIB} ${PASS}  < ${1}.bc >  /dev/null
# 2 check correctness
# Generate binary excutable before FPLICM: Unoptimzied code
clang ${1}.bc -o ${1}_baseline
# # Generate binary executable after FPLICM: Optimized code
clang ${1}.bc -o ${1}_optimized
# # clang ${1}.fplicm.bc -o ${1}_fplicm.bc
# # Produce output from binary to check correctness
./${1}_optimized > optimized_output

echo -e "\n=== Correctness Check ==="
if [ "$(diff correct_output Optimization 1)" != "" ]; then
    echo -e ">> FAIL\n"
else
    echo -e ">> PASS\n"
    # Measure performance
    echo -e "1. Performance of unoptimized code"
    time ./${1}_baseline > /dev/null
    echo -e "\n\n"
    echo -e "2. Performance of optimized code"
    time ./${1}_optimized > /dev/null
    echo -e "\n\n"
fi

# Prepare input to run
setup
# Apply your pass to bitcode (IR)
opt -enable-new-pm=0 -pgo-instr-use -pgo-test-profile-file=pgo.profdata -load ${PATH2LIB} ${PASS} < ${1}.bc > /dev/null
