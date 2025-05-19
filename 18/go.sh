cd /home/maho/dgemm_tutorial/18 ; \
g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_openblas.cpp  ../07/OpenBLAS-0.3.29/libopenblas_zenp-r0.3.29.a -o 18_dgemm_openblas ; ./18_dgemm_openblas                  --nocheck
g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_4x12avx2_NMK_openmp_kernel.cpp     -o 18_dgemm_4x12avx2_NMK_openmp_kernel   ; OMP_NUM_THREADS=32 OMP_PROC_BIND=spread OMP_PLACES=cores ./18_dgemm_4x12avx2_NMK_openmp_kernel --nocheck
g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_4x12avx2_MNK_openmp_kernel.cpp     -o 18_dgemm_4x12avx2_MNK_openmp_kernel   ; OMP_NUM_THREADS=32 OMP_PROC_BIND=spread OMP_PLACES=cores ./18_dgemm_4x12avx2_MNK_openmp_kernel --nocheck

g++ -O3 -march=znver2 -mfma -mavx2 -funroll-loops -fopenmp 18_dgemm_4x12avx2_NKM_openmp_kernel.cpp     -o 18_dgemm_4x12avx2_NKM_openmp_kernel   ; OMP_NUM_THREADS=32 OMP_PROC_BIND=spread OMP_PLACES=cores ./18_dgemm_4x12avx2_NKM_openmp_kernel --nocheck

python3 plot.py dgemm_benchmark_4x12avx2_NMK_openmp_kernel.csv dgemm_benchmark_4x12avx2_NKM_openmp_kernel.csv  dgemm_benchmark_4x12avx2_MNK_openmp_kernel.csv dgemm_benchmark_openblas_results.csv


