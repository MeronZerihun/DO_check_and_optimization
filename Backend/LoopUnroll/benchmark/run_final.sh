rm -f default.profraw ${1}_prof ${1} ${1}_baseline *.bc ${1}.profdata *_output *.ll

#convert source code to IR
# clang -emit-llvm ${1}.c -S -o ${1}.bc
clang -O0 -Xclang -disable-O0-optnone -emit-llvm ${1}.cpp -S -o ${1}_${2}.bc
#canonicalize natural loops
# opt -enable-new-pm=0  -mem2reg -loop-simplify  ${1}.bc -o ${1}.ls.bc 

# opt -loop-unroll -unroll-count=8  ${1}.bc -o ${1}.unroll.bc 
#new
opt -mem2reg -simplifycfg -loops -lcssa -loop-simplify -loop-rotate -inline -inline-threshold=1000000 ${1}_${2}.bc -o ${1}_${2}.ls.bc > /dev/null
opt  -loop-unroll -unroll-count=${2} -unroll-allow-partial ${1}_${2}.ls.bc -o ${1}_${2}.unroll.bc > /dev/null

llvm-dis ${1}_${2}.unroll.bc -o ${1}_${2}_r.unroll.bc
clang ${1}_${2}.bc -o ${1}_${2}_baseline
# Generate binary executable after unroll: Optimized code
clang ${1}_${2}.unroll.bc -o ${1}_${2}_unroll
./${1}_${2}_unroll > unroll_output


echo -e "1. Performance of unoptimized code"
time ./${1}_${2}_baseline > /dev/null
echo -e "\n\n"
echo -e "2. Performance of optimized code"
time ./${1}_${2}_unroll > /dev/null
echo -e "\n\n"

