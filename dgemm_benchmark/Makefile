CXX      := g++
CXXFLAGS := -fopenmp -O2 -Wall -Wextra

TARGETS  := dgemm_benchmark

all: $(TARGETS)

dgemm_benchmark: dgemm_benchmark.cpp dgemm_ref.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ /home/maho/dgemm_tutorial/OpenBLAS-0.3.29/libopenblas_zenp-r0.3.29.a

clean:
	rm -f $(TARGETS)

.PHONY: all clean
