CXX      := g++
CXXFLAGS := -O2 -std=c++11 -Wall -Wextra

TARGETS  := dgemm_benchmark dgemm_test

all: $(TARGETS)

dgemm_benchmark: dgemm_benchmark.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

dgemm_test: dgemm_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)

.PHONY: all clean
