for (( c=5; c<=${1}/2; c+=${2} ))
do
    echo "Welcome $c times"
    bash run_unroll_compact.sh LoopUnrollPerformance $c
    mkdir Unrolled${c}
    mv *"."*".bc" Unrolled${c}/
    mv *".bc" Unrolled${c}/
done


