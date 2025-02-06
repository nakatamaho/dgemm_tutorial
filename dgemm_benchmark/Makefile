CXX      := g++
CXXFLAGS := -O2 -std=c++11 -Wall -Wextra

TARGETS  := dgemm_benchmark

all: $(TARGETS)

dgemm_benchmark: dgemm_benchmark.cpp dgemm_ref.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lblas

clean:
	rm -f $(TARGETS)

.PHONY: all clean
