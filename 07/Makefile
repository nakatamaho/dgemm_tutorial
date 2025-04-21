CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native -fopenmp
LDFLAGS = -lopenblas -lpthread -fopenmp

all: benchmark_openblas

benchmark_openblas: benchmark_openblas.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f benchmark_openblas openblas_results.csv
