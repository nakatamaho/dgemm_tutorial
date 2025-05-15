cd /home/maho/dgemm_tutorial/17 ; \
g++ -O3 -march=native -funroll-loops 17_dgemm_4x12avx2_kernel.cpp   -o 17_dgemm_4x12avx2_kernel ; ./17_dgemm_4x12avx2_kernel  --nocheck
g++ -O3 -march=znver2 -mtune=znver2 -ffast-math -fpeel-loops -floop-unroll-and-jam -funroll-all-loops -flto 17_dgemm_4x12avx2_kernel.2.cpp -o 17_dgemm_4x12avx2_kernel.2 ; ./17_dgemm_4x12avx2_kernel.2  --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_4x4avx2_kernel.cpp    -o 17_dgemm_4x4avx2_kernel  ; ./17_dgemm_4x4avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_4x8avx2_kernel.cpp    -o 17_dgemm_4x8avx2_kernel  ; ./17_dgemm_4x8avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_6x8avx2_kernel.cpp    -o 17_dgemm_6x8avx2_kernel  ; ./17_dgemm_6x8avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_8x4avx2_kernel.cpp    -o 17_dgemm_8x4avx2_kernel  ; ./17_dgemm_8x4avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_8x6avx2_kernel.cpp    -o 17_dgemm_8x6avx2_kernel  ; ./17_dgemm_8x6avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_8x8avx2_kernel.cpp    -o 17_dgemm_8x8avx2_kernel  ; ./17_dgemm_8x8avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_12x4avx2_kernel.cpp   -o 17_dgemm_12x4avx2_kernel ; ./17_dgemm_12x4avx2_kernel  --nocheck
g++ -O3 -march=native 17_dgemm_openblas.cpp  -o 17_dgemm_openblas ../07/OpenBLAS-0.3.29/libopenblas_zenp-r0.3.29.a   ; ./17_dgemm_openblas --nocheck

python3 plot.py \
  dgemm_benchmark_12x4avx2_kernel_results.csv \
  dgemm_benchmark_4x12avx2_kernel_results.csv \
  dgemm_benchmark_4x12avx2_kernel_results.2.csv \
  dgemm_benchmark_4x4avx2_kernel_results.csv \
  dgemm_benchmark_4x8avx2_kernel_results.csv \
  dgemm_benchmark_6x8avx2_kernel_results.csv \
  dgemm_benchmark_8x4avx2_kernel_results.csv \
  dgemm_benchmark_8x6avx2_kernel_results.csv \
  dgemm_benchmark_8x8avx2_kernel_results.csv \
  dgemm_benchmark_openblas_results.csv \
  --out all_kernels_comparison.png

