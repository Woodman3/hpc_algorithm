CPPFLAGS = -std=c++20 -g -fopenmp
CPPFLAGS += -Ofast
CXX=g++

run: clean main
	yhrun -n 1 -N 1 -c 16 -p thcp3 ./main

p: main
	yhrun -n 1 -N 1 -c 16 -p thcp3 perf record -g -e task-clock ./main

sort:sort.o
	$(CXX) $(CPPFLAGS) -o $@ $^

main:main.o scan.hpp
	$(CXX) $(CPPFLAGS) -o $@ $<

clean:
	rm -f *.o main sort
