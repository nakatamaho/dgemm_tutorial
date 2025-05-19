cd /home/maho/dgemm_tutorial/18 ; \
g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_4x12avx2_openmp_kernel.cpp     -o 18_dgemm_4x12avx2_openmp_kernel   ; ./18_dgemm_4x12avx2_openmp_kernel    --nocheck
g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_openblas.cpp  ../07/OpenBLAS-0.3.29/libopenblas_zenp-r0.3.29.a -o 18_dgemm_openblas ; ./18_dgemm_openblas                  --nocheck


#python3 plot.py \
#  dgemm_benchmark_openblas_results.csv \
#  --out all_kernels_comparison.png

