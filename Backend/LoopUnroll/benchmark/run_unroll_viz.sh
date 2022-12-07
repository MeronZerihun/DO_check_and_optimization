rm -f default.profraw ${1}_prof ${1} ${1}_baseline *.bc ${1}.profdata *_output *.ll

#convert source code to IR
clang -O0 -Xclang -disable-O0-optnone -emit-llvm ${1}.cpp -S -o ${1}_${2}.bc
#canonicalize natural loops
opt -enable-new-pm=0  -mem2reg -loop-simplify  ${1}_${2}.bc -o ${1}_${2}.ls.bc 
# Instrument profiler
opt -enable-new-pm=0 -pgo-instr-gen -instrprof ${1}_${2}.ls.bc -o ${1}_${2}.ls.prof.bc
clang -fprofile-instr-generate ${1}_${2}.ls.prof.bc -o ${1}_${2}_prof

# Generate profiled data
./${1}_${2}_prof > correct_output

opt -loop-unroll -unroll-count=${2}  ${1}_${2}.bc -o ${1}_${2}.unroll.bc 

llvm-dis ${1}_${2}.unroll.bc -o ${1}_${2}_r.unroll.bc
clang ${1}_${2}.bc -o ${1}_${2}_baseline
# Generate binary executable after unroll: Optimized code
clang ${1}_${2}.unroll.bc -o ${1}_${2}_unroll
./${1}_${2}_unroll > unroll_output
echo -e "\n=== Correctness Check ==="
if [ "$(diff correct_output unroll_output)" != "" ]; then
    echo -e ">> FAIL\n"
else
    echo -e ">> PASS\n"
    # Measure performance
    echo -e "1. Performance of unoptimized code"
    # time ./${1}_${2}_baseline > /dev/null
    x=`./${1}_${2}_baseline
    echo Time: ${x}`
    # echo -e "\n\n"
    echo -e "2. Performance of optimized code"
    y=`./${1}_${2}_unroll`
    echo Time: ${y}
    div=`echo $x / $y | bc -l`
    echo ${2} = $div >> testing.txt
    # echo -e "\n\n"
fi
# Cleanup
# rm -f default.profraw ${1}_${2}_prof ${1}_fplicm ${1}_no_fplicm *.bc ${1}.profdata *_output *.ll

# opt -mem2reg -simplifycfg -loops -lcssa -loop-simplify -loop-rotate -inline -inline-threshold=1000000 ${1}.ll -o LoopUnrollCorrectnessbaseline.bc > /dev/null

