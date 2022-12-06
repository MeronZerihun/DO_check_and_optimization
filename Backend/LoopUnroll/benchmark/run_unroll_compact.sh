rm -f default.profraw ${1}_prof ${1} ${1}_baseline *.bc ${1}.profdata *_output *.ll

#convert source code to IR
clang -O0 -Xclang -disable-O0-optnone -emit-llvm ${1}.c -S -o ${1}_${2}.bc

#canonicalize natural loops

opt -mem2reg -simplifycfg -loops -lcssa -loop-simplify -loop-rotate -inline -inline-threshold=1000000 ${1}_${2}.bc -o ${1}_${2}.ls.bc > /dev/null
# Instrument profiler
opt -enable-new-pm=0 -pgo-instr-gen -instrprof ${1}_${2}.ls.bc -o ${1}_${2}.ls.prof.bc
clang -fprofile-instr-generate ${1}_${2}.ls.prof.bc -o ${1}_${2}_prof

# Generate profiled data
./${1}_${2}_prof > correct_output



opt  -loop-unroll -unroll-count=${2} -unroll-allow-partial ${1}_${2}.ls.bc -o ${1}_${2}.unroll.bc > /dev/null

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
    x=`./${1}_${2}_baseline`
    echo Time ${x}
    echo -e "\n\n"
    echo -e "2. Performance of optimized code"
    # time ./${1}_${2}_unroll > /dev/null
    y=`./${1}_${2}_unroll`
    echo Time ${y}
    # X=`(time ./${1}_${2}_unroll > /dev/null) 2>&1 | grep sys`
    div=`echo $x / $y | bc -l`
    echo ${2} = $div >> testing.txt
    echo -e "\n\n"
fi
