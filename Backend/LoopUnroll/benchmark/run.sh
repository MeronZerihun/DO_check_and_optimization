bash run_unroll_compact.sh LoopUnrollCorrectness 4
mkdir Unrolled4
mv *"."*".bc" Unrolled4/
mv *".bc" Unrolled8/
bash run_unroll_compact.sh LoopUnrollCorrectness 8
mkdir Unrolled8
mv *"."*".bc" Unrolled8/
mv *".bc" Unrolled8/
bash run_unroll_compact.sh LoopUnrollCorrectness 10
mkdir Unrolled10
mv *"."*".bc" Unrolled10/
mv *".bc" Unrolled10/
bash run_unroll_compact.sh LoopUnrollCorrectness 16
mkdir Unrolled16
mv *"."*".bc" Unrolled16/
mv *".bc" Unrolled16/
bash run_unroll_compact.sh LoopUnrollCorrectness 20
mkdir Unrolled20
mv *"."*".bc" Unrolled20/
mv *".bc" Unrolled20/


