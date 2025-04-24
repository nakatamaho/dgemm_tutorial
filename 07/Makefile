CXX = g++
CXXFLAGS = -std=c++17 -O2 -march=native -fopenmp
LDFLAGS = OpenBLAS-0.3.29/libopenblas.a -lpthread -fopenmp

all: benchmark_openblas benchmark_openblas_small

benchmark_openblas: benchmark_openblas.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

benchmark_openblas_small: benchmark_openblas_small.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f benchmark_openblas benchmark_openblas_small openblas_results.csv
