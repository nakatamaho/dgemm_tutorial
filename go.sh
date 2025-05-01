cd /home/maho/dgemm_tutorial/13 ; g++ -march=native -O2 13_dgemm_noavx4x4_kernel.cpp -o 13_dgemm_noavx4x4_kernel ; ./13_dgemm_noavx4x4_kernel --nocheck ; python3 plot.py ; python3 plot.py --comp ../06/dgemm_naive_benchmark_results.csv

cd /home/maho/dgemm_tutorial/14 ; g++ -O2 -march=native 14_dgemm_noavx4x4_copy_trans_kernel.cpp -o 14_dgemm_noavx4x4_copy_trans_kernel ; \
g++ -O2 -march=native 14_dgemm_noavx4x4_copy_kernel.cpp -o 14_dgemm_noavx4x4_copy_kernel ; ./14_dgemm_noavx4x4_copy_trans_kernel --nocheck  ; ./14_dgemm_noavx4x4_copy_kernel --nocheck  ; \
python3 plot.py --main dgemm_benchmark_noavx_copy_trans_kernel_results.csv --comp dgemm_benchmark_noavx_copy_kernel_results.csv ; \
mv dgemm_benchmark_comparison_plot.png dgemm_benchmark_comparison_trans_plot.png ; python3 plot.py --main dgemm_benchmark_noavx_copy_trans_kernel_results.csv --comp ../13/dgemm_benchmark_noavx4x4kernel_results.csv 

cd /home/maho/dgemm_tutorial/15 ; g++ -O2 -march=native 15_dgemm_noavx_block_kernel.cpp -o 15_dgemm_noavx_block_kernel ; ./15_dgemm_noavx_block_kernel --nocheck ; \
python3 plot.py --comp ../14/dgemm_benchmark_noavx_copy_trans_kernel_results.csv

cd /home/maho/dgemm_tutorial/16 ; g++ -O2 -march=native 16_dgemm_avx_block_kernel.cpp -o 16_dgemm_avx_block_kernel;  ./16_dgemm_avx_block_kernel --nocheck ; \
python3 plot.py --comp ../15/dgemm_benchmark_noavx_block_kernel_results.csv
