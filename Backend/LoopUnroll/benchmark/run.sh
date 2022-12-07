for (( c=${3}; c<=${2}; c+=${3} ))
do
    echo "Welcome $c times"
    bash run_unroll_compact.sh ${1} $c
    mkdir Unrolled${c}
    mv *"."*".bc" Unrolled${c}/
    mv *".bc" Unrolled${c}/
    mv *"_"*"_unroll" Unrolled${c}/
    mv *"_"*"_baseline" Unrolled${c}/
done


mv testing.txt ../Pass/