cd /home/maho/dgemm_tutorial/17 ; \
g++ -O3 -march=native -funroll-loops 17_dgemm_4x12avx2_kernel.cpp   -o 17_dgemm_4x12avx2_kernel ; ./17_dgemm_4x12avx2_kernel  --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_4x4avx2_kernel.cpp    -o 17_dgemm_4x4avx2_kernel  ; ./17_dgemm_4x4avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_4x8avx2_kernel.cpp    -o 17_dgemm_4x8avx2_kernel  ; ./17_dgemm_4x8avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_6x8avx2_kernel.cpp    -o 17_dgemm_6x8avx2_kernel  ; ./17_dgemm_6x8avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_8x4avx2_kernel.cpp    -o 17_dgemm_8x4avx2_kernel  ; ./17_dgemm_8x4avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_8x6avx2_kernel.cpp    -o 17_dgemm_8x6avx2_kernel  ; ./17_dgemm_8x6avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_8x8avx2_kernel.cpp    -o 17_dgemm_8x8avx2_kernel  ; ./17_dgemm_8x8avx2_kernel   --nocheck
g++ -O3 -march=native -funroll-loops 17_dgemm_12x4avx2_kernel.cpp   -o 17_dgemm_12x4avx2_kernel ; ./17_dgemm_12x4avx2_kernel  --nocheck

python3 plot.py \
  dgemm_benchmark_12x4avx2_kernel_results.csv \
  dgemm_benchmark_4x12avx2_kernel_results.csv \
  dgemm_benchmark_4x4avx2_kernel_results.csv \
  dgemm_benchmark_4x8avx2_kernel_results.csv \
  dgemm_benchmark_6x8avx2_kernel_results.csv \
  dgemm_benchmark_8x4avx2_kernel_results.csv \
  dgemm_benchmark_8x6avx2_kernel_results.csv \
  dgemm_benchmark_8x8avx2_kernel_results.csv \
  --out all_kernels_comparison.png

