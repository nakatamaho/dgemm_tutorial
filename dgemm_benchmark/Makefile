CXX      := g++
CXXFLAGS := -fopenmp -O2 -Wall -Wextra

TARGETS  := dgemm_benchmark

all: $(TARGETS)

dgemm_benchmark: dgemm_benchmark.cpp dgemm_ref.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lopenblas

clean:
	rm -f $(TARGETS)

.PHONY: all clean
