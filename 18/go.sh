cd /home/maho/dgemm_tutorial/18 ; \
g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_4x12avx2_openmp_kernel.cpp     -o 18_dgemm_4x12avx2_openmp_kernel   ; ./18_dgemm_4x12avx2_openmp_kernel    --nocheck
g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_4x12avx2_openmp_kernel.2.cpp   -o 18_dgemm_4x12avx2_openmp_kernel.2 ; ./18_dgemm_4x12avx2_openmp_kernel.2  --nocheck


#python3 plot.py \
#  dgemm_benchmark_openblas_results.csv \
#  --out all_kernels_comparison.png

