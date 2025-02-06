CXX      := g++
CXXFLAGS := -O2 -std=c++11 -Wall -Wextra

TARGETS  := dgemm_debug_print

all: $(TARGETS)

dgemm_debug_print: dgemm_debug_print.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ -lblas

clean:
	rm -f $(TARGETS)

.PHONY: all clean
