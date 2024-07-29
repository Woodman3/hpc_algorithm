CPPFLAGS = -std=c++20 -g -fopenmp
CPPFLAGS += -Ofast
CXX=g++

run: clean main
	yhrun -n 1 -N 1 -c 16 -p thcp3 ./main

p: clean main
	yhrun -n 1 -N 1 -c 16 -p thcp3 perf stat -e l1d_cache,l1d_cache_refill,l2d_cache,l2d_cache_refill ./main

sort:sort.o
	$(CXX) $(CPPFLAGS) -o $@ $^

main:main.o scan.hpp
	$(CXX) $(CPPFLAGS) -o $@ $<

asm:main.cpp
	$(CXX) $(CPPFLAGS) -S -fverbose-asm $< -o main.s

clean:
	rm -f *.o main
