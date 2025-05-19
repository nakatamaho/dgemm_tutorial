cd /home/maho/dgemm_tutorial/18 ; \
g++ -fopenmp -O3 -march=native -funroll-loops 18_dgemm_4x12avx2_openmp_kernel.cpp   -o 18_dgemm_4x12avx2_openmp_kernel ; ./18_dgemm_4x12avx2_openmp_kernel  --nocheck

#python3 plot.py \
#  dgemm_benchmark_openblas_results.csv \
#  --out all_kernels_comparison.png

